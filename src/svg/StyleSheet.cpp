/*
 * Copyright 2026 Pie Laboratories
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "svg/StyleSheet.hpp"

#include <algorithm>
#include <cctype>
#include <functional>
#include <limits>
#include <tuple>

namespace Draw2d::Svg {

    namespace {

        bool IsIdentifierCharacter(char c)
        {
            return std::isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_' || static_cast<unsigned char>(c) >= 0x80;
        }

        Core::String Trimmed(const std::string &str)
        {
            size_t nStart = 0, nEnd = str.size();
            while(nStart < nEnd && std::isspace(static_cast<unsigned char>(str[nStart]))) nStart++;
            while(nEnd > nStart && std::isspace(static_cast<unsigned char>(str[nEnd - 1]))) nEnd--;
            return str.substr(nStart, nEnd - nStart);
        }

        Core::String Lowered(Core::String str)
        {
            std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
            return str;
        }

        // CSS comments out, and the HTML-era <!-- --> markers some tools still wrap styles in
        std::string WithoutComments(const std::string &strCss)
        {
            std::string strResult;
            strResult.reserve(strCss.size());
            for(size_t n = 0; n < strCss.size(); n++) {
                if(strCss.compare(n, 2, "/*") == 0) {
                    size_t nEnd = strCss.find("*/", n + 2);
                    if(nEnd == std::string::npos) break;
                    n = nEnd + 1;
                    continue;
                }
                if(strCss.compare(n, 4, "<!--") == 0) { n += 3; continue; }
                if(strCss.compare(n, 3, "-->") == 0) { n += 2; continue; }
                strResult += strCss[n];
            }
            return strResult;
        }

        // index of the '}' closing the block whose '{' is at nOpen, or npos
        size_t ClosingBrace(const std::string &strCss, size_t nOpen)
        {
            int nDepth = 0;
            char cQuote = 0;
            for(size_t n = nOpen; n < strCss.size(); n++) {
                const char c = strCss[n];
                if(cQuote) {
                    if(c == '\\') n++;
                    else if(c == cQuote) cQuote = 0;
                    continue;
                }
                if(c == '"' || c == '\'') cQuote = c;
                else if(c == '{') nDepth++;
                else if(c == '}' && --nDepth == 0) return n;
            }
            return std::string::npos;
        }

    } // namespace

    void StyleSheet::append(const Core::String &cstrCss)
    {
        const std::string strCss = WithoutComments(cstrCss);
        size_t nPos = 0;
        while(nPos < strCss.size()) {
            while(nPos < strCss.size() && std::isspace(static_cast<unsigned char>(strCss[nPos]))) nPos++;
            if(nPos >= strCss.size()) break;

            if(strCss[nPos] == '@') {
                // @-rules are skipped: either "@import ...;" or "@media ... { ... }"
                const size_t nSemicolon = strCss.find(';', nPos);
                const size_t nBrace = strCss.find('{', nPos);
                if(nBrace != std::string::npos && (nSemicolon == std::string::npos || nBrace < nSemicolon)) {
                    const size_t nClose = ClosingBrace(strCss, nBrace);
                    if(nClose == std::string::npos) break;
                    nPos = nClose + 1;
                }
                else {
                    if(nSemicolon == std::string::npos) break;
                    nPos = nSemicolon + 1;
                }
                continue;
            }

            const size_t nOpen = strCss.find('{', nPos);
            if(nOpen == std::string::npos) break;   // trailing junk without a block
            const size_t nClose = ClosingBrace(strCss, nOpen);
            if(nClose == std::string::npos) break;  // unterminated block: drop it, as browsers do
            __addRule(strCss.substr(nPos, nOpen - nPos), ParseDeclarations(strCss.substr(nOpen + 1, nClose - nOpen - 1)));
            nPos = nClose + 1;
        }
    }

    void StyleSheet::__addRule(const Core::String &cstrSelectors, const std::vector<CssDeclaration> &declarations)
    {
        if(declarations.empty()) return;
        size_t nStart = 0;
        while(nStart <= cstrSelectors.size()) {
            size_t nComma = cstrSelectors.find(',', nStart);
            if(nComma == std::string::npos) nComma = cstrSelectors.size();
            const Core::String strSelector = Trimmed(cstrSelectors.substr(nStart, nComma - nStart));
            nStart = nComma + 1;
            if(strSelector.empty()) continue;

            Rule rule {};
            if(!__ParseCompoundSelector(strSelector, rule.selector)) {
                m_nSkippedSelectors++;
                continue;
            }
            // (ids, classes, types), packed so a plain comparison orders them
            rule.nSpecificity = (rule.selector.strId.empty() ? 0 : 10000)
                              + static_cast<unsigned>(rule.selector.classes.size()) * 100
                              + (rule.selector.strTag.empty() ? 0 : 1);
            rule.nOrder = m_rules.size();
            rule.declarations = declarations;

            const size_t nIndex = m_rules.size();
            if(!rule.selector.strId.empty()) m_rulesById[rule.selector.strId].push_back(nIndex);
            else if(!rule.selector.classes.empty()) m_rulesByClass[rule.selector.classes.front()].push_back(nIndex);
            else if(!rule.selector.strTag.empty()) m_rulesByTag[rule.selector.strTag].push_back(nIndex);
            else m_universalRules.push_back(nIndex);
            m_rules.push_back(std::move(rule));
        }
    }

    bool StyleSheet::__ParseCompoundSelector(const Core::String &cstrSelector, CompoundSelector &selector)
    {
        size_t n = 0;
        if(cstrSelector[0] == '*') n = 1;
        else if(IsIdentifierCharacter(cstrSelector[0]) && !std::isdigit(static_cast<unsigned char>(cstrSelector[0]))) {
            while(n < cstrSelector.size() && IsIdentifierCharacter(cstrSelector[n])) n++;
            selector.strTag = cstrSelector.substr(0, n);
        }
        while(n < cstrSelector.size()) {
            const char cKind = cstrSelector[n];
            if(cKind != '.' && cKind != '#') return false;   // combinator, pseudo-class, [attribute]...
            const size_t nStart = ++n;
            while(n < cstrSelector.size() && IsIdentifierCharacter(cstrSelector[n])) n++;
            if(n == nStart) return false;
            const Core::String strName = cstrSelector.substr(nStart, n - nStart);
            if(cKind == '.') selector.classes.push_back(strName);
            else if(selector.strId.empty()) selector.strId = strName;
            else if(selector.strId != strName) return false;  // #a#b never matches
        }
        return true;
    }

    bool StyleSheet::__Matches(const CompoundSelector &selector, const Core::String &cstrTag, const Core::String &cstrId, const std::vector<Core::String> &classes)
    {
        if(!selector.strTag.empty() && selector.strTag != cstrTag) return false;
        if(!selector.strId.empty() && selector.strId != cstrId) return false;
        for(const Core::String &cstrClass : selector.classes) {
            if(std::find(classes.begin(), classes.end(), cstrClass) == classes.end()) return false;
        }
        return true;
    }

    std::unordered_map<Core::String, Core::String> StyleSheet::cascade(const Core::String &cstrTag,
                                                                       const Core::String &cstrId,
                                                                       const std::vector<Core::String> &classes,
                                                                       const std::vector<CssDeclaration> &inlineDeclarations) const
    {
        // every applicable declaration with its rank: (important, specificity, rule order,
        //  position in the rule).  The style attribute ranks above every rule.
        struct Candidate {
            bool bImportant;
            unsigned nSpecificity;
            size_t nOrder;
            size_t nPosition;
            const CssDeclaration *cpDeclaration;
        };
        std::vector<Candidate> candidates {};

        auto consider = [&](const std::vector<size_t> &ruleIndices) {
            for(size_t nIndex : ruleIndices) {
                const Rule &rule = m_rules[nIndex];
                if(!__Matches(rule.selector, cstrTag, cstrId, classes)) continue;
                for(size_t nPosition = 0; nPosition < rule.declarations.size(); nPosition++) {
                    candidates.push_back({ rule.declarations[nPosition].bImportant, rule.nSpecificity, rule.nOrder, nPosition, &rule.declarations[nPosition] });
                }
            }
        };
        if(!cstrId.empty()) {
            if(auto it = m_rulesById.find(cstrId); it != m_rulesById.end()) consider(it->second);
        }
        for(const Core::String &cstrClass : classes) {
            if(auto it = m_rulesByClass.find(cstrClass); it != m_rulesByClass.end()) consider(it->second);
        }
        if(auto it = m_rulesByTag.find(cstrTag); it != m_rulesByTag.end()) consider(it->second);
        consider(m_universalRules);

        // a rule filed under its first class is reached once per matching class name
        //  (class="a a"): keep one candidate per declaration
        std::sort(candidates.begin(), candidates.end(), [](const Candidate &a, const Candidate &b) { return std::less<const CssDeclaration *>()(a.cpDeclaration, b.cpDeclaration); });
        candidates.erase(std::unique(candidates.begin(), candidates.end(), [](const Candidate &a, const Candidate &b) { return a.cpDeclaration == b.cpDeclaration; }), candidates.end());

        for(size_t nPosition = 0; nPosition < inlineDeclarations.size(); nPosition++) {
            candidates.push_back({ inlineDeclarations[nPosition].bImportant, std::numeric_limits<unsigned>::max(), 0, nPosition, &inlineDeclarations[nPosition] });
        }

        std::sort(candidates.begin(), candidates.end(), [](const Candidate &a, const Candidate &b) {
            return std::tie(a.bImportant, a.nSpecificity, a.nOrder, a.nPosition) < std::tie(b.bImportant, b.nSpecificity, b.nOrder, b.nPosition);
        });

        std::unordered_map<Core::String, Core::String> values {};
        for(const Candidate &candidate : candidates) values[candidate.cpDeclaration->property] = candidate.cpDeclaration->value;
        return values;
    }

    std::vector<CssDeclaration> StyleSheet::ParseDeclarations(const Core::String &cstrDeclarations)
    {
        std::vector<CssDeclaration> declarations {};
        auto addDeclaration = [&declarations](const std::string &strDeclaration) {
            const size_t nColon = strDeclaration.find(':');
            if(nColon == std::string::npos) return;
            CssDeclaration declaration {};
            declaration.property = Lowered(Trimmed(strDeclaration.substr(0, nColon)));
            Core::String strValue = Trimmed(strDeclaration.substr(nColon + 1));
            // "!important", possibly "! important"
            const size_t nBang = strValue.rfind('!');
            if(nBang != std::string::npos && Lowered(Trimmed(strValue.substr(nBang + 1))) == "important") {
                declaration.bImportant = true;
                strValue = Trimmed(strValue.substr(0, nBang));
            }
            declaration.value = strValue;
            if(declaration.property.empty() || declaration.value.empty()) return;
            declarations.push_back(std::move(declaration));
        };

        int nParentheses = 0;
        char cQuote = 0;
        size_t nStart = 0;
        for(size_t n = 0; n < cstrDeclarations.size(); n++) {
            const char c = cstrDeclarations[n];
            if(cQuote) {
                if(c == '\\') n++;
                else if(c == cQuote) cQuote = 0;
                continue;
            }
            if(c == '"' || c == '\'') cQuote = c;
            else if(c == '(') nParentheses++;
            else if(c == ')' && nParentheses > 0) nParentheses--;
            else if(c == ';' && nParentheses == 0) {
                addDeclaration(cstrDeclarations.substr(nStart, n - nStart));
                nStart = n + 1;
            }
        }
        addDeclaration(cstrDeclarations.substr(nStart));
        return declarations;
    }

} // namespace Draw2d::Svg

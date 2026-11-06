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

#ifndef DRAW2D_SVG_STYLESHEET_DOT_HPP
#define DRAW2D_SVG_STYLESHEET_DOT_HPP

#include <unordered_map>
#include <vector>

#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif

namespace Draw2d::Svg {

    struct CssDeclaration {
        Core::String property;
        Core::String value;
        bool bImportant = false;
    };

    /// The CSS an SVG needs from its <style> elements: rules whose selectors are a single
    ///  compound selector -- a type, #id, .class or *, combined (path.cls-1, g#logo.dark) --
    ///  cascaded by !important, specificity and source order.  Selectors with combinators,
    ///  pseudo-classes or attribute tests are skipped (counted in getSkippedSelectorCount()),
    ///  as are @-rules.
    class StyleSheet
    {
    public:
        /// adds the rules in cstrCss after those already there (later rules win ties)
        void append(const Core::String &cstrCss);

        /// the cascaded value of each property set for an element by the stylesheet and its
        ///  style attribute (inlineDeclarations): normal rules < inline < !important rules <
        ///  !important inline.  Presentation attributes rank below all of these; merging them
        ///  is up to the caller.
        std::unordered_map<Core::String, Core::String> cascade(const Core::String &cstrTag,
                                                               const Core::String &cstrId,
                                                               const std::vector<Core::String> &classes,
                                                               const std::vector<CssDeclaration> &inlineDeclarations = {}) const;

        bool isEmpty() const { return m_rules.empty(); }
        size_t getRuleCount() const { return m_rules.size(); }
        size_t getSkippedSelectorCount() const { return m_nSkippedSelectors; }

        /// "fill: red; stroke: blue !important" -> declarations, in order; properties are
        ///  lower-cased, values trimmed.  Semicolons inside quotes or parentheses don't split.
        static std::vector<CssDeclaration> ParseDeclarations(const Core::String &cstrDeclarations);

    private:
        struct CompoundSelector {
            Core::String strTag {};       // empty: any element
            Core::String strId {};
            std::vector<Core::String> classes {};
        };

        struct Rule {
            CompoundSelector selector;
            unsigned nSpecificity = 0;
            size_t nOrder = 0;
            std::vector<CssDeclaration> declarations {};
        };

        std::vector<Rule> m_rules {};
        // each rule is filed once, under its most selective part: id, else first class, else tag
        std::unordered_map<Core::String, std::vector<size_t>> m_rulesById {};
        std::unordered_map<Core::String, std::vector<size_t>> m_rulesByClass {};
        std::unordered_map<Core::String, std::vector<size_t>> m_rulesByTag {};
        std::vector<size_t> m_universalRules {};
        size_t m_nSkippedSelectors = 0;

        void __addRule(const Core::String &cstrSelectors, const std::vector<CssDeclaration> &declarations);
        static bool __ParseCompoundSelector(const Core::String &cstrSelector, CompoundSelector &selector);
        static bool __Matches(const CompoundSelector &selector, const Core::String &cstrTag, const Core::String &cstrId, const std::vector<Core::String> &classes);
    };

} // namespace Draw2d::Svg

#endif  /* DRAW2D_SVG_STYLESHEET_DOT_HPP */

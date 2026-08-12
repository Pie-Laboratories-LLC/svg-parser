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

#ifndef CORE_STRING_DOT_HPP
#define CORE_STRING_DOT_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <format>
#include <regex>

namespace Core {
    class String : public std::string {
    public:
        String() = default;
        String(const String &copy) = default;
        String &operator=(const String &copy) = default;
        String(String &&move) = default;
        virtual ~String() = default;

        String(const char *cpszString): std::string(cpszString) {}
        String(std::string::const_iterator begin, std::string::const_iterator end): std::string(begin,end) {}
        String(const std::string &cstrString): std::string(cstrString) {}
        String(int nValue);
        String(double dValue);
        String(unsigned nValue);
        template<typename... Args>
        String(std::format_string<Args...> fmt, Args&&... args)
            : String(std::format(fmt, std::forward<Args>(args)...)) {}

        // NOTE: claude says it's appropriate to std::move() inPlace here, but g++ insists it prevents copy elision.
        //  prolly never use another compiler, but worth noting.
        String trim(char cTrimCharacter) const
            { String inPlace(*this); inPlace.trim_in_place(cTrimCharacter); return inPlace; }
        String trim(const char *cpszTrimCharacters) const
            { String inPlace(*this); inPlace.trim_in_place(cpszTrimCharacters); return inPlace; }
        String trim(const String &cstrTrimCharacters = " \t\n\r\f\v") const
            { String inPlace(*this); inPlace.trim_in_place(cstrTrimCharacters.c_str()); return inPlace; }
        String trim_start(char cTrimCharacter) const
            { String inPlace(*this); inPlace.trim_start_in_place(cTrimCharacter); return inPlace; }
        String trim_start(const char *cpszTrimCharacters) const
            { String inPlace(*this); inPlace.trim_start_in_place(cpszTrimCharacters); return inPlace; }
        String trim_start(const String &cstrTrimCharacters = " \t\n\r\f\v") const
            { return trim_start(cstrTrimCharacters.c_str()); }
        String trim_end(char cTrimCharacter) const
            { String inPlace(*this); inPlace.trim_end_in_place(cTrimCharacter); return inPlace; }
        String trim_end(const char *cpszTrimCharacters) const
            { String inPlace(*this); inPlace.trim_end_in_place(cpszTrimCharacters); return inPlace; }
        String trim_end(const String &cstrTrimCharacters = " \t\n\r\f\v") const
            { String inPlace(*this); inPlace.trim_end_in_place(cstrTrimCharacters.c_str()); return inPlace; }

        void trim_in_place(char cTrimCharacter);
        void trim_in_place(const char *cpszTrimCharacters);
        void trim_in_place(const String &cstrTrimCharacters = " \t\n\r\f\v") { trim_in_place(cstrTrimCharacters.c_str()); }
        void trim_start_in_place(char cTrimCharacter);
        void trim_start_in_place(const char *cpszTrimCharacters);
        void trim_start_in_place(const String &cstrTrimCharacters = " \t\n\r\f\v") { trim_start_in_place(cstrTrimCharacters.c_str()); }
        void trim_end_in_place(char cTrimCharacter);
        void trim_end_in_place(const char *cpszTrimCharacters);
        void trim_end_in_place(const String &cstrTrimCharacters = " \t\n\r\f\v") { trim_end_in_place(cstrTrimCharacters.c_str()); }

        String to_lower() const;
        String to_upper() const;

        template<typename T>
        requires std::ranges::range<T>
        static String Join(const T& elems, const String &delimiter) {
            String strResult;
            bool first = true;
            for (auto elem : elems) {
                if(!first) strResult += delimiter;
                first = false;
                strResult += elem;
            }
            return strResult;
        }

        std::vector<Core::String> regex_split(const Core::String &cstrRegex, std::regex::flag_type flags = std::regex_constants::ECMAScript, bool bPreserveEmpties = false) const {
            return regex_split(std::regex(cstrRegex.c_str(), flags), bPreserveEmpties);
        }
        std::vector<Core::String> regex_split(const std::regex &regex, bool bPreserveEmpties = false) const {
            // Create token iterators
            // The -1 argument specifies that we want the text BETWEEN the matches
            std::sregex_token_iterator pos(begin(), end(), regex, -1);
            std::sregex_token_iterator end; // Default constructor acts as the past-the-end iterator
            
            // Store tokens into a vector
            std::vector<std::string> tokens(pos, end);
            if(!bPreserveEmpties) std::erase_if(tokens,[](std::string &value) { return !value.length(); });

            std::vector<Core::String> results(tokens.begin(),tokens.end());
            return results;
        }

        std::vector<Core::String> split(char delimiter,
                                        bool bSkipEmpty = true,
                                        std::optional<size_t> maxSplits = std::nullopt);

        static bool IsEmpty(const String &strString) {
            return strString.size() == 0;
        }
        static bool IsWhitespace(const String &strString) {
            return strString.size() == 0 || std::all_of(strString.begin(), strString.end(), [](unsigned char c) {
                return std::isspace(c);
            });
        }

        static inline std::regex WsRegex { R"xxx(\s+)xxx" }; 
            

    };
}

namespace std {
    template <>
    struct hash<Core::String> {
        // thanks claude :P
        size_t operator()(const Core::String& s) const noexcept {
            return std::hash<std::string_view>{}(std::string_view(s.c_str()));
        }
    };
}

template<>
struct std::formatter<Core::String> : std::formatter<std::string_view> {
    auto format(const Core::String& s, std::format_context& ctx) const {
        return std::formatter<std::string_view>::format(
            std::string_view(s.c_str(), s.length()), ctx);
    }
};

#endif  /* CORE_STRING_DOT_HPP */

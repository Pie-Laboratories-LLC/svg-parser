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

#include "core/String.hpp"

namespace Core {

    String::String(int nValue)
    {
        char pszBuffer[20];
        sprintf(pszBuffer, "%d", nValue);
        *this = pszBuffer;
    }

    String::String(double dValue)
    {
        char pszBuffer[20];
        sprintf(pszBuffer, "%f", dValue);
        *this = pszBuffer;
    }

    String::String(unsigned nValue)
    {
        char pszBuffer[20];
        sprintf(pszBuffer, "%u", nValue);
        *this = pszBuffer;
    }

    void String::trim_in_place(char cTrimCharacter)

    {
        // claude <3 -- unavoidable ....  I wrote the lambda :P
        auto firstNonCharacter = [cTrimCharacter](char c) { return (c != cTrimCharacter); };

        auto start = std::find_if(begin(), end(), firstNonCharacter);
        auto end = std::find_if(rbegin(), rend(), firstNonCharacter).base();

        if (start < end) *this = String(start, end);
        else clear();
    }

    void String::trim_in_place(const char *cpszTrimCharacters)

    {
        auto start = find_first_not_of(cpszTrimCharacters);
        auto end = find_last_not_of(cpszTrimCharacters);

        if(start == npos) start = 0;
        if(end == npos) end = length() - 1;
        if(start <= end) *this = substr(start, (end - start) + 1);
        else clear();
    }

    void String::trim_start_in_place(char cTrimCharacter)

    {
        auto firstNonCharacter = [cTrimCharacter](char c) { return (c != cTrimCharacter); };

        auto start = std::find_if(begin(), end(), firstNonCharacter);
        auto end_ = end();

        if (start < end_) *this = String(start, end_);
        else clear(); 
    }

    void String::trim_start_in_place(const char *cpszTrimCharacters)

    {
        auto start = find_first_not_of(cpszTrimCharacters);

        if(start != std::string::npos) *this = substr(start);
        else clear();
    }

    void String::trim_end_in_place(char cTrimCharacter)

    {
        auto firstNonCharacter = [cTrimCharacter](char c) { return (c != cTrimCharacter); };

        auto start = begin();
        auto end = std::find_if(rbegin(), rend(), firstNonCharacter).base();

        if (start < end) *this = String(start, end);
        else clear(); 
    }

    void String::trim_end_in_place(const char *cpszTrimCharacters)

    {
        auto end = find_last_not_of(cpszTrimCharacters);

        if(end != std::string::npos) *this = substr(0, end);
        else clear();
    }

    String String::to_lower() const
    {
        String toReturn {};
        for(auto c : *this) toReturn += std::tolower(c);
        return toReturn;
    }

    String String::to_upper() const
    {
        String toReturn {};
        for(auto c : *this) toReturn += std::toupper(c);
        return toReturn;
    }

    std::vector<Core::String> String::split(char delimiter,
                                            bool bSkipEmpty,
                                            std::optional<size_t> maxSplits)
    {
        std::vector<Core::String> result;
        std::string_view sv(c_str());
        size_t start = 0;
    
        while (true) {
            if (maxSplits.has_value() && result.size() >= maxSplits.value()) {
                // remaining text becomes the final piece, unsplit
                Core::String s(std::string(sv.substr(start)));
                if (!bSkipEmpty || !IsWhitespace(s)) result.push_back(std::move(s));
                break;
            }
    
            size_t pos = sv.find(delimiter, start);
            std::string_view piece = (pos == std::string_view::npos)
                ? sv.substr(start)
                : sv.substr(start, pos - start);
    
            Core::String s { std::string(piece) };
            if (!bSkipEmpty || !IsWhitespace(s)) result.push_back(std::move(s));
    
            if (pos == std::string_view::npos) break;
            start = pos + 1;
        }
    
        return result;
    }
}

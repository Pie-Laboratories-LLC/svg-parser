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

#include "core/StdNumeric.hpp"

#include <charconv>

#ifndef CORE_EXCEPTION_DOT_HPP
    #include "core/Exception.hpp"
#endif

namespace Core {

    // le plus long nombre au début de [offset, offset + size): sa valeur, et
    // combien de caractères il prend. Le reste est à l'appelant -- l'unité
    // d'une longueur, par exemple: "1e-5em" -> (1e-5, 4), "1em" -> (1, 1).
    // Un + devant est permis, comme en SVG, bien que from_chars le refuse
    std::pair<float, unsigned> ParseFloatPrefix(const String &cstrFloat, unsigned offset, unsigned size)
    {
        if (offset > cstrFloat.length())
            throw Exception("Offset {} past the end of: {}", offset, cstrFloat.c_str());
        size = std::min<unsigned>(size, static_cast<unsigned>(cstrFloat.length()) - offset);

        const char *début = cstrFloat.c_str() + offset;
        const char *fin = début + size;
        const char *p = début;
        if (p != fin && *p == '+') {
            ++p;
            // "+-5" n'est pas un nombre
            if (p != fin && *p == '-') throw Exception("Not a valid float: {}", cstrFloat.substr(offset, size).c_str());
        }

        float fResult;
        auto [ptr, ec] = std::from_chars(p, fin, fResult);

        if (ec == std::errc::invalid_argument)
            throw Exception("Not a valid float: {}", cstrFloat.substr(offset, size).c_str());
        if (ec == std::errc::result_out_of_range)
            throw Exception("Value out of range: {}", cstrFloat.substr(offset, size).c_str());
        if (ec != std::errc())
            throw Exception("Internal error, float didn't parse correctly: {}", cstrFloat.substr(offset, size).c_str());

        return { fResult, static_cast<unsigned>(ptr - début) };
    }

    float ParseFloat(const String &cstrFloat, unsigned offset, unsigned size)
    {
        if (offset <= cstrFloat.length())
            size = std::min<unsigned>(size, static_cast<unsigned>(cstrFloat.length()) - offset);

        const auto [fResult, nConsommés] = ParseFloatPrefix(cstrFloat, offset, size);
        if (nConsommés != size)
            throw Exception("Extra characters at end: {}", cstrFloat.substr(offset, size).c_str());
        return fResult;
    }

    int ParseInteger(const String &cstrInteger,unsigned base,unsigned offset,unsigned size) {
        if(size == std::numeric_limits<unsigned>::max()) size = cstrInteger.length();

        int nResult;

        auto [ptr, ec] = std::from_chars(cstrInteger.c_str() + offset, cstrInteger.c_str() + offset + size, nResult, base);

        if (ec == std::errc()) {
            if(cstrInteger.c_str() + offset + size != ptr) throw Exception("Extra characters at end: {}", cstrInteger.substr(offset, size).c_str());
        } else if (ec == std::errc::invalid_argument) {
            throw Exception("Not a valid float: {}",cstrInteger.substr(offset,size).c_str());
        } else if (ec == std::errc::result_out_of_range) {
            throw Exception("Value out of range: {}", cstrInteger.substr(offset,size).c_str());
        }
        else throw Exception("Internal error, integer didn't parse correctly: {}", cstrInteger.substr(offset,size).c_str());

        return nResult;
    }

    int ParseUnsigned(const String &cstrUnsigned,unsigned base,unsigned offset,unsigned size) {
        if(size == std::numeric_limits<unsigned>::max()) size = cstrUnsigned.length();

        unsigned nResult;

        auto [ptr, ec] = std::from_chars(cstrUnsigned.c_str() + offset, cstrUnsigned.c_str() + offset + size, nResult, base);

        if (ec == std::errc()) {
            if(cstrUnsigned.c_str() + offset + size != ptr) throw Exception("Extra characters at end: {}", cstrUnsigned.substr(offset, size).c_str());
        } else if (ec == std::errc::invalid_argument) {
            throw Exception("Not a valid float: {}",cstrUnsigned.substr(offset,size).c_str());
        } else if (ec == std::errc::result_out_of_range) {
            throw Exception("Value out of range: {}", cstrUnsigned.substr(offset,size).c_str());
        }
        else throw Exception("Internal error, unsigned didn't parse correctly: {}", cstrUnsigned.substr(offset,size).c_str());

        return nResult;
    }

} // namespace Core

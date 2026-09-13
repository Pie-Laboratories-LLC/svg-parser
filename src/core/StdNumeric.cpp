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

#ifndef CORE_EXCEPTION_DOT_HPP
    #include "core/Exception.hpp"
#endif

namespace Core {

    float ParseFloat(const String &cstrFloat,unsigned offset,unsigned size) {
        if(size == std::numeric_limits<unsigned>::max()) size = cstrFloat.length();

        float fResult;

        auto [ptr, ec] = std::from_chars(cstrFloat.c_str() + offset, cstrFloat.c_str() + size + offset, fResult);

        if (ec == std::errc()) {
            if(cstrFloat.c_str() + offset + size != ptr) throw Exception("Extra characters at end: {}", cstrFloat.substr(offset, size).c_str());
        }
        else if (ec == std::errc::invalid_argument) {
            throw Exception("Not a valid float: {}", cstrFloat.substr(offset, size).c_str());
        }
        else if (ec == std::errc::result_out_of_range) {
            throw Exception("Value out of range: {}", cstrFloat.substr(offset, size).c_str());
        }
        else throw Exception("Internal error, float didn't parse correctly: {}", cstrFloat.substr(offset, size).c_str());

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

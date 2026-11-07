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

#include "openssl/evp.h"
#include "core/Base64.hpp"

#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif
#ifndef CORE_EXCEPTION_DOT_HPP
    #include "core/Exception.hpp"
#endif

namespace Core {

    std::vector<uint8_t> base64Decode(const Core::String &input)
    {
        // EVP_DecodeBlock requires input length to be a multiple of 4.
        if (input.length() % 4 != 0)
            throw Exception("Invalid base64 data: length {} is not a multiple of 4", input.length());

        std::vector<uint8_t> output(input.length() / 4 * 3);

        int decodedLength;
        try {
            decodedLength = EVP_DecodeBlock(
                output.data(),
                reinterpret_cast<const unsigned char *>(input.data()),
                static_cast<int>(input.size()));
        }
        catch(const std::exception &e) {
            throw Exception(e.what());
        }

        if (decodedLength < 0)
            throw Exception("Invalid base64 data: EVP_DecodeBlock failed");

        // EVP_DecodeBlock doesn't account for '=' padding -- it always decodes
        // as if the input were unpadded, so the output includes trailing zero
        // byte(s) that need trimming based on how many '=' characters were present.
        size_t padding = 0;
        if (input.size() >= 1 && input[input.size() - 1] == '=') padding++;
        if (input.size() >= 2 && input[input.size() - 2] == '=') padding++;

        output.resize(static_cast<size_t>(decodedLength) - padding);
        return output;
    }

}

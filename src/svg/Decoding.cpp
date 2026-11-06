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

#include "svg/Decoding.hpp"
#ifndef DRAW2D_SVG_SVGEXCEPTION_DOT_HPP
    #include "svg/SvgException.hpp"
#endif
#define DECODING_NOTADECODING "NotADecoding"
#define DECODING_ASYNC "async"
#define DECODING_SYNC "sync"
namespace Draw2d::Svg {
    Decoding ParseDecoding(const Core::String &cstrDecoding)
    {
        if (cstrDecoding == DECODING_NOTADECODING) return Decoding::NotADecoding;
        if (cstrDecoding == DECODING_ASYNC) return Decoding::Async;
        if (cstrDecoding == DECODING_SYNC) return Decoding::Sync;
        throw SvgException("Unrecognized decoding {}", cstrDecoding.c_str());
    }
    Core::String DecodingToString(Decoding enumDecoding)
    {
        switch(enumDecoding) {
        case Decoding::NotADecoding: return "NotADecoding";
        case Decoding::Async: return "Async";
        case Decoding::Sync: return "Sync";
        default: throw SvgException("Invalid illegal unknown unrecognized unhandled garbage Decoding {}", static_cast<int32_t>(enumDecoding));
        }
    }
}

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

#include "svg/FetchPriority.hpp"
#ifndef DRAW2D_SVG_SVGEXCEPTION_DOT_HPP
    #include "svg/SvgException.hpp"
#endif
#define FETCHPRIORITY_NOTAFETCHPRIORITY "NotAFetchPriority"
#define FETCHPRIORITY_HIGH "high"
#define FETCHPRIORITY_LOW "low"
namespace Draw2d::Svg {
    FetchPriority ParseFetchPriority(const Core::String &cstrFetchPriority)
    {
        if (cstrFetchPriority == FETCHPRIORITY_NOTAFETCHPRIORITY) return FetchPriority::NotAFetchPriority;
        if (cstrFetchPriority == FETCHPRIORITY_HIGH) return FetchPriority::High;
        if (cstrFetchPriority == FETCHPRIORITY_LOW) return FetchPriority::Low;
        throw SvgException("Unrecognized fetch priority {}", cstrFetchPriority.c_str());
    }
    Core::String FetchPriorityToString(FetchPriority enumFetchPriority)
    {
        switch(enumFetchPriority) {
        case FetchPriority::NotAFetchPriority: return "NotAFetchPriority";
        case FetchPriority::High: return "High";
        case FetchPriority::Low: return "Low";
        default: throw SvgException("Invalid illegal unknown unrecognized unhandled garbage FetchPriority {}", static_cast<int32_t>(enumFetchPriority));
        }
    }
}

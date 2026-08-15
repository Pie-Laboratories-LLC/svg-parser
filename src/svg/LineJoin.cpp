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

#include "svg/LineJoin.hpp"

#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif
#include "svg/SvgException.hpp"

namespace Draw2d::Svg {

#define NOTALINEJOIN "NotALineJoin"
#define ARCS         "arcs"
#define BEVEL        "bevel"
#define MITER        "miter"
#define MITERCLIP    "miterClip"
#define ROUND        "round"

    LineJoin ParseLineJoin(const Core::String &cstrLineJoin)
    {
        if(cstrLineJoin == NOTALINEJOIN) return LineJoin::NotALineJoin;
        if(cstrLineJoin == ARCS) return LineJoin::Arcs;
        if(cstrLineJoin == BEVEL) return LineJoin::Bevel;
        if(cstrLineJoin == MITER) return LineJoin::Miter;
        if(cstrLineJoin == MITERCLIP) return LineJoin::MiterClip;
        if(cstrLineJoin == ROUND) return LineJoin::Round;
        throw SvgException("Invalid value for miter-join {}", cstrLineJoin.c_str());
    }

    Core::String LineJoinToString(LineJoin enumLineJoin)
    {
        switch(enumLineJoin) {
        case LineJoin::NotALineJoin: return "NotALineJoin";
        case LineJoin::Arcs: return "Arcs";
        case LineJoin::Bevel: return "Bevel";
        case LineJoin::Miter: return "Miter";
        case LineJoin::MiterClip: return "MiterClip";
        case LineJoin::Round: return "Round";
        default: return Core::String("Invalid illegal unknown unrecognized unprcessed garbage line join {}", static_cast<int32_t>(enumLineJoin));
        }
    }
}

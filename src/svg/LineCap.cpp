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

#include "svg/LineCap.hpp"

#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif
#ifndef DRAW2D_SVG_SVGEXCEPTION_DOT_HPP
    #include "svg/SvgException.hpp"
#endif

namespace Draw2d::Svg {

#define BUTT "butt"
#define ROUND "round"
#define SQUARE "square"

    LineCap ParseLineCap(const Core::String &cstrLineCap) {
        if(cstrLineCap == BUTT) return LineCap::Butt;
        if(cstrLineCap == ROUND) return LineCap::Round;
        if(cstrLineCap == SQUARE) return LineCap::Square;
        throw SvgException("Unrecognized line cap {}", cstrLineCap.c_str());
    }
    Core::String LineCapToString(LineCap enumLineCap) {
        switch(enumLineCap) {
        case LineCap::Butt: return "Butt :)";
        case LineCap::Round: return "Round";
        case LineCap::Square: return "Square";
        default: return Core::String("Invalid illegal unkown unrecognized unhandled garbage line cap {}", static_cast<int32_t>(enumLineCap));
        }
    }
}

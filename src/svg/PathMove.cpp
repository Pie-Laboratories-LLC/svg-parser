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

#include <unordered_map>
#include "svg/PathMove.hpp"

#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif
#ifndef DRAW2D_SVG_SVGEXCEPTION_DOT_HPP
    #include "svg/SvgException.hpp"
#endif

namespace Draw2d::Svg {

    PathMove ParsePathMove(const Core::String &cstrValue) {
        static std::unordered_map<Core::String,PathMove> PathMoveLookup {
        { "NotAPathMove", NotAPathMove },
        { "M", MoveTo },
        { "L", LineTo },
        { "C", ClosePath },
        { "Q", QuadBézierTo },
        { "T", SmoothQuadBézierTo },
        { "C", CubicBézierTo },
        { "S", SmoothCubicBézierTo },
        { "H", HorizontalLineTo },
        { "V", VerticalLineTo },
        { "A", ArcEllipticTo }
        };
        if(cstrValue.length() == 1) {
            Core::String strValue = cstrValue.to_upper();
            if (auto search = PathMoveLookup.find(strValue); search != PathMoveLookup.end()) return (*search).second;
        }
        throw SvgException("Unrecognized PathMove {}", cstrValue.c_str());
    }

    Core::String PathMoveToString(PathMove enumPathMove) {
        switch(enumPathMove) {
        case NotAPathMove: return "NotAPathMove";
        case MoveTo: return "MoveTo";
        case LineTo: return "LineTo";
        case ClosePath: return "ClosePath";
        case QuadBézierTo: return "QuadBézierTo";
        case SmoothQuadBézierTo: return "SmoothQuadBézierTo";
        case CubicBézierTo: return "CubicBézierTo";
        case SmoothCubicBézierTo: return "SmoothCubicBézierTo";
        case HorizontalLineTo: return "HorizontalLineTo";
        case VerticalLineTo: return "VerticalLineTo";
        case ArcEllipticTo: return "ArcEllipticTo";
        default: return Core::String("Invalid Illegal Unknown Garbage PathMove {}",static_cast<int32_t>(enumPathMove));
        }
    }

}

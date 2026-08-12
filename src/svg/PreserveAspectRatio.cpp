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
#include "svg/PreserveAspectRatio.hpp"

#ifndef DRAW2D_SVG_SVGEXCEPTION_DOT_HPP
    #include "svg/SvgException.hpp"
#endif

namespace Draw2d::Svg {
    PreserveAspectRatio ParsePreserveAspectRatio(const Core::String &cstrPreserveAspectRatio) {
        static std::unordered_map<Core::String,PreserveAspectRatio> PreserveAspectRatioMap {
            { "NotAPreserveAspectRatio", PreserveAspectRatio::NotAPreserveAspectRatio },
            { "none", PreserveAspectRatio::none },
            { "xMinYMin", PreserveAspectRatio::xMinYMin },
            { "xMidYMin", PreserveAspectRatio::xMidYMin },
            { "xMaxYMin", PreserveAspectRatio::xMaxYMin },
            { "xMinYMid", PreserveAspectRatio::xMinYMid },
            { "xMidYMid", PreserveAspectRatio::xMidYMid },
            { "xMaxYMid", PreserveAspectRatio::xMaxYMid },
            { "xMinYMax", PreserveAspectRatio::xMinYMax },
            { "xMidYMax", PreserveAspectRatio::xMidYMax },
            { "xMaxYMax", PreserveAspectRatio::xMaxYMax }
        };
        if(PreserveAspectRatioMap.contains(cstrPreserveAspectRatio)) {
            return PreserveAspectRatioMap[cstrPreserveAspectRatio];
        }
        throw SvgException("Unrecognized PreserveAspectRatio {}", cstrPreserveAspectRatio.c_str());
    }

    Core::String PreserveAspectRatioToString(PreserveAspectRatio enumPreserveAspectRatio)
    {
        switch(enumPreserveAspectRatio) {
        case PreserveAspectRatio::NotAPreserveAspectRatio: return "NotAPreserveAspectRatio";
        case PreserveAspectRatio::none: return "none";
        case PreserveAspectRatio::xMinYMin: return "xMinYMin";
        case PreserveAspectRatio::xMidYMin: return "xMidYMin";
        case PreserveAspectRatio::xMaxYMin: return "xMaxYMin";
        case PreserveAspectRatio::xMinYMid: return "xMinYMid";
        case PreserveAspectRatio::xMidYMid: return "xMidYMid";
        case PreserveAspectRatio::xMaxYMid: return "xMaxYMid";
        case PreserveAspectRatio::xMinYMax: return "xMinYMax";
        case PreserveAspectRatio::xMidYMax: return "xMidYMax";
        case PreserveAspectRatio::xMaxYMax: return "xMaxYMax";
        default: return Core::String("Invalid illegal unknown unrecognized unprocessed garbage PreserveAspectRatio {}", static_cast<int32_t>(enumPreserveAspectRatio));
        }
    }

#define MEET "meet"
#define SLICE "slice"
    PreserveAspectRatioMode ParsePreserveAspectRatioMode(const Core::String &cstrPreserveAspectRatioMode)
    {
        if(cstrPreserveAspectRatioMode == MEET) return PreserveAspectRatioMode::Meet;
        if(cstrPreserveAspectRatioMode == SLICE) return PreserveAspectRatioMode::Slice;
        throw SvgException("Unrecognized PreserveAspectRatioMode {}", cstrPreserveAspectRatioMode.c_str());
    }

    Core::String PreserveAspectRatioModeToString(PreserveAspectRatioMode enumPreserveAspectRatioMode)
    {
        switch(enumPreserveAspectRatioMode) {
            case PreserveAspectRatioMode::NotAPreserveAspectRatioMode: return "NotAPreserveAspectRatioMode";
            case PreserveAspectRatioMode::Meet: return "Meet";
            case PreserveAspectRatioMode::Slice: return "Slice";
            default: return Core::String("Invalid illegal uknown unrecognized unhandled unprocessed garbage PreserveAspectRatioMode {}", static_cast<int32_t>(enumPreserveAspectRatioMode));
        }
    }
}

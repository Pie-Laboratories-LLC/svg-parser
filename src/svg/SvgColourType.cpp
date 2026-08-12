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
#include "svg/SvgColourType.hpp"

#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif
#ifndef DRAW2D_SVG_SVGEXCEPTION_DOT_HPP
    #include "svg/SvgException.hpp"
#endif

namespace Draw2d::Svg {

    SvgColourType ParseSvgColourType(const Core::String &cstrSvgColourType)
    {
        static const std::unordered_map<Core::String, SvgColourType> SvgColourTypeLookup {
            { "NotAnSvgColourType", SvgColourType::NotAnSvgColourType },
            { "CurrentColor", SvgColourType::CurrentColor },
            { "Cpal", SvgColourType::Cpal },
            { "Bgr", SvgColourType::Bgr },
            { "BgrA", SvgColourType::BgrA },
            { "Hsl", SvgColourType::Hsl },
            { "HslA", SvgColourType::HslA },
            { "Gradient", SvgColourType::Gradient },
            { "None", SvgColourType::None }
        };
        if (auto search = SvgColourTypeLookup.find(cstrSvgColourType); search != SvgColourTypeLookup.end()) return (*search).second;
        throw SvgException("Unrecognized SvgColourType {}", cstrSvgColourType.c_str());
    }

    Core::String SvgColourTypeToString(SvgColourType enumSvgColourType)
    {
        switch(enumSvgColourType) {
        case SvgColourType::NotAnSvgColourType: return "NotAnSvgColourType";
        case SvgColourType::CurrentColor: return "CurrentColor";
        case SvgColourType::Cpal: return "Cpal";
        case SvgColourType::Bgr: return "Bgr";
        case SvgColourType::BgrA: return "BgrA";
        case SvgColourType::Hsl: return "Hsl";
        case SvgColourType::HslA: return "HslA";
        case SvgColourType::Gradient: return "Gradient";
        case SvgColourType::None: return "None";
        default: return Core::String("Invalid Illegal Unknown Unrecognized Garbage SvgColourType {}",static_cast<int32_t>(enumSvgColourType));
        }
    }

}

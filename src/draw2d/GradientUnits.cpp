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

#include <format>
#include <map>
#include "draw2d/GradientUnits.hpp"

#ifndef STRING_DOT_HPP
    #include "core/String.hpp"
#endif
#ifndef SVGEXCEPTION_DOT_HPP
    #include "svg/SvgException.hpp"
#endif

namespace Draw2d {

    const char * const GradientUnitsToString(GradientUnits enumGradientUnits) {
        switch(enumGradientUnits) {
        case NotAGradientUnit: return "NotAGradientUnit";
        case UserSpaceOnUse: return "UserSpaceOnUse";
        case ObjectBoundingBox: return "ObjectBoundingBox";
        default: return "Invalid Illegal Unknown Unrecognized Garbage GradientUnits";
        }
    }

    std::map<Core::String, GradientUnits> stringToGradientUnits = {
        { "userSpaceOnUse", UserSpaceOnUse },
        { "objectBoundingBox", ObjectBoundingBox }
    };

    GradientUnits ParseGradientUnits(const Core::String &cstrGradientUnits) {
        if (auto search = stringToGradientUnits.find(cstrGradientUnits); search != stringToGradientUnits.end()) return (*search).second;
        throw Svg::SvgException("Invalid illegal unknown unhandled unrecognized garbage GradientUnits {}", cstrGradientUnits.c_str());
    }
}

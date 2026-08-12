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

#include "svg/LengthUnits.hpp"

#include "svg/SvgException.hpp"

namespace Draw2d::Svg {

#define PX "px"
#define MM "mm"
#define EM "em"
    LengthUnits ParseLengthUnits(const Core::String &cstrLengthUnits) {
        if(cstrLengthUnits == PX) return LengthUnits::Px;
        if(cstrLengthUnits == MM) return LengthUnits::Mm;
        if(cstrLengthUnits == EM) return LengthUnits::Em;
        throw SvgException("Unrecognized LengthUnit {}", cstrLengthUnits.c_str());
    }

    Core::String LengthUnitsToString(LengthUnits enumLengthUnits)
    {
        switch(enumLengthUnits) {
        case LengthUnits::Px: return "px";
        case LengthUnits::Mm: return "mm";
        case LengthUnits::Em: return "em";
        default: return Core::String("Invalid unknown illegal unhandled unrecognized garbage LengthUnits {}", static_cast<int32_t>(enumLengthUnits));
        }
    }
}

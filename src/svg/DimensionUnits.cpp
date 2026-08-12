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
#include "svg/DimensionUnits.hpp"

#ifndef DRAW2D_SVG_SVGEXCEPTION_DOT_HPP
    #include "svg/SvgException.hpp"
#endif

namespace Draw2d::Svg {
    DimensionUnits ParseDimensionUnits(const Core::String &cstrDimensionUnits)
    {
        static std::unordered_map<Core::String,DimensionUnits> LookupDimensionUnits = {
            { "NotADimensionUnit", DimensionUnits::NotADimensionUnit },
            { "percent", DimensionUnits::Percentage },
            { "%", DimensionUnits::Percentage },
            { "px", DimensionUnits::Px },
            { "pt", DimensionUnits::Pt },
            { "pc", DimensionUnits::Pc },
            { "cm", DimensionUnits::Cm },
            { "mm", DimensionUnits::Mm },
            { "in", DimensionUnits::In },
            { "q", DimensionUnits::Q },
            { "em", DimensionUnits::Em },
            { "ex", DimensionUnits::Ex },
            { "ch", DimensionUnits::Ch },
            { "rem", DimensionUnits::Rem },
            { "vw", DimensionUnits::Vw },
            { "vh", DimensionUnits::Vh },
            { "vmin", DimensionUnits::Vmin },
            { "vmax", DimensionUnits::Vmax }
        };
        if (auto search = LookupDimensionUnits.find(cstrDimensionUnits); search != LookupDimensionUnits.end()) return (*search).second;
        throw SvgException("Unrecognized dimension unit {}", cstrDimensionUnits.c_str());
    }

    Core::String DimensionUnitsToString(DimensionUnits enumDimensionUnits)
    {
        switch(enumDimensionUnits) {
        case DimensionUnits::NotADimensionUnit: return "NotADimensionUnit";
        case DimensionUnits::Percentage: return "Percentage";
        case DimensionUnits::Px: return "Px";
        case DimensionUnits::Pt: return "Pt";
        case DimensionUnits::Pc: return "Pc";
        case DimensionUnits::Cm: return "Cm";
        case DimensionUnits::Mm: return "Mm";
        case DimensionUnits::In: return "In";
        case DimensionUnits::Q: return "Q";
        case DimensionUnits::Em: return "Em";
        case DimensionUnits::Ex: return "Ex";
        case DimensionUnits::Ch: return "Ch";
        case DimensionUnits::Rem: return "Rem";
        case DimensionUnits::Vw: return "Vw";
        case DimensionUnits::Vh: return "Vh";
        case DimensionUnits::Vmin: return "Vmin";
        case DimensionUnits::Vmax: return "Vmax";
        default: throw SvgException("Invalid illegal unknown unrecognized unhandled garbage DimensionUnits {}", static_cast<int32_t>(enumDimensionUnits));
        }
    }

}

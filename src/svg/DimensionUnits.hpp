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

#ifndef DRAW2D_SVG_DIMENSIONUNITS_DOT_HPP
#define DRAW2D_SVG_DIMENSIONUNITS_DOT_HPP

#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif

namespace Draw2d::Svg {

    enum class DimensionUnits {
        NotADimensionUnit = -1,
        Percentage,
        Px,
        Pt,
        Pc,
        Cm,
        Mm,
        In,
        Q,
        Em,
        Ex,
        Ch,
        Rem,
        Vw,
        Vh,
        Vmin,
        Vmax
    };

    DimensionUnits ParseDimensionUnits(const Core::String &cstrDimensionUnits);
    Core::String DimensionUnitsToString(DimensionUnits enumDimensionUnits);
}

#endif  /* DRAW2D_SVG_DIMENSIONUNITS_DOT_HPP */

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

#ifndef DRAW2D_GRADIENTTEMPLATE_DOT_HPP
#define DRAW2D_GRADIENTTEMPLATE_DOT_HPP

#include <array>

#ifndef DRAW2D_GRADIENTUNITS_DOT_HPP
    #include "draw2d/GradientUnits.hpp"
#endif
#ifndef DRAW2D_SPREADMETHOD_DOT_HPP
    #include "draw2d/SpreadMethod.hpp"
#endif
#ifndef DRAW2D_STOP_DOT_HPP
    #include "draw2d/Stop.hpp"
#endif
#ifndef DRAW2D_SVG_SVGCOLOUR_DOT_HPP
    #include "svg/SvgColour.hpp"
#endif

namespace Draw2d::Svg {

    struct GradientTemplate
    {
        Core::String         strId;
        enum {
            linear, radial
        }                    kind;
        std::array<float, 6> geometry;       // x1,y1,x2,y2 or cx,cy,r,fx,fy,fr
        GradientUnits enumGradientUnits;     // objectBoundingBox | userSpaceOnUse
        SpreadMethod enumSpreadMethod;       // pad | reflect | repeat
        std::optional<std::array<float,6>> transform;      // gradientTransform
        std::vector<Stop<SvgColour>> stops;
        Core::String strHrefId;              // for xlink:href inheritance

        // which of the above the element set itself; unset ones come from the gradient
        //  strHrefId names, then from the SVG defaults (SvgDocument::resolveGradientReferences)
        std::array<bool, 6> geometrySpecified {};
        bool bUnitsSpecified = false;
        bool bSpreadMethodSpecified = false;

        // takes what this gradient leaves unset from the one it references: stops if it has
        //  none; units, spread method and transform; geometry only between gradients of the
        //  same kind
        void inheritFrom(const GradientTemplate &referenced);
        // the SVG defaults for any geometry still unset: x1 0, y1 0, x2 100%, y2 0; cx, cy and
        //  r 50%, fx and fy at the centre, fr 0
        void applyDefaults();

        std::vector<Stop<SvgColour>> cloneStops() const;
        std::optional<std::array<float,6>> cloneTransform() const;
    };

}

#endif  /* DRAW2D_GRADIENTTEMPLATE_DOT_HPP */

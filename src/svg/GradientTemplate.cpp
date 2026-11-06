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

#include "svg/GradientTemplate.hpp"

#ifndef DRAW2D_SVG_MATRIXMATH_DOT_HPP
    #include "svg/MatrixMath.hpp"
#endif

namespace Draw2d::Svg {
    void GradientTemplate::inheritFrom(const GradientTemplate &referenced)
    {
        if(stops.empty()) stops = referenced.cloneStops();
        if(!bUnitsSpecified && referenced.bUnitsSpecified) {
            enumGradientUnits = referenced.enumGradientUnits;
            bUnitsSpecified = true;
        }
        if(!bSpreadMethodSpecified && referenced.bSpreadMethodSpecified) {
            enumSpreadMethod = referenced.enumSpreadMethod;
            bSpreadMethodSpecified = true;
        }
        if(!transform.has_value() && referenced.transform.has_value()) transform = referenced.cloneTransform();
        if(kind != referenced.kind) return;
        for(size_t n = 0; n < geometry.size(); n++) {
            if(!geometrySpecified[n] && referenced.geometrySpecified[n]) {
                geometry[n] = referenced.geometry[n];
                geometrySpecified[n] = true;
            }
        }
    }

    void GradientTemplate::applyDefaults()
    {
        // defaults aren't marked as specified: a gradient referencing this one must fall back
        //  to its own defaults (its own centre for fx/fy), not inherit these
        auto setDefault = [this](size_t nIndex, float fValue) {
            if(!geometrySpecified[nIndex]) geometry[nIndex] = fValue;
        };
        if(kind == linear) {
            setDefault(0, 0.0f);
            setDefault(1, 0.0f);
            setDefault(2, 1.0f);
            setDefault(3, 0.0f);
        }
        else {
            setDefault(0, 0.5f);
            setDefault(1, 0.5f);
            setDefault(2, 0.5f);
            // the focal point defaults to the centre, whatever the centre turned out to be
            setDefault(3, geometry[0]);
            setDefault(4, geometry[1]);
            setDefault(5, 0.0f);
        }
    }

    std::vector<Stop<SvgColour>> GradientTemplate::cloneStops() const
    {
        std::vector<Stop<SvgColour>> returnStops{};
        for(auto &stop: stops) returnStops.push_back(Stop<SvgColour>(stop));
        return returnStops;
    }

    std::optional<std::array<float,6>> GradientTemplate::cloneTransform() const
    {
        return transform.has_value()
          ? std::optional<std::array<float,6>>(Svg::duplicateMatrix6<float>(transform.value()))
          : std::nullopt;
    }
}

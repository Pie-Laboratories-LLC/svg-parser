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

#ifndef DRAW2D_SVG_CIRCLE_DOT_HPP
#define DRAW2D_SVG_CIRCLE_DOT_HPP

#ifndef DRAW2D_SVG_SVGENTITY_DOT_HPP
    #include "svg/SvgEntity.hpp"
#endif
#ifndef DRAW2D_SVG_DIMENSION_DOT_HPP
    #include "svg/Dimension.hpp"
#endif
#ifndef DRAW2D_SVG_SVGPATHLENGTHABLEENTITY_DOT_HPP
    #include "svg/SvgPathLengthableEntity.hpp"
#endif

namespace Draw2d::Svg {

    struct SvgCircleParams : public SvgEntityParams {
        SvgPathLengthableParams svgPathLengthableParams;
        std::optional<Dimension> cx;
        std::optional<Dimension> cy;
        std::optional<Dimension> r;
    };

    // it is tempting to use SvgDimensionedEntity for this, as the ellipse needs to fit in a particular
    //  bounding box which can be derived from cx, cy, rx, ry, but it's easier to deal with in this form,
    //  even notwithstanding that Bresenham's ellipse draws in a bounding box
    class Circle : public SvgEntity, public SvgPathLengthableEntity {
    public:
        Circle(SvgCircleParams svgCircleParams):
            SvgEntity(static_cast<SvgEntityParams &&>(svgCircleParams))
           ,SvgPathLengthableEntity(std::move(svgCircleParams.svgPathLengthableParams))
           ,m_cx(svgCircleParams.cx)
           ,m_cy(svgCircleParams.cy)
           ,m_r(svgCircleParams.r)
        {}

        std::optional<Dimension> getCx() const { return m_cx; }
        std::optional<Dimension> getCy() const { return m_cy; }
        std::optional<Dimension> getR() const { return m_r; }

        virtual const char * const getType() const { return "Circle"; }

    private:
        std::optional<Dimension> m_cx;
        std::optional<Dimension> m_cy;
        std::optional<Dimension> m_r;
    };

}

#endif  /* DRAW2D_SVG_CIRCLE_DOT_HPP */

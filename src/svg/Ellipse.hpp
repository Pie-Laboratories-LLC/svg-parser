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

#ifndef DRAW2D_SVG_ELLIPSE_DOT_HPP
#define DRAW2D_SVG_ELLIPSE_DOT_HPP

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

    struct SvgEllipseParams : public SvgEntityParams {
        SvgPathLengthableParams svgPathLengthableParams;
        std::optional<Dimension> cx;
        std::optional<Dimension> cy;
        std::optional<Dimension> rx;
        std::optional<Dimension> ry;
    };

    // it is tempting to use SvgDimensionedEntity for this, as the ellipse needs to fit in a particular
    //  bounding box which can be derived from cx, cy, rx, ry, but it's easier to deal with in this form,
    //  even notwithstanding that Bresenham's ellipse draws in a bounding box
    class Ellipse : public SvgEntity, public SvgPathLengthableEntity {
    public:
        Ellipse(SvgEllipseParams svgEllipseParams):
            SvgEntity(static_cast<SvgEntityParams &&>(svgEllipseParams))
           ,SvgPathLengthableEntity(std::move(svgEllipseParams.svgPathLengthableParams))
           ,m_cx(svgEllipseParams.cx)
           ,m_cy(svgEllipseParams.cy)
           ,m_rx(svgEllipseParams.rx)
           ,m_ry(svgEllipseParams.ry)
        {}

        std::optional<Dimension> getCx() const { return m_cx; }
        std::optional<Dimension> getCy() const { return m_cy; }
        std::optional<Dimension> getRx() const { return m_rx; }
        std::optional<Dimension> getRy() const { return m_ry; }

        virtual const char * const getType() const { return "Ellipse"; }

    private:
        std::optional<Dimension> m_cx;
        std::optional<Dimension> m_cy;
        std::optional<Dimension> m_rx;
        std::optional<Dimension> m_ry;
    };

}

#endif  /* DRAW2D_SVG_ELLIPSE_DOT_HPP */

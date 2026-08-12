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

#ifndef DRAW2D_SVG_RECT_DOT_HPP
#define DRAW2D_SVG_RECT_DOT_HPP

#ifndef DRAW2D_SVG_SVGENTITY_DOT_HPP
    #include "svg/SvgEntity.hpp"
#endif
#ifndef DRAW2D_SVG_SVGDIMENSIONEDENTITY_DOT_HPP
    #include "svg/SvgDimensionedEntity.hpp"
#endif

namespace Draw2d::Svg {

    struct SvgRectParams : public SvgEntityParams {
        SvgDimensionedParams svgDimensionedParams;
        std::optional<Dimension> cx;
        std::optional<Dimension> cy;
    };

    class Rect: public SvgEntity {
    public:
        // a lil ugly.  claude chastised me for using inheritance rather than
        //  composing the entity params and the dimensioned entity params
        //  separately.  this ensures move-correctness and preserves the
        //  parallel class hierarchy
        Rect(SvgRectParams p)
            : SvgEntity(static_cast<SvgEntityParams &&>(p))
            , m_svgDimensionedEntity(std::move(p.svgDimensionedParams))
            , m_cx(std::move(p.cx))
            , m_cy(std::move(p.cy))
        {}
        virtual ~Rect() = default;

        std::optional<Dimension> getX() const  { return m_svgDimensionedEntity.getX(); }
        std::optional<Dimension> getY() const  { return m_svgDimensionedEntity.getY(); }
        std::optional<Dimension> getWidth() const  { return m_svgDimensionedEntity.getWidth(); }
        std::optional<Dimension> getHeight() const  { return m_svgDimensionedEntity.getHeight(); }

        std::optional<Dimension> getCx() const { return m_cx; }
        std::optional<Dimension> getCy() const { return m_cy; }

        virtual const char * const getType() const override { return "Rect"; }
    private:
        SvgDimensionedEntity m_svgDimensionedEntity;
        std::optional<Dimension> m_cx;
        std::optional<Dimension> m_cy;
    };

}

#endif  /* DRAW2D_SVG_RECT_DOT_HPP */

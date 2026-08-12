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

#ifndef DRAW2D_SVGENTITY_DOT_HPP
#define DRAW2D_SVGENTITY_DOT_HPP

#include <optional>
#include <memory>
#ifndef STRING_DOT_HPP
    #include "core/String.hpp"
#endif
#ifndef DRAW2D_SVGEXCEPTION_DOT_HPP
    #include "svg/SvgException.hpp"
#endif
#ifndef DRAW2D_SVGPAINT_DOT_HPP
    #include "svg/SvgPaint.hpp"
#endif
#ifndef DRAW2D_SVG_FILLRULE_DOT_HPP
    #include "svg/FillRule.hpp"
#endif
#ifndef DRAW2D_SVG_LINECAP_DOT_HPP
    #include "svg/LineCap.hpp"
#endif
#ifndef DRAW2D_SVG_LINEJOIN_DOT_HPP
    #include "svg/LineJoin.hpp"
#endif
#ifndef DRAW2D_POINT_DOT_HPP
    #include "draw2d/Point.hpp"
#endif

namespace Draw2d::Svg {

    class Svg;
    class SvgDocument;

    struct SvgEntityParams {
        SvgDocument &svgDocument;
        bool render = true;
        Core::String id { };
        Core::String cssStyle { };
        Core::String cssClass { };
        std::unique_ptr<float[]> pMatrix = nullptr;
        SvgPaint fillColour { SvgColourType::None };
        FillRule fillRule;
        float fillOpacity = 1.0f;
        FillRule m_enumFillRule = FillRule::NonZero;
        SvgPaint strokeColour { SvgColourType::None };
        float strokeWidth = 1;
        float strokeOpacity = 1.0f;
        LineCap lineCap = LineCap::Butt;
        LineJoin lineJoin = LineJoin::Miter;
        float miterLimit = 4.0f;
        std::vector<float> dashArray {};
        float dashOffset = 0.0f;
    };

    class SvgEntity
    {
    public:
        SvgDocument &getDocument() const { return m_svgDocument; }
        virtual bool getRender() const { return m_bRender; }
        virtual const Core::String &getId() const { return m_strId; }
        virtual const float * const getMatrix() const { return m_pMatrix.get(); }
        virtual const Draw2d::Point &getUpperLeft() const { return m_upperLeft; }
        virtual const Draw2d::Point &getLowerRight() const { return m_lowerRight; }

        const std::optional<float> &getX() const { return m_fX; }
        const std::optional<float> &getY() const { return m_fY; }
        const std::optional<float> &getWidth() const { return m_fWidth; }
        const std::optional<float> &getHeight() const { return m_fHeight; }
        const SvgPaint &getFillColour() const { return m_fillColour; }
        FillRule getFillRule() const { return m_enumFillRule; }
        float getFillOpacity() const { return m_fFillOpacity; }
        const SvgPaint &getStrokeColour() const { return m_strokeColour; }
        float getStrokeOpacity() const { return m_fStrokeOpacity; }
        float getStrokeWidth() const { return m_fStrokeWidth; }
        LineCap getLineCap() const { return m_enumLineCap; }
        LineJoin getLineJoin() const { return m_enumLineJoin; }
        float getMiterLimit() const { return m_fMiterLimit; }
        const std::vector<float> getDashArray() const { return m_dashArray; }
        virtual const char * const getType() const { return "SvgEntity"; }

        SvgEntity(SvgEntityParams params);
        SvgEntity(const SvgEntity &copy);
        SvgEntity(SvgEntity &&move);
        virtual ~SvgEntity() = default;

    protected:
        SvgDocument &m_svgDocument;
        bool m_bRender = true;
        Core::String m_strId { };
        std::unique_ptr<float[]> m_pMatrix = nullptr;
        Draw2d::Point m_upperLeft { std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
        Draw2d::Point m_lowerRight { std::numeric_limits<float>::min(), std::numeric_limits<float>::min() };
        std::optional<float> m_fX = {};
        std::optional<float> m_fY = {};
        std::optional<float> m_fWidth = {};
        std::optional<float> m_fHeight = {};
        SvgPaint m_fillColour { SvgColourType::None };
        float m_fFillOpacity = 1;
        FillRule m_enumFillRule = FillRule::NonZero;
        SvgPaint m_strokeColour { SvgColourType::None };
        float m_fStrokeOpacity = 1;
        int m_fStrokeWidth = 1;
        LineCap m_enumLineCap = LineCap::Butt;
        LineJoin m_enumLineJoin = LineJoin::Miter;
        float m_fMiterLimit = 4.0f;
        std::vector<float> m_dashArray { };
        Core::String m_strCssClass {};
        Core::String m_strCssStyle {};
    }; // class SvgEntity: ISvgEntity

} // namespace Draw2d::Svg

#endif  /* SVGENTITY_DOT_HPP */

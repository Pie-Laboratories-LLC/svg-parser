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

#ifndef DRAW2D_SVG_SVGDRAWPROPERTIES_DOT_HPP
#define DRAW2D_SVG_SVGDRAWPROPERTIES_DOT_HPP

#include <cstring>
#ifndef DRAW2D_SVG_SVGPAINT_DOT_HPP
    #include "svg/SvgPaint.hpp"
#endif
#ifndef DRAW2D_SVG_MATRIXMATHS_DOT_HPP
    #include "svg/MatrixMath.hpp"
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
#include "svg/SvgContainerEntity.hpp"

namespace Draw2d::Svg {

    class SvgEntity;
    class SvgDocument;

    /// manages state used during the parse; color for fill/stroke/etc = "currentColor", the current dash
    ///  pattern and so forth.  It is the caller's responsibility to set:
    ///  * svgDocument & => a reference to the root SvgDocument
    ///  * width, height => the user space coordinates into which to render.  This will either be canvas
    ///    size or the size of glyph being rendered in to, if rendering OT-SVG fonts.
    class SvgParserContext
    {
    public:
        SvgContainerEntity *getChildWrangler() { return m_pSvgChildWrangler; }
        void setChildWrangler(SvgContainerEntity *pSvgChildWrangler) { m_pSvgChildWrangler = pSvgChildWrangler; }
        bool getRender() const { return m_bRender; }
        void setRender(bool bRender) { m_bRender = bRender; }
        float getViewportWidth() const { return m_fViewportWidth; }
        void setViewportWidth(float fWidth) { m_fViewportWidth = fWidth; }
        float getViewportHeight() const { return m_fViewportHeight; }
        void setViewportHeight(float fHeight) { m_fViewportHeight = fHeight; }
        const SvgColour &getColour() const { return m_colour; }
        void setColour(const SvgColour &colour) { m_colour = colour; }
        const SvgPaint &getFillColour() const { return m_fillColour.has_value() ? m_fillColour.value() : sm_defaultFillColour; }
        void setFillColour(const SvgPaint &fillColour) { m_fillColour = fillColour; }
        FillRule getFillRule() const { return m_enumFillRule; }
        void setFillRule(FillRule enumFillRule) { m_enumFillRule = enumFillRule; }
        float getFillOpacity() const { return m_fFillOpacity; }
        void setFillOpacity(float fFillOpacity) { m_fFillOpacity = fFillOpacity; }
        const SvgPaint &getStrokeColour() const { return m_strokeColour.has_value() ? m_strokeColour.value() : sm_defaultStrokeColour; }
        void setStrokeColour(const SvgPaint &strokeColour) { m_strokeColour = strokeColour; }
        float getStrokeOpacity() const { return m_fStrokeOpacity; }
        void setStrokeOpacity(float fStrokeOpacity) { m_fStrokeOpacity = fStrokeOpacity; }
        float getStrokeWidth() const { return m_fStrokeWidth; }
        void setStrokeWidth(float fStrokeWidth) { m_fStrokeWidth = fStrokeWidth; }
        LineCap getLineCap() const { return m_enumLineCap; }
        void setLineCap(LineCap enumLineCap) { m_enumLineCap = enumLineCap; }
        LineJoin getLineJoin() const { return m_enumLineJoin; }
        void setLineJoin(LineJoin enumLineJoin) { m_enumLineJoin = enumLineJoin; }
        float getStrokeMiterLimit() const { return m_fStrokeMiterLimit; }
        void setStrokeMiterLimit(float fStrokeMiterLimit) { m_fStrokeMiterLimit = fStrokeMiterLimit; }
        const std::vector<float> &getDashArray() const { return m_dashArray; }
        void setDashArray(std::vector<float> &dashArray) { m_dashArray = std::move(dashArray); }
        float getDashOffset() const { return m_fDashOffset; }
        void setDashOffset(float fDashOffset) { m_fDashOffset = fDashOffset; }
        float getX() const { return m_fX; }
        void setX(float fX) { m_fX = fX; }
        float getY() const { return m_fY; }
        void setY(float fY) { m_fY = fY; }
        const std::array<float,6> getMatrix() const { return m_matrix; }
        static const SvgPaint &getDefaultStrokeColour() { return sm_defaultStrokeColour; }
        static const SvgPaint &getDefaultFillColour() { return sm_defaultFillColour; }

        void multiplyRight(const std::array<float,6> matrix)
        {
            m_matrix = multiply6s(m_matrix, matrix);
        }

        SvgParserContext updateDrawProperties(const SvgParserContext &reference, const SvgEntity *cpSvgEntity);

    private:
        SvgContainerEntity *m_pSvgChildWrangler = nullptr;
        bool m_bRender = true;
        float m_fViewportWidth = 0.0f;
        float m_fViewportHeight = 0.0f;
        SvgColour m_colour { SvgColourType::Bgr, -1, { { 0, 0, 0, 0xff } } };
        std::optional<SvgPaint> m_fillColour = {};
        FillRule m_enumFillRule = FillRule::NonZero;
        float m_fFillOpacity = 1.0;
        std::optional<SvgPaint> m_strokeColour = {};
        float m_fStrokeWidth = 1;
        float m_fStrokeOpacity = 1.0f;
        LineCap m_enumLineCap = LineCap::Butt; // it's the default, really
        LineJoin m_enumLineJoin = LineJoin::Miter; // it's the default, really
        float m_fStrokeMiterLimit = 4.0;
        std::vector<float> m_dashArray {};
        size_t m_nDashArrayLength = 0;
        float m_fDashOffset = 0.0f;
        float m_fX = 0.0f;
        float m_fY = 0.0f;
        std::array<float,6> m_matrix = { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };

        static SvgPaint sm_defaultStrokeColour;
        static SvgPaint sm_defaultFillColour;
    }; // class SvgParserContext

} // namespace Draw2d::Svg

#endif  /* DRAW2D_SVG_SVGDRAWPROPERTIES_DOT_HPP */

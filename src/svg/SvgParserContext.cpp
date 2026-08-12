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

#include "svg/SvgParserContext.hpp"

#ifndef CORE_STDUNIQUEPTREXTENSIONS_DOT_HPP
    #include "core/StdUniquePtrExtensions.hpp"
#endif

namespace Draw2d::Svg {

    SvgPaint SvgParserContext::sm_defaultStrokeColour { SvgColourType::Bgr, "", SvgColour { SvgColourType::Bgr, -1, Core::makeUniqueArray<uint8_t>({ 0, 0, 0 }), 3 } };
    SvgPaint SvgParserContext::sm_defaultFillColour { SvgColourType::Bgr, "", SvgColour { SvgColourType::Bgr, -1, Core::makeUniqueArray<uint8_t>({ 255, 255, 255 }), 3 } };

    void SvgParserContext::swap(SvgParserContext &other)
    {
        std::swap(m_pSvgChildWrangler,other.m_pSvgChildWrangler);
        std::swap(m_bRender,other.m_bRender);
        std::swap(m_fViewportWidth,other.m_fViewportWidth);
        std::swap(m_fViewportHeight,other.m_fViewportHeight);
        std::swap(m_colour,other.m_colour);
        std::swap(m_fillColour,other.m_fillColour);
        std::swap(m_enumFillRule,other.m_enumFillRule);
        std::swap(m_fFillOpacity,other.m_fFillOpacity);
        std::swap(m_strokeColour,other.m_strokeColour);
        std::swap(m_fStrokeWidth,other.m_fStrokeWidth);
        std::swap(m_fStrokeOpacity,other.m_fStrokeOpacity);
        std::swap(m_enumLineCap,other.m_enumLineCap);
        std::swap(m_enumLineJoin,other.m_enumLineJoin);
        std::swap(m_fStrokeMiterLimit,other.m_fStrokeMiterLimit);
        std::swap(m_dashArray,other.m_dashArray);
        std::swap(m_nDashArrayLength,other.m_nDashArrayLength);
        std::swap(m_fDashOffset,other.m_fDashOffset);
        std::swap(m_fX,other.m_fX);
        std::swap(m_fY,other.m_fY);
        std::swap(m_pMatrix,other.m_pMatrix);
    }
} // namespace Draw2d::Svg

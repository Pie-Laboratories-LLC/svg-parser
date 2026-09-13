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

#include "svg/Svg.hpp"

#ifndef CORE_STDUNIQUEPTREXTENSIONS_DOT_HPP
    #include "core/StdUniquePtrExtensions.hpp"
#endif

namespace Draw2d::Svg {

    const SvgEntity *Svg::debugGetChild(unsigned nIndex) {
        return m_svgEntities[nIndex].get();
    }

    /// claude (Anthropic) generated.
    std::array<float,6> Svg::computeViewboxTransform(float fViewportX, float fViewportY,
                                                     float fViewportWidth, float fViewportHeight) const
    {
        if(m_enumPreserveAspectRatio == PreserveAspectRatio::NotAPreserveAspectRatio) {
            throw SvgException("PreserveAspectRatio was never set!");
        }
        if(m_enumPreserveAspectRatioMode == PreserveAspectRatioMode::NotAPreserveAspectRatioMode) {
            throw SvgException("PreserveAspectRatioMode was never set!");
        }
    
        if(!m_viewbox.has_value()) {
            return { 1.0f, 0.0f, 0.0f, 1.0f, fViewportX, fViewportY };
        }
        const Viewbox &vb = m_viewbox.value();
    
        float fScaleX = fViewportWidth  / vb.width;
        float fScaleY = fViewportHeight / vb.height;
    
        if(m_enumPreserveAspectRatio != PreserveAspectRatio::none) {
            float fScale = (m_enumPreserveAspectRatioMode == PreserveAspectRatioMode::Slice)
                ? std::max(fScaleX, fScaleY)
                : std::min(fScaleX, fScaleY);
            fScaleX = fScaleY = fScale;
        }
    
        float fTranslateX = fViewportX - vb.x * fScaleX;
        float fTranslateY = fViewportY - vb.y * fScaleY;
    
        float fExtraX = fViewportWidth  - vb.width  * fScaleX;
        float fExtraY = fViewportHeight - vb.height * fScaleY;
    
        switch(m_enumPreserveAspectRatio) {
            case PreserveAspectRatio::xMidYMin:
            case PreserveAspectRatio::xMidYMid:
            case PreserveAspectRatio::xMidYMax:
                fTranslateX += fExtraX / 2.0f;
                break;
            case PreserveAspectRatio::xMaxYMin:
            case PreserveAspectRatio::xMaxYMid:
            case PreserveAspectRatio::xMaxYMax:
                fTranslateX += fExtraX;
                break;
            default:
                break;
        }

        switch(m_enumPreserveAspectRatio) {
            case PreserveAspectRatio::xMinYMid:
            case PreserveAspectRatio::xMidYMid:
            case PreserveAspectRatio::xMaxYMid:
                fTranslateY += fExtraY / 2.0f;
                break;
            case PreserveAspectRatio::xMinYMax:
            case PreserveAspectRatio::xMidYMax:
            case PreserveAspectRatio::xMaxYMax:
                fTranslateY += fExtraY;
                break;
            default:
                break;
        }

        return { fScaleX, 0.0f, 0.0f, fScaleY, fTranslateX, fTranslateY };
    }
}

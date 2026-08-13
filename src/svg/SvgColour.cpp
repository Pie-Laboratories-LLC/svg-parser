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

#include <algorithm>
#include "svg/SvgColour.hpp"

#include "draw2d/Colour.hpp"

namespace Draw2d::Svg {

    SvgColour::SvgColour(SvgColourType enumSvgColourType, int colourIndex, std::optional<std::array<uint8_t,4>> pBgra, std::optional<std::array<float,4>> pHsla, std::unique_ptr<SvgColour> pFallback):
        m_enumSvgColourType(enumSvgColourType)
       ,m_pFallback(std::move(pFallback))
    {

        if(pBgra.has_value() && enumSvgColourType != SvgColourType::Bgr && enumSvgColourType != SvgColourType::BgrA) throw SvgException(std::format("SvgColourType {} doesn't support bgr(a) but it was supplied!", SvgColourTypeToString(enumSvgColourType).c_str()));
        if(pHsla.has_value() && enumSvgColourType != SvgColourType::Hsl && enumSvgColourType != SvgColourType::HslA) throw SvgException(std::format("SvgColourType {} doesn't support bgr(a) but it was supplied!", SvgColourTypeToString(enumSvgColourType).c_str()));

        switch (enumSvgColourType)
        {
        case SvgColourType::CurrentColor: break;
        case SvgColourType::Cpal:
            if (colourIndex < 0) throw SvgException(std::format("ColourIndex is required for {}", SvgColourTypeToString(enumSvgColourType).c_str()));
            m_nColourIndex = colourIndex;
            break;

        case SvgColourType::Bgr:
        case SvgColourType::BgrA:
            if (!pBgra.has_value()) throw SvgException(std::format("bgra values are required for bgr of type {}", SvgColourTypeToString(enumSvgColourType).c_str()));
            m_pBgra = std::move(pBgra);
            break;

        case SvgColourType::Hsl:
        case SvgColourType::HslA:
            if (!pHsla.has_value()) throw SvgException(std::format("hsla values are required for hsl of type {}", SvgColourTypeToString(enumSvgColourType).c_str()));
            m_pHsla = std::move(pHsla);
            break;

        // none is a legitimate value for SvgColour.  There's no data with it.
        case SvgColourType::None: break;

        case SvgColourType::Gradient:
            throw SvgException("SvgColour does not support SvgColourType {}, use SvgPaint", SvgColourTypeToString(enumSvgColourType).c_str());
            break;

        default: throw SvgException(std::format("Unrecognized SvgColourType {}!", SvgColourTypeToString(enumSvgColourType).c_str()));
        }
    }

    SvgColour::SvgColour(SvgColourType svgColourType, int nColourIndex, std::optional<std::array<uint8_t,4>> pBgra, std::optional<std::array<float,4>> pHsla):
        SvgColour(svgColourType, nColourIndex, std::move(pBgra), std::move(pHsla), nullptr)
    {
    }

    void SvgColour::swap(SvgColour &copy) noexcept
    {
        std::swap(m_enumSvgColourType, copy.m_enumSvgColourType);
        std::swap(m_nColourIndex, copy.m_nColourIndex);
        std::swap(m_pBgra, copy.m_pBgra);
        std::swap(m_pHsla, copy.m_pHsla);
        std::swap(m_pFallback, copy.m_pFallback);
    }

    std::unique_ptr<SvgColour> SvgColour::clone() const
    {
        std::optional<std::array<uint8_t,4>> pBgra = std::nullopt;
        std::optional<std::array<float,4>> pHsla = std::nullopt;
        if(m_pBgra.has_value()) pBgra = m_pBgra.value();
        if(m_pHsla.has_value()) pHsla = m_pHsla.value();
        return std::make_unique<SvgColour>(m_enumSvgColourType, m_nColourIndex, std::move(pBgra), std::move(pHsla));
    }

    Colour hslaToRgba(float fH, float fS, float fL, float fA)
    {
        fH = std::fmod(std::fmod(fH, 360.0f) + 360.0f, 360.0f);

        const float fC  = (1.0f - std::fabs(2.0f * fL - 1.0f)) * fS;
        const float fHp = fH / 60.0f;
        const float fX  = fC * (1.0f - std::fabs(std::fmod(fHp, 2.0f) - 1.0f));
        const float fM  = fL - fC / 2.0f;

        float fR = 0.0f, fG = 0.0f, fB = 0.0f;
        switch (static_cast<int>(fHp))
        {
            case 0:  fR = fC; fG = fX; break;
            case 1:  fR = fX; fG = fC; break;
            case 2:  fG = fC; fB = fX; break;
            case 3:  fG = fX; fB = fC; break;
            case 4:  fR = fX; fB = fC; break;
            default: fR = fC; fB = fX; break;
        }

        return Colour(fR + fM, fG + fM, fB + fM, fA);
    }
} // namespace Draw2d::Svg

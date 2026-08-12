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

    SvgColour::SvgColour(SvgColourType enumSvgColourType, int colourIndex, std::unique_ptr<uint8_t[]> pBgra, unsigned nBgraLength, std::unique_ptr<float[]> pHsla, unsigned nHslaLength)
    {
        m_enumSvgColourType = enumSvgColourType;

        if(pBgra && enumSvgColourType != SvgColourType::Bgr && enumSvgColourType != SvgColourType::BgrA) throw SvgException(std::format("SvgColourType {} doesn't support bgr(a) but it was supplied!", SvgColourTypeToString(enumSvgColourType).c_str()));
        if(pHsla && enumSvgColourType != SvgColourType::Hsl && enumSvgColourType != SvgColourType::HslA) throw SvgException(std::format("SvgColourType {} doesn't support bgr(a) but it was supplied!", SvgColourTypeToString(enumSvgColourType).c_str()));

        switch (enumSvgColourType)
        {
        case SvgColourType::CurrentColor: break;
        case SvgColourType::Cpal:
            if (colourIndex < 0) throw SvgException(std::format("ColourIndex is required for {}", SvgColourTypeToString(enumSvgColourType).c_str()));
            m_nColourIndex = colourIndex;
            break;

        case SvgColourType::Bgr:
            if ((nullptr == pBgra) || (nBgraLength != 3)) throw SvgException(std::format("3 values are required for bgr with {}, not {}", SvgColourTypeToString(enumSvgColourType).c_str(), nBgraLength));
            m_pBgra = std::move(pBgra);
            m_nBgraLength = nBgraLength;
            break;

        case SvgColourType::BgrA:
            if ((nullptr == pBgra) || (nBgraLength != 4)) throw SvgException(std::format("4 values are required for bgr with {}, not {}", SvgColourTypeToString(enumSvgColourType).c_str(), nBgraLength));
            m_pBgra = std::move(pBgra);
            m_nBgraLength = nBgraLength;
            break;

        case SvgColourType::Hsl:
            if ((nullptr == pHsla) || (nHslaLength != 3)) throw SvgException(std::format("3 values are required for bgr with {}, not {}", SvgColourTypeToString(enumSvgColourType).c_str(), nHslaLength));
            m_pHsla = std::move(pHsla);
            m_nHslaLength = nHslaLength;
            break;

        case SvgColourType::HslA:
            if ((nullptr == pHsla) || (nHslaLength != 4)) throw SvgException(std::format("4 values are required for bgr with {}, not {}", SvgColourTypeToString(enumSvgColourType).c_str(), nHslaLength));
            m_pHsla = std::move(pHsla);
            m_nHslaLength = nHslaLength;
            break;

        // none is a legitimate value for SvgColour.  There's no data with it.
        case SvgColourType::None: break;

        case SvgColourType::Gradient:
            throw SvgException("SvgColour does not support SvgColourType {}, use SvgPaint", SvgColourTypeToString(enumSvgColourType).c_str());
            break;

        default: throw SvgException(std::format("Unrecognized SvgColourType {}!", SvgColourTypeToString(enumSvgColourType).c_str()));
        }
    }

    SvgColour::SvgColour(SvgColourType svgColourType, int nColourIndex, std::unique_ptr<uint8_t[]> pBgra, unsigned nBgraLength, std::unique_ptr<float[]> pHsla, unsigned nHslaLength, std::unique_ptr<SvgColour> pFallback):
        SvgColour(svgColourType, nColourIndex, std::move(pBgra), nBgraLength, std::move(pHsla), nHslaLength)
    {
        setFallback(std::move(pFallback));
    }

    void SvgColour::swap(SvgColour &copy) noexcept
    {
        std::swap(m_enumSvgColourType, copy.m_enumSvgColourType);
        std::swap(m_nColourIndex, copy.m_nColourIndex);
        std::swap(m_nBgraLength, copy.m_nBgraLength);
        std::swap(m_pBgra, copy.m_pBgra);
        std::swap(m_nHslaLength, copy.m_nHslaLength);
        std::swap(m_pHsla, copy.m_pHsla);
        std::swap(m_pFallback, copy.m_pFallback);
    }

    std::unique_ptr<SvgColour> SvgColour::clone() const
    {
        std::unique_ptr<uint8_t[]> pBgra = nullptr;
        std::unique_ptr<float[]> pHsla = nullptr;
        if(m_pBgra) {
            pBgra = std::make_unique<uint8_t[]>(m_nBgraLength);
            for(unsigned i = 0; i < m_nBgraLength; i++) pBgra[i] = m_pBgra[i];
        }
        if(m_pHsla) {
            pHsla = std::make_unique<float[]>(m_nHslaLength);
            for(unsigned i = 0; i < m_nHslaLength; i++) pHsla[i] = m_pHsla[i];
        }
        return std::make_unique<SvgColour>(m_enumSvgColourType, m_nColourIndex, std::move(pBgra), m_nBgraLength, std::move(pHsla), m_nHslaLength);
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

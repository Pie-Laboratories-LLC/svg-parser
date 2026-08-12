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

#ifndef SVGCOLOUR_DOT_HPP
#define SVGCOLOUR_DOT_HPP

#include <memory>

#ifndef SVGCOLOURTYPE_DOT_HPP
    #include "svg/SvgColourType.hpp"
#endif
#ifndef SVGEXCEPTION_DOT_HPP
    #include "svg/SvgException.hpp"
#endif
#ifndef DRAW2D_COLOUR_DOT_HPP
    #include "draw2d/Colour.hpp"
#endif

namespace Draw2d {
    class Colour;
}

namespace Draw2d::Svg {

    class SvgColour
    {
    public:
        typedef std::function<Colour(const SvgColour &,float)> SvgColourToColourConvertor;

        SvgColourType getSvgColourType() const { return m_enumSvgColourType; }
        int getColourIndex() const { return m_nColourIndex; }

        uint8_t *getBgrA() const { return (m_enumSvgColourType == SvgColourType::Bgr || m_enumSvgColourType == SvgColourType::BgrA) ? m_pBgra.get() : throw new SvgException("SvgColourType is {}, not rgb(a);", SvgColourTypeToString(m_enumSvgColourType).c_str()); }
        unsigned getBgrALength() const { return (m_enumSvgColourType == SvgColourType::Bgr || m_enumSvgColourType == SvgColourType::BgrA) ? m_nBgraLength : throw new SvgException("SvgColourType is {}, not rgb(a);", SvgColourTypeToString(m_enumSvgColourType).c_str()); }

        float *getHslA() const { return (m_enumSvgColourType == SvgColourType::Hsl || m_enumSvgColourType == SvgColourType::HslA) ? m_pHsla.get() : throw new SvgException("SvgColourType is {}, not hsl(a);", SvgColourTypeToString(m_enumSvgColourType).c_str()); }
        unsigned getHslALength() const { return (m_enumSvgColourType == SvgColourType::Hsl || m_enumSvgColourType == SvgColourType::HslA) ? m_nHslaLength : throw new SvgException("SvgColourType is {}, not rgb(a);", SvgColourTypeToString(m_enumSvgColourType).c_str()); }

        SvgColour() { m_enumSvgColourType = SvgColourType::None; }
        SvgColour(SvgColourType svgColourType, int colourIndex = -1, std::unique_ptr<uint8_t[]> bgra = nullptr, unsigned nBgraLength = -1, std::unique_ptr<float[]> pHsla = nullptr, unsigned nHslaLength = -1);
        SvgColour(SvgColourType svgColourType, int colourIndex, std::unique_ptr<uint8_t[]> bgra, unsigned nBgraLength, std::unique_ptr<float[]> pHsla, unsigned nHslaLength, std::unique_ptr<SvgColour> pFallback);

        void setFallback(std::unique_ptr<SvgColour> pFallback) {
            m_pFallback = std::move(pFallback);
        }

        void setFallback(const SvgColour &fallback) {
            m_pFallback = std::make_unique<SvgColour>(fallback);
        }

        const SvgColour *getFallback() const {
            return m_pFallback.get();
        }

        SvgColour(const SvgColour &copy):
            m_enumSvgColourType(copy.m_enumSvgColourType)
           ,m_nColourIndex(copy.m_nColourIndex)
           ,m_nBgraLength(copy.m_nBgraLength)
           ,m_nHslaLength(copy.m_nHslaLength)
           ,m_pFallback(copy.m_pFallback ? std::make_unique<SvgColour>(*copy.m_pFallback) : nullptr)
        {
            if(copy.m_pBgra) {
                m_pBgra = std::make_unique<uint8_t[]>(m_nBgraLength);
                std::copy(copy.m_pBgra.get(),copy.m_pBgra.get() + m_nBgraLength, m_pBgra.get());
            }
            if(copy.m_pHsla) {
                m_pHsla = std::make_unique<float[]>(m_nHslaLength);
                std::copy(copy.m_pHsla.get(),copy.m_pHsla.get() + m_nHslaLength, m_pHsla.get());
            }
        }

        void swap(SvgColour &copy) noexcept;

        SvgColour &operator = (SvgColour copy) {
            swap(copy);
            return *this;
        }

        SvgColour(SvgColour &&move) = default;

        bool operator == (const SvgColour &other) const
        {
            if(m_enumSvgColourType != other.m_enumSvgColourType) return false;
            switch(m_enumSvgColourType) {
            case SvgColourType::None:
            case SvgColourType::CurrentColor: return true;

            case SvgColourType::Cpal: return m_nColourIndex == other.m_nColourIndex;

            case SvgColourType::Bgr:
            case SvgColourType::BgrA:
                return m_nBgraLength == other.m_nBgraLength &&
                       std::equal(m_pBgra.get(), m_pBgra.get() + m_nBgraLength, other.m_pBgra.get());

            case SvgColourType::Hsl:
            case SvgColourType::HslA:
                return m_nHslaLength == other.m_nHslaLength &&
                       std::equal(m_pHsla.get(), m_pHsla.get() + m_nHslaLength, other.m_pHsla.get());

            default: throw SvgException("Invalid illegal unsupported/unknown unhandled unrecognized garbage SvgColourType {}", SvgColourTypeToString(m_enumSvgColourType).c_str());
            }
            return true;
        }

        bool operator != (const SvgColour &other) const
        {
            return !(*this == other);
        }

        std::unique_ptr<SvgColour> clone() const;

    private:
        SvgColourType m_enumSvgColourType = SvgColourType::NotAnSvgColourType;
        int m_nColourIndex = -1;
        std::unique_ptr<uint8_t[]> m_pBgra = nullptr;
        unsigned m_nBgraLength = -1;
        std::unique_ptr<float[]> m_pHsla = nullptr;
        unsigned m_nHslaLength = -1;
        std::unique_ptr<SvgColour> m_pFallback = nullptr;

        void __doCopy(const SvgColour &copy)
        {
            m_enumSvgColourType = copy.m_enumSvgColourType;
            m_nColourIndex = copy.m_nColourIndex;
            m_nBgraLength = copy.m_nBgraLength;
            m_pBgra = std::make_unique<uint8_t[]>(m_nBgraLength);
            std::copy(copy.m_pBgra.get(), copy.m_pBgra.get() + copy.m_nBgraLength, m_pBgra.get());
        }
    }; // class SvgColour

    Colour hslaToRgba(float fH, float fS, float fL, float fA = 1.0f);
} // namespace Draw2d::Svg

#endif  /* SVGCOLOUR_DOT_HPP */

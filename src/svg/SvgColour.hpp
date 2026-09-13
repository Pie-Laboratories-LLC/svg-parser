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

#include <functional>
#include <memory>
#include <array>

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

        const std::optional<std::array<uint8_t,4>> &getBgrA() const { return (m_enumSvgColourType == SvgColourType::Bgr || m_enumSvgColourType == SvgColourType::BgrA) ? m_pBgra : throw SvgException("SvgColourType is {}, not rgb(a);", SvgColourTypeToString(m_enumSvgColourType).c_str()); }

        const std::optional<std::array<float,4>> getHslA() const { return (m_enumSvgColourType == SvgColourType::Hsl || m_enumSvgColourType == SvgColourType::HslA) ? m_pHsla : throw SvgException("SvgColourType is {}, not hsl(a);", SvgColourTypeToString(m_enumSvgColourType).c_str()); }

        SvgColour() { m_enumSvgColourType = SvgColourType::None; }
        SvgColour(SvgColourType svgColourType, int colourIndex, std::optional<std::array<uint8_t,4>> bgra, std::optional<std::array<float,4>> pHsla, std::unique_ptr<SvgColour> pFallback);
        SvgColour(SvgColourType svgColourType, int colourIndex = -1, std::optional<std::array<uint8_t,4>> bgra = std::nullopt, std::optional<std::array<float,4>> pHsla = std::nullopt);

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
           ,m_pBgra(copy.m_pBgra)
           ,m_pHsla(copy.m_pHsla)
           ,m_pFallback(copy.m_pFallback ? std::make_unique<SvgColour>(*copy.m_pFallback) : nullptr)
        {
        }

        void swap(SvgColour &copy) noexcept;

        inline friend void swap(SvgColour &a,SvgColour &b) noexcept {
            a.swap(b);
        }

        SvgColour &operator = (SvgColour copy) {
            swap(copy);
            return *this;
        }

        SvgColour(SvgColour &&move) noexcept = default;
        SvgColour &operator = (SvgColour &&move) noexcept = default;

        bool operator == (const SvgColour &other) const
        {
            if(m_enumSvgColourType != other.m_enumSvgColourType) return false;
            switch(m_enumSvgColourType) {
            case SvgColourType::None:
            case SvgColourType::CurrentColor: return true;

            case SvgColourType::Cpal: return m_nColourIndex == other.m_nColourIndex;

            case SvgColourType::Bgr:
            case SvgColourType::BgrA:
                return m_pBgra.has_value() == other.m_pBgra.has_value()
                    && (!m_pBgra.has_value()
                     || std::equal(m_pBgra.value().begin(), m_pBgra.value().end(), other.m_pBgra.value().begin()));

            case SvgColourType::Hsl:
            case SvgColourType::HslA:
                return m_pHsla.has_value() == other.m_pHsla.has_value()
                    && (!m_pHsla.has_value()
                     || std::equal(m_pHsla.value().begin(), m_pHsla.value().end(), other.m_pHsla.value().begin()));

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
        std::optional<std::array<uint8_t,4>> m_pBgra = std::nullopt;
        std::optional<std::array<float,4>> m_pHsla = std::nullopt;
        std::unique_ptr<SvgColour> m_pFallback = nullptr;
    }; // class SvgColour

    Colour hslaToRgba(float fH, float fS, float fL, float fA = 1.0f);
} // namespace Draw2d::Svg

#endif  /* SVGCOLOUR_DOT_HPP */

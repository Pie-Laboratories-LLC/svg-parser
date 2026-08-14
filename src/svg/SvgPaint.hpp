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

#ifndef DRAW2D_SVG_SVGPAINT_DOT_HPP
#define DRAW2D_SVG_SVGPAINT_DOT_HPP

#ifndef DRAW2D_SVG_SVGCOLOUR_DOT_HPP
    #include "svg/SvgColour.hpp"
#endif

namespace Draw2d {
    template<typename T>
    class Gradient;

    class Colour;
}

namespace Draw2d::Svg {

    class SvgPaint {
    public:
        SvgPaint(SvgColourType enumColourType, const Core::String &strGradientId = "", const SvgColour &colour = SvgColour { SvgColourType::None });
        SvgPaint(const SvgPaint &copy);
        SvgPaint(SvgPaint &&move) noexcept;
        SvgPaint &operator = (SvgPaint &&move) noexcept = delete;
        SvgPaint &operator =(SvgPaint copy);
        ~SvgPaint() = default;

        void swap(SvgPaint &copy) noexcept;

        const std::optional<SvgColour> &getFallback() const { return m_fallback; }
        void setFallback(const SvgColour &fallback) {
            m_fallback = fallback;
        }

        SvgColourType getSvgColourType() const { return m_enumSvgColourType; }
        const Core::String &getGradientId() const { return m_enumSvgColourType == SvgColourType::Gradient ? m_strGradientId : throw SvgException("Not a Gradient type {}", SvgColourTypeToString(m_enumSvgColourType).c_str()); }
        const SvgColour &getColour() const { return (m_enumSvgColourType != SvgColourType::Gradient) ? m_colour : throw SvgException("Can't retrieve colour object when SvgColourType is {}", SvgColourTypeToString(m_enumSvgColourType).c_str()); }

        bool operator ==(const SvgPaint &other) const;

    private:
        SvgColourType m_enumSvgColourType = SvgColourType::NotAnSvgColourType;
        Core::String m_strGradientId {};
        SvgColour m_colour { SvgColourType::None };
        std::optional<SvgColour> m_fallback { };
    }; // class SvgPaint

}

#endif  /* DRAW2D_SVG_SVGPAINT_DOT_HPP */

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

#ifndef COLOUR_DOT_HPP
#define COLOUR_DOT_HPP

#include <memory>
#ifndef POLY_DOT_HPP
    #include "draw2d/Poly.hpp"
#endif

namespace Draw2d {

    class Colour
    {
    public:
        float getRed() const { return m_fRed; }
        void setRed(float fRed) { m_fRed = fRed; }
        float getGreen() const { return m_fGreen; }
        void setGreen(float fGreen) { m_fGreen = fGreen; }
        float getBlue() const { return m_fBlue; }
        void setBlue(float fBlue) { m_fBlue = fBlue; }
        float getAlpha() const { return m_fAlpha; }
        void setAlpha(float fAlpha) { m_fAlpha = fAlpha; }

        Colour(float red = 1.0f, float green = 1.0f, float blue = 1.0f, float alpha = 1.0f)
        {
            m_fRed = red;
            m_fGreen = green;
            m_fBlue = blue;
            m_fAlpha = alpha;
        }
        
        Colour(const Colour &copy)
        {
            m_fRed = copy.m_fRed;
            m_fGreen = copy.m_fGreen;
            m_fBlue = copy.m_fBlue;
            m_fAlpha = copy.m_fAlpha;
        }

        Colour(const Colour &copy,float fNewAlpha)
        {
            m_fRed = copy.m_fRed;
            m_fGreen = copy.m_fGreen;
            m_fBlue = copy.m_fBlue;
            m_fAlpha = fNewAlpha;
        }

        bool operator == (const Colour &compare) const
        {
            return std::abs(m_fRed - compare.m_fRed) < Poly::TOLERANCE
                && std::abs(m_fGreen - compare.m_fGreen) < Poly::TOLERANCE
                && std::abs(m_fBlue - compare.m_fBlue) < Poly::TOLERANCE
                && std::abs(m_fAlpha - compare.m_fAlpha) < Poly::TOLERANCE;
        }

        auto operator <=> (const Colour &other) const
        {
            if(std::abs(m_fAlpha - other.m_fAlpha) > Poly::TOLERANCE) {
                return m_fAlpha <=> other.m_fAlpha;
            }
            if(std::abs(m_fRed - other.m_fRed) > Poly::TOLERANCE) {
                return m_fRed <=> other.m_fRed;
            }
            if(std::abs(m_fGreen - other.m_fGreen) > Poly::TOLERANCE) {
                return m_fGreen <=> other.m_fGreen;
            }
            if(std::abs(m_fBlue - other.m_fBlue) > Poly::TOLERANCE) {
                return m_fBlue <=> other.m_fBlue;
            }
            return std::partial_ordering::equivalent;;
        }

        void set(float fRed, float fGreen, float fBlue, float fAlpha = 1.0f)
        {
            m_fRed = fRed;
            m_fGreen = fGreen;
            m_fBlue = fBlue;
            m_fAlpha = fAlpha;
        }

        std::unique_ptr<Colour> clone() const {
            return std::make_unique<Colour>(m_fRed, m_fGreen, m_fBlue, m_fAlpha);
        }

    private:
        float m_fRed = 0.0f;
        float m_fGreen = 0.0f;
        float m_fBlue = 0.0f;
        float m_fAlpha = 1.0f;
    }; // class Colour

} // namespace Draw2d

#endif  /* COLOUR_DOT_HPP */

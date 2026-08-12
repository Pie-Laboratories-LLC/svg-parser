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

#ifndef STOP_DOT_HPP
#define STOP_DOT_HPP

#include <memory>

#ifndef POLY_DOT_HPP
    #include "draw2d/Poly.hpp"
#endif
#ifndef CORE_STL_DOT_HPP
    #include "core/stl.hpp"
#endif

namespace Draw2d {

    template<typename T>
    class Stop
    {
    public:
        using ColourType = T;

        float getOffset() const { return m_fOffset; } 
        const T &getColour() const { return m_colour; }
        float getOpacity() const { return m_fOpacity; }

        Stop(float offset, const T &colour, float opacity)
        {
            m_fOffset = offset;
            m_fOpacity = opacity;
            m_colour = colour;
        }

        Stop(float offset, const T &colour)
        {
            m_fOffset = offset;
            m_colour = colour;
        }

        std::partial_ordering operator<=>(const Stop<T> &other) const {
            // check for floating point inaccuracy.
            auto result = m_colour <=> other.m_colour;
            if(result != 0) return result;
            if(std::abs(other.m_fOffset - m_fOffset) < Poly::TOLERANCE) return std::partial_ordering::equivalent;
            return m_fOffset <=> other.m_fOffset; 
        }

        bool operator==(const Stop<T> &other) const {
            return std::abs(other.m_fOffset - m_fOffset) < Poly::TOLERANCE;
        }

    private:
        float m_fOffset = 0.0f;
        T m_colour { };
        float m_fOpacity = 1.0f;
    };

} // namespace Draw2d

#endif  /* STOP_DOT_HPP */

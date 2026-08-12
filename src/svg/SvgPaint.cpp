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

#include "svg/SvgPaint.hpp"

namespace Draw2d::Svg {

    SvgPaint::SvgPaint(SvgColourType enumSvgColourType, const Core::String &cstrGradientId, const SvgColour &colour):
        m_enumSvgColourType(enumSvgColourType),
        m_strGradientId(cstrGradientId),
        m_colour(colour)
    {
        // all validations, ensure they passed the correct bits and bobs for the colour type for an SvgPaint
        if(!Core::String::IsEmpty(cstrGradientId) && enumSvgColourType != SvgColourType::Gradient) throw SvgException("You can only supply a gradient if the gradient type is Gradient, not {}", SvgColourTypeToString(enumSvgColourType).c_str());
        if(Core::String::IsEmpty(cstrGradientId) && enumSvgColourType == SvgColourType::Gradient) throw SvgException("You must supply a gradient if the gradient type is Gradient");
    }

    SvgPaint::SvgPaint(const SvgPaint &copy):
        m_enumSvgColourType(copy.m_enumSvgColourType),
        m_strGradientId(copy.m_strGradientId),
        m_colour(copy.m_colour)
    {
    }

    SvgPaint::SvgPaint(SvgPaint &&move) noexcept:
        m_enumSvgColourType(move.m_enumSvgColourType),
        m_strGradientId(move.m_strGradientId),
        m_colour(std::move(move.m_colour)),
        m_fallback(std::move(move.m_fallback))
    {
    }

    SvgPaint &SvgPaint::operator =(SvgPaint copy)
    {
        swap(copy);
        return *this;
    }

    void SvgPaint::swap(SvgPaint &copy) noexcept
    {
        std::swap(m_enumSvgColourType,copy.m_enumSvgColourType);
        std::swap(m_strGradientId,copy.m_strGradientId);
        std::swap(m_colour,copy.m_colour);
        std::swap(m_fallback,copy.m_fallback);
    }

    bool SvgPaint::operator ==(const SvgPaint &other) const
    {
        if(m_enumSvgColourType != other.m_enumSvgColourType) return false;
        if (m_fallback != other.m_fallback) return false;
        switch(m_enumSvgColourType) {
        case SvgColourType::Gradient:
            return m_strGradientId == other.m_strGradientId;

        case SvgColourType::None:
            return true;

        case SvgColourType::CurrentColor:
        case SvgColourType::Cpal:
        case SvgColourType::Bgr:
        case SvgColourType::BgrA:
        case SvgColourType::Hsl:
        case SvgColourType::HslA:
            return m_colour == other.m_colour;

        default: throw SvgException("Logic error, unhandled type {}",SvgColourTypeToString(m_enumSvgColourType).c_str());
        }
    }
}

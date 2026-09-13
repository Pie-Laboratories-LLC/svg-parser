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

#ifndef DRAW2D_SVG_TEXT_DOT_HPP
#define DRAW2D_SVG_TEXT_DOT_HPP

#ifndef DRAW2D_SVG_SVGENTITY_DOT_HPP
    #include "svg/SvgEntity.hpp"
#endif
#ifndef DRAW2D_SVG_LENGTHADJUST_DOT_HPP
    #include "svg/LengthAdjust.hpp"
#endif

namespace Draw2d::Svg {

    struct SvgTextParams : public SvgEntityParams {
        std::vector<float> x { };
        std::vector<float> y { };
        std::vector<float> dx { };
        std::vector<float> dy { };
        std::vector<float> rotate {};
        LengthAdjust lengthAdjust = LengthAdjust::Spacing;
        std::optional<float> textLength = std::nullopt;
        bool textLengthPercentage = false;
    };

    class Text : public SvgEntity {
    public:
        Text(SvgTextParams svgTextParams):
            SvgEntity(static_cast<SvgEntityParams &&>(svgTextParams))
          , m_x(std::move(svgTextParams.x))
          , m_y(std::move(svgTextParams.y))
          , m_dx(std::move(svgTextParams.dx))
          , m_dy(std::move(svgTextParams.dy))
          , m_rotate(std::move(svgTextParams.rotate))
          , m_enumLengthAdjust(svgTextParams.lengthAdjust)
          , m_textLength(svgTextParams.textLength)
          , m_bTextLengthPercentage(svgTextParams.textLengthPercentage)
        {}

        const std::vector<float> getX() const { return m_x; }
        const std::vector<float> getY() const { return m_y; }
        const std::vector<float> getDx() const { return m_dx; }
        const std::vector<float> getDy() const { return m_dy; }
        const std::vector<float> getRotate() const { return m_rotate; }
        LengthAdjust getLengthAdjust() const { return m_enumLengthAdjust; }
        std::optional<float> getTextLength() const { return m_textLength; }

        virtual const char * const getType() const { return "Text"; }

    private:
        std::vector<float> m_x { };
        std::vector<float> m_y { };
        std::vector<float> m_dx { };
        std::vector<float> m_dy { };
        std::vector<float> m_rotate { };
        LengthAdjust m_enumLengthAdjust = LengthAdjust::Spacing;
        std::optional<float> m_textLength = std::nullopt;
        bool m_bTextLengthPercentage = false;
    };

}

#endif  /* DRAW2D_SVG_TEXT_DOT_HPP */

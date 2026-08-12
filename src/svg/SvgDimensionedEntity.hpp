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

#ifndef DRAW2D_SVG_SVGDIMENSIONEDENTITY_DOT_HPP
#define DRAW2D_SVG_SVGDIMENSIONEDENTITY_DOT_HPP

#ifndef DRAW2D_SVG_DIMENSION_DOT_HPP
    #include "svg/Dimension.hpp"
#endif

namespace Draw2d::Svg {

    struct SvgDimensionedParams {
        std::optional<Dimension> x = {};
        std::optional<Dimension> y = {};
        std::optional<Dimension> width = {};
        std::optional<Dimension> height = {};
    };

    class SvgDimensionedEntity {
    public:
        SvgDimensionedEntity(SvgDimensionedParams svgDimensionedParams);

        std::optional<Dimension> getX() const  { return m_x; }
        std::optional<Dimension> getY() const  { return m_y; }
        std::optional<Dimension> getWidth() const  { return m_width; }
        std::optional<Dimension> getHeight() const  { return m_height; }

    private:
        std::optional<Dimension> m_x = {};
        std::optional<Dimension> m_y = {};
        std::optional<Dimension> m_width = {};
        std::optional<Dimension> m_height = {};
    };
}

#endif  /* DRAW2D_SVG_SVGDIMENSIONEDENTITY_DOT_HPP */

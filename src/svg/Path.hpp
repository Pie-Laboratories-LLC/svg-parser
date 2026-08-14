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

#ifndef DRAW2D_SVG_PATH_DOT_HPP
#define DRAW2D_SVG_PATH_DOT_HPP

#include <vector>
#ifndef DRAW2D_SVG_PATHMOVE_DOT_HPP
    #include "svg/PathMove.hpp"
#endif
#ifndef SVGENTITY_DOT_HPP
    #include "svg/SvgEntity.hpp"
#endif
#ifndef DRAW2D_SVG_MATRIXMATH_DOT_HPP
    #include "svg/MatrixMath.hpp"
#endif

namespace Draw2d::Svg {

    struct SvgPathParams : public SvgEntityParams {
        std::vector<float> points { };
        std::vector<PathMove> pathMoves { };
    };

    class Path: public SvgEntity
    {
    public:
        std::vector<float> getPoints() const { return m_points; }
        std::vector<PathMove> getPathMoves() const { return m_pathMoves; }

        Path(SvgPathParams svgPathParams);
        virtual ~Path() = default;

        Path(const Path &copy);
        Path(Path &&copy);

        /// standard case - gets the x value from a pair of points x, y at pointIndex and applies the supplied transform.
        /// @note it retrieves the y to properly calculate x via matrix multiplication
        float getX(unsigned index, const std::optional<const std::array<float,6>> &matrix) const
        {
            float x = m_points[index];
            float y = m_points[index + 1];
            return (!matrix.has_value()) ? x : multiply6X(matrix.value(), x, y);
        }

        /// standard case - retrieves the y value from a pair of points x, y at pointIndex and applies the supplied transform
        /// @note it retrieves the x to properly calculate x via matrix multiplication
        float getY(unsigned index, const std::optional<std::array<float,6>> &matrix) const
        {
            float x = m_points[index];
            float y = m_points[index + 1];
            return (!matrix.has_value()) ? y : multiply6Y(matrix.value(), x, y);
        }

        /// this variant is for HorizontalLineTo; get the x from points, use y from the last known value
        /// @note the x value is retrieved from index, the yValue is supplied
        float getX(unsigned index, float yValue, const std::optional<std::array<float,6>> &matrix) const
        {
            float x = m_points[index];
            float y = yValue;
            return (!matrix.has_value()) ? x : (x * matrix.value()[0] + y * matrix.value()[2] + matrix.value()[4]);
        }

        /// this variant is for VerticalLineTo; get the x from points, use x from the last known value
        /// @note the y value is retrieved from index, the xValue is supplied
        float getY(unsigned index, float xValue, const std::optional<std::array<float,6>> &matrix) const
        {
            float x = xValue;
            float y = m_points[index];
            return (!matrix.has_value()) ? y : (x * matrix.value()[1] + y * matrix.value()[3] + matrix.value()[5]);
        }

        virtual const char * const getType() const override { return "Path"; }

    private:
        std::vector<float> m_points { };
        std::vector<PathMove> m_pathMoves { };

        void __checkPoint(float x, float y)
        {
            if (x < m_upperLeft.getX()) m_upperLeft.setX(x);
            if (y < m_upperLeft.getY()) m_upperLeft.setY(y);
            if (x > m_lowerRight.getX()) m_lowerRight.setX(x);
            if (y > m_lowerRight.getY()) m_lowerRight.setY(y);
        }
    };

} // namespace Draw2d::Svg

#endif  /* DRAW2D_SVG_PATH_DOT_HPP */

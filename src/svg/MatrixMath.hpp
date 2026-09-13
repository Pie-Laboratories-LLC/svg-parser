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

#ifndef DRAW2D_SVG_MATRIXMATH_DOT_HPP
#define DRAW2D_SVG_MATRIXMATH_DOT_HPP

#include <array>

namespace Draw2d::Svg {

    template<typename T>
    std::array<T,6> multiply6s(const std::array<T,6> &left, const std::array<T,6> &right)
    {
        std::array<T,6> combinedMatrix { };

        combinedMatrix[0] = left[0] * right[0] + left[2] * right[1] + left[4] * 0;
        combinedMatrix[1] = left[1] * right[0] + left[3] * right[1] + left[5] * 0;
        combinedMatrix[2] = left[0] * right[2] + left[2] * right[3] + left[4] * 0;
        combinedMatrix[3] = left[1] * right[2] + left[3] * right[3] + left[5] * 0;
        combinedMatrix[4] = left[0] * right[4] + left[2] * right[5] + left[4] * 1;
        combinedMatrix[5] = left[1] * right[4] + left[3] * right[5] + left[5] * 1;

        return combinedMatrix;
    }

    template<typename T>
    void multiply6s(std::array<T,6> &combinedMatrix, const std::array<T,6> &left, const std::array<T,6> &right)
    {
        combinedMatrix[0] = left[0] * right[0] + left[2] * right[1] + left[4] * 0;
        combinedMatrix[1] = left[1] * right[0] + left[3] * right[1] + left[5] * 0;
        combinedMatrix[2] = left[0] * right[2] + left[2] * right[3] + left[4] * 0;
        combinedMatrix[3] = left[1] * right[2] + left[3] * right[3] + left[5] * 0;
        combinedMatrix[4] = left[0] * right[4] + left[2] * right[5] + left[4] * 1;
        combinedMatrix[5] = left[1] * right[4] + left[3] * right[5] + left[5] * 1;
    }

    /// calculates the x value of an x, y pair given an svg 6-element matrix
    template<typename T>
    T multiply6X(const std::array<T,6> &left, T x, T y) {
        T originalX = x;
        T originalY = y;
        return originalX * left[0] + originalY * left[2] + left[4];
    }

    /// calculates the y value of an x, y pair given an svg 6-element matrix
    template<typename T>
    T multiply6Y(const std::array<float,6> &left, T x, T y) {
        T originalX = x;
        T originalY = y;
        return originalX * left[1] + originalY * left[3] + left[5];
    }

    /// calculates the y value of a 2d vector given an svg 6-element matrix
    template<typename T>
    T multiply4X(const std::array<T,6> &cpLeft, T x, T y) {
        T originalX = x;
        T originalY = y;
        return originalX * cpLeft[0] + originalY * cpLeft[2];
    }

    template<typename T>
    T multiply4Y(const std::array<T,6> &cpLeft, T x, T y) {
        T originalX = x;
        T originalY = y;
        return originalX * cpLeft[1] + originalY * cpLeft[3];
    }

    template<typename T>
    void multiply6XY(const std::array<float,6> left, T &x, T &y) {
        T originalX = x;
        T originalY = y;
        x = originalX * left[0] + originalY * left[2] + left[4];
        y = originalX * left[1] + originalY * left[3] + left[5];
    }

    template<typename T>
    std::array<T,6> duplicateMatrix6(const std::array<T,6> &matrix)
    {
        // TODO this method is now officially dumb
        std::array<T,6> duplicate { };
        std::copy(matrix.begin(), matrix.end(), duplicate.begin());
        return duplicate;
    }

}

#endif  /* DRAW2D_SVG_MATRIXMATH_DOT_HPP */

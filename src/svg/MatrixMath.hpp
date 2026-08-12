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

#include <memory>

namespace Draw2d::Svg {

    template<typename T>
    std::unique_ptr<T[]> multiply6s(const T *pLeft, const T *pRight)
    {
        std::unique_ptr<float[]> combinedMatrix = std::make_unique<float[]>(6);

        combinedMatrix[0] = pLeft[0] * pRight[0] + pLeft[2] * pRight[1] + pLeft[4] * 0;
        combinedMatrix[1] = pLeft[1] * pRight[0] + pLeft[3] * pRight[1] + pLeft[5] * 0;
        combinedMatrix[2] = pLeft[0] * pRight[2] + pLeft[2] * pRight[3] + pLeft[4] * 0;
        combinedMatrix[3] = pLeft[1] * pRight[2] + pLeft[3] * pRight[3] + pLeft[5] * 0;
        combinedMatrix[4] = pLeft[0] * pRight[4] + pLeft[2] * pRight[5] + pLeft[4] * 1;
        combinedMatrix[5] = pLeft[1] * pRight[4] + pLeft[3] * pRight[5] + pLeft[5] * 1;

        return combinedMatrix;
    }

    template<typename T>
    void multiply6s(T *pCombinedMatrix, const T *pLeft, const T *pRight)
    {
        pCombinedMatrix[0] = pLeft[0] * pRight[0] + pLeft[2] * pRight[1] + pLeft[4] * 0;
        pCombinedMatrix[1] = pLeft[1] * pRight[0] + pLeft[3] * pRight[1] + pLeft[5] * 0;
        pCombinedMatrix[2] = pLeft[0] * pRight[2] + pLeft[2] * pRight[3] + pLeft[4] * 0;
        pCombinedMatrix[3] = pLeft[1] * pRight[2] + pLeft[3] * pRight[3] + pLeft[5] * 0;
        pCombinedMatrix[4] = pLeft[0] * pRight[4] + pLeft[2] * pRight[5] + pLeft[4] * 1;
        pCombinedMatrix[5] = pLeft[1] * pRight[4] + pLeft[3] * pRight[5] + pLeft[5] * 1;
    }

    /// calculates the x value of an x, y pair given an svg 6-element matrix
    template<typename T>
    T multiply6X(const T *cpLeft, T x, T y) {
        T originalX = x;
        T originalY = y;
        return originalX * cpLeft[0] + originalY * cpLeft[2] + cpLeft[4];
    }

    /// calculates the y value of an x, y pair given an svg 6-element matrix
    template<typename T>
    T multiply6Y(const T *cpLeft, T x, T y) {
        T originalX = x;
        T originalY = y;
        return originalX * cpLeft[1] + originalY * cpLeft[3] + cpLeft[5];
    }

    /// calculates the y value of a 2d vector given an svg 6-element matrix
    template<typename T>
    T multiply4X(const T *cpLeft, T x, T y) {
        T originalX = x;
        T originalY = y;
        return originalX * cpLeft[0] + originalY * cpLeft[2];
    }

    template<typename T>
    T multiply4Y(const T *cpLeft, T x, T y) {
        T originalX = x;
        T originalY = y;
        return originalX * cpLeft[1] + originalY * cpLeft[3];
    }

    template<typename T>
    void multiply6XY(const T *cpLeft, T &x, T &y) {
        T originalX = x;
        T originalY = y;
        x = originalX * cpLeft[0] + originalY * cpLeft[2] + cpLeft[4];
        y = originalX * cpLeft[1] + originalY * cpLeft[3] + cpLeft[5];
    }

    template<typename T>
    std::unique_ptr<T[]> duplicateMatrix6(const T *pMatrix)
    {
        std::unique_ptr<float[]> pDuplicate = std::make_unique<float[]>(6);
        std::copy(pMatrix, pMatrix + 6, pDuplicate.get());
        return pDuplicate;
    }

}

#endif  /* DRAW2D_SVG_MATRIXMATH_DOT_HPP */

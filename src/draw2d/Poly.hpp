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

#ifndef POLY_DOT_HPP
#define POLY_DOT_HPP

#include <cmath>
#include <vector>
#include <format>
#include <stdexcept>

namespace Draw2d {

    class Poly {
    public:
        static const double POLYNOMIAL_TOLERANCE; // = 1e-6;
        static const double TOLERANCE; //            = 1e-12;

        template <typename TargetType>
        requires std::is_arithmetic_v<TargetType>
        static std::vector<TargetType> getPolynomialRoots(std::vector<TargetType> arr) {

            unsigned degree  = arr.size() - 1;
            unsigned n       = degree;
            for (unsigned i = 0; i <= degree; i++) {
                if (std::abs(arr[i]) <= TOLERANCE) degree--; else break;
            }

            switch (degree) {
            case 1: return getLinearRoots(arr[n], arr[n - 1]); break;
            case 2: return getQuadraticRoots(arr[n], arr[n - 1], arr[n - 2]); break;
            case 3: return getCubicRoots(arr[n], arr[n - 1], arr[n - 2], arr[n - 3]); break;
            default: /*return new double[] { };*/throw std::runtime_error(std::format("Invalid degree {} from {}", degree, arr.size()));/**/
            }
        }

        template <typename TargetType>
        requires std::is_arithmetic_v<TargetType>
        static std::vector<TargetType> getLinearRoots(TargetType C0,TargetType C1) {
            if (std::abs(C1) > TOLERANCE) return std::vector<TargetType> { -C0 / C1 };
            return std::vector<TargetType> {};
        }

        template <typename TargetType>
        requires std::is_arithmetic_v<TargetType>
        static std::vector<TargetType> getQuadraticRoots(TargetType C0, TargetType C1, TargetType C2) {
            TargetType a = C2;
            TargetType b = C1 / a;
            TargetType c = C0 / a;
            TargetType d = b * b - 4 * c;

            if (d > 0) {
                TargetType e = std::sqrt(d);
  
                return std::vector<TargetType> { 0.5 * (-b + e), 0.5 * (-b - e) };
            }
            if (d == 0) return std::vector<TargetType> { 0.5 * -b };

            return std::vector<TargetType> {};
        }

        template <typename TargetType>
        requires std::is_arithmetic_v<TargetType>
        static std::vector<TargetType> getCubicRoots(TargetType C0, TargetType C1, TargetType C2, TargetType C3) {
            TargetType c3 = C3;
            TargetType c2 = C2 / c3;
            TargetType c1 = C1 / c3;
            TargetType c0 = C0 / c3;
            TargetType a       = (3 * c1 - c2 * c2) / 3;
            TargetType b       = (2 * c2 * c2 * c2 - 9 * c1 * c2 + 27 * c0) / 27;
            TargetType offset  = c2 / 3;
            TargetType discrim = b * b / 4 + a * a * a / 27;
            TargetType halfB   = b / 2;
        
            if (std::abs(discrim) <= POLYNOMIAL_TOLERANCE) discrim = 0;
        
            if (discrim > 0) {
                TargetType e = std::sqrt(discrim);
                TargetType root = std::cbrt(-halfB + e) + std::cbrt(-halfB - e);
                return std::vector<TargetType> { root - offset };
            }
            if (discrim < 0) {
                TargetType distance = std::sqrt(-a/3);
                TargetType angle    = std::atan2(std::sqrt(-discrim), -halfB) / 3;
                TargetType cos      = std::cos(angle);
                TargetType sin      = std::sin(angle);
                TargetType sqrt3    = std::sqrt(3);
        
                return std::vector<TargetType> {
                    2 * distance * cos - offset,
                    -distance * (cos + sqrt3 * sin) - offset,
                    -distance * (cos - sqrt3 * sin) - offset
                };
            }
            // discrim == 0: a repeated root case
            TargetType tmp = std::cbrt(-halfB);
            return std::vector<TargetType> {
                2 * tmp - offset,
                // really should return next root twice, but we return only one
                -tmp - offset
            };
        } // std::vector<TargetType> getCubicRoots(double C0, double C1, double C2, double C3)
    }; // class Poly

}

#endif  /* POLY_DOT_HPP */

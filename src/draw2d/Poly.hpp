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

#include <algorithm>
#include <cmath>
#include <vector>

#ifndef DRAW2D_DRAW2DEXCEPTION_DOT_HPP
    #include "draw2d/Draw2dException.hpp"
#endif

namespace Draw2d {

    class Poly {
    public:
        static const double POLYNOMIAL_TOLERANCE; // = 1e-6;
        static const double TOLERANCE; //            = 1e-12;
        // le discriminant d'une cubique est nul à cette fraction près de son plus grand terme
        static constexpr double DISCRIMINANT_RELATIF = 1e-12;
        // un coefficient de tête est nul à cette fraction près du plus grand des autres
        //  coefficients non constants
        static constexpr double COEFFICIENT_DE_TÊTE_RELATIF = 1e-6;

        template <typename TargetType>
        requires std::is_arithmetic_v<TargetType>
        // les racines réelles d'un polynôme, coefficients du plus haut degré au
        // plus bas. Les coefficients de tête nuls (à TOLERANCE près) abaissent
        // le degré; le dernier, la constante, n'est jamais retiré.
        // Nul aussi, un coefficient de tête minuscule devant les autres: une
        // cubique d'Illustrator qui est en fait une quadratique a un t^3 de
        // l'ordre de 1e-7 des autres termes, et la rendre unitaire (diviser par
        // lui) noie ses racines dans les arrondis. Sur [0,1], le seul intervalle
        // qui intéresse une courbe de Bézier, le retirer déplace le polynôme
        // d'au plus ce coefficient; la racine perdue est vers t ~ 1e6.
        // Degré 0: une constante -- non nulle, aucune racine; nulle partout, la
        // courbe est couchée sur la droite (un bord horizontal sur la ligne de
        // balayage): aucune intersection isolée, comme dans l'original
        static std::vector<TargetType> getPolynomialRoots(std::vector<TargetType> arr) {
            if (arr.empty()) return {};

            const size_t n = arr.size() - 1;
            size_t premier = 0;
            while (premier < n) {
                TargetType plusGrandAutre = 0;
                for (size_t i = premier + 1; i < n; i++) plusGrandAutre = std::max(plusGrandAutre, std::abs(arr[i]));
                const TargetType tête = std::abs(arr[premier]);
                if (tête > TOLERANCE && tête > COEFFICIENT_DE_TÊTE_RELATIF * plusGrandAutre) break;
                premier++;
            }
            const size_t degree = n - premier;

            switch (degree) {
            case 0: return {};
            case 1: return getLinearRoots(arr[n], arr[n - 1]);
            case 2: return getQuadraticRoots(arr[n], arr[n - 1], arr[n - 2]);
            case 3: return getCubicRoots(arr[n], arr[n - 1], arr[n - 2], arr[n - 3]);
            default: throw Draw2dException("Unsupported degree {} ({} coefficients)", degree, arr.size());
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

            // nul seulement aux arrondis de ses deux termes près: une tolérance
            //  absolue (c'était 1e-6) dépend de l'échelle de la courbe, et fondait
            //  les deux racines proches et distinctes d'une courbe qui frôle une
            //  ligne de balayage en une seule, fausse -- un nombre impair de croisements
            const TargetType échelleDiscrim = std::max(halfB * halfB, std::abs(a * a * a / 27));
            if (std::abs(discrim) <= DISCRIMINANT_RELATIF * échelleDiscrim) discrim = 0;

            std::vector<TargetType> roots;
            if (discrim > 0) {
                TargetType e = std::sqrt(discrim);
                TargetType root = std::cbrt(-halfB + e) + std::cbrt(-halfB - e);
                roots = { root - offset };
            }
            else if (discrim < 0) {
                TargetType distance = std::sqrt(-a/3);
                TargetType angle    = std::atan2(std::sqrt(-discrim), -halfB) / 3;
                TargetType cos      = std::cos(angle);
                TargetType sin      = std::sin(angle);
                TargetType sqrt3    = std::sqrt(3);

                roots = {
                    2 * distance * cos - offset,
                    -distance * (cos + sqrt3 * sin) - offset,
                    -distance * (cos - sqrt3 * sin) - offset
                };
            }
            else {
                // une racine double, rendue deux fois: comme croisement, c'est une
                //  paire qui s'annule
                TargetType tmp = std::cbrt(-halfB);
                roots = { 2 * tmp - offset, -tmp - offset, -tmp - offset };
            }

            // Cardan et la forme trigonométrique perdent des chiffres quand les
            //  racines sont proches: quelques pas de Newton sur la cubique même,
            //  gardés tant qu'ils l'améliorent
            for (TargetType &racine : roots) {
                auto valeur = [&](TargetType t) { return ((t + c2) * t + c1) * t + c0; };
                TargetType fRacine = valeur(racine);
                for (int pas = 0; pas < 4 && fRacine != 0; pas++) {
                    const TargetType dérivée = (3 * racine + 2 * c2) * racine + c1;
                    if (dérivée == 0) break;
                    const TargetType suivante = racine - fRacine / dérivée;
                    const TargetType fSuivante = valeur(suivante);
                    if (!(std::abs(fSuivante) < std::abs(fRacine))) break;
                    racine = suivante;
                    fRacine = fSuivante;
                }
            }
            return roots;
        } // std::vector<TargetType> getCubicRoots(double C0, double C1, double C2, double C3)
    }; // class Poly

}

#endif  /* POLY_DOT_HPP */

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

#include "svg/BlendColours.hpp"

#include <algorithm>
#include <cmath>

namespace Draw2d::Svg {

    namespace {

        // the separable modes, channel by channel (cb backdrop, cs source)
        float Multiply(float cb, float cs) { return cb * cs; }
        float Screen(float cb, float cs) { return cb + cs - cb * cs; }
        float HardLight(float cb, float cs) { return cs <= 0.5f ? Multiply(cb, 2.0f * cs) : Screen(cb, 2.0f * cs - 1.0f); }

        float ColorDodge(float cb, float cs)
        {
            if(cb <= 0.0f) return 0.0f;
            if(cs >= 1.0f) return 1.0f;
            return std::min(1.0f, cb / (1.0f - cs));
        }

        float ColorBurn(float cb, float cs)
        {
            if(cb >= 1.0f) return 1.0f;
            if(cs <= 0.0f) return 0.0f;
            return 1.0f - std::min(1.0f, (1.0f - cb) / cs);
        }

        float SoftLight(float cb, float cs)
        {
            if(cs <= 0.5f) return cb - (1.0f - 2.0f * cs) * cb * (1.0f - cb);
            const float fD = cb <= 0.25f ? ((16.0f * cb - 12.0f) * cb + 4.0f) * cb : std::sqrt(cb);
            return cb + (2.0f * cs - 1.0f) * (fD - cb);
        }

        float Separable(BlendMode enumBlendMode, float cb, float cs)
        {
            switch(enumBlendMode) {
                case BlendMode::Multiply: return Multiply(cb, cs);
                case BlendMode::Screen: return Screen(cb, cs);
                case BlendMode::Overlay: return HardLight(cs, cb);
                case BlendMode::Darken: return std::min(cb, cs);
                case BlendMode::Lighten: return std::max(cb, cs);
                case BlendMode::ColorDodge: return ColorDodge(cb, cs);
                case BlendMode::ColorBurn: return ColorBurn(cb, cs);
                case BlendMode::HardLight: return HardLight(cb, cs);
                case BlendMode::SoftLight: return SoftLight(cb, cs);
                case BlendMode::Difference: return std::fabs(cb - cs);
                case BlendMode::Exclusion: return cb + cs - 2.0f * cb * cs;
                default: return cs;
            }
        }

        // the non-separable modes work on whole colours
        typedef std::array<float,3> Rgb;

        float Lum(const Rgb &c) { return 0.3f * c[0] + 0.59f * c[1] + 0.11f * c[2]; }

        Rgb ClipColor(Rgb c)
        {
            const float fL = Lum(c);
            const float fN = std::min({ c[0], c[1], c[2] });
            const float fX = std::max({ c[0], c[1], c[2] });
            for(float &f : c) {
                if(fN < 0.0f) f = fL + (f - fL) * fL / (fL - fN);
                if(fX > 1.0f) f = fL + (f - fL) * (1.0f - fL) / (fX - fL);
            }
            return c;
        }

        Rgb SetLum(const Rgb &c, float fL)
        {
            const float fD = fL - Lum(c);
            return ClipColor({ c[0] + fD, c[1] + fD, c[2] + fD });
        }

        float Sat(const Rgb &c) { return std::max({ c[0], c[1], c[2] }) - std::min({ c[0], c[1], c[2] }); }

        Rgb SetSat(const Rgb &c, float fS)
        {
            // the largest channel becomes s, the smallest 0, the middle one in proportion
            size_t nMax = 0, nMin = 0;
            for(size_t n = 1; n < 3; n++) {
                if(c[n] > c[nMax]) nMax = n;
                if(c[n] < c[nMin]) nMin = n;
            }
            if(nMax == nMin) return { 0.0f, 0.0f, 0.0f };
            const size_t nMid = 3 - nMax - nMin;
            Rgb result {};
            result[nMid] = (c[nMid] - c[nMin]) * fS / (c[nMax] - c[nMin]);
            result[nMax] = fS;
            result[nMin] = 0.0f;
            return result;
        }

    } // namespace

    std::array<float,3> BlendColours(BlendMode enumBlendMode, const std::array<float,3> &backdrop, const std::array<float,3> &source)
    {
        switch(enumBlendMode) {
            case BlendMode::Hue: return SetLum(SetSat(source, Sat(backdrop)), Lum(backdrop));
            case BlendMode::Saturation: return SetLum(SetSat(backdrop, Sat(source)), Lum(backdrop));
            case BlendMode::Color: return SetLum(source, Lum(backdrop));
            case BlendMode::Luminosity: return SetLum(backdrop, Lum(source));
            default: {
                std::array<float,3> result {};
                for(size_t n = 0; n < 3; n++) result[n] = Separable(enumBlendMode, backdrop[n], source[n]);
                return result;
            }
        }
    }

} // namespace Draw2d::Svg

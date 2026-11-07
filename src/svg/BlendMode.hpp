/*
 * Copyright 2026 Pie Laboratories LLC
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

#ifndef DRAW2D_SVG_BLENDMODE_DOT_HPP
#define DRAW2D_SVG_BLENDMODE_DOT_HPP
#include <optional>
#include <string_view>
#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif
namespace Draw2d::Svg {
    enum class BlendMode {
        NotABlendMode = -1,
        Normal,
        Multiply,
        Screen,
        Overlay,
        Darken,
        Lighten,
        ColorDodge,
        ColorBurn,
        HardLight,
        SoftLight,
        Difference,
        Exclusion,
        Hue,
        Saturation,
        Color,
        Luminosity
    };
    // from its CSS name, without throwing: nothing if it isn't one
    std::optional<BlendMode> ChercherBlendMode(std::string_view texte);
    // the same, but an unknown name throws -- listing the valid ones
    BlendMode ParseBlendMode(const Core::String &cstrBlendMode);
    // its CSS name, the one Chercher and Parse read -- without throwing, even for a
    // value outside the enumeration
    Core::String BlendModeToString(BlendMode enumBlendMode);
}
#endif  /* DRAW2D_SVG_BLENDMODE_DOT_HPP */

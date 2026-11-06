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

#ifndef DRAW2D_SVG_BLENDCOLOURS_DOT_HPP
#define DRAW2D_SVG_BLENDCOLOURS_DOT_HPP

#include <array>

#ifndef DRAW2D_SVG_BLENDMODE_DOT_HPP
    #include "svg/BlendMode.hpp"
#endif

namespace Draw2d::Svg {

    /// B(Cb, Cs) from Compositing and Blending Level 1: the colour where an opaque source
    ///  colour (red, green, blue in 0-1, not premultiplied) lands on an opaque backdrop colour.
    ///  The caller mixes it with the plain source by the backdrop's alpha and composites the
    ///  result source-over.
    std::array<float,3> BlendColours(BlendMode enumBlendMode, const std::array<float,3> &backdrop, const std::array<float,3> &source);

} // namespace Draw2d::Svg

#endif  /* DRAW2D_SVG_BLENDCOLOURS_DOT_HPP */

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

#ifndef DRAW2D_SVG_LINEJOIN_DOT_HPP
#define DRAW2D_SVG_LINEJOIN_DOT_HPP

namespace Core {
    class String;
}

namespace Draw2d::Svg {

    enum class LineJoin {
        NotALineJoin = -1,
        Arcs,
        Bevel,
        Miter,
        MiterClip,
        Round
    };

    LineJoin ParseLineJoin(const Core::String &cstrLineJoin);
    Core::String LineJoinToString(LineJoin enumLineJoin);
}

#endif  /* DRAW2D_SVG_LINEJOIN_DOT_HPP */

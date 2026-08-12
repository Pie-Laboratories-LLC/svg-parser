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

#ifndef PATHMOVE_DOT_HPP
#define PATHMOVE_DOT_HPP

#include <cstdint>

namespace Core {
    class String;
}

namespace Draw2d::Svg {

    enum PathMove : int8_t
    {
        NotAPathMove = -1,
        MoveTo,
        LineTo,
        ClosePath,
        QuadBézierTo,
        SmoothQuadBézierTo,
        CubicBézierTo,
        SmoothCubicBézierTo,
        HorizontalLineTo,
        VerticalLineTo,
        ArcEllipticTo
    };

    PathMove ParsePathMove(const Core::String &cstrValue);
    Core::String PathMoveToString(PathMove enumPathMove);

    // ArcEllipticTo
    // rx ry  the arc's ellipse radii
    // x-axis-rotation  rotation of the ellipse in degrees
    // large-arc-flag  0 or 1, picks between the two possible arcs =180° vs >180°
    // sweep-flag  0 or 1, picks direction (clockwise vs counterclockwise)
    // x y  the endpoint
}

#endif  /* PATHMOVE_DOT_HPP */

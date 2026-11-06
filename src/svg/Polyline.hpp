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

#ifndef DRAW2D_SVG_POLYLINE_DOT_HPP
#define DRAW2D_SVG_POLYLINE_DOT_HPP

#ifndef DRAW2D_SVG_PATH_DOT_HPP
    #include "svg/Path.hpp"
#endif

namespace Draw2d::Svg {

    /// <polyline points="...">: straight lines through the points, left open (filled as if closed).
    ///  Parsed into the equivalent path data, so it renders as a Path.
    class Polyline: public Path
    {
    public:
        Polyline(SvgPathParams svgPathParams):
            Path(std::move(svgPathParams))
        {}
        virtual ~Polyline() = default;

        virtual const char * const getType() const override { return "Polyline"; }
    };

} // namespace Draw2d::Svg

#endif  /* DRAW2D_SVG_POLYLINE_DOT_HPP */

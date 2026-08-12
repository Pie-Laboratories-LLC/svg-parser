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

#ifndef DRAW2D_SVG_SVGDPARSER_DOT_HPP
#define DRAW2D_SVG_SVGDPARSER_DOT_HPP

#include <vector>
#include <utility>
#include <regex>
#include <unordered_map>

#ifndef DRAW2D_SVG_PATHMOVE_DOT_HPP
    #include "svg/PathMove.hpp"
#endif
#ifndef DRAW2D_SVG_SVGPARSER_DOT_HPP
    #include "svg/SvgParser.hpp"
#endif

namespace Core {
    class String;
}

namespace Draw2d::Svg {
    class SvgDParser {
    public:
        SvgDParser() = default;
        SvgDParser(const SvgDParser &copy) = delete;
        SvgDParser &operator=(const SvgDParser &copy) = delete;
        SvgDParser(SvgDParser &&move) = delete;
        virtual ~SvgDParser() = default;

        void setParserCallback(SvgParser::SvgParserCallback callback);
        std::pair<std::vector<PathMove>, std::vector<float>> parseD(const Core::String &cstrD);

        static std::regex PathPartsRegex;
    private:
        inline static std::unordered_map<char,PathMove> PathMoveLookup
        {
            { 'M', MoveTo },
            { 'L', LineTo },
            { 'H', HorizontalLineTo },
            { 'V', VerticalLineTo },
            { 'Q', QuadBézierTo },
            { 'T', SmoothQuadBézierTo },
            { 'C', CubicBézierTo },
            { 'S', SmoothCubicBézierTo },
            { 'A', ArcEllipticTo },
            { 'Z', ClosePath }
        };
        SvgParser::SvgParserCallback m_callback;
        std::vector<PathMove> pathMoves { };
        std::vector<float> points { };
        bool moveTo = false;
        bool bCurrentPathComplete = false;
        // haha fu svg spec.  For Smooth béz's, keep track of the last path move type so
        //  if we see the smooth variety we can contend with whatever the previous point
        //  was
        PathMove enumLastPathMove = NotAPathMove;
        // haha fu svg spec.  For Quad/Cubic béz's we keep track of the control point or
        //  second control point, respectively for the smooth cases.
        float fLastControlX = -1;
        float fLastControlY = -1;
        // haha fu svg spec. we'll track the current point here for the initial relative 'm',
        //  which behaves as if absolute.  As we go we update this with the endpoint of the
        //  segment
        float fCurrentX = 0.0f;
        float fCurrentY = 0.0f;
        PathMove enumCurrentPathMove = NotAPathMove;
        bool currentPathMoveRelative = false;
        unsigned nFirstPointIndex = -1;

        bool __checkSavePoint();
        void __postProcessPoints();
        void __yoinkCurrentPoint();
        void __yoinkControlPoint();
        void __reflectControlPoint();
        void __processEllipticalArc(std::vector<PathMove> &pathMoves, std::vector<float> &points, size_t nFirstPointIndex);
        void __insertLastMoveto();
    };
}

#endif  /* DRAW2D_SVG_SVGDPARSER_DOT_HPP */

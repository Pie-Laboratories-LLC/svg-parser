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

#include "svg/Path.hpp"

#ifndef SVGEXCEPTION_DOT_HPP
    #include "svg/SvgException.hpp"
#endif

namespace Draw2d::Svg {

    Path::Path(SvgPathParams svgPathParams):
        SvgEntity(std::move(svgPathParams)),
        m_points(std::move(svgPathParams.points)),
        m_pathMoves(std::move(svgPathParams.pathMoves))
    {
        // validate that the number of points is correct for the specified path moves
        unsigned nPointIndex = 0;
        Point firstPoint {};
        Point lastPoint {};
        bool bLastPointSet = false;
        for (unsigned i = 0; i < m_pathMoves.size(); i++)
        {
            switch (m_pathMoves[i])
            {
            case MoveTo:
            case LineTo:
                if (nPointIndex + 2 > m_points.size()) throw SvgException(std::format("Not enough m_points at PathMove[{}] {}; ({} / {})", i, PathMoveToString(m_pathMoves[i]).c_str(), nPointIndex, m_points.size()));
                bLastPointSet = true;
                lastPoint.setX(getX(nPointIndex, m_fX.value_or(0), m_fY.value_or(0),getMatrix()));
                lastPoint.setY(getY(nPointIndex, m_fX.value_or(0), m_fY.value_or(0),getMatrix()));
                __checkPoint(lastPoint.getX(), lastPoint.getY());
                nPointIndex += 2;
                if(m_pathMoves[i] == MoveTo) firstPoint = lastPoint;
                break;

            case VerticalLineTo:
                if (nPointIndex + 1 > m_points.size()) throw SvgException(std::format("Not enough m_points at PathMove[{}] {}; ({} / {})", i, PathMoveToString(m_pathMoves[i]), nPointIndex, m_points.size()));
                // this is really paranoia: this shouldn't be possible coming from SvgDParser.
                if(!bLastPointSet) throw SvgException("Internal error.  Encountered a VerticalLineTo but no previous point has been seen.");
                lastPoint.setY(getY(nPointIndex, lastPoint.getX(), m_fX.value_or(0), m_fY.value_or(0),getMatrix()));
                __checkPoint(lastPoint.getX(), lastPoint.getY());
                nPointIndex++;
                break;

            case HorizontalLineTo:
                if (nPointIndex + 1 > m_points.size()) throw SvgException(std::format("Not enough m_points at PathMove[{}] {}; ({} / {})", i, PathMoveToString(m_pathMoves[i]), nPointIndex, m_points.size()));
                // this is really paranoia: this shouldn't be possible coming from SvgDParser.
                if(!bLastPointSet) throw SvgException("Internal error.  Encountered a HorizontalLineTo but no previous point has been seen.");
                lastPoint.setX(getX(nPointIndex, lastPoint.getY(), m_fX.value_or(0), m_fY.value_or(0), getMatrix()));
                __checkPoint(lastPoint.getX(), lastPoint.getY());
                nPointIndex++;
                break;

            case QuadBézierTo:
                if (nPointIndex + 4 > m_points.size()) throw SvgException(std::format("Not enough m_points at PathMove[{}] {}; ({} / {})", i, PathMoveToString(m_pathMoves[i]).c_str(), nPointIndex, m_points.size()));
                if(!bLastPointSet) throw SvgException("Internal error.  Encountered a QuadBézierTo but no previous point has been seen.");
                bLastPointSet = true;
                for(unsigned nIndex = 1; nIndex < 2; nIndex++) {
                    lastPoint.setX(getX(nPointIndex + nIndex, m_fX.value_or(0), m_fY.value_or(0),getMatrix()));
                    lastPoint.setY(getY(nPointIndex + nIndex, m_fX.value_or(0), m_fY.value_or(0),getMatrix()));
                    __checkPoint(lastPoint.getX(), lastPoint.getY());
                }
                nPointIndex += 4;
                break;

            case CubicBézierTo:
                if (nPointIndex + 6 > m_points.size()) throw SvgException(std::format("Not enough m_points at PathMove[{}] {}; ({} / {})", i, PathMoveToString(m_pathMoves[i]).c_str(), nPointIndex, m_points.size()));
                if(!bLastPointSet) throw SvgException("Internal error.  Encountered a CubicBézierTo but no previous point has been seen.");
                bLastPointSet = true;
                for(unsigned nIndex = 1; nIndex < 3; nIndex++) {
                    lastPoint.setX(getX(nPointIndex + nIndex, m_fX.value_or(0), m_fY.value_or(0),getMatrix()));
                    lastPoint.setY(getY(nPointIndex + nIndex, m_fX.value_or(0), m_fY.value_or(0),getMatrix()));
                    __checkPoint(lastPoint.getX(), lastPoint.getY());
                }
                nPointIndex += 6;
                break;

            case ClosePath:
                bLastPointSet = true;
                lastPoint = firstPoint;
                break;

            default: throw SvgException(std::format("Invalid PathMove at PathMove[{}] {}", i, PathMoveToString(m_pathMoves[i]).c_str()));
            }
        }

        if (nPointIndex != m_points.size()) throw SvgException(std::format("Invalid number of points {} != {}",nPointIndex,m_points.size()));
    }

    Path::Path(const Path &copy): SvgEntity(copy)
    {
        m_points.clear();
        for(float fP : copy.m_points) m_points.push_back(fP);
        m_pathMoves.clear();
        for(PathMove enumPathMove : copy.m_pathMoves) m_pathMoves.push_back(enumPathMove);
    }

    Path::Path(Path &&copy):
        SvgEntity(copy)
       ,m_points(std::move(copy.m_points))
       ,m_pathMoves(std::move(copy.m_pathMoves))
    {
    }

} // namespace Draw2d::Svg

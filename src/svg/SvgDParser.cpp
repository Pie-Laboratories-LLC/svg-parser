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

#include <cmath>

#include "svg/SvgDParser.hpp"

#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif
#ifndef DRAW2D_SVG_SVGEXCEPTION_DOT_HPP
    #include "svg/SvgException.hpp"
#endif
#ifndef CORE_STDNUMERIC_DOT_HPP
    #include "core/StdNumeric.hpp"
#endif
#ifndef CORE_SPLICE_DOT_HPP
    #include "core/Splice.hpp"
#endif
#ifndef CORE_LOGGER_DOT_HPP
    #include "core/Logger.hpp"
#endif
#ifndef DRAW2D_LOGGINGCATEGORIES_DOT_HPP
    #include "draw2d/LoggingCategories.hpp"
#endif

namespace Draw2d::Svg {

    std::regex SvgDParser::PathPartsRegex { R"xxx(,?\s*(?:([+-]?(?:\d+\.?\d*|\.\d+)(?:[eE][+-]?\d+)?)|([a-z])\s*))xxx", std::regex::icase };

    void SvgDParser::setParserCallback(SvgParser::SvgParserCallback callback)
    {
        m_callback = callback;
    }

    std::pair<std::vector<PathMove>, std::vector<float>> SvgDParser::parseD(const Core::String &cstrD)
    {
        Core::Logger::Singleton().Trace(LoggingCategories::SVG, "SVGD: new path");
        std::sregex_iterator begin(cstrD.begin(), cstrD.end(), PathPartsRegex);
        std::sregex_iterator end;

        for (auto it = begin; it != end ; it++)
        {
            Core::String strValue = (*it).str();
            // get rid of any left over cruft from "lexxing"
            strValue.trim_start_in_place(',');
            strValue.trim_in_place();

            // first see if this is a new command.
            if (strValue.length() == 1 && std::isalpha(strValue.front()))
            {
                // save the last path move type for smooth varieties
                enumLastPathMove = enumCurrentPathMove;
                // we'll reset this when we get the required number of points
                nFirstPointIndex = -1;
                char cPathMove = std::toupper(strValue.front());
                if (auto search = PathMoveLookup.find(cPathMove); search != PathMoveLookup.end())
                {
                    PathMove pathMove = (*search).second;
                    switch (pathMove)
                    {
                    case PathMove::MoveTo:
                        if (moveTo) {
                            // moveto can follow anything.  Here we check to see if there are two movetos in
                            //  a row, and if so, remove the previous one as two movetos, the first moveto
                            //  is a noop
                            if(enumCurrentPathMove == PathMove::MoveTo) {
                                if(!bCurrentPathComplete) throw SvgException("Invalid moveto at {}; not enough points!", pathMoves.size() - 1);
                                // remove the previous moveto, it's a noop
                                pathMoves.pop_back();
                                points.pop_back();
                                points.pop_back();
                            }
                        }
                        moveTo = true;
                        break;

                    case PathMove::ClosePath:
                        if(moveTo) {
                            // special case; we don't really deal with this in __checkSavePoint, so here be sure to
                            //  add it explicitly.
                            pathMoves.push_back(pathMove);
                        }
                        else {
                            if(m_callback) m_callback(SvgParserStatus::Warning, "Encountered a closepath with no previous moveto - ignoring");
                        }
                        moveTo = false;
                        break;

                    default:
                        if (!moveTo) {
                            if(!points.size()) {
                                pathMoves.push_back(PathMove::MoveTo);
                                points.push_back(0.0f);
                                points.push_back(0.0f);
                            }
                            else {
                                __insertLastMoveto();
                            }
                            // in either case, we've inserted a moveto now.
                            moveTo = true;
                        }
                        break;
                    }
                    enumCurrentPathMove = pathMove;
                    // so much pita in SVG: lowercase means the coordinates are specified relative
                    currentPathMoveRelative = std::islower(static_cast<unsigned char>(strValue.front()));
                    nFirstPointIndex = points.size();
                    continue;
                }
                else throw SvgException("Unrecognized PathMove `{}' at {}: {} in {}", strValue, pathMoves.size(), strValue, cstrD);
            }

            if(enumCurrentPathMove == NotAPathMove) throw SvgException("d=\"...\" must start with a path move.  No path move found in {}!", cstrD);

            float f = Core::ParseFloat(strValue);
            bool bSavePoint = true;

            // ugh, arc eliptic fields 3 and 4 are flags that can be mushed together.  deal with that here.
            if(enumCurrentPathMove == PathMove::ArcEllipticTo
            && points.size() - nFirstPointIndex == 3) {
                // special case, check and see if the points are mushed together
                if(strValue.starts_with("00")) {
                    points.push_back(0);
                    points.push_back(0);
                    bSavePoint = false;
                }
                else if(strValue.starts_with("01")) {
                    points.push_back(0);
                    points.push_back(1);
                    bSavePoint = false;
                }
                else if(strValue.starts_with("10")) {
                    points.push_back(1);
                    points.push_back(0);
                    bSavePoint = false;
                }
                else if(strValue.starts_with("11")) {
                    points.push_back(1);
                    points.push_back(1);
                    bSavePoint = false;
                }

                // it's not sufficient for some automated tools to mush the two flags together,
                //  they have to mush the mushed-two-flags with cx.  Be sure to catch that.
                if(!bSavePoint && strValue.length() > 2) {
                    points.push_back(Core::ParseFloat(strValue.substr(2)));
                }
            }

            if(bSavePoint) points.push_back(f);

            // keep track of whether the current path segment is complete
            bCurrentPathComplete = __checkSavePoint();
            if(bCurrentPathComplete) {
                Core::Logger::Singleton().Trace(LoggingCategories::SVG, "SVGD: finish {} ({}), cp = {}, {}", PathMoveToString(enumCurrentPathMove), currentPathMoveRelative, fCurrentX, fCurrentY);
            }
        }

        // closing the path is optional, so letś verify the number of points after the last path move is correct
        //  if we still think a moveto is active, based upon whatever the last PathMove was.
        if (moveTo) {
            bool bClosed = __checkSavePoint();
            if(moveTo && bClosed) {
                Core::Logger::Singleton().Trace(LoggingCategories::SVG, "SVGD: _final_ finish {} ({}), cp = {}, {}", PathMoveToString(enumCurrentPathMove), currentPathMoveRelative, fCurrentX, fCurrentY);
            }
            std::string strReason = Core::String("trailing {} doesn't have enough points.", PathMoveToString(enumCurrentPathMove));
            if(!bClosed) throw SvgException("Path not closed {}", strReason);
            // see PathMove::Close case above; we don't deal with this in __checkSavePoint, so special
            //  case add this explicitly
            pathMoves.push_back(PathMove::ClosePath);
        }

        Core::Logger::Singleton().Trace(LoggingCategories::SVG, "SVGD: DONE path");
        return { std::move(pathMoves), std::move(points) };
    }

    bool SvgDParser::__checkSavePoint()
    {
        // indicates whether we saved the path move
        bool bResult = false;

        // now let's see if we have enough points to constitute the current path move type & if
        //  so save the path move & process it.
        // things to do to close a current path segment:
        //  post-process the points to make them relative if needed
        //  for smooth béz's, reflect the last control point over the current point
        //  update index into points to know we've processed all the points
        //  yoink the end point to the current point for path-relative calculations
        //  save the path move
        //  yoink the current control point for the smooth béz variants
        //  update the current path move (only for MoveTo => LineTo) to indicate
        //    subsequent points after the first two to moveto are lineto, per the
        //    spec
        //  update the last path move (only Smooth béz's) to be the non-smooth
        //    variant since we're storing that path move
        // In any event, if the index into points is the same as the number of
        //  points, it means the last path was effectively closed.
        switch(enumCurrentPathMove) {
        case PathMove::MoveTo:
        case PathMove::LineTo:
        {
            // more than two points after MoveTo (or LineTo) is an implicit lineto.
            //  we require that to be explicit.  didn't have to be implemented that
            //  way, we just need to know which points are which.
            if(points.size() - nFirstPointIndex && !((points.size() - nFirstPointIndex) % 2)) {
                // deal with relative paths
                __postProcessPoints();
                __yoinkCurrentPoint();
                // now that we've saved the points, save what kind of move it was
                pathMoves.push_back(enumCurrentPathMove);
                enumCurrentPathMove = LineTo;
                nFirstPointIndex = points.size();
                __yoinkControlPoint();
            }
        }; break;

        case PathMove::HorizontalLineTo:
        case PathMove::VerticalLineTo:
        {
            // same logic as above; these can be strung together.
            if(points.size() - nFirstPointIndex) {
                // deal with relative paths
                __postProcessPoints();
                __yoinkCurrentPoint();
                // now that we've saved the points, save what kind of move it was
                pathMoves.push_back(enumCurrentPathMove);
                nFirstPointIndex = points.size();
                __yoinkControlPoint();
            }
        }; break;

        case PathMove::QuadBézierTo:
        {
            // similar logic to above, these can be strung together.
            if(points.size() - nFirstPointIndex && !((points.size() - nFirstPointIndex) % 4)) {
                // deal with relative paths
                __postProcessPoints();
                __yoinkCurrentPoint();
                nFirstPointIndex = points.size();
                // now that we've saved the points, save what kind of move it was
                pathMoves.push_back(enumCurrentPathMove);
                __yoinkControlPoint();
            }
        }; break;

        case PathMove::SmoothQuadBézierTo:
        {
            // similar logic to above, these can be strung together.
            if(points.size() - nFirstPointIndex && !((points.size() - nFirstPointIndex) % 2)) {
                // deal with relative paths
                __postProcessPoints();
                // ok, now weŕe going to reflect the previous control point; NOTE *must*
                //  be done before yoinking the current point
                __reflectControlPoint();
                // insert the reflected control point.
                Core::splice(points, points.size() - 2, 0, { fLastControlX, fLastControlY });
                __yoinkCurrentPoint();
                // note - reflecting the control and the splice means we've converted this into
                //  a standard quad bézier
                enumLastPathMove = PathMove::QuadBézierTo;
                pathMoves.push_back(enumLastPathMove);
                nFirstPointIndex = points.size();
                __yoinkControlPoint();
            }
        }; break;

        case PathMove::CubicBézierTo:
        {
            // similar logic to above, these can be strung together.
            if(points.size() - nFirstPointIndex && !((points.size() - nFirstPointIndex) % 6)) {
                // deal with relative paths
                __postProcessPoints();
                __yoinkCurrentPoint();
                pathMoves.push_back(PathMove::CubicBézierTo);
                nFirstPointIndex = points.size();
                // now that we've saved the points, save what kind of move it was
                __yoinkControlPoint();
            }
        }; break;

        case PathMove::SmoothCubicBézierTo:
        {
            // similar logic to above, these can be strung together.
            if(points.size() - nFirstPointIndex && !((points.size() - nFirstPointIndex) % 4)) {
                // deal with relative paths
                __postProcessPoints();
                // ok, now weŕe going to reflect the previous control point
                __reflectControlPoint();
                // insert the reflected control point.
                Core::splice(points, points.size() - 4, 0, { fLastControlX, fLastControlY });
                // now we can yoink the current point
                __yoinkCurrentPoint();
                // note we have converted the smooth version to the standard
                enumLastPathMove = PathMove::CubicBézierTo;
                pathMoves.push_back(enumLastPathMove);
                nFirstPointIndex = points.size();
                // get the control point for the next SmoothQuadBézierTo.
                __yoinkControlPoint();
            }
        }; break;

        case PathMove::ArcEllipticTo:
        {
            // similar logic - and also with A, the "large-arc" and "sweep-flag", fields
            //  3 & 4, zero based and can be mushed together.
            if(points.size() - nFirstPointIndex && !((points.size() - nFirstPointIndex) % 7)) {
                // deal with relative, if appropriate
                __postProcessPoints();
                // similar logic from above, ArcEllipticTo has 7 points
                // we convert it to cubic béz per claude's suggestion and
                // the difficulty of coordinage translations of the elliptic
                // arc goes away.  note this method will update pathMoves.
                __processEllipticalArc(pathMoves, points, nFirstPointIndex);
                nFirstPointIndex = points.size();
                __yoinkCurrentPoint();
                __yoinkControlPoint();
            }
        }; break;

        default: throw SvgException("Unhandled path move {}",PathMoveToString(enumCurrentPathMove).c_str());
        }

        // NOTE: if there are no points left, the path is closed, regardless of the type
        //  above we checked to see if there were enough points to close the current
        //  path move type; here we check to see if there are any more points at all.
        if(points.size() == nFirstPointIndex) bResult = true;

        return bResult;
    }

    void SvgDParser::__postProcessPoints()
    {
        if(!currentPathMoveRelative) return;

        switch(enumCurrentPathMove) {
        case PathMove::HorizontalLineTo:
        case PathMove::VerticalLineTo:
            // paranoia will destroya
            if (nFirstPointIndex + 1 > points.size()) throw SvgException("Internal error; expecting to update a single point, but there isn't one");
            if(enumCurrentPathMove == PathMove::HorizontalLineTo) points[nFirstPointIndex] += fCurrentX;
            else points[nFirstPointIndex] += fCurrentY;
            break;


        case PathMove::ArcEllipticTo:
            if(points.size() - nFirstPointIndex != 7) throw SvgException("Internal error; arcEllipticTo requires 7 points, there are only {}", points.size() - nFirstPointIndex);
            points[points.size() - 2] += fCurrentX;
            points[points.size() - 1] += fCurrentY;
            break;

        case PathMove::MoveTo:
        case PathMove::LineTo:
        case PathMove::QuadBézierTo:
        case PathMove::SmoothQuadBézierTo:
        case PathMove::CubicBézierTo:
        case PathMove::SmoothCubicBézierTo:
        {
            unsigned nPointCount;
            // holy inception.  this is fairly ugly but don´t see a better way to solve this.
            //  the number of points to update varies by path move type, obviously...
            switch(enumCurrentPathMove) {
            case PathMove::MoveTo:
                // initial relative moveto is an absolute point
                // otherwise fall through, make the moveto relative
                if(pathMoves.size() == 1) return;
            case PathMove::LineTo:
            case PathMove::SmoothQuadBézierTo: nPointCount = 1;
                break;

            case PathMove::QuadBézierTo:
            case PathMove::SmoothCubicBézierTo: nPointCount = 2;
                break;
            case PathMove::CubicBézierTo: nPointCount = 3;
                break;

            default: throw SvgException("Unhandled path move {}", PathMoveToString(enumCurrentPathMove));
            }
            // hereś the meat of what weŕe about, update the number of points just determined
            // paranoia will destroya
            if (nFirstPointIndex + nPointCount * 2 > points.size()) throw SvgException("Internal error; expecting to update {} points, but there are only {} points", nPointCount * 2, points.size() - nFirstPointIndex);
            for(unsigned nPointIndex = 0; nPointIndex < nPointCount; nPointIndex++, nFirstPointIndex += 2) {
                points[nFirstPointIndex] += fCurrentX;
                points[nFirstPointIndex + 1] += fCurrentY;
            }
        }; break;

        default: throw SvgException("Unhandled path move {}", PathMoveToString(enumCurrentPathMove));
        }
    }

    void SvgDParser::__yoinkCurrentPoint()
    {
        if(points.size() < 1) throw SvgException("Internal error - there are only {} points; can't yoink the last point!", points.size());
        // for vertical/horizontal lineto, only update the corresponding axis, which will be the last
        //  point on the stack - fCurrentX looks weird here for that reason.
        if(enumCurrentPathMove == PathMove::VerticalLineTo) fCurrentY = points[points.size() - 1];
        else if(enumCurrentPathMove == PathMove::HorizontalLineTo) fCurrentX = points[points.size() - 1];
        else {
            fCurrentY = points[points.size() - 1];
            fCurrentX = points[points.size() - 2];
        }
    }

    void SvgDParser::__yoinkControlPoint()
    {
        switch(enumCurrentPathMove) {
            case PathMove::QuadBézierTo:
            case PathMove::SmoothQuadBézierTo:
            case PathMove::CubicBézierTo:
            case PathMove::SmoothCubicBézierTo:
                if(points.size() < 4) throw SvgException("Internal error - there are only {} points; can't yoink the last control point for {}!", points.size(), PathMoveToString(enumCurrentPathMove));
                // for both quad & cubic béziers, the last control point is before the endpoint.
                fLastControlX = points[points.size() - 4];
                fLastControlY = points[points.size() - 3];
                break;

            case PathMove::HorizontalLineTo:
                if(points.size() < 3) throw SvgException("Internal error - there are only {} points; can't yoink the last point as control for {}!", points.size(), PathMoveToString(enumCurrentPathMove));
                fLastControlX = points[points.size() - 1];
                fLastControlY = points[points.size() - 2];
                break;

            case PathMove::VerticalLineTo:
                if(points.size() < 3) throw SvgException("Internal error - there are only {} points; can't yoink the last point as control for {}!", points.size(), PathMoveToString(enumCurrentPathMove));
                fLastControlY = points[points.size() - 1];
                fLastControlX = points[points.size() - 3];
                break;

            default:
                if(points.size() < 2) throw SvgException("Internal error - there are only {} points; can't yoink the last point as control for {}!", points.size(), PathMoveToString(enumCurrentPathMove));
                // degenerate case if used - defaults to last point for non-béz
                fLastControlX = points[points.size() - 2];
                fLastControlY = points[points.size() - 1];
                break;
        }
    }

    void SvgDParser::__reflectControlPoint()
    {
        // for the smooth variants, if the move before wasn't the corresponding type, per the spec,
        //  we just use the endpoint for the control point 
        if((enumCurrentPathMove == PathMove::SmoothQuadBézierTo &&
            enumLastPathMove != PathMove::QuadBézierTo)
        || (enumCurrentPathMove == PathMove::SmoothCubicBézierTo &&
           enumLastPathMove != PathMove::CubicBézierTo)) {
            fLastControlX = fCurrentX;
            fLastControlY = fCurrentY;
            return;
        }
        // NOTE - chicken and an egg problem.  we don't want to update the
        //  current point to support the above case, yet the point we're
        //  reflecting over 
        // handle these separately for clarity - issue is, there are two points for SmoothQuad,
        //  four points for SmootCubic
        if(enumCurrentPathMove == PathMove::SmoothQuadBézierTo) {
            if(points.size() < 6) throw SvgException("Not enough points {} to reflect the previous control point :(", points.size());
            fLastControlX = 2 * fCurrentX - points[points.size() - 6];
            fLastControlY = 2 * fCurrentY - points[points.size() - 5];
            Core::Logger::Singleton().Trace(LoggingCategories::SVG, "SVGD: Reflected Quad Béz CP {} {}", fLastControlX, fLastControlY);
        }
        else if(enumCurrentPathMove == PathMove::SmoothCubicBézierTo) {
            if(points.size() < 8) throw SvgException("Not enough points {} to reflect the previous control point :(", points.size());
            fLastControlX = 2 * fCurrentX - points[points.size() - 8];
            fLastControlY = 2 * fCurrentY - points[points.size() - 7];
            Core::Logger::Singleton().Trace(LoggingCategories::SVG, "SVGD: Reflected Cubic Béz CP {} {}", fLastControlX, fLastControlY);
        }
    }

    void SvgDParser::__processEllipticalArc(std::vector<PathMove> &pathMoves, std::vector<float> &points, size_t nFirstPointIndex)
    {
        float x0, y0, rx, ry, phiDeg, x1, y1;
        bool largeArc, sweep;
        if(nFirstPointIndex < 2) throw SvgException("ArcEllipticTo must follow moveto or some other segment!");
        x0 =       fCurrentX;
        y0 =       fCurrentY;
        rx =       points[nFirstPointIndex];
        ry =       points[nFirstPointIndex + 1];
        phiDeg =   points[nFirstPointIndex + 2];
        largeArc = points[nFirstPointIndex + 3];
        sweep =    points[nFirstPointIndex + 4];
        x1 =       points[nFirstPointIndex + 5];
        y1 =       points[nFirstPointIndex + 6];

        Core::Logger::Singleton().Trace(LoggingCategories::SVG,
            "Initial arcTo points [{}, {}], {} {} {} {} {} {} {}",
            x0, y0, rx, ry, phiDeg, largeArc, sweep, x1, y1);

        // get rid of the elliptical arc points and the elliptical arc path move
        Core::splice(points, nFirstPointIndex);

        // following is adopted from Renderer<T>::PlotEllipticalArc.
        // Degenerate radius -> straight line per SVG spec F.6.2
        if (rx == 0.0 || ry == 0.0) {
            points.push_back(x1);
            points.push_back(y1);
            pathMoves.push_back(PathMove::LineTo);
            Core::Logger::Singleton().Trace(LoggingCategories::SVG,
                "    degenerated into line from arc {} {} ", x1, y1);
            return;
        }
        rx = std::abs(rx); ry = std::abs(ry);

        const double phi = phiDeg * std::numbers::pi / 180.0;
        const double cosPhi = std::cos(phi), sinPhi = std::sin(phi);

        // --- Endpoint -> center parameterization (SVG 1.1 Appendix F.6.5) ---
        const double dx2 = (x0 - x1) / 2.0, dy2 = (y0 - y1) / 2.0;
        const double x1p =  cosPhi * dx2 + sinPhi * dy2;
        const double y1p = -sinPhi * dx2 + cosPhi * dy2;

        // Correct out-of-range radii (F.6.6)
        double rxSq = rx*rx, rySq = ry*ry;
        const double x1pSq = x1p*x1p, y1pSq = y1p*y1p;
        const double lambda = x1pSq/rxSq + y1pSq/rySq;
        if (lambda > 1.0) {
            const double s = std::sqrt(lambda);
            rx *= s; ry *= s; rxSq = rx*rx; rySq = ry*ry;
        }

        const double num = rxSq*rySq - rxSq*y1pSq - rySq*x1pSq;
        const double den = rxSq*y1pSq + rySq*x1pSq;
        const double coef = (largeArc == sweep ? -1.0 : 1.0) * std::sqrt(std::max(0.0, num/den));

        const double cxp = coef * ( rx*y1p / ry);
        const double cyp = coef * (-ry*x1p / rx);

        const double cx = cosPhi*cxp - sinPhi*cyp + (x0+x1)/2.0;
        const double cy = sinPhi*cxp + cosPhi*cyp + (y0+y1)/2.0;

        auto vecAngle = [](double ux, double uy, double vx, double vy) {
            const double dot = ux*vx + uy*vy;
            const double len = std::sqrt((ux*ux+uy*uy)*(vx*vx+vy*vy));
            const double a = std::acos(std::clamp(dot/len, -1.0, 1.0));
            return (ux*vy - uy*vx < 0.0) ? -a : a;
        };

        const double theta1 = vecAngle(1,0, (x1p-cxp)/rx, (y1p-cyp)/ry);
        double dTheta       = vecAngle((x1p-cxp)/rx, (y1p-cyp)/ry, (-x1p-cxp)/rx, (-y1p-cyp)/ry);

        if (!sweep && dTheta > 0) dTheta -= 2*std::numbers::pi;
        if ( sweep && dTheta < 0) dTheta += 2*std::numbers::pi;

        // --- Split into <= 90 degree chunks, approximate each with a cubic Bezier ---
        // (Maisonobe 2003: kappa = 4/3 * tan(delta/4))
        const int segments   = static_cast<int>(std::ceil(std::abs(dTheta) / (std::numbers::pi/2.0)));
        const double delta   = dTheta / segments;
        const double kappa   = 4.0/3.0 * std::tan(delta/4.0);

        auto ex  = [&](double ct,double st){ return cx + rx*ct*cosPhi - ry*st*sinPhi; };
        auto ey  = [&](double ct,double st){ return cy + rx*ct*sinPhi + ry*st*cosPhi; };
        auto dex = [&](double ct,double st){ return -rx*st*cosPhi - ry*ct*sinPhi; };
        auto dey = [&](double ct,double st){ return -rx*st*sinPhi + ry*ct*cosPhi; };

        double theta = theta1;
        for (int i = 0; i < segments; ++i, theta += delta) {
            const double thetaNext = theta + delta;
            const double cosT = std::cos(theta),     sinT = std::sin(theta);
            const double cosN = std::cos(thetaNext), sinN = std::sin(thetaNext);

            const double ex0 = ex(cosT,sinT), ey0 = ey(cosT,sinT);
            const double ex3 = ex(cosN,sinN), ey3 = ey(cosN,sinN);
            const double ex1 = ex0 + kappa*dex(cosT,sinT), ey1 = ey0 + kappa*dey(cosT,sinT);
            const double ex2 = ex3 - kappa*dex(cosN,sinN), ey2 = ey3 - kappa*dey(cosN,sinN);

            Core::Logger::Singleton().Trace(LoggingCategories::SVG,
                "    generated cubic béz points from arc {} {} {} {} {} {}",
                ex1, ey1, ex2,
                ey2, ex3, ey3);

            pathMoves.push_back(PathMove::CubicBézierTo);
            points.push_back(ex1);
            points.push_back(ey1);
            points.push_back(ex2);
            points.push_back(ey2);
            points.push_back(ex3);
            points.push_back(ey3);
        }
    }

    void SvgDParser::__insertLastMoveto()
    {
        // here we walk the pathMoves looking for the last moveto
        int nPathMoveIndex = pathMoves.size() - 1;
        unsigned nPointIndex = points.size() - 1;
        while(nPathMoveIndex >= 0) {
            switch(pathMoves[nPathMoveIndex]) {
                case PathMove::LineTo:
                    nPointIndex -= 2;
                    break;
                case PathMove::ClosePath:
                    break;
                case PathMove::QuadBézierTo:
                    nPointIndex -= 4;
                    break;
                case PathMove::CubicBézierTo:
                    nPointIndex -= 6;
                    break;
                case PathMove::HorizontalLineTo:
                case PathMove::VerticalLineTo:
                    nPointIndex -= 1;
                    break;
                case PathMove::MoveTo:
                    // oh, frabjous day!
                    pathMoves.push_back(PathMove::MoveTo);
                    points.push_back(points[nPointIndex - 1]);
                    points.push_back(points[nPointIndex]);
                    return;
                default: throw SvgException("Unhandled PathMove {} looking for the last MoveTo", PathMoveToString(pathMoves[nPathMoveIndex]));
            }
            nPathMoveIndex--;
        }
        throw SvgException("Failed to find previous moveto starting from {}", pathMoves.size() - 1);
    }

}

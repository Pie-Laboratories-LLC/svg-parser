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

#define CATCH_CONFIG_MAIN 
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>

#include "svg/SvgDParser.hpp"
#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif
#ifndef CORE_LOGGER_DOT_HPP
    #include "core/Logger.hpp"
#endif
#ifndef CORE_LOGLEVEL_DOT_HPP
    #include "core/LogLevel.hpp"
#endif

class LoggerFixtureListener : public Catch::EventListenerBase {
public:
    using Catch::EventListenerBase::EventListenerBase;

    void testRunStarting(Catch::TestRunInfo const&) override {
        // Runs before EVERY TEST_CASE. Swap for whatever actually
        // initializes your Logger singleton -- e.g. setting a sink,
        // a minimum level, etc.
    }
};
CATCH_REGISTER_LISTENER(LoggerFixtureListener)

TEST_CASE("1a. moveto-lineto-closepath happy path", "[svg-d]") {
    auto strD = R"xxx(M0 0 L10 10Z)xxx";
    Draw2d::Svg::SvgDParser svgDParser {};
    auto [ pathMoves, points ] = svgDParser.parseD(strD);
    REQUIRE( pathMoves.size() == 3 );
    REQUIRE( points.size() == 4 );
}

TEST_CASE("1b. relative lineto matches absolute lineto", "[svg-d]") {
    Draw2d::Svg::SvgDParser parserAbs {};
    Draw2d::Svg::SvgDParser parserRel {};
    auto [ movesAbs, pointsAbs ] = parserAbs.parseD(R"xxx(M10 10 L20 20 L30 10)xxx");
    auto [ movesRel, pointsRel ] = parserRel.parseD(R"xxx(m10 10 l10 10 l10 -10)xxx");
    REQUIRE( movesAbs.size() == movesRel.size() );
    REQUIRE( pointsAbs.size() == pointsRel.size() );
    for (std::size_t i = 0; i < pointsAbs.size(); i++)
        REQUIRE( pointsAbs[i] == Catch::Approx(pointsRel[i]) );
}

TEST_CASE("1c. first moveto is relative, treated as absolute", "[svg-d]") {
    Draw2d::Svg::SvgDParser parserAbs {};
    Draw2d::Svg::SvgDParser parserRel {};
    auto [ movesAbs, pointsAbs ] = parserAbs.parseD(R"xxx(M50 50 L60 60)xxx");
    auto [ movesRel, pointsRel ] = parserRel.parseD(R"xxx(m50 50 l10 10)xxx");
    REQUIRE( pointsAbs.size() == pointsRel.size() );
    for (std::size_t i = 0; i < pointsAbs.size(); i++)
        REQUIRE( pointsAbs[i] == Catch::Approx(pointsRel[i]) );
}

TEST_CASE("1d. moveto with implicit repeated points becomes lineto", "[svg-d]") {
    auto strD = R"xxx(M0 0 10 10 20 20)xxx";
    Draw2d::Svg::SvgDParser svgDParser {};
    auto [ pathMoves, points ] = svgDParser.parseD(strD);
    // First pair is MoveTo, remaining pairs are implicit LineTo
    REQUIRE( pathMoves[0] == Draw2d::Svg::PathMove::MoveTo );
    REQUIRE( pathMoves[1] == Draw2d::Svg::PathMove::LineTo );
    REQUIRE( pathMoves[2] == Draw2d::Svg::PathMove::LineTo );
    REQUIRE( points.size() == 6 );
}

TEST_CASE("1e. horizontal and vertical lineto only update one axis", "[svg-d]") {
    auto strD = R"xxx(M10 10 H50 V60)xxx";
    Draw2d::Svg::SvgDParser svgDParser {};
    auto [ pathMoves, points ] = svgDParser.parseD(strD);
    REQUIRE( pathMoves.size() == 4 );
    REQUIRE( points.size() == 4 );
    // H50 -> (50,10)
    REQUIRE( points[2] == Catch::Approx(50.0f) );
    // V60 -> (50,60)
    REQUIRE( points[3] == Catch::Approx(60.0f) );
}

TEST_CASE("1f. relative horizontal/vertical lineto", "[svg-d]") {
    auto strD = R"xxx(M10 10 h40 v50)xxx";
    Draw2d::Svg::SvgDParser svgDParser {};
    auto [ pathMoves, points ] = svgDParser.parseD(strD);
    REQUIRE( points[2] == Catch::Approx(50.0f) );  // 10+40
    REQUIRE( points[3] == Catch::Approx(60.0f) );  // 10+50
}

TEST_CASE("1g. cubic bezier absolute", "[svg-d]") {
    auto strD = R"xxx(M0 0 C10 0 10 10 20 10)xxx";
    Draw2d::Svg::SvgDParser svgDParser {};
    auto [ pathMoves, points ] = svgDParser.parseD(strD);
    REQUIRE( pathMoves[1] == Draw2d::Svg::PathMove::CubicBézierTo );
    // control1, control2, endpoint = 6 floats for this segment
    REQUIRE( points.size() == 2 + 6 );
}

TEST_CASE("1h. cubic bezier relative matches absolute", "[svg-d]") {
    Draw2d::Svg::SvgDParser parserAbs {};
    Draw2d::Svg::SvgDParser parserRel {};
    auto [ movesAbs, pointsAbs ] = parserAbs.parseD(R"xxx(M0 0 C10 0 10 10 20 10)xxx");
    auto [ movesRel, pointsRel ] = parserRel.parseD(R"xxx(M0 0 c10 0 10 10 20 10)xxx");
    REQUIRE( pointsAbs.size() == pointsRel.size() );
    for (std::size_t i = 0; i < pointsAbs.size(); i++)
        REQUIRE( pointsAbs[i] == Catch::Approx(pointsRel[i]) );
}

TEST_CASE("1i. smooth cubic reflects previous control point", "[svg-d]") {
    // S after C should reflect C's second control point through current point.
    // C10 0 10 10 20 10  ->  second control (10,10), endpoint (20,10)
    // S30 0 40 10        ->  reflected first control = 2*(20,10) - (10,10) = (30,10)
    auto strD = R"xxx(M0 0 C10 0 10 10 20 10 S30 0 40 10)xxx";
    Draw2d::Svg::SvgDParser svgDParser {};
    auto [ pathMoves, points ] = svgDParser.parseD(strD);
    REQUIRE( pathMoves[2] == Draw2d::Svg::PathMove::CubicBézierTo );
    // points layout: [M: x,y] [C: cx1,cy1,cx2,cy2,ex,ey] [S: cx1,cy1,cx2,cy2,ex,ey]
    // S's reflected first control point should be (30,10)
    REQUIRE( points[8]  == Catch::Approx(30.0f) );
    REQUIRE( points[9]  == Catch::Approx(10.0f) );
}

TEST_CASE("1j. smooth cubic with no preceding cubic degenerates to current point", "[svg-d]") {
    // S right after a lineto: first control point should equal current point (10,10)
    auto strD = R"xxx(M0 0 L10 10 S30 0 40 10)xxx";
    Draw2d::Svg::SvgDParser svgDParser {};
    auto [ pathMoves, points ] = svgDParser.parseD(strD);
    REQUIRE( points[4] == Catch::Approx(10.0f) );  // reflected control1.x == current x
    REQUIRE( points[5] == Catch::Approx(10.0f) );  // reflected control1.y == current y
}

TEST_CASE("1k. arcellipticto is replaced by Cubic beziers", "[svg-d]") {
    auto strD = R"xxx(M0 0 A10 20 15 0 0 20 20Z)xxx";
    Draw2d::Svg::SvgDParser svgDParser {};
    auto [ pathMoves, points ] = svgDParser.parseD(strD);
    REQUIRE( pathMoves[0] == Draw2d::Svg::PathMove::MoveTo );
    REQUIRE( pathMoves[pathMoves.size() - 1] == Draw2d::Svg::PathMove::ClosePath );
    for(unsigned i = 1; i < pathMoves.size() - 1; i++) {
        REQUIRE( pathMoves[i] == Draw2d::Svg::PathMove::CubicBézierTo );
    }
}

TEST_CASE("1l. arc final expanded endpoint matches the arc's absolute endpoint", "[svg-d]") {
    auto strD = R"xxx(M0 0 A10 20 15 0 0 20 20)xxx";
    Draw2d::Svg::SvgDParser svgDParser {};
    auto [ pathMoves, points ] = svgDParser.parseD(strD);
    REQUIRE( points[points.size() - 2] == Catch::Approx(20.0f).margin(1.0) );
    REQUIRE( points[points.size() - 1] == Catch::Approx(20.0f).margin(1.0) );
}

TEST_CASE("1m. relative arc endpoint matches absolute arc endpoint", "[svg-d]") {
    Draw2d::Svg::SvgDParser parserAbs {};
    Draw2d::Svg::SvgDParser parserRel {};
    auto [ movesAbs, pointsAbs ] = parserAbs.parseD(R"xxx(M0 0 A10 20 15 0 0 20 20)xxx");
    auto [ movesRel, pointsRel ] = parserRel.parseD(R"xxx(M0 0 a10 20 15 0 0 20 20)xxx");
    REQUIRE( pointsAbs.size() == pointsRel.size() );
    REQUIRE( movesAbs.size() == movesRel.size() );
    REQUIRE( pointsAbs[pointsAbs.size() - 2] == Catch::Approx(pointsRel[pointsRel.size() - 2]).margin(1.0) );
    REQUIRE( pointsAbs[pointsAbs.size() - 1] == Catch::Approx(pointsRel[pointsRel.size() - 1]).margin(1.0) );
}

TEST_CASE("1n. arc flags glued together with no separators parse correctly", "[svg-d]") {
    // classic flag-glue case: "01" after rotation must parse as flag=0, flag=1, not "01" as one number
    auto strD = R"xxx(M0 0 A10 10 0 01100 100)xxx";
    Draw2d::Svg::SvgDParser svgDParser {};
    auto [ pathMoves, points ] = svgDParser.parseD(strD);
    // endpoint should be (100,100), not garbage from a mis-tokenized flag run
    REQUIRE( points[points.size() - 2] == Catch::Approx(100.0f).margin(1.0) );
    REQUIRE( points[points.size() - 1] == Catch::Approx(100.0f).margin(1.0) );
}

TEST_CASE("1o. all four arc flag combinations parse without throwing", "[svg-d]") {
    auto strD = R"xxx(M0 0 A10 20 15 0 0 20 20
                       M0 0 A10 20 15 0 1 20 20
                       M0 0 A10 20 15 1 0 20 20
                       M0 0 A10 20 15 1 1 20 20)xxx";
    Draw2d::Svg::SvgDParser svgDParser {};
    REQUIRE_NOTHROW( svgDParser.parseD(strD) );
}

TEST_CASE("1p. quadratic bezier absolute", "[svg-d]") {
    auto strD = R"xxx(M0 0 Q10 10 20 0)xxx";
    Draw2d::Svg::SvgDParser svgDParser {};
    auto [ pathMoves, points ] = svgDParser.parseD(strD);
    REQUIRE( pathMoves[1] == Draw2d::Svg::PathMove::QuadBézierTo );
    REQUIRE( points.size() == 2 + 4 );  // control + endpoint
}

TEST_CASE("1q. smooth quadratic reflects previous control point", "[svg-d]") {
    // Q10 10 20 0 -> control (10,10), endpoint (20,0)
    // T30 0 -> reflected control = 2*(20,0) - (10,10) = (30,-10)
    auto strD = R"xxx(M0 0 Q10 10 20 0 T30 0)xxx";
    Draw2d::Svg::SvgDParser svgDParser {};
    auto [ pathMoves, points ] = svgDParser.parseD(strD);
    REQUIRE( pathMoves[2] == Draw2d::Svg::PathMove::QuadBézierTo );
    REQUIRE( points[6] == Catch::Approx(30.0f) );
    REQUIRE( points[7] == Catch::Approx(-10.0f) );
}

TEST_CASE("1r. smooth quadratic with no preceding quad degenerates to current point", "[svg-d]") {
    auto strD = R"xxx(M0 0 L10 10 T30 0)xxx";
    Draw2d::Svg::SvgDParser svgDParser {};
    auto [ pathMoves, points ] = svgDParser.parseD(strD);
    REQUIRE( points[4] == Catch::Approx(10.0f) );  // reflected control.x == current x
    REQUIRE( points[5] == Catch::Approx(10.0f) );  // reflected control.y == current y
}

TEST_CASE("1s. closepath followed by lineto continues from subpath start", "[svg-d]") {
    // After Z, current point resets to the subpath's start (0,0), so the
    // following lineto with no M should start from (0,0), not (10,10).
    auto strD = R"xxx(M0 0 L10 10 Z L5 5)xxx";
    Draw2d::Svg::SvgDParser svgDParser {};
    auto [ pathMoves, points ] = svgDParser.parseD(strD);
    // last segment's start implied at (0,0), endpoint (5,5)
    REQUIRE( points[points.size() - 2] == Catch::Approx(5.0f) );
    REQUIRE( points[points.size() - 1] == Catch::Approx(5.0f) );
}

TEST_CASE("1t. multiple subpaths via repeated moveto", "[svg-d]") {
    auto strD = R"xxx(M0 0 L10 10 M20 20 L30 30)xxx";
    Draw2d::Svg::SvgDParser svgDParser {};
    auto [ pathMoves, points ] = svgDParser.parseD(strD);
    REQUIRE( pathMoves.size() == 5 );
    REQUIRE( pathMoves[0] == Draw2d::Svg::PathMove::MoveTo );
    REQUIRE( pathMoves[2] == Draw2d::Svg::PathMove::MoveTo );
}

TEST_CASE("1u. glued negative numbers parse correctly", "[svg-d]") {
    // "10-5" must tokenize as two numbers: 10 and -5, no separator needed
    auto strD = R"xxx(M0 0 L10-5)xxx";
    Draw2d::Svg::SvgDParser svgDParser {};
    auto [ pathMoves, points ] = svgDParser.parseD(strD);
    REQUIRE( points[2] == Catch::Approx(10.0f) );
    REQUIRE( points[3] == Catch::Approx(-5.0f) );
}

TEST_CASE("1v. glued decimal numbers parse correctly", "[svg-d]") {
    // "1.5.5" must tokenize as 1.5 and .5, since a second decimal point starts a new number
    auto strD = R"xxx(M0 0 L1.5.5)xxx";
    Draw2d::Svg::SvgDParser svgDParser {};
    auto [ pathMoves, points ] = svgDParser.parseD(strD);
    REQUIRE( points[2] == Catch::Approx(1.5f) );
    REQUIRE( points[3] == Catch::Approx(0.5f) );
}

TEST_CASE("1w. whitespace and comma separators are interchangeable", "[svg-d]") {
    Draw2d::Svg::SvgDParser parserCommas {};
    Draw2d::Svg::SvgDParser parserSpaces {};
    auto [ movesC, pointsC ] = parserCommas.parseD(R"xxx(M0,0 L10,10,20,10)xxx");
    auto [ movesS, pointsS ] = parserSpaces.parseD(R"xxx(M0 0 L10 10 20 10)xxx");
    REQUIRE( pointsC.size() == pointsS.size() );
    for (std::size_t i = 0; i < pointsC.size(); i++)
        REQUIRE( pointsC[i] == Catch::Approx(pointsS[i]) );
}

TEST_CASE("1x. degenerate arc with zero radius becomes a straight line", "[svg-d]") {
    auto strD = R"xxx(M0 0 A0 20 15 0 0 20 20)xxx";
    Draw2d::Svg::SvgDParser svgDParser {};
    auto [ pathMoves, points ] = svgDParser.parseD(strD);
    REQUIRE( points[points.size() - 2] == Catch::Approx(20.0f) );
    REQUIRE( points[points.size() - 1] == Catch::Approx(20.0f) );
}

TEST_CASE("1y. empty d attribute produces no path moves", "[svg-d]") {
    auto strD = R"xxx()xxx";
    Draw2d::Svg::SvgDParser svgDParser {};
    auto [ pathMoves, points ] = svgDParser.parseD(strD);
    REQUIRE( pathMoves.size() == 0 );
    REQUIRE( points.size() == 0 );
}

TEST_CASE("1z. poo!!!!!!!", "[svg-d]") {
    auto strD = R"xxx(M33.541 23.198c.364-1.578.243-3.266-.458-4.946a8.018 8.018 0 0 0-3.271-3.773c.318-1.192.234-2.475-.324-3.75-.841-1.92-2.66-3.201-4.712-3.562.249-.572.329-1.289.036-2.167-1-3-5-1-8-4.999-2.44 1.464-2.97 3.64-2.878 5.487-2.421.412-3.8.936-3.8.936v.002a3.713 3.713 0 0 0-2.322 3.442c0 .879.318 1.676.828 2.312l-.692.258.001.003c-2.33.871-3.975 2.976-3.975 5.439 0 1.047.3 2.027.82 2.878C1.971 22.027 0 24.781 0 28c0 4.418 3.691 8 8.244 8 3.269 0 6.559-.703 9.531-1.665C20.018 35.375 23.47 36 28.667 36A7.333 7.333 0 0 0 36 28.667a7.31 7.31 0 0 0-2.459-5.469z)xxx";
    Draw2d::Svg::SvgDParser svgDParser {};
    auto [ pathMoves, points ] = svgDParser.parseD(strD);
    REQUIRE( true );
}

TEST_CASE("2a. relative flag does not leak into a following absolute command", "[svg-d][regression]") {
    // Regression test for the "currentPathMoveRelative never reset" bug:
    // once a lowercase (relative) command fires, every subsequent command
    // must still respect its OWN case, not inherit the previous one's.
    // L100 100 (abs) -> l10 10 (rel, ends at 110,110) -> L5 5 (abs, must
    // land exactly at 5,5, NOT be treated as relative-to-110,110).
    auto strD = R"xxx(M0 0 L100 100 l10 10 L5 5)xxx";
    Draw2d::Svg::SvgDParser svgDParser {};
    auto [ pathMoves, points ] = svgDParser.parseD(strD);
    REQUIRE( points[points.size() - 2] == Catch::Approx(5.0f) );
    REQUIRE( points[points.size() - 1] == Catch::Approx(5.0f) );
}

TEST_CASE("2b. absolute vertical lineto followed by absolute arc uses the correct current point", "[svg-d][regression]") {
    // Regression test for the "arc read x0/y0 from trailing array slots"
    // bug: a VerticalLineTo only stores one float, so an immediately
    // following arc must pull its start point from the tracked current
    // point (fCurrentX/fCurrentY), not from wherever the previous
    // primitive's array entries happen to sit.
    auto strD = R"xxx(M10 10 V50 A5 5 0 0 0 20 55)xxx";
    Draw2d::Svg::SvgDParser svgDParser {};
    auto [ pathMoves, points ] = svgDParser.parseD(strD);
    // Arc's true start is (10,50) -- if the bug were present, the arc's
    // internal start-point math would silently use stale/wrong coordinates,
    // and the flattened endpoint would drift far from the intended (20,55).
    REQUIRE( points[points.size() - 2] == Catch::Approx(20.0f).margin(0.5) );
    REQUIRE( points[points.size() - 1] == Catch::Approx(55.0f).margin(0.5) );
}

TEST_CASE("2c. arc-generated cubic control points retain sub-integer precision", "[svg-d][regression]") {
    // Regression test for the std::floor(x + 0.5) rounding bug in the
    // arc-to-cubic conversion. Tolerance is deliberately tight (0.01) --
    // the existing arc tests (1l/1m) use margin(1.0), which is wide enough
    // that rounding to the nearest integer would silently pass.
    auto strD = R"xxx(M7.818 1.469A1.07 1.07 0 0 0 7 2.471)xxx";
    Draw2d::Svg::SvgDParser svgDParser {};
    auto [ pathMoves, points ] = svgDParser.parseD(strD);
    REQUIRE( points[points.size() - 2] == Catch::Approx(7.0f).margin(0.01) );
    REQUIRE( points[points.size() - 1] == Catch::Approx(2.471f).margin(0.01) );
}

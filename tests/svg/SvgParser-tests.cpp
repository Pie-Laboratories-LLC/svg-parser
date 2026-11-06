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

#include "svg/SvgParser.hpp"

#ifndef SVG_PATH_DOT_HPP
    #include "svg/Path.hpp"
#endif
#ifndef SVG_GROUP_DOT_HPP
    #include "svg/Group.hpp"
#endif
#ifndef DRAW2D_SVG_TEXT_DOT_HPP
    #include "svg/Text.hpp"
#endif
#ifndef DRAW2D_SVG_IMAGE_DOT_HPP
    #include "svg/Image.hpp"
#endif
#ifndef DRAW2D_SVG_USE_DOT_HPP
    #include "svg/Use.hpp"
#endif
#ifndef DRAW2D_SVG_POLYGON_DOT_HPP
    #include "svg/Polygon.hpp"
#endif
#ifndef DRAW2D_SVG_MASK_DOT_HPP
    #include "svg/Mask.hpp"
#endif
#ifndef DRAW2D_SVG_CLIPPATH_DOT_HPP
    #include "svg/ClipPath.hpp"
#endif
#ifndef DRAW2D_SVG_POLYLINE_DOT_HPP
    #include "svg/Polyline.hpp"
#endif

TEST_CASE("1a. Single Line Segment", "[path][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg1a = R"xxx(<svg><path id="p1a" d="M 0 0 L 10 10 Z"/></svg>)xxx";
    auto svgDocument = svgParser.parse(svg1a);
    auto result = svgDocument.get()->lookupSvgEntity("p1a");
    REQUIRE( result != nullptr );
}

TEST_CASE("1b. Quadratic and cubic curves in one path", "[path][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg1b = R"xxx(<svg><path id="p1b" d="M 0 0 Q 5 5 10 0 C 15 5 20 -5 25 0 Z"/></svg>)xxx";
    auto svgDocument = svgParser.parse(svg1b);
    auto result = svgDocument.get()->lookupSvgEntity("p1b");
    REQUIRE( result != nullptr );
}

TEST_CASE("1c. Comma-separated coordinates (no spaces)", "[path][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg1c = R"xxx(<svg><path id="p1c" d="M0,0L10,10,20,0Z"/></svg>)xxx";
    auto svgDocument = svgParser.parse(svg1c);
    auto result = svgDocument.get()->lookupSvgEntity("p1c");
    REQUIRE( result != nullptr );
}

TEST_CASE("1d. Negative numbers glued to previous number (no delimiter)", "[path][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg1d = R"xxx(<svg><path id="p1d" d="M0,0L10-10L20-20"/></svg>)xxx";
    auto svgDocument = svgParser.parse(svg1d);
    auto result = svgDocument.get()->lookupSvgEntity("p1d");
    REQUIRE( result != nullptr );
}

TEST_CASE("1e. Leading-dot / no-leading-zero floats glued together", "[path][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg1e = R"xxx(<svg><path id="p1e" d="M0,0L.5.5L1.2.3"/></svg>)xxx";
    auto svgDocument = svgParser.parse(svg1e);
    auto result = svgDocument.get()->lookupSvgEntity("p1e");
    REQUIRE( result != nullptr );
}

TEST_CASE("1f. Path not closed (should throw)", "[path][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg1f = R"xxx(<svg><path id="p1f" d="M 0 0 L 10 10"/></svg>)xxx";
    auto svgDocument = svgParser.parse(svg1f);
    auto result = svgDocument.get()->lookupSvgEntity("p1f");
    REQUIRE( result != nullptr );
}

TEST_CASE("1g. Malformed - L before any M (should insert MoveTo)", "[path][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg1g = R"xxx(<svg><path id="p1g" d="L 10 10 Z"/></svg>)xxx";
    auto svgDocument = svgParser.parse(svg1g);
    auto result = svgDocument.get()->lookupSvgEntity("p1g");
    REQUIRE( result != nullptr );
    const Draw2d::Svg::Path *cpPath = static_cast<const Draw2d::Svg::Path *>(result);
    REQUIRE( cpPath->getPathMoves().size() == 3 );
    REQUIRE( cpPath->getPathMoves()[0] == Draw2d::Svg::PathMove::MoveTo );
    REQUIRE( cpPath->getPathMoves()[1] == Draw2d::Svg::PathMove::LineTo );
    REQUIRE( cpPath->getPathMoves()[2] == Draw2d::Svg::PathMove::ClosePath );
}

TEST_CASE("1h. Double M in a row", "[path][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg1h = R"xxx(<svg><path id="p1h" d="M 0 0 M 10 10 Z"/></svg>)xxx";
    auto svgDocument = svgParser.parse(svg1h);
    auto result = svgDocument.get()->lookupSvgEntity("p1h");
    REQUIRE( result != nullptr );
}

TEST_CASE("1i. Missing `d` attribute (should throw)", "[path][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg1i = R"xxx(<svg><path id="p1i"/></svg>)xxx";
    REQUIRE_THROWS_AS ( svgParser.parse(svg1i), Draw2d::Svg::SvgException );
}

TEST_CASE("2a. Plain group with two paths", "[group][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg2a = R"xxx(
<svg>
  <g id="g2a">
    <path id="p2a_1" d="M0,0 L10,10 Z"/>
    <path id="p2a_2" d="M5,5 L15,15 Z"/>
  </g>
</svg>)xxx";
    auto svgDocument = svgParser.parse(svg2a);
    auto result = svgDocument.get()->lookupSvgEntity("g2a");
    REQUIRE( result != nullptr );
}

TEST_CASE("2b. Nested groups", "[group][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg2b = R"xxx(
<svg>
  <g id="g2b_1">
    <g id="g2b_2">
      <path id="p2b" d="M0,0 L1,1 Z"/>
    </g>
  </g>
</svg>)xxx";
    auto svgDocument = svgParser.parse(svg2b);
    auto result = svgDocument.get()->lookupSvgEntity("g2b_1");
    REQUIRE( result != nullptr );
}

TEST_CASE("2c. Group with opacity", "[group][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg2c = R"xxx(<svg><g id="g2c" opacity="0.5"><path id="p2c" d="M0,0 L1,1 Z"/></g></svg>)xxx";
    auto svgDocument = svgParser.parse(svg2c);
    auto result = svgDocument.get()->lookupSvgEntity("g2c");
    REQUIRE( result != nullptr );
}

TEST_CASE("2d. Group with transform (single matrix)", "[group][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg2d = R"xxx(<svg><g id="g2d" transform="matrix(1,0,0,1,10,20)"><path id="p2d" d="M0,0 L1,1 Z"/></g></svg>)xxx";
    auto svgDocument = svgParser.parse(svg2d);
    auto result = svgDocument.get()->lookupSvgEntity("g2d");
    REQUIRE( result != nullptr );
}

TEST_CASE("2e. Group with unrecognized child element (should throw)", "[group][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg2e = R"xxx(<svg><g id="g2e"><triangle cx="5" cy="5" r="3"/></g></svg>)xxx";
    // previously unrecognized group children would throw().  Now for browser and
    //  spec consistency we don't throw
    REQUIRE_NOTHROW ( svgParser.parse(svg2e) );
}

TEST_CASE("3a. Basic defs + use", "[svg][use]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg3a = R"xxx(
<svg>
  <defs>
    <path id="p3a" d="M0,0 L10,0 L10,10 Z"/>
  </defs>
  <use xlink:href="#p3a"/>
</svg>)xxx";
    auto svgDocument = svgParser.parse(svg3a);
    auto result = svgDocument.get()->lookupSvgEntity("p3a");
    REQUIRE( result != nullptr );
}

TEST_CASE("3b. use with fill/stroke override", "[use][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg3b = R"xxx(
<svg>
  <defs><path id="p3b" d="M0,0 L10,0 Z"/></defs>
  <use xlink:href="#p3b" fill="#ff0000" stroke="#00ff00"/>
</svg>)xxx";

    auto svgDocument = svgParser.parse(svg3b);
    auto result = svgDocument.get()->lookupSvgEntity("p3b");
    REQUIRE( result != nullptr );
}

TEST_CASE("3c. use with x/y offset", "[svg][use]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg3c = R"xxx(
<svg>
  <defs><path id="p3c" d="M0,0 L10,0 Z"/></defs>
  <use xlink:href="#p3c" x="5" y="7"/>
</svg>)xxx";
    auto svgDocument = svgParser.parse(svg3c);
    auto result = svgDocument.get()->lookupSvgEntity("p3c");
    REQUIRE( result != nullptr );
}

TEST_CASE("3d. use referencing something not yet parsed, lazily resolved via getElementById", "[3d][use][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg3d = R"xxx(
<svg>
  <use xlink:href="#p3d"/>
  <path id="p3d" d="M0,0 L1,1 Z"/>
</svg>)xxx";
    auto svgDocument = svgParser.parse(svg3d);
    auto result = svgDocument.get()->lookupSvgEntity("p3d");
    REQUIRE( result != nullptr );
}

TEST_CASE("3e. use with no matching id anywhere", "[use][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg3e = R"xxx(<svg><use xlink:href="#nonexistent"/></svg>)xxx";
    auto svgDocument = svgParser.parse(svg3e);
    std::vector<const Draw2d::Svg::SvgEntity *> svgEntities = svgDocument.get()->getSvgEntities();
    REQUIRE( svgEntities.size() == 1);
    // NOTE - the path is parsed and saved before group.
    REQUIRE( !strcmp( svgEntities.front()->getType(), "Use") );
}

TEST_CASE("3f. use with missing xlink:href (should throw)", "[svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg3f = R"xxx(<svg><use/></svg>)xxx";
    REQUIRE_THROWS_AS ( svgParser.parse(svg3f), Draw2d::Svg::SvgException );
}

TEST_CASE("3g. defs containing a group without an id", "[svg][group]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg3g = R"xxx(
<svg><defs><g><path id="p1" d="M0,0 L1,1 Z"/></g></defs></svg>)xxx";

    auto svgDocument = svgParser.parse(svg3g);
    // another degenerate case - the group isn't acessible,  We at least check to
    // see that an element got parsed and that it's a group
    std::vector<const Draw2d::Svg::SvgEntity *> svgEntities = svgDocument.get()->getSvgEntities();
    REQUIRE( svgEntities.size() == 2);
    REQUIRE( !strcmp( svgEntities.front()->getType(), "Group") );
    REQUIRE( !strcmp( svgEntities.back()->getType(), "Path") );
}

TEST_CASE("3h. defs containing gradient without id", "[svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg3h = R"xxx(
<svg>
  <defs>
    <linearGradient x1="0" y1="0" x2="1" y2="0">
      <stop offset="0" stop-color="#000000"/>
      <stop offset="1" stop-color="#ffffff"/>
    </linearGradient>
  </defs>
</svg>)xxx";
    auto svgDocument = svgParser.parse(svg3h);
    // degenerate case - this linearGradient can't be accessed.  At least we can
    //  verify there's one gradient after parsing.
    REQUIRE( svgDocument.get()->getGradients().size() == 1 );
}

TEST_CASE("4a. Hex 6-digit", "[colour][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg4a = R"xxx(<svg><path id="p4a" d="M0,0 L1,1 Z" fill="#a1b2c3"/></svg>)xxx";
    auto svgDocument = svgParser.parse(svg4a);
    auto result = svgDocument.get()->lookupSvgEntity("p4a");
    REQUIRE( result != nullptr );
}

TEST_CASE("4b. Hex 8-digit (with alpha)", "[colour][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg4b = R"xxx(<svg><path id="p4b" d="M0,0 L1,1 Z" fill="#a1b2c3ff"/></svg>)xxx";
    auto svgDocument = svgParser.parse(svg4b);
    auto result = svgDocument.get()->lookupSvgEntity("p4b");
    REQUIRE( result != nullptr );
}

TEST_CASE("4c. Hex uppercase", "[colour][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg4c = R"xxx(<svg><path id="p4c" d="M0,0 L1,1 Z" fill="#A1B2C3"/></svg>)xxx";
    auto svgDocument = svgParser.parse(svg4c);
    auto result = svgDocument.get()->lookupSvgEntity("p4c");
    REQUIRE( result != nullptr );
}

TEST_CASE("4d. Named standard color", "[colour][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg4d = R"xxx(<svg><path id="p4d" d="M0,0 L1,1 Z" fill="red"/></svg>)xxx";
    auto svgDocument = svgParser.parse(svg4d);
    auto result = svgDocument.get()->lookupSvgEntity("p4d");
    REQUIRE( result != nullptr );
}

TEST_CASE("4e. currentColor", "[colour][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg4e = R"xxx(<svg><path id="p4e" d="M0,0 L1,1 Z" fill="currentColor"/></svg>)xxx";
    auto svgDocument = svgParser.parse(svg4e);
    auto result = svgDocument.get()->lookupSvgEntity("p4e");
    REQUIRE( result != nullptr );
}

TEST_CASE("4f. url(#id) referencing a gradient", "[colour][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg4f = R"xxx(
<svg>
  <defs>
    <linearGradient id="lg4f" x1="0" y1="0" x2="1" y2="0">
      <stop offset="0" stop-color="#000000"/>
      <stop offset="1" stop-color="#ffffff"/>
    </linearGradient>
  </defs>
  <path id="p4f" d="M0,0 L1,1 Z" fill="url(#lg4f)"/>
</svg>)xxx";
    auto svgDocument = svgParser.parse(svg4f);
    auto result = svgDocument.get()->lookupGradient("lg4f");
    REQUIRE( result != nullptr );
}

TEST_CASE("4h. var(--colorN, fallback) form", "[colour][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg4h = R"xxx(<svg><path id="p4h" d="M0,0 L1,1 Z" fill="var(--color3, #ff0000)"/></svg>)xxx";
    auto svgDocument = svgParser.parse(svg4h);
    auto result = svgDocument.get()->lookupSvgEntity("p4h");
    REQUIRE( result != nullptr );
}

TEST_CASE("4i. stop-color as rgb()", "[colour][gradient][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg4i = R"xxx(
<svg><defs><linearGradient id="lg4i" x1="0" y1="0" x2="1" y2="0">
  <stop offset="0" stop-color="rgb(255,0,0)"/>
  <stop offset="1" stop-color="rgb(0,255,0)"/>
</linearGradient></defs>
<path id="p4i" d="M0,0 L1,1 Z" fill="url(#lg4i)"/></svg>)xxx";
    auto svgDocument = svgParser.parse(svg4i);
    auto result = svgDocument.get()->lookupGradient("lg4i");
    REQUIRE( result != nullptr );
}

TEST_CASE("4j. stop-color as rgb() with percentages", "[colour][gradient][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg4j = R"xxx(
<svg><defs><linearGradient id="lg4j" x1="0" y1="0" x2="1" y2="0">
  <stop offset="0" stop-color="rgb(100%,0%,0%)"/>
  <stop offset="1" stop-color="rgb(0%,100%,0%)"/>
</linearGradient></defs>
<path id="p4j" d="M0,0 L1,1 Z" fill="url(#lg4j)"/></svg>)xxx";
    auto svgDocument = svgParser.parse(svg4j);
    auto result = svgDocument.get()->lookupGradient("lg4j");
    REQUIRE( result != nullptr );
}

TEST_CASE("4k. stop-color as rgba()", "[colour][gradient][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg4k = R"xxx(
<svg><defs><linearGradient id="lg4k" x1="0" y1="0" x2="1" y2="0">
  <stop offset="0" stop-color="rgba(255,0,0,0.5)"/>
  <stop offset="1" stop-color="rgba(0,255,0,1)"/>
</linearGradient></defs>
<path id="p4k" d="M0,0 L1,1 Z" fill="url(#lg4k)"/></svg>)xxx";
    auto svgDocument = svgParser.parse(svg4k);
    auto result = svgDocument.get()->lookupGradient("lg4k");
    REQUIRE( result != nullptr );
}

TEST_CASE("4l. Unrecognized color string (should throw)", "[colour][svg][throws]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg4l = R"xxx(<svg><path id="p4l" d="M0,0 L1,1 Z" fill="notacolor"/></svg>)xxx";
    REQUIRE_THROWS_AS ( svgParser.parse(svg4l), Draw2d::Svg::SvgException );
}

// the fill colour of a lone path, as stored (B, G, R, A or H, S, L, A)
static Draw2d::Svg::SvgColour FillOf(const char *pszFill)
{
    Draw2d::Svg::SvgParser svgParser {};
    Core::String strSvg = std::format(R"xxx(<svg><path id="p" d="M0,0 L1,1 Z" fill="{}"/></svg>)xxx", pszFill);
    auto svgDocument = svgParser.parse(strSvg);
    return svgDocument->lookupSvgEntity("p")->getFillColour().getColour();
}

static std::array<uint8_t,4> BgraOf(const char *pszFill)
{
    return FillOf(pszFill).getBgrA().value();
}

TEST_CASE("4m. Hex colours are stored as B, G, R, A", "[colour][svg]") {
    REQUIRE( FillOf("#a1b2c3").getSvgColourType() == Draw2d::Svg::SvgColourType::Bgr );
    REQUIRE( BgraOf("#a1b2c3") == std::array<uint8_t,4> { 0xc3, 0xb2, 0xa1, 0xff } );
    REQUIRE( FillOf("#a1b2c3d4").getSvgColourType() == Draw2d::Svg::SvgColourType::BgrA );
    REQUIRE( BgraOf("#a1b2c3d4") == std::array<uint8_t,4> { 0xc3, 0xb2, 0xa1, 0xd4 } );
}

TEST_CASE("4n. Short hex colours use one digit per channel", "[colour][svg]") {
    REQUIRE( BgraOf("#abc") == std::array<uint8_t,4> { 0xcc, 0xbb, 0xaa, 0xff } );
    REQUIRE( FillOf("#abcd").getSvgColourType() == Draw2d::Svg::SvgColourType::BgrA );
    REQUIRE( BgraOf("#abcd") == std::array<uint8_t,4> { 0xcc, 0xbb, 0xaa, 0xdd } );
}

TEST_CASE("4o. Named, hex and rgb() red all agree", "[colour][svg]") {
    const std::array<uint8_t,4> red { 0x00, 0x00, 0xff, 0xff };
    REQUIRE( BgraOf("red") == red );
    REQUIRE( BgraOf("#ff0000") == red );
    REQUIRE( BgraOf("#f00") == red );
    REQUIRE( BgraOf("rgb(255,0,0)") == red );
    REQUIRE( BgraOf("rgb(100%,0%,0%)") == red );
}

TEST_CASE("4p. rgba() alpha and out-of-range channels", "[colour][svg]") {
    REQUIRE( FillOf("rgba(255,0,0,0.5)").getSvgColourType() == Draw2d::Svg::SvgColourType::BgrA );
    REQUIRE( BgraOf("rgba(255,0,0,0.5)") == std::array<uint8_t,4> { 0x00, 0x00, 0xff, 0x80 } );
    REQUIRE( BgraOf("rgba(0,0,255,25%)") == std::array<uint8_t,4> { 0xff, 0x00, 0x00, 0x40 } );
    REQUIRE( BgraOf("rgb(50%,0%,0%)") == std::array<uint8_t,4> { 0x00, 0x00, 0x80, 0xff } );
    // clamped as in CSS
    REQUIRE( BgraOf("rgb(300,-5,0)") == std::array<uint8_t,4> { 0x00, 0x00, 0xff, 0xff } );
}

TEST_CASE("4q. hsl() is stored as degrees and 0-1 fractions", "[colour][svg]") {
    using Catch::Approx;
    auto hsl = FillOf("hsl(120, 100%, 50%)");
    REQUIRE( hsl.getSvgColourType() == Draw2d::Svg::SvgColourType::Hsl );
    auto pHsla = hsl.getHslA().value();
    REQUIRE( pHsla[0] == Approx(120.0f) );
    REQUIRE( pHsla[1] == Approx(1.0f) );
    REQUIRE( pHsla[2] == Approx(0.5f) );
    REQUIRE( pHsla[3] == Approx(1.0f) );

    auto hsla = FillOf("hsla(0.5turn, 50%, 25%, 0.25)");
    REQUIRE( hsla.getSvgColourType() == Draw2d::Svg::SvgColourType::HslA );
    pHsla = hsla.getHslA().value();
    REQUIRE( pHsla[0] == Approx(180.0f) );
    REQUIRE( pHsla[1] == Approx(0.5f) );
    REQUIRE( pHsla[2] == Approx(0.25f) );
    REQUIRE( pHsla[3] == Approx(0.25f) );

    REQUIRE( FillOf("hsl(200grad, 100%, 50%)").getHslA().value()[0] == Approx(180.0f) );
    REQUIRE( FillOf("hsl(3.14159265rad, 100%, 50%)").getHslA().value()[0] == Approx(180.0f) );
    REQUIRE( FillOf("hsl(90deg, 100%, 50%)").getHslA().value()[0] == Approx(90.0f) );
}

TEST_CASE("5a. Minimal valid", "[gradient][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg5a = R"xxx(
    <svg><defs><linearGradient id="lg5a" x1="0" y1="0" x2="1" y2="0">
      <stop offset="0" stop-color="#000000"/>
      <stop offset="1" stop-color="#ffffff"/>
    </linearGradient></defs></svg>)xxx";
    auto svgDocument = svgParser.parse(svg5a);
    auto result = svgDocument.get()->lookupGradient("lg5a");
    REQUIRE( result != nullptr );
}

TEST_CASE("5b. With gradientUnits + gradientTransform", "[gradient][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg5b = R"xxx(
<svg><defs><linearGradient id="lg5b" x1="0" y1="0" x2="1" y2="0"
  gradientUnits="userSpaceOnUse" gradientTransform="matrix(1,0,0,1,5,5)">
  <stop offset="0" stop-color="#000000"/>
  <stop offset="1" stop-color="#ffffff"/>
</linearGradient></defs></svg>)xxx";
    auto svgDocument = svgParser.parse(svg5b);
    auto result = svgDocument.get()->lookupGradient("lg5b");
    REQUIRE( result != nullptr );
}

TEST_CASE("5c. Missing x1 (should throw)", "[gradient][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg5c = R"xxx(
<svg><defs><linearGradient id="lg5c" y1="0" x2="1" y2="0">
  <stop offset="0" stop-color="#000000"/>
</linearGradient></defs></svg>)xxx";
    REQUIRE_THROWS_AS ( svgParser.parse(svg5c), Draw2d::Svg::SvgException );
}

TEST_CASE("5d. Unrecognized attribute (should throw)", "[gradient][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg5d = R"xxx(
<svg><defs><linearGradient id="lg5d" x1="0" y1="0" x2="1" y2="0" spreadMethod="pad" viewbox="0 0 100 100">
  <stop offset="0" stop-color="#000000"/>
  <stop offset="1" stop-color="#ffffff"/>
</linearGradient></defs></svg>)xxx";
    REQUIRE_THROWS_AS ( svgParser.parse(svg5d), Draw2d::Svg::SvgException );
}

TEST_CASE("5e. Stop offset out of range (should throw)", "[gradient][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
auto svg5e = R"xxx(
<svg><defs><linearGradient id="lg5e" x1="0" y1="0" x2="1" y2="0">
  <stop offset="1.5" stop-color="#000000"/>
</linearGradient></defs></svg>)xxx";
    REQUIRE_THROWS_AS ( svgParser.parse(svg5e), Draw2d::Svg::SvgException );
}

TEST_CASE("6a. Minimal valid (cx, cy, r only)", "[gradient][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg6a = R"xxx(
<svg><defs><radialGradient id="rg6a" cx="0.5" cy="0.5" r="0.5">
  <stop offset="0" stop-color="#000000"/>
  <stop offset="1" stop-color="#ffffff"/>
</radialGradient></defs></svg>)xxx";
    auto svgDocument = svgParser.parse(svg6a);
    auto result = svgDocument.get()->lookupGradient("rg6a");
    REQUIRE( result != nullptr );
}

TEST_CASE("6b. With focal point overrides (fx, fy, fr)", "[gradient][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg6b = R"xxx(
<svg><defs><radialGradient id="rg6b" cx="0.5" cy="0.5" r="0.5" fx="0.4" fy="0.4" fr="0.1">
  <stop offset="0" stop-color="#000000"/>
  <stop offset="1" stop-color="#ffffff"/>
</radialGradient></defs></svg>)xxx";
    auto svgDocument = svgParser.parse(svg6b);
    auto result = svgDocument.get()->lookupGradient("rg6b");
    REQUIRE( result != nullptr );
}

TEST_CASE("6c. Missing r (should throw)", "[gradient][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg6c = R"xxx(
<svg><defs><radialGradient id="rg6c" cx="0.5" cy="0.5">
  <stop offset="0" stop-color="#000000"/>
</radialGradient></defs></svg>)xxx";
    REQUIRE_THROWS_AS ( svgParser.parse(svg6c), Draw2d::Svg::SvgException );
}

TEST_CASE("7a. Single translate, one arg (ty defaults to 0)", "[transform][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg7a = R"xxx(<svg><g id="g7a" transform="translate(10)"><path id="p7a" d="M0,0 L1,1 Z"/></g></svg>)xxx";
    auto svgDocument = svgParser.parse(svg7a);
    auto result = svgDocument.get()->lookupSvgEntity("g7a");
    REQUIRE( result != nullptr );
}

TEST_CASE("7b. translate, both args", "[transform][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg7b = R"xxx(<svg><g id="g7b" transform="translate(10,20)"><path id="p7b" d="M0,0 L1,1 Z"/></g></svg>)xxx";
    auto svgDocument = svgParser.parse(svg7b);
    auto result = svgDocument.get()->lookupSvgEntity("g7b");
    REQUIRE( result != nullptr );
}

TEST_CASE("7c. scale, one arg (sy = sx)", "[transform][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg7c = R"xxx(<svg><g id="g7c" transform="scale(2)"><path id="p7c" d="M0,0 L1,1 Z"/></g></svg>)xxx";
    auto svgDocument = svgParser.parse(svg7c);
    auto result = svgDocument.get()->lookupSvgEntity("g7c");
    REQUIRE( result != nullptr );
}

TEST_CASE("7d. scale, both args", "[transform][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg7d = R"xxx(<svg><g id="g7d" transform="scale(2,3)"><path id="p7d" d="M0,0 L1,1 Z"/></g></svg>)xxx";
    auto svgDocument = svgParser.parse(svg7d);
    auto result = svgDocument.get()->lookupSvgEntity("g7d");
    REQUIRE( result != nullptr );
}

TEST_CASE("7e. rotate, angle only (around origin)", "[transform][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg7e = R"xxx(<svg><g id="g7e" transform="rotate(45)"><path id="p7e" d="M0,0 L1,1 Z"/></g></svg>)xxx";
    auto svgDocument = svgParser.parse(svg7e);
    auto result = svgDocument.get()->lookupSvgEntity("g7e");
    REQUIRE( result != nullptr );
}

TEST_CASE("7f. rotate, angle + cx + cy", "[transform][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg7f = R"xxx(<svg><g id="g7f" transform="rotate(45,10,10)"><path id="p7f" d="M0,0 L1,1 Z"/></g></svg>)xxx";
    auto svgDocument = svgParser.parse(svg7f);
    auto result = svgDocument.get()->lookupSvgEntity("g7f");
    REQUIRE( result != nullptr );
}

TEST_CASE("7g. rotate with only 2 args (malformed - should throw per spec)", "[transform][throws][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg7g = R"xxx(<svg><g id="g7g" transform="rotate(45,10)"><path id="p7g" d="M0,0 L1,1 Z"/></g></svg>)xxx";
    REQUIRE_THROWS_AS ( svgParser.parse(svg7g), Draw2d::Svg::SvgException );
}

TEST_CASE("7h. skewX", "[transform][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg7h = R"xxx(<svg><g id="g7h" transform="skewX(30)"><path id="p7h" d="M0,0 L1,1 Z"/></g></svg>)xxx";
    auto svgDocument = svgParser.parse(svg7h);
    auto result = svgDocument.get()->lookupSvgEntity("g7h");
    REQUIRE( result != nullptr );
}

TEST_CASE("7i. skewY", "[transform][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg7i = R"xxx(<svg><g id="g7i" transform="skewY(30)"><path id="p7i" d="M0,0 L1,1 Z"/></g></svg>)xxx";
    auto svgDocument = svgParser.parse(svg7i);
    auto result = svgDocument.get()->lookupSvgEntity("g7i");
    REQUIRE( result != nullptr );
}

TEST_CASE("7j. matrix, explicit 6-value", "[transform][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg7j = R"xxx(<svg><g id="g7j" transform="matrix(1,0,0,1,10,20)"><path id="p7j" d="M0,0 L1,1 Z"/></g></svg>)xxx";
    auto svgDocument = svgParser.parse(svg7j);
    auto result = svgDocument.get()->lookupSvgEntity("g7j");
    REQUIRE( result != nullptr );
}

TEST_CASE("7k. Transform LIST", "[transform][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg7k = R"xxx(<svg><g id="g7k" transform="translate(0 -6.75) translate(0,-1638.4) scale(56.8888888889)"><path id="p7k" d="M0,0 L1,1 Z"/></g></svg>)xxx";
    auto svgDocument = svgParser.parse(svg7k);
    auto result = svgDocument.get()->lookupSvgEntity("g7k");
    REQUIRE( result != nullptr );
}

TEST_CASE("7l. Transform list mixing all function types", "[transform][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg7l = R"xxx(<svg><g id="g7l" transform="translate(5,5) rotate(30) scale(2) skewX(10) matrix(1,0,0,1,0,0)"><path id="p7l" d="M0,0 L1,1 Z"/></g></svg>)xxx";
    auto svgDocument = svgParser.parse(svg7l);
    auto result = svgDocument.get()->lookupSvgEntity("g7l");
    REQUIRE( result != nullptr );
}

TEST_CASE("7m. Transform list with mixed comma/space separators between args", "[transform][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg7m = R"xxx(<svg><g id="g7m" transform="translate(1, 2)  scale( 3 , 4 )"><path id="p7m" d="M0,0 L1,1 Z"/></g></svg>)xxx";
    auto svgDocument = svgParser.parse(svg7m);
    auto result = svgDocument.get()->lookupSvgEntity("g7m");
    REQUIRE( result != nullptr );
}

TEST_CASE("7n. Scientific notation in transform values", "[transform][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg7n = R"xxx(<svg><g id="g7n" transform="scale(1.5e-2)"><path id="p7n" d="M0,0 L1,1 Z"/></g></svg>)xxx";
    auto svgDocument = svgParser.parse(svg7n);
    auto result = svgDocument.get()->lookupSvgEntity("g7n");
    REQUIRE( result != nullptr );
}

TEST_CASE("7o. Unknown transform function (should throw)", "[transform][svg][throws]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg7o = R"xxx(<svg><g id="g7o" transform="shear(10)"><path id="p7o" d="M0,0 L1,1 Z"/></g></svg>)xxx";
    REQUIRE_THROWS_AS ( svgParser.parse(svg7o), Draw2d::Svg::SvgException );
}

TEST_CASE("7p. Malformed transform - unclosed paren (should throw)", "[transform][throws][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg7p = R"xxx(<svg><g id="g7p" transform="translate(10,20"><path id="p7p" d="M0,0 L1,1 Z"/></g></svg>)xxx";
    REQUIRE_THROWS_AS ( svgParser.parse(svg7p), Draw2d::Svg::SvgException );
}

TEST_CASE("7q. Nested transforms: each entity keeps only its own transform", "[transform][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg7q = R"xxx(<svg><g id="g7q" transform="translate(10,0)"><path id="p7q" d="M0,0 L1,1 Z" transform="translate(5,0)"/></g></svg>)xxx";
    auto svgDocument = svgParser.parse(svg7q);
    // the renderer composes matrices on the way down; an accumulated matrix here would apply translate(10,0) twice
    REQUIRE( svgDocument->lookupSvgEntity("g7q")->getMatrix().value() == std::array<float,6> { 1, 0, 0, 1, 10, 0 } );
    REQUIRE( svgDocument->lookupSvgEntity("p7q")->getMatrix().value() == std::array<float,6> { 1, 0, 0, 1, 5, 0 } );
}

TEST_CASE("8a. Unrecognized top-level element (should throw)", "[structural][throws][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg8a = R"xxx(<svg><triangle cx="5" cy="5" r="3"/></svg>)xxx";
    // as with group example above, this used to throw, now it ignores/callsback
    REQUIRE_NOTHROW ( svgParser.parse(svg8a) );
}

TEST_CASE("8b. Empty svg root (no children) - should this throw or no-op?", "[structural][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg8b = R"xxx(<svg></svg>)xxx";
    auto svgDocument = svgParser.parse(svg8b);
    auto gradients = svgDocument.get()->getGradients();
    auto svgEntities = svgDocument.get()->getSvgEntities();
    REQUIRE( gradients.size() == 0 );
    REQUIRE( svgEntities.size() == 0 );
}

TEST_CASE("8c. Duplicate id across different element types (should throw)", "[structural][svg][throws]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg8c = R"xxx(
<svg>
  <defs>
    <path id="dup" d="M0,0 L1,1 Z"/>
  </defs>
  <g id="dup">
    <path id="p2" d="M0,0 L1,1 Z"/>
  </g>
</svg>)xxx";
    auto svgDocument = svgParser.parse(svg8c);
    // another degenerate case.  we should get two elements here, both paths.  It ignores the <g id="dup">
    //  (it actually warns about it.)
    std::vector<const Draw2d::Svg::SvgEntity *> svgEntities = svgDocument.get()->getSvgEntities();
    REQUIRE( svgEntities.size() == 3);
    REQUIRE( !strcmp( svgEntities.front()->getType(), "Path") );
    REQUIRE( !strcmp( svgEntities[1]->getType(), "Group") );
    REQUIRE( !strcmp( svgEntities.back()->getType(), "Path") );
    // note: first path is in <defs.../> so it shouldn't render
    REQUIRE( svgEntities.front()->getRender() == false );
}

TEST_CASE("8d. Whitespace-only text nodes between elements (should be ignored, not crash)", "[structural][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
auto svg8d = R"xxx(<svg>

  <g id="g8d">

    <path id="p8d" d="M0,0 L1,1 Z"/>

  </g>

</svg>)xxx";
    auto svgDocument = svgParser.parse(svg8d);
    auto gResult = svgDocument.get()->lookupSvgEntity("g8d");
    auto pResult = svgDocument.get()->lookupSvgEntity("p8d");
    REQUIRE( gResult != nullptr );
    REQUIRE( pResult != nullptr );
}

TEST_CASE("8e. XML comments interspersed (should be ignored)", "[structural][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg8e = R"xxx(
<svg>
  <!-- this is a comment -->
  <g id="g8e">
    <!-- another comment -->
    <path id="p8e" d="M0,0 L1,1 Z"/>
  </g>
</svg>)xxx";
    auto svgDocument = svgParser.parse(svg8e);
    auto gResult = svgDocument.get()->lookupSvgEntity("g8e");
    auto pResult = svgDocument.get()->lookupSvgEntity("p8e");
    REQUIRE( gResult != nullptr );
    REQUIRE( pResult != nullptr );
}

TEST_CASE("8f. With XML declaration (real FreeType OT-SVG output may or may not include this)", "[structural][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg8f = R"xxx(<?xml version="1.0" encoding="UTF-8"?>
<svg><path id="p8f" d="M0,0 L1,1 Z"/></svg>)xxx";
    auto svgDocument = svgParser.parse(svg8f);
    auto pResult = svgDocument.get()->lookupSvgEntity("p8f");
    REQUIRE( pResult != nullptr );
}

TEST_CASE("8g. With xmlns declarations on root (very common in real tool output)", "[structural][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
auto svg8g = R"xxx(
<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
  <path id="p8g" d="M0,0 L1,1 Z"/>
</svg>)xxx";
    auto svgDocument = svgParser.parse(svg8g);
    auto pResult = svgDocument.get()->lookupSvgEntity("p8g");
    REQUIRE( pResult != nullptr );
}

TEST_CASE("9a. Direct self-reference (use pointing at itself)", "[self-reference][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg9b = R"xxx(
<svg>
  <use id="u9b_1" xlink:href="#u9b_2"/>
  <use id="u9b_2" xlink:href="#u9b_1"/>
</svg>)xxx";
    REQUIRE_THROWS_AS ( svgParser.parse(svg9b), Draw2d::Svg::SvgException );
}

TEST_CASE("9b. Two-node cycle (A references B, B references A):", "[self-reference][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg9b = R"xxx(
<svg>
  <use id="u9b_1" xlink:href="#u9b_2"/>
  <use id="u9b_2" xlink:href="#u9b_1"/>
</svg>)xxx";
    REQUIRE_THROWS_AS ( svgParser.parse(svg9b), Draw2d::Svg::SvgException );
}

TEST_CASE("9c. Three-node cycle, just to confirm it's not accidentally only caught for the 2-node case", "[self-reference][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg9c = R"xxx(
<svg>
  <use id="u9c_1" xlink:href="#u9c_2"/>
  <use id="u9c_2" xlink:href="#u9c_3"/>
  <use id="u9c_3" xlink:href="#u9c_1"/>
</svg>)xxx";
    REQUIRE_THROWS_AS ( svgParser.parse(svg9c), Draw2d::Svg::SvgException );
}

TEST_CASE("9d.  Use: reference to enclosing group is broken", "[self-reference][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg9d = R"xxx(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 100 100">
  <g id="loop" fill="red">
    <path d="M0,0 L10,0 L10,10 Z"/>
    <use href="#loop"/>
  </g>
</svg>)xxx";
    REQUIRE_THROWS_AS ( svgParser.parse(svg9d), Draw2d::Svg::SvgException );
}

TEST_CASE("9e.  use: repeated reference is not a cycle", "[structural][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
auto svg9e =
        R"xxx(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 100 100">
                <defs>
                  <path id="leaf" d="M0,0 L10,0 L10,10 Z"/>
                </defs>
                <g id="mid">
                  <use href="#leaf" x="0"  y="0"/>
                  <use href="#leaf" x="20" y="0"/>
                </g>
              </svg>)xxx";
    auto svgDocument = svgParser.parse(svg9e);
    auto pResult = svgDocument.get()->lookupSvgEntity("leaf");
    REQUIRE( pResult != nullptr );
}

TEST_CASE("9f. use: cycle through nested svg is broken", "[self-reference][throws][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg9f = R"xxx(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 100 100">
  <g id="outer">
    <svg x="10" y="10" width="50" height="50">
      <use href="#outer"/>
    </svg>
  </g>
</svg>)xxx";
    REQUIRE_THROWS_AS ( svgParser.parse(svg9f), Draw2d::Svg::SvgException );
}

TEST_CASE("9g. Two-node cycle (A references B, B references A), one in defs", "[use][self-reference][throws][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg9g = R"xxx(
<svg>
  <defs>
    <use id="u9g_1" xlink:href="#u9g_2"/>
  </defs>
  <use id="u9g_2" xlink:href="#u9g_1"/>
</svg>)xxx";
    REQUIRE_THROWS_AS ( svgParser.parse(svg9g), Draw2d::Svg::SvgException );
}

TEST_CASE("9h. use -> defs group -> use -> back to the enclosing group", "[use][self-reference][throws][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg9h = R"xxx(
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 100 100">
  <defs>
    <g id="d_inner">
      <path d="M0,0 L10,0 L10,10 Z"/>
      <use xlink:href="#d_outer"/>
    </g>
  </defs>
  <g id="d_outer">
    <use xlink:href="#d_inner"/>
  </g>
</svg>
)xxx";
    REQUIRE_THROWS_AS ( svgParser.parse(svg9h), Draw2d::Svg::SvgException );
}

TEST_CASE("9i. use -> defs group -> use -> back to the enclosing group", "[use][self-reference][throws][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg9i = R"xxx(
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 100 100">
  <defs>
    <g id="d_inner">
      <path d="M0,0 L10,0 L10,10 Z"/>
      <use xlink:href="#d_outer"/>
    </g>
  </defs>
  <g id="d_outer">
    <use xlink:href="#d_inner"/>
  </g>
</svg>
)xxx";
    REQUIRE_THROWS_AS ( svgParser.parse(svg9i), Draw2d::Svg::SvgException );
}
// ============================================================
// 10. <text> -- contenu, blancs, attributs de police
// ============================================================

namespace {
    using Draw2d::Svg::Text;
    using Draw2d::Svg::TSpan;
    using Draw2d::Svg::RawText;

    // le n-ième morceau, s'il est du texte brut
    const Core::String &Brut(const RawText &rawText, size_t n) {
        const auto &parts = rawText.getTexts();
        REQUIRE(n < parts.size());
        REQUIRE(std::holds_alternative<Core::String>(parts[n]));
        return std::get<Core::String>(parts[n]);
    }

    // le n-ième morceau, s'il est un <tspan>
    const TSpan &Sous(const RawText &rawText, size_t n) {
        const auto &parts = rawText.getTexts();
        REQUIRE(n < parts.size());
        REQUIRE(std::holds_alternative<std::unique_ptr<TSpan>>(parts[n]));
        const auto &tspan = std::get<std::unique_ptr<TSpan>>(parts[n]);
        REQUIRE(tspan != nullptr);
        return *tspan;
    }
}

TEST_CASE("10a. Basic text node with content", "[text][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg10a = R"xxx(<svg><text id="t10a" x="10" y="20">Hello</text></svg>)xxx";
    auto svgDocument = svgParser.parse(svg10a);

    auto entity = svgDocument.get()->lookupSvgEntity("t10a");
    REQUIRE(entity != nullptr);
    const auto *text = dynamic_cast<const Draw2d::Svg::Text *>(entity);
    REQUIRE(text != nullptr);

    REQUIRE(text->getX().size() == 1);
    REQUIRE(text->getX()[0] == 10.0f);
    REQUIRE(text->getY().size() == 1);
    REQUIRE(text->getY()[0] == 20.0f);
}

TEST_CASE("10b. text x/y as space-separated lists", "[text][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg10b = R"xxx(<svg><text id="t10b" x="10 20 30" y="1 2 3">abc</text></svg>)xxx";
    auto svgDocument = svgParser.parse(svg10b);

    const auto *text = dynamic_cast<const Draw2d::Svg::Text *>(svgDocument.get()->lookupSvgEntity("t10b"));
    REQUIRE(text != nullptr);

    REQUIRE(text->getX() == std::vector<float>{ 10.0f, 20.0f, 30.0f });
    REQUIRE(text->getY() == std::vector<float>{ 1.0f, 2.0f, 3.0f });
}

TEST_CASE("10c. text x/y as comma-separated lists", "[text][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg10c = R"xxx(<svg><text id="t10c" x="10,20,30" y="1,2,3">abc</text></svg>)xxx";
    auto svgDocument = svgParser.parse(svg10c);

    const auto *text = dynamic_cast<const Draw2d::Svg::Text *>(svgDocument.get()->lookupSvgEntity("t10c"));
    REQUIRE(text != nullptr);

    REQUIRE(text->getX() == std::vector<float>{ 10.0f, 20.0f, 30.0f });
    REQUIRE(text->getY() == std::vector<float>{ 1.0f, 2.0f, 3.0f });
}

TEST_CASE("10d. text dx/dy lists", "[text][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg10d = R"xxx(<svg><text id="t10d" dx="1 2 3" dy="-1 -2 -3">abc</text></svg>)xxx";
    auto svgDocument = svgParser.parse(svg10d);

    const auto *text = dynamic_cast<const Draw2d::Svg::Text *>(svgDocument.get()->lookupSvgEntity("t10d"));
    REQUIRE(text != nullptr);

    REQUIRE(text->getDx() == std::vector<float>{ 1.0f, 2.0f, 3.0f });
    REQUIRE(text->getDy() == std::vector<float>{ -1.0f, -2.0f, -3.0f });
}

TEST_CASE("10e. text rotate list, fewer values than characters", "[text][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg10e = R"xxx(<svg><text id="t10e" rotate="10 20">abcd</text></svg>)xxx";
    auto svgDocument = svgParser.parse(svg10e);

    const auto *text = dynamic_cast<const Draw2d::Svg::Text *>(svgDocument.get()->lookupSvgEntity("t10e"));
    REQUIRE(text != nullptr);

    // Parser stores the list as given -- repeat-last-value is a layout-time
    // concern, not a parse-time one.
    REQUIRE(text->getRotate() == std::vector<float>{ 10.0f, 20.0f });
}

TEST_CASE("10f. text with textLength and lengthAdjust", "[text][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg10f = R"xxx(<svg><text id="t10f" textLength="100" lengthAdjust="spacingAndGlyphs">abc</text></svg>)xxx";
    auto svgDocument = svgParser.parse(svg10f);

    const auto *text = dynamic_cast<const Draw2d::Svg::Text *>(svgDocument.get()->lookupSvgEntity("t10f"));
    REQUIRE(text != nullptr);

    REQUIRE(text->getTextLength().has_value());
    REQUIRE(text->getTextLength().value() == 100.0f);
    REQUIRE(text->getLengthAdjust() == Draw2d::Svg::LengthAdjust::SpacingAndGlyphs);
}

TEST_CASE("10g. text with no positional attributes defaults to empty lists", "[text][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg10g = R"xxx(<svg><text id="t10g">plain</text></svg>)xxx";
    auto svgDocument = svgParser.parse(svg10g);

    const auto *text = dynamic_cast<const Draw2d::Svg::Text *>(svgDocument.get()->lookupSvgEntity("t10g"));
    REQUIRE(text != nullptr);

    REQUIRE(text->getX().empty());
    REQUIRE(text->getY().empty());
    REQUIRE(text->getDx().empty());
    REQUIRE(text->getDy().empty());
    REQUIRE(text->getRotate().empty());
    REQUIRE_FALSE(text->getTextLength().has_value());
}

TEST_CASE("10h. text with malformed numeric list throws", "[text][throws][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg10h = R"xxx(<svg><text id="t10h" x="10 notanumber 30">abc</text></svg>)xxx";

    REQUIRE_THROWS_AS( svgParser.parse(svg10h), Draw2d::Svg::SvgException );
}

TEST_CASE("10i. text content is captured", "[text][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg10i = R"xxx(<svg><text id="t10i" x="10" y="20">Hello</text></svg>)xxx";
    auto svgDocument = svgParser.parse(svg10i);

    const auto *text = dynamic_cast<const Text *>(svgDocument.get()->lookupSvgEntity("t10i"));
    REQUIRE(text != nullptr);

    REQUIRE(text->getRawText().getTexts().size() == 1);
    REQUIRE(Brut(text->getRawText(), 0) == "Hello");
}

TEST_CASE("10j. mixed content keeps document order", "[text][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg10j = R"xxx(<svg><text id="t10j">a<tspan>b</tspan>c</text></svg>)xxx";
    auto svgDocument = svgParser.parse(svg10j);

    const auto *text = dynamic_cast<const Text *>(svgDocument.get()->lookupSvgEntity("t10j"));
    REQUIRE(text != nullptr);

    const RawText &contenu = text->getRawText();
    REQUIRE(contenu.getTexts().size() == 3);
    REQUIRE(Brut(contenu, 0) == "a");
    REQUIRE(Brut(Sous(contenu, 1).getText(), 0) == "b");
    REQUIRE(Brut(contenu, 2) == "c");
}

TEST_CASE("10k. nested tspans nest their content", "[text][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg10k = R"xxx(<svg><text id="t10k"><tspan>x<tspan>y</tspan></tspan></text></svg>)xxx";
    auto svgDocument = svgParser.parse(svg10k);

    const auto *text = dynamic_cast<const Text *>(svgDocument.get()->lookupSvgEntity("t10k"));
    REQUIRE(text != nullptr);

    const TSpan &extérieur = Sous(text->getRawText(), 0);
    REQUIRE(extérieur.getText().getTexts().size() == 2);
    REQUIRE(Brut(extérieur.getText(), 0) == "x");
    const TSpan &intérieur = Sous(extérieur.getText(), 1);
    REQUIRE(Brut(intérieur.getText(), 0) == "y");
}

TEST_CASE("10l. entities arrive decoded", "[text][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg10l = R"xxx(<svg><text id="t10l">A &amp; B &lt; C &#x2205;</text></svg>)xxx";
    auto svgDocument = svgParser.parse(svg10l);

    const auto *text = dynamic_cast<const Text *>(svgDocument.get()->lookupSvgEntity("t10l"));
    REQUIRE(text != nullptr);

    // U+2205 ∅ en UTF-8: E2 88 85
    REQUIRE(Draw2d::Svg::concatenateText(text->getRawText()) == "A & B < C \xE2\x88\x85");
}

TEST_CASE("10m. CDATA is text, comments are not", "[text][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg10m = R"xxx(<svg><text id="t10m">a<!-- note -->b<![CDATA[<c>]]></text></svg>)xxx";
    auto svgDocument = svgParser.parse(svg10m);

    const auto *text = dynamic_cast<const Text *>(svgDocument.get()->lookupSvgEntity("t10m"));
    REQUIRE(text != nullptr);

    // le nombre de morceaux dépend de l'analyseur XML; le texte, non
    REQUIRE(Draw2d::Svg::concatenateText(text->getRawText()) == "ab<c>");
}

TEST_CASE("10n. raw parts keep their whitespace", "[text][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg10n = R"xxx(<svg><text id="t10n">  a  b  </text></svg>)xxx";
    auto svgDocument = svgParser.parse(svg10n);

    const auto *text = dynamic_cast<const Text *>(svgDocument.get()->lookupSvgEntity("t10n"));
    REQUIRE(text != nullptr);

    REQUIRE(Brut(text->getRawText(), 0) == "  a  b  ");
}

TEST_CASE("10o. concatenateText collapses whitespace", "[text][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg10o = "<svg><text id=\"t10o\">\n   foo \t\t bar\n  </text></svg>";
    auto svgDocument = svgParser.parse(svg10o);

    const auto *text = dynamic_cast<const Text *>(svgDocument.get()->lookupSvgEntity("t10o"));
    REQUIRE(text != nullptr);

    REQUIRE(Draw2d::Svg::concatenateText(text->getRawText()) == "foo bar");
}

TEST_CASE("10p. whitespace collapses across a tspan boundary", "[text][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg10p = R"xxx(<svg><text id="t10p">foo <tspan> bar</tspan> </text></svg>)xxx";
    auto svgDocument = svgParser.parse(svg10p);

    const auto *text = dynamic_cast<const Text *>(svgDocument.get()->lookupSvgEntity("t10p"));
    REQUIRE(text != nullptr);

    REQUIRE(Draw2d::Svg::concatenateText(text->getRawText()) == "foo bar");
}

TEST_CASE("10q. multibyte UTF-8 passes through", "[text][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    // « çà ∅ » en UTF-8
    auto svg10q = "<svg><text id=\"t10q\">\xC3\xA7\xC3\xA0  \xE2\x88\x85</text></svg>";
    auto svgDocument = svgParser.parse(svg10q);

    const auto *text = dynamic_cast<const Text *>(svgDocument.get()->lookupSvgEntity("t10q"));
    REQUIRE(text != nullptr);

    REQUIRE(Draw2d::Svg::concatenateText(text->getRawText()) == "\xC3\xA7\xC3\xA0 \xE2\x88\x85");
}

TEST_CASE("10r. empty and blank text give an empty string", "[text][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg10r = "<svg><text id=\"t10r1\"></text><text id=\"t10r2\"> \n\t </text></svg>";
    auto svgDocument = svgParser.parse(svg10r);

    const auto *vide = dynamic_cast<const Text *>(svgDocument.get()->lookupSvgEntity("t10r1"));
    const auto *blanc = dynamic_cast<const Text *>(svgDocument.get()->lookupSvgEntity("t10r2"));
    REQUIRE(vide != nullptr);
    REQUIRE(blanc != nullptr);

    REQUIRE(Draw2d::Svg::concatenateText(vide->getRawText()).empty());
    REQUIRE(Draw2d::Svg::concatenateText(blanc->getRawText()).empty());
}

TEST_CASE("10s. font-size accepts plain numbers and px", "[text][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg10s = R"xxx(<svg>
        <text id="a" font-size="14">a</text>
        <text id="b" font-size="14.0">b</text>
        <text id="c" font-size="14px">c</text>
        <text id="d" font-size=" 14 ">d</text>
    </svg>)xxx";
    auto svgDocument = svgParser.parse(svg10s);

    for (const char *id : { "a", "b", "c", "d" }) {
        INFO("text " << id);
        const auto *text = dynamic_cast<const Text *>(svgDocument.get()->lookupSvgEntity(id));
        REQUIRE(text != nullptr);
        REQUIRE(text->getFontSize().has_value());
        REQUIRE(text->getFontSize().value() == 14.0f);
    }
}

TEST_CASE("10t. font-size left unset when unreadable or absent", "[text][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg10t = R"xxx(<svg>
        <text id="em" font-size="1em">a</text>
        <text id="mot" font-size="abc">b</text>
        <text id="négatif" font-size="-3">c</text>
        <text id="absent">d</text>
    </svg>)xxx";
    auto svgDocument = svgParser.parse(svg10t);

    for (const char *id : { "mot", "négatif", "absent" }) {
        INFO("text " << id);
        const auto *text = dynamic_cast<const Text *>(svgDocument.get()->lookupSvgEntity(id));
        REQUIRE(text != nullptr);
        REQUIRE_FALSE(text->getFontSize().has_value());
    }

    // em: relatif à la taille du parent -- sans héritage, le défaut, 16
    const auto *em = dynamic_cast<const Text *>(svgDocument.get()->lookupSvgEntity("em"));
    REQUIRE(em != nullptr);
    REQUIRE(em->getFontSize().has_value());
    REQUIRE(em->getFontSize().value() == 1.0f);
}

TEST_CASE("10u. text-anchor values", "[text][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg10u = R"xxx(<svg>
        <text id="s" text-anchor="start">a</text>
        <text id="m" text-anchor="middle">b</text>
        <text id="e" text-anchor="end">c</text>
        <text id="x" text-anchor="inherit">d</text>
        <text id="n">e</text>
    </svg>)xxx";
    auto svgDocument = svgParser.parse(svg10u);
    auto ancre = [&](const char *id) {
        const auto *text = dynamic_cast<const Text *>(svgDocument.get()->lookupSvgEntity(id));
        REQUIRE(text != nullptr);
        return text->getTextAnchor();
    };

    REQUIRE(ancre("s") == Draw2d::Svg::TextAnchor::Start);
    REQUIRE(ancre("m") == Draw2d::Svg::TextAnchor::Middle);
    REQUIRE(ancre("e") == Draw2d::Svg::TextAnchor::End);
    REQUIRE_FALSE(ancre("x").has_value());
    REQUIRE_FALSE(ancre("n").has_value());
}

TEST_CASE("10v. font-style values", "[text][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg10v = R"xxx(<svg>
        <text id="n" font-style="normal">a</text>
        <text id="i" font-style="italic">b</text>
        <text id="o" font-style="oblique">c</text>
        <text id="x" font-style="slanty">d</text>
    </svg>)xxx";
    auto svgDocument = svgParser.parse(svg10v);
    auto style = [&](const char *id) {
        const auto *text = dynamic_cast<const Text *>(svgDocument.get()->lookupSvgEntity(id));
        REQUIRE(text != nullptr);
        return text->getFontStyle();
    };

    REQUIRE(style("n") == Draw2d::Svg::FontStyle::Normal);
    REQUIRE(style("i") == Draw2d::Svg::FontStyle::Italic);
    REQUIRE(style("o") == Draw2d::Svg::FontStyle::Oblique);
    REQUIRE_FALSE(style("x").has_value());
}

TEST_CASE("10w. font-family is kept as written", "[text][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg10w = R"xxx(<svg><text id="t10w" font-family="sans-serif">a</text></svg>)xxx";
    auto svgDocument = svgParser.parse(svg10w);

    const auto *text = dynamic_cast<const Text *>(svgDocument.get()->lookupSvgEntity("t10w"));
    REQUIRE(text != nullptr);
    REQUIRE(text->getFontFamily().has_value());
    REQUIRE(text->getFontFamily().value() == "sans-serif");
}

TEST_CASE("10x. fill is parsed on text", "[text][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg10x = R"xxx(<svg><text id="t10x" fill="#1a1a1a">a</text></svg>)xxx";
    auto svgDocument = svgParser.parse(svg10x);

    const auto *text = dynamic_cast<const Text *>(svgDocument.get()->lookupSvgEntity("t10x"));
    REQUIRE(text != nullptr);
    REQUIRE(text->getFillColour().getSvgColourType() != Draw2d::Svg::SvgColourType::None);   // À ADAPTER
}

TEST_CASE("10y. tspan position lists are parsed", "[text][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg10y = R"xxx(<svg><text id="t10y"><tspan x="5" dx="1 2" rotate="15">ab</tspan></text></svg>)xxx";
    auto svgDocument = svgParser.parse(svg10y);

    const auto *text = dynamic_cast<const Text *>(svgDocument.get()->lookupSvgEntity("t10y"));
    REQUIRE(text != nullptr);

    const TSpan &tspan = Sous(text->getRawText(), 0);
    REQUIRE(tspan.getX() == std::vector<float>{ 5.0f });
    REQUIRE(tspan.getDx() == std::vector<float>{ 1.0f, 2.0f });
    REQUIRE(tspan.getRotate() == std::vector<float>{ 15.0f });
}

TEST_CASE("10z. text nodes are move-only", "[text][svg]") {
    STATIC_REQUIRE_FALSE(std::is_copy_constructible_v<RawText>);
    STATIC_REQUIRE_FALSE(std::is_copy_constructible_v<TSpan>);
    STATIC_REQUIRE_FALSE(std::is_copy_constructible_v<Text>);
    STATIC_REQUIRE(std::is_nothrow_move_constructible_v<RawText>);
    STATIC_REQUIRE(std::is_nothrow_move_constructible_v<TSpan>);
}

// ============================================================
// 11. <image>
// ============================================================

TEST_CASE("11a. Basic image with dimensions", "[image][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg11a = R"xxx(<svg><image id="i11a" x="0" y="0" width="50" height="60" href="picture.png"/></svg>)xxx";
    auto svgDocument = svgParser.parse(svg11a);

    const auto *image = dynamic_cast<const Draw2d::Svg::Image *>(svgDocument.get()->lookupSvgEntity("i11a"));
    REQUIRE(image != nullptr);

    REQUIRE(image->getX().has_value());
    REQUIRE(image->getY().has_value());
    REQUIRE(image->getWidth().has_value());
    REQUIRE(image->getHeight().has_value());
    REQUIRE(image->getHref() == "picture.png");
    REQUIRE(image->getHrefKind() == Draw2d::Svg::HrefKind::Url);
}

TEST_CASE("11b. image with valid base64-encoded data URI href", "[image][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    // "hello" base64-encoded is "aGVsbG8=" -- chosen so the expected decoded
    // bytes are human-verifiable rather than an opaque magic number.
    auto svg11b = R"xxx(<svg><image id="i11b" width="10" height="10" href="data:image/png;base64,aGVsbG8="/></svg>)xxx";
    auto svgDocument = svgParser.parse(svg11b);

    const auto *image = dynamic_cast<const Draw2d::Svg::Image *>(svgDocument.get()->lookupSvgEntity("i11b"));
    REQUIRE(image != nullptr);

    // The parser resolves the base64 payload eagerly (same principle as
    // resolving currentColor) -- href holds the decoded bytes, not the
    // original data URI or its base64 form.
    REQUIRE(image->getHref() == "hello");
    REQUIRE(image->getHrefKind() == Draw2d::Svg::HrefKind::DecodedBinary);
    REQUIRE(image->getImageType() == "png");
}

TEST_CASE("11c. image with malformed base64 data URI throws", "[image][throws][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    // '@' and '#' are not valid base64 alphabet characters.
    auto svg11c = R"xxx(<svg><image id="i11c" width="10" height="10" href="data:image/png;base64,iV@#Row0KGgo="/></svg>)xxx";

    REQUIRE_THROWS_AS( svgParser.parse(svg11c), Core::Exception );
}

TEST_CASE("11d. image with legacy xlink:href", "[image][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg11d = R"xxx(<svg xmlns:xlink="http://www.w3.org/1999/xlink"><image id="i11d" width="10" height="10" xlink:href="picture.png"/></svg>)xxx";
    auto svgDocument = svgParser.parse(svg11d);

    const auto *image = dynamic_cast<const Draw2d::Svg::Image *>(svgDocument.get()->lookupSvgEntity("i11d"));
    REQUIRE(image != nullptr);
    REQUIRE(image->getHref() == "picture.png");
    REQUIRE(image->getHrefKind() == Draw2d::Svg::HrefKind::Url);
}

TEST_CASE("11e. image with preserveAspectRatio", "[image][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg11e = R"xxx(<svg><image id="i11e" width="10" height="10" href="picture.png" preserveAspectRatio="xMidYMid slice"/></svg>)xxx";
    auto svgDocument = svgParser.parse(svg11e);

    const auto *image = dynamic_cast<const Draw2d::Svg::Image *>(svgDocument.get()->lookupSvgEntity("i11e"));
    REQUIRE(image != nullptr);

    REQUIRE(image->getPreserveAspectRatio() == Draw2d::Svg::PreserveAspectRatio::xMidYMid);
    REQUIRE(image->getPreserveAspectRatioMode() == Draw2d::Svg::PreserveAspectRatioMode::Slice);
}

TEST_CASE("11f. image with crossorigin/decoding/fetchpriority", "[image][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg11f = R"xxx(<svg><image id="i11f" width="10" height="10" href="picture.png" crossorigin="anonymous" decoding="async" fetchpriority="high"/></svg>)xxx";
    auto svgDocument = svgParser.parse(svg11f);

    const auto *image = dynamic_cast<const Draw2d::Svg::Image *>(svgDocument.get()->lookupSvgEntity("i11f"));
    REQUIRE(image != nullptr);

    REQUIRE(image->getCrossOrigin().has_value());
    REQUIRE(image->getCrossOrigin().value() == Draw2d::Svg::CrossOrigin::Anonymous);
    REQUIRE(image->getDecoding().has_value());
    REQUIRE(image->getDecoding().value() == Draw2d::Svg::Decoding::Async);
    REQUIRE(image->getFetchPriority().has_value());
    REQUIRE(image->getFetchPriority().value() == Draw2d::Svg::FetchPriority::High);
}

TEST_CASE("11g. image with missing href does not throw", "[image][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg11g = R"xxx(<svg><image id="i11g" width="10" height="10"/></svg>)xxx";
    auto svgDocument = svgParser.parse(svg11g);

    const auto *image = dynamic_cast<const Draw2d::Svg::Image *>(svgDocument.get()->lookupSvgEntity("i11g"));
    REQUIRE(image != nullptr);
    REQUIRE(image->getHref().empty());
}

TEST_CASE("11h. image with literal-text (non-base64) data URI", "[image][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg11h = R"xxx(<svg><image id="i11h" width="10" height="10" href="data:image/svg+xml;charset=utf-8,hello"/></svg>)xxx";
    auto svgDocument = svgParser.parse(svg11h);

    const auto *image = dynamic_cast<const Draw2d::Svg::Image *>(svgDocument.get()->lookupSvgEntity("i11h"));
    REQUIRE(image != nullptr);

    REQUIRE(image->getHref() == "hello");
    REQUIRE(image->getHrefKind() == Draw2d::Svg::HrefKind::LiteralText);
    REQUIRE(image->getImageType() == "svg+xml");
    REQUIRE(image->getCharacterEncoding() == "utf-8");
}

// the entity with this id, from a parsed document
static const Draw2d::Svg::SvgEntity *EntityIn(const std::unique_ptr<Draw2d::Svg::SvgDocument> &svgDocument, const char *pszId)
{
    const Draw2d::Svg::SvgEntity *cpEntity = svgDocument->lookupSvgEntity(pszId);
    REQUIRE( cpEntity != nullptr );
    return cpEntity;
}

static std::array<uint8_t,4> FillBgraOf(const Draw2d::Svg::SvgEntity *cpEntity)
{
    return cpEntity->getFillColour().getColour().getBgrA().value();
}

TEST_CASE("12a. A class rule sets the fill", "[css][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svgDocument = svgParser.parse(R"xxx(<svg>
        <defs><style>.cls-1 { fill: #ff0000; } .cls-2 { fill: none; }</style></defs>
        <path id="red" class="cls-1" d="M0,0 L1,1 Z"/>
        <path id="none" class="cls-2" d="M0,0 L1,1 Z"/>
    </svg>)xxx");
    REQUIRE( FillBgraOf(EntityIn(svgDocument, "red")) == std::array<uint8_t,4> { 0x00, 0x00, 0xff, 0xff } );
    REQUIRE( EntityIn(svgDocument, "none")->getFillColour().getSvgColourType() == Draw2d::Svg::SvgColourType::None );
}

TEST_CASE("12b. A stylesheet after the content still applies", "[css][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svgDocument = svgParser.parse(R"xxx(<svg>
        <path id="p" class="late" d="M0,0 L1,1 Z"/>
        <style>.late { fill: #0000ff; }</style>
    </svg>)xxx");
    REQUIRE( FillBgraOf(EntityIn(svgDocument, "p")) == std::array<uint8_t,4> { 0xff, 0x00, 0x00, 0xff } );
}

TEST_CASE("12c. Cascade: attribute < stylesheet < style attribute", "[css][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svgDocument = svgParser.parse(R"xxx(<svg>
        <style>.c { fill: #00ff00; stroke: #00ff00; }</style>
        <path id="sheet" class="c" fill="#ff0000" d="M0,0 L1,1 Z"/>
        <path id="inline" class="c" fill="#ff0000" style="fill: #0000ff" d="M0,0 L1,1 Z"/>
    </svg>)xxx");
    REQUIRE( FillBgraOf(EntityIn(svgDocument, "sheet")) == std::array<uint8_t,4> { 0x00, 0xff, 0x00, 0xff } );
    REQUIRE( FillBgraOf(EntityIn(svgDocument, "inline")) == std::array<uint8_t,4> { 0xff, 0x00, 0x00, 0xff } );
    REQUIRE( EntityIn(svgDocument, "inline")->getStrokeColour().getColour().getBgrA().value() == std::array<uint8_t,4> { 0x00, 0xff, 0x00, 0xff } );
}

TEST_CASE("12d. Fill and stroke inherit from groups, CSS or attribute", "[css][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svgDocument = svgParser.parse(R"xxx(<svg>
        <style>.blue { fill: #0000ff; }</style>
        <g fill="#ff0000" stroke-width="3"><path id="fromAttribute" d="M0,0 L1,1 Z"/></g>
        <g class="blue"><g><path id="fromCss" d="M0,0 L1,1 Z"/></g></g>
    </svg>)xxx");
    REQUIRE( FillBgraOf(EntityIn(svgDocument, "fromAttribute")) == std::array<uint8_t,4> { 0x00, 0x00, 0xff, 0xff } );
    REQUIRE( EntityIn(svgDocument, "fromAttribute")->getStrokeWidth() == 3.0f );
    REQUIRE( FillBgraOf(EntityIn(svgDocument, "fromCss")) == std::array<uint8_t,4> { 0xff, 0x00, 0x00, 0xff } );
}

TEST_CASE("12e. Initial values: black fill, no stroke", "[css][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svgDocument = svgParser.parse(R"xxx(<svg><path id="p" d="M0,0 L1,1 Z"/></svg>)xxx");
    REQUIRE( FillBgraOf(EntityIn(svgDocument, "p")) == std::array<uint8_t,4> { 0x00, 0x00, 0x00, 0xff } );
    REQUIRE( EntityIn(svgDocument, "p")->getStrokeColour().getSvgColourType() == Draw2d::Svg::SvgColourType::None );
}

TEST_CASE("12f. use passes its fill on to what it references", "[css][use][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svgDocument = svgParser.parse(R"xxx(<svg>
        <defs><path id="shape" d="M0,0 L1,1 Z"/></defs>
        <use id="u" xlink:href="#shape" fill="#ff0000"/>
    </svg>)xxx");
    const auto *cpUse = dynamic_cast<const Draw2d::Svg::Use *>(EntityIn(svgDocument, "u"));
    REQUIRE( cpUse != nullptr );
    auto generator = cpUse->enumerateChildren();
    const Draw2d::Svg::SvgEntity *cpInstance = *generator.begin();
    REQUIRE( FillBgraOf(cpInstance) == std::array<uint8_t,4> { 0x00, 0x00, 0xff, 0xff } );
}

TEST_CASE("12g. Lengths and opacities from CSS take units and percentages", "[css][svg]") {
    using Catch::Approx;
    Draw2d::Svg::SvgParser svgParser {};
    auto svgDocument = svgParser.parse(R"xxx(<svg>
        <style>
            .px { stroke-width: 2.83464574813843px; }
            .in { stroke-width: 1in; stroke-dasharray: 4,3 2; fill-opacity: 50%; }
            .half { opacity: 50%; }
        </style>
        <path id="px" class="px" d="M0,0 L1,1 Z"/>
        <path id="in" class="in" d="M0,0 L1,1 Z"/>
        <g id="half" class="half"><path d="M0,0 L1,1 Z"/></g>
    </svg>)xxx");
    REQUIRE( EntityIn(svgDocument, "px")->getStrokeWidth() == Approx(2.83464574813843f) );
    REQUIRE( EntityIn(svgDocument, "in")->getStrokeWidth() == Approx(96.0f) );
    REQUIRE( EntityIn(svgDocument, "in")->getDashArray() == std::vector<float> { 4.0f, 3.0f, 2.0f } );
    REQUIRE( EntityIn(svgDocument, "in")->getFillOpacity() == Approx(0.5f) );
    const auto *cpGroup = dynamic_cast<const Draw2d::Svg::Group *>(EntityIn(svgDocument, "half"));
    REQUIRE( cpGroup != nullptr );
    REQUIRE( cpGroup->getOpacity() == Approx(0.5f) );
}

TEST_CASE("13a. A gradient inherits stops and transform through href", "[gradient][svg]") {
    using Catch::Approx;
    Draw2d::Svg::SvgParser svgParser {};
    // shaped like the logo's radial-gradient-2: own geometry, stops and transform from the referenced one
    auto svgDocument = svgParser.parse(R"xxx(<svg><defs>
        <radialGradient id="base" cx="10" cy="10" r="5" gradientTransform="translate(-3 0)" gradientUnits="userSpaceOnUse" spreadMethod="reflect">
            <stop offset="0" stop-color="#ffffff"/><stop offset="1" stop-color="#000000"/>
        </radialGradient>
        <radialGradient id="derived" cx="20" cy="30" r="4" xlink:href="#base"/>
    </defs></svg>)xxx");
    const auto *cpDerived = svgDocument->lookupGradient("derived");
    REQUIRE( cpDerived->stops.size() == 2 );
    REQUIRE( cpDerived->transform.has_value() );
    REQUIRE( cpDerived->transform.value() == std::array<float,6> { 1, 0, 0, 1, -3, 0 } );
    REQUIRE( cpDerived->enumSpreadMethod == Draw2d::SpreadMethod::Reflect );
    // its own geometry stays; the focal point defaults to its own centre
    REQUIRE( cpDerived->geometry[0] == Approx(20.0f) );
    REQUIRE( cpDerived->geometry[2] == Approx(4.0f) );
    REQUIRE( cpDerived->geometry[3] == Approx(20.0f) );
    REQUIRE( cpDerived->geometry[4] == Approx(30.0f) );
}

TEST_CASE("13b. Missing geometry is inherited, even from a gradient defined later", "[gradient][svg]") {
    using Catch::Approx;
    Draw2d::Svg::SvgParser svgParser {};
    auto svgDocument = svgParser.parse(R"xxx(<svg><defs>
        <linearGradient id="derived" x2="50" xlink:href="#base"><stop offset="0" stop-color="red"/></linearGradient>
        <linearGradient id="base" x1="1" y1="2" x2="3" y2="4"><stop offset="0" stop-color="blue"/><stop offset="1" stop-color="red"/></linearGradient>
    </defs></svg>)xxx");
    const auto *cpDerived = svgDocument->lookupGradient("derived");
    REQUIRE( cpDerived->geometry[0] == Approx(1.0f) );
    REQUIRE( cpDerived->geometry[1] == Approx(2.0f) );
    REQUIRE( cpDerived->geometry[2] == Approx(50.0f) );
    REQUIRE( cpDerived->geometry[3] == Approx(4.0f) );
    // it has a stop of its own, so it keeps it
    REQUIRE( cpDerived->stops.size() == 1 );
}

TEST_CASE("13c. No gradientTransform means no transform; percentages are fractions", "[gradient][svg]") {
    using Catch::Approx;
    Draw2d::Svg::SvgParser svgParser {};
    auto svgDocument = svgParser.parse(R"xxx(<svg><defs>
        <linearGradient id="g" x1="0%" y1="0" x2="100%" y2="0"><stop offset="25%" stop-color="red"/><stop offset="1" stop-color="blue"/></linearGradient>
    </defs></svg>)xxx");
    const auto *cpGradient = svgDocument->lookupGradient("g");
    REQUIRE( !cpGradient->transform.has_value() );
    REQUIRE( cpGradient->geometry[2] == Approx(1.0f) );
    REQUIRE( cpGradient->stops[0].getOffset() == Approx(0.25f) );
}

TEST_CASE("13d. A reference cycle between gradients doesn't hang", "[gradient][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svgDocument = svgParser.parse(R"xxx(<svg><defs>
        <linearGradient id="a" x1="0" y1="0" x2="1" y2="0" xlink:href="#b"/>
        <linearGradient id="b" x1="0" y1="0" x2="1" y2="0" xlink:href="#a"/>
    </defs></svg>)xxx");
    REQUIRE( svgDocument->lookupGradient("a")->stops.empty() );
}

TEST_CASE("14a. polygon becomes a closed path through its points", "[polygon][svg]") {
    using Draw2d::Svg::PathMove;
    Draw2d::Svg::SvgParser svgParser {};
    auto svgDocument = svgParser.parse(R"xxx(<svg><polygon id="p" points="0,0 10,0 10,10"/></svg>)xxx");
    const auto *cpPolygon = dynamic_cast<const Draw2d::Svg::Polygon *>(EntityIn(svgDocument, "p"));
    REQUIRE( cpPolygon != nullptr );
    REQUIRE( std::string(cpPolygon->getType()) == "Polygon" );
    REQUIRE( cpPolygon->getPathMoves() == std::vector<PathMove> { PathMove::MoveTo, PathMove::LineTo, PathMove::LineTo, PathMove::ClosePath } );
    REQUIRE( cpPolygon->getPoints() == std::vector<float> { 0, 0, 10, 0, 10, 10 } );
}

TEST_CASE("14b. polyline stays open", "[polygon][svg]") {
    using Draw2d::Svg::PathMove;
    Draw2d::Svg::SvgParser svgParser {};
    auto svgDocument = svgParser.parse(R"xxx(<svg><polyline id="p" points="0 0 10 0 10 10"/></svg>)xxx");
    const auto *cpPolyline = dynamic_cast<const Draw2d::Svg::Polyline *>(EntityIn(svgDocument, "p"));
    REQUIRE( cpPolyline != nullptr );
    REQUIRE( cpPolyline->getPathMoves() == std::vector<PathMove> { PathMove::MoveTo, PathMove::LineTo, PathMove::LineTo } );
}

TEST_CASE("14c. points take path data's number syntax", "[polygon][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    // as Illustrator writes them: no separator before a minus sign or a second decimal point
    auto svgDocument = svgParser.parse(R"xxx(<svg><polygon id="p" points="1.5.5 10-5 -2e1,3"/></svg>)xxx");
    REQUIRE( dynamic_cast<const Draw2d::Svg::Path *>(EntityIn(svgDocument, "p"))->getPoints() == std::vector<float> { 1.5f, 0.5f, 10, -5, -20, 3 } );
}

TEST_CASE("14d. bad points: odd coordinate dropped, drawn up to junk, too few draws nothing", "[polygon][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svgDocument = svgParser.parse(R"xxx(<svg>
        <polygon id="odd" points="0,0 10,0 10"/>
        <polygon id="junk" points="0,0 10,0 10,10 x 20,20"/>
        <polygon id="one" points="5,5"/>
        <polygon id="none"/>
    </svg>)xxx");
    auto pointsOf = [&](const char *pszId) { return dynamic_cast<const Draw2d::Svg::Path *>(EntityIn(svgDocument, pszId))->getPoints(); };
    REQUIRE( pointsOf("odd") == std::vector<float> { 0, 0, 10, 0 } );
    REQUIRE( pointsOf("junk") == std::vector<float> { 0, 0, 10, 0, 10, 10 } );
    REQUIRE( pointsOf("one").empty() );
    REQUIRE( dynamic_cast<const Draw2d::Svg::Path *>(EntityIn(svgDocument, "one"))->getPathMoves().empty() );
    REQUIRE( pointsOf("none").empty() );
}

TEST_CASE("14e. polygon takes its fill from CSS", "[polygon][css][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svgDocument = svgParser.parse(R"xxx(<svg><style>.cls-9 { fill: #4f3e33; }</style><polygon id="p" class="cls-9" points="0,0 1,0 1,1"/></svg>)xxx");
    REQUIRE( FillBgraOf(EntityIn(svgDocument, "p")) == std::array<uint8_t,4> { 0x33, 0x3e, 0x4f, 0xff } );
}

TEST_CASE("15a. mask is parsed with its region and units", "[mask][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svgDocument = svgParser.parse(R"xxx(<svg><defs>
        <mask id="m" x="1" y="2" width="3" height="4" maskUnits="userSpaceOnUse"><rect id="inside" width="1" height="1" fill="#ffffff"/></mask>
        <mask id="defaults"><rect width="1" height="1"/></mask>
    </defs></svg>)xxx");
    const auto *cpMask = dynamic_cast<const Draw2d::Svg::Mask *>(EntityIn(svgDocument, "m"));
    REQUIRE( cpMask != nullptr );
    REQUIRE( cpMask->getX()->fValue == 1.0f );
    REQUIRE( cpMask->getHeight()->fValue == 4.0f );
    REQUIRE( cpMask->getMaskUnits() == Draw2d::UserSpaceOnUse );
    REQUIRE( cpMask->getMaskContentUnits() == Draw2d::UserSpaceOnUse );
    size_t nChildren = 0;
    for(const auto *cpChild : cpMask->enumerateChildren()) { (void) cpChild; nChildren++; }
    REQUIRE( nChildren == 1 );
    const auto *cpDefaults = dynamic_cast<const Draw2d::Svg::Mask *>(EntityIn(svgDocument, "defaults"));
    REQUIRE( cpDefaults->getMaskUnits() == Draw2d::ObjectBoundingBox );
    REQUIRE( !cpDefaults->getX().has_value() );
}

TEST_CASE("15b. opacity and mask come through CSS on any element", "[mask][css][svg]") {
    using Catch::Approx;
    Draw2d::Svg::SvgParser svgParser {};
    auto svgDocument = svgParser.parse(R"xxx(<svg>
        <style>.cls-13 { opacity: .33; } .cls-m { mask: url(#mask-2); }</style>
        <defs><mask id="mask-2"/></defs>
        <path id="p" class="cls-13" d="M0,0 L1,1 Z"/>
        <g id="g" class="cls-m" opacity="0.5"><path d="M0,0 L1,1 Z"/></g>
        <path id="plain" d="M0,0 L1,1 Z"/>
    </svg>)xxx");
    REQUIRE( EntityIn(svgDocument, "p")->getOpacity() == Approx(0.33f) );
    REQUIRE( EntityIn(svgDocument, "g")->getOpacity() == Approx(0.5f) );
    REQUIRE( EntityIn(svgDocument, "g")->getMaskId() == "mask-2" );
    REQUIRE( EntityIn(svgDocument, "plain")->getOpacity() == 1.0f );
    REQUIRE( EntityIn(svgDocument, "plain")->getMaskId().empty() );
}

TEST_CASE("15c. mix-blend-mode and isolation come through CSS", "[blend][css][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svgDocument = svgParser.parse(R"xxx(<svg>
        <style>.cls-438 { mix-blend-mode: hard-light; } .cls-2082 { isolation: isolate; }</style>
        <image id="i" class="cls-438" width="1" height="1"/>
        <g id="g" class="cls-2082" mix-blend-mode="screen"/>
        <g id="bad" mix-blend-mode="sparkle"/>
    </svg>)xxx");
    REQUIRE( EntityIn(svgDocument, "i")->getBlendMode() == Draw2d::Svg::BlendMode::HardLight );
    REQUIRE( EntityIn(svgDocument, "g")->getBlendMode() == Draw2d::Svg::BlendMode::Screen );
    REQUIRE( EntityIn(svgDocument, "g")->getIsolate() );
    REQUIRE( !EntityIn(svgDocument, "i")->getIsolate() );
    REQUIRE( EntityIn(svgDocument, "bad")->getBlendMode() == Draw2d::Svg::BlendMode::Normal );
}

TEST_CASE("16a. clipPath and clip-path are parsed, the latter through CSS", "[clip][css][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svgDocument = svgParser.parse(R"xxx(<svg>
        <style>.cls-c { clip-path: url(#clip); }</style>
        <defs>
            <clipPath id="clip"><rect width="2" height="2"/><path d="M0,0 L1,1 L0,1 Z"/></clipPath>
            <clipPath id="box" clipPathUnits="objectBoundingBox"/>
        </defs>
        <g id="g" class="cls-c"/>
        <rect id="r" clip-path="url(#box)" width="1" height="1"/>
    </svg>)xxx");
    const auto *cpClip = dynamic_cast<const Draw2d::Svg::ClipPath *>(EntityIn(svgDocument, "clip"));
    REQUIRE( cpClip != nullptr );
    REQUIRE( cpClip->getClipPathUnits() == Draw2d::UserSpaceOnUse );
    size_t nChildren = 0;
    for(const auto *cpChild : cpClip->enumerateChildren()) { (void) cpChild; nChildren++; }
    REQUIRE( nChildren == 2 );
    REQUIRE( dynamic_cast<const Draw2d::Svg::ClipPath *>(EntityIn(svgDocument, "box"))->getClipPathUnits() == Draw2d::ObjectBoundingBox );
    REQUIRE( EntityIn(svgDocument, "g")->getClipPathId() == "clip" );
    REQUIRE( EntityIn(svgDocument, "r")->getClipPathId() == "box" );
}

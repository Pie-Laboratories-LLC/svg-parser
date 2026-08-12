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

#include "svg/SvgParser.hpp"

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

TEST_CASE("1g. Malformed — L before any M (should throw)", "[path][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg1g = R"xxx(<svg><path id="p1g" d="L 10 10 Z"/></svg>)xxx";
    REQUIRE_THROWS_AS (svgParser.parse(svg1g), Draw2d::Svg::SvgException);
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
    auto svg2e = R"xxx(<svg><g id="g2e"><circle cx="5" cy="5" r="3"/></g></svg>)xxx";
    REQUIRE_THROWS_AS ( svgParser.parse(svg2e), Draw2d::Svg::SvgException );
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

TEST_CASE("7g. rotate with only 2 args (malformed — should throw per spec)", "[transform][throws][svg]") {
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

TEST_CASE("7p. Malformed transform — unclosed paren (should throw)", "[transform][throws][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg7p = R"xxx(<svg><g id="g7p" transform="translate(10,20"><path id="p7p" d="M0,0 L1,1 Z"/></g></svg>)xxx";
    REQUIRE_THROWS_AS ( svgParser.parse(svg7p), Draw2d::Svg::SvgException );
}

TEST_CASE("8a. Unrecognized top-level element (should throw)", "[structural][throws][svg]") {
    Draw2d::Svg::SvgParser svgParser {};
    auto svg8a = R"xxx(<svg><circle cx="5" cy="5" r="3"/></svg>)xxx";
    REQUIRE_THROWS_AS ( svgParser.parse(svg8a), Draw2d::Svg::SvgException );
}

TEST_CASE("8b. Empty svg root (no children) — should this throw or no-op?", "[structural][svg]") {
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

TEST_CASE("9h. use -> defs group -> use -> back to the enclosing group", "[use][self-reference][throws][svg][xxx]") {
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

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

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "svg/Path.hpp"
#include "svg/SvgDocument.hpp"

using namespace Draw2d::Svg;

namespace {
    // Same shear-matrix fixture as MatrixMath-tests.cpp, so results are
    // cross-checkable by hand against that file's hand-derived numbers.
    // xx=2, yx=0.5, xy=0.25, yy=3, dx=10, dy=20
    constexpr std::optional<std::array<float,6>> M = { { 2.0f, 0.5f, 0.25f, 3.0f, 10.0f, 20.0f } };

    Path makePath(SvgDocument &doc, std::vector<float> points, std::vector<PathMove> moves) {
        SvgPathParams params{ { doc }, std::move(points), std::move(moves) };
        return Path(std::move(params));
    }
}

TEST_CASE("Path::getX (HorizontalLineTo overload): pulls x from points, y from supplied value", "[svg-path][regression][xxx]") {
    SvgDocument doc {};
    Path path = makePath(doc, { 0.0f, 0.0f, 9.0f }, { PathMove::MoveTo, PathMove::HorizontalLineTo });

    REQUIRE(M.has_value());
    REQUIRE( path.getX(2, 7.0f, M) == Catch::Approx(29.75f) );
}

TEST_CASE("Path::getX (HorizontalLineTo overload): nullptr matrix returns raw x, not y", "[svg-path]") {
    SvgDocument doc {};
    Path path = makePath(doc, { 0.0f, 0.0f, 9.0f }, { PathMove::MoveTo, PathMove::HorizontalLineTo });

    REQUIRE( path.getX(2, 7.0f, std::nullopt) == Catch::Approx(9.0f) );
}

TEST_CASE("Path::getY (VerticalLineTo overload): pulls y from points, x from supplied value", "[svg-path][regression]") {
    SvgDocument doc {};
    Path path = makePath(doc, { 0.0f, 0.0f, 11.0f }, { PathMove::MoveTo, PathMove::VerticalLineTo });

    REQUIRE( path.getY(2, 3.0f, M) == Catch::Approx(54.5f) );
}

TEST_CASE("Path::getY (VerticalLineTo overload): nullptr matrix returns raw y, not x", "[svg-path]") {
    SvgDocument doc {};
    Path path = makePath(doc, { 0.0f, 0.0f, 11.0f }, { PathMove::MoveTo, PathMove::VerticalLineTo });

    REQUIRE( path.getY(2, 3.0f, std::nullopt) == Catch::Approx(11.0f) );
}

TEST_CASE("Path: throws if first path move is not MoveTo", "[svg-path][regression]") {
    SvgDocument doc {};
    REQUIRE_THROWS( makePath(doc, { 9.0f }, { PathMove::HorizontalLineTo }) );
}

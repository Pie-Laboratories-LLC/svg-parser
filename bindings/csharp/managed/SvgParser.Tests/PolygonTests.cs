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

using PieLaboratories.Svg;

namespace SvgParserTests;

[Collection(SvgParserTestCollection.Name)]
public class PolygonTests : IDisposable
{
    private readonly SvgDocument _document;

    public PolygonTests()
    {
        _document = TestSvg.Parse("""
            <svg><g id="wrapper">
              <polygon id="pg" points="0,0 10,0 10,10"/>
              <polyline id="pl" points="0,0 5,5 10,0"/>
            </g></svg>
            """);
    }

    public void Dispose() => _document.Dispose();

    [Fact]
    public void Polygon_IsAPathThroughItsPoints()
    {
        var polygon = Assert.IsType<Polygon>(_document.lookupSvgEntity("pg"));
        Assert.Equal("Polygon", polygon.Type);
        Assert.Equal([0f, 0f, 10f, 0f, 10f, 10f], polygon.Points);
        Assert.Equal(PathMove.ClosePath, polygon.PathMoves[^1]);
    }

    [Fact]
    public void Polyline_IsAPathLeftOpen()
    {
        var polyline = Assert.IsType<Polyline>(_document.lookupSvgEntity("pl"));
        Assert.Equal("Polyline", polyline.Type);
        Assert.Equal([0f, 0f, 5f, 5f, 10f, 0f], polyline.Points);
        Assert.DoesNotContain(PathMove.ClosePath, polyline.PathMoves);
    }

    [Fact]
    public void EnumerateChildren_ReturnsPolygonAndPolyline()
    {
        var group = (Group)_document.lookupSvgEntity("wrapper")!;
        var children = group.enumerateChildren();
        Assert.Collection(children,
            child => Assert.IsType<Polygon>(child),
            child => Assert.IsType<Polyline>(child));
    }
}

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
public class EllipseTests : IDisposable
{
    private readonly SvgDocument _document;
    private readonly Ellipse _full;
    private readonly Ellipse _bare;

    public EllipseTests()
    {
        // rect/circle/ellipse aren't recognized as direct children of the root <svg> by the
        // parser today, only inside a container like <g> -- see __parseGlobalScope.
        _document = TestSvg.Parse("""
            <svg><g id="wrapper">
              <ellipse id="e-full" cx="1" cy="2" rx="3" ry="4"/>
              <ellipse id="e-bare"/>
            </g></svg>
            """);

        _full = (Ellipse)_document.lookupSvgEntity("e-full")!;
        _bare = (Ellipse)_document.lookupSvgEntity("e-bare")!;
    }

    public void Dispose() => _document.Dispose();

    [Fact]
    public void Cx_Cy_Rx_Ry_ReturnSetValues()
    {
        Assert.Equal(1f, _full.Cx!.Value.Value);
        Assert.Equal(DimensionUnits.Px, _full.Cx!.Value.Units);
        Assert.Equal(2f, _full.Cy!.Value.Value);
        Assert.Equal(3f, _full.Rx!.Value.Value);
        Assert.Equal(4f, _full.Ry!.Value.Value);
    }

    [Fact]
    public void Cx_Cy_Rx_Ry_AreNull_WhenNotSet()
    {
        Assert.Null(_bare.Cx);
        Assert.Null(_bare.Cy);
        Assert.Null(_bare.Rx);
        Assert.Null(_bare.Ry);
    }
}

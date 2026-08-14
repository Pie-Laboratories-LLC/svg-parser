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
public class CircleTests : IDisposable
{
    private readonly SvgDocument _document;
    private readonly Circle _full;
    private readonly Circle _bare;

    public CircleTests()
    {
        // rect/circle/ellipse aren't recognized as direct children of the root <svg> by the
        // parser today, only inside a container like <g> -- see __parseGlobalScope.
        _document = TestSvg.Parse("""
            <svg><g id="wrapper">
              <circle id="c-full" cx="1" cy="2" r="3"/>
              <circle id="c-bare"/>
            </g></svg>
            """);

        _full = (Circle)_document.lookupSvgEntity("c-full")!;
        _bare = (Circle)_document.lookupSvgEntity("c-bare")!;
    }

    public void Dispose() => _document.Dispose();

    [Fact]
    public void Cx_Cy_R_ReturnSetValues()
    {
        Assert.Equal(1f, _full.Cx!.Value.Value);
        Assert.Equal(DimensionUnits.Px, _full.Cx!.Value.Units);
        Assert.Equal(2f, _full.Cy!.Value.Value);
        Assert.Equal(3f, _full.R!.Value.Value);
    }

    [Fact]
    public void Cx_Cy_R_AreNull_WhenNotSet()
    {
        Assert.Null(_bare.Cx);
        Assert.Null(_bare.Cy);
        Assert.Null(_bare.R);
    }
}

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
public class RectTests : IDisposable
{
    private readonly SvgDocument _document;
    private readonly Rect _full;
    private readonly Rect _bare;

    public RectTests()
    {
        // rect/circle/ellipse aren't recognized as direct children of the root <svg> by the
        // parser today, only inside a container like <g> -- see __parseGlobalScope.
        _document = TestSvg.Parse("""
            <svg><g id="wrapper">
              <rect id="r-full" x="1" y="2" width="3" height="4" rx="5" ry="6"/>
              <rect id="r-bare" width="1" height="1"/>
            </g></svg>
            """);

        _full = (Rect)_document.lookupSvgEntity("r-full")!;
        _bare = (Rect)_document.lookupSvgEntity("r-bare")!;
    }

    public void Dispose() => _document.Dispose();

    [Fact]
    public void Dimensions_ReturnSetValues_InPixels()
    {
        Assert.Equal(1f, _full.X!.Value.Value);
        Assert.Equal(DimensionUnits.Px, _full.X!.Value.Units);
        Assert.Equal(2f, _full.Y!.Value.Value);
        Assert.Equal(3f, _full.Width!.Value.Value);
        Assert.Equal(4f, _full.Height!.Value.Value);
    }

    [Fact]
    public void Rx_Ry_ReturnSetValues()
    {
        Assert.Equal(5f, _full.Rx!.Value.Value);
        Assert.Equal(6f, _full.Ry!.Value.Value);
    }

    [Fact]
    public void X_Y_Rx_Ry_AreNull_WhenNotSet()
    {
        Assert.Null(_bare.X);
        Assert.Null(_bare.Y);
        Assert.Null(_bare.Rx);
        Assert.Null(_bare.Ry);
    }

    [Fact]
    public void Width_Height_ReturnSetValues_OnBareRect()
    {
        Assert.Equal(1f, _bare.Width!.Value.Value);
        Assert.Equal(1f, _bare.Height!.Value.Value);
    }
}

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
public class MaskTests : IDisposable
{
    private readonly SvgDocument _document;
    private readonly Mask _full;
    private readonly Mask _bare;

    public MaskTests()
    {
        _document = TestSvg.Parse("""
            <svg><defs>
              <mask id="m-full" x="0" y="0" width="1" height="1"
                    maskUnits="userSpaceOnUse" maskContentUnits="objectBoundingBox">
                <rect width="1" height="1" fill="white"/>
              </mask>
              <mask id="m-bare"><rect width="1" height="1"/></mask>
            </defs></svg>
            """);
        _full = (Mask)_document.lookupSvgEntity("m-full")!;
        _bare = (Mask)_document.lookupSvgEntity("m-bare")!;
    }

    public void Dispose() => _document.Dispose();

    [Fact]
    public void Region_ReturnsSetValues()
    {
        Assert.Equal(0f, _full.X!.Value.Value);
        Assert.Equal(DimensionUnits.Px, _full.X!.Value.Units);
        Assert.Equal(0f, _full.Y!.Value.Value);
        Assert.Equal(1f, _full.Width!.Value.Value);
        Assert.Equal(1f, _full.Height!.Value.Value);
    }

    [Fact]
    public void Region_IsNull_WhenNotSet()
    {
        Assert.Null(_bare.X);
        Assert.Null(_bare.Y);
        Assert.Null(_bare.Width);
        Assert.Null(_bare.Height);
    }

    [Fact]
    public void Units_ReturnSetValues()
    {
        Assert.Equal(GradientUnits.UserSpaceOnUse, _full.MaskUnits);
        Assert.Equal(GradientUnits.ObjectBoundingBox, _full.MaskContentUnits);
    }

    [Fact]
    public void Units_DefaultPerSpec()
    {
        Assert.Equal(GradientUnits.ObjectBoundingBox, _bare.MaskUnits);
        Assert.Equal(GradientUnits.UserSpaceOnUse, _bare.MaskContentUnits);
    }

    [Fact]
    public void EnumerateChildren_ReturnsContent()
    {
        Assert.IsType<Rect>(Assert.Single(_full.enumerateChildren()));
    }
}

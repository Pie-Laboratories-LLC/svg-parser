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
public class SvgEntityTests : IDisposable
{
    private readonly SvgDocument _document;
    private readonly SvgEntity _full;
    private readonly SvgEntity _bare;
    private readonly SvgEntity _inDefs;

    public SvgEntityTests()
    {
        _document = TestSvg.Parse("""
            <svg>
              <defs>
                <path id="p-in-defs" d="M0,0 L1,1 Z"/>
              </defs>
              <path id="p-full" d="M0,0 L1,1 Z"
                    transform="matrix(1 0 0 1 13 24)"
                    fill="#112233" fill-opacity="0.25" fill-rule="evenodd"
                    stroke="#445566" stroke-opacity="0.75" stroke-width="3.5"
                    stroke-linecap="round" stroke-linejoin="Bevel" stroke-miterlimit="2.5"
                    stroke-dasharray="1 3 4 2"
                    class="my-class" style="opacity:1"/>
              <path id="p-bare" d="M0,0 L1,1 Z"/>
            </svg>
            """);

        Svg root = _document.getRoot();
        _full = root.enumerateChildren().Single(c => c.Id == "p-full");
        _bare = root.enumerateChildren().Single(c => c.Id == "p-bare");
        _inDefs = _document.lookupSvgEntity("p-in-defs")!;
    }

    public void Dispose() => _document.Dispose();

    [Fact]
    public void GetType_ReturnsEntityTypeName() =>
        Assert.Equal("Path", _full.Type);

    [Fact]
    public void Id_ReturnsSetId() =>
        Assert.Equal("p-full", _full.Id);

    [Fact]
    public void Render_DefaultsToTrue() =>
        Assert.True(_full.Render);

    [Fact]
    public void Render_IsFalse_ForEntityInsideDefs() =>
        Assert.False(_inDefs.Render);

    [Fact]
    public void Matrix_ReturnsSetTransform() =>
        Assert.Equal(new float[] { 1, 0, 0, 1, 13, 24 }, _full.Matrix);

    [Fact]
    public void Matrix_IsNull_WhenNotSet() =>
        Assert.Null(_bare.Matrix);

    [Fact]
    public void FillColour_ReturnsSetColour()
    {
        SvgColour colour = _full.FillColour.SvgColour!;
        Assert.Equal(SvgColourType.Bgr, colour.SvgColourType);
        // Hex pairs are copied into the Bgra array in the order written, not reversed --
        // fill="#112233" -> Bgra = [0x11, 0x22, 0x33, 0xFF].
        Assert.Equal(new byte[] { 0x11, 0x22, 0x33, 0xFF }, colour.Bgra);
    }

    [Fact]
    public void FillColour_DefaultsToNone() =>
        Assert.Equal(SvgColourType.None, _bare.FillColour.SvgColour!.SvgColourType);

    [Fact]
    public void FillRule_ReturnsSetRule() =>
        Assert.Equal(FillRule.EvenOdd, _full.FillRule);

    [Fact]
    public void FillRule_DefaultsToNonZero() =>
        Assert.Equal(FillRule.NonZero, _bare.FillRule);

    [Fact]
    public void FillOpacity_ReturnsSetValue() =>
        Assert.Equal(0.25f, _full.FillOpacity);

    [Fact]
    public void FillOpacity_DefaultsToOne() =>
        Assert.Equal(1f, _bare.FillOpacity);

    [Fact]
    public void StrokeColour_ReturnsSetColour()
    {
        SvgColour colour = _full.StrokeColour.SvgColour!;
        Assert.Equal(SvgColourType.Bgr, colour.SvgColourType);
        Assert.Equal(new byte[] { 0x44, 0x55, 0x66, 0xFF }, colour.Bgra);
    }

    [Fact]
    public void StrokeOpacity_ReturnsSetValue() =>
        Assert.Equal(0.75f, _full.StrokeOpacity);

    [Fact]
    public void StrokeOpacity_DefaultsToOne() =>
        Assert.Equal(1f, _bare.StrokeOpacity);

    [Fact]
    public void StrokeWidth_ReturnsSetValue() =>
        Assert.Equal(3.5f, _full.StrokeWidth);

    [Fact]
    public void StrokeWidth_DefaultsToOne() =>
        Assert.Equal(1f, _bare.StrokeWidth);

    [Fact]
    public void LineCap_ReturnsSetValue() =>
        Assert.Equal(LineCap.Round, _full.LineCap);

    [Fact]
    public void LineCap_DefaultsToButt() =>
        Assert.Equal(LineCap.Butt, _bare.LineCap);

    [Fact]
    public void LineJoin_ReturnsSetValue() =>
        Assert.Equal(LineJoin.Bevel, _full.LineJoin);

    [Fact]
    public void LineJoin_DefaultsToMiter() =>
        Assert.Equal(LineJoin.Miter, _bare.LineJoin);

    [Fact]
    public void MiterLimit_ReturnsSetValue() =>
        Assert.Equal(2.5f, _full.MiterLimit);

    [Fact]
    public void MiterLimit_DefaultsToFour() =>
        Assert.Equal(4f, _bare.MiterLimit);

    [Fact]
    public void DashArray_ReturnsSetValues() =>
        Assert.Equal(new float[] { 1, 3, 4, 2 }, _full.DashArray);

    [Fact]
    public void DashArray_DefaultsToEmpty() =>
        Assert.Empty(_bare.DashArray);

    [Fact]
    public void CssClass_ReturnsSetValue() =>
        Assert.Equal("my-class", _full.CssClass);

    [Fact]
    public void CssClass_DefaultsToEmpty() =>
        Assert.Equal(string.Empty, _bare.CssClass);

    [Fact]
    public void CssStyle_ReturnsSetValue() =>
        Assert.Equal("opacity:1", _full.CssStyle);

    [Fact]
    public void CssStyle_DefaultsToEmpty() =>
        Assert.Equal(string.Empty, _bare.CssStyle);
}

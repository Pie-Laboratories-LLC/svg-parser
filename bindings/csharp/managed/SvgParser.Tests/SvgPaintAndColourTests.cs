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
public class SvgPaintAndColourTests : IDisposable
{
    private readonly SvgDocument _document;

    public SvgPaintAndColourTests()
    {
        _document = TestSvg.Parse("""
            <svg>
              <defs>
                <linearGradient id="lg1" x1="0" y1="0" x2="1" y2="0">
                  <stop offset="0" stop-color="#000000"/>
                  <stop offset="1" stop-color="#ffffff"/>
                </linearGradient>
              </defs>
              <path id="p-hex8" d="M0,0 L1,1 Z" fill="#deadbeef"/>
              <path id="p-current-with-fallback" d="M0,0 L1,1 Z" fill="var(currentColor,#deadbeef)"/>
              <path id="p-gradient" d="M0,0 L1,1 Z" fill="url(#lg1)"/>
              <path id="p-cpal" d="M0,0 L1,1 Z" fill="--color3"/>
            </svg>
            """);
    }

    public void Dispose() => _document.Dispose();

    private SvgEntity Find(string id) => _document.lookupSvgEntity(id)!;

    [Fact]
    public void HexColourWithAlpha_ParsesAsBgrAInWrittenByteOrder()
    {
        SvgColour colour = Find("p-hex8").FillColour.SvgColour!;

        Assert.Equal(SvgColourType.BgrA, colour.SvgColourType);
        Assert.Equal(new byte[] { 0xDE, 0xAD, 0xBE, 0xEF }, colour.Bgra);
    }

    [Fact]
    public void CurrentColorWithFallback_PaintIsCurrentColor()
    {
        SvgPaint paint = Find("p-current-with-fallback").FillColour;

        Assert.Equal(SvgColourType.Bgr, paint.SvgColourType);
        Assert.Equal(SvgColourType.Bgr, paint.SvgColour!.SvgColourType);
    }

    [Fact]
    public void CurrentColorWithFallback_FallbackLivesOnTheColour_NotThePaint()
    {
        // The parser only ever calls SvgColour::setFallback (never SvgPaint::setFallback),
        // so the "var(x, fallback)" fallback surfaces via paint.SvgColour.Fallback, not
        // paint.Fallback -- the latter is always empty through the public parsing API today.
        SvgPaint paint = Find("p-current-with-fallback").FillColour;

        SvgColour? colourFallback = paint.SvgColour!.Fallback;
        Assert.NotNull(colourFallback);
        Assert.Equal(SvgColourType.BgrA, colourFallback!.SvgColourType);
        Assert.Equal(new byte[] { 0xDE, 0xAD, 0xBE, 0xEF }, colourFallback.Bgra);
    }

    [Fact]
    public void Paint_HasFallback_IsFalse_ThroughThePublicParsingApi()
    {
        // See note above: SvgPaint::setFallback is never invoked by the parser.
        Assert.Null(Find("p-current-with-fallback").FillColour.Fallback);
    }

    [Fact]
    public void GradientReference_PaintIsGradient_WithGradientIdSet()
    {
        SvgPaint paint = Find("p-gradient").FillColour;

        Assert.Equal(SvgColourType.Gradient, paint.SvgColourType);
        Assert.Equal("lg1", paint.GradientId);
    }

    [Fact]
    public void GradientReference_SvgColourIsNull()
    {
        // Regression check: svgparser_paint_get_colour() returns nullptr for Gradient
        // paints, and SvgPaint.SvgColour must reflect that as null rather than wrapping it.
        Assert.Null(Find("p-gradient").FillColour.SvgColour);
    }

    [Fact]
    public void CpalReference_ParsesAsCpalWithIndex()
    {
        SvgColour colour = Find("p-cpal").FillColour.SvgColour!;

        Assert.Equal(SvgColourType.Cpal, colour.SvgColourType);
        Assert.Equal(3, colour.Index);
    }
}

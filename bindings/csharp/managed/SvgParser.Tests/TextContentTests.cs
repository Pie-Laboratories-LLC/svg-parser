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
public class TextContentTests : IDisposable
{
    private readonly SvgDocument _document;
    private readonly Text _styled;
    private readonly Text _bare;

    public TextContentTests()
    {
        _document = TestSvg.Parse("""
            <svg>
              <text id="t-styled" font-size="12px" font-family="Helvetica, sans-serif"
                    text-anchor="middle" font-style="italic" textLength="50%">Hello <tspan x="5">big <tspan>wide</tspan></tspan>  world</text>
              <text id="t-bare">plain</text>
            </svg>
            """);
        _styled = (Text)_document.lookupSvgEntity("t-styled")!;
        _bare = (Text)_document.lookupSvgEntity("t-bare")!;
    }

    public void Dispose() => _document.Dispose();

    [Fact]
    public void Content_ConcatenatesTSpansAndCollapsesWhitespace()
    {
        Assert.Equal("Hello big wide world", _styled.Content);
        Assert.Equal("plain", _bare.Content);
    }

    [Fact]
    public void Parts_KeepDocumentOrderAndRawWhitespace()
    {
        Assert.Collection(_styled.Parts,
            part => Assert.Equal("Hello ", part.Text),
            part => Assert.True(part.IsTSpan),
            part => Assert.Equal("  world", part.Text));
    }

    [Fact]
    public void TSpans_NestAndKeepTheirAttributes()
    {
        var outer = _styled.Parts[1].TSpan!;
        Assert.Equal([5f], outer.X);
        Assert.Equal("big wide", outer.Content);
        var inner = Assert.Single(outer.Parts, part => part.IsTSpan).TSpan!;
        Assert.Empty(inner.X);
        Assert.Equal("wide", inner.Content);
    }

    [Fact]
    public void FontAttributes_ReturnSetValues()
    {
        Assert.Equal(12f, _styled.FontSize);
        Assert.Equal(DimensionUnits.Px, _styled.FontSizeUnits);
        Assert.Equal("Helvetica, sans-serif", _styled.FontFamily);
        Assert.Equal(TextAnchor.Middle, _styled.TextAnchor);
        Assert.Equal(FontStyle.Italic, _styled.FontStyle);
        Assert.True(_styled.TextLengthPercentage);
    }

    [Fact]
    public void FontAttributes_AreNull_WhenNotSet()
    {
        Assert.Null(_bare.FontSize);
        Assert.Null(_bare.FontSizeUnits);
        Assert.Null(_bare.FontFamily);
        Assert.Null(_bare.TextAnchor);
        Assert.Null(_bare.FontStyle);
        Assert.False(_bare.TextLengthPercentage);
    }
}

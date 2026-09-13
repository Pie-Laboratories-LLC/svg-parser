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
public class TextTests : IDisposable
{
    private readonly SvgDocument _document;
    private readonly Text _full;
    private readonly Text _bare;

    public TextTests()
    {
        Console.WriteLine("Here 1");
        _document = TestSvg.Parse("""
            <svg>
              <text id="t-full" x="10 20 30" y="1 2 3" dx="1 2" dy="-1 -2" rotate="5 10 15"
                    textLength="100" lengthAdjust="spacingAndGlyphs">Hello</text>
              <text id="t-bare">Plain</text>
            </svg>
            """);
        Console.WriteLine("Here 2");
        _full = (Text)_document.lookupSvgEntity("t-full")!;
        Console.WriteLine("Here 3");
        _bare = (Text)_document.lookupSvgEntity("t-bare")!;
        Console.WriteLine("Here 4");
    }

    public void Dispose() => _document.Dispose();

    [Fact]
    public void X_ReturnsSetValues()
    {
        Assert.Equal([10f, 20f, 30f], _full.X);
    }

    [Fact]
    public void Y_ReturnsSetValues()
    {
        Assert.Equal([1f, 2f, 3f], _full.Y);
    }

    [Fact]
    public void Dx_ReturnsSetValues()
    {
        Assert.Equal([1f, 2f], _full.Dx);
    }

    [Fact]
    public void Dy_ReturnsSetValues()
    {
        Assert.Equal([-1f, -2f], _full.Dy);
    }

    [Fact]
    public void Rotate_ReturnsSetValues()
    {
        Assert.Equal([5f, 10f, 15f], _full.Rotate);
    }

    [Fact]
    public void LengthAdjust_ReturnsSetValue()
    {
        Assert.Equal(LengthAdjust.SpacingAndGlyphs, _full.LengthAdjust);
    }

    [Fact]
    public void TextLength_ReturnsSetValue()
    {
        Assert.Equal(100f, _full.TextLength);
    }

    [Fact]
    public void TextLength_ReturnsNullWhenAbsent()
    {
        Assert.Null(_bare.TextLength);
    }

    [Fact]
    public void ListAttributes_ReturnEmptyWhenAbsent()
    {
        Assert.Empty(_bare.X);
        Assert.Empty(_bare.Y);
        Assert.Empty(_bare.Dx);
        Assert.Empty(_bare.Dy);
        Assert.Empty(_bare.Rotate);
    }
}

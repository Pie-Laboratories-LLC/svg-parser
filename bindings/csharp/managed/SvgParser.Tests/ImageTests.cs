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
public class ImageTests : IDisposable
{
    private readonly SvgDocument _document;
    private readonly Image _full;
    private readonly Image _bare;

    public ImageTests()
    {
        // "hello" base64-encoded is "aGVsbG8=" -- chosen so the expected decoded
        // bytes are trivially human-verifiable rather than an opaque magic number.
        _document = TestSvg.Parse("""
            <svg>
              <image id="i-full" x="1" y="2" width="50" height="60"
                     href="data:image/png;charset=utf-8;base64,aGVsbG8="
                     preserveAspectRatio="xMidYMid slice"
                     crossorigin="anonymous" decoding="async" fetchpriority="high"/>
              <image id="i-bare" width="10" height="10"/>
            </svg>
            """);
        _full = (Image)_document.lookupSvgEntity("i-full")!;
        _bare = (Image)_document.lookupSvgEntity("i-bare")!;
    }

    public void Dispose() => _document.Dispose();

    [Fact]
    public void Dimensions_ReturnSetValues()
    {
        Assert.Equal(1f, _full.X!.Value.Value);
        Assert.Equal(2f, _full.Y!.Value.Value);
        Assert.Equal(50f, _full.Width!.Value.Value);
        Assert.Equal(60f, _full.Height!.Value.Value);
    }

    [Fact]
    public void Href_ReturnsDecodedBytesForBase64DataUri()
    {
        // The parser resolves the base64 payload eagerly (same principle as
        // resolving currentColor) -- href holds the decoded bytes, not the
        // original data URI or its base64 form.
        Assert.Equal("hello", _full.Href);
        Assert.Equal(HrefKind.DecodedBinary, _full.HrefKind);
    }

    [Fact]
    public void ImageType_ReturnsParsedType()
    {
        Assert.Equal("png", _full.ImageType);
    }

    [Fact]
    public void CharacterEncoding_ReturnsParsedCharset()
    {
        Assert.Equal("utf-8", _full.CharacterEncoding);
    }

    [Fact]
    public void PreserveAspectRatio_ReturnsSetValue()
    {
        Assert.Equal(PreserveAspectRatio.xMidYMid, _full.PreserveAspectRatio);
        Assert.Equal(PreserveAspectRatioMode.Slice, _full.PreserveAspectRatioMode);
    }

    [Fact]
    public void CrossOrigin_ReturnsSetValue()
    {
        Assert.Equal(CrossOrigin.Anonymous, _full.CrossOrigin);
    }

    [Fact]
    public void HrefKind_DefaultsToUrl()
    {
        Assert.Equal(HrefKind.Url,_bare.HrefKind);
    }

    [Fact]
    public void CrossOrigin_ReturnsNullWhenAbsent()
    {
        Assert.Null(_bare.CrossOrigin);
    }

    [Fact]
    public void Decoding_ReturnsSetValue()
    {
        Assert.Equal(Decoding.Async, _full.Decoding);
    }

    [Fact]
    public void Decoding_ReturnsNullWhenAbsent()
    {
        Assert.Null(_bare.Decoding);
    }

    [Fact]
    public void FetchPriority_ReturnsSetValue()
    {
        Assert.Equal(FetchPriority.High, _full.FetchPriority);
    }

    [Fact]
    public void FetchPriority_ReturnsNullWhenAbsent()
    {
        Assert.Null(_bare.FetchPriority);
    }

    [Fact]
    public void Href_ReturnsEmptyWhenAbsent()
    {
        Assert.Equal(string.Empty, _bare.Href);
    }

    [Fact]
    public void Href_ReturnsPlainUrlUnchanged()
    {
        using var doc = TestSvg.Parse("""<svg><image id="i-url" width="10" height="10" href="picture.png"/></svg>""");
        var image = (Image)doc.lookupSvgEntity("i-url")!;

        Assert.Equal("picture.png", image.Href);
    }

    [Fact]
    public void Href_ReturnsLiteralTextForNonBase64DataUri()
    {
        using var doc = TestSvg.Parse("""<svg><image id="i-literal" width="10" height="10" href="data:image/svg+xml;charset=utf-8,hello"/></svg>""");
        var image = (Image)doc.lookupSvgEntity("i-literal")!;

        Assert.Equal("hello", image.Href);
        Assert.Equal("svg+xml", image.ImageType);
        Assert.Equal("utf-8", image.CharacterEncoding);
    }
}

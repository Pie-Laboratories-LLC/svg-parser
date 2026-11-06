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
public class CompositingTests : IDisposable
{
    private readonly SvgDocument _document;

    public CompositingTests()
    {
        _document = TestSvg.Parse("""
            <svg>
              <style>.blended { mix-blend-mode: multiply; opacity: 0.5 }</style>
              <defs>
                <mask id="m1"><rect width="1" height="1" fill="white"/></mask>
                <clipPath id="c1"><rect width="1" height="1"/></clipPath>
              </defs>
              <g id="isolated" isolation="isolate">
                <path id="styled" d="M0,0 L1,1 Z" class="blended" mask="url(#m1)" clip-path="url(#c1)"/>
                <path id="plain" d="M0,0 L1,1 Z"/>
              </g>
            </svg>
            """);
    }

    public void Dispose() => _document.Dispose();

    [Fact]
    public void OpacityAndBlendMode_ComeThroughCss()
    {
        var entity = _document.lookupSvgEntity("styled")!;
        Assert.Equal(0.5f, entity.Opacity);
        Assert.Equal(BlendMode.Multiply, entity.BlendMode);
    }

    [Fact]
    public void MaskAndClipPath_NameTheirTargets()
    {
        var entity = _document.lookupSvgEntity("styled")!;
        Assert.Equal("m1", entity.MaskId);
        Assert.Equal("c1", entity.ClipPathId);
    }

    [Fact]
    public void Isolation_IsReported()
    {
        Assert.True(_document.lookupSvgEntity("isolated")!.Isolate);
    }

    [Fact]
    public void Defaults_WhenNothingIsSet()
    {
        var entity = _document.lookupSvgEntity("plain")!;
        Assert.Equal(1f, entity.Opacity);
        Assert.Equal(BlendMode.Normal, entity.BlendMode);
        Assert.Equal(string.Empty, entity.MaskId);
        Assert.Equal(string.Empty, entity.ClipPathId);
        Assert.False(entity.Isolate);
    }
}

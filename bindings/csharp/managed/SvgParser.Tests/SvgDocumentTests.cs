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
public class SvgDocumentTests
{
    [Fact]
    public void GetRoot_ReturnsSvgEntity()
    {
        using SvgDocument document = TestSvg.Parse("<svg><path id=\"p1\" d=\"M0,0 L1,1 Z\"/></svg>");

        Svg root = document.getRoot();

        Assert.Equal("Svg", root.getType);
    }

    [Fact]
    public void GetStyle_WithStyleElement_ReturnsItsContent()
    {
        using SvgDocument document = TestSvg.Parse("""
            <svg>
              <style>.my-class { fill: red; }</style>
              <path id="p1" d="M0,0 L1,1 Z"/>
            </svg>
            """);

        Assert.Contains("my-class", document.getStyle());
    }

    [Fact]
    public void GetStyle_WithoutStyleElement_ReturnsEmpty()
    {
        using SvgDocument document = TestSvg.Parse("<svg><path id=\"p1\" d=\"M0,0 L1,1 Z\"/></svg>");

        Assert.Equal(string.Empty, document.getStyle());
    }

    [Fact]
    public void LookupSvgEntity_ExistingId_ReturnsCorrectlyTypedEntity()
    {
        using SvgDocument document = TestSvg.Parse("""
            <svg><g><rect id="r1" x="1" y="2" width="3" height="4"/></g></svg>
            """);

        SvgEntity? entity = document.lookupSvgEntity("r1");

        Assert.IsType<Rect>(entity);
        Assert.Equal("r1", entity!.Id);
    }

    [Fact]
    public void LookupSvgEntity_MissingId_ReturnsNull()
    {
        using SvgDocument document = TestSvg.Parse("<svg><path id=\"p1\" d=\"M0,0 L1,1 Z\"/></svg>");

        Assert.Null(document.lookupSvgEntity("does-not-exist"));
    }

    [Fact]
    public void Dispose_CalledTwice_DoesNotThrow()
    {
        SvgDocument document = TestSvg.Parse("<svg><path id=\"p1\" d=\"M0,0 L1,1 Z\"/></svg>");

        document.Dispose();
        document.Dispose();
    }
}

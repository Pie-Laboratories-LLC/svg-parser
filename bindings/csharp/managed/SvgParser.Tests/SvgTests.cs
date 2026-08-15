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
public class SvgTests : IDisposable
{
    private readonly SvgDocument _document;

    public SvgTests()
    {
        _document = TestSvg.Parse("""
            <svg x="1" y="2" width="300" height="150">
              <path id="p1" d="M0,0 L1,1 Z"/>
              <path id="p2" d="M0,0 L2,2 Z"/>
            </svg>
            """);
    }

    public void Dispose() => _document.Dispose();

    [Fact]
    public void Dimensions_ReturnSetValues()
    {
        Svg root = _document.RootSvg;

        Assert.Equal(1f, root.X!.Value.Value);
        Assert.Equal(2f, root.Y!.Value.Value);
        Assert.Equal(300f, root.Width!.Value.Value);
        Assert.Equal(150f, root.Height!.Value.Value);
    }

    [Fact]
    public void EnumerateChildren_ReturnsChildrenInDocumentOrder()
    {
        List<SvgEntity> children = _document.RootSvg.enumerateChildren();

        Assert.Equal(["p1", "p2"], children.Select(c => c.Id));
    }
}

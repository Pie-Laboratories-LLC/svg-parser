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
public class GroupTests : IDisposable
{
    private readonly SvgDocument _document;
    private readonly Group _group;

    public GroupTests()
    {
        _document = TestSvg.Parse("""
            <svg>
              <g id="g1">
                <path id="p1" d="M0,0 L1,1 Z"/>
                <path id="p2" d="M0,0 L2,2 Z"/>
              </g>
            </svg>
            """);

        _group = (Group)_document.lookupSvgEntity("g1")!;
    }

    public void Dispose() => _document.Dispose();

    [Fact]
    public void EnumerateChildren_ReturnsChildrenInDocumentOrder()
    {
        List<SvgEntity> children = _group.enumerateChildren();

        Assert.Equal(["p1", "p2"], children.Select(c => c.Id));
        Assert.All(children, c => Assert.IsType<Path>(c));
    }
}

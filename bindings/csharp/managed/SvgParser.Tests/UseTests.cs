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
public class UseTests : IDisposable
{
    private readonly SvgDocument _document;
    private readonly Use _use;

    public UseTests()
    {
        _document = TestSvg.Parse("""
            <svg>
              <defs><path id="target" d="M0,0 L10,0 Z"/></defs>
              <use id="u1" xlink:href="#target" x="5" y="7"/>
            </svg>
            """);

        _use = (Use)_document.lookupSvgEntity("u1")!;
    }

    public void Dispose() => _document.Dispose();

    [Fact]
    public void X_Y_ReturnSetValues()
    {
        Assert.Equal(5f, _use.X!.Value.Value);
        Assert.Equal(7f, _use.Y!.Value.Value);
    }

    [Fact]
    public void EnumerateChildren_ReturnsTheReferencedEntity()
    {
        SvgEntity child = Assert.Single(_use.enumerateChildren());

        Assert.IsType<Path>(child);
        Assert.Equal("target", child.Id);
    }
}

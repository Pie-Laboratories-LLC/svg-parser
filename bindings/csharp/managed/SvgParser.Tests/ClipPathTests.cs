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
public class ClipPathTests : IDisposable
{
    private readonly SvgDocument _document;

    public ClipPathTests()
    {
        _document = TestSvg.Parse("""
            <svg><defs>
              <clipPath id="c-obb" clipPathUnits="objectBoundingBox">
                <circle cx="0.5" cy="0.5" r="0.5"/>
              </clipPath>
              <clipPath id="c-bare"><rect width="1" height="1"/></clipPath>
            </defs></svg>
            """);
    }

    public void Dispose() => _document.Dispose();

    [Fact]
    public void ClipPathUnits_ReturnsSetValue()
    {
        var clipPath = (ClipPath)_document.lookupSvgEntity("c-obb")!;
        Assert.Equal(GradientUnits.ObjectBoundingBox, clipPath.ClipPathUnits);
    }

    [Fact]
    public void ClipPathUnits_DefaultsToUserSpaceOnUse()
    {
        var clipPath = (ClipPath)_document.lookupSvgEntity("c-bare")!;
        Assert.Equal(GradientUnits.UserSpaceOnUse, clipPath.ClipPathUnits);
    }

    [Fact]
    public void EnumerateChildren_ReturnsGeometry()
    {
        var clipPath = (ClipPath)_document.lookupSvgEntity("c-obb")!;
        Assert.IsType<Circle>(Assert.Single(clipPath.enumerateChildren()));
    }
}

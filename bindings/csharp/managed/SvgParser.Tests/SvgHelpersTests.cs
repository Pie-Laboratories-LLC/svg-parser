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
public class SvgHelpersTests
{
    [Fact]
    public void GetChildren_BuildsCorrectlyTypedEntity_ForEachRecognizedTag_InDocumentOrder()
    {
        using SvgDocument document = TestSvg.Parse("""
            <svg>
              <defs><path id="target" d="M0,0 L1,1 Z"/></defs>
              <g id="parent">
                <rect id="c-rect" x="0" y="0" width="1" height="1"/>
                <circle id="c-circle" cx="0" cy="0" r="1"/>
                <ellipse id="c-ellipse" cx="0" cy="0" rx="1" ry="1"/>
                <path id="c-path" d="M0,0 L1,1 Z"/>
                <use id="c-use" xlink:href="#target"/>
                <g id="c-group"><path id="nested" d="M0,0 L1,1 Z"/></g>
              </g>
            </svg>
            """);

        Svg root = document.getRoot();
        var parent = (Group)root.enumerateChildren().Single(c => c.Id == "parent");
        List<SvgEntity> children = parent.enumerateChildren();

        Assert.Equal(6, children.Count);
        Assert.Collection(children,
            c => Assert.IsType<Rect>(c),
            c => Assert.IsType<Circle>(c),
            c => Assert.IsType<Ellipse>(c),
            c => Assert.IsType<Path>(c),
            c => Assert.IsType<Use>(c),
            c => Assert.IsType<Group>(c));
        Assert.Equal(["c-rect", "c-circle", "c-ellipse", "c-path", "c-use", "c-group"],
            children.Select(c => c.Id));
    }
}

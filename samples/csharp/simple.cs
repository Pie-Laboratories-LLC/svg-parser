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

// dotnet run simple.cs

#:package PieLaboratories.SvgParser@1.0.0-alpha01

using PieLaboratories.Svg;

var svgText = """
<svg>
  <g id="g2a">
    <path id="p2a_1" d="M0,0 L10,10 Z"/>
    <path id="p2a_2" d="M5,5 L15,15 Z"/>
  </g>
</svg>
""";

SvgParser.init();

try {
    SvgParser svgParser = new();
    using(SvgDocument? svgDocument = svgParser.Parse(svgText))
    {
        if (svgDocument == null) throw new Exception("SVG document didn't parse.");
        var groupEntity = svgDocument.lookupSvgEntity("g2a");
        Console.WriteLine("Found a group with id {groupEntity.Id}");
        // alternatively
        var svg = svgDocument.RootSvg;

        foreach(var child in svg.enumerateChildren()) {
            Console.WriteLine($"have a child with id {child.Id} of type {child.Type}");
            if(child is Group groupElement) {
                foreach(var groupChild in groupElement.enumerateChildren()) {
                    Console.WriteLine($"Group has a child with id {groupChild.Id} of type {groupChild.Type}");
                }
            }
        }
    }
}
finally {
    SvgParser.shutdown();
}

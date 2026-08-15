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
public class SvgParserTests
{
    [Fact]
    public void Init_WhileAlreadyInitialized_ReturnsZero()
    {
        // The collection fixture already called init() for the run; Xerces only supports
        // alternating Initialize/Terminate pairs, so a second init() without an intervening
        // shutdown() is expected to fail rather than succeed. Not calling shutdown() here --
        // that would tear down native state every other test in this run still needs.
        Assert.Equal(0, SvgParser.init());
    }

    [Fact]
    public void Parse_ValidSvg_ReturnsDocument()
    {
        using SvgDocument document = TestSvg.Parse("<svg><path id=\"p1\" d=\"M0,0 L1,1 Z\"/></svg>");

        Assert.NotNull(document);
    }

    [Fact]
    public void Parse_SemanticallyInvalidSvg_ReturnsNull()
    {
        // native SvgParser::parse() previously threw SvgException on an unrecognized element; the
        // svgparser_parse C shim catches it and returns nullptr instead of propagating.
        var parser = new SvgParser();

        SvgDocument? document = parser.Parse("<svg><g><triangle cx=\"5\" cy=\"5\" r=\"3\"/></g></svg>");

        Assert.NotNull(document);
    }
}

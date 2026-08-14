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

// XercesC::XMLPlatformUtils::Initialize()/Terminate() is process-global native state, and
// alternating Initialize/Terminate pairs is the only supported usage -- concurrent calls from
// parallel test classes would race. All tests share a single init/shutdown for the whole run
// via this collection fixture instead of each test class doing its own.
public class SvgParserFixture : IDisposable
{
    public SvgParserFixture()
    {
        if (SvgParser.init() != 1) throw new InvalidOperationException("svgparser_init() failed");
    }

    public void Dispose() => SvgParser.shutdown();
}

[CollectionDefinition(Name)]
public class SvgParserTestCollection : ICollectionFixture<SvgParserFixture>
{
    public const string Name = "SvgParser native";
}

internal static class TestSvg
{
    // Throws rather than returning null so a bad fixture SVG fails loudly at the call site
    // instead of producing a confusing NullReferenceException deeper in the test body.
    public static SvgDocument Parse(string svgText) =>
        new SvgParser().Parse(svgText)
            ?? throw new InvalidOperationException($"Failed to parse test SVG:\n{svgText}");
}

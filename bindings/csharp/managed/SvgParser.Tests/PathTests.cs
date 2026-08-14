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
public class PathTests : IDisposable
{
    private readonly SvgDocument _document;
    private readonly Path _path;

    public PathTests()
    {
        _document = TestSvg.Parse("""<svg><path id="p1" d="M0,0 L10,0 L10,10 Z"/></svg>""");
        _path = (Path)_document.lookupSvgEntity("p1")!;
    }

    public void Dispose() => _document.Dispose();

    [Fact]
    public void PathMoves_MatchesTheDString()
    {
        Assert.Equal(
            [PathMove.MoveTo, PathMove.LineTo, PathMove.LineTo, PathMove.ClosePath],
            _path.PathMoves);
    }

    [Fact]
    public void Points_MatchesTheDString()
    {
        Assert.Equal(new float[] { 0, 0, 10, 0, 10, 10 }, _path.Points);
    }
}

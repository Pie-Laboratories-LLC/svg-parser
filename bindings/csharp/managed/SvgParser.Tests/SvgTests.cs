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
    private readonly SvgDocument _document2;
    private readonly Svg _full;
    private readonly Svg _bare;

    public SvgTests()
    {
        _document = TestSvg.Parse("""
            <svg x="1" y="2" width="300" height="150">
              <path id="p1" d="M0,0 L1,1 Z"/>
              <path id="p2" d="M0,0 L2,2 Z"/>
            </svg>
            """);
        _document2 = TestSvg.Parse("""
            <svg id="s-bare">
              <svg id="s-full" viewBox="10 20 300 150" preserveAspectRatio="xMidYMid slice"/>
            </svg>
            """);
        _full = (Svg)_document2.lookupSvgEntity("s-full")!;
        if(_full == null) throw new Exception("_full is not set");
        _bare = (Svg)_document2.lookupSvgEntity("s-bare")!;
        if(_bare == null) throw new Exception("_bare is not set");
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

    [Fact]
    public void ViewBox_ReturnsSetValues()
    {
        ViewBox? viewBox = _full.ViewBox;

        Assert.NotNull(viewBox);
        Assert.Equal(10f, viewBox!.x);
        Assert.Equal(20f, viewBox!.y);
        Assert.Equal(300f, viewBox!.width);
        Assert.Equal(150f, viewBox!.height);
    }

    [Fact]
    public void ViewBox_ReturnsNullWhenAbsent()
    {
        Assert.Null(_bare.ViewBox);
    }

    [Fact]
    public void PreserveAspectRatio_ReturnsSetValue()
    {
        Assert.Equal(PreserveAspectRatio.xMidYMid, _full.PreserveAspectRatio);
        Assert.Equal(PreserveAspectRatioMode.Slice, _full.PreserveAspectRatioMode);
    }
}

// Deliberately not namespace "SvgParser.Tests" -- the "SvgParser" segment would
// collide with the top-level SvgParser class in the referenced assembly (CS0118).
namespace SvgParserTests;

public class SvgParserSmokeTests : IDisposable
{
    public SvgParserSmokeTests()
    {
        Assert.Equal(1, global::SvgParser.init());
    }

    public void Dispose()
    {
        global::SvgParser.shutdown();
    }

    [Fact]
    public void Parse_SimpleRect_ProducesExpectedTree()
    {
        var parser = new global::SvgParser();
        using SvgDocument document = parser.Parse("""
            <svg>
              <g id="g1">
                <rect id="r1" x="5" y="6" width="7.5" height="8" fill-opacity="0.5" stroke-width="2.5"/>
              </g>
            </svg>
            """);

        Svg root = document.getRoot();
        var group = (Group)root.enumerateChildren().Single(c => c.Id == "g1");
        var rect = (Rect)group.enumerateChildren().Single(c => c.Id == "r1");

        Assert.Equal(5f, rect.X?.Value);
        Assert.Equal(6f, rect.Y?.Value);
        Assert.Equal(7.5f, rect.Width?.Value);
        Assert.Equal(8f, rect.Height?.Value);
        Assert.Equal(0.5f, rect.FillOpacity);
        Assert.Equal(2.5f, rect.StrokeWidth);
    }
}

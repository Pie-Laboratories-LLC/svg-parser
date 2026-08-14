namespace PieLaboratories.Svg;

public class SvgParser {
    public static int init() {
        return SvgParserNative.svgparser_init();
    }

    public static void shutdown()
    {
        SvgParserNative.svgparser_shutdown();
    }

    public SvgDocument Parse(string svgText)
    {
        IntPtr document = SvgParserNative.svgparser_parse(svgText);

        return new SvgDocument (document);
    }
}

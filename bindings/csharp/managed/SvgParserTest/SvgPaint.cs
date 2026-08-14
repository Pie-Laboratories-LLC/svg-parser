public class SvgPaint {
    IntPtr _paint;
    public SvgPaint(IntPtr paint) {
        _paint = paint;
    }

    public SvgColourType SvgColourType => SvgParserNative.GetPaintColourType(_paint);
    public string GradientId => SvgParserNative.PtrToString(SvgParserNative.svgparser_paint_get_gradient_id(_paint));
    public SvgColour SvgColour =>  new SvgColour(SvgParserNative.svgparser_paint_get_colour(_paint));
    public SvgColour? Fallback {
        get
        {
            if(SvgParserNative.svgparser_paint_has_fallback(_paint) == 0) return null;
            return new SvgColour(SvgParserNative.svgparser_paint_get_fallback(_paint));
        }
    }
}

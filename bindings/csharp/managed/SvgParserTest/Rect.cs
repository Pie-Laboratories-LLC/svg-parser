public class Rect: SvgEntity, ISvgDimensionedEntity {
    private IntPtr _rect;

    public Rect(IntPtr rect): base(rect) {
        _rect = rect;
    }

    public Dimension? X => SvgHelpers.GetDimensionedX(_rect);
    public Dimension? Y => SvgHelpers.GetDimensionedY(_rect);
    public Dimension? Width => SvgHelpers.GetDimensionedWidth(_rect);
    public Dimension? Height => SvgHelpers.GetDimensionedHeight(_rect);

    public Dimension? Rx {
        get {
            if(SvgParserNative.svgparser_rect_has_rx(_rect) == 0) return null;
            return new Dimension (
                SvgParserNative.svgparser_rect_get_rx_measurement(_rect),
                SvgParserNative.GetRectRxUnits(_rect)
            );
        }
    }

    public Dimension? Ry {
        get {
            if(SvgParserNative.svgparser_rect_has_ry(_rect) == 0) return null;
            return new Dimension (
                SvgParserNative.svgparser_rect_get_ry_measurement(_rect),
                SvgParserNative.GetRectRyUnits(_rect)
            );
        }
    }
}

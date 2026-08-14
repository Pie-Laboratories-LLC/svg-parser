public class Circle: SvgEntity {
    private IntPtr _circle;
    public Circle(IntPtr circle): base(circle) {
        _circle = circle;
    }

    public Dimension? Cx {
        get {
            if(SvgParserNative.svgparser_circle_has_cx(_circle) == 0) return null;
            return new Dimension (
                SvgParserNative.svgparser_circle_get_cx_measurement(_circle),
                SvgParserNative.GetCircleCxUnits(_circle)
            );
        }
    }

    public Dimension? Cy {
        get {
            if(SvgParserNative.svgparser_circle_has_cy(_circle) == 0) return null;
            return new Dimension (
                SvgParserNative.svgparser_circle_get_cy_measurement(_circle),
                SvgParserNative.GetCircleCyUnits(_circle)
            );
        }
    }

    public Dimension? R {
        get {
            if(SvgParserNative.svgparser_circle_has_r(_circle) == 0) return null;
            return new Dimension (
                SvgParserNative.svgparser_circle_get_r_measurement(_circle),
                SvgParserNative.GetCircleRUnits(_circle)
            );
        }
    }
}

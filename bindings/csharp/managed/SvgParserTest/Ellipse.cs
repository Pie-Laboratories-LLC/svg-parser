public class Ellipse: SvgEntity {
    private IntPtr _ellipse;
    public Ellipse(IntPtr ellipse): base(ellipse) {
        _ellipse = ellipse;
    }

    public Dimension? Cx {
        get {
            if(SvgParserNative.svgparser_ellipse_has_cx(_ellipse) == 0) return null;
            return new Dimension (
                SvgParserNative.svgparser_ellipse_get_cx_measurement(_ellipse),
                SvgParserNative.GetEllipseCxUnits(_ellipse)
            );
        }
    }

    public Dimension? Cy {
        get {
            if(SvgParserNative.svgparser_ellipse_has_cy(_ellipse) == 0) return null;
            return new Dimension (
                SvgParserNative.svgparser_ellipse_get_cy_measurement(_ellipse),
                SvgParserNative.GetEllipseCyUnits(_ellipse)
            );
        }
    }

    public Dimension? Rx {
        get {
            if(SvgParserNative.svgparser_ellipse_has_rx(_ellipse) == 0) return null;
            return new Dimension (
                SvgParserNative.svgparser_ellipse_get_rx_measurement(_ellipse),
                SvgParserNative.GetEllipseRxUnits(_ellipse)
            );
        }
    }

    public Dimension? Ry {
        get {
            if(SvgParserNative.svgparser_ellipse_has_ry(_ellipse) == 0) return null;
            return new Dimension (
                SvgParserNative.svgparser_ellipse_get_ry_measurement(_ellipse),
                SvgParserNative.GetEllipseRyUnits(_ellipse)
            );
        }
    }
}

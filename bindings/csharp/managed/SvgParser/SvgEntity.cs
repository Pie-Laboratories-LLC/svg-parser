namespace PieLaboratories.Svg;

public class SvgEntity {
    private IntPtr _entity;
    public SvgEntity(IntPtr entity) {
        _entity = entity;
    }

    virtual public bool Render => SvgParserNative.svgparser_entity_get_render(_entity) > 0 ? true : false;
    virtual public string Id => SvgParserNative.PtrToString(SvgParserNative.svgparser_entity_get_id(_entity));
    virtual public float[]? Matrix {
        get {
            float[] matrix = new float[6];
            int result = SvgParserNative.svgparser_entity_get_matrix(_entity, matrix);
            if(result == 0) return null;
            return matrix;
        }
    }

    virtual public SvgPaint FillColour => new SvgPaint(SvgParserNative.svgparser_entity_get_fill(_entity));
    virtual public FillRule FillRulea => SvgParserNative.GetFillRule(_entity);
    virtual public float FillOpacity => SvgParserNative.svgparser_entity_get_fill_opacity(_entity);
    virtual public SvgPaint StrokeColour => new SvgPaint(SvgParserNative.svgparser_entity_get_stroke(_entity));
    virtual public float StrokeOpacity => SvgParserNative.svgparser_entity_get_stroke_opacity(_entity);
    virtual public float StrokeWidth => SvgParserNative.svgparser_entity_get_stroke_width(_entity);
    virtual public LineCap LineCap => SvgParserNative.GetLineCap(_entity);
    virtual public LineJoin LineJoin => SvgParserNative.GetLineJoin(_entity);
    virtual public float MiterLimit => SvgParserNative.svgparser_entity_get_miter_limit(_entity);
    virtual public float[] DashArray {
        get {
            int dashArrayCount = SvgParserNative.svgparser_entity_get_dash_array_count(_entity);
            float[] dashArray = new float[dashArrayCount];
            SvgParserNative.svgparser_entity_get_dash_array(_entity, dashArray, dashArrayCount);
            return dashArray;
        }
    }
    virtual public string CssClass => SvgParserNative.PtrToString(SvgParserNative.svgparser_entity_get_css_class(_entity));
    virtual public string CssStyle => SvgParserNative.PtrToString(SvgParserNative.svgparser_entity_get_css_style(_entity));
    virtual public string getType => SvgParserNative.PtrToString(SvgParserNative.svgparser_entity_get_type(_entity));
}

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
    virtual public FillRule FillRule => SvgParserNative.GetFillRule(_entity);
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
    virtual public string Type => SvgParserNative.PtrToString(SvgParserNative.svgparser_entity_get_type(_entity));

    virtual public bool IsDimensionedEntity {
        get {
            IntPtr dimensionedEntity = SvgParserNative.svgparser_entity_get_dimensioned(_entity);
            return dimensionedEntity != IntPtr.Zero;
        }
    }
}

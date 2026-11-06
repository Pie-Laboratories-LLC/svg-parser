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

public class Text: SvgEntity {
    private IntPtr _text;
    public Text(IntPtr text): base(text) {
        _text = text;
    }

    public float[] X {
        get {
            int xCount = SvgParserNative.svgparser_text_get_x_count(_text);
            float []x = new float[xCount];
            SvgParserNative.svgparser_text_get_x(_text, x, xCount);
            return x;
        }
    }

    public float[] Y {
        get {
            int yCount = SvgParserNative.svgparser_text_get_y_count(_text);
            float []y = new float[yCount];
            SvgParserNative.svgparser_text_get_y(_text, y, yCount);
            return y;
        }
    }

    public float[] Dx {
        get {
            int dxCount = SvgParserNative.svgparser_text_get_dx_count(_text);
            float []dx = new float[dxCount];
            SvgParserNative.svgparser_text_get_dx(_text, dx, dxCount);
            return dx;
        }
    }

    public float[] Dy {
        get {
            int dyCount = SvgParserNative.svgparser_text_get_dy_count(_text);
            float []dy = new float[dyCount];
            SvgParserNative.svgparser_text_get_dy(_text, dy, dyCount);
            return dy;
        }
    }

    public float[] Rotate {
        get {
            int rotateCount = SvgParserNative.svgparser_text_get_rotate_count(_text);
            float []rotate = new float[rotateCount];
            SvgParserNative.svgparser_text_get_rotate(_text, rotate, rotateCount);
            return rotate;
        }
    }

    public LengthAdjust LengthAdjust => SvgParserNative.GetLengthAdjust(_text);

    public float? TextLength => SvgParserNative.svgparser_text_has_text_length(_text) > 0 ? SvgParserNative.svgparser_text_get_text_length(_text) : null;
    // whether TextLength was written as a percentage
    public bool TextLengthPercentage => SvgParserNative.svgparser_text_get_text_length_percentage(_text) > 0;

    // the characters and nested tspans, in document order
    public List<TextPart> Parts => TextPart.FromRawText(SvgParserNative.svgparser_text_get_raw_text(_text));
    // all the text, tspans included, with whitespace collapsed as a browser would
    public string Content => SvgParserNative.ConcatenateText(SvgParserNative.svgparser_text_get_raw_text(_text));

    public float? FontSize => SvgParserNative.svgparser_text_has_font_size(_text) > 0 ? SvgParserNative.svgparser_text_get_font_size(_text) : null;
    public DimensionUnits? FontSizeUnits => SvgParserNative.svgparser_text_has_font_size_units(_text) > 0 ? SvgParserNative.GetFontSizeUnits(_text) : null;
    // kept as written, e.g. "Helvetica, sans-serif"; null if absent
    public string? FontFamily {
        get {
            IntPtr fontFamily = SvgParserNative.svgparser_text_get_font_family(_text);
            return fontFamily == IntPtr.Zero ? null : SvgParserNative.PtrToString(fontFamily);
        }
    }
    public TextAnchor? TextAnchor => SvgParserNative.svgparser_text_has_text_anchor(_text) > 0 ? SvgParserNative.GetTextAnchor(_text) : null;
    public FontStyle? FontStyle => SvgParserNative.svgparser_text_has_font_style(_text) > 0 ? SvgParserNative.GetFontStyle(_text) : null;
}

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

public class TSpan {
    private IntPtr _tspan;
    public TSpan(IntPtr tspan) {
        _tspan = tspan;
    }

    // the characters and nested tspans, in document order
    public List<TextPart> Parts => TextPart.FromRawText(SvgParserNative.svgparser_tspan_get_raw_text(_tspan));
    // all the text, nested tspans included, with whitespace collapsed as a browser would
    public string Content => SvgParserNative.ConcatenateText(SvgParserNative.svgparser_tspan_get_raw_text(_tspan));

    public float[] X {
        get {
            int xCount = SvgParserNative.svgparser_tspan_get_x_count(_tspan);
            float []x = new float[xCount];
            SvgParserNative.svgparser_tspan_get_x(_tspan, x, xCount);
            return x;
        }
    }

    public float[] Y {
        get {
            int yCount = SvgParserNative.svgparser_tspan_get_y_count(_tspan);
            float []y = new float[yCount];
            SvgParserNative.svgparser_tspan_get_y(_tspan, y, yCount);
            return y;
        }
    }

    public float[] Dx {
        get {
            int dxCount = SvgParserNative.svgparser_tspan_get_dx_count(_tspan);
            float []dx = new float[dxCount];
            SvgParserNative.svgparser_tspan_get_dx(_tspan, dx, dxCount);
            return dx;
        }
    }

    public float[] Dy {
        get {
            int dyCount = SvgParserNative.svgparser_tspan_get_dy_count(_tspan);
            float []dy = new float[dyCount];
            SvgParserNative.svgparser_tspan_get_dy(_tspan, dy, dyCount);
            return dy;
        }
    }

    public float[] Rotate {
        get {
            int rotateCount = SvgParserNative.svgparser_tspan_get_rotate_count(_tspan);
            float []rotate = new float[rotateCount];
            SvgParserNative.svgparser_tspan_get_rotate(_tspan, rotate, rotateCount);
            return rotate;
        }
    }

    public LengthAdjust LengthAdjust => SvgParserNative.GetTSpanLengthAdjust(_tspan);

    public float? TextLength => SvgParserNative.svgparser_tspan_has_text_length(_tspan) > 0 ? SvgParserNative.svgparser_tspan_get_text_length(_tspan) : null;
    // whether TextLength was written as a percentage
    public bool TextLengthPercentage => SvgParserNative.svgparser_tspan_get_text_length_percentage(_tspan) > 0;
}

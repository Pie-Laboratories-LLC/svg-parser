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

public class SvgPaint {
    IntPtr _paint;
    public SvgPaint(IntPtr paint) {
        _paint = paint;
    }

    public SvgColourType SvgColourType => SvgParserNative.GetPaintColourType(_paint);
    public string GradientId => SvgParserNative.PtrToString(SvgParserNative.svgparser_paint_get_gradient_id(_paint));
    // null when SvgColourType == Gradient: svgparser_paint_get_colour() returns nullptr in that
    // case, and native code doesn't tolerate being handed a null SvgColour* back.
    public SvgColour? SvgColour {
        get {
            IntPtr colour = SvgParserNative.svgparser_paint_get_colour(_paint);
            return colour == IntPtr.Zero ? null : new SvgColour(colour);
        }
    }
    public SvgColour? Fallback {
        get
        {
            if(SvgParserNative.svgparser_paint_has_fallback(_paint) == 0) return null;
            return new SvgColour(SvgParserNative.svgparser_paint_get_fallback(_paint));
        }
    }
}

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

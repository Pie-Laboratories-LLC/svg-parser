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

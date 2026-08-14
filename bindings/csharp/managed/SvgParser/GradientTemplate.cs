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

public class GradientTemplate {
    private IntPtr _gradientTemplate;
    public GradientTemplate(IntPtr gradientTemplate) {
        _gradientTemplate = gradientTemplate;
    }

    public GradientKind GradientKind => SvgParserNative.GetGradientKind(_gradientTemplate);
    public float[] Geometry {
        get {
            float[] geometry = new float[6];
            if(SvgParserNative.svgparser_gradient_get_geometry(_gradientTemplate, geometry, geometry.Length) == 0) throw new Exception("Failed to get gradient geometry");
            return geometry;
        }
    }
    public GradientUnits GradientUnits => SvgParserNative.GetGradientUnits(_gradientTemplate);
    public SpreadMethod SpreadMethod => SvgParserNative.GetGradientSpreadMethod(_gradientTemplate);
    public float[]? Transform {
        get {
            float[] transform = new float[6];
            if(SvgParserNative.svgparser_gradient_get_transform(_gradientTemplate, transform, transform.Length) == 0) throw new Exception("Failed to get gradient transform");
            return transform;
        }
    }

    public string HrefId => SvgParserNative.PtrToString(SvgParserNative.svgparser_gradient_get_href_id(_gradientTemplate));
}

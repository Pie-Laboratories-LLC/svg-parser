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

public class Mask: SvgEntity, ISvgContainerEntity {
    private IntPtr _mask;
    public Mask(IntPtr mask): base(mask) {
        _mask = mask;
    }

    // the mask region; null where it wasn't set, leaving the default for MaskUnits
    public Dimension? X => SvgParserNative.svgparser_mask_has_x(_mask) == 0 ? null
        : new Dimension(SvgParserNative.svgparser_mask_get_x_measurement(_mask), SvgParserNative.GetMaskXUnits(_mask));
    public Dimension? Y => SvgParserNative.svgparser_mask_has_y(_mask) == 0 ? null
        : new Dimension(SvgParserNative.svgparser_mask_get_y_measurement(_mask), SvgParserNative.GetMaskYUnits(_mask));
    public Dimension? Width => SvgParserNative.svgparser_mask_has_width(_mask) == 0 ? null
        : new Dimension(SvgParserNative.svgparser_mask_get_width_measurement(_mask), SvgParserNative.GetMaskWidthUnits(_mask));
    public Dimension? Height => SvgParserNative.svgparser_mask_has_height(_mask) == 0 ? null
        : new Dimension(SvgParserNative.svgparser_mask_get_height_measurement(_mask), SvgParserNative.GetMaskHeightUnits(_mask));

    public GradientUnits MaskUnits => SvgParserNative.GetMaskUnits(_mask);
    public GradientUnits MaskContentUnits => SvgParserNative.GetMaskContentUnits(_mask);

    public List<SvgEntity> enumerateChildren() {
        return SvgHelpers.GetChildren(_mask);
    }
}

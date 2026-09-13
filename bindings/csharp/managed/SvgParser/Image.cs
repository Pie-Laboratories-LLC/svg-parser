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

public class Image: SvgEntity, ISvgDimensionedEntity {
    private IntPtr _image;
    public Image(IntPtr image): base(image) {
        _image = image;
    }

    public Dimension? X => SvgHelpers.GetDimensionedX(_image);
    public Dimension? Y => SvgHelpers.GetDimensionedY(_image);
    public Dimension? Width => SvgHelpers.GetDimensionedWidth(_image);
    public Dimension? Height => SvgHelpers.GetDimensionedHeight(_image);

    public string Href => SvgParserNative.PtrToString(SvgParserNative.svgparser_image_get_href(_image));
    public string ImageType => SvgParserNative.PtrToString(SvgParserNative.svgparser_image_get_image_type(_image));
    public string CharacterEncoding => SvgParserNative.PtrToString(SvgParserNative.svgparser_image_get_character_encoding(_image));

    public PreserveAspectRatio PreserveAspectRatio => SvgParserNative.GetPreserveAspectRatio_Image(_image);
    public PreserveAspectRatioMode PreserveAspectRatioMode => SvgParserNative.GetPreserveAspectRatioMode_Image(_image);

    public CrossOrigin? CrossOrigin => SvgParserNative.svgparser_image_get_has_cross_origin(_image) > 0 ? SvgParserNative.GetCrossOrigin(_image) : null;
    public Decoding? Decoding => SvgParserNative.svgparser_image_get_has_decoding(_image) > 0 ? SvgParserNative.GetDecoding(_image) : null;
    public FetchPriority? FetchPriority => SvgParserNative.svgparser_image_get_has_fetch_priority(_image) > 0 ? SvgParserNative.GetFetchPriority(_image) : null;
}

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

// One piece of a <text> or <tspan>'s content: either a run of characters, kept exactly
// as written, or a nested <tspan>.
public class TextPart {
    public string? Text { get; }
    public TSpan? TSpan { get; }
    public bool IsTSpan => TSpan != null;

    internal TextPart(string text) { Text = text; }
    internal TextPart(TSpan tspan) { TSpan = tspan; }

    internal static List<TextPart> FromRawText(IntPtr rawText) {
        List<TextPart> parts = new();
        int count = SvgParserNative.svgparser_rawtext_get_part_count(rawText);
        for(int index = 0; index < count; ++index) {
            if(SvgParserNative.svgparser_rawtext_part_is_tspan(rawText, index) > 0) {
                parts.Add(new TextPart(new TSpan(SvgParserNative.svgparser_rawtext_get_part_tspan(rawText, index))));
            } else {
                parts.Add(new TextPart(SvgParserNative.PtrToString(SvgParserNative.svgparser_rawtext_get_part_string(rawText, index))));
            }
        }
        return parts;
    }
}

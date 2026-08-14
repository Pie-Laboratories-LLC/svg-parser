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

public class SvgParser {
    public static int init() {
        return SvgParserNative.svgparser_init();
    }

    public static void shutdown()
    {
        SvgParserNative.svgparser_shutdown();
    }

    // null on parse failure: svgparser_parse() returns nullptr, and every SvgDocument
    // method calls a (non-virtual, but this-dereferencing) member function on the native
    // pointer -- wrapping a null pointer here would crash the process on first use rather
    // than surfacing a catchable error.
    public SvgDocument? Parse(string svgText)
    {
        IntPtr document = SvgParserNative.svgparser_parse(svgText);
        if (document == IntPtr.Zero) return null;

        return new SvgDocument (document);
    }
}

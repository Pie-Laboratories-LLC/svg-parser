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

using System.Runtime.InteropServices;

public class SvgParser : IDisposable {
    private IntPtr _handle;
    private GCHandle _callbackHandle;
    private SvgParserNative.ParserErrorCallback? _errorCallback;

    public event Action<SvgParserStatus, string>? OnParseError;

    public static int init() => SvgParserNative.svgparser_init();
    public static void shutdown() => SvgParserNative.svgparser_shutdown();

    public SvgParser() {
        _handle = SvgParserNative.svgparser_parser_create();
        if (_handle == IntPtr.Zero) throw new Exception("Failed to create native SvgParser.");
    }

    public void EnableErrorReporting() {
        _errorCallback = (status, msgPtr, userData) =>
            OnParseError?.Invoke((SvgParserStatus)status, SvgParserNative.PtrToString(msgPtr));
        _callbackHandle = GCHandle.Alloc(_errorCallback);
        SvgParserNative.svgparser_set_parser_callback(_handle, _errorCallback, IntPtr.Zero);
    }

    public SvgDocument? Parse(string svgText) {
        IntPtr document = SvgParserNative.svgparser_parser_parse(_handle, svgText);
        if (document == IntPtr.Zero) return null;
        return new SvgDocument(document);
    }

    public void Dispose() {
        if (_handle != IntPtr.Zero) {
            SvgParserNative.svgparser_parser_free(_handle);
            _handle = IntPtr.Zero;
        }
        if (_callbackHandle.IsAllocated) {
            _callbackHandle.Free();
        }
    }
}

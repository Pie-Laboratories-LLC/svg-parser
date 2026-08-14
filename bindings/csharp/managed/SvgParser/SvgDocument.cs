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

public class SvgDocument : IDisposable {
    IntPtr _document;

    public SvgDocument(IntPtr document) {
        _document = document;
    }

    public Svg getRoot() {
        IntPtr svg = SvgParserNative.svgparser_document_get_root(_document);
        return new Svg(svg);
    }

    public string getStyle() {
        return SvgParserNative.PtrToString(SvgParserNative.svgparser_document_get_style(_document));
    }

    public SvgEntity? lookupSvgEntity(string id) {
        IntPtr svgEntity = SvgParserNative.svgparser_document_lookup_svg_entity(_document,id);
        if(svgEntity != IntPtr.Zero) {
            return SvgHelpers.MakeCorrectEntity(svgEntity);
        }
        return null;
    }

    public GradientTemplate? lookupGradient(string id) {
        IntPtr gradient = SvgParserNative.svgparser_document_lookup_gradient(_document, id);
        if(gradient == IntPtr.Zero) return null;
        return new GradientTemplate(gradient);
    }

    public void Dispose()
    {
        if(_document != IntPtr.Zero) {
            SvgParserNative.svgparser_document_free(_document);
            _document = IntPtr.Zero;
        }
        // This object will be cleaned up by the Dispose method.
        // Therefore, you should call GC.SuppressFinalize to
        // take this object off the finalization queue
        // and prevent finalization code for this object
        // from executing a second time.
        GC.SuppressFinalize(this);
    }
}

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

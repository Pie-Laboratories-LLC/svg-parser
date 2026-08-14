using System;

string svg = """
<svg>
    <defs>
      <linearGradient id="lg" x1="0" y1="0" x2="1" y2="0">
        <stop offset="0" stop-color="#000000"/>
        <stop offset="1" stop-color="#ffffff"/>
      </linearGradient>
      <path id="p0defs" d="M0,0L10,0L10,10L0,10Z" fill="var(currentColor,#deadbeef)"/>
    </defs>
    
    <g id="g1" transform="matrix(1 0 0 1 13 24)" fill="url(#lg)">
      <path id="p1" d="M0,0 L10,10" fill="#deadbeef" stroke-dasharray="1 3 4 2"/>
    </g>
</svg>
""";

if (SvgParserNative.svgparser_init() == 0) {
    Console.WriteLine("Xerces init failed.");
    return;
}

IntPtr doc = SvgParserNative.svgparser_parse(svg);
if (doc == IntPtr.Zero) {
    Console.WriteLine("Parse failed.");
    return;
}

try {
    IntPtr root = SvgParserNative.svgparser_document_get_root(doc);
    PrintEntity(root, 0);
} finally {
    SvgParserNative.svgparser_document_free(doc);
    SvgParserNative.svgparser_shutdown();
}

void PrintEntity(IntPtr entity, int depth) {
    string id = SvgParserNative.PtrToString(SvgParserNative.svgparser_entity_get_id(entity));
    string type = SvgParserNative.PtrToString(SvgParserNative.svgparser_entity_get_type(entity));

    int count = SvgParserNative.svgparser_entity_get_dash_array_count(entity);
    float[] dashArray = new float[count];
    if (count > 0) {
        SvgParserNative.svgparser_entity_get_dash_array(entity, dashArray, count);
    }

    float[] matrixBuffer = new float[6];
    bool hasMatrix = SvgParserNative.svgparser_entity_get_matrix(entity, matrixBuffer) != 0;

    bool render = SvgParserNative.svgparser_entity_get_render(entity) != 0 ? true : false;

    Console.WriteLine(new string(' ', depth * 4) + $"{type} (id={id}) (render={render})");
    if(dashArray.Length > 0) {
        Console.WriteLine(new string(' ', depth * 4 + 2) + $"(dash array: {String.Join(", ", dashArray)})");
    }
    if(hasMatrix) {
        Console.WriteLine(new string(' ', depth * 4 + 2) + $"(matrix: {String.Join(' ', matrixBuffer)})");
    }

    IntPtr fillPaint = SvgParserNative.svgparser_entity_get_fill(entity);
    if(fillPaint != 0) {
        PrintPaint(fillPaint,"fill", depth);
        float fillOpacity = SvgParserNative.svgparser_entity_get_fill_opacity(entity);
        Console.WriteLine(new string(' ', depth * 4 + 2) + $"(fill opacity: {fillOpacity})");
        FillRule fillRule = SvgParserNative.GetFillRule(entity);
        Console.WriteLine(new string(' ', depth * 4 + 2) + $"(fill rule: {fillRule})");
    }
    IntPtr strokePaint = SvgParserNative.svgparser_entity_get_stroke(entity);
    if(strokePaint != 0) {
        PrintPaint(strokePaint,"stroke", depth);
        float strokeOpacity = SvgParserNative.svgparser_entity_get_stroke_opacity(entity);
        Console.WriteLine(new string(' ', depth * 4 + 2) + $"(stroke opacity: {strokeOpacity})");
        int strokeWidth = SvgParserNative.svgparser_entity_get_stroke_width(entity);
        Console.WriteLine(new string(' ', depth * 4 + 2) + $"(stroke width: {strokeWidth})");
        LineCap lineCap = SvgParserNative.GetLineCap(entity);
        Console.WriteLine(new string(' ', depth * 4 + 2) + $"(line cap: {lineCap})");
        LineJoin lineJoin = SvgParserNative.GetLineJoin(entity);
        Console.WriteLine(new string(' ', depth * 4 + 2) + $"(line join: {lineJoin})");
        float miterLimit = SvgParserNative.svgparser_entity_get_miter_limit(entity);
        Console.WriteLine(new string(' ', depth * 4 + 2) + $"(mitre limit: {miterLimit})");
    }

    string cssClass = SvgParserNative.PtrToString(SvgParserNative.svgparser_entity_get_css_class(entity));
    if(cssClass.Length > 0) Console.WriteLine(new string(' ', depth * 4 + 2) + $"(css class: {cssClass})");

    string cssStyle = SvgParserNative.PtrToString(SvgParserNative.svgparser_entity_get_css_style(entity));
    if(cssStyle.Length > 0) Console.WriteLine(new string(' ', depth * 4 + 2) + $"(css style: {cssStyle})");

    foreach (var child in SvgParserNative.GetChildren(entity)) {
        PrintEntity(child, depth + 1);
    }
}

void PrintPaint(IntPtr paint,string strName,int depth) {
    SvgColourType enumFillColourType = SvgParserNative.GetPaintColourType(paint);
    string colourDetails = "";
    List<string> stops = new();
    switch(enumFillColourType) {
        case SvgColourType.Gradient:
            string gradientId = SvgParserNative.PtrToString(SvgParserNative.svgparser_paint_get_gradient_id(paint));
            colourDetails = gradientId;
            var gradient = SvgParserNative.svgparser_document_lookup_gradient(doc,gradientId);
            if(gradient != 0) {
                float[] geometry = new float[6];
                GradientKind gradientKind = SvgParserNative.GetGradientKind(gradient);
                colourDetails += "; " + gradientKind + ". ";
                GradientUnits gradientUnits = SvgParserNative.GetGradientUnits(gradient);
                colourDetails += "; " + gradientUnits + ". ";
                SpreadMethod spreadMethod = SvgParserNative.GetGradientSpreadMethod(gradient);
                colourDetails += "; " + spreadMethod + ". ";
                SvgParserNative.svgparser_gradient_get_geometry(gradient, geometry, geometry.Length);
                if(gradientKind == GradientKind.Linear) {
                    colourDetails += "x1: " + geometry[0];
                    colourDetails += "; y1: " + geometry[1];
                    colourDetails += "; x2: " + geometry[2];
                    colourDetails += "; y2: " + geometry[3];
                }
                else {
                    colourDetails += "cx: " + geometry[0];
                    colourDetails += "; cy: " + geometry[1];
                    colourDetails += "; r: " + geometry[2];
                    colourDetails += "; fx: " + geometry[3];
                    colourDetails += "; fy: " + geometry[4];
                    colourDetails += "; fr: " + geometry[5];
                }
                float[] transform = new float[6];
                if(SvgParserNative.svgparser_gradient_get_transform(gradient, transform, transform.Length) != 0) {
                    colourDetails += "; xform = [" + String.Join(' ', transform) + ']';
                }
                string hrefId = SvgParserNative.PtrToString(SvgParserNative.svgparser_gradient_get_href_id(gradient));
                colourDetails += "; href inherit = " + hrefId;
                for(int i = 0; i < SvgParserNative.svgparser_gradient_get_stop_count(gradient); i++) {
                    IntPtr stop = SvgParserNative.svgparser_gradient_get_stop(gradient, i);
                    float offset = SvgParserNative.svgparser_stop_get_offset(stop);
                    float opacity = SvgParserNative.svgparser_stop_get_offset(stop);
                    IntPtr stopColour = SvgParserNative.svgparser_stop_get_colour(stop);
                    SvgColourType colourType = SvgParserNative.GetColourColourType(stopColour);
                    stops.Add (new string(' ', depth * 4 + 4) + "offset: " + offset
                                        + " opacity: " + opacity + " colourType=" + colourType);
                }
            }
            break;
        default:
            IntPtr colour = SvgParserNative.svgparser_paint_get_colour(paint);
            PrintColour(colour,strName,depth);
            return;
    }
    Console.WriteLine(new string(' ', depth * 4 + 2) + $"({strName}: {enumFillColourType}{(colourDetails.Length > 1 ? " - " + colourDetails : "")})");
    if(stops.Count > 0) {
        foreach(var line in stops) {
            Console.WriteLine(line);
        }
    }
}

void PrintColour(IntPtr colour,string strName,int depth) {
    string colourDetails = "";
    bool hasFallback = false;
    IntPtr fallback = 0;
    hasFallback = SvgParserNative.svgparser_colour_has_fallback(colour) != 0;
    if(hasFallback) fallback = SvgParserNative.svgparser_colour_get_fallback(colour);
    SvgColourType enumFillColourType = SvgParserNative.GetColourColourType(colour);
    switch(enumFillColourType) {
    case SvgColourType.Cpal:
        int index = SvgParserNative.svgparser_colour_get_colour_index(colour);
        colourDetails = $"{index}";
        break;
    case SvgColourType.Bgr:
    case SvgColourType.BgrA:
        byte[] bgra = new byte[4];
        SvgParserNative.svgparser_colour_get_bgra(colour,bgra);
        colourDetails = String.Join(", ", bgra);
        break;
    case SvgColourType.Hsl:
    case SvgColourType.HslA:
        float[] hsla = new float[4];
        SvgParserNative.svgparser_colour_get_hsla(colour,hsla,hsla.Length);
        colourDetails = String.Join(", ", hsla);
        break;
    }
    Console.WriteLine(new string(' ', depth * 4 + 2) + $"({strName}: {enumFillColourType}{(colourDetails.Length > 1 ? " - " + colourDetails : "")})");
    if(hasFallback) {
        PrintColour(fallback,"=>fallback",depth);
    }
}

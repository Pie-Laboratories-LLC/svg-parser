# INTRODUCTION

This is a simple svg parser which parses the subset of svg primarily used by
OT-SVG fonts, particularly emoji fonts.  The parser is written in native c++;
the nuget package consists of bindings over that native c++.  The project can
be found on
[Github](https://github.com/Pie-Laboratories-LLC/svg-parser/tree/csharp-bindings).
It's part of a larger project whose progress can be tracked on
[Youtube](https://www.youtube.com/@PieLaboratoriesLLC).

The particular svg elements covered consist of:

* `<path>`, `<rect>`, `<group>`, `<use>`, `<ellipse>`, `<circle>`, `<defs>`
  `<linearGradient>`, `<radialGradient>`, `<text>`, `<image>`
* fill and stroke operations and "currentColor"
* limited support for the CSS `var()` primitive in colour attributes, e.g.
  `fill="var(--currentColor,#deadbeef)"`
* optional built-in support for Xerces XML parsing
* support for XML namespaces (optional)
* support for DTD validation (optional)
* bindings for windows, linux, and macOS.

# PARSING

The following comes from a longer example included in
samples/csharp/Round-the-World.  That sample is meant to demonstrate the entire c#
api by exercising each of its access points.  The following excerpt shows how to
create the parser and parse an svg document; dumpEntity shows how to access some
common fields of SvgEntity in the following, as well as how to get the `d`
component of Svg Paths.  They're stored as two arrays, one of `PathMove`s, and
one of points.  One combines the two by examining the PathMove and using the
correct number of points.  Remember: this is the abstract syntax tree, so it
stores the minimal information necessary to construct the path.  For instance,
there are only two points associated with a LineTo, the x and the y.  The source
is held over from the previous PathMove, be it a MoveTo or QuadBézierTo.
QuadBézierTo, then, has 4 points: the control point and the endpoint, where the
start point is the end point of the previous PathMove:

```c#
SvgParser.init();

try {
    SvgParser svgParser = new();
    using(SvgDocument? svgDocument = svgParser.Parse(svgText))
    {
        if (svgDocument == null) throw new Exception("SVG document didn't parse.");
        var root = svgDocument.RootSvg;
        foreach (SvgEntity svgEntity in root.enumerateChildren())
        {
            var messages = dumpEntity(svgDocument, svgEntity);

            Console.WriteLine(string.Join("\n", messages));
        }
    }
}
finally {
    SvgParser.shutdown();
}

List<string> dumpEntity(SvgDocument svgDocument, SvgEntity svgEntity,int indent = 0)
{
    List<string> messages = new();
    messages.Add($"ID: {svgEntity.Id}; Type = {svgEntity.Type}; Render = {svgEntity.Render}");
    float[]? matrix = svgEntity.Matrix;
    if (matrix != null) messages.Add($"Matrix: |{string.Join(" ", matrix)}|");
    messages.Add($"FillColour: {dumpPaint(svgDocument, svgEntity.FillColour)}");
    messages.Add($"FillRule: {svgEntity.FillRule.ToString()}; FillOpacity: {svgEntity.FillOpacity}");
    messages.Add($"StrokeColour: {dumpPaint(svgDocument, svgEntity.StrokeColour)}");
    messages.Add($"StrokeOpacity: {svgEntity.StrokeOpacity}; LineWidth: {svgEntity.StrokeWidth}; LineCap: {svgEntity.LineCap.ToString()}; LineJoin: {svgEntity.LineJoin.ToString()}; MiterLimit: {svgEntity.MiterLimit}; Dash Array: {string.Join(" ",svgEntity.DashArray)}");
    messages.Add($"CssStyle: {svgEntity.CssStyle}");
    messages.Add($"CssClass: {svgEntity.CssClass}");
    
    switch (svgEntity.Type)
    {
        case "Path":
            PieLaboratories.Svg.Path cpath = (PieLaboratories.Svg.Path)svgEntity;
            PathMove[] pathMoves = cpath.PathMoves;
            float[] points = cpath.Points;
            string strMessage = "";
            int nPointIndex = 0;
            for(int i = 0; i < pathMoves.Length; i++) {
                if(strMessage.Length > 0) strMessage += " ";
                strMessage += pathMoves[i].ToString()[0] + " ";
                int nPointCount = 0;
                // note here we don't care about ArcEllipticalTo, or the Smooth Bézs because they get converted to
                //  cubic béz or the appropriate type of béz.
                switch(pathMoves[i]) {
                    case PathMove.MoveTo:
                    case PathMove.LineTo:
                        nPointCount = 2;
                        break;
                    case PathMove.QuadBézierTo:
                        nPointCount = 4;
                        break;
                    case PathMove.CubicBézierTo:
                        nPointCount = 6;
                        break;
                    case PathMove.HorizontalLineTo:
                    case PathMove.VerticalLineTo:
                        nPointCount = 1;
                        break;
                    case PathMove.ClosePath:
                        break;
                    default: throw new Exception($"Unhandled PathMove {pathMoves[i]}");
                }
                float[] thesePoints = new float[nPointCount];
                for(int j = 0; j < nPointCount; j++) thesePoints[j] = points[nPointIndex + j];
                strMessage += string.Join(",", thesePoints);
                nPointIndex += nPointCount;
            }
            messages.Add(strMessage);
            break;

        case "Rect":
            /* Rect, Ellipse, Use, Group, etc. follow suit.
               See Round-the=World/Program.cs for the full listing. */
    }

    /* ... */
}
```

# ERROR REPORTING

SvgParser can silently skip content it doesn't understand - unrecognized
elements, duplicate ids, and similar issues are logged rather than treated
as fatal, so a single unsupported tag doesn't abort parsing an otherwise
valid document. To be notified when this happens, register a callback
before parsing:

**C#:**
```c#
using SvgParser svgParser = new();
svgParser.EnableErrorReporting();
svgParser.OnParseError += (status, msg) => Console.WriteLine($"[{status}] {msg}");
```

Without this, issues are silently ignored, which may result in missing
content with no indication why. Registering the callback is strongly
recommended, especially while developing against untrusted or
hand-authored SVG.

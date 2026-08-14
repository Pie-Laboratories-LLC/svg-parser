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

using PieLaboratories.Svg;

string svgText = """
<svg color="#336699">
  <defs>
    <linearGradient id="baseGradient" x1="0" y1="0" x2="1" y2="0">
      <stop offset="0" stop-color="#000000" stop-opacity="1"/>
      <stop offset="1" stop-color="#ffffff" stop-opacity="0.5"/>
    </linearGradient>

    <linearGradient id="derivedGradient" xlink:href="#baseGradient" x1="0" y1="1" x2="1" y2="0"/>

    <!-- reusable template, referenced below via <use> -->
    <rect id="reusableRect" x="0" y="0" width="10" height="10" rx="2" ry="2" fill="#deadbeef"/>
  </defs>

  <!-- Group: opacity + matrix transform, container for children -->
  <g id="mainGroup" opacity="0.75" transform="matrix(1 0 0 1 13 24)">

    <!-- Path: matrix inherited via group, own dasharray, fill via var() with currentColor fallback -->
    <path id="pathWithEverything"
          d="M0,0 L10,10 C15,15 20,5 25,10 Z"
          fill="var(currentColor,#deadbeef)"
          stroke="#112233"
          stroke-width="2"
          stroke-opacity="0.8"
          stroke-dasharray="1 3 4 2"
          stroke-dashoffset="0.5"
          stroke-linecap="round"
          stroke-linejoin="bevel"
          stroke-miterlimit="8"
          fill-rule="evenodd"/>

    <!-- Rect: dimensioned (x/y/width/height) plus rx/ry, fill via gradient reference -->
    <rect id="rectWithEverything" x="5" y="5" width="40" height="20" rx="4" ry="4" fill="url(#derivedGradient)"/>

    <!-- Circle: cx/cy/r, no stroke (None) -->
    <circle id="circleWithEverything" cx="30" cy="30" r="12" fill="#00ff00"/>

    <!-- Ellipse: cx/cy/rx/ry, both fill and stroke set -->
    <ellipse id="ellipseWithEverything" cx="50" cy="50" rx="20" ry="10" fill="#ff00ff" stroke="#000000" stroke-width="1.5"/>

    <!-- Use: references the reusable rect defined in defs -->
    <use id="useWithEverything" xlink:href="#reusableRect" x="60" y="60"/>

    <!-- Path with no fill/stroke set at all (both default to None) -->
    <path id="pathWithNoPaint" d="M0,0 L5,5"/>

    <!-- Nested group to exercise container-within-container enumeration -->
    <g id="nestedGroup">
      <path id="nestedPath" d="M1,1 L2,2" fill="#123456"/>
    </g>

  </g>
</svg>
""";

SvgParser.init();

try {
    SvgParser svgParser = new();
    using(SvgDocument? svgDocument = svgParser.Parse(svgText))
    {
        if (svgDocument == null) throw new Exception("SVG document didn't parse.");
        var root = svgDocument.getRoot();
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
List<string> dumpEntity(SvgDocument svgDocument, SvgEntity svgEntity)
{
    List<string> messages = new();
    messages.Add($"ID: {svgEntity.Id}; Type = {svgEntity.Type}; Render = {svgEntity.Render}");
    float[]? matrix = svgEntity.Matrix;
    if (matrix != null) messages.Add($"Matrix: |{string.Join(" ", matrix)}|");
    messages.Add($"FillColour: {dumpPaint(svgDocument, svgEntity.FillColour)}");
    messages.Add($"FillRule: {svgEntity.FillRule.ToString()}; FillOpacity: {svgEntity.FillOpacity}");
    messages.Add($"StrokeColour: {dumpPaint(svgDocument, svgEntity.StrokeColour)}");
    messages.Add($"StrokeOpacity: {svgEntity.StrokeOpacity}; LineWidth: {svgEntity.StrokeWidth}; LineCap: {svgEntity.LineCap.ToString()}; LineJoin: {svgEntity.LineJoin.ToString()}; MiterLimit: {svgEntity.MiterLimit}; Dash Array: {{string.Join(\" \",svgEntity.DashArray)}}");
    messages.Add($"CssStyle: {svgEntity.CssStyle}");
    messages.Add($"CssStyle: {svgEntity.CssStyle}");
    
    switch (svgEntity.Type)
    {
        case "Rect":
            string strMessage = dumpDimensionedEntity(svgDocument, (Rect)svgEntity);
            Dimension? rx = ((Rect)svgEntity).Rx;
            Dimension? ry = ((Rect)svgEntity).Ry;
            strMessage += "; ";
            if (rx != null) strMessage += $"{rx.Value.Value} {rx.Value.Units}";
            else strMessage += "auto";
            strMessage += " x ";
            if (rx != null) strMessage += $"{rx.Value.Value} {rx.Value.Units}";
            else strMessage += "auto";
            messages.Add(strMessage);
            break;
        case "Ellipse":
            strMessage = "";
            Dimension? cx = ((Ellipse)svgEntity).Cx;
            Dimension? cy = ((Ellipse)svgEntity).Cy;
            rx = ((Ellipse)svgEntity).Rx;
            ry = ((Ellipse)svgEntity).Ry;
            if (cx != null) strMessage += $"{cx.Value.Value} {cx.Value.Units}";
            else strMessage += "auto";
            strMessage += " x ";
            if (cy != null) strMessage += $"{cy.Value.Value} {cy.Value.Units}";
            else strMessage += "auto";
            if (rx != null) strMessage += $"{rx.Value.Value} {rx.Value.Units}";
            else strMessage += "auto";
            strMessage += " x ";
            if (ry != null) strMessage += $"{ry.Value.Value} {ry.Value.Units}";
            else strMessage += "auto";
            messages.Add(strMessage);
            break;
        
        case "Circle":
            strMessage = "";
            cx = ((Circle)svgEntity).Cx;
            cy = ((Circle)svgEntity).Cy;
            Dimension? r = ((Circle)svgEntity).R;
            if (cx != null) strMessage += $"{cx.Value.Value} {cx.Value.Units}";
            else strMessage += "auto";
            strMessage += " x ";
            if (cy != null) strMessage += $"{cy.Value.Value} {cy.Value.Units}";
            else strMessage += "auto";
            if (r != null) strMessage += $"{r.Value.Value} {r.Value.Units}";
            else strMessage += "auto";
            messages.Add(strMessage);
            break;
        
        case "Svg":
            strMessage = dumpDimensionedEntity(svgDocument, (Svg)svgEntity);
            messages.Add(strMessage);
            break;
        
        // de rien
        case "Group": break;
        
        case "Use":
            break;

        default: throw new Exception($"Unknown SVG entity type {svgEntity.Type.ToString()}");
    }

    ISvgContainerEntity containerEntity = (ISvgContainerEntity)(svgEntity);
    if (containerEntity != null)
    {
        foreach (SvgEntity child in containerEntity.enumerateChildren())
        {
            List<string> childMessages = dumpEntity(svgDocument, child);
            messages.AddRange(childMessages);
        }
    }

    return messages;
}

string dumpDimensionedEntity(SvgDocument svgDocument, ISvgDimensionedEntity svgEntity)
{
    Dimension? x = svgEntity.X;
    Dimension? y = svgEntity.Y;
    Dimension? width = svgEntity.Width;
    Dimension? height = svgEntity.Height;
    string result = "";
    if (x != null) result += $"{x.Value.Value} {x.Value.Units}";
    else result += "auto";
    result += " x ";
    if (y != null) result += $"{y.Value.Value} {y.Value.Units}";
    else result += "auto";
    result += "; ";
    if (width != null) result += $"{width.Value.Value} {width.Value.Units}";
    else result += "auto";
    result += " x ";
    if (height != null) result += $"{height.Value.Value} {height.Value.Units}";
    else result += "auto";
    return result;
}

string dumpPaint(SvgDocument svgDocument, SvgPaint svgPaint)
{
    string result = svgPaint.SvgColourType.ToString();
    result += ": ";
    if (!string.IsNullOrEmpty(svgPaint.GradientId)) {
        result += $"Gradient Id: {svgPaint.GradientId}";
        result += "[" + dumpGradient(svgDocument, svgPaint.GradientId) + "]";
    }
    else
    {
        return dumpColour(svgDocument, svgPaint.SvgColour);
    }

    return result;
}

string dumpColour(SvgDocument svgDocument, SvgColour svgColour)
{
    string result = svgColour.SvgColourType.ToString() + ": ";
    switch (svgColour.SvgColourType)
    {
        case SvgColourType.CurrentColor: return "CurrentColor";
        case SvgColourType.None: return "None";
        case SvgColourType.Bgr:
        case SvgColourType.BgrA:
            byte[] bgra = svgColour.Bgra;
            result += ": BGRA (";
            result += string.Join(",", bgra) + ")";
            break;
        case SvgColourType.Hsl:
        case SvgColourType.HslA:
            float[] hsla = svgColour.Hsla;
            result += ": HSLA (";
            result += string.Join(",", hsla) + ")";
            break;
        default: throw new Exception($"Unhandled SvgColourType: {svgColour.SvgColourType}");
    }
    return result;
}

string dumpGradient(SvgDocument svgDocument, string gradientId)
{
    GradientTemplate? gradient = svgDocument.lookupGradient(gradientId);
    if (gradient == null) return $"failed to load gradient {gradientId}";
    GradientKind kind = gradient.GradientKind;
    string result = kind.ToString();
    result += "; ";
    float[] geometry = gradient.Geometry;
    result += "[";
    if (kind == GradientKind.Linear)
    {
        result += "x1: " + geometry[0] + "; y1: " + geometry[1] + "; x2: " + geometry[2] + "; y2: " + geometry[3];
    }
    else
    {
        result += "cx: " + geometry[0] + "; cy: " + geometry[1] + "; r: " + geometry[2] + "; fx: " + geometry[3] + "; fy: " + geometry[4] + "; fr: " + geometry[5];
    }
    result += "] ";
    result += gradient.GradientUnits.ToString();
    result += "; ";
    result += gradient.SpreadMethod.ToString();
    
    float[]? transform = gradient.Transform;

    if (transform != null)
    {
        result += " |";
        result += string.Join(" ", transform);
        result += "| ";
    }

    result += gradient.HrefId;

    return result;
}

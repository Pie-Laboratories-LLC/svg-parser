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

using System;
using System.Runtime.InteropServices;
using System.Text;
using System.Collections.Generic;

internal static class SvgParserNative {
    private const string LibName = "svgparser_c";

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_init();
    
    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void svgparser_shutdown();

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void SvgEntityCallback(IntPtr entity, IntPtr userData);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    public static extern IntPtr svgparser_parse(string svgText);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void svgparser_document_free(IntPtr doc);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr svgparser_document_get_root(IntPtr doc);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr svgparser_document_get_style(IntPtr doc);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    public static extern IntPtr svgparser_document_lookup_svg_entity(IntPtr doc, string id);

    // --- SvgDocument -> Gradient lookup ---
    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr svgparser_document_lookup_gradient(IntPtr doc, string id);

    // --- GradientTemplate accessors ---
    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr svgparser_gradient_get_id(IntPtr gradient);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int svgparser_gradient_get_kind(IntPtr gradient);

    public static GradientKind GetGradientKind(IntPtr gradient) =>
        (GradientKind)svgparser_gradient_get_kind(gradient);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_gradient_get_geometry(IntPtr gradient, float[] outBuffer6, int bufferSize);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int svgparser_gradient_get_units(IntPtr gradient);

    public static GradientUnits GetGradientUnits(IntPtr gradient) =>
        (GradientUnits)svgparser_gradient_get_units(gradient);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int svgparser_gradient_get_spread_method(IntPtr gradient);

    public static SpreadMethod GetGradientSpreadMethod(IntPtr gradient) =>
        (SpreadMethod)svgparser_gradient_get_spread_method(gradient);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_gradient_get_transform(IntPtr gradient, float[] outBuffer6, int bufferSize);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr svgparser_gradient_get_href_id(IntPtr gradient);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_gradient_get_stop_count(IntPtr gradient);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr svgparser_gradient_get_stop(IntPtr gradient, int index);

    // --- Stop accessors ---
    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern float svgparser_stop_get_offset(IntPtr stop);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern float svgparser_stop_get_opacity(IntPtr stop);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr svgparser_stop_get_colour(IntPtr stop);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr svgparser_entity_get_type(IntPtr entity);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr svgparser_entity_get_id(IntPtr entity);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_entity_get_dash_array_count(IntPtr entity);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void svgparser_entity_get_dash_array(IntPtr entity, float[] buffer, int bufferSize);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_entity_get_matrix(IntPtr entity, float[] outBuffer6);


    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_entity_get_render(IntPtr entity);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern float svgparser_entity_get_fill_opacity(IntPtr entity);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int svgparser_entity_get_fill_rule(IntPtr entity);

    public static FillRule GetFillRule(IntPtr paint) =>
        (FillRule)svgparser_entity_get_fill_rule(paint);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern float svgparser_entity_get_stroke_opacity(IntPtr entity);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern float svgparser_entity_get_stroke_width(IntPtr entity);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int svgparser_entity_get_line_cap(IntPtr entity);

    public static LineCap GetLineCap(IntPtr paint) =>
        (LineCap)svgparser_entity_get_line_cap(paint);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int svgparser_entity_get_line_join(IntPtr entity);

    public static LineJoin GetLineJoin(IntPtr paint) =>
        (LineJoin)svgparser_entity_get_line_join(paint);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern float svgparser_entity_get_miter_limit(IntPtr entity);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr svgparser_entity_get_css_class(IntPtr entity);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr svgparser_entity_get_css_style(IntPtr entity);


    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr svgparser_entity_get_fill(IntPtr entity);   // returns SvgPaint*

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr svgparser_entity_get_stroke(IntPtr entity); // returns SvgPaint*

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int svgparser_paint_get_colour_type(IntPtr paint);

    public static SvgColourType GetPaintColourType(IntPtr paint) =>
        (SvgColourType)svgparser_paint_get_colour_type(paint);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr svgparser_paint_get_gradient_id(IntPtr paint);   // only valid if type == Gradient

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr svgparser_paint_get_colour(IntPtr paint);       // returns SvgColour*, null if type == Gradient

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_paint_has_fallback(IntPtr paint);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr svgparser_paint_get_fallback(IntPtr paint);     // returns SvgColour*, null if none

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int svgparser_colour_get_colour_type(IntPtr colour);

    public static SvgColourType GetColourColourType(IntPtr paint) =>
        (SvgColourType)svgparser_colour_get_colour_type(paint);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_colour_get_colour_index(IntPtr colour);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_colour_get_bgra(IntPtr colour, byte []outBuffer4); // 1 if present+valid

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_colour_get_hsla(IntPtr colour, float []outBuffer4, int bufferSize);          // 1 if present+valid

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_colour_has_fallback(IntPtr colour);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr svgparser_colour_get_fallback(IntPtr colour); // returns SvgColour*, null if none

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void svgparser_entity_enumerate_children(IntPtr entity, SvgEntityCallback callback, IntPtr userData);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr svgparser_entity_get_dimensioned(IntPtr entity); // does implement SvgDimensionedEntity?

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_dimensionedentity_has_x(IntPtr dimensionedEntity); // does implement SvgDimensionedEntity?

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern float svgparser_dimensionedentity_get_x_measurement(IntPtr dimensionedEntity); // does implement SvgDimensionedEntity?

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int svgparser_dimensionedentity_get_x_units(IntPtr dimensionedEntity); // does implement SvgDimensionedEntity?

    public static DimensionUnits GetDimensionedXUnits(IntPtr paint) =>
        (DimensionUnits)svgparser_dimensionedentity_get_x_units(paint);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_dimensionedentity_has_y(IntPtr dimensionedEntity); // does implement SvgDimensionedEntity?

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern float svgparser_dimensionedentity_get_y_measurement(IntPtr dimensionedEntity); // does implement SvgDimensionedEntity?

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int svgparser_dimensionedentity_get_y_units(IntPtr dimensionedEntity); // does implement SvgDimensionedEntity?

    public static DimensionUnits GetDimensionedYUnits(IntPtr paint) =>
        (DimensionUnits)svgparser_dimensionedentity_get_y_units(paint);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_dimensionedentity_has_width(IntPtr dimensionedEntity); // does implement SvgDimensionedEntity?

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern float svgparser_dimensionedentity_get_width_measurement(IntPtr dimensionedEntity); // does implement SvgDimensionedEntity?

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int svgparser_dimensionedentity_get_width_units(IntPtr dimensionedEntity); // does implement SvgDimensionedEntity?

    public static DimensionUnits GetDimensionedWidthUnits(IntPtr paint) =>
        (DimensionUnits)svgparser_dimensionedentity_get_width_units(paint);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_dimensionedentity_has_height(IntPtr dimensionedEntity); // does implement SvgDimensionedEntity?

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern float svgparser_dimensionedentity_get_height_measurement(IntPtr dimensionedEntity); // does implement SvgDimensionedEntity?

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int svgparser_dimensionedentity_get_height_units(IntPtr dimensionedEntity); // does implement SvgDimensionedEntity?

    public static DimensionUnits GetDimensionedHeightUnits(IntPtr paint) =>
        (DimensionUnits)svgparser_dimensionedentity_get_height_units(paint);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr svgparser_entity_get_container(IntPtr entity); // does implement SvgContainerEntity?

    // --- Path ---
    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_path_get_points_count(IntPtr path);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void svgparser_path_get_points(IntPtr path, float[] buffer, int bufferSize);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_path_get_moves_count(IntPtr path);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void svgparser_path_get_moves(IntPtr path, int[] buffer, int bufferSize);

    public static PathMove[] GetPathMoves(IntPtr path) {
        int count = svgparser_path_get_moves_count(path);
        int[] buffer = new int[count];
        svgparser_path_get_moves(path, buffer, count);
        PathMove[] toReturn = new PathMove[count];
        int index = 0;
        foreach(int pathMove in buffer) {
            toReturn[index++] = (PathMove)pathMove;
        }
        return toReturn;
    }

    // --- Group ---
    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern float svgparser_group_get_opacity(IntPtr group);

    // --- Rect ---
    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_rect_has_rx(IntPtr rect);
    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern float svgparser_rect_get_rx_measurement(IntPtr rect);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int svgparser_rect_get_rx_units(IntPtr rect);

    public static DimensionUnits GetRectRxUnits(IntPtr paint) =>
        (DimensionUnits)svgparser_rect_get_rx_units(paint);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_rect_has_ry(IntPtr rect);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern float svgparser_rect_get_ry_measurement(IntPtr rect);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int svgparser_rect_get_ry_units(IntPtr rect);

    public static DimensionUnits GetRectRyUnits(IntPtr rect) =>
        (DimensionUnits)svgparser_rect_get_ry_units(rect);

    // --- Circle ---
    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_circle_has_cx(IntPtr circle);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern float svgparser_circle_get_cx_measurement(IntPtr circle);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int svgparser_circle_get_cx_units(IntPtr circle);

    public static DimensionUnits GetCircleCxUnits(IntPtr circle) =>
        (DimensionUnits)svgparser_circle_get_cx_units(circle);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_circle_has_cy(IntPtr circle);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern float svgparser_circle_get_cy_measurement(IntPtr circle);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int svgparser_circle_get_cy_units(IntPtr circle);

    public static DimensionUnits GetCircleCyUnits(IntPtr circle) =>
        (DimensionUnits)svgparser_circle_get_cy_units(circle);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_circle_has_r(IntPtr circle);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern float svgparser_circle_get_r_measurement(IntPtr circle);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int svgparser_circle_get_r_units(IntPtr circle);

    public static DimensionUnits GetCircleRUnits(IntPtr circle) =>
        (DimensionUnits)svgparser_circle_get_r_units(circle);


    // --- Ellipse ---
    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_ellipse_has_cx(IntPtr ellipse);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern float svgparser_ellipse_get_cx_measurement(IntPtr ellipse);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int svgparser_ellipse_get_cx_units(IntPtr ellipse);

    public static DimensionUnits GetEllipseCxUnits(IntPtr ellipse) =>
        (DimensionUnits)svgparser_ellipse_get_cx_units(ellipse);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_ellipse_has_cy(IntPtr ellipse);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern float svgparser_ellipse_get_cy_measurement(IntPtr ellipse);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int svgparser_ellipse_get_cy_units(IntPtr ellipse);

    public static DimensionUnits GetEllipseCyUnits(IntPtr ellipse) =>
        (DimensionUnits)svgparser_ellipse_get_cy_units(ellipse);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_ellipse_has_rx(IntPtr ellipse);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern float svgparser_ellipse_get_rx_measurement(IntPtr ellipse);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int svgparser_ellipse_get_rx_units(IntPtr ellipse);

    public static DimensionUnits GetEllipseRxUnits(IntPtr ellipse) =>
        (DimensionUnits)svgparser_ellipse_get_rx_units(ellipse);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_ellipse_has_ry(IntPtr ellipse);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern float svgparser_ellipse_get_ry_measurement(IntPtr ellipse);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int svgparser_ellipse_get_ry_units(IntPtr ellipse);

    public static DimensionUnits GetEllipseRyUnits(IntPtr ellipse) =>
        (DimensionUnits)svgparser_ellipse_get_ry_units(ellipse);

    // helper: marshal a returned const char* into a managed string
    public static string PtrToString(IntPtr ptr) =>
        ptr == IntPtr.Zero ? string.Empty : Marshal.PtrToStringAnsi(ptr);

    // helper: drive the callback-based enumeration into a managed List<IntPtr>
    public static List<IntPtr> GetChildren(IntPtr entity) {
        var children = new List<IntPtr>();
        var handle = GCHandle.Alloc(children);
        try {
            SvgEntityCallback cb = (childPtr, userData) => {
                var list = (List<IntPtr>)GCHandle.FromIntPtr(userData).Target;
                list.Add(childPtr);
            };
            svgparser_entity_enumerate_children(entity, cb, GCHandle.ToIntPtr(handle));
        } finally {
            handle.Free();
        }
        return children;
    }
}

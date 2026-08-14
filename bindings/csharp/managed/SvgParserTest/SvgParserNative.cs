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
    public static extern IntPtr svgparser_entity_get_tag_name(IntPtr entity);

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
    public static extern int svgparser_entity_get_stroke_width(IntPtr entity);

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

    // helper: marshal a returned const char* into a managed string
    public static string PtrToString(IntPtr ptr) =>
        ptr == IntPtr.Zero ? null : Marshal.PtrToStringAnsi(ptr);

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

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
    public static extern IntPtr svgparser_document_get_element_by_id(IntPtr doc, string id);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr svgparser_entity_get_tag_name(IntPtr entity);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr svgparser_entity_get_id(IntPtr entity);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr svgparser_entity_get_id(IntPtr entity);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_entity_get_dash_array_count(IntPtr entity);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void svgparser_entity_get_dash_array(IntPtr entity, float[] buffer, int bufferSize);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int svgparser_entity_get_matrix(IntPtr entity, float[] outBuffer6);

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

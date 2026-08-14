#pragma once
// yes I do know #pragma once :P

#ifdef _WIN32
  #define SVGPARSER_API extern "C" __declspec(dllexport)
#else
  #define SVGPARSER_API extern "C"
#endif

// Callback signature: cdecl, receives a non-owning entity handle + the userData token
typedef void (*SvgEntityCallback)(const void* entity, void* userData);

// --- xerces initialization ---
SVGPARSER_API int  svgparser_init(void);   // returns 1 on success, 0 on failure
SVGPARSER_API void svgparser_shutdown(void);

// --- Document lifecycle ---
SVGPARSER_API void* svgparser_parse(const char* svgText);
SVGPARSER_API void  svgparser_document_free(void* doc);

// --- Document -> tree access ---
SVGPARSER_API const void* svgparser_document_get_root(const void* doc);
SVGPARSER_API const void* svgparser_document_lookup_svg_entity(const void* doc, const char* id);
//
// --- SvgDocument -> Gradient lookup ---
SVGPARSER_API const void* svgparser_document_lookup_gradient(const void* doc, const char* id); // returns GradientTemplate*, null if not found

// --- GradientTemplate accessors ---
SVGPARSER_API const char* svgparser_gradient_get_id(const void* gradient);
SVGPARSER_API int         svgparser_gradient_get_kind(const void* gradient);           // 0=linear, 1=radial
SVGPARSER_API int         svgparser_gradient_get_geometry(const void* gradient, float* outBuffer6, int bufferSize);
SVGPARSER_API int         svgparser_gradient_get_units(const void* gradient);          // GradientUnits as int
SVGPARSER_API int         svgparser_gradient_get_spread_method(const void* gradient);  // SpreadMethod as int
SVGPARSER_API int         svgparser_gradient_get_transform(const void* gradient, float* outBuffer6, int bufferSize);
SVGPARSER_API const char* svgparser_gradient_get_href_id(const void* gradient);

SVGPARSER_API int         svgparser_gradient_get_stop_count(const void* gradient);
SVGPARSER_API const void* svgparser_gradient_get_stop(const void* gradient, int index); // returns const Stop<SvgColour>*, null if out of range

// --- Stop accessors ---
SVGPARSER_API float       svgparser_stop_get_offset(const void* stop);
SVGPARSER_API float       svgparser_stop_get_opacity(const void* stop);
SVGPARSER_API const void* svgparser_stop_get_colour(const void* stop); // returns const SvgColour*, reuses existing svgparser_colour_* accessors

// --- Entity accessors ---
SVGPARSER_API const char* svgparser_entity_get_tag_name(const void* entity);
SVGPARSER_API const char* svgparser_entity_get_id(const void* entity);
SVGPARSER_API int  svgparser_entity_get_dash_array_count(const void* entity);
SVGPARSER_API void svgparser_entity_get_dash_array(const void* entity, float* buffer, int bufferSize);
SVGPARSER_API int svgparser_entity_get_matrix(const void* entity, float* outBuffer6); // returns 1 if present, 0 if not
SVGPARSER_API int svgparser_entity_get_render(const void* entity); // returns 1 if true, 0 if not
SVGPARSER_API float svgparser_entity_get_fill_opacity(const void* entity);
SVGPARSER_API int svgparser_entity_get_fill_rule(const void* entity);
SVGPARSER_API float svgparser_entity_get_stroke_opacity(const void* entity);
SVGPARSER_API int svgparser_entity_get_stroke_width(const void* entity);
SVGPARSER_API int svgparser_entity_get_line_cap(const void* entity);
SVGPARSER_API int svgparser_entity_get_line_join(const void* entity);
SVGPARSER_API float svgparser_entity_get_miter_limit(const void* entity);
SVGPARSER_API const char *svgparser_entity_get_css_class(const void* entity);
SVGPARSER_API const char *svgparser_entity_get_css_style(const void* entity);

// --- SvgPaint accessors ---
SVGPARSER_API const void* svgparser_entity_get_fill(const void* entity);   // returns SvgPaint*
SVGPARSER_API const void* svgparser_entity_get_stroke(const void* entity); // returns SvgPaint*

SVGPARSER_API int         svgparser_paint_get_colour_type(const void* paint);
SVGPARSER_API const char* svgparser_paint_get_gradient_id(const void* paint);   // only valid if type == Gradient
SVGPARSER_API const void* svgparser_paint_get_colour(const void* paint);       // returns SvgColour*, null if type == Gradient
SVGPARSER_API int         svgparser_paint_has_fallback(const void* paint);
SVGPARSER_API const void* svgparser_paint_get_fallback(const void* paint);     // returns SvgColour*, null if none

// --- SvgColour accessors ---
SVGPARSER_API int    svgparser_colour_get_colour_type(const void* colour);
SVGPARSER_API int    svgparser_colour_get_colour_index(const void* colour);
SVGPARSER_API int    svgparser_colour_get_bgra(const void* colour, unsigned char* outBuffer4); // 1 if present+valid
SVGPARSER_API int    svgparser_colour_get_hsla(const void* colour, float* outBuffer4, int bufferSize);          // 1 if present+valid
SVGPARSER_API int    svgparser_colour_has_fallback(const void* colour);
SVGPARSER_API const void* svgparser_colour_get_fallback(const void* colour); // returns SvgColour*, null if none

// --- Enumeration (callback-based) ---
SVGPARSER_API void svgparser_entity_enumerate_children(const void* entity, SvgEntityCallback callback, void* userData);

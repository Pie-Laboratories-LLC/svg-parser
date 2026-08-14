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
SVGPARSER_API const char* svgparser_document_get_style(const void* doc);
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
SVGPARSER_API const char* svgparser_entity_get_type(const void* entity);
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

// --- SvgDimensionedEntity accessors ---
SVGPARSER_API void* svgparser_entity_get_dimensioned(const void* entity);             // does implement SvgDimensionedEntity? 
SVGPARSER_API int   svgparser_dimensionedentity_has_x(const void* dimensionedEntity); // is x dimension set
SVGPARSER_API float svgparser_dimensionedentity_get_x_measurement(const void* dimensionedEntity); // get x' measurement
SVGPARSER_API int   svgparser_dimensionedentity_get_x_units(const void* dimensionedEntity); // get x' units
SVGPARSER_API int   svgparser_dimensionedentity_has_y(const void* dimensionedEntity);
SVGPARSER_API float svgparser_dimensionedentity_get_y_measurement(const void* dimensionedEntity);
SVGPARSER_API int   svgparser_dimensionedentity_get_y_units(const void* dimensionedEntity);
SVGPARSER_API int   svgparser_dimensionedentity_has_width(const void* dimensionedEntity);
SVGPARSER_API float svgparser_dimensionedentity_get_width_measurement(const void* dimensionedEntity);
SVGPARSER_API int   svgparser_dimensionedentity_get_width_units(const void* dimensionedEntity);
SVGPARSER_API int   svgparser_dimensionedentity_has_height(const void* dimensionedEntity);
SVGPARSER_API float svgparser_dimensionedentity_get_height_measurement(const void* dimensionedEntity);
SVGPARSER_API int   svgparser_dimensionedentity_get_height_units(const void* dimensionedEntity);

// --- SvgContainerEntity ---
SVGPARSER_API void*       svgparser_entity_get_container(const void* entity);

// --- Path ---
SVGPARSER_API int  svgparser_path_get_points_count(const void* path);
SVGPARSER_API void svgparser_path_get_points(const void* path, float* buffer, int bufferSize);
SVGPARSER_API int  svgparser_path_get_moves_count(const void* path);
SVGPARSER_API void svgparser_path_get_moves(const void* path, int* buffer, int bufferSize); // PathMove enum values as int

// --- Group ---
SVGPARSER_API float svgparser_group_get_opacity(const void* group);

// --- Rect ---
SVGPARSER_API int   svgparser_rect_has_rx(const void* rect);
SVGPARSER_API float svgparser_rect_get_rx_measurement(const void* rect);
SVGPARSER_API int   svgparser_rect_get_rx_units(const void* rect);
SVGPARSER_API int   svgparser_rect_has_ry(const void* rect);
SVGPARSER_API float svgparser_rect_get_ry_measurement(const void* rect);
SVGPARSER_API int   svgparser_rect_get_ry_units(const void* rect);

// --- Circle ---
SVGPARSER_API int   svgparser_circle_has_cx(const void* circle);
SVGPARSER_API float svgparser_circle_get_cx_measurement(const void* circle);
SVGPARSER_API int   svgparser_circle_get_cx_units(const void* circle);
SVGPARSER_API int   svgparser_circle_has_cy(const void* circle);
SVGPARSER_API float svgparser_circle_get_cy_measurement(const void* circle);
SVGPARSER_API int   svgparser_circle_get_cy_units(const void* circle);
SVGPARSER_API int   svgparser_circle_has_r(const void* circle);
SVGPARSER_API float svgparser_circle_get_r_measurement(const void* circle);
SVGPARSER_API int   svgparser_circle_get_r_units(const void* circle);

// --- Ellipse ---
SVGPARSER_API int   svgparser_ellipse_has_cx(const void* ellipse);
SVGPARSER_API float svgparser_ellipse_get_cx_measurement(const void* ellipse);
SVGPARSER_API int   svgparser_ellipse_get_cx_units(const void* ellipse);
SVGPARSER_API int   svgparser_ellipse_has_cy(const void* ellipse);
SVGPARSER_API float svgparser_ellipse_get_cy_measurement(const void* ellipse);
SVGPARSER_API int   svgparser_ellipse_get_cy_units(const void* ellipse);
SVGPARSER_API int   svgparser_ellipse_has_rx(const void* ellipse);
SVGPARSER_API float svgparser_ellipse_get_rx_measurement(const void* ellipse);
SVGPARSER_API int   svgparser_ellipse_get_rx_units(const void* ellipse);
SVGPARSER_API int   svgparser_ellipse_has_ry(const void* ellipse);
SVGPARSER_API float svgparser_ellipse_get_ry_measurement(const void* ellipse);
SVGPARSER_API int   svgparser_ellipse_get_ry_units(const void* ellipse);

// --- Enumeration (callback-based) ---
SVGPARSER_API void svgparser_entity_enumerate_children(const void* entity, SvgEntityCallback callback, void* userData);

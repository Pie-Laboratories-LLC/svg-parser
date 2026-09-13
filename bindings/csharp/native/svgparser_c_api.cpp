#include <xercesc/util/PlatformUtils.hpp>
#include "svgparser_c_api.h"
#include "svg/SvgParser.hpp"
#include "svg/SvgDocument.hpp"
#include "svg/SvgContainerEntity.hpp"
#include "svg/SvgDimensionedEntity.hpp"
#include "svg/Rect.hpp"
#include "svg/Group.hpp"
#include "svg/Path.hpp"
#include "svg/Circle.hpp"
#include "svg/Ellipse.hpp"
#include "svg/Image.hpp"
#include "svg/Text.hpp"
#include "svg/Svg.hpp"
#include "core/Logger.hpp"

using namespace Draw2d::Svg;
using namespace Draw2d;

XERCES_CPP_NAMESPACE_USE

int svgparser_init(void) {
    try {
        XMLPlatformUtils::Initialize();
        Core::Logger::Instance(std::unordered_set<const char *,CStrHash,CStrEq> { }, Core::Error);
        return 1;
    } catch (...) {
        return 0;
    }
}

void svgparser_shutdown(void) {
    try {
        XMLPlatformUtils::Terminate();
    } catch (...) {
    }
}

void* svgparser_parse(const char* svgText) {
    try {
        SvgParser parser{};
        std::unique_ptr<SvgDocument> doc = parser.parse(svgText);
        return doc.release();
    } catch (const std::exception& e) {
        fprintf(stderr, "svgparser_parse failed: %s\n", e.what());
        return nullptr;
    } catch (...) {
        fprintf(stderr, "svgparser_parse failed: unknown exception\n");
        return nullptr;
    }
}

void svgparser_document_free(void* doc) {
    delete static_cast<SvgDocument*>(doc); // reclaims what .release() gave up
}

void* svgparser_parser_create() {
    try {
        return new SvgParser();
    } catch (...) { return nullptr; }
}

void svgparser_parser_free(void* parser) {
    delete static_cast<SvgParser*>(parser);
}

void* svgparser_parser_parse(void* parser, const char* svgText) {
    try {
        std::unique_ptr<SvgDocument> doc = static_cast<SvgParser*>(parser)->parse(svgText);
        return doc.release();
    } catch (...) { return nullptr; }
}

void svgparser_set_parser_callback(void* parser, SvgParserErrorCallback callback, void* userData) {
    try {
        static_cast<SvgParser*>(parser)->setParserCallback(
            [callback, userData](SvgParserStatus status, const Core::String& msg) {
                callback(static_cast<int>(status), msg.c_str(), userData);
            }
        );
    } catch (...) {}
}

const void* svgparser_document_get_root(const void* doc) {
    try {
        return static_cast<const SvgDocument*>(doc)->getRootSvg();
    } catch (...) {
        return nullptr;
    }
}

const char* svgparser_document_get_style(const void* doc) {
    try {
        return static_cast<const SvgDocument*>(doc)->getStyle().c_str();
    } catch (...) {
        return nullptr;
    }
}

const void* svgparser_document_lookup_svg_entity(const void* doc, const char* id) {
    try {
        // NOTE: adjust if IDomDocument::getElementById lives elsewhere in your real API �
        // this assumes SvgDocument exposes an equivalent lookup for SvgEntity, not IDomEntity.
        return static_cast<const SvgDocument*>(doc)->lookupSvgEntity(id);
    } catch (...) {
        return nullptr;
    }
}

const void* svgparser_document_lookup_gradient(const void* doc, const char* id) {
    try {
        return static_cast<const SvgDocument*>(doc)->lookupGradient(id); // adjust to real method name/signature
    } catch (...) { return nullptr; }
}

const char* svgparser_gradient_get_id(const void* gradient) {
    try {
        return static_cast<const GradientTemplate*>(gradient)->strId.c_str();
    } catch (...) { return nullptr; }
}

int svgparser_gradient_get_kind(const void* gradient) {
    try {
        return static_cast<int>(static_cast<const GradientTemplate*>(gradient)->kind);
    } catch (...) { return -1; }
}

int svgparser_gradient_get_geometry(const void* gradient, float* outBuffer6, int bufferSize) {
    try {
        const auto& geo = static_cast<const GradientTemplate*>(gradient)->geometry;
        int n = std::min<int>(bufferSize, static_cast<int>(geo.size()));
        std::copy(geo.begin(), geo.begin() + n, outBuffer6);
        return n;
    } catch (...) { return 0; }
}

int svgparser_gradient_get_units(const void* gradient) {
    try {
        return static_cast<int>(static_cast<const GradientTemplate*>(gradient)->enumGradientUnits);
    } catch (...) { return -1; }
}

int svgparser_gradient_get_spread_method(const void* gradient) {
    try {
        return static_cast<int>(static_cast<const GradientTemplate*>(gradient)->enumSpreadMethod);
    } catch (...) { return -1; }
}

int svgparser_gradient_get_transform(const void* gradient, float* outBuffer6, int bufferSize) {
    try {
        const auto& tf = static_cast<const GradientTemplate*>(gradient)->transform;
        if(!tf.has_value()) return 0;
        int n = std::min<int>(bufferSize, static_cast<int>(tf.value().size()));
        std::copy(tf.value().begin(), tf.value().begin() + n, outBuffer6);
        return 1;
    } catch (...) { return 0; }
}

const char* svgparser_gradient_get_href_id(const void* gradient) {
    try {
        return static_cast<const GradientTemplate*>(gradient)->strHrefId.c_str();
    } catch (...) { return nullptr; }
}

int svgparser_gradient_get_stop_count(const void* gradient) {
    try {
        return static_cast<int>(static_cast<const GradientTemplate*>(gradient)->stops.size());
    } catch (...) { return 0; }
}

const void* svgparser_gradient_get_stop(const void* gradient, int index) {
    try {
        const auto& stops = static_cast<const GradientTemplate*>(gradient)->stops;
        if (index < 0 || static_cast<size_t>(index) >= stops.size()) return nullptr;
        return &stops[index];
    } catch (...) { return nullptr; }
}

float svgparser_stop_get_offset(const void* stop) {
    try {
        return static_cast<const Stop<SvgColour>*>(stop)->getOffset();
    } catch (...) { return 0.0f; }
}

float svgparser_stop_get_opacity(const void* stop) {
    try {
        return static_cast<const Stop<SvgColour>*>(stop)->getOpacity();
    } catch (...) { return 1.0f; }
}

const void* svgparser_stop_get_colour(const void* stop) {
    try {
        return &static_cast<const Stop<SvgColour>*>(stop)->getColour();
    } catch (...) { return nullptr; }
}

const char* svgparser_entity_get_type(const void* entity) {
    try {
        return static_cast<const SvgEntity*>(entity)->getType();
    } catch (...) {
        return nullptr;
    }
}

const char* svgparser_entity_get_id(const void* entity) {
    try {
        return static_cast<const SvgEntity*>(entity)->getId().c_str();
    } catch (...) {
        return nullptr;
    }
}

int svgparser_entity_get_dash_array_count(const void* entity) {
    try {
        return static_cast<int>(static_cast<const SvgEntity*>(entity)->getDashArray().size());
    } catch (...) { return 0; }
}

void svgparser_entity_get_dash_array(const void* entity, float* buffer, int bufferSize) {
    try {
        const auto& arr = static_cast<const SvgEntity*>(entity)->getDashArray();
        int n = std::min<int>(bufferSize, static_cast<int>(arr.size()));
        std::copy(arr.begin(), arr.begin() + n, buffer);
    } catch (...) {}
}

int svgparser_entity_get_matrix(const void* entity, float* outBuffer6) {
    try {
        const auto& m = static_cast<const SvgEntity*>(entity)->getMatrix();
        if (!m.has_value()) return 0;
        std::copy(m->begin(), m->end(), outBuffer6);
        return 1;
    } catch (...) { return 0; }
}

int svgparser_entity_get_render(const void* entity)
{
    try {
        return static_cast<int>(static_cast<const SvgEntity*>(entity)->getRender()) ? 1 : 0;
    } catch (...) { return 0; }
}
float svgparser_entity_get_fill_opacity(const void* entity)
{
    try {
        return static_cast<const SvgEntity*>(entity)->getFillOpacity();
    } catch (...) { return 0; }
}

int svgparser_entity_get_fill_rule(const void* entity)
{
    try {
        return static_cast<int>(static_cast<const SvgEntity*>(entity)->getFillRule());
    } catch (...) { return 0; }
}

float svgparser_entity_get_stroke_opacity(const void* entity)
{
    try {
        return static_cast<const SvgEntity*>(entity)->getStrokeOpacity();
    } catch (...) { return 0; }
}

float svgparser_entity_get_stroke_width(const void* entity)
{
    try {
        return static_cast<const SvgEntity*>(entity)->getStrokeWidth();
    } catch (...) { return 0; }
}

int svgparser_entity_get_line_cap(const void* entity)
{
    try {
        return static_cast<int>(static_cast<const SvgEntity*>(entity)->getLineCap());
    } catch (...) { return 0; }
}

int svgparser_entity_get_line_join(const void* entity)
{
    try {
        return static_cast<int>(static_cast<const SvgEntity*>(entity)->getLineJoin());
    } catch (...) { return 0; }
}

float svgparser_entity_get_miter_limit(const void* entity)
{
    try {
        return static_cast<const SvgEntity*>(entity)->getMiterLimit();
    } catch (...) { return 0; }
}

const char *svgparser_entity_get_css_class(const void* entity)
{
    try {
        return static_cast<const SvgEntity*>(entity)->getCssClass().c_str();
    } catch (...) { return 0; }
}

const char *svgparser_entity_get_css_style(const void* entity)
{
    try {
        return static_cast<const SvgEntity*>(entity)->getCssStyle().c_str();
    } catch (...) { return 0; }
}

const void* svgparser_entity_get_fill(const void* entity) {
    try {
        return &static_cast<const SvgEntity*>(entity)->getFillColour(); // SvgPaint member, address-of since it's held by value
    } catch (...) { return nullptr; }
}

const void* svgparser_entity_get_stroke(const void* entity) {
    try {
        return &static_cast<const SvgEntity*>(entity)->getStrokeColour();
    } catch (...) { return nullptr; }
}

int svgparser_paint_get_colour_type(const void* paint) {
    try {
        return static_cast<int>(static_cast<const SvgPaint*>(paint)->getSvgColourType());
    } catch (...) { return -1; }
}

const char* svgparser_paint_get_gradient_id(const void* paint) {
    try {
        auto* p = static_cast<const SvgPaint*>(paint);
        if (p->getSvgColourType() != SvgColourType::Gradient) return nullptr;
        return p->getGradientId().c_str(); // Core::String, lives as long as the SvgPaint/entity does
    } catch (...) { return nullptr; }
}

const void* svgparser_paint_get_colour(const void* paint) {
    try {
        auto* p = static_cast<const SvgPaint*>(paint);
        if (p->getSvgColourType() == SvgColourType::Gradient) return nullptr;
        return &p->getColour();
    } catch (...) { return nullptr; }
}

int svgparser_paint_has_fallback(const void* paint) {
    try {
        return static_cast<const SvgPaint*>(paint)->getFallback().has_value() ? 1 : 0;
    } catch (...) { return 0; }
}

const void* svgparser_paint_get_fallback(const void* paint) {
    try {
        auto& fallback = static_cast<const SvgPaint*>(paint)->getFallback();
        return fallback.has_value() ? &fallback.value() : nullptr;
    } catch (...) { return nullptr; }
}

int svgparser_colour_get_colour_type(const void* colour) {
    try {
        return static_cast<int>(static_cast<const SvgColour*>(colour)->getSvgColourType());
    } catch (...) { return -1; }
}

int svgparser_colour_get_colour_index(const void* colour) {
    try {
        return static_cast<const SvgColour*>(colour)->getColourIndex();
    } catch (...) { return -1; }
}

int svgparser_colour_get_bgra(const void* colour, unsigned char* outBuffer4) {
    try {
        auto* c = static_cast<const SvgColour*>(colour);
        auto type = c->getSvgColourType();
        if (type != SvgColourType::Bgr && type != SvgColourType::BgrA) return 0;
        const auto& bgra = c->getBgrA();
        if (!bgra.has_value()) return 0;
        std::copy(bgra->begin(), bgra->end(), outBuffer4);
        return 1;
    } catch (...) { return 0; }
}

int svgparser_colour_get_hsla(const void* colour, float* outBuffer4, int bufferSize) {
    try {
        auto* c = static_cast<const SvgColour*>(colour);
        auto type = c->getSvgColourType();
        if (type != SvgColourType::Hsl && type != SvgColourType::HslA) return 0;
        auto hsla = c->getHslA(); // note: returns by value per your signature, not const&
        if (!hsla.has_value()) return 0;
        std::copy(hsla->begin(), hsla->end(), outBuffer4);
        return 1;
    } catch (...) { return 0; }
}

int svgparser_colour_has_fallback(const void* colour) {
    try {
        return static_cast<const SvgColour*>(colour)->getFallback() != nullptr ? 1 : 0;
    } catch (...) { return 0; }
}

const void* svgparser_colour_get_fallback(const void* colour) {
    try {
        return static_cast<const SvgColour*>(colour)->getFallback();
    } catch (...) { return nullptr; }
}

void* svgparser_entity_get_dimensioned(const void* entity) {
    try {
        return const_cast<void*>(static_cast<const void*>(
            static_cast<const SvgEntity*>(entity)->getDimensionedEntity()));
    } catch (...) { return nullptr; }
}

int svgparser_dimensionedentity_has_x(const void* dimensionedEntity) {
    try {
        return static_cast<const SvgDimensionedEntity*>(dimensionedEntity)->getX().has_value() ? 1 : 0;
    } catch (...) { return 0; }
}
float svgparser_dimensionedentity_get_x_measurement(const void* dimensionedEntity) {
    try {
        return static_cast<const SvgDimensionedEntity*>(dimensionedEntity)->getX().value().fValue;
    } catch (...) { return 0.0f; }
}
int svgparser_dimensionedentity_get_x_units(const void* dimensionedEntity) {
    try {
        return static_cast<int>(static_cast<const SvgDimensionedEntity*>(dimensionedEntity)->getX().value().enumUnits);
    } catch (...) { return -1; }
}

int svgparser_dimensionedentity_has_y(const void* dimensionedEntity) {
    try {
        return static_cast<const SvgDimensionedEntity*>(dimensionedEntity)->getY().has_value() ? 1 : 0;
    } catch (...) { return 0; }
}
float svgparser_dimensionedentity_get_y_measurement(const void* dimensionedEntity) {
    try {
        return static_cast<const SvgDimensionedEntity*>(dimensionedEntity)->getY().value().fValue;
    } catch (...) { return 0.0f; }
}
int svgparser_dimensionedentity_get_y_units(const void* dimensionedEntity) {
    try {
        return static_cast<int>(static_cast<const SvgDimensionedEntity*>(dimensionedEntity)->getY().value().enumUnits);
    } catch (...) { return -1; }
}

int svgparser_dimensionedentity_has_width(const void* dimensionedEntity) {
    try {
        return static_cast<const SvgDimensionedEntity*>(dimensionedEntity)->getWidth().has_value() ? 1 : 0;
    } catch (...) { return 0; }
}
float svgparser_dimensionedentity_get_width_measurement(const void* dimensionedEntity) {
    try {
        return static_cast<const SvgDimensionedEntity*>(dimensionedEntity)->getWidth().value().fValue;
    } catch (...) { return 0.0f; }
}
int svgparser_dimensionedentity_get_width_units(const void* dimensionedEntity) {
    try {
        return static_cast<int>(static_cast<const SvgDimensionedEntity*>(dimensionedEntity)->getWidth().value().enumUnits);
    } catch (...) { return -1; }
}

int svgparser_dimensionedentity_has_height(const void* dimensionedEntity) {
    try {
        return static_cast<const SvgDimensionedEntity*>(dimensionedEntity)->getHeight().has_value() ? 1 : 0;
    } catch (...) { return 0; }
}
float svgparser_dimensionedentity_get_height_measurement(const void* dimensionedEntity) {
    try {
        return static_cast<const SvgDimensionedEntity*>(dimensionedEntity)->getHeight().value().fValue;
    } catch (...) { return 0.0f; }
}
int svgparser_dimensionedentity_get_height_units(const void* dimensionedEntity) {
    try {
        return static_cast<int>(static_cast<const SvgDimensionedEntity*>(dimensionedEntity)->getHeight().value().enumUnits);
    } catch (...) { return -1; }
}

void* svgparser_entity_get_container(const void* entity) {
    try {
        return const_cast<void*>(static_cast<const void*>(
            dynamic_cast<const SvgContainerEntity*>(static_cast<const SvgEntity*>(entity))));
    } catch (...) { return nullptr; }
}

int svgparser_path_get_points_count(const void* path) {
    try {
        return static_cast<int>(static_cast<const Path*>(path)->getPoints().size());
    } catch (...) { return 0; }
}
void svgparser_path_get_points(const void* path, float* buffer, int bufferSize) {
    try {
        auto pts = static_cast<const Path*>(path)->getPoints(); // by value per your getter
        int n = std::min<int>(bufferSize, static_cast<int>(pts.size()));
        std::copy(pts.begin(), pts.begin() + n, buffer);
    } catch (...) {}
}
int svgparser_path_get_moves_count(const void* path) {
    try {
        return static_cast<int>(static_cast<const Path*>(path)->getPathMoves().size());
    } catch (...) { return 0; }
}
void svgparser_path_get_moves(const void* path, int* buffer, int bufferSize) {
    try {
        auto moves = static_cast<const Path*>(path)->getPathMoves(); // by value
        int n = std::min<int>(bufferSize, static_cast<int>(moves.size()));
        for (int i = 0; i < n; ++i) buffer[i] = static_cast<int>(moves[i]);
    } catch (...) {}
}

float svgparser_group_get_opacity(const void* group) {
    try {
        return static_cast<const Group*>(group)->getOpacity();
    } catch (...) { return 1.0f; }
}

int svgparser_rect_has_rx(const void* rect) {
    try { return static_cast<const Rect*>(rect)->getRx().has_value() ? 1 : 0; } catch (...) { return 0; }
}
float svgparser_rect_get_rx_measurement(const void* rect) {
    try { return static_cast<const Rect*>(rect)->getRx().value().fValue; } catch (...) { return 0.0f; }
}
int svgparser_rect_get_rx_units(const void* rect) {
    try { return static_cast<int>(static_cast<const Rect*>(rect)->getRx().value().enumUnits); } catch (...) { return -1; }
}
int svgparser_rect_has_ry(const void* rect) {
    try { return static_cast<const Rect*>(rect)->getRy().has_value() ? 1 : 0; } catch (...) { return 0; }
}
float svgparser_rect_get_ry_measurement(const void* rect) {
    try { return static_cast<const Rect*>(rect)->getRy().value().fValue; } catch (...) { return 0.0f; }
}
int svgparser_rect_get_ry_units(const void* rect) {
    try { return static_cast<int>(static_cast<const Rect*>(rect)->getRy().value().enumUnits); } catch (...) { return -1; }
}
// --- Circle ---
int svgparser_circle_has_cx(const void* circle) {
    try { return static_cast<const Circle*>(circle)->getCx().has_value() ? 1 : 0; } catch (...) { return 0; }
}
float svgparser_circle_get_cx_measurement(const void* circle) {
    try { return static_cast<const Circle*>(circle)->getCx().value().fValue; } catch (...) { return 0.0f; }
}
int svgparser_circle_get_cx_units(const void* circle) {
    try { return static_cast<int>(static_cast<const Circle*>(circle)->getCx().value().enumUnits); } catch (...) { return -1; }
}
int svgparser_circle_has_cy(const void* circle) {
    try { return static_cast<const Circle*>(circle)->getCy().has_value() ? 1 : 0; } catch (...) { return 0; }
}
float svgparser_circle_get_cy_measurement(const void* circle) {
    try { return static_cast<const Circle*>(circle)->getCy().value().fValue; } catch (...) { return 0.0f; }
}
int svgparser_circle_get_cy_units(const void* circle) {
    try { return static_cast<int>(static_cast<const Circle*>(circle)->getCy().value().enumUnits); } catch (...) { return -1; }
}
int svgparser_circle_has_r(const void* circle) {
    try { return static_cast<const Circle*>(circle)->getR().has_value() ? 1 : 0; } catch (...) { return 0; }
}
float svgparser_circle_get_r_measurement(const void* circle) {
    try { return static_cast<const Circle*>(circle)->getR().value().fValue; } catch (...) { return 0.0f; }
}
int svgparser_circle_get_r_units(const void* circle) {
    try { return static_cast<int>(static_cast<const Circle*>(circle)->getR().value().enumUnits); } catch (...) { return -1; }
}

// --- Ellipse ---
int svgparser_ellipse_has_cx(const void* ellipse) {
    try { return static_cast<const Ellipse*>(ellipse)->getCx().has_value() ? 1 : 0; } catch (...) { return 0; }
}
float svgparser_ellipse_get_cx_measurement(const void* ellipse) {
    try { return static_cast<const Ellipse*>(ellipse)->getCx().value().fValue; } catch (...) { return 0.0f; }
}
int svgparser_ellipse_get_cx_units(const void* ellipse) {
    try { return static_cast<int>(static_cast<const Ellipse*>(ellipse)->getCx().value().enumUnits); } catch (...) { return -1; }
}
int svgparser_ellipse_has_cy(const void* ellipse) {
    try { return static_cast<const Ellipse*>(ellipse)->getCy().has_value() ? 1 : 0; } catch (...) { return 0; }
}
float svgparser_ellipse_get_cy_measurement(const void* ellipse) {
    try { return static_cast<const Ellipse*>(ellipse)->getCy().value().fValue; } catch (...) { return 0.0f; }
}
int svgparser_ellipse_get_cy_units(const void* ellipse) {
    try { return static_cast<int>(static_cast<const Ellipse*>(ellipse)->getCy().value().enumUnits); } catch (...) { return -1; }
}
int svgparser_ellipse_has_rx(const void* ellipse) {
    try { return static_cast<const Ellipse*>(ellipse)->getRx().has_value() ? 1 : 0; } catch (...) { return 0; }
}
float svgparser_ellipse_get_rx_measurement(const void* ellipse) {
    try { return static_cast<const Ellipse*>(ellipse)->getRx().value().fValue; } catch (...) { return 0.0f; }
}
int svgparser_ellipse_get_rx_units(const void* ellipse) {
    try { return static_cast<int>(static_cast<const Ellipse*>(ellipse)->getRx().value().enumUnits); } catch (...) { return -1; }
}
int svgparser_ellipse_has_ry(const void* ellipse) {
    try { return static_cast<const Ellipse*>(ellipse)->getRy().has_value() ? 1 : 0; } catch (...) { return 0; }
}
float svgparser_ellipse_get_ry_measurement(const void* ellipse) {
    try { return static_cast<const Ellipse*>(ellipse)->getRy().value().fValue; } catch (...) { return 0.0f; }
}
int svgparser_ellipse_get_ry_units(const void* ellipse) {
    try { return static_cast<int>(static_cast<const Ellipse*>(ellipse)->getRy().value().enumUnits); } catch (...) { return -1; }
}

// --- Image ---

const char* svgparser_image_get_href(const void* image) {
    try { return static_cast<const Image*>(image)->getHref().c_str(); } catch (...) { return nullptr; }
}

const char* svgparser_image_get_image_type(const void* image) {
    try { return static_cast<const Image*>(image)->getImageType().c_str(); } catch (...) { return nullptr; }
}

const char* svgparser_image_get_character_encoding(const void* image) {
    try { return static_cast<const Image*>(image)->getCharacterEncoding().c_str(); } catch (...) { return nullptr; }
}

int svgparser_image_get_preserve_aspect_ratio(const void* image) {
    try { return static_cast<int>(static_cast<const Image*>(image)->getPreserveAspectRatio()); } catch (...) { return 0; }
}

int svgparser_image_get_preserve_aspect_ratio_mode(const void* image) {
    try { return static_cast<int>(static_cast<const Image*>(image)->getPreserveAspectRatioMode()); } catch (...) { return 0; }
}

int svgparser_image_get_has_cross_origin(const void* image) {
    try { return static_cast<const Image*>(image)->getCrossOrigin().has_value() ? 1 : 0; } catch (...) { return 0; }
}

int svgparser_image_get_cross_origin(const void* image) {
    try { return static_cast<int>(static_cast<const Image*>(image)->getCrossOrigin().value()); } catch (...) { return 0; }
}

int svgparser_image_get_has_decoding(const void* image) {
    try { return static_cast<const Image*>(image)->getDecoding().has_value() ? 1 : 0; } catch (...) { return 0; }
}

int svgparser_image_get_decoding(const void* image) {
    try { return static_cast<int>(static_cast<const Image*>(image)->getDecoding().value()); } catch (...) { return 0; }
}

int svgparser_image_get_has_fetch_priority(const void* image) {
    try { return static_cast<const Image*>(image)->getFetchPriority().has_value() ? 1 : 0; } catch (...) { return 0; }
}

int svgparser_image_get_fetch_priority(const void* image) {
    try { return static_cast<int>(static_cast<const Image*>(image)->getFetchPriority().value()); } catch (...) { return 0; }
}

// --- Text ---

int svgparser_text_get_x_count(const void* text) {
    try { return static_cast<int>(static_cast<const Text*>(text)->getX().size()); } catch (...) { return 0; }
}

void svgparser_text_get_x(const void* text, float* buffer, int bufferSize) {
    try {
        const auto &values = static_cast<const Text*>(text)->getX();
        int count = std::min(bufferSize, static_cast<int>(values.size()));
        std::copy(values.begin(), values.begin() + count, buffer);
    } catch (...) {}
}

int svgparser_text_get_y_count(const void* text) {
    try { return static_cast<int>(static_cast<const Text*>(text)->getY().size()); } catch (...) { return 0; }
}

void svgparser_text_get_y(const void* text, float* buffer, int bufferSize) {
    try {
        const auto &values = static_cast<const Text*>(text)->getY();
        int count = std::min(bufferSize, static_cast<int>(values.size()));
        std::copy(values.begin(), values.begin() + count, buffer);
    } catch (...) {}
}

int svgparser_text_get_dx_count(const void* text) {
    try { return static_cast<int>(static_cast<const Text*>(text)->getDx().size()); } catch (...) { return 0; }
}

void svgparser_text_get_dx(const void* text, float* buffer, int bufferSize) {
    try {
        const auto &values = static_cast<const Text*>(text)->getDx();
        int count = std::min(bufferSize, static_cast<int>(values.size()));
        std::copy(values.begin(), values.begin() + count, buffer);
    } catch (...) {}
}

int svgparser_text_get_dy_count(const void* text) {
    try { return static_cast<int>(static_cast<const Text*>(text)->getDy().size()); } catch (...) { return 0; }
}

void svgparser_text_get_dy(const void* text, float* buffer, int bufferSize) {
    try {
        const auto &values = static_cast<const Text*>(text)->getDy();
        int count = std::min(bufferSize, static_cast<int>(values.size()));
        std::copy(values.begin(), values.begin() + count, buffer);
    } catch (...) {}
}

int svgparser_text_get_rotate_count(const void* text) {
    try { return static_cast<int>(static_cast<const Text*>(text)->getRotate().size()); } catch (...) { return 0; }
}

void svgparser_text_get_rotate(const void* text, float* buffer, int bufferSize) {
    try {
        const auto &values = static_cast<const Text*>(text)->getRotate();
        int count = std::min(bufferSize, static_cast<int>(values.size()));
        std::copy(values.begin(), values.begin() + count, buffer);
    } catch (...) {}
}

int svgparser_text_get_length_adjust(const void* text) {
    try { return static_cast<int>(static_cast<const Text*>(text)->getLengthAdjust()); } catch (...) { return 0; }
}

int svgparser_text_has_text_length(const void* text) {
    try { return static_cast<const Text*>(text)->getTextLength().has_value() ? 1 : 0; } catch (...) { return 0; }
}

float svgparser_text_get_text_length(const void* text) {
    try { return static_cast<const Text*>(text)->getTextLength().value(); } catch (...) { return 0.0f; }
}

// --- Svg ---

int svgparser_svg_get_preserve_aspect_ratio(const void* svg) {
    try { return static_cast<int>(static_cast<const Draw2d::Svg::Svg*>(svg)->getPreserveAspectRatio()); } catch (...) { return 0; }
}

int svgparser_svg_get_preserve_aspect_ratio_mode(const void* svg) {
    try { return static_cast<int>(static_cast<const Draw2d::Svg::Svg*>(svg)->getPreserveAspectRatioMode()); } catch (...) { return 0; }
}

int svgparser_svg_has_viewbox(const void* svg) {
    try { return static_cast<const Draw2d::Svg::Svg*>(svg)->getViewbox().has_value() ? 1 : 0; } catch (...) { return 0; }
}

float svgparser_svg_get_viewbox_x(const void* svg) {
    try { return static_cast<const Draw2d::Svg::Svg*>(svg)->getViewbox().value().x; } catch (...) { return 0.0f; }
}

float svgparser_svg_get_viewbox_y(const void* svg) {
    try { return static_cast<const Draw2d::Svg::Svg*>(svg)->getViewbox().value().y; } catch (...) { return 0.0f; }
}

float svgparser_svg_get_viewbox_width(const void* svg) {
    try { return static_cast<const Draw2d::Svg::Svg*>(svg)->getViewbox().value().width; } catch (...) { return 0.0f; }
}

float svgparser_svg_get_viewbox_height(const void* svg) {
    try { return static_cast<const Draw2d::Svg::Svg*>(svg)->getViewbox().value().height; } catch (...) { return 0.0f; }
}

void svgparser_entity_enumerate_children(const void* entity, SvgEntityCallback callback, void* userData) {
    try {
        auto* pContainer = dynamic_cast<const SvgContainerEntity*>(static_cast<const SvgEntity*>(entity));
        if (!pContainer) return; // not a container type � no children
        for (const SvgEntity* pChild : pContainer->enumerateChildren()) {
            callback(pChild, userData);
        }
    } catch (...) {
        // swallow � cannot propagate a C++ exception across the C boundary
    }
}

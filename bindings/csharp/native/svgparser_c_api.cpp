#include <xercesc/util/PlatformUtils.hpp>
#include "svgparser_c_api.h"
#include "svg/SvgParser.hpp"
#include "svg/SvgDocument.hpp"
#include "svg/SvgContainerEntity.hpp"
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

const void* svgparser_document_get_root(const void* doc) {
    try {
        return static_cast<const SvgDocument*>(doc)->getRootSvg();
    } catch (...) {
        return nullptr;
    }
}

const void* svgparser_document_lookup_svg_entity(const void* doc, const char* id) {
    try {
        // NOTE: adjust if IDomDocument::getElementById lives elsewhere in your real API —
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
        int n = std::min<int>(bufferSize, static_cast<int>(tf.size()));
        std::copy(tf.begin(), tf.begin() + n, outBuffer6);
        return n;
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

const char* svgparser_entity_get_tag_name(const void* entity) {
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
        return static_cast<int>(static_cast<const SvgEntity*>(entity)->getFillOpacity());
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
        return static_cast<int>(static_cast<const SvgEntity*>(entity)->getStrokeOpacity());
    } catch (...) { return 0; }
}

int svgparser_entity_get_stroke_width(const void* entity)
{
    try {
        return static_cast<int>(static_cast<const SvgEntity*>(entity)->getStrokeWidth());
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
        return static_cast<int>(static_cast<const SvgEntity*>(entity)->getMiterLimit());
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

void svgparser_entity_enumerate_children(const void* entity, SvgEntityCallback callback, void* userData) {
    try {
        auto* pContainer = dynamic_cast<const SvgContainerEntity*>(static_cast<const SvgEntity*>(entity));
        if (!pContainer) return; // not a container type — no children
        for (const SvgEntity* pChild : pContainer->enumerateChildren()) {
            callback(pChild, userData);
        }
    } catch (...) {
        // swallow — cannot propagate a C++ exception across the C boundary
    }
}

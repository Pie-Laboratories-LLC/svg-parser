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

#ifndef SVGPARSER_DOT_HPP
#define SVGPARSER_DOT_HPP

#include <xercesc/dom/DOM.hpp>               // xc::DOMDocument, xc::DOMElement, DOMNodeList, etc
#include <vector>
#include <memory>
#include <regex>
#include <tuple>
#include <unordered_set>
#ifndef STDNUMERIC_DOT_HPP
    #include "core/StdNumeric.hpp"
#endif
#ifndef STRING_DOT_HPP
    #include "core/String.hpp"
#endif
#ifndef SVGENTITY_DOT_HPP
    #include "svg/SvgEntity.hpp"
#endif
#ifndef PATHMOVE_DOT_HPP
    #include "svg/PathMove.hpp"
#endif
#ifndef DRAW2D_GRADIENTUNITS_DOT_HPP
    #include "draw2d/GradientUnits.hpp"
#endif
#ifndef DRAW2D_GRADIENTTEMPLATE_DOT_HPP
    #include "svg/GradientTemplate.hpp"
#endif
#ifndef DRAW2D_SVG_SVGDOCUMENT_DOT_HPP
    #include "svg/SvgDocument.hpp"
#endif
#ifndef DRAW2D_SVG_SVGPARSERCONTEXT_DOT_HPP
    #include "svg/SvgParserContext.hpp"
#endif
#ifndef DRAW2D_SVG_SVGPATHLENGTHABLE_DOT_HPP
    #include "svg/SvgPathLengthableEntity.hpp"
#endif
#ifndef DRAW2D_SVG_STYLESHEET_DOT_HPP
    #include "svg/StyleSheet.hpp"
#endif

namespace Core {
    class String;
}

namespace DataStructures::Tree {

    template<typename T>
    class GeneralTree;

    template<typename T>
    class GeneralTreeNode;

}

namespace Draw2d {

    template<typename T>
    class Gradient;

}

namespace Xml {

    class IDomEntity;
    class IDomParser;
    class IDomDocument;

}

namespace Draw2d::Svg {

    class SvgEntity;
    class Path;
    class Group;
    class Mask;
    class ClipPath;
    class Use;
    class Rect;
    class Ellipse;
    class Circle;
    class Image;
    class Text;
    class RawText;
    struct SvgTSpanParams;
    class TSpan;
    class SvgColour;
    class SvgPaint;
    class SvgParserContext;

    enum class SvgParserStatus {
        Info = 0,
        Warning,
        Error
    };

    struct SvgParseState {
        SvgDocument &svgDocument;
        std::unique_ptr<Xml::IDomDocument> pRootEntity;
        std::vector<Svg *> currentSvg {};
        bool bWithNamespace = false;
        bool bWithValidation = false;
        std::unordered_set<Core::String> useReferences { };
        // every <style> in the document, gathered before parsing so a stylesheet placed after
        //  the content still applies to it
        StyleSheet styleSheet { };
    };

    // an element's declared presentation values, by property name: its presentation
    //  attributes, overridden by the stylesheet, overridden by its style attribute
    typedef std::unordered_map<Core::String, Core::String> DeclaredValues;

    class SvgParser
    {
    public:
        inline static const Core::String XLINK_NAMESPACE = "http://www.w3.org/1999/xlink";
        inline static const Core::String NAME_ATTRIBUTE = "name";
        inline static const Core::String ID_ATTRIBUTE = "id";
        inline static const Core::String TRANSFORM_ATTRIBUTE = "transform";
        inline static const Core::String FILL_ATTRIBUTE = "fill";
        inline static const Core::String FILL_OPACITY_ATTRIBUTE = "fill-opacity"; // *
        inline static const Core::String FILL_RULE_ATTRIBUTE = "fill-rule"; // *
        inline static const Core::String STROKE_ATTRIBUTE = "stroke";
        inline static const Core::String STROKE_WIDTH_ATTRIBUTE = "stroke-width"; // *
        inline static const Core::String STROKE_OPACITY_ATTRIBUTE = "stroke-opacity"; // *
        inline static const Core::String STROKE_LINECAP_ATTRIBUTE = "stroke-linecap"; // *
        inline static const Core::String STROKE_LINEJOIN_ATTRIBUTE = "stroke-linejoin"; // *
        inline static const Core::String STROKE_MITRELIMIT_ATTRIBUTE = "stroke-miterlimit"; // *
        inline static const Core::String STROKE_DASHARRAY_ATTRIBUTE = "stroke-dasharray"; // *
        inline static const Core::String STROKE_DASHOFFSET_ATTRIBUTE = "stroke-dashoffset"; // *
        inline static const Core::String VISIBILITY_ATTRIBUTE = "visibility"; // *
        inline static const Core::String DISPLAY_ATTRIBUTE = "display"; // *
        inline static const Core::String HREF_ATTRIBUTE = "href";
        inline static const Core::String XLINK_HREF_ATTRIBUTE = "xlink:href";
        inline static const Core::String D_ATTRIBUTE = "d";
        inline static const Core::String X1_ATTRIBUTE = "x1";
        inline static const Core::String Y1_ATTRIBUTE = "y1";
        inline static const Core::String X2_ATTRIBUTE = "x2";
        inline static const Core::String Y2_ATTRIBUTE = "y2";
        inline static const Core::String GRADIENTUNITS_ATTRIBUTE = "gradientUnits";
        inline static const Core::String GRADIENTTRANSFORM_ATTRIBUTE = "gradientTransform";
        inline static const Core::String OPACITY_ATTRIBUTE = "opacity";
        inline static const Core::String OFFSET_ATTRIBUTE = "offset";
        inline static const Core::String STOP_COLOUR_ATTRIBUTE = "stop-color";
        inline static const Core::String STOP_OPACITY_ATTRIBUTE = "stop-opacity";
        inline static const Core::String X_ATTRIBUTE = "x";
        inline static const Core::String Y_ATTRIBUTE = "y";
        inline static const Core::String WIDTH_ATTRIBUTE = "width";
        inline static const Core::String HEIGHT_ATTRIBUTE = "height";
        inline static const Core::String CX_ATTRIBUTE = "cx";
        inline static const Core::String CY_ATTRIBUTE = "cy";
        inline static const Core::String DX_ATTRIBUTE = "dx";
        inline static const Core::String DY_ATTRIBUTE = "dy";
        inline static const Core::String FX_ATTRIBUTE = "fx";
        inline static const Core::String FY_ATTRIBUTE = "fy";
        inline static const Core::String R_ATTRIBUTE = "r";
        inline static const Core::String FR_ATTRIBUTE = "fr";
        inline static const Core::String SPREADMETHOD_ATTRIBUTE = "spreadMethod";
        inline static const Core::String RX_ATTRIBUTE = "rx";
        inline static const Core::String RY_ATTRIBUTE = "ry";
        inline static const Core::String CLASS_ATTRIBUTE = "class";
        inline static const Core::String STYLE_ATTRIBUTE = "style";
        inline static const Core::String VIEWBOX_ATTRIBUTE = "viewBox";
        inline static const Core::String PRESERVEASPECTRATIO_ATTRIBUTE = "preserveAspectRatio";
        inline static const Core::String COLOR_ATTRIBUTE = "color";
        inline static const Core::String PATHLENGTH_ATTRIBUTE = "pathLength";
        inline static const Core::String POINTS_ATTRIBUTE = "points";
        inline static const Core::String MASK_ATTRIBUTE = "mask";
        inline static const Core::String MIX_BLEND_MODE_ATTRIBUTE = "mix-blend-mode";
        inline static const Core::String ISOLATION_ATTRIBUTE = "isolation";
        inline static const Core::String CLIP_PATH_ATTRIBUTE = "clip-path";
        inline static const Core::String CLIPPATHUNITS_ATTRIBUTE = "clipPathUnits";
        inline static const Core::String MASKUNITS_ATTRIBUTE = "maskUnits";
        inline static const Core::String MASKCONTENTUNITS_ATTRIBUTE = "maskContentUnits";
        inline static const Core::String CROSSORIGIN_ATTRIBUTE = "crossorigin";
        inline static const Core::String DECODING_ATTRIBUTE = "decoding";
        inline static const Core::String FETCHPRIORITY_ATTRIBUTE = "fetchpriority";
        inline static const Core::String ROTATE_ATTRIBUTE = "rotate";
        inline static const Core::String TEXTLENGTH_ATTRIBUTE = "textLength";
        inline static const Core::String LENGTHADJUST_ATTRIBUTE = "lengthAdjust";
        inline static const Core::String FONTSIZE_ATTRIBUTE = "font-size";
        inline static const Core::String FONTFAMILY_ATTRIBUTE = "font-family";
        inline static const Core::String FONTSTYLE_ATTRIBUTE = "font-style";
        inline static const Core::String TEXTANCHOR_ATTRIBUTE = "text-anchor";
        inline static const Core::String DEFINITIONS_NAME = "defs";
        inline static const Core::String GROUP_NAME = "g";
        inline static const Core::String USE_NAME = "use";
        inline static const Core::String PATH_NAME = "path";
        inline static const Core::String LINEAR_GRADIENT_NAME = "linearGradient";
        inline static const Core::String RADIAL_GRADIENT_NAME = "radialGradient";
        inline static const Core::String SVG_NAME = "svg";
        inline static const Core::String STYLE_NAME = "style";
        inline static const Core::String RECT_NAME = "rect";
        inline static const Core::String ELLIPSE_NAME = "ellipse";
        inline static const Core::String CIRCLE_NAME = "circle";
        inline static const Core::String POLYGON_NAME = "polygon";
        inline static const Core::String MASK_NAME = "mask";
        inline static const Core::String CLIP_PATH_NAME = "clipPath";
        inline static const Core::String POLYLINE_NAME = "polyline";
        inline static const Core::String IMAGE_NAME = "image";
        inline static const Core::String IMAGE_ALTERNATE_NAME = "img";
        inline static const Core::String TEXT_NAME = "text";
        inline static const Core::String DEGREES_UNIT = "deg";
        inline static const Core::String GRADIANS_UNIT = "grad";
        inline static const Core::String RADIANS_UNIT = "rad";
        inline static const Core::String TURN_UNIT = "turn";
        inline static const Core::String NONE_VALUE = "none";
        inline static const Core::String INHERIT_VALUE = "inherit";
        inline static const Core::String AUTO_VALUE = "auto";
        inline static const Core::String CHARSET_VALUE = "charset";
        inline static const Core::String UTF8_VALUE = "utf-8";
        inline static constexpr float DEFAULT_VIEWPORT_SIZE = 128.0f;

        SvgParser();
        ~SvgParser();

        typedef std::function<void(SvgParserStatus,const Core::String &)> SvgParserCallback;
        void setParserCallback(SvgParserCallback callback);

        std::unique_ptr<SvgDocument> parse(const Core::String &cstrSvgText
                                          ,float fViewportWidth = DEFAULT_VIEWPORT_SIZE
                                          ,float fViewportHeight = DEFAULT_VIEWPORT_SIZE
                                          ,bool bWithNamespace = false
                                          ,bool bWithValidation = false);

        static std::regex SvgRgbRegex;
        static std::regex SvgHslRegex;
        static std::regex UrlRegex;
        static std::regex VarRegex;
        static std::regex HexColourRegex;
        static std::regex TranslateRegex;
        static std::regex ScaleRegex;
        static std::regex RotateRegex;
        static std::regex SkewXyRegex;
        static std::regex MatrixRegex;
        static std::regex DimensionRegex;
        static std::regex DataUriRegex;
        static std::regex NumberRegex;

    private:
        SvgParserCallback m_callback = [](SvgParserStatus, const Core::String &) {}; // noop

        void __parsePreserveAspectRatio(Xml::IDomEntity *pSvgElement,PreserveAspectRatio &preserveAspectRatio,PreserveAspectRatioMode &preserveAspectRatioMode);
        const Svg *__parseSvgElement(SvgParseState &svgParseState, Xml::IDomEntity *pRootElement, SvgParserContext svgParserContext);
        void __parseGlobalScope(SvgParseState &svgParserState, Xml::IDomEntity *pParentElement, SvgParserContext svgParserContext);

        std::tuple<std::optional<Dimension>,std::optional<Dimension>,std::optional<Dimension>,std::optional<Dimension>> __parseDimensions(SvgParseState &svgParseState, Xml::IDomEntity *pChildElement);
        std::optional<Dimension> __parseDimension(Xml::IDomEntity *pChildElement, const Core::String &cstrAttributeName);
        std::optional<Dimension> __parseAutoDimension(Xml::IDomEntity *pChildElement, const Core::String &cstrAttributeName);

        SvgPathLengthableParams __checkPathLengthable(SvgParseState &svgParseState, Xml::IDomEntity *pChildElement, SvgParserContext svgParserContext);

        void __snagStyle(SvgParseState &svgParserState, Xml::IDomEntity *pStyleElement);

        const SvgEntity *__parseSvgEntity(SvgParseState &svgParseState, Xml::IDomEntity *pChildElement, SvgParserContext svgParserContext);

        void __collectStyleSheets(SvgParseState &svgParseState, Xml::IDomEntity *pElement);
        DeclaredValues __declaredValues(SvgParseState &svgParseState, Xml::IDomEntity *pElement);
        template <typename F>
        void __applyIfSet(const DeclaredValues &declaredValues, const Core::String &cstrPropertyName, F &&apply);
        // returns the element's declared values, for the non-inherited properties the caller needs
        DeclaredValues __updateInheritedProperties(SvgParseState &svgParseState, Xml::IDomEntity *pElement, SvgParserContext &svgParserContext);
        float __parseLength(const Core::String &cstrLength, const SvgParserContext &svgParserContext);
        float __parseUnitInterval(const Core::String &cstrValue);
        void __parseCommonProperties(SvgParseState &svgParseState, Xml::IDomEntity *pRenderedElement, SvgEntityParams &params, const SvgParserContext &svgParserContext, const DeclaredValues &declaredValues);
        void __saveEntityToDocument(SvgParseState &svgParseState,const SvgEntity *cpEntity);
        const Mask *__parseMask(SvgParseState &svgParseState, Xml::IDomEntity *pMaskElement, SvgParserContext svgParserContext);
        const ClipPath *__parseClipPath(SvgParseState &svgParseState, Xml::IDomEntity *pClipPathElement, SvgParserContext svgParserContext);
        const Group *__parseGroup(SvgParseState &svgParseState, Xml::IDomEntity *pGroupNode, SvgParserContext svgParserContext);
        const Path *__parsePath(SvgParseState &svgParseState, Xml::IDomEntity *pPathNode, const SvgParserContext &svgParserContextIn);
        // <polygon> (bClosed) and <polyline>
        const Path *__parsePolyPoints(SvgParseState &svgParseState, Xml::IDomEntity *pElement, SvgParserContext svgParserContext, bool bClosed);
        const Use *__parseUse(SvgParseState &svgParseState, Xml::IDomEntity *pUseElement, SvgParserContext svgParserContext);
        const Rect *__parseRect(SvgParseState &svgParseState, Xml::IDomEntity *pRectElement, SvgParserContext svgParserContext);
        const Ellipse *__parseEllipse(SvgParseState &svgParseState, Xml::IDomEntity *pEllipseElement, SvgParserContext svgParserContext);
        const Circle *__parseCircle(SvgParseState &svgParseState, Xml::IDomEntity *pCircleElement, SvgParserContext svgParserContext);
        const Image *__parseImage(SvgParseState &svgParseState, Xml::IDomEntity *pImageElement, SvgParserContext svgParserContext);
        const Text *__parseText(SvgParseState &svgParseState, Xml::IDomEntity *pTextElement, SvgParserContext svgParserContext);
        void __parseCommonTextProperties(SvgTSpanParams &svgTSpanParams, Xml::IDomEntity *pTextElement);
        RawText __parseRawText(Xml::IDomEntity *pTextElement);
        std::unique_ptr<TSpan> __parseTSpan(Xml::IDomEntity *pTextElement);

        Core::String __retrieveHref(SvgParseState &svgParseState, Xml::IDomEntity *pElement);
        std::vector<float> __parsePoints(const Core::String &d, size_t &pos, size_t nMaximumPoints = 0, bool bExactMaximum = false, bool slurp = false);

        SvgPaint __parsePaint(SvgParseState &svgParseState, const Core::String &colour, SvgParserContext &svgParserContext);
        SvgColour __parseColour(SvgParseState &svgParseState, const Core::String &colour, SvgParserContext &svgParserContext);

        std::unique_ptr<DataStructures::Tree::GeneralTree<Core::String>> __parseVar(const Core::String &cstrVar);
        void __doParseVar(const Core::String &cstrVar, size_t &nPos, DataStructures::Tree::GeneralTree<Core::String> *pGeneralTree, DataStructures::Tree::GeneralTreeNode<Core::String> *pGeneralTreeNode = nullptr);

        const GradientTemplate *__parseGradient(SvgParseState &svgParseState, Xml::IDomEntity *pGradientElement, SvgParserContext &svgParserContext);

        std::array<float,6> __parseTransform(const Core::String &value, const Core::String &attributeName);

        std::vector<Stop<SvgColour>> __parseStops(SvgParseState &svgParseState, Xml::IDomEntity *pStopsNode, SvgParserContext &svgParserContext);

    }; // class SvgParser

} // namespace Draw2d::Svg

#endif  /* SVGPARSER_DOT_HPP */

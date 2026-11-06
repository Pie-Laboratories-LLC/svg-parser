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

#include <cstring>
#include <unordered_set>
#include <iostream>
#include <numbers>

#include "svg/SvgParser.hpp"

#ifndef DOMPARSER_DOT_HPP
    #include "xml/DomParserBuilder.hpp"
#endif
#ifndef SCOPEEXIT_DOT_HPP
    #include "core/ScopeExit.hpp"
#endif
#ifndef STDNUMERIC_DOT_HPP
    #include "core/StdNumeric.hpp"
#endif
#ifndef DRAW2D_STANDARDCOLOURS_DOT_HPP
    #include "draw2d/StandardColours.hpp"
#endif
#ifndef DRAW2D_SVG_SVG_DOT_HPP
    #include "svg/Svg.hpp"
#endif
#ifndef DRAW2D_SVG_PATH_DOT_HPP
    #include "svg/Path.hpp"
#endif
#ifndef DRAW2D_SVG_USE_DOT_HPP
    #include "svg/Use.hpp"
#endif
#ifndef DRAW2D_SVG_POLYGON_DOT_HPP
    #include "svg/Polygon.hpp"
#endif
#ifndef DRAW2D_SVG_MASK_DOT_HPP
    #include "svg/Mask.hpp"
#endif
#ifndef DRAW2D_SVG_CLIPPATH_DOT_HPP
    #include "svg/ClipPath.hpp"
#endif
#ifndef DRAW2D_SVG_POLYLINE_DOT_HPP
    #include "svg/Polyline.hpp"
#endif
#ifndef DRAW2D_SVG_GROUP_DOT_HPP
    #include "svg/Group.hpp"
#endif
#ifndef DRAW2D_SVG_RECT_DOT_HPP
    #include "svg/Rect.hpp"
#endif
#ifndef DRAW2D_SVG_ELLIPSE_DOT_HPP
    #include "svg/Ellipse.hpp"
#endif
#ifndef DRAW2D_SVG_CIRCLE_DOT_HPP
    #include "svg/Circle.hpp"
#endif
#ifndef DRAW2D_SVG_IMAGE_DOT_HPP
    #include "svg/Image.hpp"
#endif
#ifndef DRAW2D_SVG_TEXT_DOT_HPP
    #include "svg/Text.hpp"
#endif
#ifndef DRAW2D_SVG_SVGGRADIENT_DOT_HPP
    #include "svg/SvgColour.hpp"
#endif
#ifndef DRAW2D_SVG_SVGPAINT_DOT_HPP
    #include "svg/SvgPaint.hpp"
#endif
#ifndef XML_STRING_DOT_HPP
    #include "xml/String.hpp"
#endif
#ifndef CORE_STDUNIQUEPTREXTENSIONS_DOT_HPP
    #include "core/StdUniquePtrExtensions.hpp"
#endif
#ifndef DRAW2D_SVG_MATRIXMATH_DOT_HPP
    #include "svg/MatrixMath.hpp"
#endif
#ifndef DATASTRUCTURES_TREE_GENERALTREE_DOT_HPP
    #include "data-structures/tree/GeneralTree.hpp"
#endif
#ifndef DRAW2D_SVG_LINECAP_DOT_HPP
    #include "svg/LineCap.hpp"
#endif
#ifndef DRAW2D_SVG_LINEJOIN_DOT_HPP
    #include "svg/LineJoin.hpp"
#endif
#ifndef DRAW2D_SVG_TEXTPARTS_DOT_HPP
    #include "svg/TextParts.hpp"
#endif
#ifndef SVG_SVGDPARSER_DOT_HPP
    #include "svg/SvgDParser.hpp"
#endif
#ifndef XML_DOMPARSERBUILDER_DOT_HPP
    #include "xml/DomParserBuilder.hpp"
#endif
#ifndef XML_IDOMPARSER_DOT_HPP
    #include "xml/IDomParser.hpp"
#endif
#ifndef XML_IDOMDOCUMENT_DOT_HPP
    #include "xml/IDomDocument.hpp"
#endif
#ifndef XML_IDOMENTITY_DOT_HPP
    #include "xml/IDomEntity.hpp"
#endif
#ifndef CORE_BASE64_DOT_HPP
    #include "core/Base64.hpp"
#endif
#ifndef CORE_LOGGER_DOT_HPP
    #include "core/Logger.hpp"
#endif
#include "draw2d/LoggingCategories.hpp"

namespace Draw2d::Svg {

    XERCES_CPP_NAMESPACE_USE

    SvgParser::SvgParser() = default;
    SvgParser::~SvgParser() = default;

    void SvgParser::setParserCallback(SvgParserCallback callback)
    {
        m_callback = callback;
    }

    std::unique_ptr<SvgDocument> SvgParser::parse(const Core::String &cstrSvgText
                                                 ,float fViewportWidth
                                                 ,float fViewportHeight
                                                 ,bool bWithNamespace
                                                 ,bool bWithValidation)
    {
        std::unique_ptr<Xml::IDomParser> domParser = Xml::DomParserBuilder::makeDomParser();

        std::unique_ptr<Xml::IDomDocument> domDocument = domParser->parse(cstrSvgText, bWithNamespace, bWithValidation);
        std::unique_ptr<Xml::IDomEntity> domEntity = domDocument->getRootEntity();

        std::unique_ptr<SvgDocument> pSvgDocument = std::make_unique<SvgDocument>();
        SvgParseState svgParseState {
            .svgDocument = *pSvgDocument.get(),
            .pRootEntity = std::move(domDocument),
            .bWithNamespace = bWithNamespace,
            .bWithValidation = bWithValidation
        };
        SvgParserContext svgParserContext { };
        svgParserContext.setViewportWidth(fViewportWidth);
        svgParserContext.setViewportHeight(fViewportHeight);
        __collectStyleSheets(svgParseState, domEntity.get());
        (void) __parseSvgElement(svgParseState, domEntity.get(), svgParserContext);
        pSvgDocument->resolveGradientReferences();

//        __resolveUses(svgParseState);

        return pSvgDocument;
    }

    std::regex SvgParser::SvgRgbRegex { R"xxx(^\s*rgba?\s*\(\s*([+-]?\d*\.?\d+)\s*(%?)\s*,?\s*([+-]?\d*\.?\d+)\s*\2\s*,?\s*([+-]?\d*\.?\d+)\s*\2\s*[,/]?\s*(?:([+-]?\d*\.?\d+)\s*(%?))?\)\s*$)xxx" };
    std::regex SvgParser::SvgHslRegex { R"xxx(^\s*hsla?\s*\(\s*([+-]?\d*\.?\d+)\s*(deg|grad|rad|turn)?\s*,?\s*([+-]?\d*\.?\d+)\s*(%?)\s*,?\s*([+-]?\d*\.?\d+)\s*(%?)\s*(?:[,/]\s*([+-]?\d*\.?\d+)(%?)\s*)?\)\s*$)xxx" };
    std::regex SvgParser::UrlRegex { R"xxx(url\(#([^)]+)\))xxx"};
    std::regex SvgParser::VarRegex { R"xxx(^\s*var\s*\()xxx"};
    std::regex SvgParser::HexColourRegex { R"xxx(^#([0-9a-f]{3,8})$)xxx", std::regex::icase };
    std::regex SvgParser::TranslateRegex { R"xxx(^\s*translate\(([^)]+)\))xxx", std::regex::icase };
    std::regex SvgParser::ScaleRegex { R"xxx(^\s*scale\(([^)]+)\))xxx", std::regex::icase };
    std::regex SvgParser::RotateRegex { R"xxx(^\s*rotate\(([^)]+)\))xxx", std::regex::icase };
    std::regex SvgParser::SkewXyRegex { R"xxx(^\s*skew([XY])\(([^)]+)\))xxx", std::regex::icase };
    std::regex SvgParser::MatrixRegex { R"xxx(^\s*matrix\(([^)]+)\))xxx", std::regex::icase };
    std::regex SvgParser::DimensionRegex { R"xxx(([+-]?(?:\d+\.?\d*|\.\d+)(?:[eE][+-]?\d+)?)\s*(%|px|pt|pc|cm|mm|in|q|em|ex|ch|rem|vw|vh|vmin|vmax)?)xxx", std::regex::icase };
    // a number as path data writes it: separators optional where unambiguous ("10-5", ".5.5")
    std::regex SvgParser::NumberRegex { R"xxx([+-]?(?:\d+\.?\d*|\.\d+)(?:[eE][+-]?\d+)?)xxx" };
    std::regex SvgParser::DataUriRegex { R"xxx(^data:image/(png|gif|jpe?g|svg\+xml)((?:;(?!base64\s*,)[a-zA-Z0-9\-]+(?:=[^;,]*)?)*)(;base64)?,)xxx", std::regex::icase };


    void SvgParser::__parsePreserveAspectRatio(Xml::IDomEntity *pSvgElement,PreserveAspectRatio &preserveAspectRatio,PreserveAspectRatioMode &preserveAspectRatioMode)
    {
        Core::String strPreserveAspectRatio { };
        if(pSvgElement->tryGetAttribute(PRESERVEASPECTRATIO_ATTRIBUTE, strPreserveAspectRatio)) {
            std::vector<Core::String> parts = strPreserveAspectRatio.regex_split(Core::String::WsRegex);
            if(parts.size() != 1 && parts.size() != 2) throw SvgException("Invalid preserveAspectRatio {}. expected one or two distinct strings, got {}",strPreserveAspectRatio.c_str(), parts.size());
            preserveAspectRatio = ParsePreserveAspectRatio(parts[0]);
            preserveAspectRatioMode = PreserveAspectRatioMode::Meet;
            if(parts.size() == 2) {
                preserveAspectRatioMode = ParsePreserveAspectRatioMode(parts[1]);
            }
        }
    }

    const Svg *SvgParser::__parseSvgElement(SvgParseState &svgParseState, Xml::IDomEntity *pSvgElement, SvgParserContext svgParserContext)
    {
        SvgSvgParams svgSvgParams { SvgContainerParams { SvgEntityParams { .svgDocument = svgParseState.svgDocument } } };

        const DeclaredValues declaredValues = __updateInheritedProperties(svgParseState, pSvgElement, svgParserContext);
        __parseCommonProperties(svgParseState, pSvgElement, svgSvgParams, svgParserContext, declaredValues);

        // paranoia will destroya.  TODO - we don't validate any other tag names like this
        //  the other methods could profit from this
        Core::String strTagName = pSvgElement->getTagName();
        if(strTagName != SVG_NAME) throw SvgException("expected an <svg .../> tag, got {}", strTagName.c_str());

        auto [ x, y, width, height ] = __parseDimensions(svgParseState,pSvgElement);
        svgSvgParams.svgDimensionedParams.x = x;
        svgSvgParams.svgDimensionedParams.y = y;
        svgSvgParams.svgDimensionedParams.width = width;
        svgSvgParams.svgDimensionedParams.height = height;
        Core::String strViewboxAttribute { };
        if(pSvgElement->tryGetAttribute(VIEWBOX_ATTRIBUTE, strViewboxAttribute)) {
            size_t nPos = 0;
            auto viewBox = __parsePoints(strViewboxAttribute, nPos, 4, true);

            svgSvgParams.viewbox = Viewbox {
                .x = viewBox[0],
                .y = viewBox[1],
                .width = viewBox[2],
                .height = viewBox[3]
            };
            // viewBox width/height trumps current viewport width/height, even when supplied by user.
            svgParserContext.setViewportWidth(viewBox[2]);
            svgParserContext.setViewportHeight(viewBox[3]);
        }

        __parsePreserveAspectRatio(pSvgElement,svgSvgParams.preserveAspectRatio,svgSvgParams.preserveAspectRatioMode);

        std::unique_ptr<Svg> pSvg = std::make_unique<Svg>(std::move(svgSvgParams));

        Svg *toReturn = nullptr;
        // we have to set the root svg in the document; just make sure it hasn´t
        //  already been set
        if(!svgParseState.svgDocument.hasRootSvg()) toReturn = svgParseState.svgDocument.setRootSvg(std::move(pSvg));
        else toReturn = svgParserContext.getChildWrangler()->addChildAs(std::move(pSvg));
        __saveEntityToDocument(svgParseState, toReturn);
        svgParserContext.setChildWrangler(toReturn);

        svgParseState.currentSvg.push_back(toReturn);

        const Core::String &cstrId = toReturn->getId();
        std::optional<Core::ScopeExit> scopeGuard;
        if(!Core::String::IsWhitespace(cstrId)) {
            auto [ it, bInserted ] = svgParseState.useReferences.insert(cstrId);
            if(!bInserted) throw SvgException("Found circular reference in use {}; all references in the chain {}", cstrId, Core::String::Join(svgParseState.useReferences, ", "));
            scopeGuard.emplace([&svgParseState,&cstrId](){
                svgParseState.useReferences.erase(cstrId);
            });
        }

        __parseGlobalScope(svgParseState, pSvgElement, svgParserContext);

        return toReturn;
    }

    void SvgParser::__parseGlobalScope(SvgParseState &svgParseState, Xml::IDomEntity *pParentElement, SvgParserContext svgParserContext)
    {
        __updateInheritedProperties(svgParseState, pParentElement, svgParserContext);

        for(std::unique_ptr<Xml::IDomEntity> pChildNode = pParentElement->getFirstChild();
            pChildNode; pChildNode = pChildNode->getNextSibling())
        {
            Core::String strName = pChildNode->getTagName();

            if(strName == DEFINITIONS_NAME) {
                // render may be false if we have <defs...><defs ...>.
                //  so get the value so we can restore it below
                bool bSaveRender = svgParserContext.getRender();
                // things in <defs...> don't render
                svgParserContext.setRender(false);
                // cheeky raii - restore the render status after we parse the children
                Core::ScopeExit scopeExit([&svgParserContext,bSaveRender] () {
                    svgParserContext.setRender(bSaveRender);
                });
                __parseGlobalScope(svgParseState, pChildNode.get(), svgParserContext);
            }
            else if(strName == GROUP_NAME || strName == PATH_NAME || strName == USE_NAME
                 || strName == RECT_NAME || strName == CIRCLE_NAME || strName == ELLIPSE_NAME
                 || strName == POLYGON_NAME || strName == POLYLINE_NAME || strName == MASK_NAME
                 || strName == CLIP_PATH_NAME
                 || strName == TEXT_NAME || strName == IMAGE_NAME || strName == IMAGE_ALTERNATE_NAME 
                 || strName == SVG_NAME) {
                (void) __parseSvgEntity(svgParseState, pChildNode.get(), svgParserContext);
            }
            else if(strName == LINEAR_GRADIENT_NAME || strName == RADIAL_GRADIENT_NAME) {
                (void) __parseGradient(svgParseState, pChildNode.get(), svgParserContext);
            }
            else if(strName == STYLE_NAME) {
                __snagStyle(svgParseState, pChildNode.get());
            }
            else if(m_callback) m_callback(SvgParserStatus::Warning, std::format("Unrecognized tag {}", strName.c_str()));
        }
    }

    std::tuple<std::optional<Dimension>,std::optional<Dimension>,std::optional<Dimension>,std::optional<Dimension>> SvgParser::__parseDimensions(SvgParseState &svgParseState, Xml::IDomEntity *pChildElement)
    {
        std::optional<Dimension> x = {};
        std::optional<Dimension> y = {};
        std::optional<Dimension> width = {};
        std::optional<Dimension> height = {};
        x = __parseDimension(pChildElement, X_ATTRIBUTE);
        y = __parseDimension(pChildElement, Y_ATTRIBUTE);
        width = __parseDimension(pChildElement, WIDTH_ATTRIBUTE);
        height = __parseDimension(pChildElement, HEIGHT_ATTRIBUTE);

        return { x, y, width, height };
    } // std::tuple<std::optional<float>,std::optional<float>,std::optional<float>,std::optional<float>> SvgParser::__parseDimensions(SvgParseState &, Xml::IDomEntity *)


    std::optional<Dimension> SvgParser::__parseDimension(Xml::IDomEntity *pChildElement, const Core::String &cstrAttributeName)
    {
        std::optional<Dimension> toReturn = std::nullopt;
        std::cmatch dimensionMatch;

        Core::String strAttributeValue;
        // disconcerting that <rect width="auto"...> and <rect width=""...> are equivalent in svg
        if(!pChildElement->tryGetAttribute(cstrAttributeName, strAttributeValue)
        || strAttributeValue.trim() == AUTO_VALUE) return toReturn;

        if(std::regex_search(strAttributeValue.c_str(), dimensionMatch, SvgParser::DimensionRegex)) {
            toReturn = Dimension {
                .enumUnits = dimensionMatch[2].str().length() ? ParseDimensionUnits(dimensionMatch[2].str()) : DimensionUnits::Px,
                .fValue = Core::ParseFloat(dimensionMatch[1].str())
            };
        }

        return toReturn;
    }

    SvgPathLengthableParams SvgParser::__checkPathLengthable(SvgParseState &svgParseState, Xml::IDomEntity *pChildElement, SvgParserContext svgParserContext)
    {
        SvgPathLengthableParams svgPathLengthableParams { };
        Core::String strPathLength { };
        if(pChildElement->tryGetAttribute(PATHLENGTH_ATTRIBUTE,strPathLength)) svgPathLengthableParams.pathLength = Core::ParseFloat(strPathLength);
        return svgPathLengthableParams;
    }

    void SvgParser::__snagStyle(SvgParseState &svgParseState, Xml::IDomEntity *pStyleElement)
    {
        Core::String strTagName = Xml::String(pStyleElement->getTagName()).getTranscoded();
        if(strTagName != STYLE_NAME) throw SvgException("expected a <style .../> tag, got {}", strTagName.c_str());
        svgParseState.svgDocument.appendStyle(pStyleElement->getTextContent());
    }

    const SvgEntity *SvgParser::__parseSvgEntity(SvgParseState &svgParseState, Xml::IDomEntity *pChildElement, SvgParserContext svgParserContext)
    {
        const SvgEntity *cpSvgEntity = nullptr;

        // note -- we don't call __updateInheritedProperties here because we don't actually process
        //  a node.  We defer processing depending upon the name of the node.  Not that this could
        //  possibly have happened to me in debugging but if a node has a transform="...", calling
        //  __updateInheritedProperties here applies the transform twice, which is generally no
        //  bueno; I blame designers for not ensuring their matrices are idempotent.

        Core::String strChildElementName = pChildElement->getTagName();

        if(strChildElementName == USE_NAME) {
            cpSvgEntity = __parseUse(svgParseState, pChildElement, svgParserContext);
        }
        else if(strChildElementName == PATH_NAME)
        {
            cpSvgEntity = __parsePath(svgParseState, pChildElement, svgParserContext);
        }
        else if(strChildElementName == GROUP_NAME)
        {
            cpSvgEntity = __parseGroup(svgParseState, pChildElement, svgParserContext);
        }
        else if(strChildElementName == RECT_NAME)
        {
            cpSvgEntity = __parseRect(svgParseState, pChildElement, svgParserContext);
        }
        else if(strChildElementName == CLIP_PATH_NAME)
        {
            cpSvgEntity = __parseClipPath(svgParseState, pChildElement, svgParserContext);
        }
        else if(strChildElementName == MASK_NAME)
        {
            cpSvgEntity = __parseMask(svgParseState, pChildElement, svgParserContext);
        }
        else if(strChildElementName == POLYGON_NAME || strChildElementName == POLYLINE_NAME)
        {
            cpSvgEntity = __parsePolyPoints(svgParseState, pChildElement, svgParserContext, strChildElementName == POLYGON_NAME);
        }
        else if(strChildElementName == ELLIPSE_NAME)
        {
            cpSvgEntity = __parseEllipse(svgParseState, pChildElement, svgParserContext);
        }
        else if(strChildElementName == CIRCLE_NAME)
        {
            cpSvgEntity = __parseCircle(svgParseState, pChildElement, svgParserContext);
        }
        else if(strChildElementName == IMAGE_NAME || strChildElementName == IMAGE_ALTERNATE_NAME)
        {
            cpSvgEntity = __parseImage(svgParseState, pChildElement, svgParserContext);
        }
        else if(strChildElementName == TEXT_NAME)
        {
            cpSvgEntity = __parseText(svgParseState, pChildElement, svgParserContext);
        }
        else if(strChildElementName == SVG_NAME)
        {
            cpSvgEntity = __parseSvgElement(svgParseState, pChildElement, svgParserContext);
        }
        else if(m_callback) m_callback(SvgParserStatus::Warning, std::format("Unrecognized tag {}", strChildElementName));

        return cpSvgEntity;
    } // const SvgEntity *SvgParser::__parseSvgEntity(SvgParseState &, Xml::IDomEntity *)

    void SvgParser::__collectStyleSheets(SvgParseState &svgParseState, Xml::IDomEntity *pElement)
    {
        if(pElement->getTagName() == STYLE_NAME) {
            svgParseState.styleSheet.append(pElement->getTextContent());
            return;
        }
        for(std::unique_ptr<Xml::IDomEntity> pChild = pElement->getFirstChild(); pChild; pChild = pChild->getNextSibling()) {
            __collectStyleSheets(svgParseState, pChild.get());
        }
    }

    DeclaredValues SvgParser::__declaredValues(SvgParseState &svgParseState, Xml::IDomEntity *pElement)
    {
        // the presentation attributes the parser understands; transform stays an attribute
        static const std::array<const Core::String *, 19> PresentationAttributes {
            &COLOR_ATTRIBUTE, &FILL_ATTRIBUTE, &FILL_RULE_ATTRIBUTE, &FILL_OPACITY_ATTRIBUTE,
            &STROKE_ATTRIBUTE, &STROKE_OPACITY_ATTRIBUTE, &STROKE_WIDTH_ATTRIBUTE, &STROKE_LINECAP_ATTRIBUTE,
            &STROKE_LINEJOIN_ATTRIBUTE, &STROKE_MITRELIMIT_ATTRIBUTE, &STROKE_DASHARRAY_ATTRIBUTE,
            &STROKE_DASHOFFSET_ATTRIBUTE, &OPACITY_ATTRIBUTE, &DISPLAY_ATTRIBUTE, &VISIBILITY_ATTRIBUTE,
            &MASK_ATTRIBUTE, &MIX_BLEND_MODE_ATTRIBUTE, &ISOLATION_ATTRIBUTE, &CLIP_PATH_ATTRIBUTE
        };
        // the same names transcoded once for Xerces, rather than on every lookup
        static const std::vector<Xml::String> XercesNames = [] {
            std::vector<Xml::String> names {};
            for(const Core::String *cpstrName : PresentationAttributes) names.emplace_back(cpstrName->c_str());
            return names;
        }();
        DeclaredValues declaredValues { };
        for(size_t nIndex = 0; nIndex < PresentationAttributes.size(); nIndex++) {
            Core::String strValue { };
            if(pElement->tryGetAttribute(XercesNames[nIndex], strValue)) declaredValues[*PresentationAttributes[nIndex]] = strValue.trim();
        }

        Core::String strStyle { };
        (void) pElement->tryGetAttribute(STYLE_ATTRIBUTE, strStyle);
        if(svgParseState.styleSheet.isEmpty() && Core::String::IsWhitespace(strStyle)) return declaredValues;

        Core::String strId { };
        (void) pElement->tryGetAttribute(ID_ATTRIBUTE, strId);
        Core::String strClass { };
        std::vector<Core::String> classes { };
        if(pElement->tryGetAttribute(CLASS_ATTRIBUTE, strClass)) {
            for(Core::String &strName : strClass.regex_split(Core::String::WsRegex)) {
                if(!strName.empty()) classes.push_back(std::move(strName));
            }
        }
        for(auto &[strProperty, strValue] : svgParseState.styleSheet.cascade(pElement->getTagName(), strId, classes, StyleSheet::ParseDeclarations(strStyle))) {
            declaredValues[strProperty] = std::move(strValue);
        }
        return declaredValues;
    }

    template <typename F>
    void SvgParser::__applyIfSet(const DeclaredValues &declaredValues, const Core::String &cstrPropertyName, F &&apply)
    {
        auto it = declaredValues.find(cstrPropertyName);
        if(it == declaredValues.end() || it->second == INHERIT_VALUE) return;
        apply(it->second);
    }

    float SvgParser::__parseLength(const Core::String &cstrLength, const SvgParserContext &svgParserContext)
    {
        const Core::String strLength = cstrLength.trim();
        std::cmatch lengthMatch;
        if(!std::regex_match(strLength.c_str(), lengthMatch, DimensionRegex)) throw SvgException("Invalid length `{}'", strLength.c_str());
        const float fValue = Core::ParseFloat(lengthMatch[1].str());
        Core::String strUnit = lengthMatch[2].str();
        std::transform(strUnit.begin(), strUnit.end(), strUnit.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

        // CSS absolute units at 96px to the inch; font-relative ones against the 16px default
        //  font size, since font-size isn't inherited yet
        const float fWidth = svgParserContext.getViewportWidth();
        const float fHeight = svgParserContext.getViewportHeight();
        if(strUnit.empty() || strUnit == "px") return fValue;
        if(strUnit == "in") return fValue * 96.0f;
        if(strUnit == "pt") return fValue * 96.0f / 72.0f;
        if(strUnit == "pc") return fValue * 16.0f;
        if(strUnit == "cm") return fValue * 96.0f / 2.54f;
        if(strUnit == "mm") return fValue * 96.0f / 25.4f;
        if(strUnit == "q") return fValue * 96.0f / 101.6f;
        if(strUnit == "em" || strUnit == "rem") return fValue * 16.0f;
        if(strUnit == "ex" || strUnit == "ch") return fValue * 8.0f;
        // a percentage of a length that isn't horizontal or vertical: of the viewport's
        //  normalised diagonal (SVG 1.1 §7.10)
        if(strUnit == "%") return fValue / 100.0f * std::sqrt((fWidth * fWidth + fHeight * fHeight) / 2.0f);
        if(strUnit == "vw") return fValue / 100.0f * fWidth;
        if(strUnit == "vh") return fValue / 100.0f * fHeight;
        if(strUnit == "vmin") return fValue / 100.0f * std::min(fWidth, fHeight);
        if(strUnit == "vmax") return fValue / 100.0f * std::max(fWidth, fHeight);
        throw SvgException("Unsupported unit `{}' in length `{}'", strUnit.c_str(), strLength.c_str());
    }

    float SvgParser::__parseUnitInterval(const Core::String &cstrValue)
    {
        // a number or a percentage, clamped to 0-1 (opacities)
        Core::String strValue = cstrValue.trim();
        float fValue = 0.0f;
        if(!strValue.empty() && strValue.back() == '%') fValue = Core::ParseFloat(Core::String(strValue.substr(0, strValue.size() - 1)).trim()) / 100.0f;
        else fValue = Core::ParseFloat(strValue);
        return std::clamp(fValue, 0.0f, 1.0f);
    }

    DeclaredValues SvgParser::__updateInheritedProperties(SvgParseState &svgParseState, Xml::IDomEntity *pElement, SvgParserContext &svgParserContext)
    {
        const DeclaredValues declaredValues = __declaredValues(svgParseState, pElement);

        // great suggestion from claude.  Most of these properties can take the value "inherit,"
        //  so we explicitly check for that and ignore any inherit values.  There are a few exceptions
        __applyIfSet(declaredValues, COLOR_ATTRIBUTE, [&](auto s) {svgParserContext.setColour(__parseColour(svgParseState, s, svgParserContext)); });

        Core::String transformAttribute { };
        if (pElement->tryGetAttribute(TRANSFORM_ATTRIBUTE,transformAttribute)) {
            std::array<float,6> matrix { };
            matrix = __parseTransform(transformAttribute,"transform");
            svgParserContext.multiplyRight(matrix);
        }

        __applyIfSet(declaredValues, FILL_ATTRIBUTE, [&](auto s) {svgParserContext.setFillColour(__parsePaint(svgParseState, s, svgParserContext)); });

        __applyIfSet(declaredValues, FILL_RULE_ATTRIBUTE, [&](auto s) { svgParserContext.setFillRule(ParseFillRule(s)); });

        __applyIfSet(declaredValues, FILL_OPACITY_ATTRIBUTE, [&](auto s) { svgParserContext.setFillOpacity(__parseUnitInterval(s)); });

        __applyIfSet(declaredValues, STROKE_ATTRIBUTE, [&](auto s) { svgParserContext.setStrokeColour(__parsePaint(svgParseState, s, svgParserContext)); });

        __applyIfSet(declaredValues, STROKE_OPACITY_ATTRIBUTE, [&](auto s) { svgParserContext.setStrokeOpacity(__parseUnitInterval(s)); });

        __applyIfSet(declaredValues, STROKE_WIDTH_ATTRIBUTE, [&](auto s) { svgParserContext.setStrokeWidth(__parseLength(s, svgParserContext)); });

        __applyIfSet(declaredValues, STROKE_LINECAP_ATTRIBUTE, [&](auto s) { svgParserContext.setLineCap(ParseLineCap(s)); });

        __applyIfSet(declaredValues, STROKE_LINEJOIN_ATTRIBUTE, [&](auto s) { svgParserContext.setLineJoin(ParseLineJoin(s)); });

        __applyIfSet(declaredValues, STROKE_MITRELIMIT_ATTRIBUTE, [&](auto s) { svgParserContext.setStrokeMiterLimit(Core::ParseFloat(s)); });

        // dash array is a list of lengths, separated by commas and/or whitespace, that controls
        //  when the dash is on and off; "none" sets an empty array
        __applyIfSet(declaredValues, STROKE_DASHARRAY_ATTRIBUTE, [&](const Core::String &s) {
            std::vector<float> dashArray {};
            if(s != NONE_VALUE) {
                static const std::regex Delimiter("[\\s,]+");
                std::sregex_token_iterator iter(s.begin(), s.end(), Delimiter, -1);
                std::sregex_token_iterator end;
                for(; iter != end; ++iter) {
                    if(iter->length()) dashArray.push_back(__parseLength(iter->str(), svgParserContext));
                }
            }
            svgParserContext.setDashArray(dashArray);
        });

        __applyIfSet(declaredValues, STROKE_DASHOFFSET_ATTRIBUTE, [&](auto s) { svgParserContext.setDashOffset(__parseLength(s, svgParserContext)); });

        return declaredValues;
    }

    void SvgParser::__parseCommonProperties(SvgParseState &svgParseState, Xml::IDomEntity *pRenderedElement, SvgEntityParams &params, const SvgParserContext &svgParserContext, const DeclaredValues &declaredValues)
    {
        Core::String strId { };
        (void) pRenderedElement->tryGetAttribute(ID_ATTRIBUTE,strId);
        params.id = strId;

        params.render = svgParserContext.getRender();

        // only the element's own transform: SvgRenderer already composes the parents' transforms
        //  on its way down the tree, so storing the accumulated matrix applied them twice.
        Core::String strTransform { };
        if(pRenderedElement->tryGetAttribute(TRANSFORM_ATTRIBUTE, strTransform)) {
            params.matrix = __parseTransform(strTransform, TRANSFORM_ATTRIBUTE);
        }

        // the computed values -- set on the element itself or inherited -- so the renderer
        //  needn't walk back up the tree
        params.fillColour = svgParserContext.getFillColour();
        params.fillRule = svgParserContext.getFillRule();
        params.fillOpacity = svgParserContext.getFillOpacity();
        params.strokeColour = svgParserContext.getStrokeColour();
        params.strokeOpacity = svgParserContext.getStrokeOpacity();
        params.strokeWidth = svgParserContext.getStrokeWidth();
        params.lineCap = svgParserContext.getLineCap();
        params.lineJoin = svgParserContext.getLineJoin();
        params.miterLimit = svgParserContext.getStrokeMiterLimit();
        params.dashArray = svgParserContext.getDashArray();
        params.dashOffset = svgParserContext.getDashOffset();

        // not inherited, so straight from this element's own declared values
        __applyIfSet(declaredValues, OPACITY_ATTRIBUTE, [&](auto s) { params.opacity = __parseUnitInterval(s); });
        __applyIfSet(declaredValues, MASK_ATTRIBUTE, [&](const Core::String &s) {
            std::cmatch urlMatch;
            if(std::regex_search(s.c_str(), urlMatch, UrlRegex)) params.maskId = Core::String(urlMatch[1].str()).trim();
            else if(s != NONE_VALUE && m_callback) m_callback(SvgParserStatus::Warning, std::format("Unsupported mask `{}'; only url(#id) is", s.c_str()));
        });
        __applyIfSet(declaredValues, CLIP_PATH_ATTRIBUTE, [&](const Core::String &s) {
            std::cmatch urlMatch;
            if(std::regex_search(s.c_str(), urlMatch, UrlRegex)) params.clipPathId = Core::String(urlMatch[1].str()).trim();
            else if(s != NONE_VALUE && m_callback) m_callback(SvgParserStatus::Warning, std::format("Unsupported clip-path `{}'; only url(#id) is", s.c_str()));
        });
        __applyIfSet(declaredValues, MIX_BLEND_MODE_ATTRIBUTE, [&](const Core::String &s) {
            if(std::optional<BlendMode> blendMode = ChercherBlendMode(s)) params.blendMode = blendMode.value();
            else if(m_callback) m_callback(SvgParserStatus::Warning, std::format("Unrecognized mix-blend-mode `{}'; using normal", s.c_str()));
        });
        __applyIfSet(declaredValues, ISOLATION_ATTRIBUTE, [&](const Core::String &s) { params.bIsolate = s == "isolate"; });

        Core::String strClass { };
        pRenderedElement->tryGetAttribute(CLASS_ATTRIBUTE, strClass);
        params.cssClass = strClass;

        Core::String strStyle { };
        pRenderedElement->tryGetAttribute(STYLE_ATTRIBUTE, strStyle);
        params.cssStyle = strStyle;
    }

    void SvgParser::__saveEntityToDocument(SvgParseState &svgParseState,const SvgEntity *cpEntity) {
        auto [ bInserted, bDuplicate ] = svgParseState.svgDocument.saveSvgEntity(cpEntity);
        if(!bInserted && bDuplicate && m_callback) {
            m_callback(SvgParserStatus::Warning, std::format("Encountered duplicate id {}; keeping original, discarding the second", cpEntity->getId().c_str()));
        }
    }

    const Group *SvgParser::__parseGroup(SvgParseState &svgParseState, Xml::IDomEntity *pGroupElement, SvgParserContext svgParserContext)
    {
        SvgGroupParams svgGroupParams { SvgContainerParams { SvgEntityParams { .svgDocument = svgParseState.svgDocument } } };

        const DeclaredValues declaredValues = __updateInheritedProperties(svgParseState, pGroupElement, svgParserContext);
        __parseCommonProperties(svgParseState, pGroupElement, svgGroupParams, svgParserContext, declaredValues);

        std::vector<const SvgEntity *> svgEntities { };
        std::unique_ptr<Group> pGroup = std::make_unique<Group>(std::move(svgGroupParams));
        Group *toReturn = svgParserContext.getChildWrangler()->addChildAs(std::move(pGroup));
        __saveEntityToDocument(svgParseState, toReturn);

        svgParserContext.setChildWrangler(toReturn);

        const Core::String &cstrId = toReturn->getId();
        std::optional<Core::ScopeExit> scopeGuard;
        if(!Core::String::IsWhitespace(cstrId)) {
            auto [ it, bInserted ] = svgParseState.useReferences.insert(cstrId);
            if(!bInserted) throw SvgException("Found circular reference in use {}; all references in the chain {}", cstrId, Core::String::Join(svgParseState.useReferences, ", "));
            scopeGuard.emplace([&svgParseState,&cstrId](){
                svgParseState.useReferences.erase(cstrId);
            });
        }

        for(std::unique_ptr<Xml::IDomEntity> pChildNode = pGroupElement->getFirstChild();
            pChildNode; pChildNode = pChildNode->getNextSibling())
        {
            const SvgEntity *cpSvgEntity = __parseSvgEntity(svgParseState, pChildNode.get(), svgParserContext);
            if(!cpSvgEntity) {
                Core::String strChildElementName = pChildNode->getTagName();
                if(m_callback) m_callback(SvgParserStatus::Warning,std::format("Unrecognized child node {} of group entity",strChildElementName.c_str()));
            }

            svgEntities.push_back(cpSvgEntity);
        }

        return toReturn;
    } // const Group *SvgParser::__parseGroup(DOMDocument *, DOMElement *)

    const Mask *SvgParser::__parseMask(SvgParseState &svgParseState, Xml::IDomEntity *pMaskElement, SvgParserContext svgParserContext)
    {
        SvgMaskParams svgMaskParams { SvgContainerParams { SvgEntityParams { .svgDocument = svgParseState.svgDocument } } };

        // a mask is only ever drawn through the elements that reference it; SvgRenderer skips
        //  Mask entities met on their own
        const DeclaredValues declaredValues = __updateInheritedProperties(svgParseState, pMaskElement, svgParserContext);
        __parseCommonProperties(svgParseState, pMaskElement, svgMaskParams, svgParserContext, declaredValues);

        auto [ x, y, width, height ] = __parseDimensions(svgParseState, pMaskElement);
        svgMaskParams.x = x;
        svgMaskParams.y = y;
        svgMaskParams.width = width;
        svgMaskParams.height = height;
        Core::String strUnits { };
        if(pMaskElement->tryGetAttribute(MASKUNITS_ATTRIBUTE, strUnits)) svgMaskParams.maskUnits = (GradientUnits)ParseGradientUnits(strUnits);
        strUnits = { };
        if(pMaskElement->tryGetAttribute(MASKCONTENTUNITS_ATTRIBUTE, strUnits)) svgMaskParams.maskContentUnits = (GradientUnits)ParseGradientUnits(strUnits);

        std::unique_ptr<Mask> pMask = std::make_unique<Mask>(std::move(svgMaskParams));
        Mask *toReturn = svgParserContext.getChildWrangler()->addChildAs(std::move(pMask));
        __saveEntityToDocument(svgParseState, toReturn);
        svgParserContext.setChildWrangler(toReturn);

        const Core::String &cstrId = toReturn->getId();
        std::optional<Core::ScopeExit> scopeGuard;
        if(!Core::String::IsWhitespace(cstrId)) {
            auto [ it, bInserted ] = svgParseState.useReferences.insert(cstrId);
            if(!bInserted) throw SvgException("Found circular reference in use {}; all references in the chain {}", cstrId, Core::String::Join(svgParseState.useReferences, ", "));
            scopeGuard.emplace([&svgParseState,&cstrId](){
                svgParseState.useReferences.erase(cstrId);
            });
        }

        for(std::unique_ptr<Xml::IDomEntity> pChildNode = pMaskElement->getFirstChild(); pChildNode; pChildNode = pChildNode->getNextSibling()) {
            (void) __parseSvgEntity(svgParseState, pChildNode.get(), svgParserContext);
        }
        return toReturn;
    }

    const ClipPath *SvgParser::__parseClipPath(SvgParseState &svgParseState, Xml::IDomEntity *pClipPathElement, SvgParserContext svgParserContext)
    {
        SvgClipPathParams svgClipPathParams { SvgContainerParams { SvgEntityParams { .svgDocument = svgParseState.svgDocument } } };

        // like a mask, only ever applied through the elements that reference it
        const DeclaredValues declaredValues = __updateInheritedProperties(svgParseState, pClipPathElement, svgParserContext);
        __parseCommonProperties(svgParseState, pClipPathElement, svgClipPathParams, svgParserContext, declaredValues);
        Core::String strUnits { };
        if(pClipPathElement->tryGetAttribute(CLIPPATHUNITS_ATTRIBUTE, strUnits)) svgClipPathParams.clipPathUnits = (GradientUnits)ParseGradientUnits(strUnits);

        std::unique_ptr<ClipPath> pClipPath = std::make_unique<ClipPath>(std::move(svgClipPathParams));
        ClipPath *toReturn = svgParserContext.getChildWrangler()->addChildAs(std::move(pClipPath));
        __saveEntityToDocument(svgParseState, toReturn);
        svgParserContext.setChildWrangler(toReturn);

        const Core::String &cstrId = toReturn->getId();
        std::optional<Core::ScopeExit> scopeGuard;
        if(!Core::String::IsWhitespace(cstrId)) {
            auto [ it, bInserted ] = svgParseState.useReferences.insert(cstrId);
            if(!bInserted) throw SvgException("Found circular reference in use {}; all references in the chain {}", cstrId, Core::String::Join(svgParseState.useReferences, ", "));
            scopeGuard.emplace([&svgParseState,&cstrId](){
                svgParseState.useReferences.erase(cstrId);
            });
        }

        for(std::unique_ptr<Xml::IDomEntity> pChildNode = pClipPathElement->getFirstChild(); pChildNode; pChildNode = pChildNode->getNextSibling()) {
            (void) __parseSvgEntity(svgParseState, pChildNode.get(), svgParserContext);
        }
        return toReturn;
    }

    const Path *SvgParser::__parsePath(SvgParseState &svgParseState, Xml::IDomEntity *pPathElement, const SvgParserContext &svgParserContextIn)
    {
        SvgParserContext svgParserContext = svgParserContextIn;

        SvgPathParams svgPathParams { SvgEntityParams { .svgDocument = svgParseState.svgDocument } };

        const DeclaredValues declaredValues = __updateInheritedProperties(svgParseState, pPathElement, svgParserContext);
        __parseCommonProperties(svgParseState, pPathElement, svgPathParams, svgParserContext, declaredValues);

        Core::String strD { };
        if (!pPathElement->tryGetAttribute(D_ATTRIBUTE,strD)) throw SvgException("Missing d attribute on pathNode");

        SvgDParser svgDParser { };
        auto [ pathMoves, points ] = svgDParser.parseD(strD);

        svgPathParams.points = std::move(points);
        svgPathParams.pathMoves = std::move(pathMoves);
        std::unique_ptr<Path> pPath = std::make_unique<Path>(std::move(svgPathParams));

        Path *toReturn = svgParserContext.getChildWrangler()->addChildAs(std::move(pPath));
        __saveEntityToDocument(svgParseState, toReturn);

        return toReturn;
    } // const Path *SvgParser::__parsePath(DOMDocument *, DOMElement *)

    const Path *SvgParser::__parsePolyPoints(SvgParseState &svgParseState, Xml::IDomEntity *pElement, SvgParserContext svgParserContext, bool bClosed)
    {
        SvgPathParams svgPathParams { SvgEntityParams { .svgDocument = svgParseState.svgDocument } };

        const DeclaredValues declaredValues = __updateInheritedProperties(svgParseState, pElement, svgParserContext);
        __parseCommonProperties(svgParseState, pElement, svgPathParams, svgParserContext, declaredValues);

        // numbers separated by whitespace and/or a comma; anything else is an error, and as
        //  the spec asks, the points before it are still drawn
        Core::String strPoints { };
        (void) pElement->tryGetAttribute(POINTS_ATTRIBUTE, strPoints);
        std::vector<float> coordinates { };
        size_t nPos = 0;
        while(nPos < strPoints.size()) {
            const unsigned char c = strPoints[nPos];
            if(std::isspace(c) || c == ',') {
                nPos++;
                continue;
            }
            std::cmatch numberMatch;
            if(!std::regex_search(strPoints.c_str() + nPos, numberMatch, NumberRegex, std::regex_constants::match_continuous)) {
                if(m_callback) m_callback(SvgParserStatus::Warning, std::format("Invalid points `{}' at {}; drawing the points before it", strPoints.c_str(), nPos));
                break;
            }
            coordinates.push_back(Core::ParseFloat(numberMatch[0].str()));
            nPos += numberMatch[0].length();
        }
        // an odd coordinate out is dropped (SVG 2: draw up to the last complete pair)
        if(coordinates.size() % 2) {
            if(m_callback) m_callback(SvgParserStatus::Warning, std::format("Odd number of coordinates in points `{}'; ignoring the last", strPoints.c_str()));
            coordinates.pop_back();
        }

        // M p0 L p1 ... [Z]; fewer than two points draws nothing
        if(coordinates.size() >= 4) {
            svgPathParams.pathMoves.push_back(PathMove::MoveTo);
            for(size_t n = 2; n < coordinates.size(); n += 2) svgPathParams.pathMoves.push_back(PathMove::LineTo);
            if(bClosed) svgPathParams.pathMoves.push_back(PathMove::ClosePath);
            svgPathParams.points = std::move(coordinates);
        }

        std::unique_ptr<Path> pPath = bClosed
            ? std::unique_ptr<Path>(std::make_unique<Polygon>(std::move(svgPathParams)))
            : std::unique_ptr<Path>(std::make_unique<Polyline>(std::move(svgPathParams)));
        Path *toReturn = svgParserContext.getChildWrangler()->addChildAs(std::move(pPath));
        __saveEntityToDocument(svgParseState, toReturn);
        return toReturn;
    }

    const Use *SvgParser::__parseUse(SvgParseState &svgParseState, Xml::IDomEntity *pUseElement, SvgParserContext svgParserContext)
    {
        SvgUseParams svgUseParams { SvgContainerParams { SvgEntityParams { .svgDocument = svgParseState.svgDocument } } };

        // when we encounter use, we create the use node itself, set the child wrangler to
        //  it and instance it as we go, being mindful to check for circular references
        //  in the expansion
        const DeclaredValues declaredValues = __updateInheritedProperties(svgParseState, pUseElement, svgParserContext);
        __parseCommonProperties(svgParseState, pUseElement, svgUseParams, svgParserContext, declaredValues);

        Core::String strHref = __retrieveHref(svgParseState, pUseElement);
        if (Core::String::IsWhitespace(strHref)) throw SvgException("Use node has no xlink:href attribute");

        auto [ x, y, width, height ] = __parseDimensions(svgParseState,pUseElement);

        svgUseParams.svgDimensionedParams.x = x;
        svgUseParams.svgDimensionedParams.y = y;
        svgUseParams.svgDimensionedParams.width = width;
        svgUseParams.svgDimensionedParams.height = height;
//        svgUseParams.cpSvgEntity = cpSvgEntity;
        std::unique_ptr<Use> pUse = std::make_unique<Use>(std::move(svgUseParams));

        Use *toReturn = svgParserContext.getChildWrangler()->addChildAs(std::move(pUse));
        __saveEntityToDocument(svgParseState, toReturn);

        // now look up the node the use is referencing.  we'll completely parse it again,
        //  whether we've seen it or not
        strHref.trim_start_in_place('#');

        // we want to "instance" the use.  Look up the dom node this use node is referencing
        std::shared_ptr<Xml::IDomEntity> pUsedElement = svgParseState.pRootEntity->getElementById(Xml::String(strHref).c_str());
        if (pUsedElement != nullptr) {
            // first thing, remember we've tracked this href; if it doesn't insert, it
            //  means we have visited that node before whilst resolving a use and it is
            //  therefore a circular reference
            auto [ it, bInserted ] = svgParseState.useReferences.insert(strHref);
            if(!bInserted) throw SvgException("Found circular reference in use {}; all references in the chain {}", strHref.c_str(), Core::String::Join(svgParseState.useReferences, ", ").c_str());
            Core::ScopeExit se([&svgParseState,&strHref](){
                svgParseState.useReferences.erase(strHref);
            });

            // we'll set the use node as the child wrangler so this child node is assigned
            //  to it.
            svgParserContext.setChildWrangler(toReturn);

            // we don't care about the result here, Because we sat use as the
            //  child wrangler, the child entity will belong to the use
            (void)__parseSvgEntity(svgParseState, pUsedElement.get(), svgParserContext);
        }

        return toReturn;
    } // const Use *SvgParser::__parseUse(SvgParseState &, Xml::IDomEntity *)

    const Rect *SvgParser::__parseRect(SvgParseState &svgParseState, Xml::IDomEntity *pRectElement, SvgParserContext svgParserContext)
    {
        SvgRectParams svgRectParams { SvgEntityParams { .svgDocument = svgParseState.svgDocument } };

        const DeclaredValues declaredValues = __updateInheritedProperties(svgParseState, pRectElement, svgParserContext);
        __parseCommonProperties(svgParseState, pRectElement, svgRectParams, svgParserContext, declaredValues);

        auto [ x, y, width, height ] = __parseDimensions(svgParseState, pRectElement);

        auto rx = __parseDimension(pRectElement, RX_ATTRIBUTE);
        auto ry = __parseDimension(pRectElement, RY_ATTRIBUTE);

        svgRectParams.svgDimensionedParams.x = x;
        svgRectParams.svgDimensionedParams.y = y;
        svgRectParams.svgDimensionedParams.width = width;
        svgRectParams.svgDimensionedParams.height = height;
        svgRectParams.rx = rx;
        svgRectParams.ry = ry;

        std::unique_ptr<Rect> pRect = std::make_unique<Rect>(std::move(svgRectParams));
        Rect *toReturn = svgParserContext.getChildWrangler()->addChildAs(std::move(pRect));
        __saveEntityToDocument(svgParseState, toReturn);
        return toReturn;
    }

    const Ellipse *SvgParser::__parseEllipse(SvgParseState &svgParseState, Xml::IDomEntity *pEllipseElement, SvgParserContext svgParserContext)
    {
        SvgEllipseParams svgEllipseParams { SvgEntityParams { .svgDocument = svgParseState.svgDocument } };
        svgEllipseParams.cx = __parseDimension(pEllipseElement, CX_ATTRIBUTE);
        svgEllipseParams.cy = __parseDimension(pEllipseElement, CY_ATTRIBUTE);
        svgEllipseParams.rx = __parseDimension(pEllipseElement, RX_ATTRIBUTE);
        svgEllipseParams.ry = __parseDimension(pEllipseElement, RY_ATTRIBUTE);
        svgEllipseParams.svgPathLengthableParams = __checkPathLengthable(svgParseState, pEllipseElement,svgParserContext);

        const DeclaredValues declaredValues = __updateInheritedProperties(svgParseState, pEllipseElement, svgParserContext);
        __parseCommonProperties(svgParseState, pEllipseElement, svgEllipseParams, svgParserContext, declaredValues);

        std::unique_ptr<Ellipse> pEllipse = std::make_unique<Ellipse>(std::move(svgEllipseParams));
        Ellipse *toReturn = svgParserContext.getChildWrangler()->addChildAs(std::move(pEllipse));
        __saveEntityToDocument(svgParseState, toReturn);
        return toReturn;
    }

    const Circle *SvgParser::__parseCircle(SvgParseState &svgParseState, Xml::IDomEntity *pCircleElement, SvgParserContext svgParserContext)
    {
        SvgCircleParams svgCircleParams { SvgEntityParams { .svgDocument = svgParseState.svgDocument } };
        svgCircleParams.cx = __parseDimension(pCircleElement, CX_ATTRIBUTE);
        svgCircleParams.cy = __parseDimension(pCircleElement, CY_ATTRIBUTE);
        svgCircleParams.r = __parseDimension(pCircleElement, R_ATTRIBUTE);
        svgCircleParams.svgPathLengthableParams = __checkPathLengthable(svgParseState, pCircleElement,svgParserContext);

        const DeclaredValues declaredValues = __updateInheritedProperties(svgParseState, pCircleElement, svgParserContext);
        __parseCommonProperties(svgParseState, pCircleElement, svgCircleParams, svgParserContext, declaredValues);

        std::unique_ptr<Circle> pCircle = std::make_unique<Circle>(std::move(svgCircleParams));
        Circle *toReturn = svgParserContext.getChildWrangler()->addChildAs(std::move(pCircle));
        __saveEntityToDocument(svgParseState, toReturn);
        return toReturn;
    }

    const Image *SvgParser::__parseImage(SvgParseState &svgParseState, Xml::IDomEntity *pImageElement, SvgParserContext svgParserContext)
    {
        SvgImageParams svgImageParams { SvgEntityParams { .svgDocument = svgParseState.svgDocument } };

        const DeclaredValues declaredValues = __updateInheritedProperties(svgParseState, pImageElement, svgParserContext);
        __parseCommonProperties(svgParseState, pImageElement, svgImageParams, svgParserContext, declaredValues);

        auto [ x, y, width, height ] = __parseDimensions(svgParseState, pImageElement);

        svgImageParams.svgDimensionedParams.x = x;
        svgImageParams.svgDimensionedParams.y = y;
        svgImageParams.svgDimensionedParams.width = width;
        svgImageParams.svgDimensionedParams.height = height;

        svgImageParams.href = __retrieveHref(svgParseState, pImageElement);
        svgImageParams.hrefKind = HrefKind::Url;
        std::cmatch dataUriMatch;
        if(std::regex_search(svgImageParams.href.c_str(), dataUriMatch, SvgParser::DataUriRegex)) {
            svgImageParams.imageType = dataUriMatch[1].str();
            Core::String strKvps = dataUriMatch[2].str();
            auto kvps = strKvps.split(';');
            for(const auto &kvp : kvps) {
                auto parts = kvp.split('=', true, 2);
                const Core::String &cstrKey = parts[0].trim();
                if(cstrKey == CHARSET_VALUE) {
                    const Core::String &cstrValue = parts[1].trim();
                    if(cstrValue != UTF8_VALUE) throw SvgException("Invalid value for charset in a data uri; only {} is supported, not {}", UTF8_VALUE, cstrValue);
                    if(!Core::String::IsEmpty(svgImageParams.characterEncoding)) throw SvgException("charset is multiply defined {}; previous value was {}", cstrValue, svgImageParams.characterEncoding);
                    svgImageParams.characterEncoding = cstrValue;
                }
            }
            // if ";base64" is specified, decode the href.
            if(dataUriMatch[3].str().length()) {
                Core::String strEncoded = svgImageParams.href.substr(dataUriMatch[0].str().length());
                auto decodedBytes = Core::base64Decode(strEncoded);
                std::string bytes(decodedBytes.begin(), decodedBytes.end());
                svgImageParams.href = bytes;
                svgImageParams.hrefKind = HrefKind::DecodedBinary;
            }
            else {
                // strip off the data uri
                svgImageParams.href = svgImageParams.href.substr(dataUriMatch[0].str().length());
                svgImageParams.hrefKind = HrefKind::LiteralText;
            }
        }

        __parsePreserveAspectRatio(pImageElement,svgImageParams.preserveAspectRatio,svgImageParams.preserveAspectRatioMode);
        Core::String strCrossOrigin { };
        if (pImageElement->tryGetAttribute(CROSSORIGIN_ATTRIBUTE,strCrossOrigin)) {
            svgImageParams.crossOrigin = ParseCrossOrigin(strCrossOrigin);
        }

        Core::String strDecoding { };
        if (pImageElement->tryGetAttribute(DECODING_ATTRIBUTE,strDecoding)
         && strDecoding != AUTO_VALUE) {
            svgImageParams.decoding = ParseDecoding(strDecoding);
        }

        Core::String strFetchPriority { };
        if (pImageElement->tryGetAttribute(FETCHPRIORITY_ATTRIBUTE,strFetchPriority)
         && strFetchPriority != AUTO_VALUE) {
            svgImageParams.fetchPriority = ParseFetchPriority(strFetchPriority);
        }

        std::unique_ptr<Image> pImage = std::make_unique<Image>(std::move(svgImageParams));
        Image *toReturn = svgParserContext.getChildWrangler()->addChildAs(std::move(pImage));
        __saveEntityToDocument(svgParseState, toReturn);
        return toReturn;
    }

    const Text *SvgParser::__parseText(SvgParseState &svgParseState, Xml::IDomEntity *pTextElement, SvgParserContext svgParserContext)
    {
        SvgTextParams svgTextParams { SvgEntityParams { .svgDocument = svgParseState.svgDocument } };

        const DeclaredValues declaredValues = __updateInheritedProperties(svgParseState, pTextElement, svgParserContext);
        __parseCommonProperties(svgParseState, pTextElement, svgTextParams, svgParserContext, declaredValues);

        __parseCommonTextProperties(svgTextParams,pTextElement);

        Core::String strFontSize { };
        if (pTextElement->tryGetAttribute(FONTSIZE_ATTRIBUTE,strFontSize)) {
            strFontSize.trim_in_place();
            try {
                auto result = Core::ParseFloatPrefix(strFontSize);
                if(result.first < 0) throw SvgException("Font Size can't be negative!");
                svgTextParams.fontSize = result.first;
                unsigned nCount = result.second;
                if(nCount != strFontSize.size()) {
                    svgTextParams.fontSizeUnits = ChercherDimensionUnits(strFontSize.c_str() + nCount);
                }
            }
            catch(std::exception &e) {
                // could be the float doesn't parse, let's handle it gracefully.
                if(m_callback) m_callback(SvgParserStatus::Warning, std::format("Error parsing font size {}", e.what()));
            }
        }

        Core::String strFontFamily { };
        if (pTextElement->tryGetAttribute(FONTFAMILY_ATTRIBUTE,strFontFamily)) {
            svgTextParams.fontFamily = strFontFamily;
        }

        Core::String strFontStyle { };
        if (pTextElement->tryGetAttribute(FONTSTYLE_ATTRIBUTE,strFontStyle)) {
            svgTextParams.fontStyle = ChercherFontStyle(strFontStyle);
        }

        Core::String strTextAnchor { };
        if (pTextElement->tryGetAttribute(TEXTANCHOR_ATTRIBUTE,strTextAnchor)) {
            svgTextParams.textAnchor = ChercherTextAnchor(strTextAnchor);
        }

        svgTextParams.rawText = __parseRawText(pTextElement);

        std::unique_ptr<Text> pText = std::make_unique<Text>(std::move(svgTextParams));
        Text *toReturn = svgParserContext.getChildWrangler()->addChildAs(std::move(pText));
        __saveEntityToDocument(svgParseState, toReturn);

        return toReturn;
    }

    void SvgParser::__parseCommonTextProperties(SvgTSpanParams &svgTSpanParams, Xml::IDomEntity *pTextElement)
    {
        Core::String strX { };
        size_t nPosition = 0;
        if (pTextElement->tryGetAttribute(X_ATTRIBUTE,strX)) {
            svgTSpanParams.x = __parsePoints(strX, nPosition, 0, false, true);
        }

        Core::String strY { };
        nPosition = 0;
        if (pTextElement->tryGetAttribute(Y_ATTRIBUTE,strY)) {
            svgTSpanParams.y = __parsePoints(strY, nPosition, 0, false, true);
        }

        Core::String strDx { };
        nPosition = 0;
        if (pTextElement->tryGetAttribute(DX_ATTRIBUTE,strDx)) {
            svgTSpanParams.dx = __parsePoints(strDx, nPosition, 0, false, true);
        }

        Core::String strDy { };
        nPosition = 0;
        if (pTextElement->tryGetAttribute(DY_ATTRIBUTE,strDy)) {
            svgTSpanParams.dy = __parsePoints(strDy, nPosition, 0, false, true);
        }

        Core::String strRotate { };
        nPosition = 0;
        if (pTextElement->tryGetAttribute(ROTATE_ATTRIBUTE,strRotate)) {
            svgTSpanParams.rotate = __parsePoints(strRotate, nPosition, 0, false, true);
        }

        Core::String strLengthAdjust { };
        if (pTextElement->tryGetAttribute(LENGTHADJUST_ATTRIBUTE,strLengthAdjust)) {
            svgTSpanParams.lengthAdjust = ParseLengthAdjust(strLengthAdjust);
        }

        Core::String strTextLength { };
        if (pTextElement->tryGetAttribute(TEXTLENGTH_ATTRIBUTE,strTextLength)) {
            auto rit = strTextLength.rbegin();
            // Skip trailing whitespace first (handles "50%", "50 %", and "50   ")
            while (rit != strTextLength.rend() && std::isspace(static_cast<unsigned char>(*rit)))
                ++rit;

            if (rit != strTextLength.rend() && *rit == '%')
            {
                svgTSpanParams.textLengthPercentage = true;
                ++rit;
                // Skip any whitespace between the number and the '%' too, e.g. "50   %"
                while (rit != strTextLength.rend() && std::isspace(static_cast<unsigned char>(*rit)))
                    ++rit;
            }

            strTextLength.erase(rit.base(), strTextLength.end());
            svgTSpanParams.textLength = ParseFloat(strTextLength);
        }
    }


    RawText SvgParser::__parseRawText(Xml::IDomEntity *pTextElement)
    {
        RawTextParams rawTextParams;
        std::unique_ptr<Xml::IDomEntity> pChild = pTextElement->getFirstChild(false);
        while(pChild) {
            if(pChild->getDomEntityType() == Xml::DomEntityType::Text) rawTextParams.texts.push_back(pChild->getTextContent());
            else rawTextParams.texts.push_back(__parseTSpan(pChild.get()));
            pChild = pChild->getNextSibling(false);
        }

        RawText rawText { std::move(rawTextParams) };
        return rawText;
    }

    std::unique_ptr<TSpan> SvgParser::__parseTSpan(Xml::IDomEntity *pTextElement)
    {
        SvgTSpanParams svgTSpanParams {};

        __parseCommonTextProperties(svgTSpanParams,pTextElement);
        svgTSpanParams.rawText = __parseRawText(pTextElement);

        std::unique_ptr<TSpan> pTSpan = std::make_unique<TSpan>(std::move(svgTSpanParams));

        return pTSpan;
    }

    Core::String SvgParser::__retrieveHref(SvgParseState &svgParseState, Xml::IDomEntity *pElement)
    {
        // spec drops http://www.w3.org/1999/xlink namespace on href but still supports it.
        // href > xlink:href but fall back to xlink::href if need be
        Core::String strHref { };
        (void) pElement-> tryGetAttribute(HREF_ATTRIBUTE, strHref);
        Core::String strXlinkHref;
        if(svgParseState.bWithNamespace) {
            (void) pElement->tryGetAttributeNS(HREF_ATTRIBUTE, XLINK_NAMESPACE, strXlinkHref);
        }
        else {
            (void) pElement->tryGetAttribute(XLINK_HREF_ATTRIBUTE, strXlinkHref);
        }
        if(!Core::String::IsWhitespace(strHref)) {
            if(!Core::String::IsWhitespace(strXlinkHref) && strHref != strXlinkHref && m_callback) {
                m_callback(SvgParserStatus::Warning, std::format("Node has both xlink:href `{}' and href `{}' attributes that disagree; per the standard, `href' wins.", strXlinkHref.c_str(), strHref.c_str()));
            }
            return strHref;
        }
        return strXlinkHref;
    }

    std::vector<float> SvgParser::__parsePoints(const Core::String &d, size_t &pos, size_t nMaximumPoints, bool bExactMaximum, bool slurp)
    {
        std::vector<float> points { };
        unsigned nextIndex = pos;
        while (nextIndex < d.length())
        {
            if (d[nextIndex] == ' ' || d[nextIndex] == ',')
            {
                nextIndex++;
                continue;
            }

            if (std::isalpha(d[nextIndex]))
            {
                pos = nextIndex;
                break;
            }

            pos = nextIndex;
            while (nextIndex < d.length() &&
                   d[nextIndex] != ' ' &&
                   d[nextIndex] != ',' &&
                   !std::isalpha(d[nextIndex])) {
                nextIndex++;
            }
            if (pos == nextIndex) throw SvgException("LogicError {}, {} in `{}`",pos, nextIndex, d.c_str());
            points.push_back(Core::ParseFloat(d, pos, nextIndex - pos));
            if(nMaximumPoints && points.size() > nMaximumPoints) throw SvgException("Too many points supplied (maximum <= {}): {}",d, nMaximumPoints);

            pos = nextIndex;
        }

        if(bExactMaximum && nMaximumPoints && points.size() != nMaximumPoints) throw SvgException("Expected exactly {} points; wound up with {}", nMaximumPoints, points.size());

        if(slurp && nextIndex != d.length()) throw SvgException("Expected to extract points from the entire string, but wound up at index {} of {}",nextIndex, d.length());

        return points;
    }

    SvgPaint SvgParser::__parsePaint(SvgParseState &svgParseState, const Core::String &cstrColour, SvgParserContext &svgParserContext)
    {
        Core::String strColour = cstrColour.trim();

        std::cmatch urlMatch;
        if (std::regex_search(strColour.c_str(), urlMatch, UrlRegex))
        {
            SvgPaint svgPaint(SvgColourType::Gradient, urlMatch[1].str());
            // url(#id) may be followed by a fallback, used when the reference doesn't resolve
            const Core::String strFallback = Core::String(urlMatch.suffix().str()).trim();
            if(!Core::String::IsEmpty(strFallback)) {
                if(strFallback == NONE_VALUE) svgPaint.setFallback(SvgColour { SvgColourType::None });
                else svgPaint.setFallback(__parseColour(svgParseState, strFallback, svgParserContext));
            }
            return svgPaint;
        }
        if(strColour.c_str() == NONE_VALUE) {
            return SvgPaint(SvgColourType::None);
        }

        SvgColour svgColour = __parseColour(svgParseState, strColour, svgParserContext);
        return SvgPaint(svgColour.getSvgColourType(), "", svgColour);
    }

    SvgColour SvgParser::__parseColour(SvgParseState &svgParseState, const Core::String &cstrColour, SvgParserContext &svgParserContext)
    {
        Core::String strColour = cstrColour.trim();

        // NOTE - not part of the svg spec, this is actually css, and it turns out this could be
        //  used anywhere.  for now, we explicitly handle the color / fallback case
        std::cmatch varMatch;
        if (std::regex_search(strColour.c_str(), varMatch, VarRegex))
        {
            std::unique_ptr<DataStructures::Tree::GeneralTree<Core::String>> pVarTree = __parseVar(strColour);
            auto roots = pVarTree->getRoots();
            if(roots.size() > 1) throw SvgException("Internal error, found {} roots in {}", roots.size(), strColour.c_str());
            DataStructures::Tree::GeneralTreeNode<Core::String> *pRoot = roots[0];
            const std::vector<DataStructures::Tree::GeneralTreeNode<Core::String> *> &children = pRoot->getChildren();
            if(children.size() > 2) throw SvgException("Internal error, found {} children in {}", children.size(), strColour.c_str());

            if(children[0]->getChildren().size()
            || (children.size() == 2 && children[1]->getChildren().size())) throw SvgException("Internal error; nested var's are not supported in {}", strColour.c_str());
            SvgColour primary = __parseColour(svgParseState, children[0]->getData(), svgParserContext);
            if(children.size() == 2) {
                std::unique_ptr<SvgColour> pFallback = std::make_unique<SvgColour>(__parseColour(svgParseState, children[1]->getData(), svgParserContext));
                primary.setFallback(std::move(pFallback));
            }
            return primary;
        }

        // check for rgba?
        std::cmatch svgRgbMatch;
        if (std::regex_search(strColour.c_str(), svgRgbMatch, SvgRgbRegex))
        {
            Core::String strRed = svgRgbMatch[1].str();
            Core::String strRgbPercent = svgRgbMatch[2].str();
            Core::String strGreen = svgRgbMatch[3].str();
            Core::String strBlue = svgRgbMatch[4].str();
            Core::String strAlpha = svgRgbMatch[5].str();
            Core::String strAlphaPercent = svgRgbMatch[6].str();

            // channels 0-255 or 0%-100%, alpha 0-1 or 0%-100%, all clamped as in CSS; worked out
            //  in floats, since an alpha of 0.5 doesn't survive a uint8_t.
            const bool bPercent = !Core::String::IsWhitespace(strRgbPercent.c_str());
            auto channel = [bPercent](const Core::String &cstrValue) -> uint8_t {
                float fValue = Core::ParseFloat(cstrValue);
                if(bPercent) fValue = fValue / 100.0f * 255.0f;
                return static_cast<uint8_t>(std::lround(std::clamp(fValue, 0.0f, 255.0f)));
            };

            // stored as B, G, R, A
            std::array<uint8_t,4> pBgra { channel(strBlue), channel(strGreen), channel(strRed), 0xff };
            const bool bAlpha = !Core::String::IsEmpty(strAlpha);
            if(bAlpha) {
                float fAlpha = Core::ParseFloat(strAlpha);
                if(!Core::String::IsEmpty(strAlphaPercent)) fAlpha /= 100.0f;
                pBgra[3] = static_cast<uint8_t>(std::lround(std::clamp(fAlpha, 0.0f, 1.0f) * 255.0f));
            }
            return SvgColour(bAlpha ? SvgColourType::BgrA : SvgColourType::Bgr, -1, std::move(pBgra));
        }

        // check for hsla
        std::cmatch svgHslMatch;
        if (std::regex_search(strColour.c_str(), svgHslMatch, SvgHslRegex))
        {
            Core::String strHue =               svgHslMatch[1].str();
            Core::String strHueUnits =          svgHslMatch[2].str();
            Core::String strSaturation =        svgHslMatch[3].str();
            Core::String strLightness =         svgHslMatch[5].str();
            Core::String strAlpha =             svgHslMatch[7].str();
            Core::String strAlphaPercent =      svgHslMatch[8].str();

            // hue in degrees, saturation and lightness scaled to 0-1: what hslaToRgba and the
            //  colour resolvers expect.
            float fHue = Core::ParseFloat(strHue);
            if(strHueUnits == GRADIANS_UNIT) fHue *= 360.0f / 400.0f;
            else if(strHueUnits == RADIANS_UNIT) fHue *= 180.0f / std::numbers::pi_v<float>;
            else if(strHueUnits == TURN_UNIT) fHue *= 360.0f;
            else if(strHueUnits.length() && strHueUnits != DEGREES_UNIT) throw SvgException("Unrecognized units on hue in hsl(a) value {}: {}", strColour.c_str(), strHueUnits.c_str());

            // CSS Color 4: a number without % means the same percentage (hsl(120 100 50) == hsl(120 100% 50%))
            const float fSaturation = std::clamp(Core::ParseFloat(strSaturation), 0.0f, 100.0f) / 100.0f;
            const float fLightness = std::clamp(Core::ParseFloat(strLightness), 0.0f, 100.0f) / 100.0f;

            const bool bAlpha = !Core::String::IsEmpty(strAlpha);
            float fAlpha = 1.0f;
            if(bAlpha) {
                fAlpha = Core::ParseFloat(strAlpha);
                if(!Core::String::IsEmpty(strAlphaPercent)) fAlpha /= 100.0f;
                fAlpha = std::clamp(fAlpha, 0.0f, 1.0f);
            }

            std::array<float,4> pHsla { fHue, fSaturation, fLightness, fAlpha };
            return SvgColour(bAlpha ? SvgColourType::HslA : SvgColourType::Hsl, -1, std::nullopt, std::move(pHsla));
        }

        if (strColour == "currentColor") {
            return SvgColour(svgParserContext.getColour());
        }

        if (strColour.starts_with("--color"))
        {
            int index = Core::ParseInteger(strColour.substr(std::strlen("--color")));
            return SvgColour(SvgColourType::Cpal, index);
        }

        if (StandardColours::Colours.contains(strColour)) return SvgColour(SvgColourType::Bgr, -1, StandardColours::BgrOf(strColour));
        
        std::cmatch colourMatch;
        if (!std::regex_search(strColour.c_str(), colourMatch, HexColourRegex)) throw SvgException("Couldn't recognize hex value `{}'",strColour.c_str());
        if((colourMatch[1].length() != 3)
        && (colourMatch[1].length() != 4)
        && (colourMatch[1].length() != 6)
        && (colourMatch[1].length() != 8)) throw SvgException(std::format("BGR/BGRA must be comprised of 3, 4, 6, or 8 colours, not {}",colourMatch[1].length()));

        // #rgb / #rgba: one digit per channel, doubled (#abc == #aabbcc); #rrggbb / #rrggbbaa: two
        const std::string strHex = colourMatch[1].str();
        const size_t nDigits = strHex.length() <= 4 ? 1 : 2;
        const size_t nChannels = strHex.length() / nDigits;
        std::array<uint8_t,4> rgba { 0, 0, 0, 0xff };
        for(size_t nIndex = 0; nIndex < nChannels; nIndex++) {
            uint8_t nValue = (uint8_t)Core::ParseInteger(strHex.substr(nIndex * nDigits, nDigits), BASE16);
            rgba[nIndex] = nDigits == 1 ? static_cast<uint8_t>((nValue << 4) | nValue) : nValue;
        }

        // stored as B, G, R, A, like StandardColours::BgrOf and rgb()
        std::array<uint8_t,4> bgra { rgba[2], rgba[1], rgba[0], rgba[3] };
        return SvgColour(nChannels == 4 ? SvgColourType::BgrA : SvgColourType::Bgr, -1, std::move(bgra));
    } // SvgColour SvgParser::__parseColour(SvgParseState &, const Core::String &, SvgParserContext &)

    std::unique_ptr<DataStructures::Tree::GeneralTree<Core::String>> SvgParser::__parseVar(const Core::String &cstrVar)
    {
        auto stringNodeComparator = [](const Core::String &cstrLeft, const Core::String &cstrRight) -> bool {
            return cstrLeft < cstrRight;
        };

        std::unique_ptr<DataStructures::Tree::GeneralTree<Core::String>> pGeneralTree = std::make_unique<DataStructures::Tree::GeneralTree<Core::String>>(stringNodeComparator);
        size_t nPos = 0;
        __doParseVar(cstrVar, nPos, pGeneralTree.get(), nullptr);
        return pGeneralTree;
    }

    void SvgParser::__doParseVar(const Core::String &cstrVar, size_t &nPos, DataStructures::Tree::GeneralTree<Core::String> *pGeneralTree, DataStructures::Tree::GeneralTreeNode<Core::String> *pGeneralTreeNode)
    {
        // harsh - to be here we would've had to see something starting with "var("
        std::cmatch varMatch;
        if (!std::regex_search(cstrVar.c_str(), varMatch, VarRegex)) throw SvgException("Called __parseVar on something not a var {}", cstrVar.c_str());

        size_t pos = varMatch[0].str().length();
        size_t startingPos = pos;
        bool bFoundComma = false;
        unsigned nParenthesisCount = 0;
        bool bIsString = false;
        bool bBackslash = false;
        while(pos < cstrVar.length()) {
            if(bIsString) {
                if(cstrVar[pos] == '"') {
                    bIsString = false;
                    pos++;
                    continue;
                }
                if(bBackslash) {
                    bBackslash = false;
                    pos++;
                    continue;
                }
                pos++;
                continue;
            }

            // if it's var(, we need to recursively invoke ourself
            if (std::regex_search(cstrVar.c_str() + pos, varMatch, VarRegex)) {
                // if this is the left-hand side: create a parent node whose left node is, in turn, our
                //  lhs var
                if(!pGeneralTreeNode) {
                    pGeneralTreeNode = pGeneralTree->AddRoot("");
                }
                // nested var; pos is updated.  note if pGeneralTreeNode is already set, we're on the
                //  rhs
                __doParseVar(cstrVar, pos, pGeneralTree, pGeneralTreeNode);
                continue;
            }

            if(cstrVar[pos] == '(') {
                nParenthesisCount++;
                pos++;
                continue;
            }
            if(cstrVar[pos] == ')') {
                // end of the var( ... )
                if(!nParenthesisCount) {
                    // note - no +1 here becuase we're pointing at the ')'
                    Core::String strVar = cstrVar.substr(startingPos, pos - startingPos);
                    if(pGeneralTreeNode) {
                        // if general tree node is set, it means this is the *second* value in the var.
                        pGeneralTree->AddChild(pGeneralTreeNode, strVar);
                    }
                    else {
                        pGeneralTreeNode = pGeneralTree->AddRoot("");
                        pGeneralTree->AddChild(pGeneralTreeNode,strVar);
                    }
                    break;
                }
                nParenthesisCount--;
                pos++;
                continue;
            }
            if(cstrVar[pos] == ',') {
                if(bFoundComma) throw SvgException("Multiple commas in var at {}, {}", pos, cstrVar.c_str());
                // add a root node
                pGeneralTreeNode = pGeneralTree->AddRoot("");
                // get the string preceeding the comma; note no + 1 here as pos is pointing at the comma.
                Core::String strVar = cstrVar.substr(startingPos, pos - startingPos);
                // add the thing to the left of the comma as the first child
                pGeneralTree->AddChild(pGeneralTreeNode,strVar);
                bFoundComma = true;
                pos++;
                // to get what's right of the comma
                startingPos = pos;
                continue;
            }
            pos++;
            continue;
        }
        if(bIsString || bBackslash || nParenthesisCount) throw SvgException("Invalid var(): in the middle of a string? {} terminated with a backslash in a string? {} surplus parenthesis count {} in {}", bIsString, bBackslash, nParenthesisCount, cstrVar.c_str());
    }

    const GradientTemplate *SvgParser::__parseGradient(SvgParseState &svgParseState, Xml::IDomEntity *pGradientElement, SvgParserContext &svgParserContext)
    {
        std::unique_ptr<GradientTemplate> pGradientTemplate = std::make_unique<GradientTemplate>();

        Core::String strId { };
        (void) pGradientElement->tryGetAttribute(ID_ATTRIBUTE, strId);
        pGradientTemplate->strId = std::move(strId);

        // objectBoundingBox unless set, as the spec says
        pGradientTemplate->enumGradientUnits = ObjectBoundingBox;
        Core::String strGradientUnitsAttribute { };
        if (pGradientElement->tryGetAttribute(GRADIENTUNITS_ATTRIBUTE, strGradientUnitsAttribute)) {
            pGradientTemplate->enumGradientUnits = (GradientUnits)ParseGradientUnits(strGradientUnitsAttribute);
            pGradientTemplate->bUnitsSpecified = true;
        }
        pGradientTemplate->enumSpreadMethod = SpreadMethod::Pad;
        Core::String strSpreadMethod { };
        if (pGradientElement->tryGetAttribute(SPREADMETHOD_ATTRIBUTE, strSpreadMethod)) {
            pGradientTemplate->enumSpreadMethod = (SpreadMethod)ParseSpreadMethod(strSpreadMethod);
            pGradientTemplate->bSpreadMethodSpecified = true;
        }
        Core::String strTransform { };
        if (pGradientElement->tryGetAttribute(GRADIENTTRANSFORM_ATTRIBUTE, strTransform)) {
            pGradientTemplate->transform = __parseTransform(strTransform, GRADIENTTRANSFORM_ATTRIBUTE);
        }
        pGradientTemplate->stops = __parseStops(svgParseState, pGradientElement, svgParserContext);
        Core::String strHref = __retrieveHref(svgParseState, pGradientElement);
        strHref.trim_start_in_place(" #");
        pGradientTemplate->strHrefId = std::move(strHref);
        // without an href nothing can supply missing geometry, so the required parts must be here
        const bool bCanInherit = !Core::String::IsEmpty(pGradientTemplate->strHrefId);

        Core::String strGradientNodeName = Xml::String(pGradientElement->getTagName()).getTranscoded();
        std::unordered_set<Core::String> attributes {
            ID_ATTRIBUTE,
            GRADIENTUNITS_ATTRIBUTE,
            GRADIENTTRANSFORM_ATTRIBUTE,
            XLINK_HREF_ATTRIBUTE,
            HREF_ATTRIBUTE,
            SPREADMETHOD_ATTRIBUTE
        };

        // one geometry attribute: a number, or a percentage (a fraction of the bounding box or viewport)
        auto readGeometry = [&](size_t nIndex, const Core::String &cstrName, bool bRequired, const char *pszKind) {
            attributes.insert(cstrName);
            Core::String strValue { };
            if (!pGradientElement->tryGetAttribute(cstrName, strValue)) {
                if (bRequired && !bCanInherit) throw SvgException("{} Gradient has no {} attribute", pszKind, cstrName.c_str());
                return;
            }
            strValue = strValue.trim();
            if (!strValue.empty() && strValue.back() == '%') pGradientTemplate->geometry[nIndex] = Core::ParseFloat(strValue.substr(0, strValue.size() - 1)) / 100.0f;
            else pGradientTemplate->geometry[nIndex] = Core::ParseFloat(strValue);
            pGradientTemplate->geometrySpecified[nIndex] = true;
        };

        if(strGradientNodeName == LINEAR_GRADIENT_NAME) {
            pGradientTemplate->kind = GradientTemplate::linear;
            readGeometry(0, X1_ATTRIBUTE, true, "Linear");
            readGeometry(1, Y1_ATTRIBUTE, true, "Linear");
            readGeometry(2, X2_ATTRIBUTE, true, "Linear");
            readGeometry(3, Y2_ATTRIBUTE, true, "Linear");
        }
        else if(strGradientNodeName == RADIAL_GRADIENT_NAME) {
            pGradientTemplate->kind = GradientTemplate::radial;
            readGeometry(0, CX_ATTRIBUTE, true, "Radial");
            readGeometry(1, CY_ATTRIBUTE, true, "Radial");
            readGeometry(2, R_ATTRIBUTE, true, "Radial");
            readGeometry(3, FX_ATTRIBUTE, false, "Radial");
            readGeometry(4, FY_ATTRIBUTE, false, "Radial");
            readGeometry(5, FR_ATTRIBUTE, false, "Radial");
        }
        else throw SvgException("Invalid gradient node {}",strGradientNodeName.c_str());

        for (auto &kvp : pGradientElement->getAttributes())
        {
            if (!attributes.contains(kvp.first)) throw SvgException("Unrecognized {} attribute {}", strGradientNodeName.c_str(), kvp.first.c_str());
        }

        return svgParseState.svgDocument.storeGradient(std::move(pGradientTemplate));
    } // const GradientTemplate *SvgParser::__parseGradient(DOMDocument *, DOMElement *)

    std::array<float,6> SvgParser::__parseTransform(const Core::String &cstrTransform, const Core::String &cstrAttributeName)
    {
        Core::String strTransform = cstrTransform.trim();
        std::cmatch translateMatch;
        std::cmatch scaleMatch;
        std::cmatch rotateMatch;
        std::cmatch skewXyMatch;
        std::cmatch matrixMatch;
        size_t pos = 0;
        std::array<float,6> runningTransform = { 1, 0, 0, 1, 0, 0 };
        while(pos < strTransform.length()) {
            if (std::regex_search(strTransform.c_str() + pos, translateMatch, TranslateRegex)) {
                size_t localPos = 0;
                auto points = __parsePoints(translateMatch[1].str(), localPos, 2);
                pos += translateMatch[0].str().length();
                if(!points.size()) throw SvgException("translate requires 1 or 2 points at pos {} {} {}", pos, translateMatch[1].str(), strTransform.c_str());
                std::array<float, 6> pTranslateMatrix = { 1, 0, 0, 1, points[0], points.size() == 2 ? points[1] : 0 };
                runningTransform = Draw2d::Svg::multiply6s<float>(runningTransform,pTranslateMatrix);
                continue;
            }
            if (std::regex_search(strTransform.c_str() + pos, scaleMatch, ScaleRegex)) {
                size_t localPos = 0;
                auto points = __parsePoints(scaleMatch[1].str(), localPos, 2);
                if(!points.size()) throw SvgException("scale must consist of 1 or 2 points at pos {} {} {}", pos, scaleMatch[1].str(), strTransform.c_str());
                pos += scaleMatch[0].str().length();
                std::array<float,6> scaleMatrix = { points[0], 0, 0,  points.size() == 2 ? points[1] : points[0], 0, 0 };
                runningTransform = Draw2d::Svg::multiply6s<float>(runningTransform,scaleMatrix);
                continue;
            }
            if (std::regex_search(strTransform.c_str() + pos, rotateMatch, RotateRegex)) {
                size_t localPos = 0;
                auto points  = __parsePoints(rotateMatch[1].str(), localPos, 3);
                pos += rotateMatch[0].str().length();
                if(points.size() != 1 && points.size() != 3) throw SvgException("rotate doesn't consist of 1 or 2 points at pos {} {} {}", pos, rotateMatch[1].str(), strTransform.c_str());
                float angleRadians = points[0] * (std::numbers::pi / 180.0f);
                float cosAngleRadians = std::cos(angleRadians);
                float sinAngleRadians = std::sin(angleRadians);
                std::array<float,6> rotationMatrix = { cosAngleRadians, sinAngleRadians, -sinAngleRadians, cosAngleRadians, 0, 0 };
                if(points.size() == 3) {
                    // three points = rotate around a point.  Translate to the point, rotate, translate back again.
                    std::array<float,6> translationMatrix_there = { 1, 0, 0, 1, points[1], points[2] };
                    std::array<float,6> translationMatrix_backAgain = { 1, 0, 0, 1, -points[1], -points[2] };
                    runningTransform = Draw2d::Svg::multiply6s<float>(runningTransform,translationMatrix_there);
                    runningTransform = Draw2d::Svg::multiply6s<float>(runningTransform,rotationMatrix);
                    runningTransform = Draw2d::Svg::multiply6s<float>(runningTransform,translationMatrix_backAgain);
                }
                else runningTransform = Draw2d::Svg::multiply6s<float>(runningTransform,rotationMatrix);
                continue;
            }
            if (std::regex_search(strTransform.c_str() + pos, skewXyMatch, SkewXyRegex)) {
                Core::String xOrY = skewXyMatch[1].str();
                if(xOrY != "X" && xOrY != "Y") throw SvgException("Invalid value for skew_; (should be 'X' or 'Y', not {}) at {} in {}", xOrY.c_str(), pos, strTransform.c_str());
                size_t localPos = 0;
                auto points  = __parsePoints(skewXyMatch[2].str(), localPos, 1, true);
                pos += skewXyMatch[0].str().length();
                float angleRadians = points[0] * (std::numbers::pi / 180.0f);
                std::array<float,6> skewMatrix = { 1, (xOrY == "Y" ? angleRadians : 0),(xOrY == "X" ? angleRadians : 0), 1, 0, 0 };
                runningTransform = Draw2d::Svg::multiply6s<float>(runningTransform,skewMatrix);
                continue;
            }
            if (std::regex_search(strTransform.c_str() + pos, matrixMatch, MatrixRegex)) {
                size_t localPos = 0;
                auto points = __parsePoints(matrixMatch[1].str(), localPos, 6, true);
                // TODO
                std::array<float,6> transform { };
                std::copy(points.begin(), points.end(), transform.begin());
                pos += matrixMatch[0].str().length();
                runningTransform = Draw2d::Svg::multiply6s<float>(runningTransform,transform);
                continue;
            }

            throw SvgException("Supplied transform {} is not a valid transform",strTransform.c_str());
        }

        return runningTransform;
    } // std::array<float,6> SvgParser::__parseTransform(Core::String, Core::String)

    std::vector<Stop<SvgColour>> SvgParser::__parseStops(SvgParseState &svgParseState, Xml::IDomEntity *pStopsEntity, SvgParserContext &svgParserContext)
    {
        std::vector<Stop<SvgColour>> results { };
        for(std::unique_ptr<Xml::IDomEntity> pStopNode = pStopsEntity->getFirstChild();
            pStopNode; pStopNode = pStopNode->getNextSibling())
        {
            float offset;
            Core::String strOffsetAttribute {};
            if (!pStopNode->tryGetAttribute(OFFSET_ATTRIBUTE, strOffsetAttribute)) throw SvgException("offset is a required attribute");
            strOffsetAttribute = strOffsetAttribute.trim();
            if (!strOffsetAttribute.empty() && strOffsetAttribute.back() == '%') offset = Core::ParseFloat(strOffsetAttribute.substr(0, strOffsetAttribute.size() - 1)) / 100.0f;
            else offset = Core::ParseFloat(strOffsetAttribute);
            if (offset < 0 || offset > 1) throw SvgException(std::format("{} is invalid for offset (0 <= offset <= 1)",offset));
            Core::String strStopColourAttribute { };
            if (!pStopNode->tryGetAttribute(STOP_COLOUR_ATTRIBUTE, strStopColourAttribute)) throw SvgException("stop-colour is a required attribute");
            float fOpacity = 1.0f;
            Core::String strOpacityAttribute = { };
            if (pStopNode->tryGetAttribute(STOP_OPACITY_ATTRIBUTE, strOpacityAttribute))
            {
                fOpacity = Core::ParseFloat(strOpacityAttribute);
                if (fOpacity < 0 || fOpacity > 1) throw SvgException(std::format("{} is invalid for opacity (0 <= opacity <= 1)",fOpacity));
            }

            SvgColour svgColour = __parseColour(svgParseState, strStopColourAttribute, svgParserContext);

            Stop<SvgColour> stop { offset, svgColour, fOpacity };
            results.push_back(stop);
        }

        return results;
    } // std::vector<Stop<Colour>> SvgParser::__parseStops(SvgParseState &, DOMElement *)

} // namespace Draw2d::Svg

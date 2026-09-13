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
        svgParserContext.setViewportHeight(fViewportWidth);
        (void) __parseSvgElement(svgParseState, domEntity.get(), svgParserContext);

//        __resolveUses(svgParseState);

        return pSvgDocument;
    }

    std::regex SvgParser::SvgRgbRegex { R"xxx(^\s*rgba?\s*\(\s*([+-]?\d*\.?\d+)\s*(%?)\s*,?\s*([+-]?\d*\.?\d+)\s*\2\s*,?\s*([+-]?\d*\.?\d+)\s*\2\s*[,/]?\s*(?:([+-]?\d*\.?\d+)\s*(%?))?\)\s*$)xxx" };
    std::regex SvgParser::SvgHslRegex { R"xxx(^\s*hsla?\s*\(\s*([+-]?\d*\.?\d+)\s*(deg|grad|rad|turn)?\s*,?\s*([+-]?\d*\.?\d+)\s*(%?)\s*,?\s*([+-]?\d*\.?\d+)\s*(%?)\s*(?:[,/]\s*([+-]?\d*\.?\d+)(%?)\s*)?\)\s*$)xxx" };
    std::regex SvgParser::UrlRegex { R"xxx(url\(#([^)]+)\))xxx"};
    std::regex SvgParser::VarRegex { R"xxx(^\s*var\s*\()xxx"};
    std::regex SvgParser::HexColourRegex { R"xxx(^#([0-9a-f]{6,8})$)xxx", std::regex::icase };
    std::regex SvgParser::TranslateRegex { R"xxx(^\s*translate\(([^)]+)\))xxx", std::regex::icase };
    std::regex SvgParser::ScaleRegex { R"xxx(^\s*scale\(([^)]+)\))xxx", std::regex::icase };
    std::regex SvgParser::RotateRegex { R"xxx(^\s*rotate\(([^)]+)\))xxx", std::regex::icase };
    std::regex SvgParser::SkewXyRegex { R"xxx(^\s*skew([XY])\(([^)]+)\))xxx", std::regex::icase };
    std::regex SvgParser::MatrixRegex { R"xxx(^\s*matrix\(([^)]+)\))xxx", std::regex::icase };
    std::regex SvgParser::DimensionRegex { R"xxx(([+-]?(?:\d+\.?\d*|\.\d+)(?:[eE][+-]?\d+)?)\s*(%|px|pt|pc|cm|mm|in|q|em|ex|ch|rem|vw|vh|vmin|vmax)?)xxx", std::regex::icase };
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

        __updateInheritedProperties(svgParseState, pSvgElement, svgParserContext);
        __parseCommonProperties(svgParseState, pSvgElement, svgSvgParams, svgParserContext);

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
            svgParserContext.setViewportHeight(viewBox[2]);
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

    template <typename F>
    void SvgParser::__applyIfSet(Xml::IDomEntity *pElement, const Core::String &cstrAttributeName, F &&apply)
    {
        Core::String strValue { };
        if(!pElement->tryGetAttribute(cstrAttributeName,strValue)
        || strValue == INHERIT_VALUE) return;
        apply(strValue);
    }

    void SvgParser::__updateInheritedProperties(SvgParseState &svgParseState, Xml::IDomEntity *pElement, SvgParserContext &svgParserContext)
    {
        // great suggestion from claude.  Most of these properties can take the value "inherit,"
        //  so we explicitly check for that and ignore any inherit values.  There are a few exceptions
        __applyIfSet(pElement, COLOR_ATTRIBUTE, [&](auto s) {svgParserContext.setColour(__parseColour(svgParseState, s, svgParserContext)); });

        Core::String transformAttribute { };
        if (pElement->tryGetAttribute(TRANSFORM_ATTRIBUTE,transformAttribute)) {
            std::array<float,6> matrix { };
            matrix = __parseTransform(transformAttribute,"transform");
            svgParserContext.multiplyRight(matrix);
        }

        __applyIfSet(pElement, FILL_ATTRIBUTE, [&](auto s) {svgParserContext.setFillColour(__parsePaint(svgParseState, s, svgParserContext)); });

        __applyIfSet(pElement, FILL_RULE_ATTRIBUTE, [&](auto s) { svgParserContext.setFillRule(ParseFillRule(s)); });

        __applyIfSet(pElement, FILL_OPACITY_ATTRIBUTE, [&](auto s) { svgParserContext.setFillOpacity(Core::ParseFloat(s)); });

        __applyIfSet(pElement, STROKE_ATTRIBUTE, [&](auto s) { svgParserContext.setStrokeColour(__parsePaint(svgParseState, s, svgParserContext)); });

        __applyIfSet(pElement, STROKE_OPACITY_ATTRIBUTE, [&](auto s) { svgParserContext.setStrokeOpacity(Core::ParseFloat(s)); });

        __applyIfSet(pElement, STROKE_WIDTH_ATTRIBUTE, [&](auto s) { svgParserContext.setStrokeWidth(Core::ParseFloat(s)); });

        __applyIfSet(pElement, STROKE_LINECAP_ATTRIBUTE, [&](auto s) { svgParserContext.setLineCap(ParseLineCap(s)); });

        __applyIfSet(pElement, STROKE_LINEJOIN_ATTRIBUTE, [&](auto s) { svgParserContext.setLineJoin(ParseLineJoin(s)); });

        __applyIfSet(pElement, STROKE_MITRELIMIT_ATTRIBUTE, [&](auto s) { svgParserContext.setStrokeMiterLimit(Core::ParseFloat(s)); });

        // dash array is a list of numbers that controls when the dash is on and off
        std::vector<float> dashArray {};
        Core::String strStrokeDashArray { };
        if(pElement->tryGetAttribute(STROKE_DASHARRAY_ATTRIBUTE, strStrokeDashArray)
        && strStrokeDashArray != INHERIT_VALUE) {
            // subtle - none should set an empty array.
            if(strStrokeDashArray != NONE_VALUE) {
                std::regex delimiter("\\s+");
                
                // The -1 argument tells the iterator to extract everything EXCEPT the pattern
                std::sregex_token_iterator iter(strStrokeDashArray.begin(), strStrokeDashArray.end(), delimiter, -1);
                std::sregex_token_iterator end;
                
                std::vector<std::string> tokens(iter, end);
                std::erase_if(tokens,[](std::string &value) { return !value.length(); });
                std::transform(tokens.begin(), tokens.end(), std::back_inserter(dashArray), [](const std::string &cstrValue) { return Core::ParseFloat(cstrValue); });
            }
            svgParserContext.setDashArray(dashArray);
        }

        __applyIfSet(pElement, STROKE_DASHOFFSET_ATTRIBUTE, [&](auto s) { svgParserContext.setDashOffset(Core::ParseFloat(s)); });
    }

    void SvgParser::__parseCommonProperties(SvgParseState &svgParseState, Xml::IDomEntity *pRenderedElement, SvgEntityParams &params, const SvgParserContext &svgParserContext)
    {
        Core::String strId { };
        (void) pRenderedElement->tryGetAttribute(ID_ATTRIBUTE,strId);
        params.id = strId;

        params.render = svgParserContext.getRender();

        if(pRenderedElement->hasAttribute(TRANSFORM_ATTRIBUTE)) {
            params.matrix = duplicateMatrix6(svgParserContext.getMatrix());
        }

        if(pRenderedElement->hasAttribute(FILL_ATTRIBUTE)) {
            params.fillColour = svgParserContext.getFillColour();
        }

        if(pRenderedElement->hasAttribute(FILL_RULE_ATTRIBUTE)) {
            params.fillRule = svgParserContext.getFillRule();
        }

        if(pRenderedElement->hasAttribute(FILL_OPACITY_ATTRIBUTE)) {
            params.fillOpacity = svgParserContext.getFillOpacity();
        }

        if(pRenderedElement->hasAttribute(STROKE_ATTRIBUTE)) {
            params.strokeColour = svgParserContext.getStrokeColour();
        }

        if(pRenderedElement->hasAttribute(STROKE_OPACITY_ATTRIBUTE)) {
            params.strokeOpacity = svgParserContext.getStrokeOpacity();
        }

        if(pRenderedElement->hasAttribute(STROKE_WIDTH_ATTRIBUTE)) {
            params.strokeWidth = svgParserContext.getStrokeWidth();
        }

        if(pRenderedElement->hasAttribute(STROKE_LINECAP_ATTRIBUTE)) {
            params.lineCap = svgParserContext.getLineCap();
        }

        if(pRenderedElement->hasAttribute(STROKE_LINEJOIN_ATTRIBUTE)) {
            params.lineJoin = svgParserContext.getLineJoin();
        }

        if(pRenderedElement->hasAttribute(STROKE_MITRELIMIT_ATTRIBUTE)) {
            params.miterLimit = svgParserContext.getStrokeMiterLimit();
        }

        if(pRenderedElement->hasAttribute(STROKE_DASHARRAY_ATTRIBUTE)) {
            params.dashArray = svgParserContext.getDashArray();
        }

        if(pRenderedElement->hasAttribute(STROKE_DASHOFFSET_ATTRIBUTE)) {
            params.dashOffset = svgParserContext.getDashOffset();
        }

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

        __updateInheritedProperties(svgParseState, pGroupElement, svgParserContext);
        __parseCommonProperties(svgParseState, pGroupElement, svgGroupParams, svgParserContext);

        std::vector<const SvgEntity *> svgEntities { };
        float fOpacity = 1.0f;
        Core::String strOpacityAttribute { };
        if (pGroupElement->tryGetAttribute(OPACITY_ATTRIBUTE, strOpacityAttribute)) fOpacity = Core::ParseFloat(strOpacityAttribute);

        svgGroupParams.fOpacity = fOpacity;
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

    const Path *SvgParser::__parsePath(SvgParseState &svgParseState, Xml::IDomEntity *pPathElement, const SvgParserContext &svgParserContextIn)
    {
        SvgParserContext svgParserContext = svgParserContextIn;

        SvgPathParams svgPathParams { SvgEntityParams { .svgDocument = svgParseState.svgDocument } };

        __updateInheritedProperties(svgParseState, pPathElement, svgParserContext);
        __parseCommonProperties(svgParseState, pPathElement, svgPathParams, svgParserContext);

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

    const Use *SvgParser::__parseUse(SvgParseState &svgParseState, Xml::IDomEntity *pUseElement, SvgParserContext svgParserContext)
    {
        SvgUseParams svgUseParams { SvgContainerParams { SvgEntityParams { .svgDocument = svgParseState.svgDocument } } };

        // when we encounter use, we create the use node itself, set the child wrangler to
        //  it and instance it as we go, being mindful to check for circular references
        //  in the expansion
        __updateInheritedProperties(svgParseState, pUseElement, svgParserContext);
        __parseCommonProperties(svgParseState, pUseElement, svgUseParams, svgParserContext);

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

        __updateInheritedProperties(svgParseState, pRectElement, svgParserContext);
        __parseCommonProperties(svgParseState, pRectElement, svgRectParams, svgParserContext);

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

        __updateInheritedProperties(svgParseState, pEllipseElement, svgParserContext);
        __parseCommonProperties(svgParseState, pEllipseElement, svgEllipseParams, svgParserContext);

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

        __updateInheritedProperties(svgParseState, pCircleElement, svgParserContext);
        __parseCommonProperties(svgParseState, pCircleElement, svgCircleParams, svgParserContext);

        std::unique_ptr<Circle> pCircle = std::make_unique<Circle>(std::move(svgCircleParams));
        Circle *toReturn = svgParserContext.getChildWrangler()->addChildAs(std::move(pCircle));
        __saveEntityToDocument(svgParseState, toReturn);
        return toReturn;
    }

    const Image *SvgParser::__parseImage(SvgParseState &svgParseState, Xml::IDomEntity *pImageElement, SvgParserContext svgParserContext)
    {
        SvgImageParams svgImageParams { SvgEntityParams { .svgDocument = svgParseState.svgDocument } };

        __updateInheritedProperties(svgParseState, pImageElement, svgParserContext);
        __parseCommonProperties(svgParseState, pImageElement, svgImageParams, svgParserContext);

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

        __updateInheritedProperties(svgParseState, pTextElement, svgParserContext);
        __parseCommonProperties(svgParseState, pTextElement, svgTextParams, svgParserContext);

        Core::String strX { };
        size_t nPosition = 0;
        if (pTextElement->tryGetAttribute(X_ATTRIBUTE,strX)) {
            svgTextParams.x = __parsePoints(strX, nPosition, 0, false, true);
        }

        Core::String strY { };
        nPosition = 0;
        if (pTextElement->tryGetAttribute(Y_ATTRIBUTE,strY)) {
            svgTextParams.y = __parsePoints(strY, nPosition, 0, false, true);
        }

        Core::String strDx { };
        nPosition = 0;
        if (pTextElement->tryGetAttribute(DX_ATTRIBUTE,strDx)) {
            svgTextParams.dx = __parsePoints(strDx, nPosition, 0, false, true);
        }

        Core::String strDy { };
        nPosition = 0;
        if (pTextElement->tryGetAttribute(DY_ATTRIBUTE,strDy)) {
            svgTextParams.dy = __parsePoints(strDy, nPosition, 0, false, true);
        }

        Core::String strRotate { };
        nPosition = 0;
        if (pTextElement->tryGetAttribute(ROTATE_ATTRIBUTE,strRotate)) {
            svgTextParams.rotate = __parsePoints(strRotate, nPosition, 0, false, true);
        }

        Core::String strLengthAdjust { };
        if (pTextElement->tryGetAttribute(LENGTHADJUST_ATTRIBUTE,strLengthAdjust)) {
            svgTextParams.lengthAdjust = ParseLengthAdjust(strLengthAdjust);
        }

        Core::String strTextLength { };
        if (pTextElement->tryGetAttribute(TEXTLENGTH_ATTRIBUTE,strTextLength)) {
            auto rit = strTextLength.rbegin();
            // Skip trailing whitespace first (handles "50%", "50 %", and "50   ")
            while (rit != strTextLength.rend() && std::isspace(static_cast<unsigned char>(*rit)))
                ++rit;

            if (rit != strTextLength.rend() && *rit == '%')
            {
                svgTextParams.textLengthPercentage = true;
                ++rit;
                // Skip any whitespace between the number and the '%' too, e.g. "50   %"
                while (rit != strTextLength.rend() && std::isspace(static_cast<unsigned char>(*rit)))
                    ++rit;
            }

            strTextLength.erase(rit.base(), strTextLength.end());
            svgTextParams.textLength = ParseFloat(strTextLength);
        }

        std::unique_ptr<Text> pText = std::make_unique<Text>(std::move(svgTextParams));
        Text *toReturn = svgParserContext.getChildWrangler()->addChildAs(std::move(pText));
        __saveEntityToDocument(svgParseState, toReturn);
        return toReturn;
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
            return SvgPaint(SvgColourType::Gradient, urlMatch[1].str());
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

            // endless pita with svg.  seems handy, though.
            std::array<uint8_t,4> pBgra { };
            unsigned nSize = 3;
            if(!Core::String::IsEmpty(strAlpha)) nSize = 4;

            pBgra[0] = Core::ParseFloat(strBlue.c_str());
            pBgra[1] = Core::ParseFloat(strGreen.c_str());
            pBgra[2] = Core::ParseFloat(strRed.c_str());
            if(nSize == 4) {
                pBgra[3] = Core::ParseFloat(strAlpha.c_str());
                if(!Core::String::IsEmpty(strAlphaPercent)) {
                    pBgra[3] = static_cast<uint8_t>(pBgra[3] / 100.0f * 255.0);
                }
            }
            else pBgra[3] = 0xff;

            if (!Core::String::IsWhitespace(strRgbPercent.c_str()))
            {
                // rgb given in percentage:
                // make sure the values are clamped 0-100
                for(unsigned index = 0; index < nSize; index++) if (pBgra[index] > 100) throw SvgException("Invalid pBgra value ({} {}, {}, {}",strColour.c_str(), pBgra[0], pBgra[1], pBgra[2]);
                // convert to 0-255 byte value
                for(unsigned index = 0; index < nSize; index++) pBgra[index] = static_cast<uint8_t>(pBgra[index]/100.0f* 255);
            }
            return SvgColour(nSize == 4 ? SvgColourType::BgrA : SvgColourType::Bgr, -1, std::move(pBgra));
        }

        // check for hsla
        std::cmatch svgHslMatch;
        if (std::regex_search(strColour.c_str(), svgHslMatch, SvgHslRegex))
        {
            Core::String strHue =               svgHslMatch[1].str();
            Core::String strHueUnits =          svgHslMatch[2].str();
            Core::String strSaturation =        svgHslMatch[3].str();
            Core::String strSaturationPercent = svgHslMatch[4].str();
            Core::String strLightness =         svgHslMatch[5].str();
            Core::String strLightnessPercent =  svgHslMatch[6].str();
            Core::String strAlpha =             svgHslMatch[7].str();
            Core::String strAlphaPercent =      svgHslMatch[8].str();

            // endless pita with svg.  seems handy, though.
            std::array<uint8_t,4> pHsla = { };
            unsigned nSize = 3;
            // NOTE - per the spec, the "a" is optional.  They can either provide an alpha or not with
            //  either hsl or hsla.  svgs are so loosey-goosey and munged up with css :-/
            if(strAlpha.length()) nSize = 4;
            pHsla[0] = Core::ParseFloat(strHue);
            pHsla[1] = Core::ParseFloat(strSaturation);
            // clamp saturation per spec
            if(strSaturationPercent.length()) {
                pHsla[1] = std::clamp<float>(pHsla[1], 0.0f, 100.0f);
                pHsla[1] /= 100.0f;
            }
            else {
                pHsla[1] = std::clamp<float>(pHsla[1], 0.0f, 1.0f);
                if(pHsla[1] < 0) pHsla[1] = 0;
                if(pHsla[1] > 100) pHsla[1] = 100;
            }
            pHsla[2] = Core::ParseFloat(strLightness);
            // clamp lightness per spec
            if(strSaturationPercent.length()) {
                pHsla[2] = std::clamp<float>(pHsla[2], 0.0f, 200.0f);
                pHsla[2] /= 200.0f;
            }
            else pHsla[2] = std::clamp<float>(pHsla[2], 0.0f, 2.0f);

            if(nSize == 4) {
                pHsla[3] = Core::ParseFloat(svgHslMatch[7].str());
                if(!Core::String::IsEmpty(strAlphaPercent)) pHsla[3] /= 100.0f;
                // ditto again even
                if(pHsla[3] < 0) pHsla[1] = 0;
                if(pHsla[3] > 1.0f) pHsla[1] = 1.0f;
            }
            else pHsla[4] = 1.0f;

            if(!strHueUnits.length() || strHueUnits.c_str() == DEGREES_UNIT) {
                pHsla[0] *= (std::numbers::pi / 180.0f);
            }
            else if(strHueUnits.length()) {
                // gradians rule, you drool!
                if(strHueUnits.c_str() == GRADIANS_UNIT) pHsla[0] *= (std::numbers::pi / 200.0f);
                else if(strHueUnits.c_str() == RADIANS_UNIT) /* noop */;
                else if(strHueUnits.c_str() == TURN_UNIT) pHsla[0] = (pHsla[0] * 360) * (std::numbers::pi / 180.0f);
                else throw SvgException("Unrecognized units on hue in hsl(a) value {}: {}", strColour.c_str(), strHueUnits.c_str());
            }
            return SvgColour(nSize == 4 ? SvgColourType::HslA : SvgColourType::Hsl, -1, std::move(pHsla));
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
        if (!std::regex_search(strColour.c_str(), colourMatch, HexColourRegex)) throw SvgException("Couldn't recognize value in subParseColour `{}'",strColour.c_str());
        if ((colourMatch[1].length() % 2) == 1) throw SvgException(std::format("BGR/BGRA must be comprised of 6 or 8 colours, not {}",colourMatch[1].length()));
        unsigned nBgraLength = colourMatch[1].length() / 2;
        std::array<uint8_t,4> bgra { };
        unsigned cIndex = 0;
        while (cIndex < colourMatch[1].length())
        {
            bgra[cIndex / 2] = (uint8_t)Core::ParseInteger(colourMatch[1].str().substr(cIndex, 2), BASE16);
            cIndex += 2;
        }

        SvgColourType enumColourType = SvgColourType::NotAnSvgColourType;
        switch (nBgraLength)
        {
            case 3: 
                enumColourType = SvgColourType::Bgr;
                bgra[3] = 0xff;
                break;
            case 4: enumColourType = SvgColourType::BgrA; break;
            default: throw SvgException("Unable to parse subcolour from {}",strColour.c_str());
        }

        return SvgColour(enumColourType, -1, std::move(bgra));
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
        GradientUnits enumGradientUnits = UserSpaceOnUse;
        Core::String strGradientUnitsAttribute { };
        if (pGradientElement->tryGetAttribute(GRADIENTUNITS_ATTRIBUTE, strGradientUnitsAttribute)) enumGradientUnits = (GradientUnits)ParseGradientUnits(strGradientUnitsAttribute);
        SpreadMethod enumSpreadMethod = SpreadMethod::Pad;
        Core::String strSpreadMethod { };
        if (pGradientElement->tryGetAttribute(SPREADMETHOD_ATTRIBUTE, strSpreadMethod)) enumSpreadMethod = (SpreadMethod)ParseSpreadMethod(strSpreadMethod);
        Core::String strValue;
        std::array<float,6> matrix {};
        strValue = { };
        if (pGradientElement->tryGetAttribute(GRADIENTTRANSFORM_ATTRIBUTE, strValue)) matrix = __parseTransform(strValue,"gradientTransform");
        std::vector<Stop<SvgColour>> stops = __parseStops(svgParseState, pGradientElement, svgParserContext);
        Core::String strHref = __retrieveHref(svgParseState, pGradientElement);
        strHref.trim_start_in_place(" #");

        pGradientTemplate->strId = std::move(strId);
        pGradientTemplate->enumGradientUnits = enumGradientUnits;
        pGradientTemplate->enumSpreadMethod = enumSpreadMethod;
        pGradientTemplate->transform = std::move(matrix);
        pGradientTemplate->stops = std::move(stops);
        pGradientTemplate->strHrefId = std::move(strHref);

        Core::String strGradientNodeName = Xml::String(pGradientElement->getTagName()).getTranscoded();
        std::unordered_set<Core::String> attributes { 
            ID_ATTRIBUTE,
            GRADIENTUNITS_ATTRIBUTE,
            GRADIENTTRANSFORM_ATTRIBUTE,
            XLINK_HREF_ATTRIBUTE,
            HREF_ATTRIBUTE,
            SPREADMETHOD_ATTRIBUTE
        };
;
        if(strGradientNodeName == LINEAR_GRADIENT_NAME) {
            float x1, y1, x2, y2;
            strValue = { };
            if (!pGradientElement->tryGetAttribute(X1_ATTRIBUTE, strValue)) throw SvgException("Linear Gradient has no x1 attribute");
            x1 = Core::ParseFloat(strValue);
            strValue = { };
            if (!pGradientElement->tryGetAttribute(X2_ATTRIBUTE, strValue)) throw SvgException("Linear Gradient has no x2 attribute");
            x2 = Core::ParseFloat(strValue);
            strValue = { };
            if(!pGradientElement->tryGetAttribute(Y1_ATTRIBUTE, strValue)) throw SvgException("Linear Gradient has no y1 attribute");
            y1 = Core::ParseFloat(strValue);
            strValue = { };
            if (!pGradientElement->tryGetAttribute(Y2_ATTRIBUTE, strValue)) throw SvgException("Linear Gradient has no y2 attribute");
            y2 = Core::ParseFloat(strValue);

            attributes.insert(X1_ATTRIBUTE);
            attributes.insert(Y1_ATTRIBUTE);
            attributes.insert(Y2_ATTRIBUTE);
            attributes.insert(X2_ATTRIBUTE);

            auto nodeAttributes = pGradientElement->getAttributes();
            for (auto &kvp : nodeAttributes)
            {
                Core::String strNodeName = kvp.first;
                if (!attributes.contains(strNodeName)) throw SvgException("Unrecognized linear gradient attribute {}",strNodeName);
            }

            pGradientTemplate->kind = GradientTemplate::linear;
            pGradientTemplate->geometry[0] = x1; 
            pGradientTemplate->geometry[1] = y1; 
            pGradientTemplate->geometry[2] = x2; 
            pGradientTemplate->geometry[3] = y2; 
        }
        else if(strGradientNodeName == RADIAL_GRADIENT_NAME) {
            float cx, cy, r, fx = 0.0f, fy = 0.0f, fr = 0.0f;
            strValue = { };
            if (!pGradientElement->tryGetAttribute(CX_ATTRIBUTE, strValue)) throw SvgException("Radial Gradient has no cx attribute");
            cx = Core::ParseFloat(strValue);
            strValue = { };
            if (!pGradientElement->tryGetAttribute(CY_ATTRIBUTE, strValue)) throw SvgException("Radial Gradient has no cy attribute");
            cy = Core::ParseFloat(strValue);
            strValue = { };
            if (!pGradientElement->tryGetAttribute(R_ATTRIBUTE, strValue)) throw SvgException("Radial Gradient has no r attribute");
            r = Core::ParseFloat(strValue);
            strValue = { };
            if (pGradientElement->tryGetAttribute(FX_ATTRIBUTE, strValue)) {
                fx = Core::ParseFloat(strValue);
            }
            strValue = { };
            if (pGradientElement->tryGetAttribute(FY_ATTRIBUTE, strValue)) {
                fy = Core::ParseFloat(strValue);
            }
            strValue = { };
            if (pGradientElement->tryGetAttribute(FR_ATTRIBUTE, strValue)) {
                fr = Core::ParseFloat(strValue);
            }

            attributes.insert(CX_ATTRIBUTE);
            attributes.insert(CY_ATTRIBUTE);
            attributes.insert(FR_ATTRIBUTE);
            attributes.insert(FX_ATTRIBUTE);
            attributes.insert(FY_ATTRIBUTE);
            attributes.insert(R_ATTRIBUTE);

            auto nodeAttributes = pGradientElement->getAttributes();
            for (auto &kvp : nodeAttributes)
            {
                Core::String strNodeName = kvp.first;
                if (!attributes.contains(strNodeName)) throw SvgException("Unrecognized linear gradient attribute {}",strNodeName);
            }

            pGradientTemplate->kind = GradientTemplate::radial;
            pGradientTemplate->geometry[0] = cx;
            pGradientTemplate->geometry[1] = cy; 
            pGradientTemplate->geometry[2] = r; 
            pGradientTemplate->geometry[3] = fx; 
            pGradientTemplate->geometry[4] = fy; 
            pGradientTemplate->geometry[5] = fr; 
        }
        else throw SvgException("Invalid gradient node {}",strGradientNodeName.c_str());

        const GradientTemplate *cpGradientTemplate = svgParseState.svgDocument.storeGradient(std::move(pGradientTemplate));

        return cpGradientTemplate;
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
            offset = Core::ParseFloat(strOffsetAttribute);
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

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
#ifndef DRAW2D_SVG_IMAGE_DOT_HPP
#define DRAW2D_SVG_IMAGE_DOT_HPP

#ifndef DRAW2D_SVG_SVGENTITY_DOT_HPP
    #include "svg/SvgEntity.hpp"
#endif
#ifndef DRAW2D_SVG_SVGDIMENSIONEDENTITY_DOT_HPP
    #include "svg/SvgDimensionedEntity.hpp"
#endif
#ifndef DRAW2D_SVG_PRESERVEASPECTRATIO_DOT_HPP
    #include "svg/PreserveAspectRatio.hpp"
#endif
#ifndef DRAW2D_SVG_CROSSORIGIN_DOT_HPP
    #include "svg/CrossOrigin.hpp"
#endif
#ifndef DRAW2D_SVG_DECODING_DOT_HPP
    #include "svg/Decoding.hpp"
#endif
#ifndef DRAW2D_SVG_FETCHPRIORITY_DOT_HPP
    #include "svg/FetchPriority.hpp"
#endif

namespace Draw2d::Svg {

    struct SvgImageParams : public SvgEntityParams {
        SvgDimensionedParams svgDimensionedParams;
        Core::String href;
        Core::String imageType;
        Core::String characterEncoding;
        PreserveAspectRatio preserveAspectRatio = PreserveAspectRatio::xMidYMid;
        PreserveAspectRatioMode preserveAspectRatioMode = PreserveAspectRatioMode::Meet;
        std::optional<CrossOrigin> crossOrigin = std::nullopt;
        std::optional<Decoding> decoding = std::nullopt;
        std::optional<FetchPriority> fetchPriority = std::nullopt;
    };

    /// if the href is expressed as a data uri, it is stripped from href and stored to
    ///  ImageType and CharacterEncoding properties, _if they are present_.  Furthermore,
    ///  if the href is base64-encoded, the href attribute is decoded.  Note that charset,
    ///  if present, must be utf-8 at the time of this writing.  And therefore, the
    ///  CharacterEncoding property will be set to utf-8, if present.
    class Image: public SvgEntity {
    public:
        Image(SvgImageParams p)
            : SvgEntity(static_cast<SvgEntityParams &&>(p))
            , m_svgDimensionedEntity(std::move(p.svgDimensionedParams))
            , m_strHref(std::move(p.href))
            , m_strImageType(std::move(p.imageType))
            , m_strCharacterEncoding(std::move(p.characterEncoding))
            , m_enumPreserveAspectRatio(p.preserveAspectRatio)
            , m_enumPreserveAspectRatioMode(p.preserveAspectRatioMode)
            , m_enumCrossOrigin(p.crossOrigin)
            , m_enumDecoding(p.decoding)
            , m_enumFetchPriority(p.fetchPriority)
        {}

        virtual ~Image() = default;

        std::optional<Dimension> getX() const  { return m_svgDimensionedEntity.getX(); }
        std::optional<Dimension> getY() const  { return m_svgDimensionedEntity.getY(); }
        std::optional<Dimension> getWidth() const  { return m_svgDimensionedEntity.getWidth(); }
        std::optional<Dimension> getHeight() const  { return m_svgDimensionedEntity.getHeight(); }

        const Core::String &getHref() const { return m_strHref; }
        const Core::String &getImageType() const { return m_strImageType; }
        const Core::String &getCharacterEncoding() const { return m_strCharacterEncoding; }
        PreserveAspectRatio getPreserveAspectRatio() const { return m_enumPreserveAspectRatio; }
        PreserveAspectRatioMode getPreserveAspectRatioMode() const { return m_enumPreserveAspectRatioMode; }
        std::optional<CrossOrigin> getCrossOrigin() const { return m_enumCrossOrigin; }
        std::optional<Decoding> getDecoding() const { return m_enumDecoding; }
        std::optional<FetchPriority> getFetchPriority() const { return m_enumFetchPriority; }

        virtual const char * const getType() const override { return "Image"; }
        const SvgDimensionedEntity *getDimensionedEntity() const override { return &m_svgDimensionedEntity; }

    private:
        SvgDimensionedEntity m_svgDimensionedEntity;
        Core::String m_strHref {};
        Core::String m_strImageType {};
        Core::String m_strCharacterEncoding {};
        PreserveAspectRatio m_enumPreserveAspectRatio = PreserveAspectRatio::xMidYMid;
        PreserveAspectRatioMode m_enumPreserveAspectRatioMode = PreserveAspectRatioMode::Meet;
        std::optional<CrossOrigin> m_enumCrossOrigin = std::nullopt;
        std::optional<Decoding> m_enumDecoding = std::nullopt;
        std::optional<FetchPriority> m_enumFetchPriority = std::nullopt;
    };

}

#endif  /* DRAW2D_SVG_IMAGE_DOT_HPP */

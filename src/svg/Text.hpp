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

#ifndef DRAW2D_SVG_TEXT_DOT_HPP
#define DRAW2D_SVG_TEXT_DOT_HPP

#ifndef DRAW2D_SVG_SVGENTITY_DOT_HPP
    #include "svg/SvgEntity.hpp"
#endif
#ifndef DRAW2D_SVG_LENGTHADJUST_DOT_HPP
    #include "svg/LengthAdjust.hpp"
#endif
#ifndef DRAW2D_SVG_TEXTPARTS_DOT_HPP
    #include "svg/TextParts.hpp"
#endif
#ifndef DRAW2D_SVG_TEXTANCHOR_DOT_HPP
    #include "svg/TextAnchor.hpp"
#endif
#ifndef DRAW2D_SVG_FONTSTYLE_DOT_HPP
    #include "svg/FontStyle.hpp"
#endif
#ifndef DRAW2D_SVG_DIMENSIONUNITS_DOT_HPP
    #include "svg/DimensionUnits.hpp"
#endif

namespace Draw2d::Svg {

    struct SvgTextParams : public SvgEntityParams, public SvgTSpanParams {
        // les attributs de présentation de la police, tels qu'écrits sur
        // l'élément; vides s'ils n'y sont pas -- l'héritage les remplira un jour
        std::optional<float> fontSize {};
        std::optional<DimensionUnits> fontSizeUnits {};
        std::optional<Core::String> fontFamily {};
        std::optional<TextAnchor> textAnchor {};
        std::optional<FontStyle> fontStyle {};
    };

    class Text : public SvgEntity {
    public:
        Text(SvgTextParams svgTextParams):
            SvgEntity(static_cast<SvgEntityParams &&>(svgTextParams))
          , m_rawText(std::move(svgTextParams.rawText))
          , m_x(std::move(svgTextParams.x))
          , m_y(std::move(svgTextParams.y))
          , m_dx(std::move(svgTextParams.dx))
          , m_dy(std::move(svgTextParams.dy))
          , m_rotate(std::move(svgTextParams.rotate))
          , m_enumLengthAdjust(svgTextParams.lengthAdjust)
          , m_textLength(svgTextParams.textLength)
          , m_bTextLengthPercentage(svgTextParams.textLengthPercentage)
          , m_fontSize(svgTextParams.fontSize)
          , m_enumFontSizeUnits(svgTextParams.fontSizeUnits)
          , m_fontFamily(std::move(svgTextParams.fontFamily))
          , m_textAnchor(svgTextParams.textAnchor)
          , m_fontStyle(svgTextParams.fontStyle)
        {
        }

        const RawText &getRawText() const { return m_rawText; }
        const std::vector<float> &getX() const { return m_x; }
        const std::vector<float> &getY() const { return m_y; }
        const std::vector<float> &getDx() const { return m_dx; }
        const std::vector<float> &getDy() const { return m_dy; }
        const std::vector<float> &getRotate() const { return m_rotate; }
        LengthAdjust getLengthAdjust() const { return m_enumLengthAdjust; }
        std::optional<float> getTextLength() const { return m_textLength; }
        bool getTextLengthPercentage() const { return m_bTextLengthPercentage; }
        std::optional<float> getFontSize() const { return m_fontSize; }
        std::optional<DimensionUnits> getFontSizeUnits() const { return m_enumFontSizeUnits; }
        const std::optional<Core::String> &getFontFamily() const { return m_fontFamily; }
        std::optional<TextAnchor> getTextAnchor() const { return m_textAnchor; }
        std::optional<FontStyle> getFontStyle() const { return m_fontStyle; }

        virtual const char * const getType() const { return "Text"; }

    private:
        RawText m_rawText;
        std::vector<float> m_x { };
        std::vector<float> m_y { };
        std::vector<float> m_dx { };
        std::vector<float> m_dy { };
        std::vector<float> m_rotate { };
        LengthAdjust m_enumLengthAdjust = LengthAdjust::Spacing;
        std::optional<float> m_textLength = std::nullopt;
        bool m_bTextLengthPercentage = false;
        std::optional<float> m_fontSize {};
        std::optional<DimensionUnits> m_enumFontSizeUnits {};
        std::optional<Core::String> m_fontFamily {};
        std::optional<TextAnchor> m_textAnchor {};
        std::optional<FontStyle> m_fontStyle {};
    };

}

#endif  /* DRAW2D_SVG_TEXT_DOT_HPP */

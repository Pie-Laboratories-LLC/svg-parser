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

#ifndef DRAW2D_SVG_RAWTEXT_DOT_HPP
#define DRAW2D_SVG_RAWTEXT_DOT_HPP

#include <memory>
#include <variant>
#include <vector>

#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif
#ifndef DRAW2D_SVG_LENGTHADJUST_DOT_HPP
    #include "svg/LengthAdjust.hpp"
#endif

namespace Draw2d::Svg {

    class TSpan;
    struct RawTextParams {
        std::vector<std::variant<Core::String,std::unique_ptr<TSpan>>> texts;

        RawTextParams() = default;
        RawTextParams(RawTextParams &&) noexcept = default;
        RawTextParams &operator=(RawTextParams &&) noexcept = default;
        RawTextParams(const RawTextParams &) = delete;
        RawTextParams &operator=(const RawTextParams &) = delete;

    };

    class RawText {
    public:
        RawText();
        RawText(RawTextParams svgRawTextParams);
        RawText(const RawText &copy) = delete;
        RawText &operator = (const RawText &copy) = delete;
        RawText(RawText &&move) noexcept;
        RawText &operator =(RawText &&move) noexcept;

        const std::vector<std::variant<Core::String,std::unique_ptr<TSpan>>> &getTexts() const { return m_texts; }

    private:
        std::vector<std::variant<Core::String,std::unique_ptr<TSpan>>> m_texts;
    };

    struct SvgTSpanParams {
        RawText rawText;
        std::vector<float> x { };
        std::vector<float> y { };
        std::vector<float> dx { };
        std::vector<float> dy { };
        std::vector<float> rotate {};
        LengthAdjust lengthAdjust = LengthAdjust::Spacing;
        std::optional<float> textLength = std::nullopt;
        bool textLengthPercentage = false;
    };

    class TSpan {
    public:
        TSpan(SvgTSpanParams svgTSpanParams):
            m_rawText(std::move(svgTSpanParams.rawText))
          , m_x(std::move(svgTSpanParams.x))
          , m_y(std::move(svgTSpanParams.y))
          , m_dx(std::move(svgTSpanParams.dx))
          , m_dy(std::move(svgTSpanParams.dy))
          , m_rotate(std::move(svgTSpanParams.rotate))
          , m_enumLengthAdjust(svgTSpanParams.lengthAdjust)
          , m_textLength(svgTSpanParams.textLength)
          , m_bTextLengthPercentage(svgTSpanParams.textLengthPercentage)
        {}

        const RawText &getText() const { return m_rawText; }
        const std::vector<float> &getX() const { return m_x; }
        const std::vector<float> &getY() const { return m_y; }
        const std::vector<float> &getDx() const { return m_dx; }
        const std::vector<float> &getDy() const { return m_dy; }
        const std::vector<float> &getRotate() const { return m_rotate; }
        LengthAdjust getLengthAdjust() const { return m_enumLengthAdjust; }
        std::optional<float> getTextLength() const { return m_textLength; }
        bool getTextLengthPercentage() const { return m_bTextLengthPercentage; }

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
    };

    inline RawText::RawText() = default;
    inline RawText::RawText(RawTextParams svgRawTextParams):
        m_texts(std::move(svgRawTextParams.texts))
    {}
    inline RawText::RawText(RawText &&move) noexcept:
        m_texts(std::move(move.m_texts))
    {}
    inline RawText &RawText::operator =(RawText &&move) noexcept = default;

    // tout le texte d'un <text> ou d'un <tspan>, sous-tspans compris, dans
    // l'ordre du document; les blancs comme xml:space="default" dans un
    // navigateur: fins de ligne et tabulations -> espaces, espaces consécutifs
    // fondus en un, ceux des deux bouts retirés. Sûr pour l'UTF-8: aucun octet
    // d'un caractère sur plusieurs octets n'est un blanc ASCII
    Core::String concatenateText(const RawText &rawText);
}


#endif  /* DRAW2D_SVG_RAWTEXT_DOT_HPP */

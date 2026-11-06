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

#ifndef DRAW2D_SVG_MASK_DOT_HPP
#define DRAW2D_SVG_MASK_DOT_HPP

#include <memory>
#include <optional>
#include <vector>

#ifndef DRAW2D_SVG_SVGCONTAINERENTITY_DOT_HPP
    #include "svg/SvgContainerEntity.hpp"
#endif
#ifndef DRAW2D_SVG_DIMENSION_DOT_HPP
    #include "svg/Dimension.hpp"
#endif
#ifndef GRADIENTUNITS_DOT_HPP
    #include "draw2d/GradientUnits.hpp"
#endif

namespace Draw2d::Svg {

    struct SvgMaskParams: public SvgContainerParams {
        // the mask region; unset parts take the defaults for maskUnits (-10%, -10%, 120%, 120%
        //  of the bounding box for objectBoundingBox)
        std::optional<Dimension> x {};
        std::optional<Dimension> y {};
        std::optional<Dimension> width {};
        std::optional<Dimension> height {};
        // the same two values as gradientUnits
        GradientUnits maskUnits = ObjectBoundingBox;
        GradientUnits maskContentUnits = UserSpaceOnUse;
    };

    /// <mask>: content never drawn by itself.  An element whose mask property names it is
    ///  drawn through it: each pixel's alpha is multiplied by the luminance times alpha of
    ///  the mask's content at that pixel, and nothing shows outside the mask region.
    class Mask: public SvgContainerEntity
    {
    public:
        Mask(SvgMaskParams svgMaskParams):
            SvgContainerEntity(static_cast<SvgContainerParams &&>(svgMaskParams))
           ,m_x(svgMaskParams.x)
           ,m_y(svgMaskParams.y)
           ,m_width(svgMaskParams.width)
           ,m_height(svgMaskParams.height)
           ,m_enumMaskUnits(svgMaskParams.maskUnits)
           ,m_enumMaskContentUnits(svgMaskParams.maskContentUnits)
        {}
        virtual ~Mask() = default;

        std::optional<Dimension> getX() const { return m_x; }
        std::optional<Dimension> getY() const { return m_y; }
        std::optional<Dimension> getWidth() const { return m_width; }
        std::optional<Dimension> getHeight() const { return m_height; }
        GradientUnits getMaskUnits() const { return m_enumMaskUnits; }
        GradientUnits getMaskContentUnits() const { return m_enumMaskContentUnits; }

        virtual SvgEntity *addChild(std::unique_ptr<SvgEntity> pSvgEntity) override {
            m_svgEntities.push_back(std::move(pSvgEntity));
            return m_svgEntities.back().get();
        }
        virtual Core::Generator<const SvgEntity *> enumerateChildren() const override {
            for(const auto &cpEntity : m_svgEntities) {
                co_yield cpEntity.get();
            }
        }

        virtual const char * const getType() const override { return "Mask"; }

    private:
        std::optional<Dimension> m_x {};
        std::optional<Dimension> m_y {};
        std::optional<Dimension> m_width {};
        std::optional<Dimension> m_height {};
        GradientUnits m_enumMaskUnits;
        GradientUnits m_enumMaskContentUnits;
        std::vector<std::unique_ptr<SvgEntity>> m_svgEntities { };
    };

} // namespace Draw2d::Svg

#endif  /* DRAW2D_SVG_MASK_DOT_HPP */

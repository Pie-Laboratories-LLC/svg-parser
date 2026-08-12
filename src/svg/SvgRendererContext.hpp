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

#ifndef DRAW2D_SVG_SVGRENDERERCONTEXT_DOT_HPP
#define DRAW2D_SVG_SVGRENDERERCONTEXT_DOT_HPP

#ifndef DRAW2D_SVG_MATRIXMATHS_DOT_HPP
    #include "svg/MatrixMath.hpp"
#endif
#ifndef CORE_STDUNIQUEPTREXTENSIONS_DOT_HPP
    #include "core/StdUniquePtrExtensions.hpp"
#endif
#ifndef DRAW2D_SVG_SVGEXCEPTION_DOT_HPP
    #include "svg/SvgException.hpp"
#endif

#ifndef CORE_LOGGER_DOT_HPP
    #include "core/Logger.hpp"
#endif
#ifndef DRAW2D_LOGGINGCATEGORIES_DOT_HPP
    #include "draw2d/LoggingCategories.hpp"
#endif

namespace Draw2d::Svg {

    class SvgColour;

    class SvgRendererContext {
    public:
        SvgRendererContext();
        SvgRendererContext(const SvgRendererContext &copy);
        SvgRendererContext &operator =(SvgRendererContext copy) noexcept;
        void swap(SvgRendererContext &copy) noexcept;
        friend void swap(SvgRendererContext &,SvgRendererContext &) noexcept;
        SvgRendererContext(SvgRendererContext &&move) noexcept = default;
        SvgRendererContext& operator =(SvgRendererContext &&move) noexcept = default;
        ~SvgRendererContext() = default;

        const SvgColour &getColour() const { throw SvgException("SvgRendererContext doesn't support colour; at render time, currentColor _must_ have already been resolved."); }

        std::optional<float> getX() const { return m_fX; }
        void setX(std::optional<float> fX) { m_fX = fX; }
        std::optional<float> getY() const { return m_fY; }
        void setY(std::optional<float> fY) { m_fY = fY; }

        const float *getMatrix() const { return m_pMatrix.get(); }
        void rightMultiply(const float *cpMatrix) {
            m_pMatrix = multiply6s<float>(m_pMatrix.get(),cpMatrix);
        }

    private:
        std::optional<float> m_fX {};
        std::optional<float> m_fY {};
        std::unique_ptr<float[]> m_pMatrix = Core::makeUniqueArray<float>({ 1, 0, 0, 1, 0, 0 });
    };
}

#endif  /* DRAW2D_SVG_SVGRENDERERCONTEXT_DOT_HPP */

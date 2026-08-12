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

#include "svg/SvgEntity.hpp"

namespace Draw2d::Svg {

    SvgEntity::SvgEntity(SvgEntityParams params):
        m_svgDocument(params.svgDocument)
       ,m_bRender(params.render)
       ,m_strId(params.id)
       ,m_pMatrix(std::move(params.pMatrix))
       ,m_fillColour(params.fillColour)
       ,m_fFillOpacity(params.fillOpacity)
       ,m_enumFillRule(params.fillRule)
       ,m_strokeColour(params.strokeColour)
       ,m_fStrokeOpacity(params.strokeOpacity)
       ,m_fStrokeWidth(params.strokeWidth)
       ,m_enumLineCap(params.lineCap)
       ,m_enumLineJoin(params.lineJoin)
       ,m_fMiterLimit(params.miterLimit)
       ,m_dashArray(std::move(params.dashArray))
       ,m_strCssClass(params.cssClass)
       ,m_strCssStyle(params.cssStyle)
    {
    }

    SvgEntity::SvgEntity(const SvgEntity &copy):
        m_svgDocument(copy.m_svgDocument)
    {
        m_bRender = copy.m_bRender;
        m_fX = copy.m_fX;
        m_fY = copy.m_fY;
        m_fWidth = copy.m_fWidth;
        m_fHeight = copy.m_fHeight;
        m_fillColour = copy.m_fillColour;
        m_fFillOpacity = copy.m_fFillOpacity;
        m_enumFillRule = copy.m_enumFillRule;
        m_strokeColour = copy.m_strokeColour;
        m_fStrokeOpacity = copy.m_fStrokeOpacity;
        m_fStrokeWidth = copy.m_fStrokeWidth;

        m_enumLineCap = copy.m_enumLineCap;
        m_enumLineJoin = copy.m_enumLineJoin;
        m_fMiterLimit = copy.m_fMiterLimit;
        m_dashArray.clear();
        m_dashArray.reserve(copy.m_dashArray.size());
        for(unsigned i = 0; i < copy.m_dashArray.size(); i++) m_dashArray[i] = copy.m_dashArray[i];
        m_strCssClass = copy.m_strCssClass;
        m_strCssStyle = copy.m_strCssStyle;
    }

    SvgEntity::SvgEntity(SvgEntity &&move):
        m_svgDocument(move.m_svgDocument)
       ,m_bRender(move.m_bRender)
       ,m_strId(move.m_strId)
       ,m_pMatrix(std::move(move.m_pMatrix))
       ,m_upperLeft(move.m_upperLeft)
       ,m_lowerRight(move.m_lowerRight)
       ,m_fX(move.m_fX)
       ,m_fY(move.m_fY)
       ,m_fWidth(move.m_fWidth)
       ,m_fHeight(move.m_fHeight)
       ,m_fillColour(std::move(move.m_fillColour))
       ,m_fFillOpacity(move.m_fFillOpacity)
       ,m_enumFillRule(move.m_enumFillRule)
       ,m_strokeColour(std::move(move.m_strokeColour))
       ,m_fStrokeOpacity(move.m_fStrokeOpacity)
       ,m_fStrokeWidth(move.m_fStrokeWidth)
       ,m_enumLineCap(move.m_enumLineCap)
       ,m_enumLineJoin(move.m_enumLineJoin)
       ,m_fMiterLimit(move.m_fMiterLimit)
       ,m_dashArray(std::move(move.m_dashArray))
       ,m_strCssClass(move.m_strCssClass)
       ,m_strCssStyle(move.m_strCssStyle)
    {}

}

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

namespace PieLaboratories.Svg;

public static class SvgHelpers {
    public static List<SvgEntity> GetChildren(IntPtr container) {
        List<SvgEntity> toReturn = new();
        foreach (var child in SvgParserNative.GetChildren(container)) {
            toReturn.Add(MakeCorrectEntity(child));
        }
        return toReturn;
    }

    public static SvgEntity MakeCorrectEntity(IntPtr entity) {
        switch (SvgParserNative.PtrToString(SvgParserNative.svgparser_entity_get_type(entity))) {
        case "Text": return new Text(entity);
        case "Image": return new Image(entity);
        case "Rect": return new Rect(entity);
        case "Circle": return new Circle(entity);
        case "Ellipse": return new Ellipse(entity);
        case "Svg": return new Svg(entity);
        case "Path": return new Path(entity);
        case "Use": return new Use(entity);
        case "Group": return new Group(entity);
        default: throw new Exception($"Unrecognized type {SvgParserNative.PtrToString(SvgParserNative.svgparser_entity_get_type(entity))}");
        }
    }

    // entity is a raw SvgEntity* handle (Rect/Svg/Use); svgparser_entity_get_dimensioned()
    // resolves it to the SvgDimensionedEntity* sub-object, or IntPtr.Zero if this entity
    // type doesn't have one.
    public static Dimension? GetDimensionedX(IntPtr entity) {
        IntPtr dimensionedEntity = SvgParserNative.svgparser_entity_get_dimensioned(entity);
        if(dimensionedEntity == IntPtr.Zero) return null;
        if(SvgParserNative.svgparser_dimensionedentity_has_x(dimensionedEntity) == 0) return null;
        return new Dimension (
            SvgParserNative.svgparser_dimensionedentity_get_x_measurement(dimensionedEntity),
            SvgParserNative.GetDimensionedXUnits(dimensionedEntity)
        );
    }

    public static Dimension? GetDimensionedY(IntPtr entity) {
        IntPtr dimensionedEntity = SvgParserNative.svgparser_entity_get_dimensioned(entity);
        if(dimensionedEntity == IntPtr.Zero) return null;
        if(SvgParserNative.svgparser_dimensionedentity_has_y(dimensionedEntity) == 0) return null;
        return new Dimension (
            SvgParserNative.svgparser_dimensionedentity_get_y_measurement(dimensionedEntity),
            SvgParserNative.GetDimensionedYUnits(dimensionedEntity)
        );
    }

    public static Dimension? GetDimensionedWidth(IntPtr entity) {
        IntPtr dimensionedEntity = SvgParserNative.svgparser_entity_get_dimensioned(entity);
        if(dimensionedEntity == IntPtr.Zero) return null;
        if(SvgParserNative.svgparser_dimensionedentity_has_width(dimensionedEntity) == 0) return null;
        return new Dimension (
            SvgParserNative.svgparser_dimensionedentity_get_width_measurement(dimensionedEntity),
            SvgParserNative.GetDimensionedWidthUnits(dimensionedEntity)
        );
    }

    public static Dimension? GetDimensionedHeight(IntPtr entity) {
        IntPtr dimensionedEntity = SvgParserNative.svgparser_entity_get_dimensioned(entity);
        if(dimensionedEntity == IntPtr.Zero) return null;
        if(SvgParserNative.svgparser_dimensionedentity_has_height(dimensionedEntity) == 0) return null;
        return new Dimension (
            SvgParserNative.svgparser_dimensionedentity_get_height_measurement(dimensionedEntity),
            SvgParserNative.GetDimensionedHeightUnits(dimensionedEntity)
        );
    }
}

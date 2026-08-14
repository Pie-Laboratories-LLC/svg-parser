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
        case "Rect": return new Rect(entity);
        case "Circle": return new Circle(entity);
        case "Ellipse": return new Ellipse(entity);
        case "Svg": return new Svg(entity);
        case "Path": return new Path(entity);
        case "Use": return new Use(entity);
        case "Group": return new Group(entity);
        default: throw new Exception($"Unrecognized type {SvgParserNative.svgparser_entity_get_type(entity)}");
        }
    }

    public static Dimension? GetDimensionedX(IntPtr dimensionedEntity) {
        if(SvgParserNative.svgparser_dimensionedentity_has_x(dimensionedEntity) == 0) return null;
        return new Dimension (
            SvgParserNative.svgparser_dimensionedentity_get_x_measurement(dimensionedEntity),
            SvgParserNative.GetDimensionedXUnits(dimensionedEntity)
        );
    }

    public static Dimension? GetDimensionedY(IntPtr dimensionedEntity) {
        if(SvgParserNative.svgparser_dimensionedentity_has_y(dimensionedEntity) == 0) return null;
        return new Dimension (
            SvgParserNative.svgparser_dimensionedentity_get_y_measurement(dimensionedEntity),
            SvgParserNative.GetDimensionedXUnits(dimensionedEntity)
        );
    }

    public static Dimension? GetDimensionedWidth(IntPtr dimensionedEntity) {
        if(SvgParserNative.svgparser_dimensionedentity_has_width(dimensionedEntity) == 0) return null;
        return new Dimension (
            SvgParserNative.svgparser_dimensionedentity_get_width_measurement(dimensionedEntity),
            SvgParserNative.GetDimensionedWidthUnits(dimensionedEntity)
        );
    }

    public static Dimension? GetDimensionedHeight(IntPtr dimensionedEntity) {
        if(SvgParserNative.svgparser_dimensionedentity_has_height(dimensionedEntity) == 0) return null;
        return new Dimension (
            SvgParserNative.svgparser_dimensionedentity_get_height_measurement(dimensionedEntity),
            SvgParserNative.GetDimensionedHeightUnits(dimensionedEntity)
        );
    }
}

namespace PieLaboratories.Svg;

public interface ISvgDimensionedEntity {
    Dimension? X { get; }
    Dimension? Y { get; }
    Dimension? Width { get; }
    Dimension? Height { get; }
}

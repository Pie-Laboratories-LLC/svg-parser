namespace PieLaboratories.Svg;

public readonly struct Dimension {
    public float Value { get; }
    public DimensionUnits Units { get; }
    public Dimension(float value, DimensionUnits units) { Value = value; Units = units; }
};

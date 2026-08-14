namespace PieLaboratories.Svg;

public enum PathMove {
    NotAPathMove = -1,
    MoveTo,
    LineTo,
    ClosePath,
    QuadBézierTo,
    SmoothQuadBézierTo,
    CubicBézierTo,
    SmoothCubicBézierTo,
    HorizontalLineTo,
    VerticalLineTo,
    ArcEllipticTo
}

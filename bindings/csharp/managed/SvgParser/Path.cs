namespace PieLaboratories.Svg;

public class Path: SvgEntity {
    IntPtr _path;
    public Path(IntPtr path): base(path) {
        _path = path;
    }

    public PathMove[] PathMoves => SvgParserNative.GetPathMoves(_path);
    public float[] Points {
        get {
            int pointsCount = SvgParserNative.svgparser_path_get_points_count(_path);
            float[] points = new float[pointsCount];
            SvgParserNative.svgparser_path_get_points(_path, points, pointsCount);
            return points;
        }
    }
}

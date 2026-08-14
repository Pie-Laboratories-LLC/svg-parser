namespace PieLaboratories.Svg;

public class SvgColour {
    private IntPtr _colour;

    public SvgColour(IntPtr colour) {
        _colour = colour;
    }

    public SvgColourType SvgColourType => SvgParserNative.GetColourColourType(_colour);
    public int Index {
        get {
            int index = SvgParserNative.svgparser_colour_get_colour_index(_colour);
            if(index == -1) throw new Exception($"Index isn't set on colour of type {SvgColourType}");
            return index;
        }
    }

    public byte[] Bgra {
        get {
            byte[] bgra = new byte[4];
            if(SvgParserNative.svgparser_colour_get_bgra(_colour, bgra) == 0) throw new Exception($"Bgra isn't set on colour of type {SvgColourType}");
            return bgra;
        }
    }

    public float[] Hsla {
        get {
            float[] hsla = new float[4];
            if(SvgParserNative.svgparser_colour_get_hsla(_colour, hsla, hsla.Length) == 0) throw new Exception($"Bgra isn't set on colour of type {SvgColourType}");
            return hsla;
        }
    }

    public SvgColour? Fallback {
        get {
            if(SvgParserNative.svgparser_colour_has_fallback(_colour) == 0) return null;
            return new SvgColour(SvgParserNative.svgparser_colour_get_fallback(_colour));
        }
    }
}

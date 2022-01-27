#pragma once

namespace fw
{
    class Color
    {
    public:
        Color() { r = 0; g = 0; b = 0; a = 0; }
        Color(float nr, float ng, float nb, float na) { r = nr; g = ng; b = nb; a = na; }

        void SetColor(float nr, float ng, float nb, float na) { r = nr; g = ng; b = nb; a = na; }
        
        static const Color Red()       { return Color(1.0f, 0.0f, 0.0f, 1.0f); }
        static const Color Green()     { return Color(0.0f, 1.0f, 0.0f, 1.0f); }
        static const Color Blue()      { return Color(0.0f, 0.0f, 1.0f, 1.0f); }

        static const Color Cyan()      { return Color(0.0f, 1.0f, 1.0f, 1.0f); }
        static const Color Magenta()   { return Color(1.0f, 0.0f, 1.0f, 1.0f); }
        static const Color Yellow()    { return Color(1.0f, 1.0f, 0.0f, 1.0f); }

        static const Color Black()     { return Color(0.0f, 0.0f, 0.0f, 1.0f); }
        static const Color White()     { return Color(1.0f, 1.0f, 1.0f, 1.0f); }
        static const Color Gray()      { return Color(0.5f, 0.5f, 0.5f, 1.0f); }

        static const Color Purple()    { return Color(0.5f, 0.1f, 1.0f, 1.0f); }
        static const Color Orange()    { return Color(1.0f, 0.5f, 0.0f, 1.0f); }
        static const Color DarkBlue()  { return Color(0.0f, 0.0f, 0.3f, 1.0f); }

    public:
        float r;
        float g;
        float b;
        float a;
    };
}
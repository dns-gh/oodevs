#ifndef _GEOMETRY2D_H_
#define _GEOMETRY2D_H_

namespace Geometry2D
{
    typedef struct RectangleInt
    {
        int x, y;
        int w, h;
    }RenderRect;

    typedef struct RectangleFloat
    {
        RectangleFloat( float a = 0, float b = 0, float c = 0, float d = 0 )
            : A_( a )
            , B_( b )
            , C_( c )
            , D_( d )
        {
            // NOTHING
        }
        float A_;
        float B_;
        float C_;
        float D_;
    }Rectangle;
}
#endif // Geometry2D.h

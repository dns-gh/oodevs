#ifndef _GEOMETRY2D_H_
#define _GEOMETRY2D_H_

namespace Geometry2D
{
    typedef struct Vector2D
    {
        Vector2D(){};
        Vector2D( float x, float y );
        float x_;
        float y_;
    }Vector2D;

    typedef struct RectangleInt
    {
        int x, y;
        int w, h;
    }RenderRect;

    typedef struct RectangleFloat
    {
        RectangleFloat( Vector2D a, Vector2D b, Vector2D c, Vector2D d )
            : A_( a )
            , B_( b )
            , C_( c )
            , D_( d )
        {
            // NOTHING
        }
        Vector2D A_;
        Vector2D B_;
        Vector2D C_;
        Vector2D D_;
    }Box;

    typedef struct Circle
    {
        Circle(){};
        Circle( Vector2D center, float radius )
            : center_( center )
            , radius_( radius )
        {
            // NOTHING
        }
        Vector2D center_;
        float radius_;
    }Circle;

    // geometry tools
    float Distance( const Vector2D& vec1, const Vector2D& vec2 );

}
#endif // Geometry2D.h

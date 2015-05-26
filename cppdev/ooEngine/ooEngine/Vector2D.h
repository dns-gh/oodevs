#ifndef _VECTOR2D_H_
#define _VECTOR2D_H_

namespace Geometry2D
{
    struct Vector2D
    {
        Vector2D(){};
        Vector2D( float x, float y );
        Vector2D& operator+= ( const Vector2D& rhs );
        Vector2D operator- () const;
        Vector2D operator* ( float scale ) const;
        Vector2D operator+ ( const Vector2D& vec ) const;
        float x_;
        float y_;
    };
}
#endif // Vector2d.h
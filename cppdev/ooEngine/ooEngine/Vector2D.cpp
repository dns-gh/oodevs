#include "Vector2D.h"

using namespace Geometry2D;

Vector2D::Vector2D( float x, float y )
    : x_( x )
    , y_( y )
{
    // NOTHING
}

Vector2D& Vector2D::operator+= ( const Vector2D& rhs )
{
    x_ += rhs.x_;
    y_ += rhs.y_;
    return *this;
}

Vector2D& Vector2D::operator- ()
{
    x_ = -x_;
    y_ = -y_;
    return *this;
}

Vector2D& Vector2D::operator* ( float scale )
{
    x_ *= scale;
    y_ *= scale;
    return *this;
}

Vector2D& Vector2D::operator+ ( const Vector2D& vec )
{
    x_ += vec.x_;
    y_ += vec.y_;
    return *this;
}
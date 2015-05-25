#include "Geometry2D.h"

#include <math.h>

using namespace Geometry2D;

float Geometry2D::Distance( const Vector2D& vec1, const Vector2D& vec2 )
{
    return sqrt( pow( vec1.x_ - vec2.x_, 2.0f ) + pow( vec1.y_ - vec2.y_, 2.0f ) );
}
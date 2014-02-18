// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Point_h_
#define __Point_h_

#include <cmath>

namespace geometry
{

// =============================================================================
/** @class  Point
    @brief  Point
*/
// Created: AGE 2006-08-17
// =============================================================================
class Point
{

public:
    //! @name Constructors/Destructor
    //@{
     Point() : x_( 0 ), y_( 0 ) {};
     Point( float x, float y ) : x_( x ), y_( y ) {};
     Point( const Point& from, const Point& to ) : x_( to.x_ - from.x_ ), y_( to.y_ - from.y_ ) {};
     ~Point() {};
    //@}

    //! @name Accessors
    //@{
    float  X() const { return x_; };
    float  Y() const { return y_; };
    float& X()       { return x_; };
    float& Y()       { return y_; };
    //@}

    //! @name Operations
    //@{
    float Length() const { return std::sqrt( (*this)*(*this) ); };
    float Distance( const Point& rhs ) const { return Point( *this, rhs ).Length(); }
    //@}

    //! @name Operators
    //@{
    float    operator*( const Point& rhs ) const { return x_ * rhs.x_ + y_ * rhs.y_; };
    Point  operator*( float ratio ) const { return Point( ratio * x_, ratio * y_ ); };
    Point  operator+( const Point& rhs ) const { return Point( x_ + rhs.x_, y_ + rhs.y_ ); };
    Point  operator-( const Point& rhs ) const { return Point( x_ - rhs.x_, y_ - rhs.y_ ); };
    Point& operator+=( const Point& rhs ) { x_+=rhs.x_; y_+=rhs.y_; return *this; };
    Point& operator-=( const Point& rhs ) { x_-=rhs.x_; y_-=rhs.y_; return *this; };
    //@}

private:
    //! @name Member data
    //@{
    float x_;
    float y_;
    //@}
};

inline
Point operator*( float ratio, const Point& p ) { return Point( ratio * p.X(), ratio*p.Y() ); };

}

#endif // __Point_h_

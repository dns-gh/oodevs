// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MT_Circle.h"
#include "MT_Line.h"
#include <cassert>

//-----------------------------------------------------------------------------
// Name: MT_Circle constructor
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
MT_Circle::MT_Circle()
: c_()
, r_(0.0)
{
}


//-----------------------------------------------------------------------------
// Name: MT_Circle constructor
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
MT_Circle::MT_Circle(const MT_Vector2D& c,double r)
: c_(c)
, r_(r)
{
}


//-----------------------------------------------------------------------------
// Name: MT_Circle constructor
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
MT_Circle::MT_Circle(const MT_Vector2D& a,const MT_Vector2D& b)
: c_(a)
, r_(a.Distance(b))
{
}


//-----------------------------------------------------------------------------
// Name: MT_Circle constructor
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
MT_Circle::MT_Circle(double x,double y,double r)
: c_(MT_Vector2D(x,y))
,  r_(r)
{
}


//-----------------------------------------------------------------------------
// Name: MT_Circle constructor
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
MT_Circle::MT_Circle( const MT_Circle& c )
        : c_( c.c_ )
        , r_( c.r_ )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MT_Circle destructor
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
MT_Circle::~MT_Circle()
{
}


#define square(x) (x)*(x)

//-----------------------------------------------------------------------------
// Name: MT_Circle::Intersection
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
T_PointVector MT_Circle::Intersection( const MT_Line& line) const
{
    T_PointVector res;
    double a, b, c, mu, i ;
    MT_Vector2D p1=line.GetPosStart();
    MT_Vector2D p2=line.GetPosEnd();

    if( p1 == p2 )
    {
        if( Inside( p1 ) )
            res.push_back( p1 );
        return res;
    }

    a=  (p2-p1).SquareMagnitude();
    b=  2   *   DotProduct(p2-p1,p1-c_);
    c=  c_.SquareMagnitude() + p1.SquareMagnitude() - 2* DotProduct(c_,p1) - square(r_) ;
    i=  square(b) - 4 * a * c ;

    assert( a != 0 );

    if( i < 0.0 )
    {
        // no intersection
    }
    else if( i == 0.0)
    {
        // one intersection
        mu = -b/(2*a) ;
        res.push_back(MT_Vector2D(p1.rX_+mu*(p2.rX_-p1.rX_),p1.rY_+mu*(p2.rY_-p1.rY_)));

    }
    else if( i > 0.0 )
    {
        double ts = sqrt( i );

        // first intersection
        mu = (-b + ts) / (2*a);
        res.push_back(MT_Vector2D(p1.rX_+mu*(p2.rX_-p1.rX_),p1.rY_+mu*(p2.rY_-p1.rY_)));

        // second intersection
        mu = (-b - ts) / (2*a);
        res.push_back(MT_Vector2D(p1.rX_+mu*(p2.rX_-p1.rX_),p1.rY_+mu*(p2.rY_-p1.rY_)));
    }
    return res;

}


//-----------------------------------------------------------------------------
// Name: MT_Circle::Intersection
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
T_PointVector MT_Circle::Intersection( const MT_Vector2D& p1, const MT_Vector2D& p2 ) const
{
    T_PointVector vect = Intersection( MT_Line( p1, p2 ) );
    T_PointVector res;
    double d = p2.SquareDistance( p1 );
    for( CIT_PointVector cit = vect.begin(); cit != vect.end(); ++cit )
    {
        double rDist1 = p1.SquareDistance( *cit );
        double rDist2 = p2.SquareDistance( *cit );

        if( rDist1 <= d && rDist2 <= d)
            res.push_back( *cit );
        else if( rDist1 < rDist2 && Inside( p1 ) )
            res.push_back( p1 );
        else if( rDist2 <= rDist1 && Inside( p2 ) )
            res.push_back( p2 );
    }
    return res;
}

// -----------------------------------------------------------------------------
// Name: MT_Circle::IntersectionArea
// Created: SBO 2006-01-19
// -----------------------------------------------------------------------------
double MT_Circle::IntersectionArea( const MT_Circle& c ) const
{
    double d = c_.Distance( c.Center() );

    if( d == 0 || d < r_ || d < c.Radius() )
    {
        if( r_ > c.Radius() )
            return MT_PI * c.Radius() * c.Radius();
        else
            return MT_PI * r_ * r_;
    }

    double d1 = ( d * d - c.Radius() * c.Radius() + r_ * r_ ) / ( 2. * d );
    double r1 = r_;
    double r2 = c.Radius();
    if( d1 < 0 )
    {
        d1 = ( d * d - r_ * r_ + c.Radius() * c.Radius() ) / ( 2. * d );
        r2 = r_;
        r1 = c.Radius();
    }
    double d2 = d - d1;

    double area1 = r1 * r1 * acos( d1 / r1 ) - d1 * sqrt( r1 * r1 - d1 * d1 );
    double area2 = r2 * r2 * acos( d2 / r2 ) - d2 * sqrt( r2 * r2 - d2 * d2 );

    return area1 + area2;
}

#undef square

//-----------------------------------------------------------------------------
// Name: MT_Circle::Center
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
MT_Vector2D MT_Circle::Center() const
{
    return c_;
}

//-----------------------------------------------------------------------------
// Name: MT_Circle::Radius
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
double MT_Circle::Radius() const
{
    return r_;
}

//-----------------------------------------------------------------------------
// Name: MT_Circle::IsTrivial
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
bool MT_Circle::IsTrivial() const
{
    return r_==0;
}

//-----------------------------------------------------------------------------
// Name: MT_Circle::Inside
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
bool MT_Circle::Inside( const MT_Vector2D& p ) const
{
    return c_.SquareDistance( p ) <= r_ * r_;
}

//-----------------------------------------------------------------------------
// Name: MT_Circle::Outside
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
bool MT_Circle::Outside( const MT_Vector2D& p ) const
{
    return c_.SquareDistance( p ) >= r_ * r_;
}

//-----------------------------------------------------------------------------
// Name: MT_Circle::Contains
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
bool MT_Circle::Contains( const MT_Vector2D& p ) const
{
    return c_.SquareDistance( p ) == r_ * r_;
}

//-----------------------------------------------------------------------------
// Name: MT_Circle::operator=
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
MT_Circle&  MT_Circle::operator=( const MT_Circle& c )
{
    c_ = c.c_;
    r_ = c.r_;
    return *this;
}

//-----------------------------------------------------------------------------
// Name: MT_Circle::operator=
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
MT_Circle MT_Circle::operator-( const MT_Vector2D& v ) const
{
    return MT_Circle( c_ - v, r_ );
}

//-----------------------------------------------------------------------------
// Name: MT_Circle::operator=
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
MT_Circle MT_Circle::operator+( const MT_Vector2D& v ) const
{
    return MT_Circle( c_ + v, r_ );
}

//-----------------------------------------------------------------------------
// Name: MT_Circle::operator=
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
MT_Circle&  MT_Circle::operator+=( const MT_Vector2D& v )
{
    c_+=v;
    return *this;
}

//-----------------------------------------------------------------------------
// Name: MT_Circle::operator=
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
MT_Circle&  MT_Circle::operator-=( const MT_Vector2D& v )
{
    c_-=v;
    return *this;
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "MT_Vector2D.h"
#include "MT_Vector2DTypes.h"
#include "MT_Droite.h"
#include <cassert>
#include <cmath>
#include <ostream>

// -----------------------------------------------------------------------------
// Name : MT_Vector2D constructor
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
MT_Vector2D::MT_Vector2D()
    : rX_( 0 )
    , rY_( 0 )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name : MT_Vector2D constructor
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
MT_Vector2D::MT_Vector2D( double rX, double rY )
    : rX_( rX )
    , rY_( rY )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name : MT_Vector2D constructor
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
MT_Vector2D::MT_Vector2D( const MT_Vector2D& v )
    : rX_( v.rX_ )
    , rY_( v.rY_ )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name : MT_Vector2D::SquareMagnitude
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
double MT_Vector2D::SquareMagnitude() const
{
    return rX_ * rX_ + rY_ * rY_;
}

//-----------------------------------------------------------------------------
// Name : MT_Vector2D::Magnitude
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
double MT_Vector2D::Magnitude() const
{
    return (double)sqrt( SquareMagnitude() );
}

//-----------------------------------------------------------------------------
// Name: MT_Vector2D::SquareDistance
// Created: JVT 02-08-30
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
double MT_Vector2D::SquareDistance( const MT_Vector2D& vPosDest ) const
{
    return( (vPosDest.rX_ - rX_) * (vPosDest.rX_ - rX_) + (vPosDest.rY_ - rY_) * (vPosDest.rY_ - rY_) );
}

//-----------------------------------------------------------------------------
// Name : MT_Vector2D::Distance
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
double MT_Vector2D::Distance( const MT_Vector2D& vPosDest ) const
{
    return (double)sqrt( SquareDistance( vPosDest ) );
}

//-----------------------------------------------------------------------------
// Name: MT_Vector2D::IsZero
// Created: NLD 2002-11-29
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
bool MT_Vector2D::IsZero() const
{
    return rX_ == 0. && rY_ == 0.;
}

//-----------------------------------------------------------------------------
// Name : MT_Vector2D::Normalized
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
MT_Vector2D MT_Vector2D::Normalized( ) const
{
    double rTmp = Magnitude();
    if( rTmp )
        return (*this) / rTmp;
    else
        return (*this);
}

//-----------------------------------------------------------------------------
// Name: MT_Vector2D::Normalize
// Created: JVT 02-12-06
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
MT_Vector2D& MT_Vector2D::Normalize()
{
    double rTmp = Magnitude();
    if( rTmp )
        return operator /= ( rTmp );
    else
        return *this;
}

//-----------------------------------------------------------------------------
// Name : operator *
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
MT_Vector2D operator* ( double rScalar, const MT_Vector2D& v )
{
    return MT_Vector2D( rScalar * v.rX_, rScalar * v.rY_ );
}

//-----------------------------------------------------------------------------
// Name : operator *
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
MT_Vector2D operator* ( const MT_Vector2D& v, double rScalar )
{
    return MT_Vector2D( rScalar * v.rX_, rScalar * v.rY_ );
}

//-----------------------------------------------------------------------------
// Name : operator +
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
MT_Vector2D operator+ ( const MT_Vector2D& v1, const MT_Vector2D& v2 )
{
    return MT_Vector2D( v1.rX_ + v2.rX_, v1.rY_ + v2.rY_ );
}

//-----------------------------------------------------------------------------
// Name : operator -
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
MT_Vector2D operator- ( const MT_Vector2D& v1, const MT_Vector2D& v2 )
{
    return MT_Vector2D( v1.rX_ - v2.rX_, v1.rY_ - v2.rY_ );
}

//-----------------------------------------------------------------------------
// Name : operator <
//  Warning: Attention operator< sert par defaut aux map et donc pas de distance
//    pour l'operation < en math
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
bool operator <( const MT_Vector2D& v1, const MT_Vector2D& v2 )
{
    return (v1.rX_ < v2.rX_ ||
          !(v2.rX_ < v1.rX_) && v1.rY_ < v2.rY_);
}

//-----------------------------------------------------------------------------
// Name : operator *
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
double operator* ( const MT_Vector2D& v1, const MT_Vector2D& v2)
{
    return v1.rX_ * v2.rX_ + v1.rY_ * v2.rY_;
}

//-----------------------------------------------------------------------------
// Name : DotProduct
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
double DotProduct( const MT_Vector2D& v1, const MT_Vector2D& v2 )
{
    return v1.rX_ * v2.rX_ + v1.rY_ * v2.rY_;
}

// -----------------------------------------------------------------------------
// Name: MT_Vector2D::CrossProduct
// Created: JSR 2011-03-29
// -----------------------------------------------------------------------------
double CrossProduct( const MT_Vector2D& v1, const MT_Vector2D& v2 )
{
    return v1.rX_ * v2.rY_ - v1.rY_ * v2.rX_;
}

// -----------------------------------------------------------------------------
// Name: MT_Vector2D::Angle
// Created: NLD 2004-06-07
// -----------------------------------------------------------------------------
double Angle( const MT_Vector2D& v1, const MT_Vector2D& v2)
{
    double rCosAngle = v1.rX_ * v2.rX_ + v1.rY_ * v2.rY_;
    double rSinAngle = v1.rX_ * v2.rY_ - v1.rY_ * v2.rX_;

    double rTmp = v1.Magnitude() * v2.Magnitude();
    if( rTmp == 0. )
        return 0.;

    rCosAngle /= rTmp;

    if( rCosAngle > 1. )
        rCosAngle = 1.;
    else if( rCosAngle < -1. )
        rCosAngle = -1;

    double rAngle = acos( rCosAngle );
    if( rSinAngle >= 0. )
        return rAngle;
    else
        return /*(2 * MT_PI )*/ - rAngle;
}

//-----------------------------------------------------------------------------
// Name: MT_Vector2D::Reset
// Created: NLD 2002-11-29
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
void MT_Vector2D::Reset()
{
    rX_ = rY_ = 0.;
}

//-----------------------------------------------------------------------------
// Name: MT_Vector2D::operator==
// Created: NLD 2002-11-29
//-----------------------------------------------------------------------------
bool MT_Vector2D::operator==( const MT_Vector2D& v ) const
{
    return MT_IsZero( rX_ - v.rX_ ) && MT_IsZero( rY_ - v.rY_ );
}

//-----------------------------------------------------------------------------
// Name: MT_Vector2D::operator!=
// Created: NLD 2002-11-29
//-----------------------------------------------------------------------------
bool MT_Vector2D::operator!=( const MT_Vector2D& v ) const
{
    return !MT_IsZero( rX_ - v.rX_ ) || !MT_IsZero( rY_ - v.rY_ );
}

//-----------------------------------------------------------------------------
// Name: MT_Vector2D::Rotated
// Created: JVT 02-12-13
//-----------------------------------------------------------------------------
MT_Vector2D MT_Vector2D::Rotated( double rad ) const
{
    double rSin = sin( rad );
    double rCos = cos( rad );

    return MT_Vector2D( rX_ * rCos + rY_ * rSin, rY_ * rCos - rX_ * rSin);
}

//-----------------------------------------------------------------------------
// Name: MT_Vector2D::Rotate
// Created: JVT 02-12-13
//-----------------------------------------------------------------------------
MT_Vector2D& MT_Vector2D::Rotate( double rad )
{
    double rSin = sin( rad );
    double rCos = cos( rad );
    double xmem = rX_;

    rX_ = rX_ * rCos + rY_ * rSin;
    rY_ = rY_ * rCos - xmem * rSin;

    return *this;
}

//-----------------------------------------------------------------------------
// Name: MT_Vector2D::Rotate90ClockWise
// Created: NLD 2003-10-09
//-----------------------------------------------------------------------------
MT_Vector2D& MT_Vector2D::Rotate90ClockWise()
{
    double rTmp = rX_;
    rX_    = rY_;
    rY_    = -rTmp;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: MT_Vector2D MT_Vector2D::Rotated90CleckWise
// Created: JVT 2005-02-16
// -----------------------------------------------------------------------------
MT_Vector2D MT_Vector2D::Rotated90ClockWise() const
{
    return MT_Vector2D( *this ).Rotate90ClockWise();
}

//-----------------------------------------------------------------------------
// Name: MT_Vector2D::Rotate90ClockWise
// Created: NLD 2003-10-09
//-----------------------------------------------------------------------------
MT_Vector2D& MT_Vector2D::Rotate90()
{
    const double rTmp = rX_;
    rX_    = -rY_;
    rY_    = rTmp;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: MT_Vector2D MT_Vector2D::Rotated90
// Created: JVT 2004-10-29
// -----------------------------------------------------------------------------
MT_Vector2D MT_Vector2D::Rotated90() const
{
    return MT_Vector2D( *this ).Rotate90();
}

//-----------------------------------------------------------------------------
// Name: MT_Vector2D::Rotate180
// Created: NLD 2003-10-09
//-----------------------------------------------------------------------------
MT_Vector2D& MT_Vector2D::Rotate180()
{
    rX_ = -rX_;
    rY_ = -rY_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: MT_Vector2D::MT_ComputeBarycenter
// Created: NLD 2003-08-20
// -----------------------------------------------------------------------------
MT_Vector2D MT_ComputeBarycenter( const T_PointVector& points )
{
    MT_Vector2D vBarycenter;

    if( points.empty() )
        return vBarycenter;
    if( points.size() == 1 )
        return points.front();

    CIT_PointVector itEnd      = points.end();
    int nNbrPoints = static_cast< int >( points.size() );
    if( points.front() == points.back() )
    {
        --itEnd;
        --nNbrPoints;
    }
    assert( nNbrPoints > 0 );

    for( CIT_PointVector itPoint = points.begin(); itPoint != itEnd; ++itPoint )
        vBarycenter += *itPoint;
    vBarycenter /= nNbrPoints;
    return vBarycenter;
}

std::ostream& operator<<( std::ostream& out, const MT_Vector2D& vect )
{
    return out << '[' << vect.rX_ << ", " << vect.rY_ << ']';
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "MT_Vector3D.h"
#include <ostream>

//-----------------------------------------------------------------------------
//  Name  :  MT_Vector3D constructor
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
MT_Vector3D::MT_Vector3D()
:   rX_( 0 )
,   rY_( 0 )
,   rZ_( 0 )
{

}

//-----------------------------------------------------------------------------
//  Name  :  MT_Vector3D constructor
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
MT_Vector3D::MT_Vector3D( double rX, double rY, double rZ )
:   rX_( rX )
,   rY_( rY )
,   rZ_( rZ )
{
}

//-----------------------------------------------------------------------------
//  Name  :  MT_Vector3D constructor
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
MT_Vector3D::MT_Vector3D( const MT_Vector3D& v )
: rX_( v.rX_ )
, rY_( v.rY_ )
, rZ_( v.rZ_ )
{
}

//-----------------------------------------------------------------------------
//  Name  :  MT_Vector3D::SquareMagnitude
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
double MT_Vector3D::SquareMagnitude() const
{
    return rX_ * rX_ + rY_ * rY_ + rZ_ * rZ_;
}

//-----------------------------------------------------------------------------
//  Name  :  MT_Vector3D::Magnitude
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
double MT_Vector3D::Magnitude() const
{
    return sqrt( SquareMagnitude() );
}

//-----------------------------------------------------------------------------
// Name: MT_Vector3D::SquareDistance
// Created: JVT 02-08-30
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
double MT_Vector3D::SquareDistance( const MT_Vector3D& vPosDest ) const
{
    return (vPosDest.rX_ - rX_)*(vPosDest.rX_ - rX_) + (vPosDest.rY_ - rY_)*(vPosDest.rY_ - rY_) + (vPosDest.rZ_ - rZ_)*(vPosDest.rZ_ - rZ_);
}

//-----------------------------------------------------------------------------
//  Name  :  MT_Vector3D::Distance
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
double MT_Vector3D::Distance( const MT_Vector3D& vPosDest ) const
{
    return sqrt( SquareDistance( vPosDest) );
}

//-----------------------------------------------------------------------------
// Name: MT_Vector3D::IsZero
// Created: NLD 2002-11-29
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
bool MT_Vector3D::IsZero() const
{
    return !rX_ && !rY_ && !rZ_;
}

//-----------------------------------------------------------------------------
//  Name  :  MT_Vector3D::Normalized
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
MT_Vector3D MT_Vector3D::Normalized() const
{
    if( IsZero() )
        return *this;
    else
        return *this / Magnitude();
}

//-----------------------------------------------------------------------------
// Name: MT_Vector3D::Normalize
// Created: JVT 02-12-06
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
MT_Vector3D& MT_Vector3D::Normalize()
{
    if( IsZero() )
        return *this;
    else
        return operator /= ( Magnitude() );
}

//-----------------------------------------------------------------------------
//  Name  :  operator *
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
MT_Vector3D operator *( double rScalar, const MT_Vector3D& v )
{
    return MT_Vector3D( rScalar * v.rX_, rScalar * v.rY_, rScalar * v.rZ_ );
}

//-----------------------------------------------------------------------------
//  Name  :  operator *
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
MT_Vector3D operator *( const MT_Vector3D& v, double rScalar )
{
    return MT_Vector3D( rScalar * v.rX_, rScalar * v.rY_, rScalar * v.rZ_ );
}

//-----------------------------------------------------------------------------
//  Name  :  operator +
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
MT_Vector3D operator +( const MT_Vector3D& v1, const MT_Vector3D& v2 )
{
    return MT_Vector3D( v1.rX_ + v2.rX_, v1.rY_ + v2.rY_, v1.rZ_ + v2.rZ_ );
}

//-----------------------------------------------------------------------------
//  Name  :  operator -
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
MT_Vector3D operator -( const MT_Vector3D& v1, const MT_Vector3D& v2 )
{
  return MT_Vector3D( v1.rX_ - v2.rX_, v1.rY_ - v2.rY_, v1.rZ_ - v2.rZ_ );
}

//-----------------------------------------------------------------------------
//  Name  :  operator <
//  Warning: Attention operator< sert par defaut aux map et donc pas de distance
//  pour l'operation < en math
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
bool operator <( const MT_Vector3D& v1, const MT_Vector3D& v2 )
{
    return v1.rX_ != v2.rX_ || v1.rY_ != v2.rY_ || v1.rZ_ != v2.rZ_;
}

//-----------------------------------------------------------------------------
//  Name  :  operator *
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
double operator *( const MT_Vector3D& v1, const MT_Vector3D& v2)
{
  return v1.rX_ * v2.rX_ + v1.rY_ * v2.rY_ + v1.rZ_ * v2.rZ_;
}

//-----------------------------------------------------------------------------
//  Name  :  CrossProduct
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
MT_Vector3D CrossProduct( const MT_Vector3D& v1, const MT_Vector3D& v2 )
{
  return MT_Vector3D( v1.rY_ * v2.rZ_ - v2.rY_ * v1.rZ_, v1.rZ_ * v2.rX_ - v2.rZ_ * v1.rX_, v1.rX_ * v2.rY_ - v2.rX_ * v1.rY_ );
}

//-----------------------------------------------------------------------------
//  Name  :  DotProduct
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
double DotProduct( const MT_Vector3D& v1, const MT_Vector3D& v2 )
{
  return v1.rX_ * v2.rX_ + v1.rY_ * v2.rY_ + v1.rZ_ * v2.rZ_;
}

//-----------------------------------------------------------------------------
// Name: MT_Vector3D::Reset
// Created: NLD 2002-11-29
//-----------------------------------------------------------------------------
void MT_Vector3D::Reset()
{
    rX_ = 0;
    rY_ = 0;
    rZ_ = 0;
}

//-----------------------------------------------------------------------------
// Name: MT_Vector3D::operator==
// Created: NLD 2002-11-29
//-----------------------------------------------------------------------------
bool MT_Vector3D::operator ==( const MT_Vector3D& v ) const
{
    return MT_IsZero( rX_ - v.rX_ ) && MT_IsZero( rY_ - v.rY_ ) && MT_IsZero( rZ_ - v.rZ_ );
}

//-----------------------------------------------------------------------------
// Name: MT_Vector3D::operator!=
// Created: NLD 2002-11-29
//-----------------------------------------------------------------------------
bool MT_Vector3D::operator !=( const MT_Vector3D& v ) const
{
    return !MT_IsZero( rX_ - v.rX_ ) || !MT_IsZero( rY_ - v.rY_ ) || !MT_IsZero( rZ_ - v.rZ_ );
}

std::ostream& operator<<( std::ostream& out, const MT_Vector3D& vect )
{
    return out << '[' << vect.rX_ << ", " << vect.rY_ << ", " << vect.rZ_ << ']';
}

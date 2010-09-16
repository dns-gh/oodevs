//*****************************************************************************
//
// $Created: AGN 03-06-12 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Vector2D.inl $
// $Author: Jvt $
// $Modtime: 16/02/05 14:41 $
// $Revision: 27 $
// $Workfile: MT_Vector2D.inl $
//
//*****************************************************************************

#include "MT_Droite.h"
#include <cmath>

// -----------------------------------------------------------------------------
//  Name  :  MT_Vector2D constructor
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
inline
MT_Vector2D::MT_Vector2D( )
    : rX_( 0 )
    , rY_( 0 )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
//  Name  :  MT_Vector2D constructor
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
inline
MT_Vector2D::MT_Vector2D( MT_Float rX, MT_Float rY )
    : rX_( rX )
    , rY_( rY )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
//  Name  :  MT_Vector2D constructor
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
inline
MT_Vector2D::MT_Vector2D( const MT_Vector2D& v )
    : rX_( v.rX_ )
    , rY_( v.rY_ )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
//  Name  :  MT_Vector2D destructor
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
inline
MT_Vector2D::~MT_Vector2D()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
//  Name  :  MT_Vector2D::SquareMagnitude
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
inline
MT_Float MT_Vector2D::SquareMagnitude() const
{ 
    return rX_ * rX_ + rY_ * rY_; 
}

//-----------------------------------------------------------------------------
//  Name  :  MT_Vector2D::Magnitude
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
inline
MT_Float MT_Vector2D::Magnitude() const
{ 
    return (MT_Float)sqrt( SquareMagnitude() );
}

//-----------------------------------------------------------------------------
// Name: MT_Vector2D::SquareDistance
// Created: JVT 02-08-30
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
inline
MT_Float MT_Vector2D::SquareDistance( const MT_Vector2D& vPosDest ) const
{
    return( (vPosDest.rX_ - rX_) * (vPosDest.rX_ - rX_) + (vPosDest.rY_ - rY_) * (vPosDest.rY_ - rY_) );
}

//-----------------------------------------------------------------------------
//  Name  :  MT_Vector2D::Distance
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
inline
MT_Float MT_Vector2D::Distance( const MT_Vector2D& vPosDest ) const
{
    return (MT_Float)sqrt( SquareDistance( vPosDest ) );
}


//-----------------------------------------------------------------------------
// Name: MT_Vector2D::IsZero
// Created: NLD 2002-11-29
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
inline
bool MT_Vector2D::IsZero() const
{
    return rX_ == 0. && rY_ == 0.;
}



//-----------------------------------------------------------------------------
//  Name  :  MT_Vector2D::Normalized
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
inline
MT_Vector2D MT_Vector2D::Normalized( ) const
{ 
    MT_Float rTmp = Magnitude();
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
inline
MT_Vector2D& MT_Vector2D::Normalize()
{
    MT_Float rTmp = Magnitude();
    if( rTmp )
        return operator /= ( rTmp );
    else        
        return *this;
}

//-----------------------------------------------------------------------------
//  Name  :  operator *
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
inline
MT_Vector2D operator* ( MT_Float rScalar, const MT_Vector2D& v )
{
    return MT_Vector2D( rScalar * v.rX_, rScalar * v.rY_ );
}

//-----------------------------------------------------------------------------
//  Name  :  operator *
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
inline
MT_Vector2D operator* ( const MT_Vector2D& v, MT_Float rScalar )
{
    return MT_Vector2D( rScalar * v.rX_, rScalar * v.rY_ );
}

//-----------------------------------------------------------------------------
//  Name  :  operator +
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
inline
MT_Vector2D operator+ ( const MT_Vector2D& v1, const MT_Vector2D& v2 )
{
    return MT_Vector2D( v1.rX_ + v2.rX_, v1.rY_ + v2.rY_ );
}


//-----------------------------------------------------------------------------
//  Name  :  operator -
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
inline
MT_Vector2D operator- ( const MT_Vector2D& v1, const MT_Vector2D& v2 )
{
  return MT_Vector2D( v1.rX_ - v2.rX_, v1.rY_ - v2.rY_ );
}

//-----------------------------------------------------------------------------
//  Name  :  operator <
//  Warning: Attention operator< sert par defaut aux map et donc pas de distance 
//    pour l'operation < en math
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
inline
bool operator <( const MT_Vector2D& v1, const MT_Vector2D& v2 )
{ 
    return (v1.rX_ < v2.rX_ ||
          !(v2.rX_ < v1.rX_) && v1.rY_ < v2.rY_);
}

//-----------------------------------------------------------------------------
//  Name  :  operator *
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
inline
MT_Float operator* ( const MT_Vector2D& v1, const MT_Vector2D& v2)
{
    return v1.rX_ * v2.rX_ + v1.rY_ * v2.rY_;
}

//-----------------------------------------------------------------------------
//  Name  :  DotProduct
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
inline
MT_Float DotProduct( const MT_Vector2D& v1, const MT_Vector2D& v2 )
{
    return v1.rX_ * v2.rX_ + v1.rY_ * v2.rY_;
}

// -----------------------------------------------------------------------------
// Name: MT_Vector2D::Angle
// Created: NLD 2004-06-07
// -----------------------------------------------------------------------------
inline
MT_Float Angle( const MT_Vector2D& v1, const MT_Vector2D& v2)
{
    MT_Float rCosAngle = v1.rX_ * v2.rX_ + v1.rY_ * v2.rY_;
    MT_Float rSinAngle = v1.rX_ * v2.rY_ - v1.rY_ * v2.rX_;
    
    MT_Float rTmp = v1.Magnitude() * v2.Magnitude();
    if( rTmp == 0. )
        return 0.;
    
    rCosAngle /= rTmp;

    if( rCosAngle > 1. )
        rCosAngle = 1.;
    else if( rCosAngle < -1. )
        rCosAngle = -1;

    MT_Float rAngle = acos( rCosAngle );
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
inline
void MT_Vector2D::Reset()
{
    rX_ = rY_ = 0.;
}

//-----------------------------------------------------------------------------
// Name: MT_Vector2D::operator==
// Created: NLD 2002-11-29
//-----------------------------------------------------------------------------
inline
bool MT_Vector2D::operator==( const MT_Vector2D& v ) const    
{ 
    return MT_IsZero( rX_ - v.rX_ ) && MT_IsZero( rY_ - v.rY_ );
}

//-----------------------------------------------------------------------------
// Name: MT_Vector2D::operator!=
// Created: NLD 2002-11-29
//-----------------------------------------------------------------------------
inline
bool MT_Vector2D::operator!=( const MT_Vector2D& v ) const    
{ 
    return !MT_IsZero( rX_ - v.rX_ ) || !MT_IsZero( rY_ - v.rY_ );
}



//-----------------------------------------------------------------------------
// Name: MT_Vector2D::Rotated
// Created: JVT 02-12-13
//-----------------------------------------------------------------------------
inline
MT_Vector2D MT_Vector2D::Rotated( MT_Float rad ) const
{
    MT_Float rSin = sin( rad );
    MT_Float rCos = cos( rad );

    return MT_Vector2D( rX_ * rCos + rY_ * rSin, rY_ * rCos - rX_ * rSin);
}


//-----------------------------------------------------------------------------
// Name: MT_Vector2D::Rotate
// Created: JVT 02-12-13
//-----------------------------------------------------------------------------
inline
MT_Vector2D& MT_Vector2D::Rotate( MT_Float rad )
{
    MT_Float rSin = sin( rad );
    MT_Float rCos = cos( rad );
    MT_Float xmem = rX_;

    rX_ = rX_ * rCos + rY_ * rSin;
    rY_ = rY_ * rCos - xmem * rSin;

    return *this;
}


//-----------------------------------------------------------------------------
// Name: MT_Vector2D::Rotate90ClockWise
// Created: NLD 2003-10-09
//-----------------------------------------------------------------------------
inline
MT_Vector2D& MT_Vector2D::Rotate90ClockWise()
{
    MT_Float rTmp = rX_;
    rX_    = rY_;
    rY_    = -rTmp; 
    return *this;
}

// -----------------------------------------------------------------------------
// Name: MT_Vector2D MT_Vector2D::Rotated90CleckWise
// Created: JVT 2005-02-16
// -----------------------------------------------------------------------------
inline
MT_Vector2D MT_Vector2D::Rotated90ClockWise() const
{
    return MT_Vector2D( *this ).Rotate90ClockWise();
}


//-----------------------------------------------------------------------------
// Name: MT_Vector2D::Rotate90ClockWise
// Created: NLD 2003-10-09
//-----------------------------------------------------------------------------
inline
MT_Vector2D& MT_Vector2D::Rotate90()
{
    const MT_Float rTmp = rX_;
    rX_    = -rY_;
    rY_    = rTmp; 
    return *this;
}


// -----------------------------------------------------------------------------
// Name: MT_Vector2D MT_Vector2D::Rotated90
// Created: JVT 2004-10-29
// -----------------------------------------------------------------------------
inline
MT_Vector2D MT_Vector2D::Rotated90() const
{
    return MT_Vector2D( *this ).Rotate90();
}

//-----------------------------------------------------------------------------
// Name: MT_Vector2D::Rotate180
// Created: NLD 2003-10-09
//-----------------------------------------------------------------------------
inline
MT_Vector2D& MT_Vector2D::Rotate180()
{
    rX_ = -rX_;
    rY_ = -rY_;
    return *this;
}

//-----------------------------------------------------------------------------
// Name: MT_Vector2D::ToStdVector
// Created: MGD 2009-10-31
//-----------------------------------------------------------------------------
inline
std::vector< MT_Float > MT_Vector2D::ToStdVector() const
{
    std::vector< MT_Float >position;
    position.push_back( rX_);
    position.push_back( rY_);
    position.push_back( 0. );
    return position;
}

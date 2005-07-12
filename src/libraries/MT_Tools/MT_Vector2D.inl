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


//-----------------------------------------------------------------------------
//  Name  :  MT_V2D constructor
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
template <class T>
MT_V2D<T>::MT_V2D( void )
:	rX_( 0 )
,	rY_( 0 )

{	
}


//-----------------------------------------------------------------------------
//  Name  :  MT_V2D constructor
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
template <class T>
MT_V2D<T>::MT_V2D( T rX, T rY )
:	rX_( rX )
,	rY_( rY )
{
}



//-----------------------------------------------------------------------------
//  Name  :  MT_V2D constructor
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
template <class T>
MT_V2D<T>::MT_V2D( const MT_V2D<T>& v )
: rX_( v.rX_ )
, rY_( v.rY_ )
{
}


//-----------------------------------------------------------------------------
//  Name  :  MT_V2D destructor
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
template <class T>
MT_V2D<T>::~MT_V2D( void )
{
}

//-----------------------------------------------------------------------------
//  Name  :  MT_V2D::SquareMagnitude
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
template <class T>
inline
T MT_V2D<T>::SquareMagnitude( void ) const
{ 
	return rX_ * rX_ + rY_ * rY_; 
}


//-----------------------------------------------------------------------------
//  Name  :  MT_V2D::Magnitude
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
template <class T>
inline
T MT_V2D<T>::Magnitude( void ) const
{ 
	return (T)sqrt( SquareMagnitude() );
}


//-----------------------------------------------------------------------------
// Name: MT_V2D::SquareDistance
// Created: JVT 02-08-30
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
template <class T>
inline
T MT_V2D<T>::SquareDistance( const MT_V2D<T>& vPosDest ) const
{
    return( (vPosDest.rX_ - rX_) * (vPosDest.rX_ - rX_) + (vPosDest.rY_ - rY_) * (vPosDest.rY_ - rY_) );
}

//-----------------------------------------------------------------------------
//  Name  :  MT_V2D::Distance
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
template <class T>
inline
T MT_V2D<T>::Distance( const MT_V2D<T>& vPosDest ) const
{
    return (T)sqrt( SquareDistance( vPosDest ) );
}


//-----------------------------------------------------------------------------
// Name: MT_Vector2D::IsZero
// Created: NLD 2002-11-29
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
template <class T>
inline
bool MT_V2D<T>::IsZero() const
{
    return rX_ == 0. && rY_ == 0.;
}



//-----------------------------------------------------------------------------
//  Name  :  MT_V2D::Normalized
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
template <class T>
MT_V2D<T> MT_V2D<T>::Normalized( ) const
{ 
    T rTmp = Magnitude();
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
template <class T>
MT_V2D<T>& MT_V2D<T>::Normalize()
{
    T rTmp = Magnitude();
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
template <class T>
inline
MT_V2D<T> operator* ( T rScalar, const MT_V2D<T>& v )
{
	return MT_V2D<T>( rScalar * v.rX_, rScalar * v.rY_ );
}

//-----------------------------------------------------------------------------
//  Name  :  operator *
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
template <class T>
inline
MT_V2D<T> operator* ( const MT_V2D<T>& v, T rScalar )
{
	return MT_V2D<T>( rScalar * v.rX_, rScalar * v.rY_ );
}

//-----------------------------------------------------------------------------
//  Name  :  operator +
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
template <class T>
inline
MT_V2D<T> operator+ ( const MT_V2D<T>& v1, const MT_V2D<T>& v2 )
{
	return MT_V2D<T>( v1.rX_ + v2.rX_, v1.rY_ + v2.rY_ );
}


//-----------------------------------------------------------------------------
//  Name  :  operator -
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
template <class T>
inline
MT_V2D<T> operator- ( const MT_V2D<T>& v1, const MT_V2D<T>& v2 )
{
  return MT_V2D<T>( v1.rX_ - v2.rX_, v1.rY_ - v2.rY_ );
}

//-----------------------------------------------------------------------------
//  Name  :  operator <
//  Warning: Attention operator< sert par defaut aux map et donc pas de distance 
//	pour l'operation < en math
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
template <class T>
inline
bool operator <( const MT_V2D<T>& v1, const MT_V2D<T>& v2 )
{ 
	return ( v1.rX_ != v2.rX_ || v1.rY_ != v2.rY_ );
    // $$$$ JVT : Ca me paraît bizzare comme test : cf la binary_function que j'ai implémentée pour le même operateur dans le .h
}


//-----------------------------------------------------------------------------
//  Name  :  operator *
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
template <class T>
inline
T operator* ( const MT_V2D<T>& v1, const MT_V2D<T>& v2)
{
    return v1.rX_ * v2.rX_ + v1.rY_ * v2.rY_;
}

//-----------------------------------------------------------------------------
//  Name  :  DotProduct
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
template <class T>
inline
T DotProduct( const MT_V2D<T>& v1, const MT_V2D<T>& v2 )
{
    return v1.rX_ * v2.rX_ + v1.rY_ * v2.rY_;
}

// -----------------------------------------------------------------------------
// Name: MT_Vector2D::Angle
// Created: NLD 2004-06-07
// -----------------------------------------------------------------------------
template <class T>
inline
T Angle( const MT_V2D<T>& v1, const MT_V2D<T>& v2)
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
// Name: MT_V2D<T>::Write
// Created: FBD 02-11-15
//-----------------------------------------------------------------------------
template <class T>
int MT_V2D<T>::Write( MT_OutputArchive_ABC& archive ) const
{
    archive.WriteField( "X", rX_ );
    archive.WriteField( "Y", rY_ );
    return 0;
}

//-----------------------------------------------------------------------------
// Name: MT_V2D<T>::Read
// Created: FBD 02-11-15
//-----------------------------------------------------------------------------
template <class T>
int MT_V2D<T>::Read( MT_InputArchive_ABC& archive )
{
    archive.ReadField( "X", rX_ );
    archive.ReadField( "Y", rY_ );
    return 0;
}


//-----------------------------------------------------------------------------
// Name: MT_Vector2D::Reset
// Created: NLD 2002-11-29
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
template <class T>
inline
void MT_V2D<T>::Reset()
{
    rX_ = rY_ = 0.;
}

//-----------------------------------------------------------------------------
// Name: MT_Vector2D::operator==
// Created: NLD 2002-11-29
//-----------------------------------------------------------------------------
template <class T>
inline
bool MT_V2D<T>::operator==( const MT_V2D<T>& v ) const    
{ 
    return MT_IsZero( rX_ - v.rX_ ) && MT_IsZero( rY_ - v.rY_ );
}

//-----------------------------------------------------------------------------
// Name: MT_Vector2D::operator!=
// Created: NLD 2002-11-29
//-----------------------------------------------------------------------------
template <class T>
inline
bool MT_V2D<T>::operator!=( const MT_V2D<T>& v ) const    
{ 
    return !MT_IsZero( rX_ - v.rX_ ) || !MT_IsZero( rY_ - v.rY_ );
}



//-----------------------------------------------------------------------------
// Name: MT_Vector2D::Rotated
// Created: JVT 02-12-13
//-----------------------------------------------------------------------------
template <class T>
MT_V2D<T> MT_V2D<T>::Rotated( MT_Float rad ) const
{
    MT_Float rSin = sin( rad );
    MT_Float rCos = cos( rad );

    return MT_V2D<T>( rX_ * rCos + rY_ * rSin, rY_ * rCos - rX_ * rSin);
}


//-----------------------------------------------------------------------------
// Name: MT_Vector2D::Rotate
// Created: JVT 02-12-13
//-----------------------------------------------------------------------------
template <class T>
MT_V2D<T>& MT_V2D<T>::Rotate( MT_Float rad )
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
template <class T>
MT_V2D<T>& MT_V2D<T>::Rotate90ClockWise()
{
    T rTmp = rX_;
    rX_    = rY_;
    rY_    = -rTmp; 
    return *this;
}

// -----------------------------------------------------------------------------
// Name: MT_V2D<T> MT_Vector2D::Rotated90CleckWise
// Created: JVT 2005-02-16
// -----------------------------------------------------------------------------
template< typename T >
inline
MT_V2D<T> MT_V2D<T>::Rotated90ClockWise() const
{
    return MT_V2D<T>( *this ).Rotate90ClockWise();
}


//-----------------------------------------------------------------------------
// Name: MT_Vector2D::Rotate90ClockWise
// Created: NLD 2003-10-09
//-----------------------------------------------------------------------------
template <class T>
MT_V2D<T>& MT_V2D<T>::Rotate90()
{
    T rTmp = rX_;
    rX_    = -rY_;
    rY_    = rTmp; 
    return *this;
}


// -----------------------------------------------------------------------------
// Name: MT_V2D<T> MT_Vector2D::Rotated90
// Created: JVT 2004-10-29
// -----------------------------------------------------------------------------
template < typename T >
inline
MT_V2D<T> MT_V2D<T>::Rotated90() const
{
    return MT_V2D<T>( *this ).Rotate90();
}

//-----------------------------------------------------------------------------
// Name: MT_Vector2D::Rotate180
// Created: NLD 2003-10-09
//-----------------------------------------------------------------------------
template <class T>
MT_V2D<T>& MT_V2D<T>::Rotate180()
{
    rX_ = -rX_;
    rY_ = -rY_;
    return *this;
}

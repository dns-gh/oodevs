//*****************************************************************************
//
// $Created: AGN 03-06-12 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Vector3D.inl $
// $Author: Agn $
// $Modtime: 12/06/03 17:30 $
// $Revision: 12 $
// $Workfile: MT_Vector3D.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
//  Name  :  MT_V3D constructor
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
template <class T>
MT_V3D<T>::MT_V3D()
:   rX_( 0 )
,   rY_( 0 )
,   rZ_( 0 )
{

}


//-----------------------------------------------------------------------------
//  Name  :  MT_V3D constructor
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
template <class T>
MT_V3D<T>::MT_V3D( T rX, T rY, T rZ )
:   rX_( rX )
,   rY_( rY )
,   rZ_( rZ )
{
}



//-----------------------------------------------------------------------------
//  Name  :  MT_V3D constructor
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
template <class T>
MT_V3D<T>::MT_V3D( const MT_V3D<T>& v )
: rX_( v.rX_ )
, rY_( v.rY_ )
, rZ_( v.rZ_ )
{
}


//-----------------------------------------------------------------------------
//  Name  :  MT_V3D destructor
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
template <class T>
MT_V3D<T>::~MT_V3D()
{
}

//-----------------------------------------------------------------------------
//  Name  :  MT_V3D::SquareMagnitude
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
template <class T>
inline
T MT_V3D<T>::SquareMagnitude() const
{
    return rX_ * rX_ + rY_ * rY_ + rZ_ * rZ_;
}


//-----------------------------------------------------------------------------
//  Name  :  MT_V3D::Magnitude
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
template <class T>
inline
T MT_V3D<T>::Magnitude() const
{
    return (T)sqrt( SquareMagnitude() );
}


//-----------------------------------------------------------------------------
// Name: MT_V3D::SquareDistance
// Created: JVT 02-08-30
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
template <class T>
inline
T MT_V3D<T>::SquareDistance( const MT_V3D<T>& vPosDest ) const
{
    return (vPosDest.rX_ - rX_)*(vPosDest.rX_ - rX_) + (vPosDest.rY_ - rY_)*(vPosDest.rY_ - rY_) + (vPosDest.rZ_ - rZ_)*(vPosDest.rZ_ - rZ_);
}

//-----------------------------------------------------------------------------
//  Name  :  MT_V3D::Distance
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
template <class T>
inline
T MT_V3D<T>::Distance( const MT_V3D<T>& vPosDest ) const
{
    return (T)sqrt( SquareDistance( vPosDest) );
}


//-----------------------------------------------------------------------------
// Name: MT_V3D::IsZero
// Created: NLD 2002-11-29
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
template <class T>
inline
bool MT_V3D<T>::IsZero() const
{
    return rX_ == (T)0 && rY_ == (T)0 && rZ_ == (T)0;
}

//-----------------------------------------------------------------------------
//  Name  :  MT_V3D::Normalized
// Created: FBD 02-03-01
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
template <class T>
MT_V3D<T> MT_V3D<T>::Normalized() const
{
    if( IsZero() )
        return (*this);
    else
        return (*this) / Magnitude();
}


//-----------------------------------------------------------------------------
// Name: MT_Vector3D::Normalize
// Created: JVT 02-12-06
// Last modified: AGN 03-06-12
//-----------------------------------------------------------------------------
template <class T>
MT_V3D<T>& MT_V3D<T>::Normalize()
{
    if( IsZero() )
        return (*this);
    else
        return operator /= ( Magnitude() );
}

//-----------------------------------------------------------------------------
//  Name  :  operator *
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
template <class T>
inline
MT_V3D<T> operator* ( T rScalar, const MT_V3D<T>& v )
{
    return MT_V3D<T>( rScalar * v.rX_, rScalar * v.rY_, rScalar * v.rZ_ );
}

//-----------------------------------------------------------------------------
//  Name  :  operator *
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
template <class T>
inline
MT_V3D<T> operator* ( const MT_V3D<T>& v, T rScalar )
{
    return MT_V3D<T>( rScalar * v.rX_, rScalar * v.rY_, rScalar * v.rZ_ );
}

//-----------------------------------------------------------------------------
//  Name  :  operator +
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
template <class T>
inline
MT_V3D<T> operator+ ( const MT_V3D<T>& v1, const MT_V3D<T>& v2 )
{
    return MT_V3D<T>( v1.rX_ + v2.rX_, v1.rY_ + v2.rY_, v1.rZ_ + v2.rZ_ );
}


//-----------------------------------------------------------------------------
//  Name  :  operator -
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
template <class T>
inline
MT_V3D<T> operator- ( const MT_V3D<T>& v1, const MT_V3D<T>& v2 )
{
  return MT_V3D<T>( v1.rX_ - v2.rX_, v1.rY_ - v2.rY_, v1.rZ_ - v2.rZ_ );
}

//-----------------------------------------------------------------------------
//  Name  :  operator <
//  Warning: Attention operator< sert par defaut aux map et donc pas de distance
//  pour l'operation < en math
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
template <class T>
inline
bool operator <( const MT_V3D<T>& v1, const MT_V3D<T>& v2 )
{
    return ( v1.rX_ != v2.rX_ || v1.rY_ != v2.rY_ || v1.rZ_ != v2.rZ_ );
}


//-----------------------------------------------------------------------------
//  Name  :  operator *
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
template <class T>
inline
T operator* ( const MT_V3D<T>& v1, const MT_V3D<T>& v2)
{
  return v1.rX_ * v2.rX_ + v1.rY_ * v2.rY_ + v1.rZ_ * v2.rZ_;
}

//-----------------------------------------------------------------------------
//  Name  :  CrossProduct
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
template <class T>
inline
MT_V3D<T> CrossProduct( const MT_V3D<T>& v1, const MT_V3D<T>& v2 )
{
  return MT_V3D<T>( v1.rY_ * v2.rZ_ - v2.rY_ * v1.rZ_, v1.rZ_ * v2.rX_ - v2.rZ_ * v1.rX_, v1.rX_ * v2.rY_ - v2.rX_ * v1.rY_ );
}

//-----------------------------------------------------------------------------
//  Name  :  DotProduct
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
template <class T>
inline
T DotProduct( const MT_V3D<T>& v1, const MT_V3D<T>& v2 )
{
  return v1.rX_ * v2.rX_ + v1.rY_ * v2.rY_ + v1.rZ_ * v2.rZ_;
}


//-----------------------------------------------------------------------------
// Name: MT_Vector3D::Reset
// Created: NLD 2002-11-29
//-----------------------------------------------------------------------------
template <class T>
inline
void MT_V3D<T>::Reset()
{
    rX_ = (T)0;
    rY_ = (T)0;
    rZ_ = (T)0;
}

//-----------------------------------------------------------------------------
// Name: MT_Vector3D::operator==
// Created: NLD 2002-11-29
//-----------------------------------------------------------------------------
template <class T>
inline
bool MT_V3D<T>::operator==( const MT_V3D<T>& v ) const
{
    return MT_IsZero( rX_ - v.rX_ ) && MT_IsZero( rY_ - v.rY_ ) && MT_IsZero( rZ_ - v.rZ_ );
}

//-----------------------------------------------------------------------------
// Name: MT_Vector3D::operator!=
// Created: NLD 2002-11-29
//-----------------------------------------------------------------------------
template <class T>
inline
bool MT_V3D<T>::operator!=( const MT_V3D<T>& v ) const
{
    return !MT_IsZero( rX_ - v.rX_ ) || !MT_IsZero( rY_ - v.rY_ ) || !MT_IsZero( rZ_ - v.rZ_ );
}

//*****************************************************************************
//
// $Created: AGN 03-06-12 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Vector3D.h $
// $Author: Agn $
// $Modtime: 12/06/03 17:38 $
// $Revision: 18 $
// $Workfile: MT_Vector3D.h $
//
//*****************************************************************************

#ifndef __MT_V3D_h_
#define __MT_V3D_h_

#include "MT/MT_Tools/MT_Types.h"
#include "MT_Tools_Types.h"

// Scipio types
//=============================================================================
// Created: FBD 02-03-01
//=============================================================================

template <class T>
class MT_V3D
{

public:

    explicit        MT_V3D( void );                                 // Constructor : les valeurs XYZ sont mises a 0
                    MT_V3D( T rX, T rY, T rZ );                     // Constructor : X Y Z ne ser<T> rien ce commentaire !!!!
                    MT_V3D( const MT_V3D<T>& v );                   // Constructor : Vector3D
                    ~MT_V3D( void );                                // Destructor :

    T               SquareMagnitude( void ) const;                  // Retourne la magnitude au carré d'un vector 3D ( longeur )
    T               Magnitude( void ) const;                        // Retourne la magnitude d'un vector 3D ( longeur )
    T               Distance( const MT_V3D<T>& vPosDest ) const;    // Retourne la distance entre deux vector 3D
    T               SquareDistance( const MT_V3D<T>& vPosDest ) const;
    MT_V3D<T>       Normalized( void ) const;
    MT_V3D<T>&      Normalize();
    bool            IsZero() const;
    void            Reset();

    MT_V3D<T>&  operator =( const MT_V3D<T>& v )            { rX_ = v.rX_; rY_ = v.rY_; rZ_ = v.rZ_; return *this; }
    MT_V3D<T>&  operator +=( const MT_V3D<T>& v )           { rX_ += v.rX_; rY_ += v.rY_; rZ_ += v.rZ_; return *this; }
    MT_V3D<T>&  operator -=( const MT_V3D<T>& v )           { rX_ -= v.rX_; rY_ -= v.rY_; rZ_ -= v.rZ_; return *this; }
    MT_V3D<T>&  operator *=( T s )                          { rX_ *= s; rY_ *= s; rZ_ *= s; return *this; }
    MT_V3D<T>&  operator /=( T s )                          { return *this *= ( 1.0f / s ); }
    MT_V3D<T>   operator / ( T s )  const                   { return MT_V3D<T>( rX_ / s, rY_ / s, rZ_ / s ); }
    bool        operator ==( const MT_V3D<T>& v ) const;
    bool        operator !=( const MT_V3D<T>& v ) const;
    T           operator []( int i ) const                  { return ( (T*)&rX_ )[i]; }
    T&          operator []( int i )                        { return ( (T*)&rX_ )[i]; }
    MT_V3D<T>   operator - ( void ) const                   { return MT_V3D<T>( -rX_, -rY_, -rZ_ ); }
    bool        operator < ( const MT_V3D<T>& v ) const     { return ( rX_ != v.rX_ || rY_ != v.rY_ || rZ_ != v.rZ_ ); }// Attention < sert par defaut aux map et donc pas de distance pour l'operationr <
    
    
    T               rX_;
    T               rY_;
    T               rZ_;

private:

};

template < class T >
MT_V3D<T>   operator *( T rScalar, const MT_V3D<T>& v);
template < class T >
T               operator *( const MT_V3D<T>& v1, const MT_V3D<T>& v2 );
template < class T >
MT_V3D<T> operator *( const MT_V3D<T>& v, T rScalar );
template < class T >
MT_V3D<T>   operator +( const MT_V3D<T>& v1, const MT_V3D<T>& v2 );
template < class T >
MT_V3D<T>   operator -( const MT_V3D<T>& v1, const MT_V3D<T>& v2 );
template < class T >
bool        operator <( const MT_V3D<T>& v1, const MT_V3D<T>& v2 );
template < class T >
MT_V3D<T>       CrossProduct( const MT_V3D<T>& v1, const MT_V3D<T>& v2);
template < class T >
T                   DotProduct( const MT_V3D<T>& v1, const MT_V3D<T>& v2);

#include "MT_Vector3D.inl"

// Scipio types
typedef MT_V3D<MT_Float>        MT_Vector3D;

typedef std::vector< MT_Vector3D >                T_Point3DVector;
typedef const T_Point3DVector                     CT_Point3DVector;
typedef T_Point3DVector::iterator                 IT_Point3DVector;
typedef T_Point3DVector::const_iterator           CIT_Point3DVector;
typedef T_Point3DVector::reverse_iterator         RIT_Point3DVector;
typedef T_Point3DVector::const_reverse_iterator   CRIT_Point3DVector;

#endif // __MT_V3D_h_

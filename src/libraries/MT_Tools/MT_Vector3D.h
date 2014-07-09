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

#ifndef __MT_Vector3D_h_
#define __MT_Vector3D_h_

#include "MT_Tools_Types.h"
#include <iosfwd>
#include <vector>

// Scipio types
//=============================================================================
// Created: FBD 02-03-01
//=============================================================================
class MT_Vector3D
{
public:
    MT_Vector3D();                                  // Constructor : les valeurs XYZ sont mises a 0
    MT_Vector3D( double rX, double rY, double rZ ); // Constructor : X Y Z ne ser<double> rien ce commentaire !!!!
    MT_Vector3D( const MT_Vector3D& v );            // Constructor : Vector3D

    double          SquareMagnitude() const;                        // Retourne la magnitude au carré d'un vector 3D ( longeur )
    double          Magnitude() const;                              // Retourne la magnitude d'un vector 3D ( longeur )
    double          Distance( const MT_Vector3D& vPosDest ) const;  // Retourne la distance entre deux vector 3D
    double          SquareDistance( const MT_Vector3D& vPosDest ) const;
    MT_Vector3D     Normalized( void ) const;
    MT_Vector3D&    Normalize();
    bool            IsZero() const;
    void            Reset();

    MT_Vector3D&    operator  =( const MT_Vector3D& v )       { rX_  = v.rX_; rY_  = v.rY_; rZ_  = v.rZ_; return *this; }
    MT_Vector3D&    operator +=( const MT_Vector3D& v )       { rX_ += v.rX_; rY_ += v.rY_; rZ_ += v.rZ_; return *this; }
    MT_Vector3D&    operator -=( const MT_Vector3D& v )       { rX_ -= v.rX_; rY_ -= v.rY_; rZ_ -= v.rZ_; return *this; }
    MT_Vector3D&    operator *=( double s )                   { rX_ *= s; rY_ *= s; rZ_ *= s; return *this; }
    MT_Vector3D&    operator /=( double s )                   { return *this *= 1 / s; }
    MT_Vector3D     operator  /( double s )  const            { return MT_Vector3D( rX_ / s, rY_ / s, rZ_ / s ); }
    MT_Vector3D     operator  -() const                       { return MT_Vector3D( -rX_, -rY_, -rZ_ ); }
    bool            operator  <( const MT_Vector3D& v ) const { return rX_ != v.rX_ || rY_ != v.rY_ || rZ_ != v.rZ_; }// Attention < sert par defaut aux map et donc pas de distance pour l'operationr <
    bool            operator ==( const MT_Vector3D& v ) const;
    bool            operator !=( const MT_Vector3D& v ) const;

    double rX_;
    double rY_;
    double rZ_;
};

MT_Vector3D operator   *( double rScalar, const MT_Vector3D& v );
MT_Vector3D operator   *( const MT_Vector3D& v, double rScalar );
double      operator   *( const MT_Vector3D& v1, const MT_Vector3D& v2 );
MT_Vector3D operator   +( const MT_Vector3D& v1, const MT_Vector3D& v2 );
MT_Vector3D operator   -( const MT_Vector3D& v1, const MT_Vector3D& v2 );
bool        operator   <( const MT_Vector3D& v1, const MT_Vector3D& v2 );
MT_Vector3D CrossProduct( const MT_Vector3D& v1, const MT_Vector3D& v2 );
double      DotProduct  ( const MT_Vector3D& v1, const MT_Vector3D& v2 );

std::ostream& operator<<( std::ostream&, const MT_Vector3D& );

#include "MT_Vector3D.inl"

typedef std::vector< MT_Vector3D > T_Point3DVector;

#endif // __MT_Vector3D_h_

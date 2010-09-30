//*****************************************************************************
//
// $Created: AGN 03-06-12 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Vector2D.h $
// $Author: Jvt $
// $Modtime: 16/02/05 14:41 $
// $Revision: 29 $
// $Workfile: MT_Vector2D.h $
//
//*****************************************************************************

#ifndef __MT_V2D_h_
#define __MT_V2D_h_

#include "MT_Tools_Types.h"
#include <vector>

//=============================================================================
// Created: FBD 02-03-01
//=============================================================================
class MT_Vector2D
{
public:
     MT_Vector2D();                           // Constructor : les valeurs XYZ sont mises a 0
     MT_Vector2D( MT_Float rX, MT_Float rY ); // Constructor : X Y Z ne ser<T> rien ce commentaire !!!!
     MT_Vector2D( const MT_Vector2D& v );     // Constructor : Vector2D
    virtual ~MT_Vector2D();                           // Destructor :

    MT_Float          SquareMagnitude   () const;                  // Retourne la magnitude au carré d'un vector 2D ( longeur )
    MT_Float          Magnitude         () const;                        // Retourne la magnitude d'un vector 2D ( longeur )
    MT_Float          Distance          ( const MT_Vector2D& vPosDest ) const;    // Retourne la distance entre deux vector 2D
    MT_Float          SquareDistance    ( const MT_Vector2D& vPosDest ) const;
    MT_Vector2D       Normalized        () const;
    MT_Vector2D&      Normalize         ();
    MT_Vector2D       Rotated           ( MT_Float rad ) const; // Dans le sens des aiguilles d'une montre
    MT_Vector2D&      Rotate            ( MT_Float rad ); // Dans le sens des aiguilles d'une montre
    MT_Vector2D&      Rotate90ClockWise ();
    MT_Vector2D       Rotated90ClockWise() const;
    MT_Vector2D&      Rotate90          ();
    MT_Vector2D       Rotated90         () const;
    MT_Vector2D&      Rotate180         ();
    std::vector< MT_Float > ToStdVector() const;
    bool            IsZero() const;
    void            Reset();

    //! @name Operators
    //@{
    MT_Vector2D&  operator = ( const MT_Vector2D& v )         { rX_ = v.rX_; rY_ = v.rY_; return *this; }
    MT_Vector2D&  operator +=( const MT_Vector2D& v )         { rX_ += v.rX_; rY_ += v.rY_; return *this; }
    MT_Vector2D&  operator -=( const MT_Vector2D& v )         { rX_ -= v.rX_; rY_ -= v.rY_; return *this; }
    MT_Vector2D&  operator *=( MT_Float s )                   { rX_ *= s; rY_ *= s; return *this; }
    MT_Vector2D&  operator /=( MT_Float s )                   { return *this *= ( 1.0f / s ); }
    MT_Vector2D   operator / ( MT_Float s ) const             { return MT_Vector2D( rX_ / s, rY_ / s); }
    bool          operator ==( const MT_Vector2D& v ) const;
    bool          operator !=( const MT_Vector2D& v ) const;
    MT_Vector2D   operator - ( void ) const                   { return MT_Vector2D( -rX_, -rY_ ); }
    //@}

    MT_Float        rX_;
    MT_Float        rY_;
};

MT_Vector2D  operator *( MT_Float rScalar, const MT_Vector2D& v);
MT_Float     operator *( const MT_Vector2D& v1, const MT_Vector2D& v2 );
MT_Vector2D  operator *( const MT_Vector2D& v, MT_Float rScalar );
MT_Vector2D  operator +( const MT_Vector2D& v1, const MT_Vector2D& v2 );
MT_Vector2D  operator -( const MT_Vector2D& v1, const MT_Vector2D& v2 );
bool         operator <( const MT_Vector2D& v1, const MT_Vector2D& v2 );

MT_Vector2D  CrossProduct( const MT_Vector2D& v1, const MT_Vector2D& v2 );
MT_Float     DotProduct  ( const MT_Vector2D& v1, const MT_Vector2D& v2 );
MT_Float     Angle       ( const MT_Vector2D& v1, const MT_Vector2D& v2 ); // Retourne l'angle (orienté) de v1 vers v2, dans le sens trigo, [0, 2*PI]

struct TER_DistanceLess : std::binary_function< MT_Vector2D, MT_Vector2D, bool >
{
    TER_DistanceLess( const MT_Vector2D& vPos ) : vPosOrigin_( vPos ){};

    bool operator () ( const MT_Vector2D& first, const MT_Vector2D& second ) const
    {
        return first.Distance( vPosOrigin_ ) < second.Distance( vPosOrigin_ );
    };

    MT_Vector2D vPosOrigin_;
};

struct vector2DLessXY : public std::binary_function< MT_Vector2D, MT_Vector2D, bool >
{
    virtual bool operator () ( const MT_Vector2D& lhs, const MT_Vector2D& rhs ) const
    {
        return lhs.rX_ < rhs.rX_ || !(rhs.rX_ < lhs.rX_) && lhs.rY_ < rhs.rY_;
    }
};


class MT_Droite;

//$$$ DEGUEULASSE, COMME LE RESTE
extern bool        MT_IsPointBetweenTwoLines( const MT_Droite& leftDroite, const MT_Droite& rightDroite, const MT_Vector2D& vPoint );

#include "MT_Vector2D.inl"

#endif // __MT_V2D_h_

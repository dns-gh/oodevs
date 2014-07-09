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
#include <functional>
#include <iosfwd>

//=============================================================================
// Created: FBD 02-03-01
//=============================================================================
class MT_Vector2D
{
public:
     MT_Vector2D();                           // Constructor : les valeurs XYZ sont mises a 0
     MT_Vector2D( double rX, double rY ); // Constructor : X Y Z ne ser<T> rien ce commentaire !!!!
     MT_Vector2D( const MT_Vector2D& v );     // Constructor : Vector2D

    double            SquareMagnitude   () const;                  // Retourne la magnitude au carré d'un vector 2D ( longeur )
    double            Magnitude         () const;                        // Retourne la magnitude d'un vector 2D ( longeur )
    double            Distance          ( const MT_Vector2D& vPosDest ) const;    // Retourne la distance entre deux vector 2D
    double            SquareDistance    ( const MT_Vector2D& vPosDest ) const;
    MT_Vector2D       Normalized        () const;
    MT_Vector2D&      Normalize         ();
    MT_Vector2D       Rotated           ( double rad ) const; // Dans le sens des aiguilles d'une montre
    MT_Vector2D&      Rotate            ( double rad ); // Dans le sens des aiguilles d'une montre
    MT_Vector2D&      Rotate90ClockWise ();
    MT_Vector2D       Rotated90ClockWise() const;
    MT_Vector2D&      Rotate90          ();
    MT_Vector2D       Rotated90         () const;
    MT_Vector2D&      Rotate180         ();
    bool              IsZero() const;
    void              Reset();
    inline double     GetX() const { return rX_; }
    inline double     GetY() const { return rY_; }

    //! @name Operators
    //@{
    MT_Vector2D&  operator  =( const MT_Vector2D& v )       { rX_ = v.rX_; rY_ = v.rY_; return *this; }
    MT_Vector2D&  operator +=( const MT_Vector2D& v )       { rX_ += v.rX_; rY_ += v.rY_; return *this; }
    MT_Vector2D&  operator -=( const MT_Vector2D& v )       { rX_ -= v.rX_; rY_ -= v.rY_; return *this; }
    MT_Vector2D&  operator *=( double s )                   { rX_ *= s; rY_ *= s; return *this; }
    MT_Vector2D&  operator /=( double s )                   { return *this *= ( 1.0f / s ); }
    MT_Vector2D   operator  /( double s ) const             { return MT_Vector2D( rX_ / s, rY_ / s); }
    MT_Vector2D   operator  -() const                       { return MT_Vector2D( -rX_, -rY_ ); }
    bool          operator ==( const MT_Vector2D& v ) const;
    bool          operator !=( const MT_Vector2D& v ) const;
    //@}

    double        rX_;
    double        rY_;
};

MT_Vector2D  operator *( double rScalar, const MT_Vector2D& v);
double       operator *( const MT_Vector2D& v1, const MT_Vector2D& v2 );
MT_Vector2D  operator *( const MT_Vector2D& v, double rScalar );
MT_Vector2D  operator +( const MT_Vector2D& v1, const MT_Vector2D& v2 );
MT_Vector2D  operator -( const MT_Vector2D& v1, const MT_Vector2D& v2 );
bool         operator <( const MT_Vector2D& v1, const MT_Vector2D& v2 );

double       CrossProduct( const MT_Vector2D& v1, const MT_Vector2D& v2 );
double       DotProduct  ( const MT_Vector2D& v1, const MT_Vector2D& v2 );
double       Angle       ( const MT_Vector2D& v1, const MT_Vector2D& v2 ); // Retourne l'angle (orienté) de v1 vers v2, dans le sens trigo, [0, 2*PI]

std::ostream& operator<<( std::ostream&, const MT_Vector2D& );

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

namespace boost
{
namespace serialization
{
    template< typename Archive >
    void serialize( Archive& file, MT_Vector2D& vect, const unsigned int )
    {
        file & vect.rX_ & vect.rY_;
    }
}
}

#include "MT_Vector2D.inl"

#endif // __MT_V2D_h_

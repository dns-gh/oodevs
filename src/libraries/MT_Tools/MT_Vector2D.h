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

#include "MT/MT_Tools/MT_Types.h"
#include "MT_Tools_Types.h"

class MT_OutputArchive_ABC;
class MT_InputArchive_ABC;

//=============================================================================
// Created: FBD 02-03-01
//=============================================================================
class MT_Vector2D
{
//    MT_BOOSTPOOLING_MEMBERS( MT_Vector2D )

public:
     MT_Vector2D();                           // Constructor : les valeurs XYZ sont mises a 0
     MT_Vector2D( MT_Float rX, MT_Float rY ); // Constructor : X Y Z ne ser<T> rien ce commentaire !!!!
     MT_Vector2D( const MT_Vector2D& v );     // Constructor : Vector2D
    ~MT_Vector2D();                           // Destructor :

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
    bool          operator < ( const MT_Vector2D& v ) const   { return ( rX_ != v.rX_ || rY_ != v.rY_ ); }// Attention < sert par defaut aux map et donc pas de distance pour l'operationr <
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

typedef std::list< MT_Vector2D >    T_PointList;
typedef T_PointList::iterator       IT_PointList;
typedef T_PointList::const_iterator CIT_PointList;

typedef std::vector< MT_Vector2D >              T_PointVector;
typedef const T_PointVector                     CT_PointVector;
typedef T_PointVector::iterator                 IT_PointVector;
typedef T_PointVector::const_iterator           CIT_PointVector;
typedef T_PointVector::reverse_iterator         RIT_PointVector;
typedef T_PointVector::const_reverse_iterator   CRIT_PointVector;

typedef std::set< MT_Vector2D, TER_DistanceLess > T_PointSet;
typedef const T_PointSet                    CT_PointSet;
typedef T_PointSet::iterator                IT_PointSet;
typedef T_PointSet::const_iterator          CIT_PointSet;


typedef std::map< MT_Vector2D, int >        T_PointMap;
typedef const T_PointMap                    CT_PointMap;
typedef T_PointMap::iterator                IT_PointMap;
typedef T_PointMap::const_iterator          CIT_PointMap;

typedef std::vector< MT_Vector2D* >         T_PointPtrVector;
typedef const T_PointPtrVector              CT_PointPtrVector;
typedef T_PointPtrVector::iterator          IT_PointPtrVector;
typedef T_PointPtrVector::const_iterator    CIT_PointPtrVector;
typedef T_PointPtrVector::reverse_iterator  RIT_PointPtrVector;

typedef std::vector< T_PointPtrVector >     T_RingVector;
typedef const T_RingVector                  CT_RingVector;
typedef T_RingVector::iterator              IT_RingVector;
typedef T_RingVector::const_iterator        CIT_RingVector;


class MT_Droite;

//$$$ DEGUEULASSE, COMME LE RESTE
extern MT_Vector2D MT_ComputeBarycenter     ( const T_PointVector& points );
extern bool        MT_IsPointBetweenTwoLines( const MT_Droite& leftDroite, const MT_Droite& rightDroite, const MT_Vector2D& vPoint );

#include "MT_Vector2D.inl"

#endif // __MT_V2D_h_

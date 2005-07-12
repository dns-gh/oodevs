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

template <class T>
class MT_V2D
{

public:

	typedef std::vector< MT_V2D<T> >            T_V2DVector;
    typedef typename T_V2DVector::iterator               IT_V2DVector;
    typedef typename T_V2DVector::const_iterator         CIT_V2DVector;

    typedef std::vector< MT_V2D<T>* >           T_V2DPtrVector;
	typedef typename T_V2DPtrVector::iterator            IT_V2DPtrVector;
	typedef typename T_V2DPtrVector::const_iterator      CIT_V2DPtrVector;
	

    explicit        MT_V2D( void );                                 // Constructor : les valeurs XYZ sont mises a 0
                    MT_V2D( T rX, T rY );                       // Constructor : X Y Z ne ser<T> rien ce commentaire !!!!
                    MT_V2D( const MT_V2D<T>& v );                   // Constructor : Vector2D
                    ~MT_V2D( void );                                // Destructor :

    T               SquareMagnitude( void ) const;                  // Retourne la magnitude au carré d'un vector 2D ( longeur )
    T               Magnitude( void ) const;                        // Retourne la magnitude d'un vector 2D ( longeur )
    T               Distance( const MT_V2D<T>& vPosDest ) const;    // Retourne la distance entre deux vector 2D
    T               SquareDistance( const MT_V2D<T>& vPosDest ) const;
    MT_V2D<T>       Normalized() const;
    MT_V2D<T>&      Normalize();
    MT_V2D<T>       Rotated( MT_Float rad ) const; // Dans le sens des aiguilles d'une montre
    MT_V2D<T>&      Rotate( MT_Float rad ); // Dans le sens des aiguilles d'une montre
    MT_V2D<T>&      Rotate90ClockWise ();
    MT_V2D<T>       Rotated90ClockWise() const;
    MT_V2D<T>&      Rotate90          ();
    MT_V2D<T>       Rotated90         () const;
    MT_V2D<T>&      Rotate180         (); 
    bool            IsZero() const;
    void            Reset();
    
    int             Write( MT_OutputArchive_ABC& archive ) const;
    int             Read( MT_InputArchive_ABC& archive );

    MT_V2D<T>&  operator =( const MT_V2D<T>& v )            { rX_ = v.rX_; rY_ = v.rY_; return *this; }
    MT_V2D<T>&  operator +=( const MT_V2D<T>& v )           { rX_ += v.rX_; rY_ += v.rY_; return *this; }
    MT_V2D<T>&  operator -=( const MT_V2D<T>& v )           { rX_ -= v.rX_; rY_ -= v.rY_; return *this; }
    MT_V2D<T>&  operator *=( T s )                          { rX_ *= s; rY_ *= s; return *this; }
    MT_V2D<T>&  operator /=( T s )                          { return *this *= ( 1.0f / s ); }
    MT_V2D<T>   operator / ( T s ) const                    { return MT_V2D<T>( rX_ / s, rY_ / s); }
    bool        operator ==( const MT_V2D<T>& v ) const;
    bool        operator !=( const MT_V2D<T>& v ) const;
    T           operator []( int i ) const                  { return ( (T*)&rX_ )[i]; }
    T&          operator []( int i )                        { return ( (T*)&rX_ )[i]; }
    MT_V2D<T>   operator - ( void ) const                   { return MT_V2D<T>( -rX_, -rY_ ); }
    bool        operator < ( const MT_V2D<T>& v ) const     { return ( rX_ != v.rX_ || rY_ != v.rY_ ); }// Attention < sert par defaut aux map et donc pas de distance pour l'operationr <
    
    
    T               rX_;
    T               rY_;
};

template < class T >
MT_V2D<T>   operator *( T rScalar, const MT_V2D<T>& v);
template < class T >
T               operator *( const MT_V2D<T>& v1, const MT_V2D<T>& v2 );
template < class T >
MT_V2D<T> operator *( const MT_V2D<T>& v, T rScalar );
template < class T >
MT_V2D<T>   operator +( const MT_V2D<T>& v1, const MT_V2D<T>& v2 );
template < class T >
MT_V2D<T>   operator -( const MT_V2D<T>& v1, const MT_V2D<T>& v2 );
template < class T >
bool        operator <( const MT_V2D<T>& v1, const MT_V2D<T>& v2 );
template < class T >
MT_V2D<T>  CrossProduct( const MT_V2D<T>& v1, const MT_V2D<T>& v2);
template < class T >
T          DotProduct( const MT_V2D<T>& v1, const MT_V2D<T>& v2);
template < class T >
T          Angle( const MT_V2D<T>& v1, const MT_V2D<T>& v2); // Retourne l'angle (orienté) de v1 vers v2, dans le sens trigo, [0, 2*PI]

// Scipio types
typedef MT_V2D<MT_Float>        MT_Vector2D;

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

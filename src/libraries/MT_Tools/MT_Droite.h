// *****************************************************************************
//
// $Created: NLD 2003-08-20 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Droite.h $
// $Author: Jvt $
// $Modtime: 14/02/05 11:17 $
// $Revision: 6 $
// $Workfile: MT_Droite.h $
//
// *****************************************************************************

#ifndef __MT_Droite_h_
#define __MT_Droite_h_

#include "MT_Tools_Types.h"
#include "MT_Vector2D.h"

class MT_Line;

enum E_CollisionState;

// =============================================================================
// Created: NLD 2003-08-20
// =============================================================================
class MT_Droite
{
public:
    //! @name Types 
    //@{
    enum E_Side
    {
        eOnNegativeSide = -1, 
        eOnPositiveSide = 0,
        eOnBoundary     = 1
    };
    //@}

public:
     MT_Droite();
     MT_Droite( const MT_Vector2D& vPointInLine1, const MT_Vector2D& vPointInLine2 );
     MT_Droite( MT_Float rX1, MT_Float rY1, MT_Float rX2, MT_Float rY2 );
     MT_Droite( const MT_Droite& );
    ~MT_Droite();
    
    MT_Droite& operator = ( const MT_Droite& );

    //! @name Operations
    //@{
    E_Side           GetSide           ( const MT_Vector2D& vPoint ) const;
    E_Side           GetSide           ( MT_Float rX, MT_Float rY )  const;
    bool             IsInside          ( const MT_Vector2D& vPoint ) const;
    E_CollisionState Intersect2D       ( const MT_Line& segment , MT_Vector2D& vPosIntersect ) const;
    E_CollisionState Intersect2D       ( const MT_Droite& droite, MT_Vector2D& vPosIntersect ) const; 
    MT_Float         GetDistanceToPoint( const MT_Vector2D& vPoint ) const;
    //@}

private:
    // Line equation : rA_ x + rB_ y + rC = 0
    MT_Float rA_;
    MT_Float rB_;
    MT_Float rC_;
};

typedef std::vector< MT_Droite >                 T_DroiteVector;
typedef const T_DroiteVector                     CT_DroiteVector;
typedef T_DroiteVector::iterator                 IT_DroiteVector;
typedef T_DroiteVector::const_iterator           CIT_DroiteVector;
typedef T_DroiteVector::reverse_iterator         RIT_DroiteVector;
typedef T_DroiteVector::const_reverse_iterator   CRIT_DroiteVector;

#include "MT_Droite.inl"

#endif // __MT_Droite_h_

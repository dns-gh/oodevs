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

#include "MT_Line.h"
#include <vector>

class MT_Line;
class MT_Vector2D;

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
     MT_Droite( double rX1, double rY1, double rX2, double rY2 );
     MT_Droite( const MT_Droite& );
    virtual ~MT_Droite();

    MT_Droite& operator = ( const MT_Droite& );

    //! @name Operations
    //@{
    E_Side           GetSide           ( const MT_Vector2D& vPoint ) const;
    E_Side           GetSide           ( double rX, double rY )  const;
    bool             IsInside          ( const MT_Vector2D& vPoint ) const;
    E_CollisionState Intersect2D       ( const MT_Line& segment , MT_Vector2D& vPosIntersect ) const;
    E_CollisionState Intersect2D       ( const MT_Droite& droite, MT_Vector2D& vPosIntersect ) const;
    double         GetDistanceToPoint( const MT_Vector2D& vPoint ) const;
    //@}

private:
    // Line equation : rA_ x + rB_ y + rC = 0
    double rA_;
    double rB_;
    double rC_;
};

bool MT_IsPointBetweenTwoLines( const MT_Droite& leftDroite, const MT_Droite& rightDroite,
        const MT_Vector2D& vPoint );

#endif // __MT_Droite_h_

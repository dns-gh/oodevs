// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Sector.h $
// $Author: Jvt $
// $Modtime: 12/04/05 9:57 $
// $Revision: 2 $
// $Workfile: MT_Sector.h $
//
// *****************************************************************************

#ifndef __MT_Sector_h_
#define __MT_Sector_h_

#include "MT_Tools_Types.h"
#include "MT_Vector2D.h"

// =============================================================================
// @class  MT_Sector
// Created: JVT 2004-08-03
// =============================================================================
class MT_Sector
{
//    MT_BOOSTPOOLING_MEMBERS( MT_Sector )

public:
     MT_Sector( const MT_Vector2D& vOrigin_, const MT_Vector2D& vDirection, MT_Float rAngle );
     MT_Sector();
    ~MT_Sector();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operators
    //@{
    bool IsInCone  ( const MT_Vector2D& vPos, MT_Float rRadius ) const;
    bool IsInSector( const MT_Vector2D& vPos ) const;
    //@}

    //! @name Accessors
    //@{
    const MT_Vector2D& GetDirection() const;
    const MT_Vector2D& GetOrigin   () const;
          MT_Float     GetAngle    () const;
    //@}

private:
    MT_Vector2D vOrigin_;
    MT_Vector2D vDirection_;
    MT_Float    rSemiAngle_;

    // Equation de la droite séparant le premier demi-plan
    MT_Float rA1_;
    MT_Float rB1_;
    MT_Float rC1_;

    // Equation de la droite séparant de deuxième demi-plan
    MT_Float rA2_;
    MT_Float rB2_;
    MT_Float rC2_;
};

#include "MT_Sector.inl"

#endif // __MT_Sector_h_

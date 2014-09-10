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

#include "MT_Vector2D.h"

// =============================================================================
// @class  MT_Sector
// Created: JVT 2004-08-03
// =============================================================================
class MT_Sector
{
public:
             MT_Sector( const MT_Vector2D& vOrigin_, const MT_Vector2D& vDirection, double rAngle );
             MT_Sector();
    virtual ~MT_Sector();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operators
    //@{
    bool IsInCone  ( const MT_Vector2D& vPos, double rRadius ) const;
    bool IsInSector( const MT_Vector2D& vPos ) const;
    //@}

    //! @name Accessors
    //@{
    const MT_Vector2D& GetDirection() const;
    const MT_Vector2D& GetOrigin   () const;
          double     GetAngle    () const;
    //@}

private:
    MT_Vector2D vOrigin_;
    MT_Vector2D vDirection_;
    double    rSemiAngle_;

    // Equation de la droite séparant le premier demi-plan
    double rA1_;
    double rB1_;
    double rC1_;

    // Equation de la droite séparant de deuxième demi-plan
    double rA2_;
    double rB2_;
    double rC2_;
};

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MT_Sector::serialize
// Created: JVT 2005-04-12
// -----------------------------------------------------------------------------
template< typename Archive >
void MT_Sector::serialize( Archive& file, const unsigned int )
{
    file & vOrigin_;
    file & vDirection_;
    file & rSemiAngle_;
    file & rA1_;
    file & rB1_;
    file & rC1_;
    file & rA2_;
    file & rB2_;
    file & rC2_;
}

#endif // __MT_Sector_h_

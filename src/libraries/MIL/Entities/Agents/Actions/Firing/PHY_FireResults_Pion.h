// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/IndirectFiring/PHY_FireResults_Pion.h $
// $Author: Nld $
// $Modtime: 12/04/05 17:19 $
// $Revision: 4 $
// $Workfile: PHY_FireResults_Pion.h $
//
// *****************************************************************************

#ifndef __PHY_FireResults_Pion_h_
#define __PHY_FireResults_Pion_h_

#include "MIL.h"

#include "Entities/Actions/PHY_FireResults_ABC.h"

class MIL_Agent_ABC;
class MIL_AgentPion;
class PHY_DotationCategory;

// =============================================================================
// @class  PHY_FireResults_Pion
// Created: JVT 2004-08-03
// =============================================================================
class PHY_FireResults_Pion : public PHY_FireResults_ABC
{
    MT_COPYNOTALLOWED( PHY_FireResults_Pion )

public:
    PHY_FireResults_Pion( const MIL_AgentPion& firer, const MIL_Agent_ABC& target );
    PHY_FireResults_Pion( const MIL_AgentPion& firer, const MT_Vector2D& vTargetPosition, const PHY_DotationCategory& dotationCategory );
    virtual ~PHY_FireResults_Pion();

    //! @name Accessors
    //@{
    uint GetID() const;
    //@}

private:
    const uint                  nID_;
    const MIL_AgentPion&        firer_;

    const MIL_Agent_ABC*        pTarget_;
    const MT_Vector2D           targetPosition_;
    const PHY_DotationCategory* pDotationCategory_;
};

#include "PHY_FireResults_Pion.inl"

#endif // __PHY_FireResults_Pion_h_

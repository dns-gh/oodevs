// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/IndirectFiring/PHY_ActionIndirectFire_ABC.h $
// $Author: Jvt $
// $Modtime: 2/05/05 16:32 $
// $Revision: 4 $
// $Workfile: PHY_ActionIndirectFire_ABC.h $
//
// *****************************************************************************

#ifndef __PHY_ActionIndirectFire_ABC_h_
#define __PHY_ActionIndirectFire_ABC_h_

#include "MIL.h"

#include "Entities/Actions/PHY_Action_ABC.h"

class PHY_RoleAction_IndirectFiring;
class PHY_IndirectFireDotationClass;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_ActionIndirectFire_ABC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionIndirectFire_ABC : public PHY_Action_ABC
{
    MT_COPYNOTALLOWED( PHY_ActionIndirectFire_ABC )

public:
    typedef MIL_AgentPion ActorType;        

public:
             PHY_ActionIndirectFire_ABC( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionIndirectFire_ABC();

protected:
    PHY_RoleAction_IndirectFiring&       role_;
    DIA_Variable_ABC&                    diaReturnCode_;
    const PHY_IndirectFireDotationClass* pIndirectWeaponClass_;
    const MT_Float                       rNbInterventionType_;
};

#include "PHY_ActionIndirectFire_ABC.inl"

#endif // __PHY_ActionIndirectFire_ABC_h_

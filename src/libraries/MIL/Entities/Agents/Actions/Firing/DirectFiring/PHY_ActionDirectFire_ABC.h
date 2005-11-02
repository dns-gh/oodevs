// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFire_ABC.h $
// $Author: Nld $
// $Modtime: 28/10/04 9:49 $
// $Revision: 2 $
// $Workfile: PHY_ActionDirectFire_ABC.h $
//
// *****************************************************************************

#ifndef __PHY_ActionDirectFire_ABC_h_
#define __PHY_ActionDirectFire_ABC_h_

#include "MIL.h"

#include "Entities/Actions/PHY_Action_ABC.h"
#include "PHY_RoleAction_DirectFiring.h"

class PHY_DirectFireResults;
class PHY_AmmoDotationClass;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_ActionDirectFire_ABC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionDirectFire_ABC : public PHY_Action_ABC
{
    MT_COPYNOTALLOWED( PHY_ActionDirectFire_ABC )

public:
    typedef MIL_AgentPion ActorType;        

public:
    PHY_ActionDirectFire_ABC( MIL_AgentPion& pion, DIA_Call_ABC& diaCall, PHY_DirectFireData::E_ComposanteFiringType nComposanteFiringType );
    virtual ~PHY_ActionDirectFire_ABC();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
          PHY_RoleAction_DirectFiring&               role_;
          DIA_Variable_ABC&                          diaReturnCode_;
    const uint                                       nTargetKnowledgeID_;
    const MT_Float                                   rPercentageComposantesToUse_;
    const PHY_DirectFireData::E_FiringMode           nFiringMode_;
    const PHY_DirectFireData::E_ComposanteFiringType nComposanteFiringType_;
    const PHY_AmmoDotationClass*                     pAmmoDotationClass_;
          PHY_DirectFireResults*                     pFireResult_;
};

#include "PHY_ActionDirectFire_ABC.inl"

#endif // __PHY_ActionDirectFire_ABC_h_

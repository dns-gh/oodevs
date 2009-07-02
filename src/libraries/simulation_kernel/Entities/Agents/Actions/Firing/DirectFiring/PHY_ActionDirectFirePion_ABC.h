// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePion_ABC.h $
// $Author: Nld $
// $Modtime: 28/10/04 9:49 $
// $Revision: 2 $
// $Workfile: PHY_ActionDirectFirePion_ABC.h $
//
// *****************************************************************************

#ifndef __PHY_ActionDirectFirePion_ABC_h_
#define __PHY_ActionDirectFirePion_ABC_h_

#include "MIL.h"

#include "Entities/Actions/PHY_Action_ABC.h"
#include "PHY_RoleAction_DirectFiring.h"

class PHY_FireResults_Pion;
class PHY_AmmoDotationClass;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_ActionDirectFirePion_ABC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionDirectFirePion_ABC : public PHY_Action_ABC
{

public:
    typedef MIL_AgentPion ActorType;        

public:
    PHY_ActionDirectFirePion_ABC( MIL_AgentPion& pion, DIA_Call_ABC& diaCall, PHY_DirectFireData::E_ComposanteFiringType nComposanteFiringType, PHY_DirectFireData::E_ComposanteFiredType nComposanteFiredType );
    virtual ~PHY_ActionDirectFirePion_ABC();

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
    const PHY_DirectFireData::E_ComposanteFiredType  nComposanteFiredType_;
    const PHY_AmmoDotationClass*                     pAmmoDotationClass_;
   
          PHY_FireResults_Pion*                      pFireResult_;
};

#endif // __PHY_ActionDirectFirePion_ABC_h_

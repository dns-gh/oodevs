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
#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"
#include "PHY_RoleAction_DirectFiring.h"

class PHY_FireResults_Pion;
class PHY_AmmoDotationClass;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_ActionDirectFirePion_ABC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionDirectFirePion_ABC : public PHY_DecisionCallbackAction_ABC
{
public:
    typedef MIL_AgentPion ActorType;

public:
             PHY_ActionDirectFirePion_ABC( MIL_AgentPion& pion, boost::shared_ptr< DEC_Knowledge_Agent > pEnemy, double percentage, int firingMode, int ammoDotationClass, firing::PHY_DirectFireData::E_ComposanteFiringType nComposanteFiringType, firing::PHY_DirectFireData::E_ComposanteFiredType nComposanteFiredType );
    virtual ~PHY_ActionDirectFirePion_ABC();

    //! @name Operations
    //@{
    virtual void Execute();
    virtual void ExecuteSuspended();
    virtual void StopAction();
    //@}

private:
    firing::PHY_RoleAction_DirectFiring& role_;
    boost::shared_ptr< DEC_Knowledge_Agent > pEnemy_;
    const double rPercentageComposantesToUse_;
    const firing::PHY_DirectFireData::E_FiringMode nFiringMode_;
    const firing::PHY_DirectFireData::E_ComposanteFiringType nComposanteFiringType_;
    const firing::PHY_DirectFireData::E_ComposanteFiredType nComposanteFiredType_;
    const PHY_AmmoDotationClass* pAmmoDotationClass_;
    PHY_FireResults_Pion* pFireResult_;
    bool mustReport_;
};

#endif // __PHY_ActionDirectFirePion_ABC_h_

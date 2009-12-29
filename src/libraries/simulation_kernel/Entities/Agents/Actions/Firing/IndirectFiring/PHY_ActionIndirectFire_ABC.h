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

#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"

namespace firing
{
    class PHY_RoleAction_IndirectFiring;
}

class PHY_DotationCategory;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_ActionIndirectFire_ABC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionIndirectFire_ABC : public PHY_DecisionCallbackAction_ABC
{

public:
    typedef MIL_AgentPion ActorType;        

public:
             PHY_ActionIndirectFire_ABC( MIL_AgentPion& pion, const PHY_DotationCategory* pDotationCategory, float rNbInterventionType );
    virtual ~PHY_ActionIndirectFire_ABC();

protected:
          firing::PHY_RoleAction_IndirectFiring& role_;
    const PHY_DotationCategory*          pDotationCategory_;
    const MT_Float                       rNbInterventionType_;
};

#endif // __PHY_ActionIndirectFire_ABC_h_

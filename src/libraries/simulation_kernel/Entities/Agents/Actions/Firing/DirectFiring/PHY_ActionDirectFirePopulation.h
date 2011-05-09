// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePopulation.h $
// $Author: Nld $
// $Modtime: 28/10/04 9:49 $
// $Revision: 2 $
// $Workfile: PHY_ActionDirectFirePopulation.h $
//
// *****************************************************************************

#ifndef __PHY_ActionDirectFirePopulation_h_
#define __PHY_ActionDirectFirePopulation_h_

#include "MIL.h"

#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"

namespace firing
{
    class PHY_RoleAction_DirectFiring;
}

class PHY_AmmoDotationClass;
class PHY_FireResults_Pion;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_ActionDirectFirePopulation
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionDirectFirePopulation : public PHY_DecisionCallbackAction_ABC
{
public:
    typedef MIL_AgentPion ActorType;

public:
             PHY_ActionDirectFirePopulation( MIL_AgentPion& pion, unsigned int populationKnowledgeID, const std::string& dotationClass );
    virtual ~PHY_ActionDirectFirePopulation();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    virtual void StopAction();
    //@}

private:
    firing::PHY_RoleAction_DirectFiring& role_;
    const unsigned int nTargetKnowledgeID_;
    PHY_FireResults_Pion* pFireResult_;
    const PHY_AmmoDotationClass* dotationClass_;
};

#endif // __PHY_ActionDirectFirePopulation_h_

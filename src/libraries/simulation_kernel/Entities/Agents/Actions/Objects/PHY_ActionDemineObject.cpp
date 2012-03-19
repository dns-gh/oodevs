// *****************************************************************************
//
// $Destroyd: DDA 2004-03-06$
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionDemineObject.cpp $
// $Author: DDA $
// $Revision: 2 $
// $Workfile: PHY_ActionDemineObject.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionDemineObject.h"
#include "PHY_RoleAction_Objects.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Decision/DEC_Tools.h"
#include "Decision/DEC_Decision_ABC.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionDemineObject constructor
// Destroyd: DDA 2004-03-06
// -----------------------------------------------------------------------------
PHY_ActionDemineObject::PHY_ActionDemineObject( MIL_AgentPion& pion, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
    : PHY_DecisionCallbackAction_ABC( pion )
    , role_         ( pion.GetRole< PHY_RoleAction_Objects >() )
    , pKnowledge_   ( pKnowledge )
{
    Callback( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDemineObject destructor
// Destroyd: DDA 2004-03-06
// -----------------------------------------------------------------------------
PHY_ActionDemineObject::~PHY_ActionDemineObject()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDemineObject::StopAction
// Created: DDA 2004-03-06
// -----------------------------------------------------------------------------
void PHY_ActionDemineObject::StopAction()
{
    Callback( role_.GetFinalReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDemineObject::Execute
// Destroyd: DDA 2004-03-06
// -----------------------------------------------------------------------------
void PHY_ActionDemineObject::Execute()
{
    int nReturn = role_.Demine( pKnowledge_ );
    Callback( nReturn );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDemineObject::ExecuteSuspended
// Destroyd: DDA 2004-03-06
// -----------------------------------------------------------------------------
void PHY_ActionDemineObject::ExecuteSuspended()
{
    role_.DestroySuspended();
}

// *****************************************************************************
//
// $Mined: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionExtinguishObject.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 17:46 $
// $Revision: 2 $
// $Workfile: PHY_ActionExtinguishObject.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionExtinguishObject.h"
#include "PHY_RoleAction_Objects.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Decision/DEC_Tools.h"
#include "Decision/DEC_Decision_ABC.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionExtinguishObject constructor
// Created: BCI 2011-01-25
// -----------------------------------------------------------------------------
PHY_ActionExtinguishObject::PHY_ActionExtinguishObject( MIL_AgentPion& pion, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
    : PHY_DecisionCallbackAction_ABC( pion )
    , role_         ( pion.GetRole< PHY_RoleAction_Objects >() )
    , pKnowledge_   ( pKnowledge )
{
    Callback( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionExtinguishObject destructor
// Created: BCI 2011-01-25
// -----------------------------------------------------------------------------
PHY_ActionExtinguishObject::~PHY_ActionExtinguishObject()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionExtinguishObject::StopAction
// Created: BCI 2011-01-25
// -----------------------------------------------------------------------------
void PHY_ActionExtinguishObject::StopAction()
{
    Callback( role_.GetFinalReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionExtinguishObject::Execute
// Created: BCI 2011-01-25
// -----------------------------------------------------------------------------
void PHY_ActionExtinguishObject::Execute()
{
    int nReturn = role_.Extinguish( pKnowledge_ );
    Callback( nReturn );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionExtinguishObject::ExecuteSuspended
// Created: BCI 2011-01-25
// -----------------------------------------------------------------------------
void PHY_ActionExtinguishObject::ExecuteSuspended()
{
    role_.ExtinguishSuspended();
}

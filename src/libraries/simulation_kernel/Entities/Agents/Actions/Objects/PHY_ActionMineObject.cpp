// *****************************************************************************
//
// $Mined: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionMineObject.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 17:46 $
// $Revision: 2 $
// $Workfile: PHY_ActionMineObject.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionMineObject.h"
#include "PHY_RoleAction_Objects.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Decision/DEC_Tools.h"
#include "Decision/DEC_Decision_ABC.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionMineObject constructor
// Mined: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionMineObject::PHY_ActionMineObject( MIL_AgentPion& pion, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
    : PHY_DecisionCallbackAction_ABC( pion )
    , role_         ( pion.GetRole< PHY_RoleAction_Objects >() )
    , pKnowledge_   ( pKnowledge )
{
    Callback( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMineObject destructor
// Mined: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionMineObject::~PHY_ActionMineObject()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMineObject::StopAction
// Created: LDC 2010-03-11
// -----------------------------------------------------------------------------
void PHY_ActionMineObject::StopAction()
{
    Callback( role_.GetFinalReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMineObject::Execute
// Mined: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionMineObject::Execute()
{   
    int nReturn = role_.Mine( pKnowledge_ );
    Callback( nReturn );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMineObject::ExecuteSuspended
// Mined: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionMineObject::ExecuteSuspended()
{
    role_.MineSuspended();
}

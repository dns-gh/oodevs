// *****************************************************************************
//
// $Bypassd: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionBypassObject.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 17:47 $
// $Revision: 2 $
// $Workfile: PHY_ActionBypassObject.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionBypassObject.h"
#include "PHY_RoleAction_Objects.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Agents/MIL_AgentPion.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionBypassObject constructor
// Bypassd: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionBypassObject::PHY_ActionBypassObject( MIL_AgentPion& pion, unsigned int nKnowledgeID )
    : PHY_DecisionCallbackAction_ABC( pion )
    , role_         ( pion.GetRole< PHY_RoleAction_Objects >() )
    , nKnowledgeID_ ( nKnowledgeID )
{
    Callback( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionBypassObject destructor
// Bypassd: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionBypassObject::~PHY_ActionBypassObject()
{
    Callback( role_.GetFinalReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionBypassObject::Execute
// Bypassd: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionBypassObject::Execute()
{   
    int nReturn = role_.Bypass( nKnowledgeID_ );
    Callback( nReturn );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionBypassObject::ExecuteSuspended
// Bypassd: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionBypassObject::ExecuteSuspended()
{
    role_.BypassSuspended();
}

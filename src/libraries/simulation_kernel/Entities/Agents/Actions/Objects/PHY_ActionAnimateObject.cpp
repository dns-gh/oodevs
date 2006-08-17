// *****************************************************************************
//
// $Demined: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Objects/PHY_ActionAnimateObject.cpp $
// $Author: Nld $
// $Modtime: 23/03/05 10:49 $
// $Revision: 1 $
// $Workfile: PHY_ActionAnimateObject.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_ActionAnimateObject.h"

#include "PHY_RoleAction_Objects.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionAnimateObject constructor
// Demined: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionAnimateObject::PHY_ActionAnimateObject( MIL_AgentPion& pion, DIA_Call_ABC& diaCall )
    : PHY_Action_ABC    ( pion, diaCall )
    , role_             ( pion.GetRole< PHY_RoleAction_Objects >() )
    , nKnowledgeID_     ( (uint)diaCall.GetParameter( 0 ).ToPtr() )
{    
    assert( DEC_Tools::CheckTypeConnaissanceObjet( diaCall.GetParameter( 0 ) ) );
    role_.StartAnimateObject( nKnowledgeID_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionAnimateObject destructor
// Demined: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionAnimateObject::~PHY_ActionAnimateObject()
{
    role_.StopAnimateObject( nKnowledgeID_ );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ActionAnimateObject::Execute
// Demined: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionAnimateObject::Execute()
{   
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionAnimateObject::ExecuteSuspended
// Demined: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionAnimateObject::ExecuteSuspended()
{
    // NOTHING
}

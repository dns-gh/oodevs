// *****************************************************************************
//
// $Demined: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionOccupyObject.cpp $
// $Author: Nld $
// $Modtime: 26/11/04 16:50 $
// $Revision: 1 $
// $Workfile: PHY_ActionOccupyObject.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionOccupyObject.h"
#include "PHY_RoleAction_Objects.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionOccupyObject constructor
// Demined: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionOccupyObject::PHY_ActionOccupyObject( MIL_AgentPion& pion, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
    : PHY_Action_ABC( pion )
    , role_         ( pion.GetRole< PHY_RoleAction_Objects >() )
    , pKnowledge_   ( pKnowledge )
{    
    role_.StartOccupyingObject( pKnowledge_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionOccupyObject destructor
// Demined: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionOccupyObject::~PHY_ActionOccupyObject()
{
    role_.StopOccupyingObject( pKnowledge_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionOccupyObject::Execute
// Demined: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionOccupyObject::Execute()
{   
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionOccupyObject::ExecuteSuspended
// Demined: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionOccupyObject::ExecuteSuspended()
{
    // NOTHING
}

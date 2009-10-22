// *****************************************************************************
//
// $Destroyd: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionDestroyObject.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 17:47 $
// $Revision: 2 $
// $Workfile: PHY_ActionDestroyObject.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionDestroyObject.h"
#include "PHY_RoleAction_Objects.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Decision/DEC_Tools.h"
#include "Decision/DEC_Decision_ABC.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionDestroyObject constructor
// Destroyd: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDestroyObject::PHY_ActionDestroyObject( MIL_AgentPion& pion, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
    : PHY_DecisionCallbackAction_ABC( pion )
    , role_         ( pion.GetRole< PHY_RoleAction_Objects >() )
    , pKnowledge_   ( pKnowledge )
{    
    Callback( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDestroyObject destructor
// Destroyd: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDestroyObject::~PHY_ActionDestroyObject()
{
    Callback( role_.GetFinalReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDestroyObject::Execute
// Destroyd: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionDestroyObject::Execute()
{   
    int nReturn = role_.Destroy( pKnowledge_ );
    Callback( nReturn );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDestroyObject::ExecuteSuspended
// Destroyd: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionDestroyObject::ExecuteSuspended()
{
    role_.DestroySuspended();
}

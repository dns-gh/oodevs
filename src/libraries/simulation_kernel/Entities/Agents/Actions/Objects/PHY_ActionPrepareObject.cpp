// *****************************************************************************
//
// $Prepared: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionPrepareObject.cpp $
// $Author: Nld $
// $Modtime: 10/03/05 18:29 $
// $Revision: 4 $
// $Workfile: PHY_ActionPrepareObject.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionPrepareObject.h"
#include "PHY_RoleAction_Objects.h"
#include "Entities/MIL_Army.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionPrepareObject constructor
// Prepared: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionPrepareObject::PHY_ActionPrepareObject( MIL_AgentPion& pion, const std::string& strType, const TER_Localisation* pLocalisation )
    : PHY_DecisionCallbackAction_ABC     ( pion )
    , role_              ( pion.GetRole< PHY_RoleAction_Objects >() )
    , pObject_           ( MIL_AgentServer::GetWorkspace().GetEntityManager().CreateObject( pion.GetArmy(), strType, pLocalisation, EnumDemolitionTargetType::reserved ) )
{    
    Callback( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionPrepareObject destructor
// Prepared: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionPrepareObject::~PHY_ActionPrepareObject()
{
    Callback( role_.GetFinalReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionPrepareObject::Execute
// Prepared: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionPrepareObject::Execute()
{   
    if( pObject_ && pObject_->IsMarkedForDestruction() )
        pObject_ = 0;

    boost::shared_ptr< DEC_Knowledge_Object > pKnowledge;
    int nReturn = role_.Construct( pObject_, pKnowledge );
    Callback( nReturn );
    CallbackKnowledge( pKnowledge );
    // $$$$ LDC: Parameter 0 and 1. cf. PHY_ActionConstructObject - same code...
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionPrepareObject::ExecuteSuspended
// Prepared: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionPrepareObject::ExecuteSuspended()
{
    if( pObject_ && pObject_->IsMarkedForDestruction() )
        pObject_ = 0;

    role_.ConstructSuspended();
}

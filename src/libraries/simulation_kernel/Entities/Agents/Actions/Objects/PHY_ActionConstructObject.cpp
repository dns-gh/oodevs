// *****************************************************************************
//
// $Constructd: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionConstructObject.cpp $
// $Author: Nld $
// $Modtime: 10/03/05 18:29 $
// $Revision: 6 $
// $Workfile: PHY_ActionConstructObject.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionConstructObject.h"
#include "PHY_RoleAction_Objects.h"
#include "Entities/MIL_Army.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionConstructObject constructor
// Constructd: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionConstructObject::PHY_ActionConstructObject( MIL_AgentPion& pion, DIA_Call_ABC& diaCall )
    : PHY_Action_ABC     ( pion, diaCall )
    , role_              ( pion.GetRole< PHY_RoleAction_Objects >() )
    , diaReturnCode_     ( diaCall.GetParameter( 0 ) )
    , diaReturnKnowledge_( diaCall.GetParameter( 1 ) )
    , pObject_           ( MIL_AgentServer::GetWorkspace().GetEntityManager().CreateObject( pion.GetArmy(), diaCall.GetParameters(), 2, EnumDemolitionTargetType::preliminary ) ) 
{    
    assert( DEC_Tools::CheckTypeConnaissanceObjet( diaCall.GetParameter( 1 ) ) );
    diaReturnCode_.SetValue( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionConstructObject destructor
// Constructd: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionConstructObject::~PHY_ActionConstructObject()
{
    diaReturnCode_.SetValue( role_.GetFinalReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionConstructObject::Execute
// Constructd: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionConstructObject::Execute()
{   
    if( pObject_ && pObject_->IsMarkedForDestruction() )
        pObject_ = 0;

    DEC_Knowledge_Object* pKnowledge = 0;
    int nReturn = role_.Construct( pObject_, pKnowledge );
    diaReturnCode_     .SetValue( nReturn );
    diaReturnKnowledge_.SetValue( (void*)( pKnowledge ? pKnowledge->GetID() : 0 ), &DEC_Tools::GetTypeConnaissanceObjet() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionConstructObject::ExecuteSuspended
// Constructd: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionConstructObject::ExecuteSuspended()
{
    if( pObject_ && pObject_->IsMarkedForDestruction() )
        pObject_ = 0;
    role_.ConstructSuspended();
}

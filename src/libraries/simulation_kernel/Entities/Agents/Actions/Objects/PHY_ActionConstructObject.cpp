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
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/ConstructionAttribute.h"
#include "Entities/Objects/MineAttribute.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Decision/DEC_Tools.h"
#include "Decision/DEC_Gen_Object.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionConstructObject constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionConstructObject::PHY_ActionConstructObject( MIL_AgentPion& pion, boost::shared_ptr< DEC_Gen_Object > pGenObject )
    : PHY_DecisionCallbackAction_ABC( pion )
    , role_( pion.GetRole< PHY_RoleAction_Objects >() )
    , pObject_( MIL_AgentServer::GetWorkspace().GetEntityManager().CreateObject( pion.GetArmy(), pGenObject->GetTypeName(), &pGenObject->GetLocalisation(), pGenObject->GetObstacleType(), pGenObject->GetExternalIdentifier(), pGenObject->GetName(), pGenObject->GetDensity() ) )
{
    role_.SetCreator( *pObject_ );
    pObject_->Initialize( *pGenObject.get() );
    ConstructionAttribute* attribute = pObject_->RetrieveAttribute< ConstructionAttribute >();
    if( attribute )
    {
        attribute->Set( 0. );//default construction is set to 100%
        attribute->NotifyBuildByGen();
    }
    MineAttribute* mineAttribute = pObject_->RetrieveAttribute< MineAttribute >();
    if( mineAttribute )
    {
        mineAttribute->Set(0.);//default valorization is set to 100%
    }
    Callback( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionConstructObject destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionConstructObject::~PHY_ActionConstructObject()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionConstructObject::StopAction
// Created: LDC 2010-03-11
// -----------------------------------------------------------------------------
void PHY_ActionConstructObject::StopAction()
{
    if( pObject_ )
        pObject_->RetrieveAttribute< ConstructionAttribute >()->NotifyStopBuildByGen();
    Callback( role_.GetFinalReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionConstructObject::Execute
// Constructd: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionConstructObject::Execute()
{
    if( pObject_ && pObject_->IsMarkedForDestruction() )
        pObject_ = 0;

    boost::shared_ptr< DEC_Knowledge_Object > pKnowledge;
    int nReturn = role_.Construct( pObject_, pKnowledge );
    Callback( nReturn ); // $$$$ LDC: Was DIA3 Parameter 0
    CallbackKnowledge( pKnowledge ); // $$$$ LDC: Was DIA3 Parameter 1
    // $$$$ LDC: Could as well hardcode the fact that myself.objMisEnCours_ = pKnowledge ? pKnowledge->GetID() : 0
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

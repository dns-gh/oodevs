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
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/ConstructionAttribute.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Decision/DEC_Tools.h"
#include "Decision/DEC_Gen_Object.h"
#include "MIL_AgentServer.h"

namespace
{
    MIL_Object_ABC* CreateObject( MIL_AgentPion& pion, const boost::shared_ptr< DEC_Gen_Object >& pGenObject )
    {
        if( !pGenObject )
            throw MASA_EXCEPTION( "Invalid genObject in PHY_ActionPrepareObject" );
        return MIL_AgentServer::GetWorkspace().GetEntityManager().CreateObject( &pion.GetArmy(), pGenObject->GetTypeName(), &pGenObject->GetLocalisation(), pGenObject->GetActivated(), pGenObject->GetExternalIdentifier(), pGenObject->GetName() );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionPrepareObject constructor
// Prepared: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionPrepareObject::PHY_ActionPrepareObject( MIL_AgentPion& pion, boost::shared_ptr< DEC_Gen_Object > pGenObject )
    : PHY_DecisionCallbackAction_ABC( pion )
    , role_( pion.GetRole< PHY_RoleAction_Objects >() )
    , pObject_( CreateObject( pion, pGenObject ) )
{
    pObject_->Initialize( *pGenObject );
    pObject_->RetrieveAttribute< ConstructionAttribute >()->Set( 0. );
    pObject_->RetrieveAttribute< ConstructionAttribute >()->NotifyBuildByGen();
    Callback( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionPrepareObject destructor
// Prepared: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionPrepareObject::~PHY_ActionPrepareObject()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionPrepareObject::StopAction
// Created: LDC 2010-03-11
// -----------------------------------------------------------------------------
void PHY_ActionPrepareObject::StopAction()
{
    if( pObject_ )
        pObject_->RetrieveAttribute< ConstructionAttribute >()->NotifyStopBuildByGen();
    Callback( role_.GetFinalReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionPrepareObject::Execute
// Prepared: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionPrepareObject::Execute()
{
    CleanObject();
    boost::shared_ptr< DEC_Knowledge_Object > pKnowledge;
    int nReturn = role_.Construct( pObject_, pKnowledge, false );
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
    CleanObject();
    role_.ConstructSuspended();
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionPrepareObject::CleanObject
// Created: LDC 2014-05-19
// -----------------------------------------------------------------------------
void PHY_ActionPrepareObject::CleanObject()
{
    if( pObject_ && ( pObject_->IsMarkedForDestruction() || pObject_->IsMarkedForDestructionNextUpdate() ) )
        pObject_ = 0;
}
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
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/AltitudeModifierAttribute.h"
#include "Entities/Objects/BridgingCapacity.h"
#include "Entities/Objects/ConstructionAttribute.h"
#include "Entities/Objects/LodgingAttribute.h"
#include "Entities/Objects/MineAttribute.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/TimeLimitedAttribute.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Decision/DEC_Tools.h"
#include "Decision/DEC_Gen_Object.h"
#include "MIL_AgentServer.h"

namespace
{
    MIL_Object_ABC* CreateObject( MIL_AgentPion& pion, const boost::shared_ptr< DEC_Gen_Object >& pGenObject )
    {
        if( !pGenObject )
            throw MASA_EXCEPTION( "Invalid genObject in PHY_ActionConstructObject" );
        return MIL_AgentServer::GetWorkspace().GetEntityManager().CreateObject( &pion.GetArmy(),
                                                                         pGenObject->GetTypeName(),
                                                                         &pGenObject->GetLocalisation(),
                                                                         pGenObject->GetActivated(),
                                                                         pGenObject->GetExternalIdentifier(),
                                                                         pGenObject->GetName(),
                                                                         pGenObject->GetDensity() );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionConstructObject constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionConstructObject::PHY_ActionConstructObject( MIL_AgentPion& pion, boost::shared_ptr< DEC_Gen_Object > pGenObject, bool instantaneous )
    : PHY_DecisionCallbackAction_ABC( pion )
    , role_         ( pion.GetRole< PHY_RoleAction_Objects >() )
    , pObject_      ( CreateObject( pion, pGenObject ) )
    , instantaneous_( instantaneous )
{
    role_.SetCreator( *pObject_ );
    pObject_->Initialize( *pGenObject );
    if( !instantaneous_ )
    {
        ConstructionAttribute* attribute = pObject_->RetrieveAttribute< ConstructionAttribute >();
        if( attribute )
        {
            attribute->Set( 0. );//default construction is set to 100%
            attribute->NotifyBuildByGen();
        }
        MineAttribute* mineAttribute = pObject_->RetrieveAttribute< MineAttribute >();
        if( mineAttribute )
        {
            mineAttribute->Set( 0. ); //default valorization is set to 100%
            mineAttribute->SetMustBeMined( pGenObject->GetMining() );
        }
        TimeLimitedAttribute* timeLimitedAttribute = pObject_->RetrieveAttribute< TimeLimitedAttribute >();
        if( timeLimitedAttribute )
            timeLimitedAttribute->SetLifeTime( static_cast< unsigned int >( pGenObject->GetTimeLimit() ) );
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
    {
        ConstructionAttribute* pConstruction = pObject_->RetrieveAttribute< ConstructionAttribute >();
        if( pConstruction && pConstruction->IsConstructed() )
        {
            BridgingCapacity* pBridging = pObject_->Retrieve< BridgingCapacity >();
            if( pBridging )
                pBridging->CreateBridge();
        }
        pObject_->RetrieveAttribute< ConstructionAttribute >()->NotifyStopBuildByGen();
    }
    Callback( role_.GetFinalReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionConstructObject::Execute
// Constructd: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionConstructObject::Execute()
{
    CleanObject();
    boost::shared_ptr< DEC_Knowledge_Object > pKnowledge;
    int nReturn = role_.Construct( pObject_, pKnowledge, instantaneous_ );
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
    CleanObject();
    role_.ConstructSuspended();
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionConstructObject::CleanObject
// Created: LDC 2014-05-19
// -----------------------------------------------------------------------------
void PHY_ActionConstructObject::CleanObject()
{
    if( pObject_ && ( pObject_->IsMarkedForDestruction() || pObject_->IsMarkedForDestructionNextUpdate() ) )
        pObject_ = 0;
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_Federate.cpp $
// $Author: Age $
// $Modtime: 24/02/05 15:13 $
// $Revision: 25 $
// $Workfile: HLA_Federate.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "HLA_Federate.h"
#include "HLA_TimeFactory.h"
#include "HLA_TimeIntervalFactory.h"
#include "HLA_Time.h"
#include "HLA_TimeInterval.h"
#include "HLA_DirectFire.h"
#include "HLA_IndirectFire.h"
#include "HLA_InteractionManager.h"
#include "HLA_LocalObject.h"
#include "HLA_DistantObject.h"
#include "MIL_AgentServer.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/Roles/Hla/HLA_RoleLocalPion.h"
#include "Entities/Agents/MIL_AgentHLA.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"
#include "Tools/MIL_IDManager.h"
#include <hla/hla_lib.h>
#include <windows.h> // For ::Sleep()

using namespace hla;

// -----------------------------------------------------------------------------
// Name: HLA_Federate constructor
// Created: AGE 2004-11-05
// -----------------------------------------------------------------------------
HLA_Federate::HLA_Federate( const std::string& strFederateName, unsigned int nTimeStepDuration )
    : pTimeFactory_    ( new HLA_TimeFactory() )
    , pIntervalFactory_( new HLA_TimeIntervalFactory() )
    , pAmbassador_     ( RtiAmbassador_ABC::CreateAmbassador( *pTimeFactory_, *pIntervalFactory_ ) )
    , pFederate_       ( new Federate( *pAmbassador_, strFederateName, HLA_Time(), HLA_TimeInterval( nTimeStepDuration ) ) )
    , pUnitClass_         ( 0 )
    , pUnitRegistration_  ( 0 )
    , pObjectClass_       ( 0 )
    , pObjectRegistration_( 0 )
    , pInteractionManager_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_Federate destructor
// Created: AGE 2004-11-05
// -----------------------------------------------------------------------------
HLA_Federate::~HLA_Federate()
{
    delete pUnitClass_;
    delete pUnitRegistration_;
    delete pObjectClass_;
    delete pObjectRegistration_;
    delete pInteractionManager_;
    delete pFederate_;
    delete pAmbassador_;
    delete pIntervalFactory_;
    delete pTimeFactory_;
}

// -----------------------------------------------------------------------------
// Name: HLA_Federate::Join
// Created: AGE 2004-11-05
// -----------------------------------------------------------------------------
bool HLA_Federate::Join( const std::string& strFederationName )
{
    try
    {
        assert( pFederate_ );
        if( ! pFederate_->Join( strFederationName ) )
            return false;
        InitializeClasses();
        return true;
    }
    catch( HLAException& e )
    {
        MT_LOG_ERROR_MSG( "Error joining federation '" << strFederationName << "' : " << e.what() );
        return false;
    }
}

namespace 
{
    struct UnitRegistration : public ObjectRegistration_ABC< HLA_RoleInterface >
    {
        UnitRegistration( HLA_InteractionManager& interactionManager )
            : interactionManager_( interactionManager )
        {}
        virtual HLA_RoleInterface& Create( const ObjectIdentifier& objectId )
        {
            MIL_AgentHLA* pPion = new MIL_AgentHLA( MIL_IDManager::units_.GetFreeSimID(), objectId, interactionManager_ );
            return pPion->GetRole< HLA_RoleInterface >();
        }
        virtual void Destroy( HLA_RoleInterface& object )
        {
            object.Destroy();
        }
    private:
        UnitRegistration& operator=( const UnitRegistration& );
        HLA_InteractionManager& interactionManager_;
    };

    class ObjectRegistration : public ObjectRegistration_ABC< HLA_Object_ABC >
    {
    public:
        ObjectRegistration( HLA_InteractionManager_ABC& interactionManager )
            : interactionManager_( interactionManager ) {};
        virtual HLA_Object_ABC& Create( const ObjectIdentifier& objectId )
        {
            return *new HLA_DistantObject( objectId, interactionManager_ );
        };
        virtual void Destroy( HLA_Object_ABC& object )
        {
            object.Destroy();
        };
    private:
        ObjectRegistration& operator=( const ObjectRegistration& );
        HLA_InteractionManager_ABC& interactionManager_;
    };
}

// -----------------------------------------------------------------------------
// Name: HLA_Federate::InitializeClasses
// Created: AGE 2004-11-24
// -----------------------------------------------------------------------------
void HLA_Federate::InitializeClasses()
{
    ::Sleep( 10000 ); // $$$$ AGE 2005-02-21: So that the FVT doesn't complain about the initialisation
    assert( pFederate_ );
    pInteractionManager_ = new HLA_InteractionManager( *pFederate_ );

    pUnitRegistration_ = new UnitRegistration( *pInteractionManager_ );
    pUnitClass_ = new Class< HLA_RoleInterface >( *pUnitRegistration_ );
    pUnitClass_->Register( AttributeIdentifier( "position" ) );
    pUnitClass_->Register( AttributeIdentifier( "hauteur" ) );
    pUnitClass_->Register( AttributeIdentifier( "direction" ) );
    pUnitClass_->Register( AttributeIdentifier( "vitesse" ) );
    pUnitClass_->Register( AttributeIdentifier( "type" ) );
    pUnitClass_->Register( AttributeIdentifier( "armee" ) );
    pUnitClass_->Register( AttributeIdentifier( "composantes" ) );
    pUnitClass_->Register( AttributeIdentifier( "posture" ) );
    pUnitClass_->Register( AttributeIdentifier( "status" ) );
    pFederate_->Register( ClassIdentifier( "Positionnable.Deplacable.Unite.UniteScipio" ), *pUnitClass_ );

    pObjectRegistration_ = new ObjectRegistration( *pInteractionManager_ );
    pObjectClass_ = new Class< HLA_Object_ABC >( *pObjectRegistration_ );
    pObjectClass_->Register( AttributeIdentifier( "coordonnees" ) );
    pObjectClass_->Register( AttributeIdentifier( "type" ) );
    pObjectClass_->Register( AttributeIdentifier( "armee" ) );
    pObjectClass_->Register( AttributeIdentifier( "completion" ) );
    pObjectClass_->Register( AttributeIdentifier( "valorisation" ) );
    pObjectClass_->Register( AttributeIdentifier( "contournement" ) );
    pObjectClass_->Register( AttributeIdentifier( "option" ) );
    pObjectClass_->Register( AttributeIdentifier( "extra" ) );
    pFederate_->Register( ClassIdentifier( "Localisable.ObjetScipio" ), *pObjectClass_ );    
    ::Sleep( 10000 );
}

// -----------------------------------------------------------------------------
// Name: HLA_Federate::Register
// Created: AGE 2004-11-05
// -----------------------------------------------------------------------------
void HLA_Federate::Register( MIL_AgentPion& agent )
{
    assert( pUnitClass_ );
    HLA_RoleLocalPion& hlaRole = (HLA_RoleLocalPion &) agent.RegisterRole< HLA_RoleLocalPion >( agent );
    const ObjectIdentifier objectId = pUnitClass_->Register( hlaRole );
    hlaRole.SetId( objectId );
    localAgents_[ objectId ] = &agent;
}

// -----------------------------------------------------------------------------
// Name: HLA_Federate::Register
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
void HLA_Federate::Register( MIL_RealObject_ABC& object )
{
    assert( pObjectClass_ );
    assert( pInteractionManager_ );
    if( object.GetHLAView() )
        return;
    HLA_LocalObject& hlaView = * new HLA_LocalObject( object );
    const ObjectIdentifier objectId = pObjectClass_->Register( hlaView );
    hlaView.SetId( objectId );
    object.SetHLAView( hlaView );
    localObjects_[ objectId ] = &object ;
}

// -----------------------------------------------------------------------------
// Name: HLA_Federate::Unregister
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
void HLA_Federate::Unregister( MIL_RealObject_ABC& object )
{
    assert( pObjectClass_ );
    if( ! object.GetHLAView() )
        throw std::exception( "local object has no HLA view" );
    localObjects_.erase( object.GetHLAView()->GetId() );
    pObjectClass_->Unregister( *object.GetHLAView() );
}

// -----------------------------------------------------------------------------
// Name: HLA_Federate::Step
// Created: AGE 2004-11-16
// -----------------------------------------------------------------------------
void HLA_Federate::Step()
{
    assert( pFederate_ );
    pFederate_->Step();
}

// -----------------------------------------------------------------------------
// Name: HLA_Federate::Tick
// Created: AGE 2004-11-23
// -----------------------------------------------------------------------------
void HLA_Federate::Tick()
{
    assert( pFederate_ );
    pFederate_->Tick();
}

// -----------------------------------------------------------------------------
// Name: HLA_Federate::GetLocalAgent
// Created: AGE 2004-11-26
// -----------------------------------------------------------------------------
MIL_Agent_ABC* HLA_Federate::GetLocalAgent( const ObjectIdentifier& objectId ) const
{
    CIT_AgentMap itAgent = localAgents_.find( objectId );
    if( itAgent != localAgents_.end() )
        return itAgent->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: HLA_Federate::GetLocalObject
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
MIL_RealObject_ABC* HLA_Federate::GetLocalObject( const ObjectIdentifier& objectId ) const
{
    CIT_ObjectMap itObject = localObjects_.find( objectId );
    if( itObject != localObjects_.end() )
        return itObject->second;
    return 0;
}

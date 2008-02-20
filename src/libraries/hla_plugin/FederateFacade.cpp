// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "FederateFacade.h"
#include "AgentExtension.h"
#include "dispatcher/Agent.h"
#include "hla/hla_lib.h"
#include "hla/SimpleTimeFactory.h"
#include "hla/SimpleTimeIntervalFactory.h"
#include "hla/SimpleTimeInterval.h"
#include "MT/MT_Logger/MT_Logger_Lib.h"

using namespace hla;

// -----------------------------------------------------------------------------
// Name: FederateFacade constructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
FederateFacade::FederateFacade( const std::string& name, unsigned int timeStepDuration )
    : timeFactory_    ( new SimpleTimeFactory() )
    , intervalFactory_( new SimpleTimeIntervalFactory() )
    , ambassador_     ( RtiAmbassador_ABC::CreateAmbassador( *timeFactory_, *intervalFactory_ ) )
    , federate_       ( new Federate( *ambassador_, name, SimpleTimeInterval( timeStepDuration ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FederateFacade destructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
FederateFacade::~FederateFacade()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::Join
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
bool FederateFacade::Join( const std::string& name )
{
    try
    {
        if( ! federate_->Join( name, false, false ) )
            return false;
        InitializeClasses();
        return true;
    }
    catch( HLAException& e )
    {
        MT_LOG_ERROR_MSG( "Error joining federation '" << name << "' : " << e.what() );
        return false;
    }
}

namespace
{
    struct UnitRegistrar : public ObjectRegistration_ABC< AgentExtension >
    {
        virtual AgentExtension& Create ( const ObjectIdentifier& /*objectID*/ )
        {
            throw std::runtime_error( "Remote unit creation not supported." );
        }
        virtual void Destroy( AgentExtension& /*object*/ )
        {
            throw std::runtime_error( "Remote unit destruction not supported." );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::InitializeClasses
// Created: SBO 2008-02-19
// -----------------------------------------------------------------------------
void FederateFacade::InitializeClasses()
{
    UnitRegistrar* unitRegistrar = new UnitRegistrar();
    agentClass_.reset( new Class< AgentExtension >( *unitRegistrar ) );
    {
        agentClass_->Register( AttributeIdentifier( "EntityType" ) );
        agentClass_->Register( AttributeIdentifier( "EntityIdentifier" ) );
        agentClass_->Register( AttributeIdentifier( "Spatial" ) );
    }
    federate_->Register( ClassIdentifier( "BaseEntity.AggregateEntity" ), *agentClass_, true, false );
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::Register
// Created: SBO 2008-02-20
// -----------------------------------------------------------------------------
void FederateFacade::Register( dispatcher::Agent& entity )
{
    agentClass_->Register( entity.Get< AgentExtension >() );
}

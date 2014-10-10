// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "UnitModelReloader.h"
#include "ContextHandler_ABC.h"
#include "ContextFactory_ABC.h"
#include "RemoteAgentSubject_ABC.h"
#include "LocalAgentResolver_ABC.h"
#include "protocol/SimulationSenders.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/DecisionalModel.h"

using namespace plugins::hla;

namespace
{
    void doReloadBrain( const uint32_t id, const std::string& brain, dispatcher::SimulationPublisher_ABC& publisher, const ContextFactory_ABC& contextFactory )
    {
        simulation::UnitMagicAction reloadMessage;
        reloadMessage().mutable_tasker()->mutable_unit()->set_id( id );
        reloadMessage().set_type( sword::UnitMagicAction::reload_brain );
        *reloadMessage().mutable_parameters();
        if( !brain.empty() )
        {
            sword::MissionParameter& parameter = *reloadMessage().mutable_parameters()->add_elem();
            parameter.set_null_value( false );
            parameter.add_value()->set_acharstr( brain );
        }
        reloadMessage.Send( publisher, contextFactory.Create() );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitModelReloader constructor
// Created: SLI 2011-09-20
// -----------------------------------------------------------------------------
UnitModelReloader::UnitModelReloader( ContextHandler_ABC< sword::UnitCreation >& contextHandler, dispatcher::SimulationPublisher_ABC& publisher,
        const ContextFactory_ABC& contextFactory, const tools::Resolver_ABC< kernel::AgentType >& agentTypeResolver, RemoteAgentSubject_ABC& remoteSubject,
        LocalAgentResolver_ABC& localResolver )
    : contextHandler_( contextHandler )
    , publisher_     ( publisher )
    , contextFactory_( contextFactory )
    , agentTypeResolver_( agentTypeResolver )
    , remoteSubject_( remoteSubject )
    , localResolver_( localResolver )
{
    contextHandler_.Register( *this );
    remoteSubject_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitModelReloader destructor
// Created: SLI 2011-09-20
// -----------------------------------------------------------------------------
UnitModelReloader::~UnitModelReloader()
{
    contextHandler_.Unregister( *this );
    remoteSubject_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitModelReloader::Notify
// Created: SLI 2011-09-20
// -----------------------------------------------------------------------------
void UnitModelReloader::Notify( const sword::UnitCreation& message, const std::string& identifier )
{
    const uint32_t unitType = message.type().id();
    const uint32_t unitId = message.unit().id();
    const std::string brainType = agentTypeResolver_.Get( unitType ).GetDecisionalModel().GetName();
    brains_[identifier] = brainType;
    simIds_[identifier] = unitId;
    doReloadBrain( unitId, "", publisher_, contextFactory_ );
}

void UnitModelReloader::RemoteCreated( const std::string& /*identifier*/, HlaClass_ABC& /*hlaClass*/, HlaObject_ABC& /*object*/ )
{
    // NOTHING
}

void UnitModelReloader::RemoteDestroyed( const std::string& /*identifier*/ )
{
    // NOTHING
}

void UnitModelReloader::LocalCreated( const std::string& identifier, HlaClass_ABC& /*hlaClass*/, HlaObject_ABC& /*object*/ )
{
    const unsigned long unitId = localResolver_.Resolve( identifier );
    const unsigned long agentType = localResolver_.AgentType( unitId );
    const std::string brainType = agentTypeResolver_.Get( agentType ).GetDecisionalModel().GetName();
    brains_[identifier] = brainType;
    simIds_[identifier] = unitId;
}

void UnitModelReloader::LocalDestroyed( const std::string& /*identifier*/ )
{
    // NOTHING
}

void UnitModelReloader::Divested( const std::string& identifier, const T_AttributeIdentifiers& /*attributes*/ )
{
    auto itSimId = simIds_.find( identifier );
    if( itSimId != simIds_.end() )
    {
        doReloadBrain( itSimId->second, "", publisher_, contextFactory_ );
    }
}

void UnitModelReloader::Acquired( const std::string& identifier, const T_AttributeIdentifiers& /*attributes*/ )
{
    auto itSimId = simIds_.find( identifier );
    auto itBrain = brains_.find( identifier );
    if( itSimId != simIds_.end() && itBrain != brains_.end() )
    {
        doReloadBrain( itSimId->second, itBrain->second, publisher_, contextFactory_ );
    }
}


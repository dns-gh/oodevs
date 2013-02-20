// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "ActionsLogger.h"
#include "actions/Action_ABC.h"
#include "actions/ActionFactory.h"
#include "actions/ActionParameterFactory.h"
#include "actions/ActionsModel.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter.h"
#include "clients_kernel/Time_ABC.h"
#include "dispatcher/AgentKnowledgeConverter.h"
#include "dispatcher/ModelAdapter.h"
#include "dispatcher/Model_ABC.h"
#include "dispatcher/ObjectKnowledgeConverter.h"
#include "protocol/Protocol.h"
#include "protocol/ServerPublisher_ABC.h"
#include "tools/SessionConfig.h"
#pragma warning( push, 0 )
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )

namespace bfs = boost::filesystem;

using namespace plugins;
using namespace logger;

namespace
{
    struct NullPublisher : public Publisher_ABC
    {
        virtual void Send( const sword::ClientToSim& /*message*/ ) {}
        virtual void Send( const sword::ClientToAuthentication& /*message*/ ) {}
        virtual void Send( const sword::ClientToReplay& /*message*/ ) {}
        virtual void Send( const sword::ClientToAar& /*message*/ ) {}
        virtual void Send( const sword::ClientToMessenger& /*message*/ ) {}

    };
}

// -----------------------------------------------------------------------------
// Name: ActionsLogger constructor
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
ActionsLogger::ActionsLogger( const tools::SessionConfig& config, const dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel, const kernel::Time_ABC& simulation )
    : config_           ( config )
    , entities_         ( new dispatcher::ModelAdapter( model ) )
    , controller_       ( new kernel::Controller() )
    , converter_        ( new kernel::CoordinateConverter( config ) )
    , publisher_        ( new NullPublisher() )
    , agentsKnowledges_ ( new dispatcher::AgentKnowledgeConverter( model ) )
    , objectsKnowledges_( new dispatcher::ObjectKnowledgeConverter( model ) )
    , parameters_       ( new actions::ActionParameterFactory( *converter_, *entities_, staticModel, *agentsKnowledges_, *objectsKnowledges_, *controller_ ) )
    , factory_          ( new actions::ActionFactory( *controller_, *parameters_, *entities_, staticModel, simulation ) )
    , actions_          ( new actions::ActionsModel( *factory_, *publisher_, *publisher_ ) )
    , ordersLoaded_     ( !config_.HasCheckpoint() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionsLogger destructor
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
ActionsLogger::~ActionsLogger()
{
    try
    {
        Commit();
    }
    catch( ... )
    {
    }
}

// -----------------------------------------------------------------------------
// Name: ActionsLogger::LogAction
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
template< typename T >
void ActionsLogger::LogAction( const T& message )
{
    LoadOrdersIfCheckpoint();
    actions::Action_ABC* action = factory_->CreateAction( message );
    actions_->Register( action->GetId(), *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsLogger::Commit
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
void ActionsLogger::Commit() const
{
    actions_->Save( config_.BuildSessionChildFile( "actions.ord" ) );
}

// -----------------------------------------------------------------------------
// Name: ActionsLogger::LoadOrdersIfCheckpoint
// Created: JSR 2010-11-05
// -----------------------------------------------------------------------------
void ActionsLogger::LoadOrdersIfCheckpoint()
{
    if( !ordersLoaded_ && config_.HasCheckpoint()  )
    {
        std::string actionsPath( config_.BuildSessionChildFile( "actions.ord" ) );
        ordersLoaded_ = true;
        if( bfs::exists( bfs::path( actionsPath ) ) )
            actions_->Load( actionsPath, config_.GetLoader() );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionsLogger::Log
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
void ActionsLogger::Log( const sword::UnitOrder& message )
{
    if( message.has_type() && message.type().id() != 0 )
        LogAction( message );
}

// -----------------------------------------------------------------------------
// Name: ActionsLogger::Log
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
void ActionsLogger::Log( const sword::AutomatOrder& message )
{
    if( message.has_type() && message.type().id() != 0 )
        LogAction( message );
}

// -----------------------------------------------------------------------------
// Name: ActionsLogger::Log
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
void ActionsLogger::Log( const sword::CrowdOrder& message )
{
    if( message.has_type() && message.type().id() != 0 )
        LogAction( message );
}

// -----------------------------------------------------------------------------
// Name: ActionsLogger::Log
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
void ActionsLogger::Log( const sword::FragOrder& message )
{
    if( message.has_type() && message.type().id() != 0 )
        LogAction( message );
}

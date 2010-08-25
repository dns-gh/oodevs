// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#pragma warning( push, 0 )
#include <qstring.h>
#pragma warning( pop )

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
#include "dispatcher/ObjectKnowledgeConverter.h"
#include "protocol/protocol.h"
#include "protocol/Publisher_ABC.h"
#include "tools/SessionConfig.h"

using namespace plugins;
using namespace logger;

namespace
{
    struct NullPublisher : public Publisher_ABC
    {
        virtual void Send( const MsgsClientToSim::MsgClientToSim& /*message*/ ) {}
        virtual void Send( const MsgsClientToAuthentication::MsgClientToAuthentication& /*message*/ ) {}
        virtual void Send( const MsgsClientToReplay::MsgClientToReplay& /*message*/ ) {}
        virtual void Send( const MsgsClientToAar::MsgClientToAar& /*message*/ ) {}
        virtual void Send( const MsgsClientToMessenger::MsgClientToMessenger& /*message*/ ) {}
    };
}

// -----------------------------------------------------------------------------
// Name: ActionsLogger constructor
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
ActionsLogger::ActionsLogger( const tools::SessionConfig& config, const dispatcher::Model& model, const kernel::StaticModel& staticModel, const kernel::Time_ABC& simulation )
    : config_           ( config )
    , entities_         ( new dispatcher::ModelAdapter( model ) )
    , controller_       ( new kernel::Controller() )
    , converter_        ( new kernel::CoordinateConverter( config ) )
    , publisher_        ( new NullPublisher() )
    , agentsKnowledges_ ( new dispatcher::AgentKnowledgeConverter( model ) )
    , objectsKnowledges_( new dispatcher::ObjectKnowledgeConverter( model ) )
    , parameters_       ( new actions::ActionParameterFactory( *converter_, *entities_, staticModel, *agentsKnowledges_, *objectsKnowledges_, *controller_ ) )
    , factory_          ( new actions::ActionFactory( *controller_, *parameters_, *entities_, staticModel, simulation ) )
    , actions_          ( new actions::ActionsModel( *factory_, *publisher_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionsLogger destructor
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
ActionsLogger::~ActionsLogger()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionsLogger::LogAction
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
template< typename T >
void ActionsLogger::LogAction( const T& message )
{
    actions::Action_ABC* action = factory_->CreateAction( message );
    actions_->Register( action->GetId(), *action );
    Commit();
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
// Name: ActionsLogger::Log
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
void ActionsLogger::Log( const Common::MsgUnitOrder& message )
{
    if( message.has_mission() && message.mission() != 0 )
        LogAction( message );
}

// -----------------------------------------------------------------------------
// Name: ActionsLogger::Log
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
void ActionsLogger::Log( const Common::MsgAutomatOrder& message )
{
    if( message.has_mission() && message.mission() != 0 )
        LogAction( message );
}

// -----------------------------------------------------------------------------
// Name: ActionsLogger::Log
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
void ActionsLogger::Log( const Common::MsgPopulationOrder& message )
{
    if( message.has_mission() && message.mission() != 0 )
        LogAction( message );
}

// -----------------------------------------------------------------------------
// Name: ActionsLogger::Log
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
void ActionsLogger::Log( const MsgsClientToSim::MsgFragOrder& message )
{
    if( message.has_frag_order() && message.frag_order() != 0 )
        LogAction( message );
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "Actions.h"
#include "actions/Action_ABC.h"
#include "actions/ActionFactory.h"
#include "actions/ActionParameterFactory.h"
#include "actions/ActionsModel.h"
#include "clients_kernel/CoordinateConverter.h"
#include "clients_kernel/Time_ABC.h"
#include "directia/brain/Brain.h"
#include "dispatcher/AgentKnowledgeConverter.h"
#include "dispatcher/ModelAdapter.h"
#include "dispatcher/Model_ABC.h"
#include "dispatcher/ObjectKnowledgeConverter.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include "protocol/protocol.h"
#include "protocol/ServerPublisher_ABC.h"
#include "tools/ExerciseConfig.h"
#include <xeumeuleu/xml.h>

using namespace plugins::script;

namespace
{
    struct SimulationTime : public kernel::Time_ABC
    {
        virtual QDateTime GetDateTime() const { return QDateTime(); }
        virtual QDateTime GetInitialDateTime() const { return QDateTime(); }
        virtual unsigned int GetTickDuration() const { return 0; }
    };
}

// -----------------------------------------------------------------------------
// Name: Actions Publisher
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
struct Actions::Publisher : public Publisher_ABC
{
    explicit Publisher( dispatcher::SimulationPublisher_ABC& sim ) : sim_( &sim ) {}

    virtual void Send( const MsgsClientToSim::MsgClientToSim& message )
    {
        sim_->Send( message );
    }
    virtual void Send( const MsgsClientToAuthentication::MsgClientToAuthentication&  ) {}
    virtual void Send( const MsgsClientToReplay::MsgClientToReplay& ){}
    virtual void Send( const MsgsClientToAar::MsgClientToAar& ) {}
    virtual void Send( const MsgsClientToMessenger::MsgClientToMessenger& ) {}
    dispatcher::SimulationPublisher_ABC* sim_;
};

// -----------------------------------------------------------------------------
// Name: Actions constructor
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
Actions::Actions( kernel::Controller& controller, const tools::ExerciseConfig& config, const dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel, dispatcher::SimulationPublisher_ABC& sim )
    : entities_         ( new dispatcher::ModelAdapter( model ) )
    , publisher_        ( new Publisher( sim ) )
    , converter_        ( new kernel::CoordinateConverter( config ) )
    , time_             ( new SimulationTime() )
    , agentsKnowledges_ ( new dispatcher::AgentKnowledgeConverter( model ) )
    , objectsKnowledges_( new dispatcher::ObjectKnowledgeConverter( model ) )
    , parameters_       ( new actions::ActionParameterFactory( *converter_, *entities_, staticModel, *agentsKnowledges_, *objectsKnowledges_, controller ) )
    , factory_          ( new actions::ActionFactory( controller, *parameters_, *entities_, staticModel, *time_ ) )
    , file_             ( config.BuildExerciseChildFile( "scripts/resources/orders.ord" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Actions destructor
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
Actions::~Actions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Actions::RegisterIn
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
void Actions::RegisterIn( directia::brain::Brain& brain )
{
    brain[ "actions" ] = this;
    brain.Register( "IssueOrder", &Actions::IssueOrder );
    brain.Register( "IssueXmlOrder", &Actions::IssueXmlOrder );
}

// -----------------------------------------------------------------------------
// Name: Actions::IssueOrder
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
void Actions::IssueOrder( const std::string& name )
{
    try
    {
        actions::ActionsModel model( *factory_, *publisher_ );
        model.Load( file_ );
        tools::Iterator< const actions::Action_ABC& > it = model.CreateIterator();
        while( it.HasMoreElements() )
        {
            const actions::Action_ABC& action = it.NextElement();
            if( action.GetName() == name.c_str() )
                action.Publish( *publisher_ );
        }
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Error in script: " << e.what() )
    }
}

// -----------------------------------------------------------------------------
// Name: Actions::IssueXmlOrder
// Created: AGE 2008-07-17
// -----------------------------------------------------------------------------
void Actions::IssueXmlOrder( const std::string& name )
{
    try
    {
        xml::xistringstream xis( name );
        xis >> xml::start( "action" );
        std::auto_ptr< actions::Action_ABC > action( factory_->CreateAction( xis ) );
        if( action.get() )
             action->Publish( *publisher_ );
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Error in script: " << e.what() )
    }
}

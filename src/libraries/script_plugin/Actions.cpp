// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "Actions.h"
#include "ActionParameterFactory.h"
#include "ActionFactory.h"
#include "actions/Action_ABC.h"
#include "actions/ActionsModel.h"
#include "clients_kernel/CoordinateConverter.h"
#include "directia/Brain.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include "protocol/protocol.h"
#include "protocol/Publisher_ABC.h"
#include "tools/ExerciseConfig.h"
#include <xeumeuleu/xml.h>

using namespace plugins::script;

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
Actions::Actions( kernel::Controller& controller, const tools::ExerciseConfig& config, const dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& sim )
    : publisher_( new Publisher( sim ) )
    , converter_( new kernel::CoordinateConverter( config ) )
    , parameters_( new ActionParameterFactory( *converter_, model, config, controller ) )
    , factory_( new ActionFactory( controller, model, *parameters_ ) )
    , file_   ( config.BuildExerciseChildFile( "scripts/resources/orders.ord" ) )
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
void Actions::RegisterIn( directia::Brain& brain )
{
    brain.RegisterObject( "actions", this );
    brain.RegisterFunction( "IssueOrder", &Actions::IssueOrder );
    brain.RegisterFunction( "IssueXmlOrder", &Actions::IssueXmlOrder );
}

// -----------------------------------------------------------------------------
// Name: Actions::IssueOrder
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
void Actions::IssueOrder( const std::string& name )
{
    try
    {
        actions::ActionsModel model( *factory_ );
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

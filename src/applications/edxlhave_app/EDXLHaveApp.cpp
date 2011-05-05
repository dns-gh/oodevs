// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "edxlhave_app_pch.h"
#include "EDXLHaveApp.h"
#include "EDXLReport.h"
#include "PublisherActor.h"
#include "Publisher.h"
#include "Simulation.h"
#include "UpdateListener.h"
#include "Model.h"
#include "Hospital.h"
#include "SwordClient.h"
#include <boost/bind.hpp>

using namespace edxl;

// -----------------------------------------------------------------------------
// Name: EDXLHaveApp constructor
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
EDXLHaveApp::EDXLHaveApp( const boost::program_options::variables_map& options )
: options_( options )
, model_( new Model( options ) )
, client_ ( new SwordClient() )
, reports_ ( new EDXLReport( *model_ ) )
, listener_ ( new UpdateListener( *model_, *client_ ) )
{
    publisher_.reset( new PublisherActor( std::auto_ptr< Publisher_ABC >( new Publisher( options_ ) ) ) );
    simulation_.reset( new Simulation( options_ ) );

    if( options[ "log" ].as< bool >() )
        std::clog <<"EDXLHaveApp : registered." << std::endl;
}

// -----------------------------------------------------------------------------
// Name: EDXLHaveApp destructor
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
EDXLHaveApp::~EDXLHaveApp()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: EDXLHaveApp::Run
// Created: BCI 2011-05-02
// -----------------------------------------------------------------------------
void EDXLHaveApp::Run()
{
    // try to connect client to host
    client_->Connect( options_[ "sword.server" ].as< std::string >(), options_[ "sword.port" ].as< std::string >(), options_[ "sword.waitconnection" ].as< bool >() );

    // send authentication request
    {
        const int client_identifier = 42;
        sword::ClientToAuthentication message;
        message.set_context( client_identifier );
        sword::AuthenticationRequest& request = *message.mutable_message()->mutable_authentication_request();
        request.mutable_version()->set_value( sword::ProtocolVersion().value() );
        request.set_login( options_[ "sword.profile" ].as< std::string >() );
        request.set_password( options_[ "sword.password" ].as< std::string >() );
        client_->Send( message );
    }

    client_->SetListener( boost::bind( &EDXLHaveApp::Receive, this, _1 ) );

    // receive and process messages until server ends
    while( client_->Receive() )
        ;
}


// -----------------------------------------------------------------------------
// Name: EDXLHaveApp::Receive
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
void EDXLHaveApp::Receive( const sword::SimToClient& wrapper )
{
    if ( wrapper.message().has_control_information() )
        simulation_->Receive( wrapper.message().control_information() );
    else if ( wrapper.message().has_object_magic_action_ack() )
        simulation_->Receive( wrapper.message().object_magic_action_ack() );
    else if ( wrapper.message().has_control_send_current_state_end() )
        publisher_->PullSituation( "", *listener_ );
    else if ( wrapper.message().has_control_end_tick() )
    {
        // Check update
        simulation_->Receive( wrapper.message().control_end_tick() );
        if( simulation_->NeedUpdate() )
        {
            reports_->Publish( *publisher_ );
            simulation_->Update( wrapper.message().control_end_tick() );
        }
    }
    else if( wrapper.message().has_object_creation() )
        model_->Receive( wrapper.message().object_creation() );
    else if( wrapper.message().has_object_update() )
    {
        const Hospital* hospital = model_->Receive( wrapper.message().object_update() );
        if( hospital )
            reports_->CreateReport( hospital->GetId(), hospital->GetName(), wrapper.message().object_update().attributes().medical_treatment() );
    }
}

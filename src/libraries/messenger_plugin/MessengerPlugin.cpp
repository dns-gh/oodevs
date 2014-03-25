// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "messenger_plugin_pch.h"
#include "MessengerPlugin.h"
#include "Model.h"
#include "TacticalLinesModel.h"
#include "DrawingsModel.h"
#include "NotesModel.h"
#include "ClientObjectsModel.h"
#include "Chat.h"
#include "protocol/ClientPublisher_ABC.h"
#include "dispatcher/LinkResolver_ABC.h"
#include "dispatcher/Services.h"
#include "protocol/MessengerSenders.h"
#include "protocol/Protocol.h"
#include "tools/MessageDispatcher_ABC.h"

using namespace plugins::messenger;

// -----------------------------------------------------------------------------
// Name: MessengerPlugin constructor
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
MessengerPlugin::MessengerPlugin( dispatcher::ClientPublisher_ABC& clients, tools::MessageDispatcher_ABC& dispatcher, dispatcher::LinkResolver_ABC& links, const dispatcher::Config& config, dispatcher::CompositeRegistrable& registrables )
    : config_ ( config )
    , clients_( clients )
    , links_  ( links )
    , model_  ( new Model( config, clients, registrables ) )
    , chat_   ( new Chat() )
{
    dispatcher.RegisterMessage( *this, &MessengerPlugin::OnReceiveClientToMessenger );
}

// -----------------------------------------------------------------------------
// Name: MessengerPlugin destructor
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
MessengerPlugin::~MessengerPlugin()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MessengerPlugin::Register
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
void MessengerPlugin::Register( dispatcher::Services& services )
{
    services.Declare( sword::service_messenger );
}

// -----------------------------------------------------------------------------
// Name: MessengerPlugin::Receive
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void MessengerPlugin::Receive( const sword::SimToClient& wrapper )
{
    if( wrapper.message().has_control_checkpoint_save_end() )
        model_->Save( wrapper.message().control_checkpoint_save_end().name() );
    if( wrapper.message().has_control_begin_tick() )
        model_->UpdateTime( wrapper.message().control_begin_tick().date_time().data() );
    if( wrapper.message().has_unit_destruction() )
        model_->DeleteUnit( wrapper.message().unit_destruction().unit().id() );
}

// -----------------------------------------------------------------------------
// Name: MessengerPlugin::NotifyClientAuthenticated
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void MessengerPlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, const std::string& /*link*/,
                                                 dispatcher::Profile_ABC& profile, unsigned int /*clientId*/, bool /*uncounted*/ )
{
    model_->SendStateToNewClient( client );
    chat_->NotifyClientAuthenticated( client, profile );
}

// -----------------------------------------------------------------------------
// Name: MessengerPlugin::NotifyClientLeft
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void MessengerPlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& client, const std::string& /*link*/, bool /*uncounted*/ )
{
    chat_->NotifyClientLeft( client );
}

// -----------------------------------------------------------------------------
// Name: MessengerPlugin::OnReceiveClientToMessenger
// Created: RDS 2008-04-03
// -----------------------------------------------------------------------------
void MessengerPlugin::OnReceiveClientToMessenger( const std::string& client, const sword::ClientToMessenger& wrapper )
{
    unsigned int nCtx = wrapper.has_context()? wrapper.context() : 0;
    dispatcher::ClientPublisher_ABC& publisher = links_.GetConnectedPublisher( client );
    // Limit
    if( wrapper.message().has_limit_creation_request() )
        model_->tacticalLines_.HandleLimitRequest( publisher, wrapper.message().limit_creation_request(), nCtx );
    if( wrapper.message().has_limit_destruction_request() )
        model_->tacticalLines_.HandleLimitRequest( publisher, wrapper.message().limit_destruction_request() );
    if( wrapper.message().has_limit_update_request() )
        model_->tacticalLines_.HandleLimitRequest( publisher, wrapper.message().limit_update_request() );
    // Lima
    if( wrapper.message().has_phase_line_creation_request() )
        model_->tacticalLines_.HandleLimaRequest( publisher, wrapper.message().phase_line_creation_request(), nCtx );
    if( wrapper.message().has_phase_line_destruction_request() )
        model_->tacticalLines_.HandleLimaRequest( publisher, wrapper.message().phase_line_destruction_request() );
    if( wrapper.message().has_phase_line_update_request() )
        model_->tacticalLines_.HandleLimaRequest( publisher, wrapper.message().phase_line_update_request() );
    // Drawings
    if( wrapper.message().has_shape_creation_request() )
        model_->drawings_.HandleRequest( publisher, wrapper.message().shape_creation_request(), nCtx );
    if( wrapper.message().has_shape_update_request() )
        model_->drawings_.HandleRequest( publisher, wrapper.message().shape_update_request() );
    if( wrapper.message().has_shape_destruction_request() )
        model_->drawings_.HandleRequest( publisher, wrapper.message().shape_destruction_request() );
    // Chat
    if( wrapper.message().has_text_message() )
        chat_->OnReceive( wrapper.message().text_message() );
    // Notes
    if( wrapper.message().has_marker_creation_request() )
        model_->notes_.HandleRequest( wrapper.message().marker_creation_request(), nCtx );
    if( wrapper.message().has_marker_update_request() )
        model_->notes_.HandleRequest( wrapper.message().marker_update_request() );
    if( wrapper.message().has_marker_destruction_request() )
        model_->notes_.HandleRequest( wrapper.message().marker_destruction_request() );
    // Client objects
    if( wrapper.message().has_client_object_creation_request() )
        model_->clientObjects_.HandleRequest( publisher, wrapper.message().client_object_creation_request() );
    if( wrapper.message().has_client_object_update_request() )
        model_->clientObjects_.HandleRequest( publisher, wrapper.message().client_object_update_request() );
    if( wrapper.message().has_client_object_destruction_request() )
        model_->clientObjects_.HandleRequest( publisher, wrapper.message().client_object_destruction_request() );
}

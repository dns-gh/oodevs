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
#include "IntelligencesModel.h"
#include "DrawingsModel.h"
#include "Chat.h"
#include "dispatcher/ClientPublisher_ABC.h"
#include "dispatcher/LinkResolver_ABC.h"
#include "dispatcher/Services.h"
#include "tools/MessageDispatcher_ABC.h"

using namespace plugins::messenger;

// -----------------------------------------------------------------------------
// Name: MessengerPlugin constructor
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
MessengerPlugin::MessengerPlugin( dispatcher::ClientPublisher_ABC& clients, tools::MessageDispatcher_ABC& dispatcher, dispatcher::LinkResolver_ABC& links, const dispatcher::Config& config, dispatcher::CompositeRegistrable& registrables )
    : config_( config )
    , clients_( clients )
    , links_( links )
    , model_( *new Model( config, clients, registrables ) )
    , chat_( new Chat() )
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
    services.Declare< Service >();
}

// -----------------------------------------------------------------------------
// Name: MessengerPlugin::Receive
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void MessengerPlugin::Receive( const ASN1T_MsgsSimToClient& message )
{
    switch( message.msg.t )
    {
    case T_MsgsSimToClient_msg_msg_control_checkpoint_save_end:
        model_.Save( message.msg.u.msg_control_checkpoint_save_end->name );
        break;
    default:
        ;
    }
}

// -----------------------------------------------------------------------------
// Name: MessengerPlugin::NotifyClientAuthenticated
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void MessengerPlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile )
{
    model_.SendStateToNewClient( client );
    chat_->NotifyClientAuthenticated( client, profile );
}

// -----------------------------------------------------------------------------
// Name: MessengerPlugin::NotifyClientLeft
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void MessengerPlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& client )
{
    chat_->NotifyClientLeft( client );
}

// -----------------------------------------------------------------------------
// Name: MessengerPlugin::OnReceiveClientToMessenger
// Created: RDS 2008-04-03
// -----------------------------------------------------------------------------
void MessengerPlugin::OnReceiveClientToMessenger( const std::string& client, const ASN1T_MsgsClientToMessenger& message )
{
    dispatcher::ClientPublisher_ABC& publisher = links_.GetPublisher( client );
    switch( message.t )
    {
    // Limit
    case T_MsgsClientToMessenger_msg_limit_creation_request:
        model_.tacticalLines_.HandleLimitRequest( publisher, *message.u.msg_limit_creation_request ); break;
    case T_MsgsClientToMessenger_msg_limit_destruction_request:
        model_.tacticalLines_.HandleLimitRequest( publisher, message.u.msg_limit_destruction_request ); break;
    case T_MsgsClientToMessenger_msg_limit_update_request:
        model_.tacticalLines_.HandleLimitRequest( publisher, *message.u.msg_limit_update_request ); break;
    // Lima
    case T_MsgsClientToMessenger_msg_lima_creation_request:
        model_.tacticalLines_.HandleLimaRequest( publisher, *message.u.msg_lima_creation_request ); break;
    case T_MsgsClientToMessenger_msg_lima_destruction_request:
        model_.tacticalLines_.HandleLimaRequest( publisher, message.u.msg_lima_destruction_request ); break;
    case T_MsgsClientToMessenger_msg_lima_update_request:
        model_.tacticalLines_.HandleLimaRequest( publisher, *message.u.msg_lima_update_request ); break;
    // Intelligence
    case T_MsgsClientToMessenger_msg_intelligence_creation_request:
        model_.intelligences_.HandleRequest( publisher, *message.u.msg_intelligence_creation_request ); break;
    case T_MsgsClientToMessenger_msg_intelligence_update_request:
        model_.intelligences_.HandleRequest( publisher, *message.u.msg_intelligence_update_request ); break;
    case T_MsgsClientToMessenger_msg_intelligence_destruction_request:
        model_.intelligences_.HandleRequest( publisher, *message.u.msg_intelligence_destruction_request ); break;
    // Drawings
    case T_MsgsClientToMessenger_msg_shape_creation_request:
        model_.drawings_.HandleRequest( publisher, *message.u.msg_shape_creation_request ); break;
    case T_MsgsClientToMessenger_msg_shape_update_request:
        model_.drawings_.HandleRequest( publisher, *message.u.msg_shape_update_request ); break;
    case T_MsgsClientToMessenger_msg_shape_destruction_request:
        model_.drawings_.HandleRequest( publisher, *message.u.msg_shape_destruction_request ); break;
    // Chat
    case T_MsgsClientToMessenger_msg_text_message:
        chat_->OnReceive( *message.u.msg_text_message ); break;
    default:
        break;
    }
}

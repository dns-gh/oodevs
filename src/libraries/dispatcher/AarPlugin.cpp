// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "AarPlugin.h"
#include "network_def.h"
#include "MessageLoader.h"
#include "LinkResolver_ABC.h"
#include "3a/FunctionFactory.h"
#include "3a/Task.h"
#include "tools/MessageDispatcher_ABC.h"
#include <xeumeuleu/xml.h>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AarPlugin constructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AarPlugin::AarPlugin( tools::MessageDispatcher_ABC& dispatcher, LinkResolver_ABC& resolver, const Config& config, const std::string& records )
    : resolver_( resolver )
    , factory_ ( new FunctionFactory() )
    , messages_( new MessageLoader( config, records, true ) )
{
    dispatcher.RegisterMessage( *this, &AarPlugin::OnReceive );
}

// -----------------------------------------------------------------------------
// Name: AarPlugin destructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AarPlugin::~AarPlugin()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AarPlugin::Receive
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AarPlugin::Receive( const ASN1T_MsgsSimToClient& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AarPlugin::NotifyClientAuthenticated
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AarPlugin::NotifyClientAuthenticated( ClientPublisher_ABC& client, Profile_ABC& )
{
    xml::xostringstream xos;
    factory_->Describe( xos );
    AsnMsgAarToClientAarInformation info;
    const std::string description = xos.str();
    info().information = description.c_str();
    info.Send( client );
}

// -----------------------------------------------------------------------------
// Name: AarPlugin::NotifyClientLeft
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AarPlugin::NotifyClientLeft( ClientPublisher_ABC& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AarPlugin::OnReceive
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AarPlugin::OnReceive( const std::string& client, const ASN1T_MsgsClientToAar& asnMsg )
{
    switch( asnMsg.msg.t )
    {
    case T_MsgsClientToAar_msg_msg_indicator_request:
        OnReceiveIndicatorRequest( client, *asnMsg.msg.u.msg_indicator_request );
        break;
    };
}

// -----------------------------------------------------------------------------
// Name: AarPlugin::OnReceiveIndicatorRequest
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AarPlugin::OnReceiveIndicatorRequest( const std::string& client, const ASN1T_MsgIndicatorRequest& request )
{
    try
    {
        xml::xistringstream xis( request.request );
        boost::shared_ptr< Task > task( factory_->CreateTask( request.identifier, xis ) );
        task->Process( *messages_, resolver_.GetPublisher( client ) ); // $$$$ AGE 2007-09-17: deconnexion en route=>crash
    } 
    catch( std::exception& e )
    {
        ASN1T_MsgIndicatorResult result;
        result.values.n = 0; result.values.elem = 0;
        result.identifier = request.identifier;
        result.error = e.what();
        ASN1T_MsgsAarToClient message;
        message.msg.t = T_MsgsAarToClient_msg_msg_indicator_result;
        message.msg.u.msg_indicator_result = &result;

        resolver_.GetPublisher( client ).Send( message );
    }
}

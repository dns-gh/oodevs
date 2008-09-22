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
#include "MessageLoader.h"
#include "LinkResolver_ABC.h"
#include "3a/AarFacade.h"
#include "3a/Task.h"
#include "tools/MessageDispatcher_ABC.h"
#include "game_asn/AarSenders.h"
#include "ClientPublisher_ABC.h"
#include "Services.h"
#include <xeumeuleu/xml.h>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AarPlugin constructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AarPlugin::AarPlugin( tools::MessageDispatcher_ABC& dispatcher, LinkResolver_ABC& resolver, const Config& config )
    : resolver_( resolver )
    , messages_( new MessageLoader( config, true ) )
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
// Name: AarPlugin::Register
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
void AarPlugin::Register( dispatcher::Services& services )
{
    services.Declare< aar::Service >();
}

// -----------------------------------------------------------------------------
// Name: AarPlugin::NotifyClientAuthenticated
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AarPlugin::NotifyClientAuthenticated( ClientPublisher_ABC& client, Profile_ABC& )
{
    aar::AarInformation info;
    const std::string description = "<functions/>"; // $$$$ AGE 2008-08-04: 
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
    case T_MsgsClientToAar_msg_msg_plot_request:
        OnReceiveIndicatorRequest( client, *asnMsg.msg.u.msg_plot_request );
        break;
    };
}

// -----------------------------------------------------------------------------
// Name: AarPlugin::OnReceiveIndicatorRequest
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AarPlugin::OnReceiveIndicatorRequest( const std::string& client, const ASN1T_MsgPlotRequest& request )
{
    messages_->ReloadIndices();
    try
    {
        xml::xistringstream xis( request.request );
        AarFacade factory( resolver_.GetPublisher( client ), request.identifier );
        boost::shared_ptr< Task > task( factory.CreateTask( xis ) );
        task->Process( *messages_ ); // $$$$ AGE 2007-09-17: deconnexion en route=>crash
    } 
    catch( std::exception& e )
    {
        aar::PlotResult message;
        message().values.n = 0; message().values.elem = 0;
        message().identifier = request.identifier;
        message().error = e.what();
        message.Send( resolver_.GetPublisher( client ) );
    }
}

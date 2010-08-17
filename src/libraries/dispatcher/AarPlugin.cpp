// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "AarPlugin.h"
#include "MessageLoader.h"
#include "LinkResolver_ABC.h"
#include "Services.h"
#include "3a/AarFacade.h"
#include "3a/Task.h"
#include "tools/MessageDispatcher_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/aarsenders.h"
#include <boost/algorithm/string.hpp>
#include <xeumeuleu/xml.hpp>

using namespace dispatcher;

namespace MsgsClientToAar
{
    class MsgClientToAar;
    class MsgPlotRequest;
}

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
void AarPlugin::Receive( const MsgsSimToClient::MsgSimToClient& )
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
    info().set_information( description.c_str() );
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
void AarPlugin::OnReceive( const std::string& client, const MsgsClientToAar::MsgClientToAar& wrapper )
{
    if( wrapper.message().has_plot_request() )
        OnReceiveIndicatorRequest( client, wrapper.message().plot_request() );
}

// -----------------------------------------------------------------------------
// Name: AarPlugin::OnReceiveIndicatorRequest
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AarPlugin::OnReceiveIndicatorRequest( const std::string& client, const MsgsClientToAar::MsgPlotRequest& request )
{
    messages_->ReloadIndices();
    try
    {
        if( boost::starts_with( request.request(), "indicator://" ) )
            return;
        xml::xistringstream xis( request.request() );
        AarFacade factory( resolver_.GetPublisher( client ), request.identifier() );
        xis >> xml::start( "indicator" );
        boost::shared_ptr< Task > task( factory.CreateTask( xis ) );
        xis >> xml::end;
        task->Process( *messages_ ); // $$$$ AGE 2007-09-17: deconnexion en route=>crash
    }
    catch( std::exception& e )
    {
        aar::PlotResult message;
        message().mutable_values();
        message().set_identifier ( request.identifier() );
        message().set_error( e.what() );
        message.Send( resolver_.GetPublisher( client ) );
    }
}

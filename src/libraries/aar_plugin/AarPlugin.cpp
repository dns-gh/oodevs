// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "AarPlugin.h"
#include "3a/AarFacade.h"
#include "3a/Task.h"
#include "3a/StaticModel.h"
#include "dispatcher/Config.h"
#include "dispatcher/MessageLoader.h"
#include "dispatcher/LinkResolver_ABC.h"
#include "dispatcher/Services.h"
#include "tools/MessageDispatcher_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/AarSenders.h"
#include <xeumeuleu/xml.hpp>
#include <boost/algorithm/string.hpp>

using namespace plugins::aar;

namespace sword
{
    class ClientToAar;
    class PlotRequest;
}

// -----------------------------------------------------------------------------
// Name: AarPlugin constructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AarPlugin::AarPlugin( tools::MessageDispatcher_ABC& dispatcher, dispatcher::LinkResolver_ABC& resolver, const dispatcher::Config& config )
    : resolver_( resolver )
    , messages_( new dispatcher::MessageLoader( config, true ) )
    , model_   ( new ::aar::StaticModel( config ) )
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
void AarPlugin::Receive( const sword::SimToClient& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AarPlugin::Register
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
void AarPlugin::Register( dispatcher::Services& services )
{
    services.Declare< ::aar::Service >();
}

// -----------------------------------------------------------------------------
// Name: AarPlugin::NotifyClientAuthenticated
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AarPlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, const std::string&, dispatcher::Profile_ABC& )
{
    ::aar::AarInformation info;
    const std::string description = "<functions/>"; // $$$$ AGE 2008-08-04:
    info().set_information( description.c_str() );
    info.Send( client );
}

// -----------------------------------------------------------------------------
// Name: AarPlugin::OnReceive
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AarPlugin::OnReceive( const std::string& client, const sword::ClientToAar& wrapper )
{
    if( wrapper.message().has_plot_request() )
        OnReceiveIndicatorRequest( client, wrapper.message().plot_request() );
}

// -----------------------------------------------------------------------------
// Name: AarPlugin::OnReceiveIndicatorRequest
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AarPlugin::OnReceiveIndicatorRequest( const std::string& client, const sword::PlotRequest& request )
{
    try
    {
        if( boost::starts_with( request.request(), "indicator://" ) )
            return;
        unsigned int firstTick = 0;
        unsigned int lastTick = std::numeric_limits< unsigned int >::max();
        if( request.has_time_range() )
        {
            firstTick = request.time_range().begin_tick();
            lastTick = request.time_range().end_tick();
        }
        xml::xistringstream xis( request.request() );
        AarFacade factory( resolver_.GetPublisher( client ), request.identifier(), *model_ );
        xis >> xml::start( "indicator" );
        boost::shared_ptr< Task > task( factory.CreateTask( xis, firstTick, lastTick ) );
        xis >> xml::end;
        task->Process( *messages_ ); // $$$$ AGE 2007-09-17: deconnexion en route=>crash
    }
    catch( std::exception& e )
    {
        ::aar::PlotResult message;
        message().mutable_values();
        message().set_identifier ( request.identifier() );
        message().set_error( e.what() );
        message.Send( resolver_.GetPublisher( client ) );
    }
}

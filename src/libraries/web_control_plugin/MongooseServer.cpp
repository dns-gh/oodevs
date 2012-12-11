// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "MongooseServer.h"
#include "WebObserver_ABC.h"
#include <tools/Exception.h>

#include <boost/assign/list_of.hpp>
#include <boost/lexical_cast.hpp>

#include <stdexcept>
#include <vector>

#include <mongoose/mongoose.h>

#ifdef _MSC_VER
#   pragma warning( disable : 4355 )
#endif

using namespace plugins::web_control;

namespace
{
    void* WebCallback( mg_event event, mg_connection* link, const mg_request_info* request )
    {
        if( event != MG_NEW_REQUEST )
            return 0;
        MongooseServer* server = reinterpret_cast< MongooseServer* >( request->user_data );
        server->Notify( link, request );
        return link;
    }

    boost::shared_ptr< mg_context > Start( MongooseServer& server, const std::string& port )
    {
        std::vector< const char* > options = boost::assign::list_of
            ( "enable_directory_listing" )( "false" )
            ( "listening_ports" )( port.c_str() )( 0 );
        mg_context* ptr = mg_start( &WebCallback, &server, &options[0] );
        if( !ptr )
            throw MASA_EXCEPTION( "unable to start mongoose server" );
        return boost::shared_ptr< mg_context >( ptr, &mg_stop );
    }
}

// -----------------------------------------------------------------------------
// Name: MongooseServer::MongooseServer
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
MongooseServer::MongooseServer( WebObserver_ABC& observer, int port )
    : observer_( observer )
    , context_( Start( *this, boost::lexical_cast< std::string >( port ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MongooseServer::~MongooseServer
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
MongooseServer::~MongooseServer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MongooseServer::Notify
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
void MongooseServer::Notify( mg_connection* link, const mg_request_info* request )
{
    const std::string reply = observer_.Notify( request->request_method, request->uri );
    mg_write( link, reply.data(), reply.size() );
}

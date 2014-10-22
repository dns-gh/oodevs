// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "timeline/api.h"
#include "moc_api.cpp"

#include "Client.h"
#include "Server.h"
#include "ServerApp.h"

#include <tools/Exception.h>

#include <boost/interprocess/detail/atomic.hpp>

using namespace timeline;
namespace bii = boost::interprocess::ipcdetail;

struct timeline::Library
{
    explicit Library( const LibraryConfiguration& cfg )
        : app_( new ServerApp( cfg.debug_port, cfg.client_log, cfg.cef_log ) )
    {
        // NOTHING
    }

    ~Library()
    {
        CefShutdown();
    }

    CefRefPtr< ServerApp > app_;
};

namespace
{
    uint32_t once = 0;
}

std::shared_ptr< Library > timeline::Initialize( const LibraryConfiguration& cfg )
{
    if( bii::atomic_inc32( &once ) )
        throw MASA_EXCEPTION( "unable to call timeline::Initialize twice" );
    return std::make_shared< Library >( cfg );
}

std::auto_ptr< Server_ABC > timeline::MakeServer( const Configuration& cfg )
{
    return std::auto_ptr< Server_ABC >( new Server( cfg ) );
}

bool timeline::RunClient( int argc, const char* argv[] )
{
    Client client( argc, argv );
    return client.Start();
}

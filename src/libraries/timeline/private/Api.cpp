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

using namespace timeline;

std::auto_ptr< Server_ABC > timeline::MakeServer( const Configuration& cfg )
{
    return std::auto_ptr< Server_ABC >( new Server( cfg ) );
}

bool timeline::RunClient( int argc, const char* argv[] )
{
    Client client( argc, argv );
    return client.Start();
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "timeline/api.h"

#include "Server.h"

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include <cef_app.h>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

using namespace timeline;

bool timeline::SpawnServer()
{
    return CefExecuteProcess( CefMainArgs( GetModuleHandle( 0 ) ), 0 ) >= 0;
}

std::auto_ptr< Server_ABC > timeline::MakeServer( const Configuration& cfg )
{
    return std::auto_ptr< Server_ABC >( new Server( cfg ) );
}
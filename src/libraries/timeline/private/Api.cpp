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

#ifndef _WIN64
#define USE_EMBEDDED_CORE
#endif

#ifdef USE_EMBEDDED_CORE
#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include <cef_app.h>
#ifdef _MSC_VER
#pragma warning( pop )
#endif
#endif

using namespace timeline;

bool timeline::SpawnServer()
{
#ifdef USE_EMBEDDED_CORE
    return CefExecuteProcess( CefMainArgs( GetModuleHandle( 0 ) ), 0 ) >= 0;
#else
    return false;
#endif
}

std::auto_ptr< Server_ABC > timeline::MakeServer( const Configuration& cfg )
{
    return std::auto_ptr< Server_ABC >( new Server( cfg ) );
}
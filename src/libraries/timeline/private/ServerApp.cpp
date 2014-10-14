// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "ServerApp.h"

#include <tools/Path.h>

#include <boost/lexical_cast.hpp>

using namespace timeline;

#ifdef _DEBUG
#define SUFFIX "_d"
#else
#define SUFFIX
#endif

ServerApp::ServerApp( int debug_port,
                      const tools::Path& client_log,
                      const tools::Path& cef_log )
    : client_log_( client_log )
{
    CefSettings settings;
    settings.multi_threaded_message_loop = true;
    settings.single_process = false;
    settings.remote_debugging_port = debug_port;
    settings.remote_debugging_port = 60000;
    settings.log_severity = LOGSEVERITY_DISABLE;
    CefString( &settings.browser_subprocess_path ).FromString( "timeline_client" SUFFIX ".exe" );
    if( !cef_log.IsEmpty() )
    {
        settings.log_severity = LOGSEVERITY_VERBOSE;
        CefString( &settings.log_file ).FromWString( cef_log.ToUnicode() );
    }
    const bool valid = CefInitialize( CefMainArgs( GetModuleHandle( nullptr ) ), settings, this, nullptr );
    if( !valid )
        throw std::runtime_error( "unable to initialize cef" );
}

ServerApp::~ServerApp()
{
    // NOTHING
}

namespace
{
    CefString FromString( const std::string& value )
    {
        CefString reply;
        reply.FromString( value );
        return reply;
    }
}

void ServerApp::OnBeforeCommandLineProcessing( const CefString& /*process_type*/,
                                               CefRefPtr< CefCommandLine > command_line )
{
    // see http://www.magpcss.org/ceforum/viewtopic.php?f=14&t=10760
    // unfortunately winhttp-proxy-resolver does not work
    if( !getenv( "TIMELINE_ENABLE_PROXY" ) )
        command_line->AppendSwitch( "no-proxy-server" );
}

CefRefPtr< CefBrowserProcessHandler > ServerApp::GetBrowserProcessHandler()
{
    return this;
}

void ServerApp::OnBeforeChildProcessLaunch( CefRefPtr< CefCommandLine > command_line )
{
    if( !client_log_.IsEmpty() )
        command_line->AppendSwitchWithValue( "sw-client-log", FromString( client_log_.ToUTF8() ) );
}

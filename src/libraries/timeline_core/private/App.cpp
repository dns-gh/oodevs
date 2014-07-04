// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "App.h"

#include "Engine.h"
#include "timeline_core/api.h"
#include <tools/Path.h>

#include <stdlib.h>

using namespace timeline::core;

App::App( const Configuration& cfg, CefRefPtr< Engine > engine )
    : engine_( engine )
{
    CefSettings settings;
    settings.multi_threaded_message_loop = true;
    /// multi_process is too complex for our needs, as it creates yet another renderer process
    /// separate from the browser context
    settings.single_process = true;
    settings.remote_debugging_port = cfg.debug_port;
    settings.log_severity = LOGSEVERITY_DISABLE;
    if( !cfg.cef_log.empty() )
    {
        settings.log_severity = LOGSEVERITY_VERBOSE;
        CefString( &settings.log_file ).FromWString( 
                tools::Path::FromUTF8( cfg.cef_log ).ToUnicode()  );
    }
    const bool valid = CefInitialize( CefMainArgs( GetModuleHandle( 0 ) ), settings, this );
    if( !valid )
        throw std::runtime_error( "unable to initialize cef" );
}

App::~App()
{
    // NOTHING
}

void App::OnBeforeCommandLineProcessing( const CefString& /*process_type*/,
                                         CefRefPtr< CefCommandLine > command_line )
{
    // see http://www.magpcss.org/ceforum/viewtopic.php?f=14&t=10760
    // unfortunately winhttp-proxy-resolver does not work
    if( !getenv( "TIMELINE_ENABLE_PROXY" ) )
        command_line->AppendSwitch( "no-proxy-server" );
}

CefRefPtr< CefRenderProcessHandler > App::GetRenderProcessHandler()
{
    return this;
}

void App::OnContextCreated( CefRefPtr< CefBrowser >,
                            CefRefPtr< CefFrame >,
                            CefRefPtr< CefV8Context > context )
{
    AutoLock lock( this );
    engine_->Register( context );
}

void App::OnContextReleased( CefRefPtr< CefBrowser >,
                             CefRefPtr< CefFrame >,
                             CefRefPtr< CefV8Context > )
{
    AutoLock lock( this );
    engine_->Unregister();
}

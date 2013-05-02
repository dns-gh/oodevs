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

using namespace timeline::core;

App::App( const Configuration& cfg )
    : engine_( new Engine() )
{
    CefSettings settings;
    settings.multi_threaded_message_loop = true;
    settings.single_process = cfg.single_process;
    settings.remote_debugging_port = cfg.debug_port;
    const bool valid = CefInitialize( CefMainArgs( GetModuleHandle( 0 ) ), settings, this );
    if( !valid )
        throw std::runtime_error( "unable to initialize cef" );
}

App::~App()
{
    // NOTHING
}

CefRefPtr< CefRenderProcessHandler > App::GetRenderProcessHandler()
{
    return this;
}

void App::OnContextCreated( CefRefPtr< CefBrowser >,
                            CefRefPtr< CefFrame >,
                            CefRefPtr< CefV8Context > context )
{
    engine_->Register( context );
}
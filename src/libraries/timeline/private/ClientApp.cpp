// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "ClientApp.h"

#include "Engine.h"
#include "controls/controls.h"

using namespace timeline;

ClientApp::ClientApp( controls::ClientHandler_ABC& handler, CefRefPtr< Engine > engine )
    : handler_( handler )
    , engine_ ( engine )
{
    // NOTHING
}

ClientApp::~ClientApp()
{
    // NOTHING
}

CefRefPtr< CefRenderProcessHandler > ClientApp::GetRenderProcessHandler()
{
    return this;
}

void ClientApp::OnContextCreated( CefRefPtr< CefBrowser > browser,
                                  CefRefPtr< CefFrame >,
                                  CefRefPtr< CefV8Context > context )
{
    AutoLock lock( this );
    engine_->Register( browser, context );
}

void ClientApp::OnContextReleased( CefRefPtr< CefBrowser >,
                                   CefRefPtr< CefFrame >,
                                   CefRefPtr< CefV8Context > )
{
    AutoLock lock( this );
    engine_->Unregister();
}

bool ClientApp::OnProcessMessageReceived( CefRefPtr< CefBrowser >,
                                          CefProcessId,
                                          CefRefPtr< CefProcessMessage > message )
{
    if( message->GetName() != controls::GetServerToClientMessage() )
        return false;
    ParseClient( handler_, message, controls::T_Logger() );
    return true;
}

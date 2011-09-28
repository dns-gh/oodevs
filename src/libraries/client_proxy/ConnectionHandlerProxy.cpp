// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "client_proxy_pch.h"
#include "ConnectionHandlerProxy.h"

// -----------------------------------------------------------------------------
// Name: ConnectionHandlerProxy constructor
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
ConnectionHandlerProxy::ConnectionHandlerProxy()
    : handler_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConnectionHandlerProxy destructor
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
ConnectionHandlerProxy::~ConnectionHandlerProxy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConnectionHandlerProxy::SetHandler
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void ConnectionHandlerProxy::SetHandler( SwordConnectionHandler_ABC* handler )
{
    handler_ = handler;
}

// -----------------------------------------------------------------------------
// Name: ConnectionHandlerProxy::OnConnectionSucceeded
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void ConnectionHandlerProxy::OnConnectionSucceeded( const std::string& endpoint )
{
    if( handler_ )
        handler_->OnConnectionSucceeded( endpoint );
}

// -----------------------------------------------------------------------------
// Name: ConnectionHandlerProxy::OnConnectionFailed
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void ConnectionHandlerProxy::OnConnectionFailed( const std::string& endpoint, const std::string& reason )
{
    if( handler_ )
        handler_->OnConnectionFailed( endpoint, reason );
}

// -----------------------------------------------------------------------------
// Name: ConnectionHandlerProxy::OnConnectionError
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
void ConnectionHandlerProxy::OnConnectionError( const std::string& endpoint, const std::string& reason )
{
    if( handler_ )
        handler_->OnConnectionError( endpoint, reason );
}

// -----------------------------------------------------------------------------
// Name: ConnectionHandlerProxy::OnConnectionWarning
// Created: MCO 2011-09-28
// -----------------------------------------------------------------------------
void ConnectionHandlerProxy::OnConnectionWarning( const std::string& endpoint, const std::string& reason )
{
    if( handler_ )
        handler_->OnConnectionWarning( endpoint, reason );
}

// -----------------------------------------------------------------------------
// Name: ConnectionHandlerProxy::OnAuthenticationSucceeded
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void ConnectionHandlerProxy::OnAuthenticationSucceeded( const std::string& profile )
{
    if( handler_ )
        handler_->OnAuthenticationSucceeded( profile );
}

// -----------------------------------------------------------------------------
// Name: ConnectionHandlerProxy::OnAuthenticationFailed
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void ConnectionHandlerProxy::OnAuthenticationFailed( const std::string& profile, const std::string& reason )
{
    if( handler_ )
        handler_->OnAuthenticationFailed( profile, reason );
}

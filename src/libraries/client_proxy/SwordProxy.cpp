// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "client_proxy_pch.h"
#include "SwordProxy.h"
#include "SwordClient.h"

class SwordProxy::SwordProxyImplementation
{
public:
    SwordProxyImplementation( const std::string& host, unsigned short port, const std::string& profile, const std::string& password )
        : client_( new SwordClient( host, port, profile, password, 0 ) ) {}
    ~SwordProxyImplementation() {}

    void Connect( SwordConnectionHandler_ABC* handler ) { client_->Connect( handler ); }
    void Disconnect() { client_->Disconnect(); }
    void Update() { client_->Update(); }
    void RegisterMessageHandler( SwordMessageHandler_ABC& handler ) { client_->RegisterMessageHandler( handler ); }
    void UnregisterMessageHandler( SwordMessageHandler_ABC& handler ) { client_->UnregisterMessageHandler( handler ); }
    void Send( const sword::ClientToSim& message ) const { client_->SendMessage( message ); }
    void Send( const sword::ClientToAuthentication& message ) const { client_->SendMessage( message ); }
private:
    std::auto_ptr< SwordClient > client_;
};

// -----------------------------------------------------------------------------
// Name: SwordProxy constructor
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
SwordProxy::SwordProxy( const std::string& host, unsigned short port, const std::string& profile, const std::string& password )
    : pimpl_( new SwordProxyImplementation( host, port, profile, password ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SwordProxy destructor
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
SwordProxy::~SwordProxy()
{
    delete pimpl_;
}

// -----------------------------------------------------------------------------
// Name: SwordProxy::Connect
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordProxy::Connect( SwordConnectionHandler_ABC* handler )
{
    if( pimpl_ )
        pimpl_->Connect( handler );
}

// -----------------------------------------------------------------------------
// Name: SwordProxy::Disconnect
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordProxy::Disconnect()
{
    if( pimpl_ )
        pimpl_->Disconnect();
}

// -----------------------------------------------------------------------------
// Name: SwordProxy::Update
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordProxy::Update()
{
    if( pimpl_ )
        pimpl_->Update();
}

// -----------------------------------------------------------------------------
// Name: SwordProxy::RegisterMessageHandler
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordProxy::RegisterMessageHandler( SwordMessageHandler_ABC* handler )
{
    if( pimpl_ && handler )
        pimpl_->RegisterMessageHandler( *handler );
}

// -----------------------------------------------------------------------------
// Name: SwordProxy::UnregisterMessageHandler
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordProxy::UnregisterMessageHandler( SwordMessageHandler_ABC* handler )
{
    if( pimpl_ && handler )
        pimpl_->UnregisterMessageHandler( *handler );
}

// -----------------------------------------------------------------------------
// Name: SwordProxy::Send
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
void SwordProxy::Send( const sword::ClientToSim& message ) const
{
    if( pimpl_ )
        pimpl_->Send( message );
}

// -----------------------------------------------------------------------------
// Name: SwordProxy::Send
// Created: AHC 2010-05-23
// -----------------------------------------------------------------------------
void SwordProxy::Send( const sword::ClientToAuthentication& message ) const
{
    if( pimpl_ )
        pimpl_->Send( message );
}

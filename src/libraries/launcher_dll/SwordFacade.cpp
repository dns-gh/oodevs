// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "launcher_dll_pch.h"
#include "SwordFacade.h"
#include "client_proxy/SwordProxy.h"
#include "frontend/ProcessWrapper.h"
#include "frontend/CommandLineTools.h"

namespace launcher
{

// -----------------------------------------------------------------------------
// Name: SwordFacade constructor
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
SwordFacade::SwordFacade( frontend::ProcessObserver_ABC& observer, boost::shared_ptr< frontend::SpawnCommand > command, bool isDispatcher )
    : isDispatcher_( isDispatcher )
    , isConnected_( false )
    , isAuthenticated_ ( false)
    , process_( new frontend::ProcessWrapper( observer, command) )
{
}

// -----------------------------------------------------------------------------
// Name: SwordFacade constructor
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
SwordFacade::~SwordFacade()
{
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::Start
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
void SwordFacade::Start()
{
    if( process_.get() )
    {
        process_->Start();
        if(isDispatcher_)
        {
            client_.reset( new SwordProxy("localhost", frontend::DispatcherPort(1), "Supervisor", "" ));
            client_->Connect(this);
        }
    }
}
// -----------------------------------------------------------------------------
// Name: SwordFacade::Stop
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
void SwordFacade::Stop()
{
    if( process_.get() )
    {
        if( IsConnected() && client_.get() )
        {
            client_->Disconnect();
            client_.reset();
        }
        process_->Stop();
    }
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::OnConnectionSucceeded
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
void SwordFacade::OnConnectionSucceeded( const std::string& endpoint )
{
    isConnected_ = true;
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::OnConnectionFailed
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
void SwordFacade::OnConnectionFailed( const std::string& endpoint, const std::string& reason )
{
    isConnected_ = false;
}
// -----------------------------------------------------------------------------
// Name: SwordFacade::OnConnectionError
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
void SwordFacade::OnConnectionError( const std::string& endpoint, const std::string& reason )
{
    isConnected_ = false;
    isAuthenticated_ = false;
}
// -----------------------------------------------------------------------------
// Name: SwordFacade::OnAuthenticationSucceeded
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
void SwordFacade::OnAuthenticationSucceeded( const std::string& profile )
{
    isAuthenticated_ = true;
}
// -----------------------------------------------------------------------------
// Name: SwordFacade::OnAuthenticationFailed
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
void SwordFacade::OnAuthenticationFailed( const std::string& profile, const std::string& reason )
{
    isAuthenticated_ = false;
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::RegisterMessageHandler
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
void SwordFacade::RegisterMessageHandler( SwordMessageHandler_ABC* handler )
{
    if( client_.get() )
        client_->RegisterMessageHandler( handler );
}
// -----------------------------------------------------------------------------
// Name: SwordFacade::UnregisterMessageHandler
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
void SwordFacade::UnregisterMessageHandler( SwordMessageHandler_ABC* handler )
{
    if( client_.get())
        client_->RegisterMessageHandler( handler );
}
// -----------------------------------------------------------------------------
// Name: SwordFacade::Send
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
void SwordFacade::Send( const sword::ClientToSim& message ) const
{
    if( client_.get() && IsConnected() )
        client_->Send( message );
}
}

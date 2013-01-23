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
#include "Config.h"
#include "client_proxy/SwordProxy.h"
#include "frontend/ProcessWrapper.h"
#include "protocol/ClientSenders.h"
#include "protocol/MessengerSenders.h"
#include "protocol/AuthenticationSenders.h"
#include "protocol/DispatcherSenders.h"
#include <boost/foreach.hpp>

using namespace launcher;

// -----------------------------------------------------------------------------
// Name: SwordFacade constructor
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
SwordFacade::SwordFacade( bool isDispatcher )
    : isDispatcher_   ( isDispatcher )
    , isConnected_    ( false )
    , isAuthenticated_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SwordFacade constructor
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
SwordFacade::~SwordFacade()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::Start
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
void SwordFacade::Start( frontend::ProcessObserver_ABC& observer, boost::shared_ptr< frontend::SpawnCommand > command,
                        const std::string& supervisorProfile, const std::string& supervisorPassword, const launcher::Config& config )
{
    if( ! config.GetTestMode() )
    {
        boost::shared_ptr< frontend::ProcessWrapper > wrapper( new frontend::ProcessWrapper( observer, command ) );
        wrapper->Start();
        process_ = wrapper;
    }
    if( isDispatcher_ )
    {
        client_.reset( new SwordProxy( "localhost", config.GetDispatcherPort(), supervisorProfile, supervisorPassword ) );
        client_->RegisterMessageHandler( this );
        client_->Connect( this );
    }
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::Stop
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
void SwordFacade::Stop()
{
    boost::shared_ptr< frontend::ProcessWrapper > process( process_ );
    if( ! process )
        return;
    if( IsConnected() && client_ )
    {
        client_->Disconnect();
        client_->UnregisterMessageHandler( this );
        client_.reset();
    }
    process->Stop();
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::OnConnectionSucceeded
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
void SwordFacade::OnConnectionSucceeded( const std::string& /*endpoint*/ )
{
    isConnected_ = true;
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::OnConnectionFailed
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
void SwordFacade::OnConnectionFailed( const std::string& /*endpoint*/, const std::string& /*reason*/ )
{
    isConnected_ = false;
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::OnConnectionError
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
void SwordFacade::OnConnectionError( const std::string& /*endpoint*/, const std::string& /*reason*/ )
{
    isConnected_ = false;
    isAuthenticated_ = false;
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::OnConnectionWarning
// Created: MCO 2011-09-28
// -----------------------------------------------------------------------------
void SwordFacade::OnConnectionWarning( const std::string& /*endpoint*/, const std::string& /*reason*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::OnAuthenticationSucceeded
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
void SwordFacade::OnAuthenticationSucceeded( const std::string& /*profile*/ )
{
    isAuthenticated_ = true;
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::OnAuthenticationFailed
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
void SwordFacade::OnAuthenticationFailed( const std::string& /*profile*/, const std::string& /*reason*/ )
{
    isAuthenticated_ = false;
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::RegisterMessageHandler
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
void SwordFacade::RegisterMessageHandler( int context, std::auto_ptr< MessageHandler_ABC > handler )
{
    messageHandlers_[ context ] = handler;
}
// -----------------------------------------------------------------------------
// Name: SwordFacade::AddPermanentMessageHandler
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
void SwordFacade::AddPermanentMessageHandler( std::auto_ptr< MessageHandler_ABC > handler )
{
    permanentHandler_.push_back( T_Handler( handler.release() ) );
}
// -----------------------------------------------------------------------------
// Name: SwordFacade::Send
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
void SwordFacade::Send( const sword::ClientToSim& message ) const
{
    if( client_ && IsConnected() )
        client_->Send( message );
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::Send
// Created: AHC 2011-05-23
// -----------------------------------------------------------------------------
void SwordFacade::Send( const sword::ClientToAuthentication& message ) const
{
    if( client_ && IsConnected() )
        client_->Send( message );
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::OnReceiveMessage
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
void SwordFacade::OnReceiveMessage( const sword::SimToClient& message )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::OnReceiveMessage
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
void SwordFacade::OnReceiveMessage( const sword::MessengerToClient& message )
{
    Update( message );
}
// -----------------------------------------------------------------------------
// Name: SwordFacade::OnReceiveMessage
// Created: LGY 2011-05-19
// -----------------------------------------------------------------------------
void SwordFacade::OnReceiveMessage( const sword::AuthenticationToClient& message )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::OnReceiveMessage
// Created: JSR 2011-10-20
// -----------------------------------------------------------------------------
void SwordFacade::OnReceiveMessage( const sword::DispatcherToClient& message )
{
    BOOST_FOREACH( T_Handler handler, permanentHandler_ )
        handler->OnReceiveMessage( message );
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::Update
// Created: LGY 2011-05-19
// -----------------------------------------------------------------------------
template< typename T >
void SwordFacade::Update( const T& message )
{
    BOOST_FOREACH( T_Handler handler, permanentHandler_ )
        handler->OnReceiveMessage( message );
    HandlerContainer::iterator it = messageHandlers_.find( message.context() );
    if( messageHandlers_.end() != it )
    {
        bool handled = it->second->OnReceiveMessage( message );
        if( handled )
            messageHandlers_.erase( message.context() );
    }
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::IsRunning
// Created: AHC 2011-05-19
// -----------------------------------------------------------------------------
bool SwordFacade::IsRunning() const
{
    return !process_.expired();
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::Update
// Created: AHC 2011-05-19
// -----------------------------------------------------------------------------
void SwordFacade::Update() const
{
    if( client_ )
        client_->Update();
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::GetProcess
// Created: RPD 2011-09-12
// -----------------------------------------------------------------------------
boost::shared_ptr< frontend::ProcessWrapper > SwordFacade::GetProcess()
{
    return process_.lock();
}

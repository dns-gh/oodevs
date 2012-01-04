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
#include "LauncherPublisher.h"
#include "LauncherService.h"
#include "client_proxy/SwordProxy.h"
#include "frontend/ProcessWrapper.h"
#include "protocol/ClientSenders.h"
#include "protocol/MessengerSenders.h"
#include "protocol/AuthenticationSenders.h"
#include "protocol/DispatcherSenders.h"
#include "protocol/LauncherSenders.h"
#include <boost/foreach.hpp>

using namespace launcher;

// -----------------------------------------------------------------------------
// Name: SwordFacade constructor
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
SwordFacade::SwordFacade( const LauncherService& server, std::string endpoint, bool isDispatcher )
    : isDispatcher_   ( isDispatcher )
    , isConnected_    ( false )
    , isAuthenticated_( false )
    , server_         ( server )
    , msTimeOut_      ( 0 )
    , endpoint_       ( endpoint )
    , checkTime_      ( false )
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
                        const std::string& supervisorProfile, const std::string& supervisorPassword, const launcher::Config& config, bool attach /*= false*/ )
{
    if( ! config.GetTestMode() )
    {
        boost::shared_ptr< frontend::ProcessWrapper > wrapper( new frontend::ProcessWrapper( observer, command ) );
        if( attach )
            wrapper->Attach();
        else
            wrapper->Start();
        process_ = wrapper;
    }
    if( isDispatcher_ )
    {
        msTimeOut_ = config.GetMsTimeOut();
        client_.reset( new SwordProxy( "localhost", config.GetDispatcherPort(), supervisorProfile, supervisorPassword ) );
        client_->RegisterMessageHandler( this );
        client_->Connect( this );
    }
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::Stop
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
void SwordFacade::Stop( bool forceProcessStop /*= true*/ )
{
    if( !process_.expired() )
    {
        if( IsConnected() && client_.get() )
        {
            client_->Disconnect();
            client_->UnregisterMessageHandler( this );
            client_.reset();
        }
        process_.lock()->Stop( forceProcessStop );
    }
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::OnConnectionSucceeded
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
void SwordFacade::OnConnectionSucceeded( const std::string& /*endpoint*/ )
{
    if( isDispatcher_ )
    {
        if( msTimeOut_ > 0 )
        {
            creationTime_ = boost::posix_time::microsec_clock::universal_time();
            checkTime_ = true;
        }
    }
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
void SwordFacade::OnConnectionError( const std::string& /*endpoint*/, const std::string& reason )
{
    isConnected_ = false;
    isAuthenticated_ = false;
    boost::shared_ptr< LauncherPublisher > publisher = server_.ResolveClient( endpoint_ );
    if( publisher.get() )
    {
        SessionStatus statusResponse;
        statusResponse().set_status( sword::SessionStatus::breakdown );
        static const std::string msg( "La connexion avec le dispatcher est perdue. " );
        statusResponse().set_breakdown_information( msg + reason );
        statusResponse().set_exercise( "" );
        statusResponse().set_session( "" );
        statusResponse.Send( *publisher );
    }
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
// Name: SwordFacade::ClearPermanentMessageHandler
// Created: JSR 2011-12-12
// -----------------------------------------------------------------------------
void SwordFacade::ClearPermanentMessageHandler()
{
    permanentHandler_.clear();
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
    if( client_.get() && IsConnected() )
        client_->Send( message );
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::Send
// Created: AHC 2011-05-23
// -----------------------------------------------------------------------------
void SwordFacade::Send( const sword::ClientToAuthentication& message ) const
{
    if( client_.get() && IsConnected() )
        client_->Send( message );
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::Send
// Created: LGY 2011-11-09
// -----------------------------------------------------------------------------
void SwordFacade::Send( const sword::ClientToMessenger& message ) const
{
    if( client_.get() && IsConnected() )
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
    checkTime_ = false;
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
    if ( server_.TestClient( endpoint_ ) )
    {
        BOOST_FOREACH( T_Handler handler, permanentHandler_ )
            handler->OnReceiveMessage( message );
        HandlerContainer::iterator it = messageHandlers_.find( message.context() );
        if( messageHandlers_.end() != it )
        {
            bool handled = it->second->OnReceiveMessage( message );
            if ( handled )
                messageHandlers_.erase( message.context() );
        }
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
    if( checkTime_ )
    {
        if( ( boost::posix_time::microsec_clock::universal_time() - creationTime_ ).total_milliseconds() > msTimeOut_ )
        {
            boost::shared_ptr< LauncherPublisher > publisher = server_.ResolveClient( endpoint_ );
            if( publisher.get() )
            {
                SessionStatus statusResponse;
                statusResponse().set_status( sword::SessionStatus::breakdown );
                static const std::string msg( "Time out : Le dispatcher n'a pas répondu au bout du temps imparti." );
                statusResponse().set_breakdown_information( msg );
                statusResponse().set_exercise( "" );
                statusResponse().set_session( "" );
                statusResponse.Send( *publisher );
                checkTime_ = false;
            }
        }
    }
    if( client_.get() )
        client_->Update();
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::GetProcess
// Created: RPD 2011-09-12
// -----------------------------------------------------------------------------
const frontend::ProcessWrapper* SwordFacade::GetProcess()
{
    return process_.lock().get();
}

// -----------------------------------------------------------------------------
// Name: SwordFacade::SetEndpoint
// Created: JSR 2011-12-13
// -----------------------------------------------------------------------------
void SwordFacade::SetEndpoint( const std::string&  endpoint )
{
    endpoint_ = endpoint;
    if( !process_.expired() )
        process_.lock()->SetEndpoint( endpoint );
}

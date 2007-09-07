// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "ClientNetworker.h"
#include "SocketManager.h"
#include "Connector.h"
#include "BufferedMessageCallback.h"
#include "BufferedConnectionCallback.h"
#include "ObjectMessageService.h"
#include <boost/bind.hpp>
#include "asio.h"
#include "MT/MT_Logger/MT_Logger_lib.h"

using namespace tools;

// -----------------------------------------------------------------------------
// Name: ClientNetworker constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
ClientNetworker::ClientNetworker( const std::string& host /* = "" */, bool retry /*= false*/ )
    : service_         ( new boost::asio::io_service() )
    , connectionBuffer_( new BufferedConnectionCallback() )
    , messageBuffer_   ( new BufferedMessageCallback() )
    , sockets_         ( new SocketManager( *messageBuffer_, *connectionBuffer_ ) )
    , messageService_  ( new ObjectMessageService() )
    , connector_       ( new Connector( *service_, *sockets_, *connectionBuffer_ ) )
    , retry_           ( retry )
    , stopped_         ( false )
    , thread_          ( boost::bind( &ClientNetworker::Run, this ) )
{
    messageService_->RegisterErrorCallback( boost::bind( &ClientNetworker::ConnectionError, this, _1, _2 ) );
    if( !host.empty() )
        Connect( host, retry );
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker destructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
ClientNetworker::~ClientNetworker()
{
    try
    {
        service_->post( boost::bind( &ClientNetworker::Stop, this ) );
        thread_.join();
    }
    catch( ... )
    {
        // NOTHING
    }
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker::Connect
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
void ClientNetworker::Connect( const std::string& host, bool retry )
{
    host_ = host;
    retry_ = retry;
    connector_->Connect( host );
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker::Disconnect
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
void ClientNetworker::Disconnect()
{
    sockets_->Disconnect();
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker::Update
// Created: NLD 2007-01-24
// -----------------------------------------------------------------------------
void ClientNetworker::Update()
{
    connectionBuffer_->Commit( *this );
    messageBuffer_->Commit( *messageService_ );
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker::ConnectionSucceeded
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void ClientNetworker::ConnectionSucceeded( const std::string& endpoint )
{
    MT_LOG_INFO_MSG( "Connected to '" << endpoint << "'" );
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker::ConnectionFailed
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void ClientNetworker::ConnectionFailed( const std::string& endpoint, const std::string& error )
{
    MT_LOG_INFO_MSG( "Connection to '" << endpoint << "' failed (" << error << ")" );
    if( retry_ )
        connector_->Connect( host_ );
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker::ConnectionError
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void ClientNetworker::ConnectionError( const std::string& endpoint, const std::string& error )
{
    sockets_->Disconnect( endpoint );
    MT_LOG_INFO_MSG( "Connection to '" << endpoint << "' lost (" << error << ")" );    
    if( retry_ )
        connector_->Connect( host_ );
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker::Run
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void ClientNetworker::Run()
{
    ::SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL );
    while( !stopped_ )
    {
        service_->run();
        ::Sleep( 100 );
        service_->reset();
    }
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker::Stop
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void ClientNetworker::Stop()
{
    stopped_ = true;
    connector_->Close();
    sockets_->Disconnect();
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker::Send
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void ClientNetworker::Send( const std::string& endpoint, unsigned long tag, const Message& message )
{
    sockets_->Send( endpoint, tag, message );
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker::Register
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void ClientNetworker::Register( unsigned long id, std::auto_ptr< ObjectMessageCallback_ABC > callback )
{
    messageService_->Register( id, callback );
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker::Retrieve
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
ObjectMessageCallback_ABC* ClientNetworker::Retrieve( unsigned long id )
{
    return messageService_->Retrieve( id );
}

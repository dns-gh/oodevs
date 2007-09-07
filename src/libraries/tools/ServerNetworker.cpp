// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "ServerNetworker.h"
#include "BufferedMessageCallback.h"
#include "BufferedConnectionCallback.h"
#include "ObjectMessageService.h"
#include "SocketManager.h"
#include "Acceptor.h"
#include "asio.h"
#include "MT/MT_Logger/MT_Logger_lib.h"

using namespace tools;

// -----------------------------------------------------------------------------
// Name: ServerNetworker constructor
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
ServerNetworker::ServerNetworker( unsigned short port )
    : service_( new boost::asio::io_service() )
    , connectionBuffer_( new BufferedConnectionCallback() )
    , messageBuffer_( new BufferedMessageCallback() )
    , sockets_( new SocketManager( *messageBuffer_, *connectionBuffer_ ) )
    , messageService_( new ObjectMessageService() )
    , acceptor_( new Acceptor( *sockets_, *service_, port ) )
    , stopped_( false )
    , thread_( boost::bind( &ServerNetworker::Run, this ) )
{
    messageService_->RegisterErrorCallback( boost::bind( &ServerNetworker::ConnectionError, this, _1, _2 ) );
}

// -----------------------------------------------------------------------------
// Name: ServerNetworker destructor
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
ServerNetworker::~ServerNetworker()
{
    try
    {
        service_->post( boost::bind( &ServerNetworker::Stop, this ) );
        thread_.join();
    }
    catch( ... )
    {
        // NOTHING
    }
}

// -----------------------------------------------------------------------------
// Name: ServerNetworker::Disconnect
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void ServerNetworker::Disconnect( const std::string& endpoint )
{
    sockets_->Disconnect( endpoint );
}

// -----------------------------------------------------------------------------
// Name: ServerNetworker::DenyConnections
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void ServerNetworker::DenyConnections()
{
    acceptor_->DenyConnections();
}

// -----------------------------------------------------------------------------
// Name: ServerNetworker::AllowConnections
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void ServerNetworker::AllowConnections()
{
    acceptor_->AllowConnections();
}

// -----------------------------------------------------------------------------
// Name: ServerNetworker::Update
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void ServerNetworker::Update()
{
    connectionBuffer_->Commit( *this );
    messageBuffer_->Commit( *messageService_ );
}

// -----------------------------------------------------------------------------
// Name: ServerNetworker::Run
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void ServerNetworker::Run()
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
// Name: ServerNetworker::Stop
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void ServerNetworker::Stop()
{
    stopped_ = true;
    acceptor_->DenyConnections();
    sockets_->Disconnect();
}

// -----------------------------------------------------------------------------
// Name: ServerNetworker::Register
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void ServerNetworker::Register( unsigned long id, std::auto_ptr< ObjectMessageCallback_ABC > callback )
{
    messageService_->Register( id, callback );
}

// -----------------------------------------------------------------------------
// Name: ServerNetworker::Retrieve
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
ObjectMessageCallback_ABC* ServerNetworker::Retrieve( unsigned long id )
{
    return messageService_->Retrieve( id );
}

// -----------------------------------------------------------------------------
// Name: ServerNetworker::Send
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void ServerNetworker::Send( const std::string& endpoint, unsigned long tag, const Message& message )
{
    return sockets_->Send( endpoint, tag, message );
}

// -----------------------------------------------------------------------------
// Name: ServerNetworker::ConnectionSucceeded
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void ServerNetworker::ConnectionSucceeded( const std::string& endpoint )
{
    MT_LOG_INFO_MSG( "Connection received from client '" << endpoint << "'" );
}

// -----------------------------------------------------------------------------
// Name: ServerNetworker::ConnectionFailed
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void ServerNetworker::ConnectionFailed( const std::string& endpoint, const std::string& error )
{
    MT_LOG_INFO_MSG( "Bad connection received from client '" << endpoint << "' (" << error << ")" );
}

// -----------------------------------------------------------------------------
// Name: ServerNetworker::ConnectionError
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void ServerNetworker::ConnectionError( const std::string& endpoint, const std::string& error )
{
    sockets_->Disconnect( endpoint );
    MT_LOG_INFO_MSG( "Connection to '" << endpoint << "' lost (" << error << ")" );    
}

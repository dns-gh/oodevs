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

#pragma warning( disable : 4355 )

using namespace tools;

// -----------------------------------------------------------------------------
// Name: ServerNetworker constructor
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
ServerNetworker::ServerNetworker( unsigned short port, unsigned long timeOut /*=10000*/ )
    : service_         ( new boost::asio::io_service() )
    , connectionBuffer_( new BufferedConnectionCallback() )
    , messageBuffer_   ( new BufferedMessageCallback() )
    , sockets_         ( new SocketManager( messageBuffer_, connectionBuffer_, timeOut ) )
    , messageService_  ( new ObjectMessageService() )
    , acceptor_        ( new Acceptor( *sockets_, *service_, port ) )
    , stopped_         ( false )
    , thread_          ( boost::bind( &ServerNetworker::Run, this ) )
{
    messageService_->RegisterErrorCallback( boost::bind( &ServerNetworker::ConnectionError, this, _1, _2 ) );
    messageService_->RegisterWarningCallback( boost::bind( &ServerNetworker::ConnectionWarning, this, _1, _2 ) );
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
    sockets_->Send( endpoint, tag, message );
}

// -----------------------------------------------------------------------------
// Name: ServerNetworker::ConnectionSucceeded
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void ServerNetworker::ConnectionSucceeded( const std::string& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ServerNetworker::ConnectionFailed
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void ServerNetworker::ConnectionFailed( const std::string& , const std::string& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ServerNetworker::ConnectionError
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void ServerNetworker::ConnectionError( const std::string& endpoint, const std::string& )
{
    sockets_->Disconnect( endpoint );
}

// -----------------------------------------------------------------------------
// Name: ServerNetworker::ConnectionWarning
// Created: MCO 2011-09-26
// -----------------------------------------------------------------------------
void ServerNetworker::ConnectionWarning( const std::string& , const std::string& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ServerNetworker::GetNbMessagesReceived
// Created: NLD 2010-10-20
// -----------------------------------------------------------------------------
unsigned long ServerNetworker::GetNbMessagesReceived() const
{
    return messageService_->GetNbMessagesReceived();
}

// -----------------------------------------------------------------------------
// Name: ServerNetworker::GetNbMessagesSent
// Created: NLD 2010-10-20
// -----------------------------------------------------------------------------
unsigned long ServerNetworker::GetNbMessagesSent() const
{
    return sockets_->GetNbMessagesSent();
}

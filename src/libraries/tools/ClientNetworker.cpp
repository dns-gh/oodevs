// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "ClientNetworker.h"
#include "SocketManager.h"
#include "Connector.h"
#include "BufferedMessageCallback.h"
#include "ObjectMessageService.h"
#include <tools/WaitEvent.h>
#include <boost/bind.hpp>
#include "asio.h"

#pragma warning( disable : 4355 )

using namespace tools;

// -----------------------------------------------------------------------------
// Name: ClientNetworker constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
ClientNetworker::ClientNetworker( const std::string& host /* = "" */, bool retry /* = false*/, unsigned long timeOut /*=10000*/ )
    : service_         ( new boost::asio::io_service() )
    , messageBuffer_   ( new BufferedMessageCallback() )
    , sockets_         ( new SocketManager( messageBuffer_, messageBuffer_, timeOut ) )
    , messageService_  ( new ObjectMessageService() )
    , connector_       ( new Connector( *service_, *sockets_, *messageBuffer_ ) )
    , quit_            ( new WaitEvent() )
    , retry_           ( retry )
    , thread_          ( boost::bind( &ClientNetworker::Run, this ) )
    , inMessages_      ( 0 )
    , inBytes_         ( 0 )
{
    messageService_->RegisterErrorCallback( boost::bind( &ClientNetworker::ConnectionError, this, _1, _2 ) );
    messageService_->RegisterWarningCallback( boost::bind( &ClientNetworker::ConnectionWarning, this, _1, _2 ) );
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
        quit_->Signal();
        service_->stop();
        thread_.join();
        connector_->Close();
        sockets_->Disconnect();
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

namespace
{

struct CommitStatistics : public MessageCallback_ABC, boost::noncopyable
{
    CommitStatistics( MessageCallback_ABC& wrapped )
        : wrapped_( wrapped )
        , messages_( 0 )
        , messagesSize_( 0 )
    {
    }

    virtual void OnError( const std::string& endpoint, const std::string& error )
    {
        wrapped_.OnError( endpoint, error );
    }

    virtual void OnWarning( const std::string& endpoint, const std::string& warning )
    {
        wrapped_.OnWarning( endpoint, warning );
    }

    virtual void OnMessage( const std::string& endpoint, Message& message )
    {
        wrapped_.OnMessage( endpoint, message );
        messages_++;
        messagesSize_ += message.Size();
    }

    MessageCallback_ABC& wrapped_;
    unsigned long messages_;
    size_t messagesSize_;
};

}  // namespace

// -----------------------------------------------------------------------------
// Name: ClientNetworker::Update
// Created: NLD 2007-01-24
// -----------------------------------------------------------------------------
void ClientNetworker::Update()
{
    CommitStatistics stats( *messageService_ );
    messageBuffer_->Commit( *this, stats );
    inMessages_ += stats.messages_;
    inBytes_ += stats.messagesSize_;
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker::ConnectionSucceeded
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void ClientNetworker::ConnectionSucceeded( const std::string&, const std::string& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker::ConnectionFailed
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void ClientNetworker::ConnectionFailed( const std::string& , const std::string& )
{
    if( retry_ )
        connector_->Connect( host_ );
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker::ConnectionError
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void ClientNetworker::ConnectionError( const std::string& endpoint, const std::string& )
{
    sockets_->Disconnect( endpoint );
    if( retry_ )
        connector_->Connect( host_ );
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker::ConnectionWarning
// Created: MCO 2011-09-26
// -----------------------------------------------------------------------------
void ClientNetworker::ConnectionWarning( const std::string& , const std::string& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker::Run
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void ClientNetworker::Run()
{
    ::SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL );
    do
    {
        service_->run();
        service_->reset();
    } while( !quit_->Wait( boost::posix_time::milliseconds( 100 ) ) );
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker::Send
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void ClientNetworker::Send( const std::string& endpoint, unsigned long tag, Message& message )
{
    sockets_->Send( endpoint, tag, message );
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker::Register
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void ClientNetworker::Register( unsigned long id, std::unique_ptr< ObjectMessageCallback_ABC > callback )
{
    messageService_->Register( id, std::move( callback ) );
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker::Retrieve
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
ObjectMessageCallback_ABC* ClientNetworker::Retrieve( unsigned long id )
{
    return messageService_->Retrieve( id );
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker::GetNbMessagesReceived
// Created: NLD 2010-10-20
// -----------------------------------------------------------------------------
unsigned long ClientNetworker::GetNbMessagesReceived() const
{
    return inMessages_;
}

uint64_t ClientNetworker::GetReceivedAmount() const
{
    return inBytes_;
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker::GetNbMessagesSent
// Created: NLD 2010-10-20
// -----------------------------------------------------------------------------
unsigned long ClientNetworker::GetNbMessagesSent() const
{
    return sockets_->GetNbMessagesSent();
}

uint64_t ClientNetworker::GetSentAmount() const
{
    return sockets_->GetSentAmount();
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker::HasAnsweredSinceLastTick
// Created: LDC 2011-11-23
// -----------------------------------------------------------------------------
bool ClientNetworker::HasAnsweredSinceLastTick( const std::string& endpoint )
{
    return sockets_->HasAnsweredSinceLastTick( endpoint );
}

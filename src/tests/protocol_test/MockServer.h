// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MockServer_h_
#define __MockServer_h_

#include "tools/ServerNetworker.h"
#include <boost/date_time/posix_time/posix_time.hpp>

namespace google
{
    namespace protobuf
    {
        class Message;
    }
}

// =============================================================================
/** @class  MockServer
    @brief  MockServer
*/
// Created: FHD 2009-08-24
// =============================================================================
class MockServer : public mockpp::ChainableMockObject
                 , public tools::ServerNetworker
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MockServer( unsigned int port = 10000 )
        : mockpp::ChainableMockObject( "MockServer", 0 )
        , tools::ServerNetworker( port )
        , OnReceivePion_mocker( "OnReceive", this )
        , ConnectionSucceeded_mocker( "ConnectionSucceeded", this )
        , ConnectionFailed_mocker( "ConnectionFailed", this )
        , ConnectionError_mocker( "ConnectionError", this )
        , received_( false )
    {
        RegisterMessage( *this, &MockServer::Receive< MsgPion > );
        AllowConnections();
    }
    //@}

    //! @name Accessors
    //@{
    bool Received() const
    {
        return received_;
    }
    //@}

    //! @name Operations
    //@{
    template< typename T >
    void Send( const T& message )
    {
        for( std::vector< std::string >::const_iterator it = clients_.begin(); it != clients_.end(); ++it )
            tools::MessageSender_ABC::Send( *it, message );
    }
    template< typename T >
    void Receive( const std::string& endpoint, const T& message )
    {
        received_ = true;
        OnReceive( endpoint, message );
    }
    void OnReceive( const std::string& endpoint, const MsgPion& message )
    {
        received_ = true;
        OnReceivePion_mocker.forward( endpoint, message );
    }
    virtual void ConnectionSucceeded( const std::string& endpoint )
    {
        clients_.push_back( endpoint );
        tools::ServerNetworker::ConnectionSucceeded( endpoint );
        ConnectionSucceeded_mocker.forward( endpoint );
    }
    virtual void ConnectionFailed( const std::string& address, const std::string& error )
    {
        tools::ServerNetworker::ConnectionFailed( address, error );
        ConnectionFailed_mocker.forward( address, error );
    }
    virtual void ConnectionError( const std::string& address, const std::string& error )
    {
        tools::ServerNetworker::ConnectionError( address, error );
        ConnectionError_mocker.forward( address, error );
    }
    void ResetReceived()
    {
        received_ = false;
    }
    //@}

public:
    //! @name Mock methods
    //@{
    mockpp::ChainableMockMethod< void, const std::string, const MsgPion >     OnReceivePion_mocker;
    mockpp::ChainableMockMethod< void, const std::string >                    ConnectionSucceeded_mocker;
    mockpp::ChainableMockMethod< void, const std::string, const std::string > ConnectionFailed_mocker;
    mockpp::ChainableMockMethod< void, const std::string, const std::string > ConnectionError_mocker;
    //@}
    
private:
    //! @name Member data
    //@{
    std::vector< std::string > clients_;
    bool received_;
    //@}
};

#endif // __MockServer_h_

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MockClient_h_
#define __MockClient_h_

#include "tools/ClientNetworker.h"
#include <boost/date_time/posix_time/posix_time.hpp>

// =============================================================================
/** @class  MockClient
    @brief  MockClient
*/
// Created: FHD 2009-08-24
// =============================================================================
class MockClient : public mockpp::ChainableMockObject
                 , public tools::ClientNetworker
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MockClient( const std::string& address )
        : mockpp::ChainableMockObject( "MockClient", 0 )
        , tools::ClientNetworker( address )
        , OnReceivePion_mocker( "OnReceive", this )
        , OnReceiveEmpty_mocker( "OnReceive", this )
        , ConnectionSucceeded_mocker( "ConnectionSucceeded", this )
        , ConnectionFailed_mocker( "ConnectionFailed", this )
        , ConnectionError_mocker( "ConnectionError", this )
        , connected_( false )
        , received_( false )
    {
        RegisterMessage( *this, &MockClient::OnReceive< MsgPion > );
        RegisterMessage( *this, &MockClient::OnReceive< EmptyMessage > );
    }
    //@}

    //! @name Accessors
    //@{
    bool Connected() const
    {
        return connected_;
    }
    bool Received() const
    {
        return received_;
    }
    void ResetReceived()
    {
        if( Received() )
            received_ = false;
    }
    //@}

    //! @name Operations
    //@{
    template< typename T >
    void OnReceive( const std::string& endpoint, const T& message )
    {
        OnReceive( endpoint, message );
    }
    void OnReceive( const std::string& endpoint, const MsgPion& message )
    {
        LogReceived( message );
        OnReceivePion_mocker.forward( endpoint, message );
    }
    void OnReceive( const std::string& endpoint, const EmptyMessage& message )
    {
        LogReceived( message );
        OnReceiveEmpty_mocker.forward( endpoint, message );
    }
    virtual void ConnectionSucceeded( const std::string& endpoint )
    {
        connected_ = true;
        tools::ClientNetworker::ConnectionSucceeded( endpoint );
        ConnectionSucceeded_mocker.forward( endpoint );
    }
    virtual void ConnectionFailed( const std::string& address, const std::string& error )
    {
        connected_ = false;
        tools::ClientNetworker::ConnectionFailed( address, error );
        ConnectionFailed_mocker.forward( address, error );
    }
    virtual void ConnectionError( const std::string& address, const std::string& error )
    {
        connected_ = false;
        tools::ClientNetworker::ConnectionError( address, error );
        ConnectionError_mocker.forward( address, error );
    }

private:
    template< typename M >
    void LogReceived( const M& message )
    {
        received_ = true;
        BOOST_TEST_MESSAGE( "Client: " << typeid( message ).name() << " received at " << boost::posix_time::to_iso_string( boost::posix_time::second_clock::local_time() ) );
    }

public:
    //! @name Mock methods
    //@{
    mockpp::ChainableMockMethod< void, const std::string, const MsgPion >      OnReceivePion_mocker;
    mockpp::ChainableMockMethod< void, const std::string, const EmptyMessage > OnReceiveEmpty_mocker;
    mockpp::ChainableMockMethod< void, const std::string >                     ConnectionSucceeded_mocker;
    mockpp::ChainableMockMethod< void, const std::string, const std::string >  ConnectionFailed_mocker;
    mockpp::ChainableMockMethod< void, const std::string, const std::string >  ConnectionError_mocker;
    //@}

private:
    //! @name Member data
    //@{
    bool connected_;
    bool received_;
    //@}
};

#endif // __MockClient_h_

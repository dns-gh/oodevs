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
    explicit MockClient( const std::string& address = "127.0.0.1:10000" )
        : mockpp::ChainableMockObject( "MockClient", 0 )
        , tools::ClientNetworker( address )
        , ConnectionSucceeded_mocker( "ConnectionSucceeded", this )
        , OnReceivePion_mocker( "OnReceive", this )
        , OnReceiveEmpty_mocker( "OnReceive", this )
        , OnReceiveTest_mocker( "OnReceive", this )
        , OnReceiveObjectInstance_mocker( "OnReceive", this )
        , connected_( false )
        , received_( false )
    {
        RegisterMessage( *this, &MockClient::OnReceive< MsgPion > );
        RegisterMessage( *this, &MockClient::OnReceive< EmptyMessage > );
        RegisterMessage( *this, &MockClient::OnReceive< Test > );
        RegisterMessage( *this, &MockClient::OnReceive< ObjectInstance > );
    }
    virtual ~MockClient() { }
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
        if( Received())
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
    void OnReceive( const std::string& endpoint, const Test& message )
    {
        LogReceived( message );
        OnReceiveTest_mocker.forward( endpoint, message );
    }
    void OnReceive( const std::string& endpoint, const ObjectInstance& message )
    {
        LogReceived( message );
        OnReceiveObjectInstance_mocker.forward( endpoint, message );
    }
    virtual void ConnectionSucceeded( const std::string& endpoint )
    {
        connected_ = true;
        tools::ClientNetworker::ConnectionSucceeded( endpoint );
        ConnectionSucceeded_mocker.forward( endpoint );
    }

private:
    template< typename M >
    void LogReceived( const M& message )
    {
        received_ = true;
        std::cout << "client: " << typeid( message ).name() 
                  << " received at " << boost::posix_time::to_iso_string( boost::posix_time::second_clock::local_time() ) << std::endl;
    }

public:
    //! @name Mock methods
    //@{
    mockpp::ChainableMockMethod< void, const std::string, const MsgPion >          OnReceivePion_mocker;
    mockpp::ChainableMockMethod< void, const std::string, const EmptyMessage >     OnReceiveEmpty_mocker;
    mockpp::ChainableMockMethod< void, const std::string, const Test >             OnReceiveTest_mocker;
    mockpp::ChainableMockMethod< void, const std::string, const ObjectInstance >   OnReceiveObjectInstance_mocker;
    mockpp::ChainableMockMethod< void, const std::string >                         ConnectionSucceeded_mocker;
    //@}

private:
    //! @name Member data
    //@{
    bool connected_;
    bool received_;
    //@}
};

#endif // __MockClient_h_

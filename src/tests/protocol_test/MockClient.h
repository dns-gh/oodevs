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
MOCK_BASE_CLASS( MockClient, tools::ClientNetworker )
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MockClient( const std::string& address )
        : tools::ClientNetworker( address )
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
        OnReceivePion( endpoint, message );
    }
    void OnReceive( const std::string& endpoint, const EmptyMessage& message )
    {
        LogReceived( message );
        OnReceiveEmpty( endpoint, message );
    }
    virtual void ConnectionSucceeded( const std::string& endpoint )
    {
        connected_ = true;
        tools::ClientNetworker::ConnectionSucceeded( endpoint );
        OnConnectionSucceeded( endpoint );
    }
    virtual void ConnectionFailed( const std::string& address, const std::string& error )
    {
        connected_ = false;
        tools::ClientNetworker::ConnectionFailed( address, error );
        OnConnectionFailed( address, error );
    }
    virtual void ConnectionError( const std::string& address, const std::string& error )
    {
        connected_ = false;
        tools::ClientNetworker::ConnectionError( address, error );
        OnConnectionError( address, error );
    }

private:
    template< typename M >
    void LogReceived( const M& message )
    {
        received_ = true;
        BOOST_TEST_MESSAGE( "Client: " << typeid( message ).name() << " received at " << boost::posix_time::to_iso_string( boost::posix_time::second_clock::local_time() ) );
    }

public:
    //! @name Member data
    //@{
    MOCK_FUNCTOR( void( const std::string&, const MsgPion& ) ) OnReceivePion;
    MOCK_FUNCTOR( void( const std::string&, const EmptyMessage& ) ) OnReceiveEmpty;
    MOCK_FUNCTOR( void( const std::string& ) ) OnConnectionSucceeded;
    MOCK_FUNCTOR( void( const std::string&, const std::string& ) ) OnConnectionFailed;
    MOCK_FUNCTOR( void( const std::string&, const std::string& ) ) OnConnectionError;
    //@}

private:
    //! @name Member data
    //@{
    bool connected_;
    bool received_;
    //@}
};

#endif // __MockClient_h_

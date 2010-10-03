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

// =============================================================================
/** @class  MockServer
    @brief  MockServer
*/
// Created: FHD 2009-08-24
// =============================================================================
MOCK_BASE_CLASS( MockServer, tools::ServerNetworker )
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MockServer( unsigned short port )
        : tools::ServerNetworker( port )
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
        OnReceivePion( endpoint, message );
    }
    virtual void ConnectionSucceeded( const std::string& endpoint )
    {
        clients_.push_back( endpoint );
        tools::ServerNetworker::ConnectionSucceeded( endpoint );
        OnConnectionSucceeded( endpoint );
    }
    virtual void ConnectionFailed( const std::string& address, const std::string& error )
    {
        tools::ServerNetworker::ConnectionFailed( address, error );
        OnConnectionFailed( address, error );
    }
    virtual void ConnectionError( const std::string& address, const std::string& error )
    {
        tools::ServerNetworker::ConnectionError( address, error );
        OnConnectionError( address, error );
    }
    void ResetReceived()
    {
        received_ = false;
    }
    //@}

public:
    //! @name Member data
    //@{
    MOCK_FUNCTOR( void( const std::string&, const MsgPion& ) ) OnReceivePion;
    MOCK_FUNCTOR( void( const std::string& ) ) OnConnectionSucceeded;
    MOCK_FUNCTOR( void( const std::string&, const std::string& ) ) OnConnectionFailed;
    MOCK_FUNCTOR( void( const std::string&, const std::string& ) ) OnConnectionError;
    //@}

private:
    //! @name Member data
    //@{
    std::vector< std::string > clients_;
    bool received_;
    //@}
};

#endif // __MockServer_h_

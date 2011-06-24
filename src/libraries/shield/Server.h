// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef shield_Server_h
#define shield_Server_h

#include "ClientListener_ABC.h"
#include "tools/ServerNetworker.h"
#include <boost/shared_ptr.hpp>
#include <map>

namespace shield
{
    class Listener_ABC;
    class Client;

// =============================================================================
/** @class  Server
    @brief  Server
*/
// Created: MCO 2010-11-29
// =============================================================================
class Server : private tools::ServerNetworker, private ClientListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Server( unsigned short port, const std::string& host, Listener_ABC& listener );
    virtual ~Server();
    //@}

    //! @name Operations
    //@{
    virtual void Update();
    //@}

private:
    //! @name Operations
    //@{
    virtual void ConnectionSucceeded( const std::string& from );
    virtual void ConnectionFailed( const std::string& from, const std::string& error );
    virtual void ConnectionError( const std::string& from, const std::string& error );

    virtual void Info( const std::string& message );
    virtual void Error( const std::string& from, const std::string& message );
    virtual void Debug( const DebugInfo_ABC& info );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, boost::shared_ptr< Client > > T_Clients;
    typedef T_Clients::const_iterator                          CIT_Clients;

    typedef std::vector< std::string > T_Errors;
    typedef T_Errors::const_iterator CIT_Errors;
    //@}

private:
    //! @name Member data
    //@{
    const std::string host_;
    Listener_ABC& listener_;
    T_Clients clients_;
    T_Errors errors_;
    //@}
};

}

#endif // shield_Server_h

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ClientsNetworker_h_
#define __ClientsNetworker_h_

#include "protocol/ClientPublisher_ABC.h"
#include "MessageHandler_ABC.h"
#include "LinkResolver_ABC.h"
#include "tools/ServerNetworker.h"

namespace dispatcher
{
    class Client;
    class Config;
    class Plugin_ABC;
    class Services;

// =============================================================================
/** @class  ClientsNetworker
    @brief  ClientsNetworker
*/
// Created: NLD 2006-09-19
// =============================================================================
class ClientsNetworker : public tools::ServerNetworker
                       , public ClientPublisher_ABC
                       , public MessageHandler_ABC
                       , public LinkResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ClientsNetworker( const Config& config, Plugin_ABC& plugin, const Services& services );
    virtual ~ClientsNetworker();
    //@}

    //! @name Main
    //@{
    virtual void Send( const sword::SimToClient& msg );
    virtual void Send( const sword::AuthenticationToClient& msg );
    virtual void Send( const sword::ReplayToClient& );
    virtual void Send( const sword::AarToClient& msg );
    virtual void Send( const sword::MessengerToClient& msg );
    virtual void Send( const sword::DispatcherToClient& msg );

    virtual Profile_ABC&         GetProfile  ( const std::string& link );
    virtual ClientPublisher_ABC& GetPublisher( const std::string& link );

    virtual std::string GetEndpoint() const;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ClientsNetworker( const ClientsNetworker& );            //!< Copy constructor
    ClientsNetworker& operator=( const ClientsNetworker& ); //!< Assignment operator
    //@}

    //! @name Connection callbacks
    //@{
    virtual void ConnectionSucceeded( const std::string& endpoint );
    virtual void ConnectionFailed   ( const std::string& address, const std::string& error );
    virtual void ConnectionError    ( const std::string& endpoint, const std::string& reason );
    virtual void ConnectionWarning  ( const std::string& endpoint, const std::string& reason );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, Client* > T_Clients;
    typedef T_Clients::const_iterator      CIT_Clients;
    //@}

private:
    //! @name Member data
    //@{
    Plugin_ABC& plugin_;
    const Services& services_;
    T_Clients clients_;
    //@}
};

}

#endif // __ClientsNetworker_h_

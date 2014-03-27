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

#include "LinkResolver_ABC.h"
#include "Plugin_ABC.h"
#include "tools/ServerNetworker.h"
#include "protocol/ClientPublisher_ABC.h"
#include <boost/shared_ptr.hpp>
#include <map>
#pragma warning( push, 0 )
#include <boost/bimap.hpp>
#pragma warning( pop )

namespace dispatcher
{
    class Config;
    class Plugin_ABC;
    class Services;
    class Model_ABC;
    class Client;

// =============================================================================
/** @class  ClientsNetworker
    @brief  ClientsNetworker
*/
// Created: NLD 2006-09-19
// =============================================================================
class ClientsNetworker : public tools::ServerNetworker
                       , public ClientPublisher_ABC
                       , public LinkResolver_ABC
                       , public Plugin_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ClientsNetworker( const Config& config, Plugin_ABC& plugin, const Services& services, const Model_ABC& model );
    virtual ~ClientsNetworker();
    //@}

    //! @name Operations
    //@{
    virtual void Send( const sword::SimToClient& msg );
    virtual void Send( const sword::AuthenticationToClient& msg );
    virtual void Send( const sword::ReplayToClient& );
    virtual void Send( const sword::AarToClient& msg );
    virtual void Send( const sword::MessengerToClient& msg );
    virtual void Send( const sword::DispatcherToClient& msg );

    virtual ClientPublisher_ABC& GetConnectedPublisher( const std::string& link ) const;

    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, const std::string& link,
                                            dispatcher::Profile_ABC& profile, unsigned int clientId, bool uncounted );
    virtual void NotifyClientLeft( dispatcher::ClientPublisher_ABC& client, const std::string& link, bool uncounted );

    virtual void Update();

    virtual void Receive( const sword::SimToClient& message );
    //@}

private:
    //! @name Operations
    //@{
    virtual void ConnectionSucceeded( const std::string& local, const std::string& remote );
    virtual void ConnectionFailed   ( const std::string& address, const std::string& error );
    virtual void ConnectionError    ( const std::string& endpoint, const std::string& reason );
    virtual void ConnectionWarning  ( const std::string& endpoint, const std::string& reason );

    void Broadcast( const sword::SimToClient& message );
    void Unicast( const sword::SimToClient& message );
    void OnNewTick();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, boost::shared_ptr< Client > > T_Clients;
    typedef boost::bimap< std::string, unsigned int > T_ClientsId;
    //@}

private:
    //! @name Member data
    //@{
    Plugin_ABC& plugin_;
    const Services& services_;
    const Model_ABC& model_;
    T_Clients clients_;
    T_Clients internals_;
    T_ClientsId clientsId_;
    std::set< std::string > uncountedClients_;
    //@}
};

}

#endif // __ClientsNetworker_h_

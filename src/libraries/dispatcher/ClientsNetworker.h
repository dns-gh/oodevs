// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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

    virtual Profile_ABC&         GetProfile  ( const std::string& link );
    virtual unsigned int         GetClientID( const std::string& link ) const;
    virtual ClientPublisher_ABC& GetPublisher( const std::string& link );

    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, const std::string& link, dispatcher::Profile_ABC& profile );
    virtual void NotifyClientLeft( dispatcher::ClientPublisher_ABC& client, const std::string& link );

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

    void OnNewTick();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, boost::shared_ptr< Client > > T_Clients;
    //@}

private:
    //! @name Member data
    //@{
    Plugin_ABC& plugin_;
    const Services& services_;
    const Model_ABC& model_;
    T_Clients clients_;
    T_Clients internals_;
    //@}
};

}

#endif // __ClientsNetworker_h_

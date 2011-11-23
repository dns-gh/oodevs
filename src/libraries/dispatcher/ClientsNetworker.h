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

#include "MessageHandler_ABC.h"
#include "LinkResolver_ABC.h"
#include "Plugin_ABC.h"
#include "tools/ServerNetworker.h"
#include "shield/ClientHandler_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientBroadcaster_ABC.h"
#include <boost/shared_ptr.hpp>
#include <map>

namespace dispatcher
{
    class Client;
    class Config;
    class Plugin_ABC;
    class Services;
    class Model_ABC;

// =============================================================================
/** @class  ClientsNetworker
    @brief  ClientsNetworker
*/
// Created: NLD 2006-09-19
// =============================================================================
class ClientsNetworker : public tools::ServerNetworker
                       , public ClientPublisher_ABC
                       , public LinkResolver_ABC
                       , public shield::ClientHandler_ABC
                       , public Plugin_ABC
                       , private ClientBroadcaster_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ClientsNetworker( const Config& config, Plugin_ABC& plugin, const Services& services, const Model_ABC& model );
    virtual ~ClientsNetworker();
    //@}

    //! @name Operations
    //@{
    virtual void Register( const std::string& endpoint, MessageSender_ABC& sender, ClientBroadcaster_ABC& broadcaster );
    virtual void Unregister( const std::string& endpoint );

    virtual void Send( const sword::SimToClient& msg );
    virtual void Send( const sword::AuthenticationToClient& msg );
    virtual void Send( const sword::ReplayToClient& );
    virtual void Send( const sword::AarToClient& msg );
    virtual void Send( const sword::MessengerToClient& msg );
    virtual void Send( const sword::DispatcherToClient& msg );

    virtual Profile_ABC&         GetProfile  ( const std::string& link );
    virtual ClientPublisher_ABC& GetPublisher( const std::string& link );

    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, const std::string& link, dispatcher::Profile_ABC& profile );
    virtual void NotifyClientLeft( dispatcher::ClientPublisher_ABC& client, const std::string& link );

    virtual void Update();
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

    //! @name Operations
    //@{
    virtual void ConnectionSucceeded( const std::string& endpoint );
    virtual void ConnectionFailed   ( const std::string& address, const std::string& error );
    virtual void ConnectionError    ( const std::string& endpoint, const std::string& reason );
    virtual void ConnectionWarning  ( const std::string& endpoint, const std::string& reason );

    virtual void Activate( const std::string& link );
    virtual void Deactivate( const std::string& link );

    virtual void Broadcast( const sword::SimToClient& message );

    void OnNewTick();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, boost::shared_ptr< Client > > T_Clients;
    typedef T_Clients::iterator                                 IT_Clients;
    typedef T_Clients::const_iterator                          CIT_Clients;

    typedef std::set< ClientBroadcaster_ABC* > T_Broadcasters;
    typedef T_Broadcasters::const_iterator   CIT_Broadcasters;
    //@}

private:
    //! @name Member data
    //@{
    Plugin_ABC& plugin_;
    const Services& services_;
    const Model_ABC& model_;
    T_Clients clients_;
    T_Clients internals_;
    T_Broadcasters broadcasters_;
    //@}
};

}

#endif // __ClientsNetworker_h_

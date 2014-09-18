// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RightsPlugin_h_
#define __RightsPlugin_h_

#include "dispatcher/Plugin_ABC.h"
#include "dispatcher/AuthenticatedLinkResolver_ABC.h"
#include <boost/shared_ptr.hpp>
#include <set>
#include <map>

namespace sword
{
    class ClientToAuthentication;
    class AuthenticationRequest;
    class ProfileUpdateRequest;
    class ProfileDestructionRequest;
    class ProfileCreationRequest;
    class ConnectedProfilesRequest;
    class AuthenticationToClient;
}

namespace tools
{
    class MessageDispatcher_ABC;
}

namespace dispatcher
{
    class ProfileManager;
    class Model;
    class Config;
    class CompositeRegistrable;
    class Profile;
    class LinkResolver_ABC;
}

namespace plugins
{
namespace rights
{
    class AuthenticationSender;

// =============================================================================
/** @class  RightsPlugin
    @brief  RightsPlugin
*/
// Created: AGE 2007-08-24
// =============================================================================
class RightsPlugin : public dispatcher::Plugin_ABC, public dispatcher::AuthenticatedLinkResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             RightsPlugin( dispatcher::Model& model, dispatcher::ClientPublisher_ABC& clients, const dispatcher::Config& config,
                           tools::MessageDispatcher_ABC& clientCommands, dispatcher::Plugin_ABC& container, const dispatcher::LinkResolver_ABC& resolver,
                           dispatcher::CompositeRegistrable& registrables, int maxConnections, bool replayer );
    virtual ~RightsPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& message );

    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, const std::string& link,
                                            dispatcher::Profile_ABC& profile, unsigned int clientId, bool uncounted );
    virtual void NotifyClientLeft( dispatcher::ClientPublisher_ABC& client, const std::string& link, bool uncounted );

    virtual void Register( dispatcher::Services& services );

    virtual dispatcher::Profile_ABC& GetProfile( const std::string& link ) const;
    virtual dispatcher::ClientPublisher_ABC& GetAuthenticatedPublisher( const std::string& link ) const;
    virtual dispatcher::ClientPublisher_ABC& GetAuthenticatedPublisher( unsigned int clientId ) const;
    virtual unsigned int GetClientID( const std::string& link ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void OnReceive( const std::string& link, const sword::ClientToAuthentication& message );
    void OnReceiveMsgAuthenticationRequest( const std::string& link, const sword::AuthenticationRequest& message, AuthenticationSender& sender );
    void OnReceiveProfileCreationRequest(const sword::ProfileCreationRequest& message, AuthenticationSender& sender, const std::string& link );
    void OnReceiveProfileUpdateRequest(const sword::ProfileUpdateRequest& message, AuthenticationSender& sender, const std::string& link );
    void OnReceiveProfileDestructionRequest(const sword::ProfileDestructionRequest& message, AuthenticationSender& sender, const std::string& link );
    void OnReceiveConnectedProfilesRequest(const sword::ConnectedProfilesRequest& message, AuthenticationSender& sender, const std::string& link );

    bool IsAuthenticated( const std::string& login ) const;
    void Logout( dispatcher::ClientPublisher_ABC& client );
    void SendProfiles( AuthenticationSender& sender ) const;
    void SendReponse( sword::AuthenticationToClient& reply, AuthenticationSender& sender, const std::string& link ) const;

    unsigned int AcquireClientId();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, boost::shared_ptr< dispatcher::Profile > > T_Profiles;

    typedef std::map< std::string, unsigned int > T_ClientsID;
    typedef std::map< std::string, std::string >  T_AuthenticationKeys;
    typedef std::set< std::string >               T_SilentClients;
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::ClientPublisher_ABC&            clients_;
    const dispatcher::Config&                   config_;
    dispatcher::Plugin_ABC&                     container_;
    const dispatcher::LinkResolver_ABC&         resolver_;
    std::unique_ptr< dispatcher::ProfileManager > profiles_;
    T_Profiles                                  authenticated_;
    int                                         maxConnections_;
    int                                         currentConnections_;
    T_ClientsID                                 clientsID_;
    std::map< unsigned int, std::string >       ids_;
    unsigned int                                countID_;
    T_AuthenticationKeys                        authenticationKeys_;
    T_SilentClients                             silentClients_;
    const bool                                  replayer_;
    //@}
};

}
}

#endif // __RightsPlugin_h_

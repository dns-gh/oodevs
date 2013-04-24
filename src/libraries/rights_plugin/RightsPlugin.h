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
#include "dispatcher/LinkResolver_ABC.h"
#include <boost/shared_ptr.hpp>
#include <map>

namespace sword
{
    class ClientToAuthentication;
    class AuthenticationRequest;
    class ProfileUpdateRequest;
    class ProfileDestructionRequest;
    class ProfileCreationRequest;
    class ConnectedProfilesRequest;
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
class RightsPlugin : public dispatcher::Plugin_ABC, public dispatcher::LinkResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             RightsPlugin( dispatcher::Model& model, dispatcher::ClientPublisher_ABC& clients, const dispatcher::Config& config,
                           tools::MessageDispatcher_ABC& clientCommands, dispatcher::Plugin_ABC& container, dispatcher::LinkResolver_ABC& base,
                           dispatcher::CompositeRegistrable& registrables, int maxConnections );
    virtual ~RightsPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& message );

    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, const std::string& link, dispatcher::Profile_ABC& profile );
    virtual void NotifyClientLeft( dispatcher::ClientPublisher_ABC& client, const std::string& link );

    virtual void Register( dispatcher::Services& services );

    virtual dispatcher::Profile_ABC& GetProfile( const std::string& link );
    virtual dispatcher::ClientPublisher_ABC& GetPublisher( const std::string& link );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    RightsPlugin( const RightsPlugin& );            //!< Copy constructor
    RightsPlugin& operator=( const RightsPlugin& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void OnReceive( const std::string& link, const sword::ClientToAuthentication& message );
    void OnReceiveMsgAuthenticationRequest( const std::string& link, const sword::AuthenticationRequest& message, AuthenticationSender& sender );
    void OnReceiveProfileCreationRequest(const sword::ProfileCreationRequest& message, AuthenticationSender& sender );
    void OnReceiveProfileUpdateRequest(const sword::ProfileUpdateRequest& message, AuthenticationSender& sender );
    void OnReceiveProfileDestructionRequest(const sword::ProfileDestructionRequest& message, AuthenticationSender& sender );
    void OnReceiveConnectedProfilesRequest(const sword::ConnectedProfilesRequest& message, AuthenticationSender& sender );

    bool IsAuthenticated( const std::string& login ) const;
    void Logout( dispatcher::ClientPublisher_ABC& client );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, boost::shared_ptr< dispatcher::Profile > > T_Profiles;
    typedef T_Profiles::iterator                         IT_Profiles;
    typedef T_Profiles::const_iterator                  CIT_Profiles;
    //@}

private:
    //! @name Helpers
    //@{
    void SendProfiles( AuthenticationSender& sender ) const;
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::ClientPublisher_ABC&            clients_;
    const dispatcher::Config&                   config_;
    dispatcher::Plugin_ABC&                     container_;
    dispatcher::LinkResolver_ABC&               base_;
    std::auto_ptr< dispatcher::ProfileManager > profiles_;
    T_Profiles                                  authenticated_;
    int                                         maxConnections_;
    int                                         currentConnections_;
    //@}
};

}
}

#endif // __RightsPlugin_h_

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

    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    virtual void NotifyClientLeft( dispatcher::ClientPublisher_ABC& client );

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
    void OnReceiveMsgAuthenticationRequest( const std::string& link, const sword::AuthenticationRequest& message );
    void OnReceiveProfileCreationRequest( dispatcher::ClientPublisher_ABC& client, const sword::ProfileCreationRequest& message );
    void OnReceiveProfileUpdateRequest( dispatcher::ClientPublisher_ABC& client, const sword::ProfileUpdateRequest& message, unsigned int context );
    void OnReceiveProfileDestructionRequest( dispatcher::ClientPublisher_ABC& client, const sword::ProfileDestructionRequest& message );
    void OnReceiveConnectedProfilesRequest( dispatcher::ClientPublisher_ABC& client, int context, const sword::ConnectedProfilesRequest& message );

    bool IsAuthenticated( const std::string& login ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, dispatcher::Profile* > T_Profiles;
    typedef T_Profiles::iterator                             IT_Profiles;
    typedef T_Profiles::const_iterator                      CIT_Profiles;
    //@}

private:
    //! @name Member data
    //@{
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

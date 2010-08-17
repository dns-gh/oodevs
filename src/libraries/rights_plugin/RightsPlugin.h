// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __RightsPlugin_h_
#define __RightsPlugin_h_

#include "dispatcher/Plugin_ABC.h"
#include "dispatcher/LinkResolver_ABC.h"
#include <map>

namespace MsgsClientToAuthentication
{
    class MsgClientToAuthentication;
    class MsgAuthenticationRequest;
    class MsgProfileUpdateRequest;
    class MsgProfileDestructionRequest;
    class MsgProfileCreationRequest;
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
    virtual void Receive( const MsgsSimToClient::MsgSimToClient& message );

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
    void OnReceive( const std::string& link, const MsgsClientToAuthentication::MsgClientToAuthentication& message );
    void OnReceiveMsgAuthenticationRequest( const std::string& link, const MsgsClientToAuthentication::MsgAuthenticationRequest& message );
    void OnReceiveMsgProfileCreationRequest( dispatcher::ClientPublisher_ABC& client, const MsgsClientToAuthentication::MsgProfileCreationRequest& message );
    void OnReceiveMsgProfileUpdateRequest( dispatcher::ClientPublisher_ABC& client, const MsgsClientToAuthentication::MsgProfileUpdateRequest& message );
    void OnReceiveMsgProfileDestructionRequest( dispatcher::ClientPublisher_ABC& client, const MsgsClientToAuthentication::MsgProfileDestructionRequest& message );

    bool IsAuthenticated( const std::string& login ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, dispatcher::Profile_ABC* > T_Profiles;
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

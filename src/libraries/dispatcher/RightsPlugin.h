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

#include "Plugin_ABC.h"
#include "LinkResolver_ABC.h"



//using namespace Common;
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

// =============================================================================
/** @class  RightsPlugin
    @brief  RightsPlugin
*/
// Created: AGE 2007-08-24
// =============================================================================
class RightsPlugin : public Plugin_ABC
                   , public LinkResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             RightsPlugin( Model& model, ClientPublisher_ABC& clients, const Config& config, tools::MessageDispatcher_ABC& clientCommands, Plugin_ABC& container, LinkResolver_ABC& base, dispatcher::CompositeRegistrable& registrables, int maxConnections );
    virtual ~RightsPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const MsgsSimToClient::MsgSimToClient& message );
    virtual void NotifyClientAuthenticated( ClientPublisher_ABC& client, Profile_ABC& profile );
    virtual void NotifyClientLeft         ( ClientPublisher_ABC& client );
    virtual void Register                 ( dispatcher::Services& );

    virtual Profile_ABC&         GetProfile  ( const std::string& link );
    virtual ClientPublisher_ABC& GetPublisher( const std::string& link );
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
    void OnReceiveMsgAuthenticationRequest    ( const std::string& link, const MsgsClientToAuthentication::MsgAuthenticationRequest& message );
    void OnReceiveMsgProfileCreationRequest   ( ClientPublisher_ABC& client, const MsgsClientToAuthentication::MsgProfileCreationRequest&    message );
    void OnReceiveMsgProfileUpdateRequest     ( ClientPublisher_ABC& client, const MsgsClientToAuthentication::MsgProfileUpdateRequest&      message );
    void OnReceiveMsgProfileDestructionRequest( ClientPublisher_ABC& client, const MsgsClientToAuthentication::MsgProfileDestructionRequest& message );
    bool IsAuthenticated( const std::string& login ) const;
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, Profile_ABC* > T_Profiles;
    typedef T_Profiles::iterator                 IT_Profiles;
    typedef T_Profiles::const_iterator          CIT_Profiles;
    //@}

private:
    //! @name Member data
    //@{
    Plugin_ABC& container_;
    LinkResolver_ABC& base_;
    std::auto_ptr< ProfileManager >  profiles_;
    T_Profiles                       authenticated_;
    int                              maxConnections_;
    int                              currentConnections_;
    //@}
};

}

#endif // __RightsPlugin_h_

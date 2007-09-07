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

namespace tools
{
    class MessageDispatcher_ABC;
}

namespace dispatcher
{
    class ProfileManager;
    class Model;
    class Config;

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
             RightsPlugin( Model& model, ClientPublisher_ABC& clients, const Config& config, tools::MessageDispatcher_ABC& clientCommands, Plugin_ABC& container, LinkResolver_ABC& base );
    virtual ~RightsPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const ASN1T_MsgsSimToClient& message );
    virtual void NotifyClientAuthenticated( ClientPublisher_ABC& client, Profile_ABC& profile );
    virtual void NotifyClientLeft         ( ClientPublisher_ABC& client );

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
    void OnReceive( const std::string& link, const ASN1T_MsgsClientToAuthentication& message );
    void OnReceiveMsgAuthenticationRequest    ( const std::string& link, const ASN1T_MsgAuthenticationRequest& message );
    void OnReceiveMsgProfileCreationRequest   ( ClientPublisher_ABC& client, const ASN1T_MsgProfileCreationRequest&    message );
    void OnReceiveMsgProfileUpdateRequest     ( ClientPublisher_ABC& client, const ASN1T_MsgProfileUpdateRequest&      message );
    void OnReceiveMsgProfileDestructionRequest( ClientPublisher_ABC& client, const ASN1T_MsgProfileDestructionRequest& message );
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
    //@}
};

}

#endif // __RightsPlugin_h_

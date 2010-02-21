// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ProfileManager_h_
#define __ProfileManager_h_

#include "MessageHandler_ABC.h"
#include "Registrable_ABC.h"

namespace MsgsSimToClient
{
    class MsgSimToClient;
}
namespace MsgsAuthenticationToClient
{
    enum MsgProfileCreationRequestAck_ErrorCode;
    enum MsgProfileUpdateRequestAck_ErrorCode;
    enum MsgProfileDestructionRequestAck_ErrorCode;
}

namespace MsgsClientToAuthentication
{
    class MsgProfileCreationRequest;
    class MsgProfileDestructionRequest;
    class MsgProfileUpdateRequest;
}

namespace xml{ class xistream; };

namespace dispatcher
{
class Config;
class Profile;
class ClientPublisher_ABC;
class Model;
class ClientsNetworker;

// =============================================================================
/** @class  ProfileManager
    @brief  Profile manager
*/
// Created: NLD 2006-09-19
// =============================================================================
class ProfileManager : public MessageHandler_ABC
                     , public dispatcher::Registrable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ProfileManager( Model& model, ClientPublisher_ABC& clients, const Config& config );
    virtual ~ProfileManager();
    //@}

    //! @name Operations
    //@{
    void Reset();
    Profile* Authenticate( const std::string& strName, const std::string& strPassword ) const;

    void Send( ClientPublisher_ABC& publisher ) const;
    virtual void Receive( const MsgsSimToClient::MsgSimToClient& message );

    MsgsAuthenticationToClient::MsgProfileCreationRequestAck_ErrorCode    Create ( const MsgsClientToAuthentication::MsgProfileCreationRequest&    message );
    MsgsAuthenticationToClient::MsgProfileUpdateRequestAck_ErrorCode      Update ( const MsgsClientToAuthentication::MsgProfileUpdateRequest&      message );
    MsgsAuthenticationToClient::MsgProfileDestructionRequestAck_ErrorCode Destroy( const MsgsClientToAuthentication::MsgProfileDestructionRequest& message );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ProfileManager( const ProfileManager& );            //!< Copy constructor
    ProfileManager& operator=( const ProfileManager& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ReadProfile( xml::xistream& xis );

    virtual void RegisterIn( directia::Brain& brain );
    void SetAutomatRight( const std::string& profile, unsigned int automat, bool readonly, bool readwrite );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, Profile* >   T_ProfileMap;
    typedef T_ProfileMap::const_iterator      CIT_ProfileMap;
    //@}

private:
    const Config&        config_;
    Model&               model_;
    ClientPublisher_ABC& clients_;
    T_ProfileMap         profiles_;
};

}

#endif // __ProfileManager_h_

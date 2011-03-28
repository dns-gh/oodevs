// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ProfileManager_h_
#define __ProfileManager_h_

#include "MessageHandler_ABC.h"
#include "Registrable_ABC.h"

namespace sword
{
    class SimToClient;
    enum  ProfileCreationRequestAck_ErrorCode;
    enum  ProfileUpdateRequestAck_ErrorCode;
    enum  ProfileDestructionRequestAck_ErrorCode;
    class AuthenticationResponse;
    class ProfileCreationRequest;
    class ProfileDestructionRequest;
    class ProfileUpdateRequest;
}

namespace tools
{
    class SchemaWriter;
}

namespace xml
{
    class xistream;
}

namespace dispatcher
{
    class Config;
    class Profile;
    class ClientPublisher_ABC;
    class Model;

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
    void Send( sword::AuthenticationResponse& message ) const;
    virtual void Receive( const sword::SimToClient& message );

    sword::ProfileCreationRequestAck_ErrorCode    Create ( const sword::ProfileCreationRequest&    message );
    sword::ProfileUpdateRequestAck_ErrorCode      Update ( const sword::ProfileUpdateRequest&      message );
    sword::ProfileDestructionRequestAck_ErrorCode Destroy( const sword::ProfileDestructionRequest& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ProfileManager( const ProfileManager& );            //!< Copy constructor
    ProfileManager& operator=( const ProfileManager& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadProfiles( xml::xistream& xis );
    void ReadProfile( xml::xistream& xis );
    void Save( const std::string& path );
    virtual void RegisterIn( directia::brain::Brain& brain );
    void SetAutomatRight( const std::string& profile, unsigned int automat, bool readonly, bool readwrite );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, Profile* > T_ProfileMap;
    typedef T_ProfileMap::const_iterator    CIT_ProfileMap;
    //@}

private:
    const Config& config_;
    Model& model_;
    ClientPublisher_ABC& clients_;
    T_ProfileMap profiles_;
    std::auto_ptr< tools::SchemaWriter > pSchemaWriter_;
};

}

#endif // __ProfileManager_h_

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
#include <boost/shared_ptr.hpp>

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
    class Path;
}

namespace xml
{
    class xistream;
}

namespace dispatcher
{
    class Config;
    class Profile_ABC;
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
    boost::shared_ptr< Profile > Authenticate( const std::string& strName,
            const std::string& strPassword, const std::string& link, bool keyAuthenticated ) const;

    void Send( ClientPublisher_ABC& publisher ) const;
    void Send( sword::AuthenticationResponse& message ) const;
    virtual void Receive( const sword::SimToClient& message );

    sword::ProfileCreationRequestAck_ErrorCode    Create ( const sword::ProfileCreationRequest&    message );
    sword::ProfileUpdateRequestAck_ErrorCode      Update ( const sword::ProfileUpdateRequest&      message, const Profile_ABC& requester );
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
    void Save( const tools::Path& path );
    virtual void RegisterIn( directia::brain::Brain& brain );
    void SetAutomatRight( const std::string& profile, unsigned int automat, bool readonly, bool readwrite );
    //@}

private:
    const Config& config_;
    Model& model_;
    ClientPublisher_ABC& clients_;
    std::map< std::string, boost::shared_ptr< Profile > > profiles_;
    std::auto_ptr< tools::SchemaWriter > pSchemaWriter_;
    boost::shared_ptr< Profile > supervisor_;
};

}

#endif // __ProfileManager_h_

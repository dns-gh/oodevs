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

#include "game_asn/Asn.h"

namespace xml{ class xistream; };

namespace dispatcher
{
class Config;
class Profile;
class Publisher_ABC;
class Model;
class ClientsNetworker;

// =============================================================================
/** @class  ProfileManager
    @brief  Profile manager
*/
// Created: NLD 2006-09-19
// =============================================================================
class ProfileManager
{
public:
    //! @name Constructors/Destructor
    //@{
             ProfileManager( Model& model, ClientsNetworker& clients, const Config& config );
    virtual ~ProfileManager();
    //@}

    //! @name Operations
    //@{
    void     Reset       ();
    Profile* Authenticate( const std::string& strName, const std::string& strPassword ) const;
    Profile* Find        ( const std::string& strName ) const;

    void Send( Publisher_ABC& publisher ) const;

    ASN1T_MsgProfileCreationRequestAck_error_code    Create ( const ASN1T_MsgProfileCreationRequest&    message );
    ASN1T_MsgProfileUpdateRequestAck_error_code      Update ( const ASN1T_MsgProfileUpdateRequest&      message );
    ASN1T_MsgProfileDestructionRequestAck_error_code Destroy( const ASN1T_MsgProfileDestructionRequest& message );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ProfileManager( const ProfileManager& );            //!< Copy constructor
    ProfileManager& operator=( const ProfileManager& ); //!< Assignement operator
    //@}

    //! @name Tools
    //@{
    void ReadProfile( xml::xistream& xis );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, Profile* > T_ProfileMap;
    typedef T_ProfileMap::const_iterator      CIT_ProfileMap;
    //@}

private:
    const Config&           config_;
          Model&            model_;
          ClientsNetworker& clients_;
          T_ProfileMap      profiles_;
};

}

#endif // __ProfileManager_h_

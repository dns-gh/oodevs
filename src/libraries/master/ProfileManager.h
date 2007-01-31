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

#include "ModelsContainer.h"
#include "Profile.h"

namespace xml{ class xistream; };

namespace master
{
class Master;
class Config;

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
             ProfileManager( Master& master, const Config& config );
    virtual ~ProfileManager();
    //@}

    //! @name Operations
    //@{
//    void     Reset       ();
    Profile* Authenticate( const std::string& strName, const std::string& strPassword ) const;
    Profile* Find        ( const std::string& strName ) const;

//    void Send( Publisher_ABC& publisher ) const;
//    ASN1T_MsgProfileCreationRequestAck_error_code    Create ( const ASN1T_MsgProfileCreationRequest&    message );
//    ASN1T_MsgProfileUpdateRequestAck_error_code      Update ( const ASN1T_MsgProfileUpdateRequest&      message );
//    ASN1T_MsgProfileDestructionRequestAck_error_code Destroy( const ASN1T_MsgProfileDestructionRequest& message );
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
    Master&                                 master_;
    ModelsContainer< std::string, Profile > profiles_;
};

}

#endif // __ProfileManager_h_

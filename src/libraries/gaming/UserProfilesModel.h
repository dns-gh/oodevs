// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UserProfilesModel_h_
#define __UserProfilesModel_h_

#include <boost/noncopyable.hpp>

namespace sword
{
    class ProfileCreation;
    class ProfileDestruction;
}

class UserProfile;
class UserProfileFactory_ABC;

// =============================================================================
/** @class  UserProfilesModel
    @brief  UserProfilesModel
*/
// Created: SBO 2007-01-19
// =============================================================================
class UserProfilesModel : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit UserProfilesModel( const UserProfileFactory_ABC& factory );
    virtual ~UserProfilesModel();
    //@}

    //! @name Operations
    //@{
    void CreateProfile( const sword::ProfileCreation& message );
    void DeleteProfile( const sword::ProfileDestruction& message );
    UserProfile& Get( const QString& login );
    const UserProfile* Find( const QString& login ) const;
    void Purge();
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< UserProfile* > T_UserProfiles;
    //@}

private:
    //! @name Member data
    //@{
    const UserProfileFactory_ABC& factory_;
    T_UserProfiles userProfiles_;
    //@}
};

#endif // __UserProfilesModel_h_

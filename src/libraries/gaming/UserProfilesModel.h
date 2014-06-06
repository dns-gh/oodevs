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

#include "clients_kernel/ProfilesModel_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class UserProfile_ABC;
    class ProfileFactory_ABC;
}

namespace sword
{
    class ProfileCreation;
    class ProfileDestruction;
}

// =============================================================================
/** @class  UserProfilesModel
    @brief  UserProfilesModel
*/
// Created: SBO 2007-01-19
// =============================================================================
class UserProfilesModel : public kernel::ProfilesModel_ABC
                        , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit UserProfilesModel( const kernel::ProfileFactory_ABC& factory );
    virtual ~UserProfilesModel();
    //@}

    //! @name From ProfilesModel_ABC
    //@{
    virtual void CommitFromEditor( kernel::ProfileEditor& editor );
    virtual kernel::ProfileEditor* CreateProfileEditor() const;
    virtual kernel::ProfileEditor* CreateProfileEditor( kernel::UserProfile_ABC& profile ) const;
    virtual void Apply( boost::function< void( kernel::UserProfile_ABC& ) > functor );
    //@}

    //! @name Operations
    //@{
    void CreateProfile( const sword::ProfileCreation& message );
    void DeleteProfile( const sword::ProfileDestruction& message );
    kernel::UserProfile_ABC& Get( const QString& login );
    const kernel::UserProfile_ABC* Find( const QString& login ) const;
    void Purge();
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< kernel::UserProfile_ABC* > T_UserProfiles;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::ProfileFactory_ABC& factory_;
    T_UserProfiles userProfiles_;
    //@}
};

#endif // __UserProfilesModel_h_

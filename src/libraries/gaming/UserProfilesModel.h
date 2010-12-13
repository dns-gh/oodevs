// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __UserProfilesModel_h_
#define __UserProfilesModel_h_

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
class UserProfilesModel
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
    //! @name Copy/Assignment
    //@{
    UserProfilesModel( const UserProfilesModel& );            //!< Copy constructor
    UserProfilesModel& operator=( const UserProfilesModel& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< UserProfile* >      T_UserProfiles;
    typedef T_UserProfiles::const_iterator CIT_UserProfiles;
    //@}

private:
    //! @name Member data
    //@{
    const UserProfileFactory_ABC& factory_;
    T_UserProfiles userProfiles_;
    //@}
};

#endif // __UserProfilesModel_h_

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "UserProfilesModel.h"
#include "UserProfile.h"
#include "clients_kernel/ProfileFactory_ABC.h"
#include "clients_kernel/ProfileEditor.h"
#include "clients_kernel/Tools.h"

// -----------------------------------------------------------------------------
// Name: UserProfilesModel constructor
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
UserProfilesModel::UserProfilesModel( const kernel::ProfileFactory_ABC& factory )
    : factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfilesModel destructor
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
UserProfilesModel::~UserProfilesModel()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: UserProfilesModel::CommitFromEditor
// Created: JSR 2014-06-06
// -----------------------------------------------------------------------------
void UserProfilesModel::CommitFromEditor( kernel::ProfileEditor& editor )
{
    const kernel::UserProfile_ABC* originalProfile = editor.GetOriginalProfile();
    const UserProfile& profile = static_cast< UserProfile& >( editor.GetProfile() );
    if( editor.IsDeleted() )
    {
        if( originalProfile )
            profile.RequestDeletion();
    }
    else if( originalProfile == 0 )
    {
        profile.RequestCreation();
        profile.RequestUpdate( profile.GetLogin() );
    }
    else if( profile != *originalProfile )
        profile.RequestUpdate( originalProfile->GetLogin() );
}

// -----------------------------------------------------------------------------
// Name: UserProfilesModel::CreateProfileEditor
// Created: JSR 2014-06-06
// -----------------------------------------------------------------------------
kernel::ProfileEditor* UserProfilesModel::CreateProfileEditor() const
{
    return new kernel::ProfileEditor( factory_.Create(), 0 );
}

// -----------------------------------------------------------------------------
// Name: UserProfilesModel::CreateProfileEditor
// Created: JSR 2014-06-06
// -----------------------------------------------------------------------------
kernel::ProfileEditor* UserProfilesModel::CreateProfileEditor( kernel::UserProfile_ABC& profile ) const
{
    return new kernel::ProfileEditor( factory_.Create( profile ), &profile );
}

// -----------------------------------------------------------------------------
// Name: UserProfilesModel::function< void
// Created: JSR 2014-06-06
// -----------------------------------------------------------------------------
void UserProfilesModel::Apply( boost::function< void( kernel::UserProfile_ABC& ) > functor )
{
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        functor( **it );
}

// -----------------------------------------------------------------------------
// Name: UserProfilesModel::Purge
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void UserProfilesModel::Purge()
{
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        delete *it;
    userProfiles_.clear();
}

// -----------------------------------------------------------------------------
// Name: UserProfilesModel::CreateProfile
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void UserProfilesModel::CreateProfile( const sword::ProfileCreation& message )
{
    std::unique_ptr< kernel::UserProfile_ABC > profile( factory_.Create( message ) );
    userProfiles_.push_back( profile.release() );
}

// -----------------------------------------------------------------------------
// Name: UserProfilesModel::DeleteProfile
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void UserProfilesModel::DeleteProfile( const sword::ProfileDestruction& message )
{
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        if( message.login() == (*it)->GetLogin().toStdString() )
        {
            const kernel::UserProfile_ABC* element = *it;
            userProfiles_.erase( it );
            delete element;
            return;
        }
}

// -----------------------------------------------------------------------------
// Name: UserProfilesModel::Find
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
const kernel::UserProfile_ABC* UserProfilesModel::Find( const QString& login ) const
{
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        if( login == (*it)->GetLogin() )
            return *it;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: UserProfilesModel::Get
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
kernel::UserProfile_ABC& UserProfilesModel::Get( const QString& login )
{
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        if( login == (*it)->GetLogin() )
            return **it;
    throw MASA_EXCEPTION( tools::translate( "UserProfilesModel", "Profile '%1' does not exist." ).arg( login ).toStdString() );
}

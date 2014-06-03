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
#include "UserProfileFactory_ABC.h"
#include "UserProfile.h"
#include "clients_kernel/Tools.h"

// -----------------------------------------------------------------------------
// Name: UserProfilesModel constructor
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
UserProfilesModel::UserProfilesModel( const UserProfileFactory_ABC& factory )
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
    std::unique_ptr< UserProfile > profile( factory_.Create( message ) );
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
            const UserProfile* element = *it;
            userProfiles_.erase( it );
            delete element;
            return;
        }
}

// -----------------------------------------------------------------------------
// Name: UserProfilesModel::Find
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
const UserProfile* UserProfilesModel::Find( const QString& login ) const
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
UserProfile& UserProfilesModel::Get( const QString& login )
{
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        if( login == (*it)->GetLogin() )
            return **it;
    throw MASA_EXCEPTION( tools::translate( "UserProfilesModel", "Profile '%1' does not exist." ).arg( login ).toStdString() );
}

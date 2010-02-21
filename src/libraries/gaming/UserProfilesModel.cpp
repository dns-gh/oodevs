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
#include "Tools.h"

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
    for( CIT_UserProfiles it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        delete *it;
    userProfiles_.clear();
}

// -----------------------------------------------------------------------------
// Name: UserProfilesModel::CreateProfile
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void UserProfilesModel::CreateProfile( const MsgsAuthenticationToClient::MsgProfileCreation& message )
{
    std::auto_ptr< UserProfile > profile( factory_.Create( message ) );
    userProfiles_.push_back( profile.release() );
}

// -----------------------------------------------------------------------------
// Name: UserProfilesModel::DeleteProfile
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void UserProfilesModel::DeleteProfile( const MsgsAuthenticationToClient::MsgProfileDestruction& message )
{
    for( T_UserProfiles::iterator it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        if( message.login().c_str() == (*it)->GetLogin().ascii() )
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
    for( T_UserProfiles::const_iterator it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
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
    for( T_UserProfiles::const_iterator it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        if( login == (*it)->GetLogin() )
            return **it;
    throw std::runtime_error( tools::translate( "UserProfilesModel", "Profile '%1' does not exist." ).arg( login ).ascii() );
}

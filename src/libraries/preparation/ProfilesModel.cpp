// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "ProfilesModel.h"
#include "UserProfile.h"
#include "ProfileFactory_ABC.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: ProfilesModel constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
ProfilesModel::ProfilesModel( const ProfileFactory_ABC& factory )
    : factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel destructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
ProfilesModel::~ProfilesModel()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::Purge
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void ProfilesModel::Purge()
{
    for( CIT_UserProfiles it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        delete *it;
    userProfiles_.clear();
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::Load
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void ProfilesModel::Load( const std::string& file )
{
    xifstream xis( file );

    xis >> start( "profiles" )
        >> list( "profile", *this, &ProfilesModel::LoadProfile );
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::Serialize
// Created: SBO 2007-01-17
// -----------------------------------------------------------------------------
void ProfilesModel::Serialize( const std::string& file ) const
{
    xofstream xos( file, xml::encoding( "ISO-8859-1" ) );
    xos << start( "profiles" );
    for( CIT_UserProfiles it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        (*it)->Serialize( xos );
    xos << end();
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::LoadProfile
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void ProfilesModel::LoadProfile( xml::xistream& xis )
{
    std::auto_ptr< UserProfile > profile( factory_.Create( xis ) );
    userProfiles_.push_back( profile.release() );
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::CreateProfile
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void ProfilesModel::CreateProfile()
{
    std::auto_ptr< UserProfile > profile( factory_.Create() );
    userProfiles_.push_back( profile.release() );
}
    
// -----------------------------------------------------------------------------
// Name: ProfilesModel::DeleteProfile
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void ProfilesModel::DeleteProfile( const UserProfile& profile )
{
    T_UserProfiles::iterator it = std::find( userProfiles_.begin(), userProfiles_.end(), &profile );
    if( it != userProfiles_.end() )
    {
        const UserProfile* element = *it;
        userProfiles_.erase( it );
        delete element;
    }
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::Exists
// Created: SBO 2007-01-17
// -----------------------------------------------------------------------------
bool ProfilesModel::Exists( const QString& login ) const
{
    for( CIT_UserProfiles it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        if( login == (*it)->GetLogin() )
            return true;
    return false;
}

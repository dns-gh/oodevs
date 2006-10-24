// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "ProfileManager.h"

#include "Profile.h"
#include "xeumeuleu/xml.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ProfileManager constructor
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
ProfileManager::ProfileManager( Dispatcher& dispatcher, const std::string& strFile )
    : dispatcher_( dispatcher )
    , strFile_   ( strFile )
{

}

// -----------------------------------------------------------------------------
// Name: ProfileManager destructor
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
ProfileManager::~ProfileManager()
{

}

// =============================================================================
// MAIN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ProfileManager::ReadProfile
// Created: NLD 2006-10-06
// -----------------------------------------------------------------------------
void ProfileManager::ReadProfile( xml::xistream& xis )
{
    std::string strName;
    xis >> xml::attribute( "name", strName );

    Profile*& pProfile = profiles_[ strName ];
    if( pProfile )
    {
        MT_LOG_ERROR_MSG( "Profile '" << strName << "' already exists - new profile ignored" );
    }
    else
    {
        MT_LOG_INFO_MSG( "New profile loaded : '" << strName << "'" );
        pProfile = new Profile( dispatcher_, strName, xis );
    }
}

// -----------------------------------------------------------------------------
// Name: ProfileManager::Reset
// Created: NLD 2006-10-06
// -----------------------------------------------------------------------------
void ProfileManager::Reset()
{
    MT_LOG_INFO_MSG( "Loading profiles" );
    for( CIT_ProfileMap it = profiles_.begin(); it != profiles_.end(); ++it )
        delete it->second;
    profiles_.clear();

    try
    {
        xml::xifstream xis( strFile_ );
        xis >> xml::start( "profiles" )
                >> xml::list( "profile", *this, & ProfileManager::ReadProfile )
            >> xml::end();
    }
    catch( xml::exception& exception )
    {
        MT_LOG_ERROR_MSG( "Error while loading profiles : " << exception.what() );
//$$$$$
    }
}

// -----------------------------------------------------------------------------
// Name: ProfileManager::Authenticate
// Created: NLD 2006-10-06
// -----------------------------------------------------------------------------
Profile* ProfileManager::Authenticate( const std::string& strName, const std::string& strPassword )
{
    CIT_ProfileMap it = profiles_.find( strName );
    if( it == profiles_.end() )
    {
        MT_LOG_INFO_MSG( "Auth - Profile '" << strName << "' doesn't exists" );
        return 0;
    }

    if( !it->second->CheckPassword( strPassword ) )
    {
        MT_LOG_INFO_MSG( "Auth - Profile '" << strName << "' invalid password" );
        return 0;
    }

    MT_LOG_INFO_MSG( "Auth - Profile '" << strName << "' authenticated" );
    return it->second;
}


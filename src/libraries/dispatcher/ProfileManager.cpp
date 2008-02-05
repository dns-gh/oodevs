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
#include "Config.h"
#include "Client.h"
#include "xeumeuleu/xml.h"
#include "MT/MT_Logger/MT_Logger_lib.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ProfileManager constructor
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
ProfileManager::ProfileManager( Model& model, ClientPublisher_ABC& clients, const Config& config )
    : model_   ( model )
    , clients_ ( clients )
    , config_  ( config )
    , profiles_()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProfileManager destructor
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
ProfileManager::~ProfileManager()
{
    // NOTHING
}

// =============================================================================
// MAIN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ProfileManager::Receive
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
void ProfileManager::Receive( const ASN1T_MsgsSimToClient& message )
{
    if( message.msg.t == T_MsgsSimToClient_msg_msg_control_send_current_state_end )
        Reset();
}

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
        pProfile = new Profile( model_, clients_, strName, xis );
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
        xml::xifstream xis( config_.GetProfilesFile() );
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
Profile* ProfileManager::Authenticate( const std::string& strName, const std::string& strPassword ) const
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

// -----------------------------------------------------------------------------
// Name: ProfileManager::Send
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
void ProfileManager::Send( ClientPublisher_ABC& publisher ) const
{
    for( CIT_ProfileMap it = profiles_.begin(); it != profiles_.end(); ++it )
        it->second->SendCreation( publisher );
}

// -----------------------------------------------------------------------------
// Name: ProfileManager::Create
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
ASN1T_MsgProfileCreationRequestAck_error_code ProfileManager::Create( const ASN1T_MsgProfileCreationRequest& message )
{
    const std::string login = message.login;
    if( login.empty() )
        return MsgProfileCreationRequestAck_error_code::invalid_login;
    // $$$$ SBO 2007-01-22: check password if needed (maybe add a way to specify password constraints...)
    Profile*& pProfile = profiles_[ login ];
    if( pProfile )
        return MsgProfileCreationRequestAck_error_code::duplicate_login;
    MT_LOG_INFO_MSG( "New profile created : '" << login << "'" );
    pProfile = new Profile( model_, clients_, message );
    return MsgProfileCreationRequestAck_error_code::success;
}

// -----------------------------------------------------------------------------
// Name: ProfileManager::Update
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
ASN1T_MsgProfileUpdateRequestAck_error_code ProfileManager::Update( const ASN1T_MsgProfileUpdateRequest& message )
{
    Profile*& pProfile = profiles_[ message.login ];
    if( !pProfile )
        return MsgProfileUpdateRequestAck_error_code::invalid_profile;
    const std::string newLogin = message.profile.login;
    if( newLogin.empty() )
        return MsgProfileUpdateRequestAck_error_code::invalid_login;
    if( newLogin != message.login && profiles_.find( newLogin ) != profiles_.end() )
        return MsgProfileUpdateRequestAck_error_code::duplicate_login;
    // $$$$ SBO 2007-01-22: check password id needed
    pProfile->Update( message );
    if( newLogin != message.login )
    {
        profiles_[ newLogin ] = pProfile;
        profiles_.erase( message.login );
    }
    return MsgProfileUpdateRequestAck_error_code::success;
}

// -----------------------------------------------------------------------------
// Name: ProfileManager::Destroy
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
ASN1T_MsgProfileDestructionRequestAck_error_code ProfileManager::Destroy( const ASN1T_MsgProfileDestructionRequest& message )
{
    T_ProfileMap::iterator it = profiles_.find( message );
    if( it == profiles_.end() )
        return MsgProfileDestructionRequestAck_error_code::invalid_profile;
    delete it->second;
    profiles_.erase( it );
    return MsgProfileDestructionRequestAck_error_code::success;
}

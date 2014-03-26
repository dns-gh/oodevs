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
#include "Supervisor.h"
#include "Config.h"
#include "Model.h"
#include "Automat.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_Scipio_enum.h"
#include "tools/FileWrapper.h"
#include "tools/Loader_ABC.h"
#include "tools/SchemaWriter.h"
#include "protocol/Protocol.h"
#include "protocol/AuthenticationSenders.h"
#include <directia/brain/Brain.h>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <xeumeuleu/xml.h>

using namespace dispatcher;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ProfileManager constructor
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
ProfileManager::ProfileManager( Model& model, ClientPublisher_ABC& clients, const Config& config )
    : model_        ( model )
    , clients_      ( clients )
    , config_       ( config )
    , profiles_     ()
    , pSchemaWriter_( new tools::SchemaWriter() )
    , supervisor_   ( boost::make_shared< Supervisor >( model, clients ) )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProfileManager destructor
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
ProfileManager::~ProfileManager()
{
    for( auto it = profiles_.begin(); it != profiles_.end(); ++it )
        it->second->SendDestruction();
}

// -----------------------------------------------------------------------------
// Name: ProfileManager::Receive
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
void ProfileManager::Receive( const sword::SimToClient& wrapper )
{
    if( wrapper.message().has_control_send_current_state_end() )
        Reset();
    if( wrapper.message().has_control_checkpoint_save_end() )
    {
        try
        {
            tools::Path strPath = config_.GetCheckpointDirectory( tools::Path::FromUTF8( wrapper.message().control_checkpoint_save_end().name() ) );
            MT_LOG_INFO_MSG( "Begin save checkpoint " << strPath.ToUTF8() );
            if( !strPath.Exists() )
                strPath.CreateDirectories();
            Save( strPath );
        }
        catch( const std::exception& e )
        {
            MT_LOG_ERROR_MSG( "Can't save checkpoint ( '" << tools::GetExceptionMsg( e ) << "' )" );
        }
        catch( ... )
        {
            MT_LOG_ERROR_MSG( "Can't save checkpoint ( Unknown error )" );
        }
        MT_LOG_INFO_MSG( "End save checkpoint" );
    }
}

// -----------------------------------------------------------------------------
// Name: ProfileManager::Save
// Created: LGY 2011-03-25
// -----------------------------------------------------------------------------
void ProfileManager::Save( const tools::Path& path )
{
    tools::Xofstream xos( config_.BuildDirectoryFile( path, "profiles.xml" ) );
    xos << xml::start( "profiles" );
    pSchemaWriter_->WriteExerciseSchema( xos, "profiles" );
    for( auto it = profiles_.begin(); it != profiles_.end(); ++it )
        it->second->SerializeProfile( xos );
}

// -----------------------------------------------------------------------------
// Name: ProfileManager::ReadProfiles
// Created: LGY 2011-03-28
// -----------------------------------------------------------------------------
void ProfileManager::ReadProfiles( xml::xistream& xis )
{
    xis >> xml::start( "profiles" )
            >> xml::list( "profile", *this, & ProfileManager::ReadProfile )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ProfileManager::ReadProfile
// Created: NLD 2006-10-06
// -----------------------------------------------------------------------------
void ProfileManager::ReadProfile( xml::xistream& xis )
{
    std::string strName;
    xis >> xml::attribute( "name", strName );

    if( profiles_.find( strName ) == profiles_.end() )
    {
        try
        {
            auto profile = boost::make_shared< Profile >( model_, clients_,
                strName, xis );
            profiles_[ strName ] = profile;
            MT_LOG_INFO_MSG( "New profile loaded : '" << strName << "'" );
        }
        catch( const xml::exception& )
        {
            MT_LOG_INFO_MSG( "Invalid profile ignored : '" << strName << "'" );
        }
    }
    else
        MT_LOG_ERROR_MSG( "Profile '" << strName << "' already exists - new profile ignored" );
}

// -----------------------------------------------------------------------------
// Name: ProfileManager::Reset
// Created: NLD 2006-10-06
// -----------------------------------------------------------------------------
void ProfileManager::Reset()
{
    MT_LOG_INFO_MSG( "Loading profiles" );
    for( auto it = profiles_.begin(); it != profiles_.end(); ++it )
        it->second->SendDestruction();
    profiles_.clear();

    try
    {
        if( config_.HasCheckpoint() )
            config_.GetLoader().LoadFile( config_.GetCheckpointDirectory() / "profiles.xml", boost::bind( &ProfileManager::ReadProfiles, this, _1 ) );
        else
            config_.GetLoader().LoadFile( config_.GetProfilesFile(), boost::bind( &ProfileManager::ReadProfiles, this, _1 ) );
    }
    catch( const xml::exception& e )
    {
        MT_LOG_ERROR_MSG( "Error while loading profiles : " << tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ProfileManager::Authenticate
// Created: NLD 2006-10-06
// -----------------------------------------------------------------------------
boost::shared_ptr< Profile > ProfileManager::Authenticate( const std::string& strName,
        const std::string& strPassword, const std::string& link, bool keyAuthenticated ) const
{
    if( keyAuthenticated && strName.empty() && strPassword.empty() )
        return supervisor_;

    auto it = profiles_.find( strName );
    if( it == profiles_.end() )
    {
        MT_LOG_INFO_MSG( "Auth - Profile '" << strName << "' doesn't exists ("
            << link << ")" );
        return boost::shared_ptr< Profile >();
    }

    if( !it->second->CheckPassword( strPassword ) )
    {
        MT_LOG_INFO_MSG( "Auth - Profile '" << strName << "' invalid password ("
            << link << ")" );
        return boost::shared_ptr< Profile >();
    }

    MT_LOG_INFO_MSG( "Auth - Profile '" << strName << "' authenticated ("
        << link << ")" );
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: ProfileManager::Send
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
void ProfileManager::Send( ClientPublisher_ABC& publisher ) const
{
    for( auto it = profiles_.begin(); it != profiles_.end(); ++it )
        it->second->SendCreation( publisher );
}

// -----------------------------------------------------------------------------
// Name: ProfileManager::Send
// Created: SBO 2009-12-18
// -----------------------------------------------------------------------------
void ProfileManager::Send( sword::AuthenticationResponse& message ) const
{
    for( auto it = profiles_.begin(); it != profiles_.end(); ++it )
        it->second->Send( *message.mutable_profiles()->add_elem() );
}

// -----------------------------------------------------------------------------
// Name: ProfileManager::Create
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
sword::ProfileCreationRequestAck_ErrorCode ProfileManager::Create( const sword::ProfileCreationRequest& message )
{
    const std::string login = message.has_profile() ? message.profile().login():"";
    if( login.empty() )
        return sword::ProfileCreationRequestAck::invalid_login;
    // $$$$ SBO 2007-01-22: check password if needed (maybe add a way to specify password constraints...)
    if( profiles_.find( login ) != profiles_.end() )
        return sword::ProfileCreationRequestAck::duplicate_login;
    MT_LOG_INFO_MSG( "New profile created : '" << login << "'" );
    auto profile = boost::make_shared< Profile >( model_, clients_, message );
    profiles_[ login ] = profile;
    return sword::ProfileCreationRequestAck::success;
}

// -----------------------------------------------------------------------------
// Name: ProfileManager::Update
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
sword::ProfileUpdateRequestAck_ErrorCode ProfileManager::Update( const sword::ProfileUpdateRequest& message, const Profile_ABC& requester )
{
    auto it = profiles_.find( message.login() );
    if( it == profiles_.end() )
        return sword::ProfileUpdateRequestAck::invalid_profile;
    const std::string newLogin = message.profile().login();
    if( newLogin.empty() )
        return sword::ProfileUpdateRequestAck::invalid_login;
    if( newLogin != message.login() && profiles_.find( newLogin ) != profiles_.end() )
        return sword::ProfileUpdateRequestAck::duplicate_login;
    // $$$$ SBO 2007-01-22: check password id needed
    auto pProfile = it->second;
    if( !pProfile->Update( message, requester ) )
        return sword::ProfileUpdateRequestAck::forbidden;
    if( newLogin != message.login() )
    {
        profiles_[ newLogin ] = pProfile;
        profiles_.erase( message.login() );
    }
    return sword::ProfileUpdateRequestAck::success;
}

// -----------------------------------------------------------------------------
// Name: ProfileManager::Destroy
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
sword::ProfileDestructionRequestAck_ErrorCode ProfileManager::Destroy( const sword::ProfileDestructionRequest& message )
{
    auto it = profiles_.find( message.login() );
    if( it == profiles_.end() )
        return sword::ProfileDestructionRequestAck::invalid_profile;
    it->second->SendDestruction();
    profiles_.erase( it );
    return sword::ProfileDestructionRequestAck::success;
}

namespace directia
{
    void UsedByDIA( ProfileManager* ) {}
    void ReleasedByDIA( ProfileManager* ) {}
}

// -----------------------------------------------------------------------------
// Name: ProfileManager::RegisterIn
// Created: SBO 2008-07-24
// -----------------------------------------------------------------------------
void ProfileManager::RegisterIn( directia::brain::Brain& brain )
{
    brain[ "profiles" ] = this;
    brain.Register( "SetAutomatRight", &ProfileManager::SetAutomatRight );
}

// -----------------------------------------------------------------------------
// Name: ProfileManager::SetAutomatRight
// Created: SBO 2008-07-24
// -----------------------------------------------------------------------------
void ProfileManager::SetAutomatRight( const std::string& profile, unsigned int automat, bool readonly, bool readwrite )
{
    auto it = profiles_.find( profile );
    if( it != profiles_.end() )
        it->second->SetRight( model_.Automats().Get( automat ), readonly, readwrite );
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "RightsPlugin.h"
#include "dispatcher/CompositeRegistrable.h"
#include "dispatcher/NullClientPublisher.h"
#include "dispatcher/Profile.h"
#include "dispatcher/ProfileManager.h"
#include "dispatcher/DefaultProfile.h"
#include "dispatcher/Services.h"
#include "dispatcher/Config.h"
#include "protocol/AuthenticationSenders.h"
#include "protocol/Version.h"
#include "tools/MessageDispatcher_ABC.h"

using namespace plugins::rights;
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: RightsPlugin constructor
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
RightsPlugin::RightsPlugin( Model& model, ClientPublisher_ABC& clients, const Config& config, tools::MessageDispatcher_ABC& clientCommands, Plugin_ABC& container, LinkResolver_ABC& base, dispatcher::CompositeRegistrable& registrables, int maxConnections )
    : config_            ( config )
    , profiles_          ( new ProfileManager( model, clients, config ) )
    , container_         ( container )
    , base_              ( base )
    , maxConnections_    ( maxConnections )
    , currentConnections_( 0 )
{
    clientCommands.RegisterMessage( *this, &RightsPlugin::OnReceive );
    registrables.Add( new dispatcher::RegistrableProxy( *profiles_ ) );
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin destructor
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
RightsPlugin::~RightsPlugin()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::Register
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
void RightsPlugin::Register( dispatcher::Services& services )
{
    services.Declare< authentication::Service >();
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::Receive
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void RightsPlugin::Receive( const sword::SimToClient& message )
{
    profiles_->Receive( message );
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::NotifyClientAuthenticated
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void RightsPlugin::NotifyClientAuthenticated( ClientPublisher_ABC& client, Profile_ABC& /*profile*/ )
{
    profiles_->Send( client );
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::NotifyClientLeft
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void RightsPlugin::NotifyClientLeft( ClientPublisher_ABC& client )
{
    for( IT_Profiles it = authenticated_.begin(); it != authenticated_.end(); ++it )
        if( &GetPublisher( it->first ) == &client )
        {
            authenticated_.erase( it );
            --currentConnections_;
            return;
        }
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::OnReceive
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void RightsPlugin::OnReceive( const std::string& link, const sword::ClientToAuthentication& wrapper )
{
    if( GetProfile( link ).CheckRights( wrapper ) )
    {
        unsigned int nCtx = wrapper.has_context()? wrapper.context() : 0;

        if( wrapper.message().has_authentication_request() )
            OnReceiveMsgAuthenticationRequest( link, wrapper.message().authentication_request() );
        if( wrapper.message().has_profile_creation_request() )
            OnReceiveProfileCreationRequest( GetPublisher( link ), wrapper.message().profile_creation_request() );
        if( wrapper.message().has_profile_update_request() )
            OnReceiveProfileUpdateRequest( GetPublisher( link ), wrapper.message().profile_update_request(), nCtx );
        if( wrapper.message().has_profile_destruction_request() )
            OnReceiveProfileDestructionRequest( GetPublisher( link ), wrapper.message().profile_destruction_request() );
        if( wrapper.message().has_connected_profiles_request() )
            OnReceiveConnectedProfilesRequest( GetPublisher( link ), wrapper.context(), wrapper.message().connected_profiles_request() );
    }
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::OnReceiveMsgAuthenticationRequest
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void RightsPlugin::OnReceiveMsgAuthenticationRequest( const std::string& link, const sword::AuthenticationRequest& message )
{
    ClientPublisher_ABC& client = base_.GetPublisher( link );
    authentication::AuthenticationResponse ack;
    ack().mutable_server_version()->set_value( sword::ProtocolVersion().value() );
    if( ! sword::CheckCompatibility( message.version() ) )
    {
        ack().set_error_code( sword::AuthenticationResponse::mismatched_protocol_version );
        profiles_->Send( ack() );
        ack.Send( client );
        return;
    }
    if( maxConnections_ && maxConnections_ <= currentConnections_ )
    {
        ack().set_error_code( sword::AuthenticationResponse::too_many_connections );
        profiles_->Send( ack() );
        ack.Send( client );
        return;
    }
    Profile* profile = profiles_->Authenticate( message.login(), message.password() );
    if( !profile )
    {
        ack().set_error_code( sword::AuthenticationResponse::invalid_login );
        profiles_->Send( ack() );
        ack.Send( client );
    }
    else
    {
        ack().set_terrain_name( config_.GetTerrainName() );
        ack().set_error_code( sword::AuthenticationResponse::success );
        profile->Send( *ack().mutable_profile() );
        ack.Send( client );
        authenticated_[ link ] = profile;
        container_.NotifyClientAuthenticated( client, *profile );
        ++currentConnections_;
    }
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::OnReceiveProfileCreationRequest
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void RightsPlugin::OnReceiveProfileCreationRequest( ClientPublisher_ABC& client, const sword::ProfileCreationRequest& message )
{
    authentication::ProfileCreationRequestAck ack;
    ack().set_error_code( profiles_->Create( message ) );
    ack().set_login( message.profile().login() );
    ack.Send( client );
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::OnReceiveProfileUpdateRequest
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void RightsPlugin::OnReceiveProfileUpdateRequest( ClientPublisher_ABC& client, const sword::ProfileUpdateRequest& message, unsigned int context )
{
    authentication::ProfileUpdateRequestAck ack;
    ack().set_error_code( profiles_->Update( message ) );
    ack().set_login( message.login() );
    ack.Send( client, context );
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::OnReceiveProfileDestructionRequest
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void RightsPlugin::OnReceiveProfileDestructionRequest( ClientPublisher_ABC& client, const sword::ProfileDestructionRequest& message )
{
    authentication::ProfileDestructionRequestAck ack;
    if( IsAuthenticated( message.login() ) )
        ack().set_error_code( sword::ProfileDestructionRequestAck::invalid_profile );
    else
        ack().set_error_code( profiles_->Destroy( message ) );
    ack().set_login( message.login() );
    ack.Send( client );
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::OnReceiveConnecedProfilesRequest
// Created: AHC 2011-05-20
// -----------------------------------------------------------------------------
void RightsPlugin::OnReceiveConnectedProfilesRequest( ClientPublisher_ABC& client, int context, const sword::ConnectedProfilesRequest& /*message*/ )
{
    authentication::ConnectedProfileList response;
    for( T_Profiles::const_iterator it = authenticated_.begin(); it != authenticated_.end(); ++it )
        it->second->Send( *response().add_elem() );
    response.Send( client, context );
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::IsAuthenticated
// Created: SBO 2010-07-19
// -----------------------------------------------------------------------------
bool RightsPlugin::IsAuthenticated( const std::string& login ) const
{
    for( T_Profiles::const_iterator it = authenticated_.begin(); it != authenticated_.end(); ++it )
        if( it->second->GetName() == login )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::GetProfile
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
Profile_ABC& RightsPlugin::GetProfile( const std::string& link )
{
    static DefaultProfile def;
    CIT_Profiles it = authenticated_.find( link );
    if( it != authenticated_.end() )
        return *(it->second);
    return def;
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::GetPublisher
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
ClientPublisher_ABC& RightsPlugin::GetPublisher( const std::string& link )
{
    CIT_Profiles it = authenticated_.find( link );
    if( it != authenticated_.end() )
        return base_.GetPublisher( link );
    static NullClientPublisher publisher;
    return publisher;
}

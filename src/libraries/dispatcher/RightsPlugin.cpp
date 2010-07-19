// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "RightsPlugin.h"
#include "Client.h"
#include "CompositeRegistrable.h"
#include "DefaultProfile.h"
#include "LinkResolver_ABC.h"
#include "NoopPublisher.h"
#include "Profile.h"
#include "Profile_ABC.h"
#include "ProfileManager.h"
#include "Services.h"
#include "protocol/authenticationsenders.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ProtocolVersionChecker.h"
#include "tools/MessageDispatcher_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: RightsPlugin constructor
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
RightsPlugin::RightsPlugin( Model& model, ClientPublisher_ABC& clients, const Config& config, tools::MessageDispatcher_ABC& clientCommands, Plugin_ABC& container, LinkResolver_ABC& base, dispatcher::CompositeRegistrable& registrables, int maxConnections )
    : profiles_( new ProfileManager( model, clients, config ) )
    , container_( container )
    , base_( base )
    , maxConnections_( maxConnections )
    , currentConnections_( 0 )
{
    clientCommands.RegisterMessage( *this, &RightsPlugin::OnReceive );
    registrables.Add( new dispatcher::RegistrableProxy( *profiles_ ) );
    profiles_->RegisterRoles();
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
void RightsPlugin::Receive( const MsgsSimToClient::MsgSimToClient& message )
{
    profiles_->Receive( message );
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::NotifyClientAuthenticated
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void RightsPlugin::NotifyClientAuthenticated( ClientPublisher_ABC& client, Profile_ABC&  )
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
void RightsPlugin::OnReceive( const std::string& link, const MsgsClientToAuthentication::MsgClientToAuthentication& wrapper )
{
    if( GetProfile( link ).CheckRights( wrapper ) )
    {
        if( wrapper.message().has_authentication_request() )
            OnReceiveMsgAuthenticationRequest( link, wrapper.message().authentication_request() );
        if( wrapper.message().has_profile_creation_request() )
            OnReceiveMsgProfileCreationRequest( GetPublisher( link ), wrapper.message().profile_creation_request() );
        if( wrapper.message().has_profile_update_request() )
            OnReceiveMsgProfileUpdateRequest( GetPublisher( link ), wrapper.message().profile_update_request() );
        if( wrapper.message().has_profile_destruction_request() )
            OnReceiveMsgProfileDestructionRequest( GetPublisher( link ), wrapper.message().profile_destruction_request() );
    }
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::OnReceiveMsgAuthenticationRequest
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void RightsPlugin::OnReceiveMsgAuthenticationRequest( const std::string& link, const MsgsClientToAuthentication::MsgAuthenticationRequest& message )
{
    ClientPublisher_ABC& client = base_.GetPublisher( link );

    ProtocolVersionChecker checker ( message.version() );
    authentication::AuthenticationResponse ack;
    ack().mutable_server_version()->set_value( ProtocolVersionChecker::GetCurrentProtocolVersion() );
    if( !checker.CheckCompatibility() )
    {
        ack().set_error_code( MsgsAuthenticationToClient::MsgAuthenticationResponse_ErrorCode_mismatched_protocol_version );
        profiles_->Send( ack() );
        ack.Send( client );
        return;
    }
    if( maxConnections_ && maxConnections_ <= currentConnections_ )
    {
        ack().set_error_code( MsgsAuthenticationToClient::MsgAuthenticationResponse_ErrorCode_too_many_connections );
        profiles_->Send( ack() );
        ack.Send( client );
        return;
    }
    Profile* profile = profiles_->Authenticate( message.login(), message.password() );
    if( !profile )
    {
        ack().set_error_code( MsgsAuthenticationToClient::MsgAuthenticationResponse_ErrorCode_invalid_login );
        profiles_->Send( ack() );
        ack.Send( client );
    }
    else
    {
        ack().set_error_code( MsgsAuthenticationToClient::MsgAuthenticationResponse_ErrorCode_success );
        profile->Send( *ack().mutable_profile() );
        ack.Send( client );
        authenticated_[ link ] = profile;
        container_.NotifyClientAuthenticated( client, *profile );
        ++currentConnections_;
    }
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::OnReceiveMsgProfileCreationRequest
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void RightsPlugin::OnReceiveMsgProfileCreationRequest( ClientPublisher_ABC& client, const MsgsClientToAuthentication::MsgProfileCreationRequest& message )
{
    authentication::ProfileCreationRequestAck ack;
    ack().set_error_code( profiles_->Create( message ) );
    ack().set_login( message.profile().login() );
    ack.Send( client );
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::OnReceiveMsgProfileUpdateRequest
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void RightsPlugin::OnReceiveMsgProfileUpdateRequest( ClientPublisher_ABC& client, const MsgsClientToAuthentication::MsgProfileUpdateRequest& message )
{
    authentication::ProfileUpdateRequestAck ack;
    ack().set_error_code( profiles_->Update( message ) );
    ack().set_login( message.login() );
    ack.Send( client );
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::OnReceiveMsgProfileDestructionRequest
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void RightsPlugin::OnReceiveMsgProfileDestructionRequest( ClientPublisher_ABC& client, const MsgsClientToAuthentication::MsgProfileDestructionRequest& message )
{
    authentication::ProfileDestructionRequestAck ack;
    if( IsAuthenticated( message.login() ) )
        ack().set_error_code( MsgsAuthenticationToClient::MsgProfileDestructionRequestAck_ErrorCode_invalid_profile );
    else
        ack().set_error_code( profiles_->Destroy( message ) );
    ack().set_login( message.login() );
    ack.Send( client );
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
    static NoopClientPublisher def;
    CIT_Profiles it = authenticated_.find( link );
    if( it != authenticated_.end() )
        return base_.GetPublisher( link );
    return def;
}

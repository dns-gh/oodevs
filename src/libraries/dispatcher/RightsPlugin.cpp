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
#include "tools/MessageDispatcher_ABC.h"
#include "LinkResolver_ABC.h"
#include "Profile_ABC.h"
#include "ClientPublisher_ABC.h"
#include "game_asn/AuthenticationSenders.h"
#include "ProfileManager.h"
#include "Profile.h"
#include "DefaultProfile.h"
#include "Client.h"
#include "NoopPublisher.h"
#include "CompositeRegistrable.h"
#include "Services.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: RightsPlugin constructor
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
RightsPlugin::RightsPlugin( Model& model, ClientPublisher_ABC& clients, const Config& config, tools::MessageDispatcher_ABC& clientCommands, Plugin_ABC& container, LinkResolver_ABC& base, dispatcher::CompositeRegistrable& registrables )
    : profiles_( new ProfileManager( model, clients, config ) )
    , container_( container )
    , base_( base )
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
void RightsPlugin::Receive( const ASN1T_MsgsSimToClient& message )
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
            return;
        }
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::OnReceive
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void RightsPlugin::OnReceive( const std::string& link, const ASN1T_MsgsClientToAuthentication& message )
{
    if( GetProfile( link ).CheckRights( message ) )
    {
        switch( message.msg.t )
        {
        case T_MsgsClientToAuthentication_msg_msg_authentication_request:
            OnReceiveMsgAuthenticationRequest( link, *message.msg.u.msg_authentication_request ); break;
        case T_MsgsClientToAuthentication_msg_msg_profile_creation_request:
            OnReceiveMsgProfileCreationRequest( GetPublisher( link ), *message.msg.u.msg_profile_creation_request ); break;
        case T_MsgsClientToAuthentication_msg_msg_profile_update_request:
            OnReceiveMsgProfileUpdateRequest( GetPublisher( link ), *message.msg.u.msg_profile_update_request ); break;
        case T_MsgsClientToAuthentication_msg_msg_profile_destruction_request:
            OnReceiveMsgProfileDestructionRequest( GetPublisher( link ), message.msg.u.msg_profile_destruction_request ); break;
        };
    }
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::OnReceiveMsgAuthenticationRequest
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void RightsPlugin::OnReceiveMsgAuthenticationRequest( const std::string& link, const ASN1T_MsgAuthenticationRequest& message )
{
    ClientPublisher_ABC& client = base_.GetPublisher( link );

    Profile* profile = profiles_->Authenticate( message.login, message.password );
    if( !profile )
    {
        authentication::AuthenticationResponse ack;
        ack().error_code = MsgAuthenticationResponse_error_code::invalid_login;
        ack.Send( client );
    }
    else
    {
        authentication::AuthenticationResponse ack;
        ack().error_code       = MsgAuthenticationResponse_error_code::success;
        ack().m.profilePresent = 1;
        profile->Send( ack().profile );
        ack.Send( client );
        Profile::AsnDelete( ack().profile );
        authenticated_[ link ] = profile;
        container_.NotifyClientAuthenticated( client, *profile );
    }
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::OnReceiveMsgProfileCreationRequest
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void RightsPlugin::OnReceiveMsgProfileCreationRequest( ClientPublisher_ABC& client, const ASN1T_MsgProfileCreationRequest& message )
{
    authentication::ProfileCreationRequestAck ack;
    ack().error_code = profiles_->Create( message );
    ack().login       = message.login;
    ack.Send( client );
}   

// -----------------------------------------------------------------------------
// Name: RightsPlugin::OnReceiveMsgProfileUpdateRequest
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void RightsPlugin::OnReceiveMsgProfileUpdateRequest( ClientPublisher_ABC& client, const ASN1T_MsgProfileUpdateRequest& message )
{
    authentication::ProfileUpdateRequestAck ack;
    ack().error_code = profiles_->Update( message );
    ack().login       = message.login;
    ack.Send( client );
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::OnReceiveMsgProfileDestructionRequest
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void RightsPlugin::OnReceiveMsgProfileDestructionRequest( ClientPublisher_ABC& client, const ASN1T_MsgProfileDestructionRequest& message )
{
    authentication::ProfileDestructionRequestAck ack;
    ack().error_code = profiles_->Destroy( message );
    ack().login       = message;
    ack.Send( client );
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

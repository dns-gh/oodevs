// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
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
#include "MT_Tools/MT_Logger.h"
#include "protocol/AuthenticationSenders.h"
#include "protocol/Version.h"
#include "protocol/RightsHelper.h"
#include "tools/MessageDispatcher_ABC.h"

using namespace dispatcher;

namespace plugins
{
namespace rights
{

class AuthenticationSender: private boost::noncopyable
{
public:
    AuthenticationSender( ClientPublisher_ABC& client,
        ClientPublisher_ABC& broadcast, int ctx )
        : client_( client )
        , broadcast_( broadcast )
        , ctx_( ctx )
    {
    }

    void Send( sword::AuthenticationToClient& msg )
    {
        msg.set_context( ctx_ );
        client_.Send( msg );
    }

    void Broadcast( sword::AuthenticationToClient& msg )
    {
        msg.set_context( 0 );
        broadcast_.Send( msg );
    }

    ClientPublisher_ABC& GetClient()
    {
        return client_;
    }

private:
    ClientPublisher_ABC& client_;
    ClientPublisher_ABC& broadcast_;
    int ctx_;
};

// -----------------------------------------------------------------------------
// Name: RightsPlugin constructor
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
RightsPlugin::RightsPlugin( Model& model, ClientPublisher_ABC& clients, const Config& config, tools::MessageDispatcher_ABC& clientCommands, Plugin_ABC& container, LinkResolver_ABC& base, dispatcher::CompositeRegistrable& registrables, int maxConnections )
    : clients_           ( clients )
    , config_            ( config )
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
void RightsPlugin::NotifyClientAuthenticated( ClientPublisher_ABC& client, const std::string& /*link*/, Profile_ABC& /*profile*/ )
{
    profiles_->Send( client );
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::NotifyClientLeft
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void RightsPlugin::NotifyClientLeft( ClientPublisher_ABC& client, const std::string& /*link*/ )
{
    Logout( client );
}

void RightsPlugin::Logout( ClientPublisher_ABC& client )
{
    for( IT_Profiles it = authenticated_.begin(); it != authenticated_.end(); ++it )
        if( &GetPublisher( it->first ) == &client )
        {
            authenticated_.erase( it );
            --currentConnections_;
            AuthenticationSender sender( client, clients_, 0 );
            SendProfiles( sender );
            MT_LOG_INFO_MSG( currentConnections_ << " clients authentified" );
            return;
        }
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::OnReceive
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void RightsPlugin::OnReceive( const std::string& link, const sword::ClientToAuthentication& wrapper )
{
    if( wrapper.message().has_disconnection_request() )
    {
        Logout( base_.GetPublisher( link ) );
        MT_LOG_INFO_MSG( "Logged out " + link );
        throw tools::DisconnectionRequest( "disconnection request from " + link );
    }

    unsigned int ctx = wrapper.has_context() ? wrapper.context() : 0;
    AuthenticationSender sender( base_.GetPublisher( link ), clients_, ctx );
    if( wrapper.message().has_authentication_request() )
    {
        // Cannot forbid an authentication request
        OnReceiveMsgAuthenticationRequest( link,
            wrapper.message().authentication_request(), sender );
        return;
    }

    if( GetProfile( link ).CheckRights( wrapper ) )
    {
        if( wrapper.message().has_profile_creation_request() )
            OnReceiveProfileCreationRequest( wrapper.message().profile_creation_request(), sender );
        if( wrapper.message().has_profile_update_request() )
            OnReceiveProfileUpdateRequest( wrapper.message().profile_update_request(), sender );
        if( wrapper.message().has_profile_destruction_request() )
            OnReceiveProfileDestructionRequest( wrapper.message().profile_destruction_request(), sender );
        if( wrapper.message().has_connected_profiles_request() )
            OnReceiveConnectedProfilesRequest( wrapper.message().connected_profiles_request(), sender );
    }
    else
    {
        sword::AuthenticationToClient ack;
        if( !protocol::GetForbiddenError( wrapper, ack ))
        {
            // Irrelevant but better than no error
            auto m = ack.mutable_message()->mutable_profile_creation_request_ack();
            m->set_error_code( sword::ProfileCreationRequestAck::forbidden );
            m->set_login( "unknown" );
        }
        sender.Send( ack );
    }
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::OnReceiveMsgAuthenticationRequest
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void RightsPlugin::OnReceiveMsgAuthenticationRequest( const std::string& link, const sword::AuthenticationRequest& message, AuthenticationSender& sender )
{
    sword::AuthenticationToClient reply;
    auto ack = reply.mutable_message()->mutable_authentication_response();
    ack->mutable_server_version()->set_value( sword::ProtocolVersion().value() );
    if( ! sword::CheckCompatibility( message.version() ) )
    {
        ack->set_error_code( sword::AuthenticationResponse::mismatched_protocol_version );
        profiles_->Send( *ack );
        sender.Send( reply );
        return;
    }
    CIT_Profiles it = authenticated_.find( link );
    if( it != authenticated_.end() )
        container_.NotifyClientLeft( sender.GetClient(), link );
    if( maxConnections_ && maxConnections_ <= currentConnections_ )
    {
        ack->set_error_code( sword::AuthenticationResponse::too_many_connections );
        profiles_->Send( *ack );
        sender.Send( reply );
        return;
    }
    auto profile = profiles_->Authenticate( message.login(), message.password(), link );
    if( !profile )
    {
        ack->set_error_code( sword::AuthenticationResponse::invalid_login );
        profiles_->Send( *ack );
        sender.Send( reply );
        SendProfiles( sender );
    }
    else
    {
        ack->set_terrain_name( config_.GetTerrainName().ToUTF8() );
        ack->set_error_code( sword::AuthenticationResponse::success );
        profile->Send( *ack->mutable_profile() );
        sender.Send( reply );
        authenticated_[ link ] = profile;
        ++currentConnections_;
        SendProfiles( sender );
        container_.NotifyClientAuthenticated( sender.GetClient(), link, *profile );
        MT_LOG_INFO_MSG( currentConnections_ << " clients authentified" );
    }
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::OnReceiveProfileCreationRequest
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void RightsPlugin::OnReceiveProfileCreationRequest( const sword::ProfileCreationRequest& message, AuthenticationSender& sender )
{
    sword::AuthenticationToClient reply;
    auto ack = reply.mutable_message()->mutable_profile_creation_request_ack();
    ack->set_error_code( profiles_->Create( message ) );
    ack->set_login( message.profile().login() );
    sender.Send( reply );
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::OnReceiveProfileUpdateRequest
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void RightsPlugin::OnReceiveProfileUpdateRequest( const sword::ProfileUpdateRequest& message, AuthenticationSender& sender )
{
    sword::AuthenticationToClient reply;
    auto ack = reply.mutable_message()->mutable_profile_update_request_ack();
    ack->set_error_code( profiles_->Update( message ) );
    ack->set_login( message.login() );
    sender.Send( reply );
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::OnReceiveProfileDestructionRequest
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void RightsPlugin::OnReceiveProfileDestructionRequest( const sword::ProfileDestructionRequest& message, AuthenticationSender& sender )
{
    sword::AuthenticationToClient reply;
    auto ack = reply.mutable_message()->mutable_profile_destruction_request_ack();
    if( IsAuthenticated( message.login() ) )
        ack->set_error_code( sword::ProfileDestructionRequestAck::invalid_profile );
    else
        ack->set_error_code( profiles_->Destroy( message ) );
    ack->set_login( message.login() );
    sender.Send( reply );
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::OnReceiveConnecedProfilesRequest
// Created: AHC 2011-05-20
// -----------------------------------------------------------------------------
void RightsPlugin::OnReceiveConnectedProfilesRequest( const sword::ConnectedProfilesRequest& /*message*/, AuthenticationSender& sender )
{
    sword::AuthenticationToClient reply;
    auto response = reply.mutable_message()->mutable_connected_profile_list();
    for( T_Profiles::const_iterator it = authenticated_.begin(); it != authenticated_.end(); ++it )
        it->second->Send( *response->add_elem() );
    sender.Send( reply );
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

// -----------------------------------------------------------------------------
// Name: RightsPlugin::SendProfiles
// Created: LGY 2011-11-21
// -----------------------------------------------------------------------------
void RightsPlugin::SendProfiles( AuthenticationSender& sender ) const
{
    sword::AuthenticationToClient reply;
    auto response = reply.mutable_message()->mutable_connected_profile_list();
    for( T_Profiles::const_iterator it = authenticated_.begin(); it != authenticated_.end(); ++it )
        it->second->Send( *response->add_elem() );
    sender.Broadcast( reply );
}

}  // namespace rights
}  // namespace plugins

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
#include "dispatcher/LinkResolver_ABC.h"
#include "dispatcher/Profile.h"
#include "dispatcher/ProfileManager.h"
#include "dispatcher/DefaultProfile.h"
#include "dispatcher/Services.h"
#include "dispatcher/Config.h"
#include "MT_Tools/MT_Logger.h"
#include "protocol/AuthenticationSenders.h"
#include "protocol/Dispatcher.h"
#include "protocol/Version.h"
#include "protocol/RightsHelper.h"
#include "tools/MessageDispatcher_ABC.h"
#include <openssl/hmac.h>
#include <iomanip>

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
RightsPlugin::RightsPlugin( Model& model, ClientPublisher_ABC& clients, const Config& config, tools::MessageDispatcher_ABC& clientCommands,
                            Plugin_ABC& container, const LinkResolver_ABC& resolver, dispatcher::CompositeRegistrable& registrables, int maxConnections,
                            bool replayer )
    : clients_           ( clients )
    , config_            ( config )
    , profiles_          ( new ProfileManager( model, clients, config ) )
    , container_         ( container )
    , resolver_          ( resolver )
    , maxConnections_    ( maxConnections )
    , currentConnections_( 0 )
    , countID_           ( 1 ) // non-nul client identifier
    , replayer_          ( replayer )
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
    services.Declare( sword::service_authentication );
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
void RightsPlugin::NotifyClientAuthenticated( ClientPublisher_ABC& client, const std::string& /*link*/, Profile_ABC& /*profile*/,
                                              unsigned int /*clientId*/, bool /*uncounted*/ )
{
    profiles_->Send( client );
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::NotifyClientLeft
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void RightsPlugin::NotifyClientLeft( ClientPublisher_ABC& client, const std::string& /*link*/, bool /*uncounted*/ )
{
    Logout( client );
}

void RightsPlugin::Logout( ClientPublisher_ABC& client )
{
    for( auto it = authenticated_.begin(); it != authenticated_.end(); ++it )
    {
        const std::string link = it->first;
        if( &GetAuthenticatedPublisher( link ) == &client )
        {
            authenticated_.erase( it );
            if( silentClients_.erase( link ) == 0 )
                --currentConnections_;
            auto id = clientsID_.find( link );
            if( id != clientsID_.end() )
            {
                ids_.erase( id->second );
                clientsID_.erase( id );
            }
            AuthenticationSender sender( client, clients_, 0 );
            SendProfiles( sender );
            MT_LOG_INFO_MSG( currentConnections_ << " clients authentified" );
            return;
        }
    }
}
namespace
{
    const std::string K1 = "29Ma500";
    const std::string K2 = "SaGro";
    const std::string K3 = "Up75";

    std::string GenerateKey()
    {
        std::stringstream s;
        for( unsigned int i = 0; i < 16; ++i )
            s << std::setbase( 16 ) << ( std::rand() % 256 );
        return s.str();
    }

    std::string MakeHmac( const std::string& key, const std::string& data )
    {
        const unsigned int hashLen = 16;
        unsigned char hash[ hashLen ];
        unsigned int written = hashLen;

        HMAC( EVP_md5(), key.c_str(), int( key.size() ), (unsigned char*)data.c_str(),
            data.size(), hash, &written );

        std::string result;
        for( unsigned int i = 0; i < written; i++ )
        {
            char hex[3];
            sprintf(hex, "%02x", (unsigned int)hash[i]);
            result.append(hex);
        }
        return result;
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
        Logout( resolver_.GetConnectedPublisher( link ) );
        MT_LOG_INFO_MSG( "Logged out " + link );
        throw tools::DisconnectionRequest( __FILE__, __FUNCTION__, __LINE__, "disconnection request from " + link );
    }

    unsigned int ctx = wrapper.has_context() ? wrapper.context() : 0;
    AuthenticationSender sender( resolver_.GetConnectedPublisher( link ), clients_, ctx );
    if( wrapper.message().has_authentication_key_request() )
    {
        sword::AuthenticationToClient ack;
        auto m = ack.mutable_message()->mutable_authentication_key_response();
        auto it = authenticationKeys_.find( link );
        const std::string key = it == authenticationKeys_.end() ? GenerateKey() : it->second;
        m->set_authentication_key( key );
        authenticationKeys_[ link ] = key;
        SendReponse( ack, sender, link );
        return;
    }

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
            OnReceiveProfileCreationRequest( wrapper.message().profile_creation_request(), sender, link );
        if( wrapper.message().has_profile_update_request() )
            OnReceiveProfileUpdateRequest( wrapper.message().profile_update_request(), sender, link );
        if( wrapper.message().has_profile_destruction_request() )
            OnReceiveProfileDestructionRequest( wrapper.message().profile_destruction_request(), sender, link );
        if( wrapper.message().has_connected_profiles_request() )
            OnReceiveConnectedProfilesRequest( wrapper.message().connected_profiles_request(), sender, link );
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
        SendReponse( ack, sender, link );
    }
}

unsigned int RightsPlugin::AcquireClientId()
{
    for(;;)
    {
        // reserve and set top bit for replayer clients
        const int32_t mask = 1 << 31;
        const auto id = replayer_ ? countID_ | mask : countID_ & ~mask;
        if( id && !ids_.count( id ) )
            return id;
        countID_++;
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

    auto it = authenticated_.find( link );
    if( it != authenticated_.end() )
        container_.NotifyClientLeft( sender.GetClient(), link, silentClients_.find( link ) != silentClients_.end() );

    bool keyAuthenticated = false;
    if( message.has_authentication_key() )
    {
        auto it = authenticationKeys_.find( link );
        if( it == authenticationKeys_.end() || MakeHmac( K1 + K2 + K3, it->second ) != message.authentication_key() )
        {
            ack->set_error_code( sword::AuthenticationResponse::invalid_authentication_key );
            profiles_->Send( *ack );
            sender.Send( reply );
            return;
        }
        keyAuthenticated = true;
    }

    if( !keyAuthenticated && maxConnections_ && maxConnections_ <= currentConnections_ )
    {
        ack->set_error_code( sword::AuthenticationResponse::too_many_connections );
        profiles_->Send( *ack );
        sender.Send( reply );
        return;
    }
    auto profile = profiles_->Authenticate( message.login(), message.password(), link, keyAuthenticated );
    if( !profile )
    {
        ack->set_error_code( sword::AuthenticationResponse::invalid_login );
        profiles_->Send( *ack );
        sender.Send( reply );
        SendProfiles( sender );
    }
    else
    {
        authenticationKeys_.erase( link );
        ack->set_terrain_name( config_.GetTerrainName().ToUTF8() );
        ack->set_error_code( sword::AuthenticationResponse::success );
        profile->Send( *ack );
        const auto clientId = AcquireClientId();
        reply.set_client_id( clientId );
        sender.Send( reply );
        authenticated_[ link ] = profile;
        clientsID_[ link ] = clientId;
        ids_[ clientId ] = link;
        if( keyAuthenticated )
            silentClients_.insert( link );
        else
            ++currentConnections_;
        SendProfiles( sender );
        container_.NotifyClientAuthenticated( sender.GetClient(), link, *profile, clientId, keyAuthenticated );
        MT_LOG_INFO_MSG( currentConnections_ << " clients authentified" );
    }
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::OnReceiveProfileCreationRequest
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void RightsPlugin::OnReceiveProfileCreationRequest( const sword::ProfileCreationRequest& message, AuthenticationSender& sender, const std::string& link )
{
    sword::AuthenticationToClient reply;
    auto ack = reply.mutable_message()->mutable_profile_creation_request_ack();
    ack->set_error_code( profiles_->Create( message ) );
    ack->set_login( message.profile().login() );
    SendReponse( reply, sender, link );
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::OnReceiveProfileUpdateRequest
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void RightsPlugin::OnReceiveProfileUpdateRequest( const sword::ProfileUpdateRequest& message, AuthenticationSender& sender, const std::string& link )
{
    sword::AuthenticationToClient reply;
    auto ack = reply.mutable_message()->mutable_profile_update_request_ack();
    ack->set_error_code( profiles_->Update( message, GetProfile( link ) ) );
    ack->set_login( message.login() );
    SendReponse( reply, sender, link );
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::OnReceiveProfileDestructionRequest
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void RightsPlugin::OnReceiveProfileDestructionRequest( const sword::ProfileDestructionRequest& message, AuthenticationSender& sender, const std::string& link )
{
    sword::AuthenticationToClient reply;
    auto ack = reply.mutable_message()->mutable_profile_destruction_request_ack();
    if( IsAuthenticated( message.login() ) )
        ack->set_error_code( sword::ProfileDestructionRequestAck::invalid_profile );
    else
        ack->set_error_code( profiles_->Destroy( message ) );
    ack->set_login( message.login() );
    SendReponse( reply, sender, link );
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::OnReceiveConnecedProfilesRequest
// Created: AHC 2011-05-20
// -----------------------------------------------------------------------------
void RightsPlugin::OnReceiveConnectedProfilesRequest( const sword::ConnectedProfilesRequest& /*message*/, AuthenticationSender& sender, const std::string& link )
{
    sword::AuthenticationToClient reply;
    auto response = reply.mutable_message()->mutable_connected_profile_list();
    for( auto it = authenticated_.begin(); it != authenticated_.end(); ++it )
        it->second->Send( *response );
    SendReponse( reply, sender, link );
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::SendReponse
// Created: LGY 2013-04-24
// -----------------------------------------------------------------------------
void RightsPlugin::SendReponse( sword::AuthenticationToClient& reply, AuthenticationSender& sender, const std::string& link ) const
{
    if( unsigned int id = GetClientID( link ) )
        reply.set_client_id( id );
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
Profile_ABC& RightsPlugin::GetProfile( const std::string& link ) const
{
    auto it = authenticated_.find( link );
    if( it != authenticated_.end() )
        return *it->second;
    static DefaultProfile profile;
    return profile;
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::GetPublisher
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------

namespace
{

NullClientPublisher nullPublisher;

}  // namespace

ClientPublisher_ABC& RightsPlugin::GetAuthenticatedPublisher( const std::string& link ) const
{
    auto it = authenticated_.find( link );
    if( it != authenticated_.end() )
        return resolver_.GetConnectedPublisher( link );
    return nullPublisher;
}

ClientPublisher_ABC& RightsPlugin::GetAuthenticatedPublisher( unsigned int clientId ) const
{
   auto it = ids_.find( clientId );
   if( it != ids_.end() )
       return resolver_.GetConnectedPublisher( it->second );
   return nullPublisher;
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
        it->second->Send( *response );
    sender.Broadcast( reply );
}

// -----------------------------------------------------------------------------
// Name: RightsPlugin::GetClientID
// Created: LGY 2013-04-24
// -----------------------------------------------------------------------------
unsigned int RightsPlugin::GetClientID( const std::string& link ) const
{
    auto it = clientsID_.find( link );
    if( it != clientsID_.end() )
        return it->second;
    return 0u;
}

}  // namespace rights
}  // namespace plugins

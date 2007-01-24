// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "Client.h"

#include "network/Simulation_Asn.h"
#include "Dispatcher.h"
#include "Model.h"
#include "Profile.h"
#include "ProfileManager.h"
#include "Network_Def.h"
#include "ClientPublisher.h"
#include "network/AsnMessageEncoder.h"
#include "DIN/MessageService/DIN_MessageService_ABC.h"
#include "DIN/DIN_Link.h"

using namespace dispatcher;
using namespace network;
using namespace DIN;

// -----------------------------------------------------------------------------
// Name: Client constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
Client::Client( Dispatcher& dispatcher, DIN_MessageService_ABC& messageService, DIN_Link& link )
    : Client_ABC ( messageService, link )
    , dispatcher_( dispatcher )
    , pProfile_  ( 0 )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: Client destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
Client::~Client()
{
    // NOTHING
}

// =============================================================================
// RIGHTS MANAGEMENT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Client::CheckRights
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
bool Client::CheckRights( const ASN1T_MsgsOutClient& asnMsg ) const
{
    if( asnMsg.msg.t == T_MsgsOutClient_msg_msg_auth_login )
        return true;

    if( !pProfile_ )
        return false;

    return pProfile_->CheckRights( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Client::CheckRights
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
bool Client::CheckRights( const ASN1T_MsgsInClient& asnMsg ) const
{
    //$$$$ TMP
    if( asnMsg.msg.t == T_MsgsInClient_msg_msg_auth_login_ack )
        return true;

    if( !pProfile_ )
        return false;

    return true;
}

// =============================================================================
// MESSAGES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Client::OnReceiveMsgAuthLogin
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
void Client::OnReceiveMsgAuthLogin( const ASN1T_MsgAuthLogin& msg )
{
    //$$$ TMP
    if( pProfile_ )
    {
        Disconnect();
        return;
    }

    ClientPublisher publisher( *this );

    pProfile_ = dispatcher_.GetProfileManager().Authenticate( msg.login, msg.password );
    if( !pProfile_ )
    {
        AsnMsgInClientAuthLoginAck ack;
        ack().etat = MsgAuthLoginAck_etat::invalid_login;
        ack.Send( publisher );
        return; //$$$ ??
    }

    // Ack message
    AsnMsgInClientAuthLoginAck ack;
    ack().etat             = MsgAuthLoginAck_etat::success;
    ack().m.profilePresent = 1;
    pProfile_->Send( ack().profile );
    ack.Send( publisher );
    Profile::AsnDelete( ack().profile );

    // Model
    dispatcher_.GetModel().Send( publisher );

    // Profiles
    dispatcher_.GetProfileManager().Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Client::OnReceiveMsgProfileCreationRequest
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
void Client::OnReceiveMsgProfileCreationRequest( const ASN1T_MsgProfileCreationRequest& asnMsg )
{
    ClientPublisher publisher( *this );
    AsnMsgInClientProfileCreationRequestAck ack;
    ack().error_code = dispatcher_.GetProfileManager().Create( asnMsg );
    ack().login      = asnMsg.login;
    ack.Send( publisher );

    // $$$$ SBO 2007-01-22: not terrific... move to ProfileManager or Profile::NotifyCreated ?
    if( ack().error_code == MsgProfileCreationRequestAck_error_code::success )
    {
        ASN1T_MsgsInClient message;
        message.msg.t = T_MsgsInClient_msg_msg_profile_creation;
        Profile& profile = *dispatcher_.GetProfileManager().Find( asnMsg.login );
        profile.Send( *message.msg.u.msg_profile_creation );
        dispatcher_.DispatchToClients( message );
        Profile::AsnDelete( *message.msg.u.msg_profile_creation );
    }
}

// -----------------------------------------------------------------------------
// Name: Client::OnReceiveMsgProfileUpdateRequest
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
void Client::OnReceiveMsgProfileUpdateRequest( const ASN1T_MsgProfileUpdateRequest& asnMsg )
{
    ClientPublisher publisher( *this );
    AsnMsgInClientProfileUpdateRequestAck ack;
    ack().error_code = dispatcher_.GetProfileManager().Update( asnMsg );
    ack().login      = asnMsg.login;
    ack.Send( publisher );

    // $$$$ SBO 2007-01-22: not terrific... move to ProfileManager or Profile::NotifyUpdated ?
    if( ack().error_code == MsgProfileUpdateRequestAck_error_code::success )
    {
        ASN1T_MsgsInClient message;
        message.msg.t = T_MsgsInClient_msg_msg_profile_update;
        message.msg.u.msg_profile_update = new ASN1T_MsgProfileUpdate();
        message.msg.u.msg_profile_update->login = asnMsg.login;
        Profile& profile = *dispatcher_.GetProfileManager().Find( asnMsg.profile.login );
        profile.Send( message.msg.u.msg_profile_update->profile );
        dispatcher_.DispatchToClients( message );
        Profile::AsnDelete( message.msg.u.msg_profile_update->profile );
        delete message.msg.u.msg_profile_update;
    }
}

// -----------------------------------------------------------------------------
// Name: Client::OnReceiveMsgProfileDestructionRequest
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
void Client::OnReceiveMsgProfileDestructionRequest( const ASN1T_MsgProfileDestructionRequest& asnMsg )
{
    ClientPublisher publisher( *this );
    AsnMsgInClientProfileDestructionRequestAck ack;
    ack().error_code = dispatcher_.GetProfileManager().Destroy( asnMsg );
    ack().login      = asnMsg;
    ack.Send( publisher );

    // $$$$ SBO 2007-01-22: not terrific... move to ProfileManager or Profile::NotifyDeleted ?
    if( ack().error_code == MsgProfileDestructionRequestAck_error_code::success )
    {
        ASN1T_MsgsInClient message;
        message.msg.t = T_MsgsInClient_msg_msg_profile_destruction;
        message.msg.u.msg_profile_destruction = asnMsg;
        dispatcher_.DispatchToClients( message );
    }
}

// 
#define DISPATCH_ASN_MSG( NAME )                                        \
    case T_MsgsOutClient_msg_msg_##NAME:                                \
    {                                                                   \
        asnOutMsg.context          = asnInMsg.context;                  \
        asnOutMsg.msg.t            = T_MsgsInSim_msg_msg_##NAME;        \
        asnOutMsg.msg.u.msg_##NAME = asnInMsg.msg.u.msg_##NAME;         \
        dispatcher_.DispatchToSimulation( asnOutMsg );                  \
        break;                                                          \
    } 

#define DISPATCH_EMPTY_ASN_MSG( NAME )                                  \
    case T_MsgsOutClient_msg_msg_##NAME:                                \
    {                                                                   \
        asnOutMsg.context          = asnInMsg.context;                  \
        asnOutMsg.msg.t            = T_MsgsInSim_msg_msg_##NAME;        \
        dispatcher_.DispatchToSimulation( asnOutMsg );                  \
        break;                                                          \
    } 

// -----------------------------------------------------------------------------
// Name: Client::OnReceive
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
void Client::OnReceive( const ASN1T_MsgsOutClient& asnInMsg )
{
    if( !CheckRights( asnInMsg ) )
    {
        Disconnect();
        return;
    }

    ASN1T_MsgsInSim asnOutMsg;
    switch( asnInMsg.msg.t )
    {
        case T_MsgsOutClient_msg_msg_auth_login                  : OnReceiveMsgAuthLogin                ( *asnInMsg.msg.u.msg_auth_login ); break;
        case T_MsgsOutClient_msg_msg_profile_creation_request    : OnReceiveMsgProfileCreationRequest   ( *asnInMsg.msg.u.msg_profile_creation_request ); break;
        case T_MsgsOutClient_msg_msg_profile_update_request      : OnReceiveMsgProfileUpdateRequest     ( *asnInMsg.msg.u.msg_profile_update_request ); break;
        case T_MsgsOutClient_msg_msg_profile_destruction_request : OnReceiveMsgProfileDestructionRequest( asnInMsg.msg.u.msg_profile_destruction_request ); break;

        DISPATCH_EMPTY_ASN_MSG( ctrl_stop );
        DISPATCH_EMPTY_ASN_MSG( ctrl_pause );
        DISPATCH_EMPTY_ASN_MSG( ctrl_resume );
        DISPATCH_ASN_MSG      ( ctrl_change_time_factor );
        DISPATCH_ASN_MSG      ( ctrl_meteo_globale );
        DISPATCH_ASN_MSG      ( ctrl_meteo_locale );
        DISPATCH_ASN_MSG      ( ctrl_checkpoint_save_now );
        DISPATCH_ASN_MSG      ( ctrl_checkpoint_set_frequency );
        DISPATCH_ASN_MSG      ( limit_creation_request );
        DISPATCH_ASN_MSG      ( limit_destruction_request );
        DISPATCH_ASN_MSG      ( limit_update_request );
        DISPATCH_ASN_MSG      ( lima_creation_request );
        DISPATCH_ASN_MSG      ( lima_destruction_request );
        DISPATCH_ASN_MSG      ( lima_update_request );
        DISPATCH_ASN_MSG      ( pion_order );
        DISPATCH_ASN_MSG      ( automate_order);
        DISPATCH_ASN_MSG      ( population_order );
        DISPATCH_ASN_MSG      ( frag_order );
        DISPATCH_ASN_MSG      ( set_automate_mode );
        DISPATCH_ASN_MSG      ( unit_magic_action );
        DISPATCH_ASN_MSG      ( object_magic_action );
        DISPATCH_ASN_MSG      ( population_magic_action );
        DISPATCH_ASN_MSG      ( change_diplomatie );
        DISPATCH_ASN_MSG      ( change_groupe_connaissance );
        DISPATCH_ASN_MSG      ( change_liens_logistiques );
        DISPATCH_ASN_MSG      ( change_automate );
        DISPATCH_ASN_MSG      ( log_ravitaillement_pousser_flux );
        DISPATCH_ASN_MSG      ( log_ravitaillement_change_quotas );
        default:
            assert( false );
    }
}

// -----------------------------------------------------------------------------
// Name: Client::OnReceive
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
void Client::OnReceive( unsigned int nMsgID, DIN::DIN_Input& dinMsg )
{
    // $$$ TMP
    if( !pProfile_ )
        return;

    dispatcher_.DispatchToSimulation( nMsgID, dinMsg );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Client::Send( const ASN1T_MsgsInClient& asnMsg )
{
    if( !CheckRights( asnMsg ) )
        return;

    network::AsnMessageEncoder< ASN1T_MsgsInClient, ASN1C_MsgsInClient > asnEncoder( messageService_, asnMsg );
    messageService_.Send( link_, eMsgInClient, asnEncoder.GetDinMsg() );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
void Client::Send( const ASN1T_MsgsInClient& asnMsg, const DIN_BufferedMessage& dinMsg )
{
    if( !CheckRights( asnMsg ) )
        return;

    messageService_.Send( link_, eMsgInClient, dinMsg );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
void Client::Send( unsigned int nMsgID, const DIN::DIN_BufferedMessage& dinMsg )
{
    // $$$ TMP
    if( !pProfile_ )
        return;

    messageService_.Send( link_, nMsgID, dinMsg );
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Client::GetClientFromLink
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
// static
Client& Client::GetClientFromLink( const DIN_Link& link )
{
    DIN_UserData_ABC* pTmp = link.GetUserData();
    assert( pTmp );
    return *static_cast< Client* >( pTmp );    
}

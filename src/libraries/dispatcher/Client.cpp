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

#include "AsnTypes.h"
#include "Dispatcher.h"
#include "Model.h"
#include "Profile.h"
#include "ProfileManager.h"
#include "Network_Def.h"
#include "ClientPublisher.h"
#include "AsnMessageEncoder.h"

using namespace dispatcher;
using namespace DIN;

// -----------------------------------------------------------------------------
// Name: Client constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
Client::Client( Dispatcher& dispatcher, DIN_MessageService_ABC& messageService, DIN_Link& link )
    : DIN_UserData_ABC()
    , dispatcher_     ( dispatcher )
    , messageService_ ( messageService )
    , link_           ( link )
    , pProfile_       ( 0 )
{
    assert( !link_.GetUserData() );
    link_.SetUserData( this );        

    messageService_.Enable( link_ );
}

//-----------------------------------------------------------------------------
// Name: Client destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
Client::~Client()
{
    messageService_.Disable( link_ );

    assert( link_.GetUserData() == this );
    link_.SetUserData( 0 );
}

// -----------------------------------------------------------------------------
// Name: Client::Disconnect
// Created: NLD 2006-10-05
// -----------------------------------------------------------------------------
void Client::Disconnect()
{
    link_.Close( true ); 
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
}

// 
#define DISPATCH_ASN_MSG( NAME )                                 \
    {                                                            \
        asnOutMsg.context          = asnInMsg.context;           \
        asnOutMsg.msg.t            = T_MsgsInSim_msg_msg_##NAME; \
        asnOutMsg.msg.u.msg_##NAME = asnInMsg.msg.u.msg_##NAME;  \
        dispatcher_.DispatchToSimulation( asnOutMsg) ;           \
    } 

#define DISPATCH_EMPTY_ASN_MSG( NAME )                  \
    {                                                   \
        asnOutMsg.context = asnInMsg.context;           \
        asnOutMsg.msg.t   = T_MsgsInSim_msg_msg_##NAME; \
        dispatcher_.DispatchToSimulation( asnOutMsg) ;  \
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
        case T_MsgsOutClient_msg_msg_auth_login                         : OnReceiveMsgAuthLogin( *asnInMsg.msg.u.msg_auth_login ); break;
        case T_MsgsOutClient_msg_msg_ctrl_profile_creation              : /*TODO$$$*/ break;
        case T_MsgsOutClient_msg_msg_ctrl_profile_update                : /*TODO$$$*/ break;
        case T_MsgsOutClient_msg_msg_ctrl_profile_destruction           : /*TODO$$$*/ break;
        case T_MsgsOutClient_msg_msg_ctrl_stop                          : DISPATCH_EMPTY_ASN_MSG( ctrl_stop ); break;
        case T_MsgsOutClient_msg_msg_ctrl_pause                         : DISPATCH_EMPTY_ASN_MSG( ctrl_pause ); break;
        case T_MsgsOutClient_msg_msg_ctrl_resume                        : DISPATCH_EMPTY_ASN_MSG( ctrl_resume ); break;
        case T_MsgsOutClient_msg_msg_ctrl_change_time_factor            : DISPATCH_ASN_MSG      ( ctrl_change_time_factor ); break;
        case T_MsgsOutClient_msg_msg_ctrl_meteo_globale                 : DISPATCH_ASN_MSG      ( ctrl_meteo_globale ); break;
        case T_MsgsOutClient_msg_msg_ctrl_meteo_locale                  : DISPATCH_ASN_MSG      ( ctrl_meteo_locale ); break;
        case T_MsgsOutClient_msg_msg_ctrl_checkpoint_save_now           : DISPATCH_ASN_MSG      ( ctrl_checkpoint_save_now ); break;
        case T_MsgsOutClient_msg_msg_ctrl_checkpoint_set_frequency      : DISPATCH_ASN_MSG      ( ctrl_checkpoint_set_frequency ); break;
        case T_MsgsOutClient_msg_msg_limit_creation                     : DISPATCH_ASN_MSG      ( limit_creation ); break;
        case T_MsgsOutClient_msg_msg_limit_destruction                  : DISPATCH_ASN_MSG      ( limit_destruction ); break;
        case T_MsgsOutClient_msg_msg_limit_update                       : DISPATCH_ASN_MSG      ( limit_update ); break;
        case T_MsgsOutClient_msg_msg_lima_creation                      : DISPATCH_ASN_MSG      ( lima_creation ); break;
        case T_MsgsOutClient_msg_msg_lima_destruction                   : DISPATCH_ASN_MSG      ( lima_destruction ); break;
        case T_MsgsOutClient_msg_msg_lima_update                        : DISPATCH_ASN_MSG      ( lima_update ); break;
        case T_MsgsOutClient_msg_msg_pion_order                         : DISPATCH_ASN_MSG      ( pion_order ); break;
        case T_MsgsOutClient_msg_msg_order_conduite                     : DISPATCH_ASN_MSG      ( order_conduite ); break;
        case T_MsgsOutClient_msg_msg_automate_order                     : DISPATCH_ASN_MSG      ( automate_order); break;
        case T_MsgsOutClient_msg_msg_population_order                   : DISPATCH_ASN_MSG      ( population_order ); break;
        case T_MsgsOutClient_msg_msg_set_automate_mode                  : DISPATCH_ASN_MSG      ( set_automate_mode ); break;
        case T_MsgsOutClient_msg_msg_unit_magic_action                  : DISPATCH_ASN_MSG      ( unit_magic_action ); break;
        case T_MsgsOutClient_msg_msg_object_magic_action                : DISPATCH_ASN_MSG      ( object_magic_action ); break;
        case T_MsgsOutClient_msg_msg_population_magic_action            : DISPATCH_ASN_MSG      ( population_magic_action ); break;
        case T_MsgsOutClient_msg_msg_change_diplomatie                  : DISPATCH_ASN_MSG      ( change_diplomatie ); break;
        case T_MsgsOutClient_msg_msg_change_groupe_connaissance         : DISPATCH_ASN_MSG      ( change_groupe_connaissance ); break;
        case T_MsgsOutClient_msg_msg_change_liens_logistiques           : DISPATCH_ASN_MSG      ( change_liens_logistiques ); break;
        case T_MsgsOutClient_msg_msg_change_automate                    : DISPATCH_ASN_MSG      ( change_automate ); break;
        case T_MsgsOutClient_msg_msg_log_ravitaillement_pousser_flux    : DISPATCH_ASN_MSG      ( log_ravitaillement_pousser_flux ); break;
        case T_MsgsOutClient_msg_msg_log_ravitaillement_change_quotas   : DISPATCH_ASN_MSG      ( log_ravitaillement_change_quotas ); break;
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

    AsnMessageEncoder< ASN1T_MsgsInClient, ASN1C_MsgsInClient > asnEncoder( messageService_, asnMsg );
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

// -----------------------------------------------------------------------------
// Name: Client::GetDinMsg
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
DIN_BufferedMessage Client::GetDinMsg()
{
    return DIN_BufferedMessage( messageService_ );
}
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
#include "Network_Def.h"

using namespace dispatcher;
using namespace DIN;

/****TEST****/

// 
//#define GENERATE_SEND_ASN_MSG_NOPTR_NOCTX( ASNMSG, ASNVAR )                     \
//class ASN_Msg##ASNMSG                                                           \
//{                                                                               \
//public:                                                                         \
//    ASN_Msg##ASNMSG()                                                           \
//    {                                                                           \
//        asnBaseMsg_.t = T_MsgsInSim_msg_##ASNVAR;                               \
//    }                                                                           \
//                                                                                \
//    ASN_Msg##ASNMSG( const ASN1T_Msg##ASNMSG& tmp )                             \
//    {                                                                           \
//        asnBaseMsg_.t              = T_MsgsInSim_msg_##ASNVAR;                  \
//        asnBaseMsg_.u.msg_##ASNVAR = tmp;                                       \
//    }                                                                           \
//                                                                                \
//    void Send()                                                                 \
//    {                                                                           \
///*       MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr().SendMsgOutSim( globalAsnMsg, pMOS );*/ \
//    }                                                                           \
//                                                                                \
//    ASN1T_Msg##ASNMSG& GetAsnMsg()                                              \
//    {                                                                           \
//        return asnBaseMsg_.u.msg_##ASNVAR;                                      \
//    }                                                                           \
//                                                                                \
//private:                                                                        \
//    ASN1T_MsgsInSim asnBaseMsg_;                                                \
//};
//
 
//#define GENERATE_SEND_ASN_MSG_PTR_NOCTX( ASNMSG, ASNVAR )                       \
//class ASN_Msg##ASNMSG                                                           \
//{                                                                               \
//public:                                                                         \
//    ASN_Msg##ASNMSG()                                                           \
//    {                                                                           \
//        asnBaseMsg_.t              = T_MsgsInSim_msg_##ASNVAR;                  \
//        asnBaseMsg_.u.msg_##ASNVAR = &asnMsg_;                                  \
//    }                                                                           \
//                                                                                \
//    ASN_Msg##ASNMSG(  const ASN1T_Msg##ASNMSG& tmp )                             \
//    : asnMsg_( tmp ) \
//    {                                                                           \
//        asnBaseMsg_.t              = T_MsgsInSim_msg_##ASNVAR;                  \
//        asnBaseMsg_.u.msg_##ASNVAR = &asnMsg_;   \
//    }                                                                           \
//                                                                                \
//    void Send()                                                                 \
//    {                                                                           \
///*       MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr().SendMsgOutSim( globalAsnMsg, pMOS );*/ \
//    }                                                                           \
//                                                                                \
//    ASN1T_Msg##ASNMSG& GetAsnMsg()                                              \
//    {                                                                           \
//        return *asnBaseMsg_.u.msg_##ASNVAR;                                     \
//    }                                                                           \
//                                                                                \
//private:                                                                        \
//    ASN1T_MsgsInSim   asnBaseMsg_;                                              \
//    ASN1T_Msg##ASNMSG asnMsg_;                                                  \
//};
//
//
//OnReceiveMsgCtrlClientAnnouncement
//
//GENERATE_SEND_ASN_MSG_NOPTR_NOCTX( CtrlClientAnnouncement, ctrl_client_announcement )
//GENERATE_SEND_ASN_MSG_PTR_NOCTX  ( CtrlMeteoGlobale      , ctrl_meteo_globale       )

//T_MsgsOutClient_msg_ctrl_meteo_globale

/*



NET_GENERATE_SEND_ASN_MSG_NOPTR_NOCTX( CtrlBeginTick                , ctrl_begin_tick                   )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX  ( CtrlEndTick                  , ctrl_end_tick                     )
NET_GENERATE_SEND_ASN_MSG_NOPTR_NOCTX( CtrlStopAck                  , ctrl_stop_ack                     )
NET_GENERATE_SEND_ASN_MSG_NOPTR_NOCTX( CtrlResumeAck                , ctrl_resume_ack                   )
NET_GENERATE_SEND_ASN_MSG_NOPTR_NOCTX( CtrlPauseAck                 , ctrl_pause_ack                    )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX  ( CtrlChangeTimeFactorAck      , ctrl_change_time_factor_ack       )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX  ( CtrlInfo                     , ctrl_info                         )
NET_GENERATE_SEND_ASN_MSG_NOMSG_NOCTX( CtrlMeteoGlobaleAck          , ctrl_meteo_globale_ack            )
NET_GENERATE_SEND_ASN_MSG_NOMSG_NOCTX( CtrlMeteoLocaleAck           , ctrl_meteo_locale_ack             )
NET_GENERATE_SEND_ASN_MSG_NOMSG_NOCTX( CtrlCheckPointSaveBegin      , ctrl_checkpoint_save_begin        )
NET_GENERATE_SEND_ASN_MSG_NOMSG_NOCTX( CtrlCheckPointLoadBegin      , ctrl_checkpoint_load_begin        )
NET_GENERATE_SEND_ASN_MSG_NOMSG_NOCTX( CtrlCheckPointSaveEnd        , ctrl_checkpoint_save_end          )
NET_GENERATE_SEND_ASN_MSG_NOMSG_NOCTX( CtrlCheckPointLoadEnd        , ctrl_checkpoint_load_end          )
NET_GENERATE_SEND_ASN_MSG_NOMSG_NOCTX( CtrlCheckPointSetFrequencyAck, ctrl_checkpoint_set_frequency_ack )
*/


// -----------------------------------------------------------------------------
// Name: Client constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
Client::Client( Dispatcher& dispatcher, DIN_MessageService_ABC& messageService, DIN_Link& link )
    : DIN_UserData_ABC()
    , dispatcher_     ( dispatcher )
    , messageService_ ( messageService )
    , link_           ( link )
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

// =============================================================================
// RIGHTS MANAGEMENT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Client::CheckRights
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
bool Client::CheckRights( const ASN1T_MsgsOutClient& asnMsg ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: Client::CheckRights
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
bool Client::CheckRights( const ASN1T_MsgsInClient& asnMsg ) const
{
    return true;
}

// =============================================================================
// MESSAGES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Client::OnReceiveMsgCtrlClientAnnouncement
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
void Client::OnReceiveMsgCtrlClientAnnouncement( const ASN1T_MsgCtrlClientAnnouncement& asnMsg )
{
    ///// profiles ....
}

// 
#define TRANSLATE_ASN_MSG( NAME )                                   \
    {                                                               \
        asnOutMsg.context          = asnInMsg.context;              \
        asnOutMsg.msg.t            = T_MsgsInSim_msg_msg_##NAME;    \
        asnOutMsg.msg.u.msg_##NAME = asnInMsg.msg.u.msg_##NAME;     \
        dispatcher_.DispatchToSimulation( asnOutMsg) ;              \
    } 

#define TRANSLATE_EMPTY_ASN_MSG( NAME )                         \
    {                                                           \
        asnOutMsg.context = asnInMsg.context;                   \
        asnOutMsg.msg.t   = T_MsgsInSim_msg_msg_##NAME;         \
        dispatcher_.DispatchToSimulation( asnOutMsg) ;          \
    } 

// -----------------------------------------------------------------------------
// Name: Client::OnReceive
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
void Client::OnReceive( const ASN1T_MsgsOutClient& asnInMsg )
{
    if( !CheckRights( asnInMsg ) )
    {
        link_.Close( true ); // Disconnect
        return;
    }

    ASN1T_MsgsInSim asnOutMsg;
    switch( asnInMsg.msg.t )
    {
        case T_MsgsOutClient_msg_msg_ctrl_client_announcement           : OnReceiveMsgCtrlClientAnnouncement( asnInMsg.msg.u.msg_ctrl_client_announcement ); break;
        case T_MsgsOutClient_msg_msg_ctrl_stop                          : TRANSLATE_EMPTY_ASN_MSG( ctrl_stop ); break;
        case T_MsgsOutClient_msg_msg_ctrl_pause                         : TRANSLATE_EMPTY_ASN_MSG( ctrl_pause ); break;
        case T_MsgsOutClient_msg_msg_ctrl_resume                        : TRANSLATE_EMPTY_ASN_MSG( ctrl_resume ); break;
        case T_MsgsOutClient_msg_msg_ctrl_change_time_factor            : TRANSLATE_ASN_MSG      ( ctrl_change_time_factor ); break;
        case T_MsgsOutClient_msg_msg_ctrl_meteo_globale                 : TRANSLATE_ASN_MSG      ( ctrl_meteo_globale ); break;
        case T_MsgsOutClient_msg_msg_ctrl_meteo_locale                  : TRANSLATE_ASN_MSG      ( ctrl_meteo_locale ); break;
        case T_MsgsOutClient_msg_msg_ctrl_checkpoint_save_now           : TRANSLATE_ASN_MSG      ( ctrl_checkpoint_save_now ); break;
        case T_MsgsOutClient_msg_msg_ctrl_checkpoint_set_frequency      : TRANSLATE_ASN_MSG      ( ctrl_checkpoint_set_frequency ); break;
        case T_MsgsOutClient_msg_msg_limit_creation                     : TRANSLATE_ASN_MSG      ( limit_creation ); break;
        case T_MsgsOutClient_msg_msg_limit_destruction                  : TRANSLATE_ASN_MSG      ( limit_destruction ); break;
        case T_MsgsOutClient_msg_msg_limit_update                       : TRANSLATE_ASN_MSG      ( limit_update ); break;
        case T_MsgsOutClient_msg_msg_lima_creation                      : TRANSLATE_ASN_MSG      ( lima_creation ); break;
        case T_MsgsOutClient_msg_msg_lima_destruction                   : TRANSLATE_ASN_MSG      ( lima_destruction ); break;
        case T_MsgsOutClient_msg_msg_lima_update                        : TRANSLATE_ASN_MSG      ( lima_update ); break;
        case T_MsgsOutClient_msg_msg_pion_order                         : TRANSLATE_ASN_MSG      ( pion_order ); break;
        case T_MsgsOutClient_msg_msg_order_conduite                     : TRANSLATE_ASN_MSG      ( order_conduite ); break;
        case T_MsgsOutClient_msg_msg_automate_order                     : TRANSLATE_ASN_MSG      ( automate_order); break;
        case T_MsgsOutClient_msg_msg_population_order                   : TRANSLATE_ASN_MSG      ( population_order ); break;
        case T_MsgsOutClient_msg_msg_set_automate_mode                  : TRANSLATE_ASN_MSG      ( set_automate_mode ); break;
        case T_MsgsOutClient_msg_msg_unit_magic_action                  : TRANSLATE_ASN_MSG      ( unit_magic_action ); break;
        case T_MsgsOutClient_msg_msg_object_magic_action                : TRANSLATE_ASN_MSG      ( object_magic_action ); break;
        case T_MsgsOutClient_msg_msg_population_magic_action            : TRANSLATE_ASN_MSG      ( population_magic_action ); break;
        case T_MsgsOutClient_msg_msg_change_diplomatie                  : TRANSLATE_ASN_MSG      ( change_diplomatie ); break;
        case T_MsgsOutClient_msg_msg_change_groupe_connaissance         : TRANSLATE_ASN_MSG      ( change_groupe_connaissance ); break;
        case T_MsgsOutClient_msg_msg_change_liens_logistiques           : TRANSLATE_ASN_MSG      ( change_liens_logistiques ); break;
        case T_MsgsOutClient_msg_msg_change_automate                    : TRANSLATE_ASN_MSG      ( change_automate ); break;
        case T_MsgsOutClient_msg_msg_log_ravitaillement_pousser_flux    : TRANSLATE_ASN_MSG      ( log_ravitaillement_pousser_flux ); break;
        case T_MsgsOutClient_msg_msg_log_ravitaillement_change_quotas   : TRANSLATE_ASN_MSG      ( log_ravitaillement_change_quotas ); break;
        default:
            assert( false );
    }
}

// -----------------------------------------------------------------------------
// Name: Client::OnReceive
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
void Client::OnReceive( unsigned int nMsgID, DIN::DIN_Input& input )
{
    DIN_BufferedMessage copiedMsg( messageService_ );
    copiedMsg.GetOutput().Append( input.GetBuffer( 0 ), input.GetAvailable() );
    dispatcher_.DispatchToSimulation( nMsgID, copiedMsg );
}

// =============================================================================
// 
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Client::HasReadRights
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
//bool Client::HasReadRights( const ASN1T_MsgSimMos& asn )
//{
//    switch( asn.t )
//    {
//        case T_MsgsSimMos_msg_ctrl_info                 1
//        case T_MsgsSimMos_msg_ctrl_begin_tick 2
//        case T_MsgsSimMos_msg_ctrl_end_tick  3
//        case T_MsgsSimMos_msg_ctrl_stop_ack  4
//        case T_MsgsSimMos_msg_ctrl_pause_ack 5
//        case T_MsgsSimMos_msg_ctrl_resume_ack 6
//        case T_MsgsSimMos_msg_ctrl_change_time_factor_ack 7
//        case T_MsgsSimMos_msg_ctrl_meteo_globale_ack 8
//        case T_MsgsSimMos_msg_ctrl_meteo_locale_ack 9
//        case T_MsgsSimMos_msg_ctrl_checkpoint_save_begin 10
//        case T_MsgsSimMos_msg_ctrl_checkpoint_save_end 11
//        case T_MsgsSimMos_msg_ctrl_checkpoint_load_begin 12
//        case T_MsgsSimMos_msg_ctrl_checkpoint_load_end 13
//        case T_MsgsSimMos_msg_ctrl_checkpoint_set_frequency_ack 14
//        case T_MsgsSimMos_msg_ctrl_checkpoint_save_now_ack 15
//        case T_MsgsSimMos_msg_ctrl_send_current_state_begin 16
//        case T_MsgsSimMos_msg_ctrl_send_current_state_end 17
//        case T_MsgsSimMos_msg_limit_creation 18
//        case T_MsgsSimMos_msg_limit_destruction 19
//        case T_MsgsSimMos_msg_lima_creation  20
//        case T_MsgsSimMos_msg_lima_destruction 21
//        case T_MsgsSimMos_msg_automate_creation                    : return CheckReadRightsOnAutomate( asn.u.msg_automate_creation->oid_automate );
//        case T_MsgsSimMos_msg_pion_creation                        : return CheckReadRightsOnPion    ( asn.u.msg_pion_creation->oid_pion );
//        case T_MsgsSimMos_msg_change_diplomatie                    : return CheckReadRightsOn
//        case T_MsgsSimMos_msg_unit_knowledge_creation              : return CheckReadRightsOn
//        case T_MsgsSimMos_msg_unit_knowledge_update                : return CheckReadRightsOn
//        case T_MsgsSimMos_msg_unit_knowledge_destruction           : return CheckReadRightsOn
//        case T_MsgsSimMos_msg_unit_attributes                      : return CheckReadRightsOn
//        case T_MsgsSimMos_msg_unit_dotations                       : return CheckReadRightsOn
//        case T_MsgsSimMos_msg_unit_pathfind                        : return CheckReadRightsOn
//        case T_MsgsSimMos_msg_start_pion_fire                      : return CheckReadRightsOn
//        case T_MsgsSimMos_msg_stop_pion_fire                       : return CheckReadRightsOn
//        case T_MsgsSimMos_msg_start_population_fire                : return CheckReadRightsOn
//        case T_MsgsSimMos_msg_stop_population_fire                 : return CheckReadRightsOn
//        case T_MsgsSimMos_msg_explosion                            : return CheckReadRightsOn
//        case T_MsgsSimMos_msg_start_fire_effect                    : return CheckReadRightsOn
//        case T_MsgsSimMos_msg_stop_fire_effect                     : return CheckReadRightsOn
//        case T_MsgsSimMos_msg_cr                                   : return CheckReadRightsOn
//        case T_MsgsSimMos_msg_order_management                     : return CheckReadRightsOn
//        case T_MsgsSimMos_msg_automate_mrt                         : return CheckReadRightsOn
//        case T_MsgsSimMos_msg_pion_order                           : return CheckReadRightsOn
//        case T_MsgsSimMos_msg_automate_order                       : return CheckReadRightsOn
//        case T_MsgsSimMos_msg_object_creation                      : return CheckReadRightsOn
//        case T_MsgsSimMos_msg_object_destruction                   : return CheckReadRightsOn
//        case T_MsgsSimMos_msg_object_update                        : return CheckReadRightsOn
//        case T_MsgsSimMos_msg_object_knowledge_creation            : return CheckReadRightsOn
//        case T_MsgsSimMos_msg_object_knowledge_update              : return CheckReadRightsOn
//        case T_MsgsSimMos_msg_object_knowledge_destruction         : return CheckReadRightsOn
//        case T_MsgsSimMos_msg_change_automate                      : return CheckReadRightsOn
//        case T_MsgsSimMos_msg_log_sante_traitement_humain_creation : return CheckReadRightsOn
//        case T_MsgsSimMos_msg_log_sante_traitement_humain_update 51
//        case T_MsgsSimMos_msg_log_sante_traitement_humain_destruction 52
//        case T_MsgsSimMos_msg_log_sante_etat 53
//        case T_MsgsSimMos_msg_log_maintenance_traitement_equipement_creation 54
//        case T_MsgsSimMos_msg_log_maintenance_traitement_equipement_update 55
//        case T_MsgsSimMos_msg_log_maintenance_traitement_equipement_destruction 56
//        case T_MsgsSimMos_msg_log_maintenance_etat 57
//        case T_MsgsSimMos_msg_log_ravitaillement_traitement_creation 58
//        case T_MsgsSimMos_msg_log_ravitaillement_traitement_update 59
//        case T_MsgsSimMos_msg_log_ravitaillement_traitement_destruction 60
//        case T_MsgsSimMos_msg_log_ravitaillement_etat 61
//        case T_MsgsSimMos_msg_log_ravitaillement_quotas 62
//        case T_MsgsSimMos_msg_population_creation 63
//        case T_MsgsSimMos_msg_population_update 64
//        case T_MsgsSimMos_msg_population_concentration_creation 65
//        case T_MsgsSimMos_msg_population_concentration_destruction 66
//        case T_MsgsSimMos_msg_population_concentration_update 67
//        case T_MsgsSimMos_msg_population_flux_creation 68
//        case T_MsgsSimMos_msg_population_flux_destruction 69
//        case T_MsgsSimMos_msg_population_flux_update 70
//        case T_MsgsSimMos_msg_population_knowledge_creation 71
//        case T_MsgsSimMos_msg_population_knowledge_update 72
//        case T_MsgsSimMos_msg_population_knowledge_destruction 73
//        case T_MsgsSimMos_msg_population_concentration_knowledge_creation 74
//        case T_MsgsSimMos_msg_population_concentration_knowledge_destruction 75
//        case T_MsgsSimMos_msg_population_concentration_knowledge_update 76
//        case T_MsgsSimMos_msg_population_flux_knowledge_creation 77
//        case T_MsgsSimMos_msg_population_flux_knowledge_destruction 78
//        case T_MsgsSimMos_msg_population_flux_knowledge_update 79
//#define T_MsgsSimMos_nb_choice          80
//
//    }
//}

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
    // RIGHTS ??
    messageService_.Send( link_, nMsgID, dinMsg );
}

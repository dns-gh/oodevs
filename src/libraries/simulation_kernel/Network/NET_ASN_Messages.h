//*****************************************************************************
//
// $Created: NLD 2003-02-24 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Network/NET_ASN_Messages.h $
// $Author: Nld $
// $Modtime: 11/03/05 16:31 $
// $Revision: 8 $
// $Workfile: NET_ASN_Messages.h $
//
//*****************************************************************************

#ifndef __NET_ASN_Messages_h_
#define __NET_ASN_Messages_h_

#include "MIL.h"

#include "MIL_AgentServer.h"
#include "Network/NET_AgentServer.h"
#include "Network/NET_AS_MOSServerMsgMgr.h"
#include "game_asn/Asn.h"

//=============================================================================
// ASN ENCODER WRAPPER MACROS
//=============================================================================

// ASN Message which isn't a pointer and doesn't take any context
#define NET_GENERATE_SEND_ASN_MSG_NOPTR( ASNMSG, ASNVAR )                   \
class NET_ASN_Msg##ASNMSG                                                   \
{                                                                           \
public:                                                                     \
    void Send( uint nCtx = 0 )                                  \
    {                                                                       \
        ASN1T_MsgsOutSim    globalAsnMsg;                                   \
                                                                            \
        globalAsnMsg.context             = nCtx;                            \
        globalAsnMsg.msg.t               = T_MsgsOutSim_msg_msg_##ASNVAR;   \
        globalAsnMsg.msg.u.msg_##ASNVAR  = asnMsg_;                         \
        MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr().SendMsgOutSim( globalAsnMsg ); \
    }                                                                       \
                                                                            \
    ASN1T_Msg##ASNMSG& operator()() { return asnMsg_; }                     \
                                                                            \
private:                                                                    \
    ASN1T_Msg##ASNMSG asnMsg_;                                              \
}; 


// ASN Message which is a pointer and doesn't take any context
#define NET_GENERATE_SEND_ASN_MSG_PTR( ASNMSG, ASNVAR )                     \
class NET_ASN_Msg##ASNMSG                                                   \
{                                                                           \
public:                                                                     \
    void Send( uint nCtx = 0 )                                  \
    {                                                                       \
        ASN1T_MsgsOutSim    globalAsnMsg;                                   \
                                                                            \
        globalAsnMsg.context             = nCtx;                            \
        globalAsnMsg.msg.t               = T_MsgsOutSim_msg_msg_##ASNVAR;   \
        globalAsnMsg.msg.u.msg_##ASNVAR  = &asnMsg_;                        \
        MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr().SendMsgOutSim( globalAsnMsg ); \
    }                                                                       \
                                                                            \
    ASN1T_Msg##ASNMSG& operator()() { return asnMsg_; }                     \
                                                                            \
private:                                                                    \
    ASN1T_Msg##ASNMSG asnMsg_;                                              \
};
// ASN Message which is a pointer and take any context
#define NET_GENERATE_SEND_ASN_MSG_NOMSG( ASNMSG, ASNVAR )                           \
class NET_ASN_Msg##ASNMSG                                                           \
{                                                                                   \
public:                                                                             \
    void Send( uint nCtx = 0 )                                          \
    {                                                                               \
        ASN1T_MsgsOutSim    globalAsnMsg;                                           \
                                                                                    \
        globalAsnMsg.context          = nCtx;                                       \
        globalAsnMsg.msg.t            = T_MsgsOutSim_msg_msg_##ASNVAR;              \
        MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr().SendMsgOutSim( globalAsnMsg ); \
    }                                                                               \
}; 

//=============================================================================
// GENERATE ASN MESSAGES
//=============================================================================

// Control
NET_GENERATE_SEND_ASN_MSG_NOPTR( CtrlBeginTick                , ctrl_begin_tick                   )
NET_GENERATE_SEND_ASN_MSG_PTR  ( CtrlEndTick                  , ctrl_end_tick                     )
NET_GENERATE_SEND_ASN_MSG_NOPTR( CtrlStopAck                  , ctrl_stop_ack                     )
NET_GENERATE_SEND_ASN_MSG_NOPTR( CtrlResumeAck                , ctrl_resume_ack                   )
NET_GENERATE_SEND_ASN_MSG_NOPTR( CtrlPauseAck                 , ctrl_pause_ack                    )
NET_GENERATE_SEND_ASN_MSG_PTR  ( CtrlChangeTimeFactorAck      , ctrl_change_time_factor_ack       )
NET_GENERATE_SEND_ASN_MSG_PTR  ( CtrlInfo                     , ctrl_info                         )
NET_GENERATE_SEND_ASN_MSG_NOMSG( CtrlMeteoGlobaleAck          , ctrl_meteo_globale_ack            )
NET_GENERATE_SEND_ASN_MSG_NOMSG( CtrlMeteoLocaleAck           , ctrl_meteo_locale_ack             )
NET_GENERATE_SEND_ASN_MSG_NOMSG( CtrlCheckPointSaveBegin      , ctrl_checkpoint_save_begin        )
NET_GENERATE_SEND_ASN_MSG_NOMSG( CtrlCheckPointSaveEnd        , ctrl_checkpoint_save_end          )
NET_GENERATE_SEND_ASN_MSG_NOMSG( CtrlCheckPointSetFrequencyAck, ctrl_checkpoint_set_frequency_ack )
NET_GENERATE_SEND_ASN_MSG_NOMSG( CtrlCheckPointSaveNowAck     , ctrl_checkpoint_save_now_ack      )
NET_GENERATE_SEND_ASN_MSG_NOMSG( CtrlSendCurrentStateBegin    , ctrl_send_current_state_begin     )
NET_GENERATE_SEND_ASN_MSG_NOMSG( CtrlSendCurrentStateEnd      , ctrl_send_current_state_end       )
                                                                                            
// Unit
NET_GENERATE_SEND_ASN_MSG_PTR( PionCreation       , pion_creation         )
NET_GENERATE_SEND_ASN_MSG_PTR( UnitAttributes     , unit_attributes       )
NET_GENERATE_SEND_ASN_MSG_PTR( UnitMagicActionAck , unit_magic_action_ack )
NET_GENERATE_SEND_ASN_MSG_PTR( UnitPathFind       , unit_pathfind         )

// Automate
NET_GENERATE_SEND_ASN_MSG_PTR( AutomateCreation   , automate_creation   )
NET_GENERATE_SEND_ASN_MSG_PTR( AutomateAttributes , automate_attributes )

// Army / formation / knowledge groups
NET_GENERATE_SEND_ASN_MSG_PTR( FormationCreation     , formation_creation       )
NET_GENERATE_SEND_ASN_MSG_PTR( KnowledgeGroupCreation, knowledge_group_creation )
NET_GENERATE_SEND_ASN_MSG_PTR( SideCreation          , side_creation            )

// Hierarchie update
NET_GENERATE_SEND_ASN_MSG_PTR( ChangeDiplomatieAck        , change_diplomatie_ack          )
NET_GENERATE_SEND_ASN_MSG_PTR( ChangeGroupeConnaissanceAck, change_groupe_connaissance_ack )
NET_GENERATE_SEND_ASN_MSG_PTR( ChangeLiensLogistiquesAck  , change_liens_logistiques_ack   )
NET_GENERATE_SEND_ASN_MSG_PTR( ChangeLiensLogistiques     , change_liens_logistiques       )
NET_GENERATE_SEND_ASN_MSG_PTR( ChangeAutomate             , change_automate                )
NET_GENERATE_SEND_ASN_MSG_PTR( ChangeAutomateAck          , change_automate_ack            )
NET_GENERATE_SEND_ASN_MSG_PTR( ChangeDiplomatie           , change_diplomatie              )

// Logistic
NET_GENERATE_SEND_ASN_MSG_PTR( LogMaintenanceTraitementEquipementCreation   , log_maintenance_traitement_equipement_creation    )
NET_GENERATE_SEND_ASN_MSG_PTR( LogMaintenanceTraitementEquipementUpdate     , log_maintenance_traitement_equipement_update      )
NET_GENERATE_SEND_ASN_MSG_PTR( LogMaintenanceTraitementEquipementDestruction, log_maintenance_traitement_equipement_destruction )
NET_GENERATE_SEND_ASN_MSG_PTR( LogMaintenanceEtat                           , log_maintenance_etat                              )
NET_GENERATE_SEND_ASN_MSG_PTR( LogSanteTraitementHumainCreation             , log_sante_traitement_humain_creation              )
NET_GENERATE_SEND_ASN_MSG_PTR( LogSanteTraitementHumainUpdate               , log_sante_traitement_humain_update                )
NET_GENERATE_SEND_ASN_MSG_PTR( LogSanteTraitementHumainDestruction          , log_sante_traitement_humain_destruction           )
NET_GENERATE_SEND_ASN_MSG_PTR( LogSanteEtat                                 , log_sante_etat                                    )
NET_GENERATE_SEND_ASN_MSG_PTR( LogRavitaillementTraitementCreation          , log_ravitaillement_traitement_creation            )
NET_GENERATE_SEND_ASN_MSG_PTR( LogRavitaillementTraitementUpdate            , log_ravitaillement_traitement_update              )
NET_GENERATE_SEND_ASN_MSG_PTR( LogRavitaillementTraitementDestruction       , log_ravitaillement_traitement_destruction         )
NET_GENERATE_SEND_ASN_MSG_PTR( LogRavitaillementEtat                        , log_ravitaillement_etat                           )
NET_GENERATE_SEND_ASN_MSG_PTR( LogRavitaillementQuotas                      , log_ravitaillement_quotas                         )

// Context infos
NET_GENERATE_SEND_ASN_MSG_NOPTR( LimitCreationRequestAck   , limit_creation_request_ack    )
NET_GENERATE_SEND_ASN_MSG_NOPTR( LimitDestructionRequestAck, limit_destruction_request_ack ) 
NET_GENERATE_SEND_ASN_MSG_NOPTR( LimitUpdateRequestAck     , limit_update_request_ack      ) 
NET_GENERATE_SEND_ASN_MSG_NOPTR( LimaCreationRequestAck    , lima_creation_request_ack     ) 
NET_GENERATE_SEND_ASN_MSG_NOPTR( LimaDestructionRequestAck , lima_destruction_request_ack  )
NET_GENERATE_SEND_ASN_MSG_NOPTR( LimaUpdateRequestAck      , lima_update_request_ack       )
NET_GENERATE_SEND_ASN_MSG_PTR  ( LimitCreation             , limit_creation                )
NET_GENERATE_SEND_ASN_MSG_PTR  ( LimitUpdate               , limit_update                  )
NET_GENERATE_SEND_ASN_MSG_NOPTR( LimitDestruction          , limit_destruction             )
NET_GENERATE_SEND_ASN_MSG_PTR  ( LimaCreation              , lima_creation                 )
NET_GENERATE_SEND_ASN_MSG_PTR  ( LimaUpdate                , lima_update                   )
NET_GENERATE_SEND_ASN_MSG_NOPTR( LimaDestruction           , lima_destruction              )

// Fire
NET_GENERATE_SEND_ASN_MSG_PTR  ( StartPionFire      , start_pion_fire       )
NET_GENERATE_SEND_ASN_MSG_PTR  ( StartPopulationFire, start_population_fire )
NET_GENERATE_SEND_ASN_MSG_PTR  ( StopPionFire       , stop_pion_fire        )
NET_GENERATE_SEND_ASN_MSG_PTR  ( StopPopulationFire , stop_population_fire  )
NET_GENERATE_SEND_ASN_MSG_PTR  ( Explosion          , explosion             )
NET_GENERATE_SEND_ASN_MSG_PTR  ( StartFireEffect    , start_fire_effect     )
NET_GENERATE_SEND_ASN_MSG_NOPTR( StopFireEffect     , stop_fire_effect      )

// Unit knowledge
NET_GENERATE_SEND_ASN_MSG_PTR ( UnitKnowledgeCreation   , unit_knowledge_creation    )
NET_GENERATE_SEND_ASN_MSG_PTR ( UnitKnowledgeUpdate     , unit_knowledge_update      )
NET_GENERATE_SEND_ASN_MSG_PTR ( UnitKnowledgeDestruction, unit_knowledge_destruction )

// Orders
NET_GENERATE_SEND_ASN_MSG_PTR( PionOrderManagement      , pion_order_management       )
NET_GENERATE_SEND_ASN_MSG_PTR( AutomateOrderManagement  , automate_order_management   )
NET_GENERATE_SEND_ASN_MSG_PTR( PopulationOrderManagement, population_order_management )
NET_GENERATE_SEND_ASN_MSG_PTR( PionOrder                , pion_order                  )
NET_GENERATE_SEND_ASN_MSG_PTR( PionOrderAck             , pion_order_ack              )
NET_GENERATE_SEND_ASN_MSG_PTR( AutomateOrder            , automate_order              )
NET_GENERATE_SEND_ASN_MSG_PTR( AutomateOrderAck         , automate_order_ack          )
NET_GENERATE_SEND_ASN_MSG_PTR( PopulationOrder          , population_order            )
NET_GENERATE_SEND_ASN_MSG_PTR( PopulationOrderAck       , population_order_ack        )
NET_GENERATE_SEND_ASN_MSG_PTR( FragOrderAck             , frag_order_ack              )

// Misc
NET_GENERATE_SEND_ASN_MSG_PTR( SetAutomateModeAck       , set_automate_mode_ack       )
NET_GENERATE_SEND_ASN_MSG_PTR( PopulationMagicActionAck , population_magic_action_ack )

// CR
NET_GENERATE_SEND_ASN_MSG_PTR( CR, cr )

// Objects
NET_GENERATE_SEND_ASN_MSG_PTR  ( ObjectCreation       , object_creation         )
NET_GENERATE_SEND_ASN_MSG_NOPTR( ObjectDestruction    , object_destruction      )
NET_GENERATE_SEND_ASN_MSG_PTR  ( ObjectUpdate         , object_update           )
NET_GENERATE_SEND_ASN_MSG_PTR  ( ObjectMagicActionAck , object_magic_action_ack )

// Object knowledge
NET_GENERATE_SEND_ASN_MSG_PTR( ObjectKnowledgeCreation   , object_knowledge_creation    )
NET_GENERATE_SEND_ASN_MSG_PTR( ObjectKnowledgeUpdate     , object_knowledge_update      )
NET_GENERATE_SEND_ASN_MSG_PTR( ObjectKnowledgeDestruction, object_knowledge_destruction )

// Logistique - Rav
NET_GENERATE_SEND_ASN_MSG_NOPTR( LogRavitaillementChangeQuotasAck, log_ravitaillement_change_quotas_ack )
NET_GENERATE_SEND_ASN_MSG_NOPTR( LogRavitaillementPousserFluxAck , log_ravitaillement_pousser_flux_ack  )

// Population
NET_GENERATE_SEND_ASN_MSG_PTR( PopulationCreation                , population_creation                  )
NET_GENERATE_SEND_ASN_MSG_PTR( PopulationUpdate                  , population_update                    )
NET_GENERATE_SEND_ASN_MSG_PTR( PopulationConcentrationCreation   , population_concentration_creation    )
NET_GENERATE_SEND_ASN_MSG_PTR( PopulationConcentrationDestruction, population_concentration_destruction )
NET_GENERATE_SEND_ASN_MSG_PTR( PopulationConcentrationUpdate     , population_concentration_update      )
NET_GENERATE_SEND_ASN_MSG_PTR( PopulationFluxCreation            , population_flux_creation             )
NET_GENERATE_SEND_ASN_MSG_PTR( PopulationFluxDestruction         , population_flux_destruction          )
NET_GENERATE_SEND_ASN_MSG_PTR( PopulationFluxUpdate              , population_flux_update               )

// Population knowledge
NET_GENERATE_SEND_ASN_MSG_PTR( PopulationKnowledgeCreation                , population_knowledge_creation                  )
NET_GENERATE_SEND_ASN_MSG_PTR( PopulationKnowledgeUpdate                  , population_knowledge_update                    )
NET_GENERATE_SEND_ASN_MSG_PTR( PopulationKnowledgeDestruction             , population_knowledge_destruction               )
NET_GENERATE_SEND_ASN_MSG_PTR( PopulationConcentrationKnowledgeCreation   , population_concentration_knowledge_creation    )
NET_GENERATE_SEND_ASN_MSG_PTR( PopulationConcentrationKnowledgeDestruction, population_concentration_knowledge_destruction )
NET_GENERATE_SEND_ASN_MSG_PTR( PopulationConcentrationKnowledgeUpdate     , population_concentration_knowledge_update      )
NET_GENERATE_SEND_ASN_MSG_PTR( PopulationFluxKnowledgeCreation            , population_flux_knowledge_creation             )
NET_GENERATE_SEND_ASN_MSG_PTR( PopulationFluxKnowledgeDestruction         , population_flux_knowledge_destruction          )
NET_GENERATE_SEND_ASN_MSG_PTR( PopulationFluxKnowledgeUpdate              , population_flux_knowledge_update               )

#endif // __NET_ASN_Messages_h_




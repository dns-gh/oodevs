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
#include "Network/NET_ASN_Types.h"

//=============================================================================
// ASN ENCODER WRAPPER MACROS
//=============================================================================

// ASN Message which isn't a pointer and doesn't take any context
#define NET_GENERATE_SEND_ASN_MSG_NOPTR_NOCTX( ASNMSG, ASNVAR )             \
class NET_ASN_Msg##ASNMSG                                                   \
{                                                                           \
public:                                                                     \
    void Send( NET_AS_MOSServer* pMOS = 0 )                                 \
    {                                                                       \
        ASN1T_MsgsSimMos    globalAsnMsg;                                   \
                                                                            \
        globalAsnMsg.t              = T_MsgsSimMos_msg_##ASNVAR;            \
        globalAsnMsg.u.msg_##ASNVAR  = asnMsg_;                             \
        MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr().SendMsgSimMos( globalAsnMsg, pMOS ); \
    }                                                                       \
                                                                            \
    ASN1T_Msg##ASNMSG& GetAsnMsg()                                          \
    {                                                                       \
        return asnMsg_;                                                     \
    }                                                                       \
                                                                            \
private:                                                                    \
    ASN1T_Msg##ASNMSG asnMsg_;                                              \
}; 


// ASN Message which is a pointer and doesn't take any context
#define NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( ASNMSG, ASNVAR )               \
class NET_ASN_Msg##ASNMSG                                                   \
{                                                                           \
public:                                                                     \
    void Send( NET_AS_MOSServer* pMOS = 0 )                                 \
    {                                                                       \
        ASN1T_MsgsSimMos    globalAsnMsg;                                   \
                                                                            \
        globalAsnMsg.t             = T_MsgsSimMos_msg_##ASNVAR;             \
        globalAsnMsg.u.msg_##ASNVAR = &asnMsg_;                             \
        MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr().SendMsgSimMos( globalAsnMsg, pMOS ); \
    }                                                                       \
                                                                            \
    ASN1T_Msg##ASNMSG& GetAsnMsg()                                          \
    {                                                                       \
        return asnMsg_;                                                     \
    }                                                                       \
                                                                            \
private:                                                                    \
    ASN1T_Msg##ASNMSG asnMsg_;                                              \
};

// ASN Message which isn't a pointer and take any context
#define NET_GENERATE_SEND_ASN_MSG_NOPTR_CTX( ASNMSG, ASNVAR )                       \
class NET_ASN_Msg##ASNMSG                                                           \
{                                                                                   \
public:                                                                             \
    void Send( MIL_MOSContextID nCtx, NET_AS_MOSServer* pMOS = 0 )                  \
    {                                                                               \
        ASN1T_MsgsSimMosWithContext    globalAsnMsg;                                \
                                                                                    \
        globalAsnMsg.t             = T_MsgsSimMosWithContext_msg_##ASNVAR;          \
        globalAsnMsg.u.msg_##ASNVAR = asnMsg_;                                      \
        MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr().SendMsgSimMosWithContext( globalAsnMsg, nCtx, pMOS ); \
    }                                                                               \
                                                                                    \
    ASN1T_Msg##ASNMSG& GetAsnMsg()                                                  \
    {                                                                               \
        return asnMsg_;                                                             \
    }                                                                               \
                                                                                    \
private:                                                                            \
    ASN1T_Msg##ASNMSG asnMsg_;                                                      \
};

// ASN Message which is a pointer and take any context
#define NET_GENERATE_SEND_ASN_MSG_PTR_CTX( ASNMSG, ASNVAR )                         \
class NET_ASN_Msg##ASNMSG                                                           \
{                                                                                   \
public:                                                                             \
    void Send( MIL_MOSContextID nCtx, NET_AS_MOSServer* pMOS = 0 )                  \
    {                                                                               \
        ASN1T_MsgsSimMosWithContext    globalAsnMsg;                                \
                                                                                    \
        globalAsnMsg.t             = T_MsgsSimMosWithContext_msg_##ASNVAR;          \
        globalAsnMsg.u.msg_##ASNVAR = &asnMsg_;                                     \
        MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr().SendMsgSimMosWithContext( globalAsnMsg, nCtx, pMOS ); \
    }                                                                               \
                                                                                    \
    ASN1T_Msg##ASNMSG& GetAsnMsg()                                                  \
    {                                                                               \
        return asnMsg_;                                                             \
    }                                                                               \
                                                                                    \
private:                                                                            \
    ASN1T_Msg##ASNMSG asnMsg_;                                                      \
};                                                          


// ASN Message which isn't a pointer and take any context
#define NET_GENERATE_SEND_ASN_MSG_NOMSG_CTX( ASNMSG, ASNVAR )                       \
class NET_ASN_Msg##ASNMSG                                                           \
{                                                                                   \
public:                                                                             \
    void Send( MIL_MOSContextID nCtx, NET_AS_MOSServer* pMOS = 0 )                  \
    {                                                                               \
        ASN1T_MsgsSimMosWithContext    globalAsnMsg;                                \
                                                                                    \
        globalAsnMsg.t             = T_MsgsSimMosWithContext_msg_##ASNVAR;          \
        MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr().SendMsgSimMosWithContext( globalAsnMsg, nCtx, pMOS ); \
    }                                                                               \
};

// ASN Message which is a pointer and take any context
#define NET_GENERATE_SEND_ASN_MSG_NOMSG_NOCTX( ASNMSG, ASNVAR )                     \
class NET_ASN_Msg##ASNMSG                                                           \
{                                                                                   \
public:                                                                             \
    void Send( NET_AS_MOSServer* pMOS = 0 )                                         \
    {                                                                               \
        ASN1T_MsgsSimMos    globalAsnMsg;                                           \
                                                                                    \
        globalAsnMsg.t             = T_MsgsSimMos_msg_##ASNVAR;                     \
        MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr().SendMsgSimMos( globalAsnMsg, pMOS ); \
    }                                                                               \
}; 

//=============================================================================
// GENERATE ASN MESSAGES
//=============================================================================

// Control
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
NET_GENERATE_SEND_ASN_MSG_NOMSG_NOCTX( CtrlCheckPointSaveNowAck     , ctrl_checkpoint_save_now_ack      )
NET_GENERATE_SEND_ASN_MSG_NOMSG_NOCTX( CtrlSendCurrentStateBegin    , ctrl_send_current_state_begin     )
NET_GENERATE_SEND_ASN_MSG_NOMSG_NOCTX( CtrlSendCurrentStateEnd      , ctrl_send_current_state_end       )
                                                                                            
// Unit attributes
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( UnitAttributes     , unit_attributes       )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( UnitDotations      , unit_dotations        )
NET_GENERATE_SEND_ASN_MSG_PTR_CTX  ( UnitMagicActionAck , unit_magic_action_ack )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( UnitPathFind       , unit_pathfind         )

// Logistic
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( LogMaintenanceTraitementEquipementCreation   , log_maintenance_traitement_equipement_creation    )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( LogMaintenanceTraitementEquipementUpdate     , log_maintenance_traitement_equipement_update      )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( LogMaintenanceTraitementEquipementDestruction, log_maintenance_traitement_equipement_destruction )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( LogMaintenanceEtat                           , log_maintenance_etat                              )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( LogSanteTraitementHumainCreation             , log_sante_traitement_humain_creation              )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( LogSanteTraitementHumainUpdate               , log_sante_traitement_humain_update                )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( LogSanteTraitementHumainDestruction          , log_sante_traitement_humain_destruction           )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( LogSanteEtat                                 , log_sante_etat                                    )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( LogRavitaillementTraitementCreation          , log_ravitaillement_traitement_creation            )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( LogRavitaillementTraitementUpdate            , log_ravitaillement_traitement_update              )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( LogRavitaillementTraitementDestruction       , log_ravitaillement_traitement_destruction         )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( LogRavitaillementEtat                        , log_ravitaillement_etat                           )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( LogRavitaillementQuotas                      , log_ravitaillement_quotas                         )

// Context infos
NET_GENERATE_SEND_ASN_MSG_PTR_CTX    ( LimitCreationAck                  , limit_creation_ack                    )
NET_GENERATE_SEND_ASN_MSG_PTR_CTX    ( LimitDestructionAck               , limit_destruction_ack                 ) 
NET_GENERATE_SEND_ASN_MSG_PTR_CTX    ( LimitUpdateAck                    , limit_update_ack                      ) 
NET_GENERATE_SEND_ASN_MSG_PTR_CTX    ( LimaCreationAck                   , lima_creation_ack                     ) 
NET_GENERATE_SEND_ASN_MSG_PTR_CTX    ( LimaDestructionAck                , lima_destruction_ack                  )
NET_GENERATE_SEND_ASN_MSG_PTR_CTX    ( LimaUpdateAck                     , lima_update_ack                       )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX  ( LimitCreation                     , limit_creation                        )
NET_GENERATE_SEND_ASN_MSG_NOPTR_NOCTX( LimitDestruction                  , limit_destruction                     )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX  ( LimaCreation                      , lima_creation                         )
NET_GENERATE_SEND_ASN_MSG_NOPTR_NOCTX( LimaDestruction                   , lima_destruction                      )

// Fire
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX  ( StartPionFire      , start_pion_fire       )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX  ( StartPopulationFire, start_population_fire )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX  ( StopPionFire       , stop_pion_fire        )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX  ( StopPopulationFire , stop_population_fire  )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX  ( Explosion          , explosion             )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX  ( StartFireEffect    , start_fire_effect     )
NET_GENERATE_SEND_ASN_MSG_NOPTR_NOCTX( StopFireEffect     , stop_fire_effect      )

// Unit knowledge
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX ( UnitKnowledgeCreation   , unit_knowledge_creation    )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX ( UnitKnowledgeUpdate     , unit_knowledge_update      )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX ( UnitKnowledgeDestruction, unit_knowledge_destruction )

// Orders
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( OrderManagement, order_management )

// Orders conduite
NET_GENERATE_SEND_ASN_MSG_PTR_CTX  ( OrderConduiteAck           , order_conduite_ack            )

// Orders pion
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( PionOrder              , pion_order                         )
NET_GENERATE_SEND_ASN_MSG_PTR_CTX  ( PionOrderAck           , pion_order_ack                     )

// Orders automate
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( AutomateOrder     , automate_order        )
NET_GENERATE_SEND_ASN_MSG_PTR_CTX  ( AutomateOrderAck  , automate_order_ack    )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( AutomateMRT       , automate_mrt          )
NET_GENERATE_SEND_ASN_MSG_PTR_CTX  ( SetAutomateModeAck, set_automate_mode_ack )

// Orders population
NET_GENERATE_SEND_ASN_MSG_PTR_CTX  ( PopulationOrderAck       , population_order_ack )
NET_GENERATE_SEND_ASN_MSG_PTR_CTX  ( PopulationMagicActionAck , population_magic_action_ack )

// CR
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( CR, cr )

// Objects
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX  ( ObjectCreation       , object_creation         )
NET_GENERATE_SEND_ASN_MSG_NOPTR_NOCTX( ObjectDestruction    , object_destruction      )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX  ( ObjectUpdate         , object_update           )
NET_GENERATE_SEND_ASN_MSG_PTR_CTX    ( ObjectMagicActionAck , object_magic_action_ack )

// Object knowledge
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( ObjectKnowledgeCreation   , object_knowledge_creation    )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( ObjectKnowledgeUpdate     , object_knowledge_update      )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( ObjectKnowledgeDestruction, object_knowledge_destruction )

// Hierarchie / init
NET_GENERATE_SEND_ASN_MSG_PTR_CTX  ( ChangeDiplomatieAck        , change_diplomatie_ack          )
NET_GENERATE_SEND_ASN_MSG_PTR_CTX  ( ChangeGroupeConnaissanceAck, change_groupe_connaissance_ack )
NET_GENERATE_SEND_ASN_MSG_PTR_CTX  ( ChangeLiensLogistiquesAck  , change_liens_logistiques_ack   )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( ChangeAutomate             , change_automate                )
NET_GENERATE_SEND_ASN_MSG_PTR_CTX  ( ChangeAutomateAck          , change_automate_ack            )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( AutomateCreation           , automate_creation              )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( PionCreation               , pion_creation                  )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( ChangeDiplomatie           , change_diplomatie              )

// Logistique - Rav
NET_GENERATE_SEND_ASN_MSG_NOPTR_CTX  ( LogRavitaillementChangeQuotasAck, log_ravitaillement_change_quotas_ack )
NET_GENERATE_SEND_ASN_MSG_NOPTR_CTX  ( LogRavitaillementPousserFluxAck , log_ravitaillement_pousser_flux_ack  )

// Population
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( PopulationCreation                , population_creation                  )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( PopulationUpdate                  , population_update                    )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( PopulationConcentrationCreation   , population_concentration_creation    )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( PopulationConcentrationDestruction, population_concentration_destruction )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( PopulationConcentrationUpdate     , population_concentration_update      )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( PopulationFluxCreation            , population_flux_creation             )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( PopulationFluxDestruction         , population_flux_destruction          )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( PopulationFluxUpdate              , population_flux_update               )

// Population knowledge
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( PopulationKnowledgeCreation                , population_knowledge_creation                  )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( PopulationKnowledgeUpdate                  , population_knowledge_update                    )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( PopulationKnowledgeDestruction             , population_knowledge_destruction               )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( PopulationConcentrationKnowledgeCreation   , population_concentration_knowledge_creation    )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( PopulationConcentrationKnowledgeDestruction, population_concentration_knowledge_destruction )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( PopulationConcentrationKnowledgeUpdate     , population_concentration_knowledge_update      )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( PopulationFluxKnowledgeCreation            , population_flux_knowledge_creation             )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( PopulationFluxKnowledgeDestruction         , population_flux_knowledge_destruction          )
NET_GENERATE_SEND_ASN_MSG_PTR_NOCTX( PopulationFluxKnowledgeUpdate              , population_flux_knowledge_update               )

#endif // __NET_ASN_Messages_h_




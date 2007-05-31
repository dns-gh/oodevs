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

#define GENERATE_EMPTY_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )  \
class NET_ASN_Msg##ASNMSG                                     \
{                                                                \
public:                                                          \
    NET_ASN_Msg##ASNMSG() {}                                  \
                                                                 \
    void Send( int context = 0 )       \
    {                                                            \
        ASN1T_Msgs##TARGET asnMsg;                               \
        asnMsg.context = context;                                \
        asnMsg.msg.t   = T_Msgs##TARGET##_msg_msg_##ASNVAR;      \
        MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr().Send( asnMsg ); \
    }                                                            \
};

#define GENERATE_NOPTR_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )           \
class NET_ASN_Msg##ASNMSG                                              \
{                                                                         \
public:                                                                   \
    NET_ASN_Msg##ASNMSG() {}                                           \
                                                                          \
    void Send( int context = 0 )                \
    {                                                                     \
        ASN1T_Msgs##TARGET asnMsg;                                        \
        asnMsg.context            = context;                              \
        asnMsg.msg.t              = T_Msgs##TARGET##_msg_msg_##ASNVAR;    \
        asnMsg.msg.u.msg_##ASNVAR = asnTmp;                               \
        MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr().Send( asnMsg ); \
    }                                                                     \
                                                                          \
    ASN1T_Msg##ASNMSG& operator()() { return asnTmp; }                    \
private:                                                                  \
    ASN1T_Msg##ASNMSG asnTmp;                                             \
                                                                          \
};

#define GENERATE_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )                 \
class NET_ASN_Msg##ASNMSG                                              \
{                                                                         \
public:                                                                   \
    NET_ASN_Msg##ASNMSG() {}                                           \
                                                                          \
    void Send( int context = 0 )                \
    {                                                                     \
        ASN1T_Msgs##TARGET asnMsg;                                        \
        asnMsg.context            = context;                              \
        asnMsg.msg.t              = T_Msgs##TARGET##_msg_msg_##ASNVAR;    \
        asnMsg.msg.u.msg_##ASNVAR = &asnTmp;                              \
        MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr().Send( asnMsg ); \
    }                                                                     \
                                                                          \
    ASN1T_Msg##ASNMSG& operator()() { return asnTmp; }                    \
private:                                                                  \
    ASN1T_Msg##ASNMSG asnTmp;                                             \
                                                                          \
};

//=============================================================================
// GENERATE ASN MESSAGES
//=============================================================================

// Control
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, CtrlBeginTick                , ctrl_begin_tick                   )
GENERATE_ASN_MSG_SENDER      ( SimToClient, CtrlEndTick                  , ctrl_end_tick                     )
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, CtrlStopAck                  , ctrl_stop_ack                     )
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, CtrlResumeAck                , ctrl_resume_ack                   )
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, CtrlPauseAck                 , ctrl_pause_ack                    )
GENERATE_ASN_MSG_SENDER      ( SimToClient, CtrlChangeTimeFactorAck      , ctrl_change_time_factor_ack       )
GENERATE_ASN_MSG_SENDER      ( SimToClient, CtrlInfo                     , ctrl_info                         )
GENERATE_EMPTY_ASN_MSG_SENDER( SimToClient, CtrlMeteoGlobaleAck          , ctrl_meteo_globale_ack            )
GENERATE_EMPTY_ASN_MSG_SENDER( SimToClient, CtrlMeteoLocaleAck           , ctrl_meteo_locale_ack             )
GENERATE_EMPTY_ASN_MSG_SENDER( SimToClient, CtrlCheckPointSaveBegin      , ctrl_checkpoint_save_begin        )
GENERATE_EMPTY_ASN_MSG_SENDER( SimToClient, CtrlCheckPointSaveEnd        , ctrl_checkpoint_save_end          )
GENERATE_EMPTY_ASN_MSG_SENDER( SimToClient, CtrlCheckPointSetFrequencyAck, ctrl_checkpoint_set_frequency_ack )
GENERATE_EMPTY_ASN_MSG_SENDER( SimToClient, CtrlCheckPointSaveNowAck     , ctrl_checkpoint_save_now_ack      )
GENERATE_EMPTY_ASN_MSG_SENDER( SimToClient, CtrlSendCurrentStateBegin    , ctrl_send_current_state_begin     )
GENERATE_EMPTY_ASN_MSG_SENDER( SimToClient, CtrlSendCurrentStateEnd      , ctrl_send_current_state_end       )
                                                                                            
// Unit
GENERATE_ASN_MSG_SENDER( SimToClient, PionCreation       , pion_creation         )
GENERATE_ASN_MSG_SENDER( SimToClient, UnitAttributes     , unit_attributes       )
GENERATE_ASN_MSG_SENDER( SimToClient, UnitMagicActionAck , unit_magic_action_ack )
GENERATE_ASN_MSG_SENDER( SimToClient, UnitPathFind       , unit_pathfind         )

// Automate
GENERATE_ASN_MSG_SENDER( SimToClient, AutomateCreation   , automate_creation   )
GENERATE_ASN_MSG_SENDER( SimToClient, AutomateAttributes , automate_attributes )

// Army / formation / knowledge groups
GENERATE_ASN_MSG_SENDER( SimToClient, FormationCreation     , formation_creation       )
GENERATE_ASN_MSG_SENDER( SimToClient, KnowledgeGroupCreation, knowledge_group_creation )
GENERATE_ASN_MSG_SENDER( SimToClient, SideCreation          , side_creation            )

// Hierarchie update
GENERATE_ASN_MSG_SENDER( SimToClient, ChangeDiplomatieAck                , change_diplomatie_ack                   )
GENERATE_ASN_MSG_SENDER( SimToClient, AutomateChangeGroupeConnaissanceAck, automate_change_groupe_connaissance_ack )
GENERATE_ASN_MSG_SENDER( SimToClient, AutomateChangeLiensLogistiquesAck  , automate_change_liens_logistiques_ack   )
GENERATE_ASN_MSG_SENDER( SimToClient, AutomateChangeLiensLogistiques     , automate_change_liens_logistiques       )
GENERATE_ASN_MSG_SENDER( SimToClient, PionChangeSuperior                 , pion_change_superior                    )
GENERATE_ASN_MSG_SENDER( SimToClient, PionChangeSuperiorAck              , pion_change_superior_ack                )
GENERATE_ASN_MSG_SENDER( SimToClient, ChangeDiplomatie                   , change_diplomatie                       )

// Logistic
GENERATE_ASN_MSG_SENDER( SimToClient, LogMaintenanceTraitementEquipementCreation   , log_maintenance_traitement_equipement_creation    )
GENERATE_ASN_MSG_SENDER( SimToClient, LogMaintenanceTraitementEquipementUpdate     , log_maintenance_traitement_equipement_update      )
GENERATE_ASN_MSG_SENDER( SimToClient, LogMaintenanceTraitementEquipementDestruction, log_maintenance_traitement_equipement_destruction )
GENERATE_ASN_MSG_SENDER( SimToClient, LogMaintenanceEtat                           , log_maintenance_etat                              )
GENERATE_ASN_MSG_SENDER( SimToClient, LogSanteTraitementHumainCreation             , log_sante_traitement_humain_creation              )
GENERATE_ASN_MSG_SENDER( SimToClient, LogSanteTraitementHumainUpdate               , log_sante_traitement_humain_update                )
GENERATE_ASN_MSG_SENDER( SimToClient, LogSanteTraitementHumainDestruction          , log_sante_traitement_humain_destruction           )
GENERATE_ASN_MSG_SENDER( SimToClient, LogSanteEtat                                 , log_sante_etat                                    )
GENERATE_ASN_MSG_SENDER( SimToClient, LogRavitaillementTraitementCreation          , log_ravitaillement_traitement_creation            )
GENERATE_ASN_MSG_SENDER( SimToClient, LogRavitaillementTraitementUpdate            , log_ravitaillement_traitement_update              )
GENERATE_ASN_MSG_SENDER( SimToClient, LogRavitaillementTraitementDestruction       , log_ravitaillement_traitement_destruction         )
GENERATE_ASN_MSG_SENDER( SimToClient, LogRavitaillementEtat                        , log_ravitaillement_etat                           )
GENERATE_ASN_MSG_SENDER( SimToClient, LogRavitaillementQuotas                      , log_ravitaillement_quotas                         )

// Context infos
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, LimitCreationRequestAck   , limit_creation_request_ack    )
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, LimitDestructionRequestAck, limit_destruction_request_ack ) 
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, LimitUpdateRequestAck     , limit_update_request_ack      ) 
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, LimaCreationRequestAck    , lima_creation_request_ack     ) 
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, LimaDestructionRequestAck , lima_destruction_request_ack  )
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, LimaUpdateRequestAck      , lima_update_request_ack       )
GENERATE_ASN_MSG_SENDER  ( SimToClient, LimitCreation             , limit_creation                )
GENERATE_ASN_MSG_SENDER  ( SimToClient, LimitUpdate               , limit_update                  )
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, LimitDestruction          , limit_destruction             )
GENERATE_ASN_MSG_SENDER  ( SimToClient, LimaCreation              , lima_creation                 )
GENERATE_ASN_MSG_SENDER  ( SimToClient, LimaUpdate                , lima_update                   )
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, LimaDestruction           , lima_destruction              )

// Fire
GENERATE_ASN_MSG_SENDER  ( SimToClient, StartPionFire      , start_pion_fire       )
GENERATE_ASN_MSG_SENDER  ( SimToClient, StartPopulationFire, start_population_fire )
GENERATE_ASN_MSG_SENDER  ( SimToClient, StopPionFire       , stop_pion_fire        )
GENERATE_ASN_MSG_SENDER  ( SimToClient, StopPopulationFire , stop_population_fire  )
GENERATE_ASN_MSG_SENDER  ( SimToClient, Explosion          , explosion             )
GENERATE_ASN_MSG_SENDER  ( SimToClient, StartFireEffect    , start_fire_effect     )
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, StopFireEffect     , stop_fire_effect      )

// Unit knowledge
GENERATE_ASN_MSG_SENDER ( SimToClient, UnitKnowledgeCreation   , unit_knowledge_creation    )
GENERATE_ASN_MSG_SENDER ( SimToClient, UnitKnowledgeUpdate     , unit_knowledge_update      )
GENERATE_ASN_MSG_SENDER ( SimToClient, UnitKnowledgeDestruction, unit_knowledge_destruction )

// Orders
GENERATE_ASN_MSG_SENDER( SimToClient, PionOrder                , pion_order                  )
GENERATE_ASN_MSG_SENDER( SimToClient, PionOrderAck             , pion_order_ack              )
GENERATE_ASN_MSG_SENDER( SimToClient, AutomateOrder            , automate_order              )
GENERATE_ASN_MSG_SENDER( SimToClient, AutomateOrderAck         , automate_order_ack          )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationOrder          , population_order            )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationOrderAck       , population_order_ack        )
GENERATE_ASN_MSG_SENDER( SimToClient, FragOrderAck             , frag_order_ack              )

// Misc
GENERATE_ASN_MSG_SENDER( SimToClient, SetAutomateModeAck       , set_automate_mode_ack       )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationMagicActionAck , population_magic_action_ack )

// CR
GENERATE_ASN_MSG_SENDER( SimToClient, CR, cr )
GENERATE_ASN_MSG_SENDER( SimToClient, Trace, trace )
GENERATE_ASN_MSG_SENDER( SimToClient, DecisionalState, decisional_state )

// Objects
GENERATE_ASN_MSG_SENDER  ( SimToClient, ObjectCreation       , object_creation         )
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, ObjectDestruction    , object_destruction      )
GENERATE_ASN_MSG_SENDER  ( SimToClient, ObjectUpdate         , object_update           )
GENERATE_ASN_MSG_SENDER  ( SimToClient, ObjectMagicActionAck , object_magic_action_ack )

// Object knowledge
GENERATE_ASN_MSG_SENDER( SimToClient, ObjectKnowledgeCreation   , object_knowledge_creation    )
GENERATE_ASN_MSG_SENDER( SimToClient, ObjectKnowledgeUpdate     , object_knowledge_update      )
GENERATE_ASN_MSG_SENDER( SimToClient, ObjectKnowledgeDestruction, object_knowledge_destruction )

// Logistique - Rav
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, LogRavitaillementChangeQuotasAck, log_ravitaillement_change_quotas_ack )
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, LogRavitaillementPousserFluxAck , log_ravitaillement_pousser_flux_ack  )

// Population
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationCreation                , population_creation                  )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationUpdate                  , population_update                    )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationConcentrationCreation   , population_concentration_creation    )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationConcentrationDestruction, population_concentration_destruction )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationConcentrationUpdate     , population_concentration_update      )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationFluxCreation            , population_flux_creation             )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationFluxDestruction         , population_flux_destruction          )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationFluxUpdate              , population_flux_update               )

// Population knowledge
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationKnowledgeCreation                , population_knowledge_creation                  )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationKnowledgeUpdate                  , population_knowledge_update                    )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationKnowledgeDestruction             , population_knowledge_destruction               )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationConcentrationKnowledgeCreation   , population_concentration_knowledge_creation    )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationConcentrationKnowledgeDestruction, population_concentration_knowledge_destruction )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationConcentrationKnowledgeUpdate     , population_concentration_knowledge_update      )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationFluxKnowledgeCreation            , population_flux_knowledge_creation             )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationFluxKnowledgeDestruction         , population_flux_knowledge_destruction          )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationFluxKnowledgeUpdate              , population_flux_knowledge_update               )

#endif // __NET_ASN_Messages_h_




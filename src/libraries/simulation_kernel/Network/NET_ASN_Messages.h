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
#include "Network/NET_Publisher_ABC.h"
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
GENERATE_ASN_MSG_SENDER      ( SimToClient, ControlBeginTick                , control_begin_tick                   )
GENERATE_ASN_MSG_SENDER      ( SimToClient, ControlEndTick                  , control_end_tick                     )
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, ControlStopAck                  , control_stop_ack                     )
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, ControlResumeAck                , control_resume_ack                   )
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, ControlPauseAck                 , control_pause_ack                    )
GENERATE_ASN_MSG_SENDER      ( SimToClient, ControlChangeTimeFactorAck      , control_change_time_factor_ack       )
GENERATE_ASN_MSG_SENDER      ( SimToClient, ControlInformation              , control_information                  )
GENERATE_EMPTY_ASN_MSG_SENDER( SimToClient, ControlGlobalMeteoAck           , control_global_meteo_ack            )
GENERATE_EMPTY_ASN_MSG_SENDER( SimToClient, ControlLocalMeteoAck            , control_local_meteo_ack             )
GENERATE_EMPTY_ASN_MSG_SENDER( SimToClient, ControlCheckPointSaveBegin      , control_checkpoint_save_begin        )
GENERATE_EMPTY_ASN_MSG_SENDER( SimToClient, ControlCheckPointSaveEnd        , control_checkpoint_save_end          )
GENERATE_EMPTY_ASN_MSG_SENDER( SimToClient, ControlCheckPointSetFrequencyAck, control_checkpoint_set_frequency_ack )
GENERATE_EMPTY_ASN_MSG_SENDER( SimToClient, ControlCheckPointSaveNowAck     , control_checkpoint_save_now_ack      )
GENERATE_EMPTY_ASN_MSG_SENDER( SimToClient, ControlSendCurrentStateBegin    , control_send_current_state_begin     )
GENERATE_EMPTY_ASN_MSG_SENDER( SimToClient, ControlSendCurrentStateEnd      , control_send_current_state_end       )
                                                                                            
// Unit
GENERATE_ASN_MSG_SENDER      ( SimToClient, UnitCreation                    , unit_creation         )
GENERATE_ASN_MSG_SENDER      ( SimToClient, UnitAttributes                  , unit_attributes       )
GENERATE_ASN_MSG_SENDER      ( SimToClient, UnitMagicActionAck              , unit_magic_action_ack )
GENERATE_ASN_MSG_SENDER      ( SimToClient, UnitPathFind                    , unit_pathfind         )
GENERATE_ASN_MSG_SENDER      ( SimToClient, UnitEnvironmentType             , unit_environment_type )
GENERATE_ASN_MSG_SENDER      ( SimToClient, UnitVisionCones                 , unit_vision_cones     )
GENERATE_ASN_MSG_SENDER      ( SimToClient, UnitDetection                   , unit_detection        )
GENERATE_ASN_MSG_SENDER      ( SimToClient, ObjectDetection                 , object_detection      )
GENERATE_ASN_MSG_SENDER      ( SimToClient, PopulationConcentrationDetection, population_concentration_detection      )
GENERATE_ASN_MSG_SENDER      ( SimToClient, PopulationFlowDetection,          population_flow_detection      )

GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, UnitCreationRequestAck    , unit_creation_request_ack )

// Automate
GENERATE_ASN_MSG_SENDER( SimToClient, AutomatCreation   , automat_creation   )
GENERATE_ASN_MSG_SENDER( SimToClient, AutomatAttributes , automat_attributes )

// Army / formation / knowledge groups
GENERATE_ASN_MSG_SENDER( SimToClient, FormationCreation     , formation_creation       )
GENERATE_ASN_MSG_SENDER( SimToClient, KnowledgeGroupCreation, knowledge_group_creation )
GENERATE_ASN_MSG_SENDER( SimToClient, TeamCreation          , side_creation            )

// Hierarchie update
GENERATE_ASN_MSG_SENDER( SimToClient, ChangeDiplomacyAck            , change_diplomacy_ack                )
GENERATE_ASN_MSG_SENDER( SimToClient, AutomatChangeKnowledgeGroupAck, automat_change_knowledge_group_ack  )
GENERATE_ASN_MSG_SENDER( SimToClient, AutomatChangeLogisticLinksAck , automat_change_logistic_links_ack   )
GENERATE_ASN_MSG_SENDER( SimToClient, AutomatChangeLogisticLinks    , automat_change_logistic_links       )
GENERATE_ASN_MSG_SENDER( SimToClient, AutomatChangeSuperiorAck      , automat_change_superior_ack         )
GENERATE_ASN_MSG_SENDER( SimToClient, UnitChangeSuperior            , unit_change_superior                )
GENERATE_ASN_MSG_SENDER( SimToClient, UnitChangeSuperiorAck         , unit_change_superior_ack            )
GENERATE_ASN_MSG_SENDER( SimToClient, ChangeDiplomacy               , change_diplomacy                    )

// Logistic
GENERATE_ASN_MSG_SENDER( SimToClient, LogMaintenanceHandlingCreation   , log_maintenance_handling_creation    )
GENERATE_ASN_MSG_SENDER( SimToClient, LogMaintenanceHandlingUpdate     , log_maintenance_handling_update      )
GENERATE_ASN_MSG_SENDER( SimToClient, LogMaintenanceHandlingDestruction, log_maintenance_handling_destruction )
GENERATE_ASN_MSG_SENDER( SimToClient, LogMaintenanceState              , log_maintenance_state                )
GENERATE_ASN_MSG_SENDER( SimToClient, LogMedicalHandlingCreation       , log_medical_handling_creation        )
GENERATE_ASN_MSG_SENDER( SimToClient, LogMedicalHandlingUpdate         , log_medical_handling_update          )
GENERATE_ASN_MSG_SENDER( SimToClient, LogMedicalHandlingDestruction    , log_medical_handling_destruction     )
GENERATE_ASN_MSG_SENDER( SimToClient, LogMedicalState                  , log_medical_state                    )
GENERATE_ASN_MSG_SENDER( SimToClient, LogSupplyHandlingCreation        , log_supply_handling_creation         )
GENERATE_ASN_MSG_SENDER( SimToClient, LogSupplyHandlingUpdate          , log_supply_handling_update           )
GENERATE_ASN_MSG_SENDER( SimToClient, LogSupplyHandlingDestruction     , log_supply_handling_destruction      )
GENERATE_ASN_MSG_SENDER( SimToClient, LogSupplyState                   , log_supply_state                     )
GENERATE_ASN_MSG_SENDER( SimToClient, LogSupplyQuotas                  , log_supply_quotas                    )

// Context infos
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, LimitCreationRequestAck   , limit_creation_request_ack    )
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, LimitDestructionRequestAck, limit_destruction_request_ack ) 
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, LimitUpdateRequestAck     , limit_update_request_ack      ) 
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, LimaCreationRequestAck    , lima_creation_request_ack     ) 
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, LimaDestructionRequestAck , lima_destruction_request_ack  )
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, LimaUpdateRequestAck      , lima_update_request_ack       )
GENERATE_ASN_MSG_SENDER      ( SimToClient, LimitCreation             , limit_creation                )
GENERATE_ASN_MSG_SENDER      ( SimToClient, LimitUpdate               , limit_update                  )
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, LimitDestruction          , limit_destruction             )
GENERATE_ASN_MSG_SENDER      ( SimToClient, LimaCreation              , lima_creation                 )
GENERATE_ASN_MSG_SENDER      ( SimToClient, LimaUpdate                , lima_update                   )
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, LimaDestruction           , lima_destruction              )

// Fire
GENERATE_ASN_MSG_SENDER  ( SimToClient, StartUnitFire      , start_unit_fire       )
GENERATE_ASN_MSG_SENDER  ( SimToClient, StartPopulationFire, start_population_fire )
GENERATE_ASN_MSG_SENDER  ( SimToClient, StopUnitFire       , stop_unit_fire        )
GENERATE_ASN_MSG_SENDER  ( SimToClient, StopPopulationFire , stop_population_fire  )
GENERATE_ASN_MSG_SENDER  ( SimToClient, Explosion          , explosion             )
GENERATE_ASN_MSG_SENDER  ( SimToClient, StartFireEffect    , start_fire_effect     )
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, StopFireEffect , stop_fire_effect      )

// Unit knowledge
GENERATE_ASN_MSG_SENDER ( SimToClient, UnitKnowledgeCreation   , unit_knowledge_creation    )
GENERATE_ASN_MSG_SENDER ( SimToClient, UnitKnowledgeUpdate     , unit_knowledge_update      )
GENERATE_ASN_MSG_SENDER ( SimToClient, UnitKnowledgeDestruction, unit_knowledge_destruction )

// Orders
GENERATE_ASN_MSG_SENDER( SimToClient, UnitOrder               , unit_order                  )
GENERATE_ASN_MSG_SENDER( SimToClient, UnitOrderAck            , unit_order_ack              )
GENERATE_ASN_MSG_SENDER( SimToClient, AutomatOrder            , automat_order               )
GENERATE_ASN_MSG_SENDER( SimToClient, AutomatOrderAck         , automat_order_ack           )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationOrder         , population_order            )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationOrderAck      , population_order_ack        )
GENERATE_ASN_MSG_SENDER( SimToClient, FragOrderAck            , frag_order_ack              )

// Misc
GENERATE_ASN_MSG_SENDER( SimToClient, SetAutomatModeAck        , set_automat_mode_ack        )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationMagicActionAck , population_magic_action_ack )

// CR
GENERATE_ASN_MSG_SENDER( SimToClient, Report,           report )
GENERATE_ASN_MSG_SENDER( SimToClient, Trace,            trace )
GENERATE_ASN_MSG_SENDER( SimToClient, DecisionalState,  decisional_state )
GENERATE_ASN_MSG_SENDER( SimToClient, DebugPoints,      debug_points )
GENERATE_ASN_MSG_SENDER( SimToClient, InvalidateReport, invalidate_report )

// Objects
GENERATE_ASN_MSG_SENDER      ( SimToClient, ObjectCreation       , object_creation         )
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, ObjectDestruction    , object_destruction      )
GENERATE_ASN_MSG_SENDER      ( SimToClient, ObjectUpdate         , object_update           )
GENERATE_ASN_MSG_SENDER      ( SimToClient, ObjectMagicActionAck , object_magic_action_ack )

// Object knowledge
GENERATE_ASN_MSG_SENDER( SimToClient, ObjectKnowledgeCreation   , object_knowledge_creation    )
GENERATE_ASN_MSG_SENDER( SimToClient, ObjectKnowledgeUpdate     , object_knowledge_update      )
GENERATE_ASN_MSG_SENDER( SimToClient, ObjectKnowledgeDestruction, object_knowledge_destruction )

// Logistique - Rav
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, LogSupplyChangeQuotasAck, log_supply_change_quotas_ack )
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, LogSupplyPushFlowAck    , log_supply_push_flow_ack  )

// Population
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationCreation                , population_creation                  )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationUpdate                  , population_update                    )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationConcentrationCreation   , population_concentration_creation    )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationConcentrationDestruction, population_concentration_destruction )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationConcentrationUpdate     , population_concentration_update      )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationFlowCreation            , population_flow_creation             )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationFlowDestruction         , population_flow_destruction          )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationFlowUpdate              , population_flow_update               )

// Population knowledge
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationKnowledgeCreation                , population_knowledge_creation                  )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationKnowledgeUpdate                  , population_knowledge_update                    )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationKnowledgeDestruction             , population_knowledge_destruction               )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationConcentrationKnowledgeCreation   , population_concentration_knowledge_creation    )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationConcentrationKnowledgeDestruction, population_concentration_knowledge_destruction )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationConcentrationKnowledgeUpdate     , population_concentration_knowledge_update      )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationFlowKnowledgeCreation            , population_flow_knowledge_creation             )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationFlowKnowledgeDestruction         , population_flow_knowledge_destruction          )
GENERATE_ASN_MSG_SENDER( SimToClient, PopulationFlowKnowledgeUpdate              , population_flow_knowledge_update               )

GENERATE_ASN_MSG_SENDER( SimToClient, FolkCreation   , folk_creation     )
GENERATE_ASN_MSG_SENDER( SimToClient, FolkGraphUpdate, folk_graph_update )

GENERATE_ASN_MSG_SENDER( SimToClient, IntelligenceCreation             , intelligence_creation )
GENERATE_ASN_MSG_SENDER( SimToClient, IntelligenceDestruction          , intelligence_destruction )
GENERATE_ASN_MSG_SENDER( SimToClient, IntelligenceCreationRequestAck   , intelligence_creation_request_ack )
GENERATE_ASN_MSG_SENDER( SimToClient, IntelligenceDestructionRequestAck, intelligence_destruction_request_ack )

#endif // __NET_ASN_Messages_h_




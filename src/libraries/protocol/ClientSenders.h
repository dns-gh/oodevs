// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __protocol_ClientSenders_h_
#define __protocol_ClientSenders_h_

#include "Simulation.h"
#include "Senders.h"

namespace client
{
    GENERATE_MESSAGE( sword::SimToClient, sword, UnitChangeSuperior                     , unit_change_superior                      )
    GENERATE_MESSAGE( sword::SimToClient, sword, AutomatChangeKnowledgeGroup            , automat_change_knowledge_group            )
    GENERATE_MESSAGE( sword::SimToClient, sword, ChangeLogisticLinks                    , automat_change_logistic_links             )
    GENERATE_MESSAGE( sword::SimToClient, sword, AutomatChangeSuperior                  , automat_change_superior                   )
    GENERATE_MESSAGE( sword::SimToClient, sword, FormationCreation                      , formation_creation                        )
    GENERATE_MESSAGE( sword::SimToClient, sword, FormationDestruction                   , formation_destruction                     )
    GENERATE_MESSAGE( sword::SimToClient, sword, ChangeDiplomacy                        , change_diplomacy                          )
    GENERATE_MESSAGE( sword::SimToClient, sword, UnitOrder                              , unit_order                                )
    GENERATE_MESSAGE( sword::SimToClient, sword, AutomatOrder                           , automat_order                             )
    GENERATE_MESSAGE( sword::SimToClient, sword, CrowdOrder                             , crowd_order                               )
    GENERATE_MESSAGE( sword::SimToClient, sword, FragOrder                              , frag_order                               )
    GENERATE_MESSAGE( sword::SimToClient, sword, ControlInformation                     , control_information                       )
    GENERATE_MESSAGE( sword::SimToClient, sword, ControlBeginTick                       , control_begin_tick                        )
    GENERATE_MESSAGE( sword::SimToClient, sword, ControlEndTick                         , control_end_tick                          )
    GENERATE_MESSAGE( sword::SimToClient, sword, ControlStopAck                         , control_stop_ack                          )
    GENERATE_MESSAGE( sword::SimToClient, sword, ControlPauseAck                        , control_pause_ack                         )
    GENERATE_MESSAGE( sword::SimToClient, sword, ControlResumeAck                       , control_resume_ack                        )
    GENERATE_MESSAGE( sword::SimToClient, sword, ControlChangeTimeFactorAck             , control_change_time_factor_ack            )
    GENERATE_MESSAGE( sword::SimToClient, sword, ControlDatetimeChangeAck               , control_date_time_change_ack              )
    GENERATE_MESSAGE( sword::SimToClient, sword, ControlSendCurrentStateBegin           , control_send_current_state_begin          )
    GENERATE_MESSAGE( sword::SimToClient, sword, ControlSendCurrentStateEnd             , control_send_current_state_end            )
    GENERATE_MESSAGE( sword::SimToClient, sword, UnitChangeSuperiorAck                  , unit_change_superior_ack                  )
    GENERATE_MESSAGE( sword::SimToClient, sword, AutomatChangeKnowledgeGroupAck         , automat_change_knowledge_group_ack        )
    GENERATE_MESSAGE( sword::SimToClient, sword, AutomatChangeSuperiorAck               , automat_change_superior_ack               )
    GENERATE_MESSAGE( sword::SimToClient, sword, PartyCreation                          , party_creation                            )
    GENERATE_MESSAGE( sword::SimToClient, sword, AutomatCreation                        , automat_creation                          )
    GENERATE_MESSAGE( sword::SimToClient, sword, AutomatDestruction                     , automat_destruction                       )
    GENERATE_MESSAGE( sword::SimToClient, sword, AutomatAttributes                      , automat_attributes                        )
    GENERATE_MESSAGE( sword::SimToClient, sword, UnitCreation                           , unit_creation                             )
    GENERATE_MESSAGE( sword::SimToClient, sword, UnitAttributes                         , unit_attributes                           )
    GENERATE_MESSAGE( sword::SimToClient, sword, UnitPathFind                           , unit_pathfind                             )
    GENERATE_MESSAGE( sword::SimToClient, sword, UnitDestruction                        , unit_destruction                          )
    GENERATE_MESSAGE( sword::SimToClient, sword, UnitKnowledgeCreation                  , unit_knowledge_creation                   )
    GENERATE_MESSAGE( sword::SimToClient, sword, UnitKnowledgeUpdate                    , unit_knowledge_update                     )
    GENERATE_MESSAGE( sword::SimToClient, sword, UnitKnowledgeDestruction               , unit_knowledge_destruction                )
    GENERATE_MESSAGE( sword::SimToClient, sword, StartUnitFire                          , start_unit_fire                           )
    GENERATE_MESSAGE( sword::SimToClient, sword, StopUnitFire                           , stop_unit_fire                            )
    GENERATE_MESSAGE( sword::SimToClient, sword, StartCrowdFire                         , start_crowd_fire                          )
    GENERATE_MESSAGE( sword::SimToClient, sword, StopCrowdFire                          , stop_crowd_fire                           )
    GENERATE_MESSAGE( sword::SimToClient, sword, Explosion                              , explosion                                 )
    GENERATE_MESSAGE( sword::SimToClient, sword, StartFireEffect                        , start_fire_effect                         )
    GENERATE_MESSAGE( sword::SimToClient, sword, StopFireEffect                         , stop_fire_effect                          )
    GENERATE_MESSAGE( sword::SimToClient, sword, Report                                 , report                                    )
    GENERATE_MESSAGE( sword::SimToClient, sword, InvalidateReport                       , invalidate_report                         )
    GENERATE_MESSAGE( sword::SimToClient, sword, DecisionalState                        , decisional_state                          )
    GENERATE_MESSAGE( sword::SimToClient, sword, ObjectCreation                         , object_creation                           )
    GENERATE_MESSAGE( sword::SimToClient, sword, ObjectDestruction                      , object_destruction                        )
    GENERATE_MESSAGE( sword::SimToClient, sword, ObjectUpdate                           , object_update                             )
    GENERATE_MESSAGE( sword::SimToClient, sword, ObjectKnowledgeCreation                , object_knowledge_creation                 )
    GENERATE_MESSAGE( sword::SimToClient, sword, ObjectKnowledgeUpdate                  , object_knowledge_update                   )
    GENERATE_MESSAGE( sword::SimToClient, sword, ObjectKnowledgeDestruction             , object_knowledge_destruction              )
    GENERATE_MESSAGE( sword::SimToClient, sword, LogMedicalHandlingCreation             , log_medical_handling_creation             )
    GENERATE_MESSAGE( sword::SimToClient, sword, LogMedicalHandlingUpdate               , log_medical_handling_update               )
    GENERATE_MESSAGE( sword::SimToClient, sword, LogMedicalHandlingDestruction          , log_medical_handling_destruction          )
    GENERATE_MESSAGE( sword::SimToClient, sword, LogMedicalState                        , log_medical_state                         )
    GENERATE_MESSAGE( sword::SimToClient, sword, LogMaintenanceHandlingCreation         , log_maintenance_handling_creation         )
    GENERATE_MESSAGE( sword::SimToClient, sword, LogMaintenanceHandlingUpdate           , log_maintenance_handling_update           )
    GENERATE_MESSAGE( sword::SimToClient, sword, LogMaintenanceHandlingDestruction      , log_maintenance_handling_destruction      )
    GENERATE_MESSAGE( sword::SimToClient, sword, LogMaintenanceState                    , log_maintenance_state                     )
    GENERATE_MESSAGE( sword::SimToClient, sword, LogSupplyHandlingCreation              , log_supply_handling_creation              )
    GENERATE_MESSAGE( sword::SimToClient, sword, LogSupplyHandlingUpdate                , log_supply_handling_update                )
    GENERATE_MESSAGE( sword::SimToClient, sword, LogSupplyHandlingDestruction           , log_supply_handling_destruction           )
    GENERATE_MESSAGE( sword::SimToClient, sword, LogSupplyState                         , log_supply_state                          )
    GENERATE_MESSAGE( sword::SimToClient, sword, LogSupplyQuotas                        , log_supply_quotas                         )
    GENERATE_MESSAGE( sword::SimToClient, sword, PopulationCreation                     , population_creation                       )
    GENERATE_MESSAGE( sword::SimToClient, sword, PopulationUpdate                       , population_update                         )
    GENERATE_MESSAGE( sword::SimToClient, sword, CrowdCreation                          , crowd_creation                            )
    GENERATE_MESSAGE( sword::SimToClient, sword, CrowdDestruction                       , crowd_destruction                         )
    GENERATE_MESSAGE( sword::SimToClient, sword, CrowdUpdate                            , crowd_update                              )
    GENERATE_MESSAGE( sword::SimToClient, sword, CrowdConcentrationCreation             , crowd_concentration_creation              )
    GENERATE_MESSAGE( sword::SimToClient, sword, CrowdConcentrationDestruction          , crowd_concentration_destruction           )
    GENERATE_MESSAGE( sword::SimToClient, sword, CrowdConcentrationUpdate               , crowd_concentration_update                )
    GENERATE_MESSAGE( sword::SimToClient, sword, CrowdFlowCreation                      , crowd_flow_creation                       )
    GENERATE_MESSAGE( sword::SimToClient, sword, CrowdFlowDestruction                   , crowd_flow_destruction                    )
    GENERATE_MESSAGE( sword::SimToClient, sword, CrowdFlowUpdate                        , crowd_flow_update                         )
    GENERATE_MESSAGE( sword::SimToClient, sword, CrowdKnowledgeCreation                 , crowd_knowledge_creation                  )
    GENERATE_MESSAGE( sword::SimToClient, sword, CrowdKnowledgeUpdate                   , crowd_knowledge_update                    )
    GENERATE_MESSAGE( sword::SimToClient, sword, CrowdKnowledgeDestruction              , crowd_knowledge_destruction               )
    GENERATE_MESSAGE( sword::SimToClient, sword, CrowdConcentrationKnowledgeCreation    , crowd_concentration_knowledge_creation    )
    GENERATE_MESSAGE( sword::SimToClient, sword, CrowdConcentrationKnowledgeDestruction , crowd_concentration_knowledge_destruction )
    GENERATE_MESSAGE( sword::SimToClient, sword, CrowdConcentrationKnowledgeUpdate      , crowd_concentration_knowledge_update      )
    GENERATE_MESSAGE( sword::SimToClient, sword, CrowdFlowKnowledgeCreation             , crowd_flow_knowledge_creation             )
    GENERATE_MESSAGE( sword::SimToClient, sword, CrowdFlowKnowledgeDestruction          , crowd_flow_knowledge_destruction          )
    GENERATE_MESSAGE( sword::SimToClient, sword, CrowdFlowKnowledgeUpdate               , crowd_flow_knowledge_update               )
    GENERATE_MESSAGE( sword::SimToClient, sword, FolkCreation                           , folk_creation                             )
    GENERATE_MESSAGE( sword::SimToClient, sword, ControlGlobalWeatherAck                , control_global_weather_ack                )
    GENERATE_MESSAGE( sword::SimToClient, sword, ControlLocalWeatherAck                 , control_local_weather_ack                 )
    GENERATE_MESSAGE( sword::SimToClient, sword, ControlCheckPointSaveBegin             , control_checkpoint_save_begin             )
    GENERATE_MESSAGE( sword::SimToClient, sword, ControlCheckPointSaveEnd               , control_checkpoint_save_end               )
    GENERATE_MESSAGE( sword::SimToClient, sword, ControlCheckPointSetFrequencyAck       , control_checkpoint_set_frequency_ack      )
    GENERATE_MESSAGE( sword::SimToClient, sword, ControlCheckPointSaveNowAck            , control_checkpoint_save_now_ack           )
    GENERATE_MESSAGE( sword::SimToClient, sword, UnitMagicActionAck                     , unit_magic_action_ack                     )
    GENERATE_MESSAGE( sword::SimToClient, sword, UnitEnvironmentType                    , unit_environment_type                     )
    GENERATE_MESSAGE( sword::SimToClient, sword, UnitVisionCones                        , unit_vision_cones                         )
    GENERATE_MESSAGE( sword::SimToClient, sword, UnitDetection                          , unit_detection                            )
    GENERATE_MESSAGE( sword::SimToClient, sword, ObjectDetection                        , object_detection                          )
    GENERATE_MESSAGE( sword::SimToClient, sword, CrowdConcentrationDetection            , crowd_concentration_detection             )
    GENERATE_MESSAGE( sword::SimToClient, sword, CrowdFlowDetection                     , crowd_flow_detection                      )
    GENERATE_MESSAGE( sword::SimToClient, sword, UnitCreationRequestAck                 , unit_creation_request_ack                 )
    GENERATE_MESSAGE( sword::SimToClient, sword, ChangeDiplomacyAck                     , change_diplomacy_ack                      )
    GENERATE_MESSAGE( sword::SimToClient, sword, ChangeLogisticLinksAck                 , automat_change_logistic_links_ack         )
    GENERATE_MESSAGE( sword::SimToClient, sword, TaskCreationRequestAck                 , order_ack                                 )
    GENERATE_MESSAGE( sword::SimToClient, sword, FragOrderAck                           , frag_order_ack                            )
    GENERATE_MESSAGE( sword::SimToClient, sword, SetAutomatModeAck                      , set_automat_mode_ack                      )
    GENERATE_MESSAGE( sword::SimToClient, sword, CrowdMagicActionAck                    , crowd_magic_action_ack                    )
    GENERATE_MESSAGE( sword::SimToClient, sword, Trace                                  , trace                                     )
    GENERATE_MESSAGE( sword::SimToClient, sword, DebugPoints                            , debug_points                              )
    GENERATE_MESSAGE( sword::SimToClient, sword, MagicActionAck                         , magic_action_ack                          )
    GENERATE_MESSAGE( sword::SimToClient, sword, ObjectMagicActionAck                   , object_magic_action_ack                   )
    GENERATE_MESSAGE( sword::SimToClient, sword, LogSupplyChangeQuotasAck               , log_supply_change_quotas_ack              )
    GENERATE_MESSAGE( sword::SimToClient, sword, LogSupplyPushFlowAck                   , log_supply_push_flow_ack                  )
    GENERATE_MESSAGE( sword::SimToClient, sword, LogSupplyPullFlowAck                   , log_supply_pull_flow_ack                  )
    GENERATE_MESSAGE( sword::SimToClient, sword, FolkGraphUpdate                        , folk_graph_update                         )
    GENERATE_MESSAGE( sword::SimToClient, sword, UrbanCreation                          , urban_creation                            )
    GENERATE_MESSAGE( sword::SimToClient, sword, UrbanUpdate                            , urban_update                              )
    GENERATE_MESSAGE( sword::SimToClient, sword, UrbanDetection                         , urban_detection                           )
    GENERATE_MESSAGE( sword::SimToClient, sword, UrbanKnowledgeCreation                 , urban_knowledge_creation                  )
    GENERATE_MESSAGE( sword::SimToClient, sword, UrbanKnowledgeUpdate                   , urban_knowledge_update                    )
    GENERATE_MESSAGE( sword::SimToClient, sword, UrbanKnowledgeDestruction              , urban_knowledge_destruction               )
    GENERATE_MESSAGE( sword::SimToClient, sword, KnowledgeGroupMagicActionAck           , knowledge_group_magic_action_ack          )
    GENERATE_MESSAGE( sword::SimToClient, sword, KnowledgeGroupCreation                 , knowledge_group_creation                  )
    GENERATE_MESSAGE( sword::SimToClient, sword, KnowledgeGroupCreationAck              , knowledge_group_creation_ack              )
    GENERATE_MESSAGE( sword::SimToClient, sword, KnowledgeGroupUpdate                   , knowledge_group_update                    )
    GENERATE_MESSAGE( sword::SimToClient, sword, KnowledgeGroupUpdateAck                , knowledge_group_update_ack                )
    GENERATE_MESSAGE( sword::SimToClient, sword, KnowledgeGroupDestruction              , knowledge_group_destruction               )
    GENERATE_MESSAGE( sword::SimToClient, sword, ActionCreateFireOrderAck               , action_create_fire_order_ack              )
    GENERATE_MESSAGE( sword::SimToClient, sword, ControlGlobalWeather                   , control_global_weather                    )
    GENERATE_MESSAGE( sword::SimToClient, sword, ControlLocalWeatherCreation            , control_local_weather_creation            )
    GENERATE_MESSAGE( sword::SimToClient, sword, ControlLocalWeatherDestruction         , control_local_weather_destruction         )
}

#endif // __protocol_ClientSenders_h_

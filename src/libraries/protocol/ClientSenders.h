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

GENERATE_ASN_MSG_SENDER( SimToClient, Common         , UnitChangeSuperior                            , unit_change_superior                              )
GENERATE_ASN_MSG_SENDER( SimToClient, Common         , AutomatChangeKnowledgeGroup                   , automat_change_knowledge_group                    )
GENERATE_ASN_MSG_SENDER( SimToClient, Common         , AutomatChangeLogisticLinks                    , automat_change_logistic_links                     )
GENERATE_ASN_MSG_SENDER( SimToClient, Common         , AutomatChangeSuperior                         , automat_change_superior                           )
GENERATE_ASN_MSG_SENDER( SimToClient, Common         , FormationCreation                             , formation_creation                                )
GENERATE_ASN_MSG_SENDER( SimToClient, Common         , ChangeDiplomacy                               , change_diplomacy                                  )
GENERATE_ASN_MSG_SENDER( SimToClient, Common         , UnitOrder                                     , unit_order                                        )
GENERATE_ASN_MSG_SENDER( SimToClient, Common         , AutomatOrder                                  , automat_order                                     )
GENERATE_ASN_MSG_SENDER( SimToClient, Common         , PopulationOrder                               , population_order                                  )

GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, ControlInformation                            , control_information                               )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, ControlBeginTick                              , control_begin_tick                                )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, ControlEndTick                                , control_end_tick                                  )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, ControlStopAck                                , control_stop_ack                                  )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, ControlPauseAck                               , control_pause_ack                                 )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, ControlResumeAck                              , control_resume_ack                                )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, ControlChangeTimeFactorAck                    , control_change_time_factor_ack                    )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, ControlDatetimeChangeAck                      , control_date_time_change_ack                      )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, ControlSendCurrentStateBegin                  , control_send_current_state_begin                  )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, ControlSendCurrentStateEnd                    , control_send_current_state_end                    )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, UnitChangeSuperiorAck                         , unit_change_superior_ack                          )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, AutomatChangeKnowledgeGroupAck                , automat_change_knowledge_group_ack                )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, AutomatChangeSuperiorAck                      , automat_change_superior_ack                       )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, TeamCreation                                  , side_creation                                     )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, AutomatCreation                               , automat_creation                                  )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, AutomatAttributes                             , automat_attributes                                )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, UnitCreation                                  , unit_creation                                     )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, UnitAttributes                                , unit_attributes                                   )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, UnitPathFind                                  , unit_pathfind                                     )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, UnitDestruction                               , unit_destruction                                  )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, UnitKnowledgeCreation                         , unit_knowledge_creation                           )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, UnitKnowledgeUpdate                           , unit_knowledge_update                             )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, UnitKnowledgeDestruction                      , unit_knowledge_destruction                        )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, StartUnitFire                                 , start_unit_fire                                   )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, StopUnitFire                                  , stop_unit_fire                                    )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, StartPopulationFire                           , start_population_fire                             )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, StopPopulationFire                            , stop_population_fire                              )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, Explosion                                     , explosion                                         )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, StartFireEffect                               , start_fire_effect                                 )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, StopFireEffect                                , stop_fire_effect                                  )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, Report                                        , report                                            )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, InvalidateReport                              , invalidate_report                                 )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, DecisionalState                               , decisional_state                                  )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, ObjectCreation                                , object_creation                                   )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, ObjectDestruction                             , object_destruction                                )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, ObjectUpdate                                  , object_update                                     )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, ObjectKnowledgeCreation                       , object_knowledge_creation                         )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, ObjectKnowledgeUpdate                         , object_knowledge_update                           )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, ObjectKnowledgeDestruction                    , object_knowledge_destruction                      )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, LogMedicalHandlingCreation                    , log_medical_handling_creation                     )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, LogMedicalHandlingUpdate                      , log_medical_handling_update                       )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, LogMedicalHandlingDestruction                 , log_medical_handling_destruction                  )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, LogMedicalState                               , log_medical_state                                 )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, LogMaintenanceHandlingCreation                , log_maintenance_handling_creation                 )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, LogMaintenanceHandlingUpdate                  , log_maintenance_handling_update                   )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, LogMaintenanceHandlingDestruction             , log_maintenance_handling_destruction              )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, LogMaintenanceState                           , log_maintenance_state                             )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, LogSupplyHandlingCreation                     , log_supply_handling_creation                      )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, LogSupplyHandlingUpdate                       , log_supply_handling_update                        )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, LogSupplyHandlingDestruction                  , log_supply_handling_destruction                   )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, LogSupplyState                                , log_supply_state                                  )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, LogSupplyQuotas                               , log_supply_quotas                                 )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, PopulationCreation                            , population_creation                               )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, PopulationUpdate                              , population_update                                 )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, PopulationConcentrationCreation               , population_concentration_creation                 )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, PopulationConcentrationDestruction            , population_concentration_destruction              )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, PopulationConcentrationUpdate                 , population_concentration_update                   )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, PopulationFlowCreation                        , population_flow_creation                          )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, PopulationFlowDestruction                     , population_flow_destruction                       )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, PopulationFlowUpdate                          , population_flow_update                            )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, PopulationKnowledgeCreation                   , population_knowledge_creation                     )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, PopulationKnowledgeUpdate                     , population_knowledge_update                       )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, PopulationKnowledgeDestruction                , population_knowledge_destruction                  )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, PopulationConcentrationKnowledgeCreation      , population_concentration_knowledge_creation       )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, PopulationConcentrationKnowledgeDestruction   , population_concentration_knowledge_destruction    )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, PopulationConcentrationKnowledgeUpdate        , population_concentration_knowledge_update         )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, PopulationFlowKnowledgeCreation               , population_flow_knowledge_creation                )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, PopulationFlowKnowledgeDestruction            , population_flow_knowledge_destruction             )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, PopulationFlowKnowledgeUpdate                 , population_flow_knowledge_update                  )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, FolkCreation                                  , folk_creation                                     )

GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, ControlGlobalMeteoAck                         , control_global_meteo_ack                          )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, ControlLocalMeteoAck                          , control_local_meteo_ack                           )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, ControlCheckPointSaveBegin                    , control_checkpoint_save_begin                     )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, ControlCheckPointSaveEnd                      , control_checkpoint_save_end                       )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, ControlCheckPointSetFrequencyAck              , control_checkpoint_set_frequency_ack              )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, ControlCheckPointSaveNowAck                   , control_checkpoint_save_now_ack                   )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, UnitMagicActionAck                            , unit_magic_action_ack                             )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, UnitEnvironmentType                           , unit_environment_type                             )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, UnitVisionCones                               , unit_vision_cones                                 )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, UnitDetection                                 , unit_detection                                    )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, ObjectDetection                               , object_detection                                  )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, PopulationConcentrationDetection              , population_concentration_detection                )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, PopulationFlowDetection                       , population_flow_detection                         )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, UnitCreationRequestAck                        , unit_creation_request_ack                         )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient , ChangeDiplomacyAck                           , change_diplomacy_ack                              )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient , AutomatChangeLogisticLinksAck                , automat_change_logistic_links_ack                 )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient , UnitOrderAck                                 , unit_order_ack                                    )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient , AutomatOrderAck                              , automat_order_ack                                 )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient , PopulationOrderAck                           , population_order_ack                              )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient , FragOrderAck                                 , frag_order_ack                                    )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient , SetAutomatModeAck                            , set_automat_mode_ack                              )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient , PopulationMagicActionAck                     , population_magic_action_ack                       )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient , Trace                                        , trace                                             )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient , DebugPoints                                  , debug_points                                      )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient , ObjectMagicActionAck                         , object_magic_action_ack                           )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient , LogSupplyChangeQuotasAck                     , log_supply_change_quotas_ack                      )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient , LogSupplyPushFlowAck                         , log_supply_push_flow_ack                          )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient , FolkGraphUpdate                              , folk_graph_update                                 )

GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient , UrbanCreation                                , urban_creation                                    )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient , UrbanDetection                               , urban_detection                                   )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient , UrbanKnowledgeCreation                       , urban_knowledge_creation                          )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient , UrbanKnowledgeUpdate                         , urban_knowledge_update                            )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient , UrbanKnowledgeDestruction                    , urban_knowledge_destruction                       )
// LTO begin
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient , KnowledgeGroupMagicActionAck                 , knowledge_group_magic_action_ack                               )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient , KnowledgeGroupCreation                       , knowledge_group_creation                          )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient , KnowledgeGroupCreationAck                    , knowledge_group_creation_ack                      )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient , KnowledgeGroupUpdate                         , knowledge_group_update                            )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient , KnowledgeGroupUpdateAck                      , knowledge_group_update_ack                        )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient , KnowledgeGroupDestruction                    , knowledge_group_destruction                       )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient , ActionCreateFireOrderAck                     , action_create_fire_order_ack                      )
// LTO end

GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, ControlGlobalMeteo                            , control_global_meteo                              )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, ControlLocalMeteoCreation                     , control_local_meteo_creation                      )
GENERATE_ASN_MSG_SENDER( SimToClient, MsgsSimToClient, ControlLocalMeteoDestruction                  , control_local_meteo_destruction                   )
}

#endif // __protocol_ClientSenders_h_

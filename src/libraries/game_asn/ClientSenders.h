// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ClientSenders_h_
#define __ClientSenders_h_

#include "MessageSenders.h"
#include "Simulation.h"
#include "tools/AsnControllers.h"

DECLARE_ASN_CONTROLLER( MsgsSimToClient )

namespace client // $$$$ AGE 2008-06-19: ...
{

GENERATE_ASN_MSG_SENDER      ( SimToClient, ControlInformation                            , control_information                               );
GENERATE_ASN_MSG_SENDER      ( SimToClient, ControlBeginTick                              , control_begin_tick                                );
GENERATE_ASN_MSG_SENDER      ( SimToClient, ControlEndTick                                , control_end_tick                                  );
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, ControlStopAck                                , control_stop_ack                                  );
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, ControlPauseAck                               , control_pause_ack                                 );
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, ControlResumeAck                              , control_resume_ack                                );
GENERATE_ASN_MSG_SENDER      ( SimToClient, ControlChangeTimeFactorAck                    , control_change_time_factor_ack                    );
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, ControlDatetimeChangeAck                      , control_date_time_change_ack                      );
GENERATE_EMPTY_ASN_MSG_SENDER( SimToClient, ControlSendCurrentStateBegin                  , control_send_current_state_begin                  );
GENERATE_EMPTY_ASN_MSG_SENDER( SimToClient, ControlSendCurrentStateEnd                    , control_send_current_state_end                    );
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, UnitChangeSuperiorAck                         , unit_change_superior_ack                          );
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, AutomatChangeKnowledgeGroupAck                , automat_change_knowledge_group_ack                );
GENERATE_ASN_MSG_SENDER      ( SimToClient, AutomatChangeKnowledgeGroup                   , automat_change_knowledge_group                    );
GENERATE_ASN_MSG_SENDER      ( SimToClient, AutomatChangeLogisticLinks                    , automat_change_logistic_links                     );
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, AutomatChangeSuperiorAck                      , automat_change_superior_ack                       );
GENERATE_ASN_MSG_SENDER      ( SimToClient, AutomatChangeSuperior                         , automat_change_superior                           );
GENERATE_ASN_MSG_SENDER      ( SimToClient, TeamCreation                                  , side_creation                                     );
GENERATE_ASN_MSG_SENDER      ( SimToClient, KnowledgeGroupCreation                        , knowledge_group_creation                          );
GENERATE_ASN_MSG_SENDER      ( SimToClient, FormationCreation                             , formation_creation                                );
GENERATE_ASN_MSG_SENDER      ( SimToClient, AutomatCreation                               , automat_creation                                  );
GENERATE_ASN_MSG_SENDER      ( SimToClient, AutomatAttributes                             , automat_attributes                                );
GENERATE_ASN_MSG_SENDER      ( SimToClient, UnitCreation                                  , unit_creation                                     );
GENERATE_ASN_MSG_SENDER      ( SimToClient, UnitAttributes                                , unit_attributes                                   );
GENERATE_ASN_MSG_SENDER      ( SimToClient, UnitPathFind                                  , unit_pathfind                                     );
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, UnitDestruction                               , unit_destruction                                  );
GENERATE_ASN_MSG_SENDER      ( SimToClient, ChangeDiplomacy                               , change_diplomacy                                  );
GENERATE_ASN_MSG_SENDER      ( SimToClient, UnitKnowledgeCreation                         , unit_knowledge_creation                           );
GENERATE_ASN_MSG_SENDER      ( SimToClient, UnitKnowledgeUpdate                           , unit_knowledge_update                             );
GENERATE_ASN_MSG_SENDER      ( SimToClient, UnitKnowledgeDestruction                      , unit_knowledge_destruction                        );
GENERATE_ASN_MSG_SENDER      ( SimToClient, StartUnitFire                                 , start_unit_fire                                   );
GENERATE_ASN_MSG_SENDER      ( SimToClient, StopUnitFire                                  , stop_unit_fire                                    );
GENERATE_ASN_MSG_SENDER      ( SimToClient, StartPopulationFire                           , start_population_fire                             );
GENERATE_ASN_MSG_SENDER      ( SimToClient, StopPopulationFire                            , stop_population_fire                              );
GENERATE_ASN_MSG_SENDER      ( SimToClient, Explosion                                     , explosion                                         );
GENERATE_ASN_MSG_SENDER      ( SimToClient, StartFireEffect                               , start_fire_effect                                 );
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, StopFireEffect                                , stop_fire_effect                                  );
GENERATE_ASN_MSG_SENDER      ( SimToClient, Report                                        , report                                            );
GENERATE_ASN_MSG_SENDER      ( SimToClient, InvalidateReport                              , invalidate_report                                 );
GENERATE_ASN_MSG_SENDER      ( SimToClient, DecisionalState                               , decisional_state                                  );
GENERATE_ASN_MSG_SENDER      ( SimToClient, UnitOrder                                     , unit_order                                        );
GENERATE_ASN_MSG_SENDER      ( SimToClient, AutomatOrder                                  , automat_order                                     );
GENERATE_ASN_MSG_SENDER      ( SimToClient, PopulationOrder                               , population_order                                  );
GENERATE_ASN_MSG_SENDER      ( SimToClient, ObjectCreation                                , object_creation                                   );
GENERATE_NOPTR_ASN_MSG_SENDER( SimToClient, ObjectDestruction                             , object_destruction                                );
GENERATE_ASN_MSG_SENDER      ( SimToClient, ObjectUpdate                                  , object_update                                     );
GENERATE_ASN_MSG_SENDER      ( SimToClient, ObjectKnowledgeCreation                       , object_knowledge_creation                         );
GENERATE_ASN_MSG_SENDER      ( SimToClient, ObjectKnowledgeUpdate                         , object_knowledge_update                           );
GENERATE_ASN_MSG_SENDER      ( SimToClient, ObjectKnowledgeDestruction                    , object_knowledge_destruction                      );
GENERATE_ASN_MSG_SENDER      ( SimToClient, UnitChangeSuperior                            , unit_change_superior                              );
GENERATE_ASN_MSG_SENDER      ( SimToClient, LogMedicalHandlingCreation                    , log_medical_handling_creation                     );
GENERATE_ASN_MSG_SENDER      ( SimToClient, LogMedicalHandlingUpdate                      , log_medical_handling_update                       );
GENERATE_ASN_MSG_SENDER      ( SimToClient, LogMedicalHandlingDestruction                 , log_medical_handling_destruction                  );
GENERATE_ASN_MSG_SENDER      ( SimToClient, LogMedicalState                               , log_medical_state                                 );
GENERATE_ASN_MSG_SENDER      ( SimToClient, LogMaintenanceHandlingCreation                , log_maintenance_handling_creation                 );
GENERATE_ASN_MSG_SENDER      ( SimToClient, LogMaintenanceHandlingUpdate                  , log_maintenance_handling_update                   );
GENERATE_ASN_MSG_SENDER      ( SimToClient, LogMaintenanceHandlingDestruction             , log_maintenance_handling_destruction              );
GENERATE_ASN_MSG_SENDER      ( SimToClient, LogMaintenanceState                           , log_maintenance_state                             );
GENERATE_ASN_MSG_SENDER      ( SimToClient, LogSupplyHandlingCreation                     , log_supply_handling_creation                      );
GENERATE_ASN_MSG_SENDER      ( SimToClient, LogSupplyHandlingUpdate                       , log_supply_handling_update                        );
GENERATE_ASN_MSG_SENDER      ( SimToClient, LogSupplyHandlingDestruction                  , log_supply_handling_destruction                   );
GENERATE_ASN_MSG_SENDER      ( SimToClient, LogSupplyState                                , log_supply_state                                  );
GENERATE_ASN_MSG_SENDER      ( SimToClient, LogSupplyQuotas                               , log_supply_quotas                                 );
GENERATE_ASN_MSG_SENDER      ( SimToClient, PopulationCreation                            , population_creation                               );
GENERATE_ASN_MSG_SENDER      ( SimToClient, PopulationUpdate                              , population_update                                 );
GENERATE_ASN_MSG_SENDER      ( SimToClient, PopulationConcentrationCreation               , population_concentration_creation                 );
GENERATE_ASN_MSG_SENDER      ( SimToClient, PopulationConcentrationDestruction            , population_concentration_destruction              );
GENERATE_ASN_MSG_SENDER      ( SimToClient, PopulationConcentrationUpdate                 , population_concentration_update                   );
GENERATE_ASN_MSG_SENDER      ( SimToClient, PopulationFlowCreation                        , population_flow_creation                          );
GENERATE_ASN_MSG_SENDER      ( SimToClient, PopulationFlowDestruction                     , population_flow_destruction                       );
GENERATE_ASN_MSG_SENDER      ( SimToClient, PopulationFlowUpdate                          , population_flow_update                            );
GENERATE_ASN_MSG_SENDER      ( SimToClient, PopulationKnowledgeCreation                   , population_knowledge_creation                     );
GENERATE_ASN_MSG_SENDER      ( SimToClient, PopulationKnowledgeUpdate                     , population_knowledge_update                       );
GENERATE_ASN_MSG_SENDER      ( SimToClient, PopulationKnowledgeDestruction                , population_knowledge_destruction                  );
GENERATE_ASN_MSG_SENDER      ( SimToClient, PopulationConcentrationKnowledgeCreation      , population_concentration_knowledge_creation       );
GENERATE_ASN_MSG_SENDER      ( SimToClient, PopulationConcentrationKnowledgeDestruction   , population_concentration_knowledge_destruction    );
GENERATE_ASN_MSG_SENDER      ( SimToClient, PopulationConcentrationKnowledgeUpdate        , population_concentration_knowledge_update         );
GENERATE_ASN_MSG_SENDER      ( SimToClient, PopulationFlowKnowledgeCreation               , population_flow_knowledge_creation                );
GENERATE_ASN_MSG_SENDER      ( SimToClient, PopulationFlowKnowledgeDestruction            , population_flow_knowledge_destruction             );
GENERATE_ASN_MSG_SENDER      ( SimToClient, PopulationFlowKnowledgeUpdate                 , population_flow_knowledge_update                  );
GENERATE_ASN_MSG_SENDER      ( SimToClient, FolkCreation                                  , folk_creation                                     );
GENERATE_ASN_MSG_SENDER      ( SimToClient, UrbanCreation                                 , urban_creation                                    );

GENERATE_ASN_MSG_SENDER      ( SimToClient, UrbanKnowledgeCreation                        , urban_knowledge_creation                          );
GENERATE_ASN_MSG_SENDER      ( SimToClient, UrbanKnowledgeUpdate                          , urban_knowledge_update                            );
GENERATE_ASN_MSG_SENDER      ( SimToClient, UrbanKnowledgeDestruction                     , urban_knowledge_destruction                       );
}

#endif // __ClientSenders_h_

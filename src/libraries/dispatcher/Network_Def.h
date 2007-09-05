// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NetworkDef_h__
#define __NetworkDef_h__

#include "ClientPublisher_ABC.h"
#include "SimulationPublisher_ABC.h"

namespace dispatcher
{
 
#define GENERATE_EMPTY_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )  \
class AsnMsg##TARGET##ASNMSG                                     \
{                                                                \
public:                                                          \
    AsnMsg##TARGET##ASNMSG() {}                                  \
                                                                 \
    template< typename Publisher >                               \
    void Send( Publisher& publisher, int context = 0 )           \
    {                                                            \
        ASN1T_Msgs##TARGET asnMsg;                               \
        asnMsg.context = context;                                \
        asnMsg.msg.t   = T_Msgs##TARGET##_msg_msg_##ASNVAR;      \
        publisher.Send( asnMsg );                                \
    }                                                            \
};

#define GENERATE_NOPTR_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )           \
class AsnMsg##TARGET##ASNMSG                                              \
{                                                                         \
public:                                                                   \
    AsnMsg##TARGET##ASNMSG() {}                                           \
                                                                          \
    template< typename Publisher >                                        \
    void Send( Publisher& publisher, int context = 0 )                    \
    {                                                                     \
        ASN1T_Msgs##TARGET asnMsg;                                        \
        asnMsg.context            = context;                              \
        asnMsg.msg.t              = T_Msgs##TARGET##_msg_msg_##ASNVAR;    \
        asnMsg.msg.u.msg_##ASNVAR = asnTmp;                               \
        publisher.Send( asnMsg );                                         \
    }                                                                     \
                                                                          \
    ASN1T_Msg##ASNMSG& operator()() { return asnTmp; }                    \
private:                                                                  \
    ASN1T_Msg##ASNMSG asnTmp;                                             \
                                                                          \
};

#define GENERATE_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )                 \
class AsnMsg##TARGET##ASNMSG                                              \
{                                                                         \
public:                                                                   \
    AsnMsg##TARGET##ASNMSG() {}                                           \
                                                                          \
    template< typename Publisher >                                        \
    void Send( Publisher& publisher, int context = 0 )                    \
    {                                                                     \
        ASN1T_Msgs##TARGET asnMsg;                                        \
        asnMsg.context            = context;                              \
        asnMsg.msg.t              = T_Msgs##TARGET##_msg_msg_##ASNVAR;    \
        asnMsg.msg.u.msg_##ASNVAR = &asnTmp;                              \
        publisher.Send( asnMsg );                                         \
    }                                                                     \
                                                                          \
    ASN1T_Msg##ASNMSG& operator()() { return asnTmp; }                    \
private:                                                                  \
    ASN1T_Msg##ASNMSG asnTmp;                                             \
                                                                          \
};

GENERATE_ASN_MSG_SENDER         ( SimToClient, ControlInformation                            , control_information                               );
GENERATE_NOPTR_ASN_MSG_SENDER   ( SimToClient, ControlBeginTick                              , control_begin_tick                                );
GENERATE_ASN_MSG_SENDER         ( SimToClient, ControlEndTick                                , control_end_tick                                  );
GENERATE_NOPTR_ASN_MSG_SENDER   ( SimToClient, ControlStopAck                                , control_stop_ack                                  );
GENERATE_NOPTR_ASN_MSG_SENDER   ( SimToClient, ControlPauseAck                               , control_pause_ack                                 );
GENERATE_NOPTR_ASN_MSG_SENDER   ( SimToClient, ControlResumeAck                              , control_resume_ack                                );
GENERATE_ASN_MSG_SENDER         ( SimToClient, ControlChangeTimeFactorAck                    , control_change_time_factor_ack                    );
GENERATE_EMPTY_ASN_MSG_SENDER   ( SimToClient, ControlSendCurrentStateBegin                  , control_send_current_state_begin                  );
GENERATE_EMPTY_ASN_MSG_SENDER   ( SimToClient, ControlSendCurrentStateEnd                    , control_send_current_state_end                    );
GENERATE_ASN_MSG_SENDER         ( SimToClient, UnitChangeSuperiorAck                         , unit_change_superior_ack                          );
GENERATE_ASN_MSG_SENDER         ( SimToClient, AutomatChangeKnowledgeGroupAck                , automat_change_knowledge_group_ack                );
GENERATE_ASN_MSG_SENDER         ( SimToClient, AutomatChangeLogisticLinks                    , automat_change_logistic_links                     );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LimitCreation                                 , limit_creation                                    );
GENERATE_NOPTR_ASN_MSG_SENDER   ( SimToClient, LimitDestruction                              , limit_destruction                                 );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LimaCreation                                  , lima_creation                                     );
GENERATE_NOPTR_ASN_MSG_SENDER   ( SimToClient, LimaDestruction                               , lima_destruction                                  );
GENERATE_ASN_MSG_SENDER         ( SimToClient, TeamCreation                                  , side_creation                                     );
GENERATE_ASN_MSG_SENDER         ( SimToClient, KnowledgeGroupCreation                        , knowledge_group_creation                          );
GENERATE_ASN_MSG_SENDER         ( SimToClient, FormationCreation                             , formation_creation                                );
GENERATE_ASN_MSG_SENDER         ( SimToClient, AutomatCreation                               , automat_creation                                  );
GENERATE_ASN_MSG_SENDER         ( SimToClient, AutomatAttributes                             , automat_attributes                                );
GENERATE_ASN_MSG_SENDER         ( SimToClient, UnitCreation                                  , unit_creation                                     );
GENERATE_ASN_MSG_SENDER         ( SimToClient, UnitAttributes                                , unit_attributes                                   );
GENERATE_ASN_MSG_SENDER         ( SimToClient, UnitPathFind                                  , unit_pathfind                                     );
GENERATE_NOPTR_ASN_MSG_SENDER   ( SimToClient, UnitDestruction                               , unit_destruction                                  );
GENERATE_ASN_MSG_SENDER         ( SimToClient, ChangeDiplomacy                               , change_diplomacy                                  );
GENERATE_ASN_MSG_SENDER         ( SimToClient, UnitKnowledgeCreation                         , unit_knowledge_creation                           );
GENERATE_ASN_MSG_SENDER         ( SimToClient, UnitKnowledgeUpdate                           , unit_knowledge_update                             );
GENERATE_ASN_MSG_SENDER         ( SimToClient, UnitKnowledgeDestruction                      , unit_knowledge_destruction                        );
GENERATE_ASN_MSG_SENDER         ( SimToClient, StartUnitFire                                 , start_unit_fire                                   );
GENERATE_ASN_MSG_SENDER         ( SimToClient, StopUnitFire                                  , stop_unit_fire                                    );
GENERATE_ASN_MSG_SENDER         ( SimToClient, StartPopulationFire                           , start_population_fire                             );
GENERATE_ASN_MSG_SENDER         ( SimToClient, StopPopulationFire                            , stop_population_fire                              );
GENERATE_ASN_MSG_SENDER         ( SimToClient, Explosion                                     , explosion                                         );
GENERATE_ASN_MSG_SENDER         ( SimToClient, StartFireEffect                               , start_fire_effect                                 );
GENERATE_NOPTR_ASN_MSG_SENDER   ( SimToClient, StopFireEffect                                , stop_fire_effect                                  );
GENERATE_ASN_MSG_SENDER         ( SimToClient, Report                                        , report                                            );
GENERATE_ASN_MSG_SENDER         ( SimToClient, DecisionalState                               , decisional_state                                  );
GENERATE_ASN_MSG_SENDER         ( SimToClient, UnitOrder                                     , unit_order                                        );
GENERATE_ASN_MSG_SENDER         ( SimToClient, AutomatOrder                                  , automat_order                                     );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationOrder                               , population_order                                  );
GENERATE_ASN_MSG_SENDER         ( SimToClient, ObjectCreation                                , object_creation                                   );
GENERATE_NOPTR_ASN_MSG_SENDER   ( SimToClient, ObjectDestruction                             , object_destruction                                );
GENERATE_ASN_MSG_SENDER         ( SimToClient, ObjectUpdate                                  , object_update                                     );
GENERATE_ASN_MSG_SENDER         ( SimToClient, ObjectKnowledgeCreation                       , object_knowledge_creation                         );
GENERATE_ASN_MSG_SENDER         ( SimToClient, ObjectKnowledgeUpdate                         , object_knowledge_update                           );
GENERATE_ASN_MSG_SENDER         ( SimToClient, ObjectKnowledgeDestruction                    , object_knowledge_destruction                      );
GENERATE_ASN_MSG_SENDER         ( SimToClient, UnitChangeSuperior                            , unit_change_superior                              );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LogMedicalHandlingCreation                    , log_medical_handling_creation                     );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LogMedicalHandlingUpdate                      , log_medical_handling_update                       );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LogMedicalHandlingDestruction                 , log_medical_handling_destruction                  );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LogMedicalState                               , log_medical_state                                 );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LogMaintenanceHandlingCreation                , log_maintenance_handling_creation                 );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LogMaintenanceHandlingUpdate                  , log_maintenance_handling_update                   );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LogMaintenanceHandlingDestruction             , log_maintenance_handling_destruction              );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LogMaintenanceState                           , log_maintenance_state                             );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LogSupplyHandlingCreation                     , log_supply_handling_creation                      );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LogSupplyHandlingUpdate                       , log_supply_handling_update                        );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LogSupplyHandlingDestruction                  , log_supply_handling_destruction                   );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LogSupplyState                                , log_supply_state                                  );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LogSupplyQuotas                               , log_supply_quotas                                 );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationCreation                            , population_creation                               );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationUpdate                              , population_update                                 );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationConcentrationCreation               , population_concentration_creation                 );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationConcentrationDestruction            , population_concentration_destruction              );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationConcentrationUpdate                 , population_concentration_update                   );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationFlowCreation                        , population_flow_creation                          );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationFlowDestruction                     , population_flow_destruction                       );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationFlowUpdate                          , population_flow_update                            );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationKnowledgeCreation                   , population_knowledge_creation                     );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationKnowledgeUpdate                     , population_knowledge_update                       );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationKnowledgeDestruction                , population_knowledge_destruction                  );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationConcentrationKnowledgeCreation      , population_concentration_knowledge_creation       );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationConcentrationKnowledgeDestruction   , population_concentration_knowledge_destruction    );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationConcentrationKnowledgeUpdate        , population_concentration_knowledge_update         );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationFlowKnowledgeCreation               , population_flow_knowledge_creation                );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationFlowKnowledgeDestruction            , population_flow_knowledge_destruction             );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationFlowKnowledgeUpdate                 , population_flow_knowledge_update                  );
GENERATE_ASN_MSG_SENDER         ( SimToClient, FolkCreation                                  , folk_creation                                     );

GENERATE_ASN_MSG_SENDER         ( AuthenticationToClient, AuthenticationResponse       , authentication_response         );
GENERATE_ASN_MSG_SENDER         ( AuthenticationToClient, ProfileCreation              , profile_creation                );
GENERATE_ASN_MSG_SENDER         ( AuthenticationToClient, ProfileUpdate                , profile_update                  );
GENERATE_NOPTR_ASN_MSG_SENDER   ( AuthenticationToClient, ProfileDestruction           , profile_destruction             );
GENERATE_ASN_MSG_SENDER         ( AuthenticationToClient, ProfileCreationRequestAck    , profile_creation_request_ack    );
GENERATE_ASN_MSG_SENDER         ( AuthenticationToClient, ProfileUpdateRequestAck      , profile_update_request_ack      );
GENERATE_ASN_MSG_SENDER         ( AuthenticationToClient, ProfileDestructionRequestAck , profile_destruction_request_ack );

GENERATE_ASN_MSG_SENDER         ( ReplayToClient, ControlReplayInformation     , control_replay_information      );
GENERATE_NOPTR_ASN_MSG_SENDER   ( ReplayToClient, ControlStopAck               , control_stop_ack                                  );
GENERATE_NOPTR_ASN_MSG_SENDER   ( ReplayToClient, ControlPauseAck              , control_pause_ack                                 );
GENERATE_NOPTR_ASN_MSG_SENDER   ( ReplayToClient, ControlResumeAck             , control_resume_ack                                );
GENERATE_ASN_MSG_SENDER         ( ReplayToClient, ControlChangeTimeFactorAck   , control_change_time_factor_ack                    );
GENERATE_ASN_MSG_SENDER         ( ReplayToClient, ControlSkipToTickAck         , control_skip_to_tick_ack        );

GENERATE_EMPTY_ASN_MSG_SENDER   ( MiddleToSim, CtrlClientAnnouncement          , control_client_announcement     );

GENERATE_EMPTY_ASN_MSG_SENDER   ( ClientToSim, ControlPause, control_pause );
GENERATE_EMPTY_ASN_MSG_SENDER   ( ClientToSim, ControlResume, control_resume );
GENERATE_ASN_MSG_SENDER         ( ClientToSim, UnitOrder, unit_order );
GENERATE_ASN_MSG_SENDER         ( ClientToSim, FragOrder, frag_order );
GENERATE_ASN_MSG_SENDER         ( ClientToSim, AutomatOrder, automat_order );

template< typename L, typename E, typename P > 
void SendContainerValues( const P& container, L& asnList )
{
    asnList.n = container.size();
    if( !container.empty() )
    {
        asnList.elem = new E[ container.size() ];
        unsigned int i = 0;
        for( P::const_iterator it = container.begin(); it != container.end(); ++it, ++i )
            asnList.elem[ i ] = *it;
    }
}

template< typename L, typename E, typename P > 
void SendContainerRefs( const P& container, L& asnList )
{
    asnList.n = container.size();
    if( !container.empty() )
    {
        asnList.elem = new E[ container.size() ];
        unsigned int i = 0;
        for( P::const_iterator it = container.begin(); it != container.end(); ++it, ++i )
            asnList.elem[ i ] = (**it).GetID();
    }
}

} // end namespace dispatcher

#endif // __NetworkDef_h__

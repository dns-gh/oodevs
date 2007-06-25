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

#include "Publisher_ABC.h"

namespace dispatcher
{
//! @name DIN Messages
//@{
static enum  
{
    eMsgSimToClient                            = 0,
    eMsgClientToSim                            = 1,

    eMsgSimToMiddle                            = 2,
    eMsgMiddleToSim                            = 3,

    eMsgClientToMiddle                         = 4,
    eMsgMiddleToClient                         = 5,

    eMsgEnableUnitVisionCones                  = 1000,
    eMsgDisableUnitVisionCones                 = 1001,
    eMsgUnitMagicAction                        = 1002,
    eMsgEnableProfiling                        = 1003,
    eMsgDisableProfiling                       = 1004,
    eMsgUnitVisionCones                        = 1005,
    eMsgProfilingValues                        = 1008,
    eMsgUnitInterVisibility                    = 1009,
    eMsgObjectInterVisibility                  = 1010,
    eMsgPopulationConcentrationInterVisibility = 1011,
    eMsgPopulationFlowInterVisibility          = 1012,
    eMsgDebugDrawPoints                        = 1015,
    eMsgEnvironmentType                        = 1016,
};
//@}

 
#define GENERATE_EMPTY_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )  \
class AsnMsg##TARGET##ASNMSG                                     \
{                                                                \
public:                                                          \
    AsnMsg##TARGET##ASNMSG() {}                                  \
                                                                 \
    void Send( Publisher_ABC& publisher, int context = 0 )       \
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
    void Send( Publisher_ABC& publisher, int context = 0 )                \
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
    void Send( Publisher_ABC& publisher, int context = 0 )                \
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

GENERATE_ASN_MSG_SENDER         ( SimToClient, ControlInformation                            , ctrl_info                                         );
GENERATE_NOPTR_ASN_MSG_SENDER   ( SimToClient, ControlBeginTick                              , ctrl_begin_tick                                   );
GENERATE_ASN_MSG_SENDER         ( SimToClient, ControlEndTick                                , ctrl_end_tick                                     );
GENERATE_NOPTR_ASN_MSG_SENDER   ( SimToClient, ControlStopAck                                , ctrl_stop_ack                                     );
GENERATE_NOPTR_ASN_MSG_SENDER   ( SimToClient, ControlPauseAck                               , ctrl_pause_ack                                    );
GENERATE_NOPTR_ASN_MSG_SENDER   ( SimToClient, ControlResumeAck                              , ctrl_resume_ack                                   );
GENERATE_ASN_MSG_SENDER         ( SimToClient, ControlChangeTimeFactorAck                    , ctrl_change_time_factor_ack                       );
GENERATE_EMPTY_ASN_MSG_SENDER   ( SimToClient, ControlSendCurrentStateBegin                  , ctrl_send_current_state_begin                     );
GENERATE_EMPTY_ASN_MSG_SENDER   ( SimToClient, ControlSendCurrentStateEnd                    , ctrl_send_current_state_end                       );
GENERATE_ASN_MSG_SENDER         ( SimToClient, UnitChangeSuperiorAck                         , pion_change_superior_ack                          );
GENERATE_ASN_MSG_SENDER         ( SimToClient, AutomatChangeKnowledgeGroupAck                , automate_change_groupe_connaissance_ack           );
GENERATE_ASN_MSG_SENDER         ( SimToClient, AutomatChangeLogisticLinks                    , automate_change_liens_logistiques                 );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LimitCreation                                 , limit_creation                                    );
GENERATE_NOPTR_ASN_MSG_SENDER   ( SimToClient, LimitDestruction                              , limit_destruction                                 );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LimaCreation                                  , lima_creation                                     );
GENERATE_NOPTR_ASN_MSG_SENDER   ( SimToClient, LimaDestruction                               , lima_destruction                                  );
GENERATE_ASN_MSG_SENDER         ( SimToClient, TeamCreation                                  , side_creation                                     );
GENERATE_ASN_MSG_SENDER         ( SimToClient, KnowledgeGroupCreation                        , knowledge_group_creation                          );
GENERATE_ASN_MSG_SENDER         ( SimToClient, FormationCreation                             , formation_creation                                );
GENERATE_ASN_MSG_SENDER         ( SimToClient, AutomatCreation                               , automate_creation                                 );
GENERATE_ASN_MSG_SENDER         ( SimToClient, AutomatAttributes                             , automate_attributes                               );
GENERATE_ASN_MSG_SENDER         ( SimToClient, UnitCreation                                  , pion_creation                                     );
GENERATE_ASN_MSG_SENDER         ( SimToClient, UnitAttributes                                , unit_attributes                                   );
GENERATE_ASN_MSG_SENDER         ( SimToClient, UnitPathFind                                  , unit_pathfind                                     );
GENERATE_NOPTR_ASN_MSG_SENDER   ( SimToClient, UnitDestruction                               , pion_destruction                                  );
GENERATE_ASN_MSG_SENDER         ( SimToClient, ChangeDiplomacy                               , change_diplomatie                                 );
GENERATE_ASN_MSG_SENDER         ( SimToClient, UnitKnowledgeCreation                         , unit_knowledge_creation                           );
GENERATE_ASN_MSG_SENDER         ( SimToClient, UnitKnowledgeUpdate                           , unit_knowledge_update                             );
GENERATE_ASN_MSG_SENDER         ( SimToClient, UnitKnowledgeDestruction                      , unit_knowledge_destruction                        );
GENERATE_ASN_MSG_SENDER         ( SimToClient, StartUnitFire                                 , start_pion_fire                                   );
GENERATE_ASN_MSG_SENDER         ( SimToClient, StopUnitFire                                  , stop_pion_fire                                    );
GENERATE_ASN_MSG_SENDER         ( SimToClient, StartPopulationFire                           , start_population_fire                             );
GENERATE_ASN_MSG_SENDER         ( SimToClient, StopPopulationFire                            , stop_population_fire                              );
GENERATE_ASN_MSG_SENDER         ( SimToClient, Explosion                                     , explosion                                         );
GENERATE_ASN_MSG_SENDER         ( SimToClient, StartFireEffect                               , start_fire_effect                                 );
GENERATE_NOPTR_ASN_MSG_SENDER   ( SimToClient, StopFireEffect                                , stop_fire_effect                                  );
GENERATE_ASN_MSG_SENDER         ( SimToClient, Report                                        , cr                                                );
GENERATE_ASN_MSG_SENDER         ( SimToClient, DecisionalState                               , decisional_state                                  );
GENERATE_ASN_MSG_SENDER         ( SimToClient, UnitOrder                                     , pion_order                                        );
GENERATE_ASN_MSG_SENDER         ( SimToClient, AutomatOrder                                  , automate_order                                    );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationOrder                               , population_order                                  );
GENERATE_ASN_MSG_SENDER         ( SimToClient, ObjectCreation                                , object_creation                                   );
GENERATE_NOPTR_ASN_MSG_SENDER   ( SimToClient, ObjectDestruction                             , object_destruction                                );
GENERATE_ASN_MSG_SENDER         ( SimToClient, ObjectUpdate                                  , object_update                                     );
GENERATE_ASN_MSG_SENDER         ( SimToClient, ObjectKnowledgeCreation                       , object_knowledge_creation                         );
GENERATE_ASN_MSG_SENDER         ( SimToClient, ObjectKnowledgeUpdate                         , object_knowledge_update                           );
GENERATE_ASN_MSG_SENDER         ( SimToClient, ObjectKnowledgeDestruction                    , object_knowledge_destruction                      );
GENERATE_ASN_MSG_SENDER         ( SimToClient, UnitChangeSuperior                            , pion_change_superior                              );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LogMedicalHandlingCreation                    , log_sante_traitement_humain_creation              );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LogMedicalHandlingUpdate                      , log_sante_traitement_humain_update                );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LogMedicalHandlingDestruction                 , log_sante_traitement_humain_destruction           );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LogMedicalState                               , log_sante_etat                                    );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LogMaintenanceHandlingCreation                , log_maintenance_traitement_equipement_creation    );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LogMaintenanceHandlingUpdate                  , log_maintenance_traitement_equipement_update      );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LogMaintenanceHandlingDestruction             , log_maintenance_traitement_equipement_destruction );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LogMaintenanceState                           , log_maintenance_etat                              );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LogSupplyHandlingCreation                     , log_ravitaillement_traitement_creation            );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LogSupplyHandlingUpdate                       , log_ravitaillement_traitement_update              );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LogSupplyHandlingDestruction                  , log_ravitaillement_traitement_destruction         );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LogSupplyState                                , log_ravitaillement_etat                           );
GENERATE_ASN_MSG_SENDER         ( SimToClient, LogSupplyQuotas                               , log_ravitaillement_quotas                         );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationCreation                            , population_creation                               );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationUpdate                              , population_update                                 );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationConcentrationCreation               , population_concentration_creation                 );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationConcentrationDestruction            , population_concentration_destruction              );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationConcentrationUpdate                 , population_concentration_update                   );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationFlowCreation                        , population_flux_creation                          );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationFlowDestruction                     , population_flux_destruction                       );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationFlowUpdate                          , population_flux_update                            );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationKnowledgeCreation                   , population_knowledge_creation                     );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationKnowledgeUpdate                     , population_knowledge_update                       );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationKnowledgeDestruction                , population_knowledge_destruction                  );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationConcentrationKnowledgeCreation      , population_concentration_knowledge_creation       );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationConcentrationKnowledgeDestruction   , population_concentration_knowledge_destruction    );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationConcentrationKnowledgeUpdate        , population_concentration_knowledge_update         );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationFlowKnowledgeCreation               , population_flux_knowledge_creation                );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationFlowKnowledgeDestruction            , population_flux_knowledge_destruction             );
GENERATE_ASN_MSG_SENDER         ( SimToClient, PopulationFlowKnowledgeUpdate                 , population_flux_knowledge_update                  );

GENERATE_ASN_MSG_SENDER         ( MiddleToClient, AuthenticationResponse       , authentication_response         );
GENERATE_ASN_MSG_SENDER         ( MiddleToClient, ProfileCreation              , profile_creation                );
GENERATE_ASN_MSG_SENDER         ( MiddleToClient, ProfileUpdate                , profile_update                  );
GENERATE_NOPTR_ASN_MSG_SENDER   ( MiddleToClient, ProfileDestruction           , profile_destruction             );
GENERATE_ASN_MSG_SENDER         ( MiddleToClient, ProfileCreationRequestAck    , profile_creation_request_ack    );
GENERATE_ASN_MSG_SENDER         ( MiddleToClient, ProfileUpdateRequestAck      , profile_update_request_ack      );
GENERATE_ASN_MSG_SENDER         ( MiddleToClient, ProfileDestructionRequestAck , profile_destruction_request_ack );
GENERATE_ASN_MSG_SENDER         ( MiddleToClient, ControlReplayInformation     , ctrl_replay_info                );
GENERATE_ASN_MSG_SENDER         ( MiddleToClient, ControlSkipToTickAck         , ctrl_skip_to_tick_ack           );

GENERATE_EMPTY_ASN_MSG_SENDER   ( MiddleToSim, CtrlClientAnnouncement         , ctrl_client_announcement        );

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

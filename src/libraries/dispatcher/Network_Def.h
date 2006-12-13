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
    eMsgOutSim                                 = 0,
    eMsgInSim                                  = 1,

    eMsgOutClient                              = 2,
    eMsgInClient                               = 3,

    eMsgEnableUnitVisionCones                  = 1000,
    eMsgDisableUnitVisionCones                 = 1001,
    eMsgUnitMagicAction                        = 1002,
    eMsgEnableProfiling                        = 1003,
    eMsgDisableProfiling                       = 1004,
    eMsgUnitVisionCones                        = 1005,
    eMsgTrace                                  = 1006,
    eMsgInit                                   = 1007,
    eMsgProfilingValues                        = 1008,
    eMsgUnitInterVisibility                    = 1009,
    eMsgObjectInterVisibility                  = 1010,
    eMsgPopulationConcentrationInterVisibility = 1011,
    eMsgPopulationFlowInterVisibility          = 1012,
    eMsgDebugDrawPoints                        = 1015,
    eMsgEnvironmentType                        = 1016,
    eMsgPopulationCollision                    = 1017
};
//@}

 
#define GENERATE_EMPTY_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )  \
class AsnMsgIn##TARGET##ASNMSG                                   \
{                                                                \
public:                                                          \
    AsnMsgIn##TARGET##ASNMSG() {}                                \
                                                                 \
    void Send( Publisher_ABC& publisher )                        \
    {                                                            \
        ASN1T_MsgsIn##TARGET asnMsg;                             \
        asnMsg.msg.t = T_MsgsIn##TARGET##_msg_msg_##ASNVAR;      \
        publisher.Send( asnMsg );                                \
    }                                                            \
};

#define GENERATE_NOPTR_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )           \
class AsnMsgIn##TARGET##ASNMSG                                            \
{                                                                         \
public:                                                                   \
    AsnMsgIn##TARGET##ASNMSG() {}                                         \
                                                                          \
    void Send( Publisher_ABC& publisher )                                 \
    {                                                                     \
        ASN1T_MsgsIn##TARGET asnMsg;                                      \
        asnMsg.msg.t              = T_MsgsIn##TARGET##_msg_msg_##ASNVAR;  \
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
class AsnMsgIn##TARGET##ASNMSG                                            \
{                                                                         \
public:                                                                   \
    AsnMsgIn##TARGET##ASNMSG() {}                                         \
                                                                          \
    void Send( Publisher_ABC& publisher )                                 \
    {                                                                     \
        ASN1T_MsgsIn##TARGET asnMsg;                                      \
        asnMsg.msg.t              = T_MsgsIn##TARGET##_msg_msg_##ASNVAR;  \
        asnMsg.msg.u.msg_##ASNVAR = &asnTmp;                              \
        publisher.Send( asnMsg );                                         \
    }                                                                     \
                                                                          \
    ASN1T_Msg##ASNMSG& operator()() { return asnTmp; }                    \
private:                                                                  \
    ASN1T_Msg##ASNMSG asnTmp;                                             \
                                                                          \
};

GENERATE_NOPTR_ASN_MSG_SENDER   ( Client, LimitCreationRequestAck                       , limit_creation_request_ack                        ); 
GENERATE_NOPTR_ASN_MSG_SENDER   ( Client, LimitDestructionRequestAck                    , limit_destruction_request_ack                     );
GENERATE_NOPTR_ASN_MSG_SENDER   ( Client, LimitUpdateRequestAck                         , limit_update_request_ack                          );
GENERATE_NOPTR_ASN_MSG_SENDER   ( Client, LimaCreationRequestAck                        , lima_creation_request_ack                         );
GENERATE_NOPTR_ASN_MSG_SENDER   ( Client, LimaDestructionRequestAck                     , lima_destruction_request_ack                      );
GENERATE_NOPTR_ASN_MSG_SENDER   ( Client, LimaUpdateRequestAck                          , lima_update_request_ack                           );
GENERATE_ASN_MSG_SENDER         ( Client, PionOrderAck                                  , pion_order_ack                                    );
GENERATE_ASN_MSG_SENDER         ( Client, AutomateOrderAck                              , automate_order_ack                                );
GENERATE_ASN_MSG_SENDER         ( Client, PopulationOrderAck                            , population_order_ack                              );
GENERATE_ASN_MSG_SENDER         ( Client, FragOrderAck                                  , frag_order_ack                                    );
GENERATE_ASN_MSG_SENDER         ( Client, SetAutomateModeAck                            , set_automate_mode_ack                             );
GENERATE_ASN_MSG_SENDER         ( Client, UnitMagicActionAck                            , unit_magic_action_ack                             );
GENERATE_ASN_MSG_SENDER         ( Client, ObjectMagicActionAck                          , object_magic_action_ack                           );
GENERATE_ASN_MSG_SENDER         ( Client, PopulationMagicActionAck                      , population_magic_action_ack                       );
GENERATE_ASN_MSG_SENDER         ( Client, ChangeDiplomatieAck                           , change_diplomatie_ack                             );
GENERATE_ASN_MSG_SENDER         ( Client, ChangeGroupeConnaissanceAck                   , change_groupe_connaissance_ack                    );
GENERATE_ASN_MSG_SENDER         ( Client, ChangeLiensLogistiquesAck                     , change_liens_logistiques_ack                      );
GENERATE_ASN_MSG_SENDER         ( Client, ChangeLiensLogistiques                        , change_liens_logistiques                          );
GENERATE_ASN_MSG_SENDER         ( Client, ChangeAutomateAck                             , change_automate_ack                               );
GENERATE_NOPTR_ASN_MSG_SENDER   ( Client, LogRavitaillementPousserFluxAck               , log_ravitaillement_pousser_flux_ack               );
GENERATE_NOPTR_ASN_MSG_SENDER   ( Client, LogRavitaillementChangeQuotasAck              , log_ravitaillement_change_quotas_ack              );
GENERATE_ASN_MSG_SENDER         ( Client, CtrlInfo                                      , ctrl_info                                         );
GENERATE_NOPTR_ASN_MSG_SENDER   ( Client, CtrlBeginTick                                 , ctrl_begin_tick                                   );
GENERATE_ASN_MSG_SENDER         ( Client, CtrlEndTick                                   , ctrl_end_tick                                     );
GENERATE_NOPTR_ASN_MSG_SENDER   ( Client, CtrlStopAck                                   , ctrl_stop_ack                                     );
GENERATE_NOPTR_ASN_MSG_SENDER   ( Client, CtrlPauseAck                                  , ctrl_pause_ack                                    );
GENERATE_NOPTR_ASN_MSG_SENDER   ( Client, CtrlResumeAck                                 , ctrl_resume_ack                                   );
GENERATE_ASN_MSG_SENDER         ( Client, CtrlChangeTimeFactorAck                       , ctrl_change_time_factor_ack                       );
GENERATE_EMPTY_ASN_MSG_SENDER   ( Client, CtrlMeteoGlobaleAck                           , ctrl_meteo_globale_ack                            );
GENERATE_EMPTY_ASN_MSG_SENDER   ( Client, CtrlMeteoLocaleAck                            , ctrl_meteo_locale_ack                             );
GENERATE_EMPTY_ASN_MSG_SENDER   ( Client, CtrlCheckPointSaveBegin                       , ctrl_checkpoint_save_begin                        );
GENERATE_EMPTY_ASN_MSG_SENDER   ( Client, CtrlCheckPointSaveEnd                         , ctrl_checkpoint_save_end                          );
GENERATE_EMPTY_ASN_MSG_SENDER   ( Client, CtrlCheckPointLoadBegin                       , ctrl_checkpoint_load_begin                        );
GENERATE_EMPTY_ASN_MSG_SENDER   ( Client, CtrlCheckPointLoadEnd                         , ctrl_checkpoint_load_end                          );
GENERATE_EMPTY_ASN_MSG_SENDER   ( Client, CtrlCheckPointSetFrequencyAck                 , ctrl_checkpoint_set_frequency_ack                 );
GENERATE_EMPTY_ASN_MSG_SENDER   ( Client, CtrlCheckPointSaveNowAck                      , ctrl_checkpoint_save_now_ack                      );
GENERATE_EMPTY_ASN_MSG_SENDER   ( Client, CtrlSendCurrentStateBegin                     , ctrl_send_current_state_begin                     );
GENERATE_EMPTY_ASN_MSG_SENDER   ( Client, CtrlSendCurrentStateEnd                       , ctrl_send_current_state_end                       );
GENERATE_ASN_MSG_SENDER         ( Client, LimitCreation                                 , limit_creation                                    );
GENERATE_NOPTR_ASN_MSG_SENDER   ( Client, LimitDestruction                              , limit_destruction                                 );
GENERATE_ASN_MSG_SENDER         ( Client, LimaCreation                                  , lima_creation                                     );
GENERATE_NOPTR_ASN_MSG_SENDER   ( Client, LimaDestruction                               , lima_destruction                                  );
GENERATE_ASN_MSG_SENDER         ( Client, SideCreation                                  , side_creation                                     );
GENERATE_ASN_MSG_SENDER         ( Client, KnowledgeGroupCreation                        , knowledge_group_creation                          );
GENERATE_ASN_MSG_SENDER         ( Client, FormationCreation                             , formation_creation                                );
GENERATE_ASN_MSG_SENDER         ( Client, AutomateCreation                              , automate_creation                                 );
GENERATE_ASN_MSG_SENDER         ( Client, AutomateAttributes                            , automate_attributes                               );
GENERATE_ASN_MSG_SENDER         ( Client, PionCreation                                  , pion_creation                                     );
GENERATE_ASN_MSG_SENDER         ( Client, UnitAttributes                                , unit_attributes                                   );
GENERATE_ASN_MSG_SENDER         ( Client, UnitPathFind                                  , unit_pathfind                                     );
GENERATE_ASN_MSG_SENDER         ( Client, ChangeDiplomatie                              , change_diplomatie                                 );
GENERATE_ASN_MSG_SENDER         ( Client, UnitKnowledgeCreation                         , unit_knowledge_creation                           );
GENERATE_ASN_MSG_SENDER         ( Client, UnitKnowledgeUpdate                           , unit_knowledge_update                             );
GENERATE_ASN_MSG_SENDER         ( Client, UnitKnowledgeDestruction                      , unit_knowledge_destruction                        );
GENERATE_ASN_MSG_SENDER         ( Client, StartPionFire                                 , start_pion_fire                                   );
GENERATE_ASN_MSG_SENDER         ( Client, StopPionFire                                  , stop_pion_fire                                    );
GENERATE_ASN_MSG_SENDER         ( Client, StartPopulationFire                           , start_population_fire                             );
GENERATE_ASN_MSG_SENDER         ( Client, StopPopulationFire                            , stop_population_fire                              );
GENERATE_ASN_MSG_SENDER         ( Client, Explosion                                     , explosion                                         );
GENERATE_ASN_MSG_SENDER         ( Client, StartFireEffect                               , start_fire_effect                                 );
GENERATE_NOPTR_ASN_MSG_SENDER   ( Client, StopFireEffect                                , stop_fire_effect                                  );
GENERATE_ASN_MSG_SENDER         ( Client, CR                                            , cr                                                );
GENERATE_ASN_MSG_SENDER         ( Client, PionOrderManagement                           , pion_order_management                             );
GENERATE_ASN_MSG_SENDER         ( Client, AutomateOrderManagement                       , automate_order_management                         );
GENERATE_ASN_MSG_SENDER         ( Client, PopulationOrderManagement                     , population_order_management                       );
GENERATE_ASN_MSG_SENDER         ( Client, PionOrder                                     , pion_order                                        );
GENERATE_ASN_MSG_SENDER         ( Client, AutomateOrder                                 , automate_order                                    );
GENERATE_ASN_MSG_SENDER         ( Client, ObjectCreation                                , object_creation                                   );
GENERATE_NOPTR_ASN_MSG_SENDER   ( Client, ObjectDestruction                             , object_destruction                                );
GENERATE_ASN_MSG_SENDER         ( Client, ObjectUpdate                                  , object_update                                     );
GENERATE_ASN_MSG_SENDER         ( Client, ObjectKnowledgeCreation                       , object_knowledge_creation                         );
GENERATE_ASN_MSG_SENDER         ( Client, ObjectKnowledgeUpdate                         , object_knowledge_update                           );
GENERATE_ASN_MSG_SENDER         ( Client, ObjectKnowledgeDestruction                    , object_knowledge_destruction                      );
GENERATE_ASN_MSG_SENDER         ( Client, ChangeAutomate                                , change_automate                                   );
GENERATE_ASN_MSG_SENDER         ( Client, LogSanteTraitementHumainCreation              , log_sante_traitement_humain_creation              );
GENERATE_ASN_MSG_SENDER         ( Client, LogSanteTraitementHumainUpdate                , log_sante_traitement_humain_update                );
GENERATE_ASN_MSG_SENDER         ( Client, LogSanteTraitementHumainDestruction           , log_sante_traitement_humain_destruction           );
GENERATE_ASN_MSG_SENDER         ( Client, LogSanteEtat                                  , log_sante_etat                                    );
GENERATE_ASN_MSG_SENDER         ( Client, LogMaintenanceTraitementEquipementCreation    , log_maintenance_traitement_equipement_creation    );
GENERATE_ASN_MSG_SENDER         ( Client, LogMaintenanceTraitementEquipementUpdate      , log_maintenance_traitement_equipement_update      );
GENERATE_ASN_MSG_SENDER         ( Client, LogMaintenanceTraitementEquipementDestruction , log_maintenance_traitement_equipement_destruction );
GENERATE_ASN_MSG_SENDER         ( Client, LogMaintenanceEtat                            , log_maintenance_etat                              );
GENERATE_ASN_MSG_SENDER         ( Client, LogRavitaillementTraitementCreation           , log_ravitaillement_traitement_creation            );
GENERATE_ASN_MSG_SENDER         ( Client, LogRavitaillementTraitementUpdate             , log_ravitaillement_traitement_update              );
GENERATE_ASN_MSG_SENDER         ( Client, LogRavitaillementTraitementDestruction        , log_ravitaillement_traitement_destruction         );
GENERATE_ASN_MSG_SENDER         ( Client, LogRavitaillementEtat                         , log_ravitaillement_etat                           );
GENERATE_ASN_MSG_SENDER         ( Client, LogRavitaillementQuotas                       , log_ravitaillement_quotas                         );
GENERATE_ASN_MSG_SENDER         ( Client, PopulationCreation                            , population_creation                               );
GENERATE_ASN_MSG_SENDER         ( Client, PopulationUpdate                              , population_update                                 );
GENERATE_ASN_MSG_SENDER         ( Client, PopulationConcentrationCreation               , population_concentration_creation                 );
GENERATE_ASN_MSG_SENDER         ( Client, PopulationConcentrationDestruction            , population_concentration_destruction              );
GENERATE_ASN_MSG_SENDER         ( Client, PopulationConcentrationUpdate                 , population_concentration_update                   );
GENERATE_ASN_MSG_SENDER         ( Client, PopulationFluxCreation                        , population_flux_creation                          );
GENERATE_ASN_MSG_SENDER         ( Client, PopulationFluxDestruction                     , population_flux_destruction                       );
GENERATE_ASN_MSG_SENDER         ( Client, PopulationFluxUpdate                          , population_flux_update                            );
GENERATE_ASN_MSG_SENDER         ( Client, PopulationKnowledgeCreation                   , population_knowledge_creation                     );
GENERATE_ASN_MSG_SENDER         ( Client, PopulationKnowledgeUpdate                     , population_knowledge_update                       );
GENERATE_ASN_MSG_SENDER         ( Client, PopulationKnowledgeDestruction                , population_knowledge_destruction                  );
GENERATE_ASN_MSG_SENDER         ( Client, PopulationConcentrationKnowledgeCreation      , population_concentration_knowledge_creation       );
GENERATE_ASN_MSG_SENDER         ( Client, PopulationConcentrationKnowledgeDestruction   , population_concentration_knowledge_destruction    );
GENERATE_ASN_MSG_SENDER         ( Client, PopulationConcentrationKnowledgeUpdate        , population_concentration_knowledge_update         );
GENERATE_ASN_MSG_SENDER         ( Client, PopulationFluxKnowledgeCreation               , population_flux_knowledge_creation                );
GENERATE_ASN_MSG_SENDER         ( Client, PopulationFluxKnowledgeDestruction            , population_flux_knowledge_destruction             );
GENERATE_ASN_MSG_SENDER         ( Client, PopulationFluxKnowledgeUpdate                 , population_flux_knowledge_update                  );
GENERATE_ASN_MSG_SENDER         ( Client, AuthLoginAck                                  , auth_login_ack                                    );
GENERATE_ASN_MSG_SENDER         ( Client, CtrlProfileCreation                           , ctrl_profile_creation                             );
GENERATE_ASN_MSG_SENDER         ( Client, CtrlProfileUpdate                             , ctrl_profile_update                               );
GENERATE_NOPTR_ASN_MSG_SENDER   ( Client, CtrlProfileDestruction                        , ctrl_profile_destruction                          );

GENERATE_EMPTY_ASN_MSG_SENDER   ( Sim, CtrlClientAnnouncement        , ctrl_client_announcement         );
GENERATE_EMPTY_ASN_MSG_SENDER   ( Sim, CtrlStop                      , ctrl_stop                        );
GENERATE_EMPTY_ASN_MSG_SENDER   ( Sim, CtrlPause                     , ctrl_pause                       );
GENERATE_EMPTY_ASN_MSG_SENDER   ( Sim, CtrlResume                    , ctrl_resume                      );
GENERATE_NOPTR_ASN_MSG_SENDER   ( Sim, CtrlChangeTimeFactor          , ctrl_change_time_factor          );
GENERATE_ASN_MSG_SENDER         ( Sim, CtrlMeteoGlobale              , ctrl_meteo_globale               );
GENERATE_ASN_MSG_SENDER         ( Sim, CtrlMeteoLocale               , ctrl_meteo_locale                );
GENERATE_ASN_MSG_SENDER         ( Sim, CtrlCheckPointSaveNow         , ctrl_checkpoint_save_now         );
GENERATE_NOPTR_ASN_MSG_SENDER   ( Sim, CtrlCheckPointSetFrequency    , ctrl_checkpoint_set_frequency    );
GENERATE_ASN_MSG_SENDER         ( Sim, LimitCreationRequest          , limit_creation_request           );
GENERATE_NOPTR_ASN_MSG_SENDER   ( Sim, LimitDestructionRequest       , limit_destruction_request        );
GENERATE_ASN_MSG_SENDER         ( Sim, LimitUpdateRequest            , limit_update_request             );
GENERATE_ASN_MSG_SENDER         ( Sim, LimaCreationRequest           , lima_creation_request            );
GENERATE_NOPTR_ASN_MSG_SENDER   ( Sim, LimaDestructionRequest        , lima_destruction_request         );
GENERATE_ASN_MSG_SENDER         ( Sim, LimaUpdateRequest             , lima_update_request              );
GENERATE_ASN_MSG_SENDER         ( Sim, PionOrder                     , pion_order                       );
GENERATE_ASN_MSG_SENDER         ( Sim, AutomateOrder                 , automate_order                   );
GENERATE_ASN_MSG_SENDER         ( Sim, PopulationOrder               , population_order                 );
GENERATE_ASN_MSG_SENDER         ( Sim, FragOrder                     , frag_order                       );
GENERATE_ASN_MSG_SENDER         ( Sim, SetAutomateMode               , set_automate_mode                );
GENERATE_ASN_MSG_SENDER         ( Sim, UnitMagicAction               , unit_magic_action                );
GENERATE_ASN_MSG_SENDER         ( Sim, ObjectMagicAction             , object_magic_action              );
GENERATE_ASN_MSG_SENDER         ( Sim, PopulationMagicAction         , population_magic_action          );
GENERATE_ASN_MSG_SENDER         ( Sim, ChangeDiplomatie              , change_diplomatie                );
GENERATE_ASN_MSG_SENDER         ( Sim, ChangeGroupeConnaissance      , change_groupe_connaissance       );
GENERATE_ASN_MSG_SENDER         ( Sim, ChangeLiensLogistiques        , change_liens_logistiques         );
GENERATE_ASN_MSG_SENDER         ( Sim, ChangeAutomate                , change_automate                  );
GENERATE_ASN_MSG_SENDER         ( Sim, LogRavitaillementPousserFlux  , log_ravitaillement_pousser_flux  );
GENERATE_ASN_MSG_SENDER         ( Sim, LogRavitaillementChangeQuotas , log_ravitaillement_change_quotas );


template< typename L, typename E, typename P > 
void SendContainerValues( const P& container, L& asnList )
{
    asnList.n = container.size();
    if( !container.empty() )
    {
        asnList.elem = new E[ container.size() ];
        uint i = 0;
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
        uint i = 0;
        for( P::const_iterator it = container.begin(); it != container.end(); ++it, ++i )
            asnList.elem[ i ] = (**it).GetID();
    }
}

} // end namespace dispatcher

#endif // __NetworkDef_h__

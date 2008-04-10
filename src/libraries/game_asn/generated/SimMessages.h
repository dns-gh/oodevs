/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 09-Apr-2008.
 */
#ifndef SIMMESSAGES_H
#define SIMMESSAGES_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "Base.h"
#include "Order.h"
#include "Control.h"
#include "KnowledgeGroup.h"
#include "Formation.h"
#include "Fire.h"
#include "Unit.h"
#include "UnitKnowledge.h"
#include "Object.h"
#include "ObjectKnowledge.h"
#include "Population.h"
#include "PopulationKnowledge.h"
#include "LogMaintenance.h"
#include "LogRavitaillement.h"
#include "LogSante.h"
#include "Automat.h"
#include "Army.h"
#include "Auth.h"
#include "Folk.h"

/**************************************************************/
/*                                                            */
/*  SimMessagesIncludes                                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_SimMessagesIncludes {
} ASN1T_SimMessagesIncludes;

class EXTERN ASN1C_SimMessagesIncludes : public ASN1CType {
public:
   ASN1T_SimMessagesIncludes& msgData;
   ASN1C_SimMessagesIncludes (
      ASN1MessageBuffer& msgBuf, ASN1T_SimMessagesIncludes& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_SimMessagesIncludes (ASN1CTXT* ctxt_p, ASN1T_SimMessagesIncludes* pvalue);
EXTERN int asn1PD_SimMessagesIncludes (ASN1CTXT* ctxt_p, ASN1T_SimMessagesIncludes* pvalue);

EXTERN void asn1Print_SimMessagesIncludes (ASN1ConstCharPtr name, ASN1T_SimMessagesIncludes* pvalue);

/**************************************************************/
/*                                                            */
/*  SimMessagesExports                                        */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_SimMessagesExports : public ASN1CType {
public:
   ASN1C_SimMessagesExports (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_SimMessagesExports (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_SimMessagesExports (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_SimMessagesExports (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgsSimToClient_msg                                       */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_MsgsSimToClient_msg_msg_unit_order_ack 1
#define T_MsgsSimToClient_msg_msg_automat_order_ack 2
#define T_MsgsSimToClient_msg_msg_population_order_ack 3
#define T_MsgsSimToClient_msg_msg_frag_order_ack 4
#define T_MsgsSimToClient_msg_msg_set_automat_mode_ack 5
#define T_MsgsSimToClient_msg_msg_unit_creation_request_ack 6
#define T_MsgsSimToClient_msg_msg_unit_magic_action_ack 7
#define T_MsgsSimToClient_msg_msg_object_magic_action_ack 8
#define T_MsgsSimToClient_msg_msg_population_magic_action_ack 9
#define T_MsgsSimToClient_msg_msg_change_diplomacy_ack 10
#define T_MsgsSimToClient_msg_msg_automat_change_knowledge_group_ack 11
#define T_MsgsSimToClient_msg_msg_automat_change_logistic_links_ack 12
#define T_MsgsSimToClient_msg_msg_automat_change_superior_ack 13
#define T_MsgsSimToClient_msg_msg_unit_change_superior_ack 14
#define T_MsgsSimToClient_msg_msg_log_supply_push_flow_ack 15
#define T_MsgsSimToClient_msg_msg_log_supply_change_quotas_ack 16
#define T_MsgsSimToClient_msg_msg_control_information 17
#define T_MsgsSimToClient_msg_msg_control_profiling_information 18
#define T_MsgsSimToClient_msg_msg_control_begin_tick 19
#define T_MsgsSimToClient_msg_msg_control_end_tick 20
#define T_MsgsSimToClient_msg_msg_control_stop_ack 21
#define T_MsgsSimToClient_msg_msg_control_pause_ack 22
#define T_MsgsSimToClient_msg_msg_control_resume_ack 23
#define T_MsgsSimToClient_msg_msg_control_change_time_factor_ack 24
#define T_MsgsSimToClient_msg_msg_control_global_meteo_ack 25
#define T_MsgsSimToClient_msg_msg_control_local_meteo_ack 26
#define T_MsgsSimToClient_msg_msg_control_checkpoint_save_begin 27
#define T_MsgsSimToClient_msg_msg_control_checkpoint_save_end 28
#define T_MsgsSimToClient_msg_msg_control_checkpoint_set_frequency_ack 29
#define T_MsgsSimToClient_msg_msg_control_checkpoint_save_now_ack 30
#define T_MsgsSimToClient_msg_msg_control_send_current_state_begin 31
#define T_MsgsSimToClient_msg_msg_control_send_current_state_end 32
#define T_MsgsSimToClient_msg_msg_formation_creation 33
#define T_MsgsSimToClient_msg_msg_knowledge_group_creation 34
#define T_MsgsSimToClient_msg_msg_side_creation 35
#define T_MsgsSimToClient_msg_msg_automat_creation 36
#define T_MsgsSimToClient_msg_msg_automat_attributes 37
#define T_MsgsSimToClient_msg_msg_unit_creation 38
#define T_MsgsSimToClient_msg_msg_unit_attributes 39
#define T_MsgsSimToClient_msg_msg_unit_pathfind 40
#define T_MsgsSimToClient_msg_msg_unit_destruction 41
#define T_MsgsSimToClient_msg_msg_unit_environment_type 42
#define T_MsgsSimToClient_msg_msg_change_diplomacy 43
#define T_MsgsSimToClient_msg_msg_unit_change_superior 44
#define T_MsgsSimToClient_msg_msg_automat_change_logistic_links 45
#define T_MsgsSimToClient_msg_msg_automat_change_knowledge_group 46
#define T_MsgsSimToClient_msg_msg_automat_change_superior 47
#define T_MsgsSimToClient_msg_msg_unit_knowledge_creation 48
#define T_MsgsSimToClient_msg_msg_unit_knowledge_update 49
#define T_MsgsSimToClient_msg_msg_unit_knowledge_destruction 50
#define T_MsgsSimToClient_msg_msg_start_unit_fire 51
#define T_MsgsSimToClient_msg_msg_stop_unit_fire 52
#define T_MsgsSimToClient_msg_msg_start_population_fire 53
#define T_MsgsSimToClient_msg_msg_stop_population_fire 54
#define T_MsgsSimToClient_msg_msg_explosion 55
#define T_MsgsSimToClient_msg_msg_start_fire_effect 56
#define T_MsgsSimToClient_msg_msg_stop_fire_effect 57
#define T_MsgsSimToClient_msg_msg_report 58
#define T_MsgsSimToClient_msg_msg_invalidate_report 59
#define T_MsgsSimToClient_msg_msg_trace 60
#define T_MsgsSimToClient_msg_msg_decisional_state 61
#define T_MsgsSimToClient_msg_msg_debug_points 62
#define T_MsgsSimToClient_msg_msg_unit_vision_cones 63
#define T_MsgsSimToClient_msg_msg_unit_detection 64
#define T_MsgsSimToClient_msg_msg_object_detection 65
#define T_MsgsSimToClient_msg_msg_population_concentration_detection 66
#define T_MsgsSimToClient_msg_msg_population_flow_detection 67
#define T_MsgsSimToClient_msg_msg_unit_order 68
#define T_MsgsSimToClient_msg_msg_automat_order 69
#define T_MsgsSimToClient_msg_msg_population_order 70
#define T_MsgsSimToClient_msg_msg_object_creation 71
#define T_MsgsSimToClient_msg_msg_object_destruction 72
#define T_MsgsSimToClient_msg_msg_object_update 73
#define T_MsgsSimToClient_msg_msg_object_knowledge_creation 74
#define T_MsgsSimToClient_msg_msg_object_knowledge_update 75
#define T_MsgsSimToClient_msg_msg_object_knowledge_destruction 76
#define T_MsgsSimToClient_msg_msg_log_medical_handling_creation 77
#define T_MsgsSimToClient_msg_msg_log_medical_handling_update 78
#define T_MsgsSimToClient_msg_msg_log_medical_handling_destruction 79
#define T_MsgsSimToClient_msg_msg_log_medical_state 80
#define T_MsgsSimToClient_msg_msg_log_maintenance_handling_creation 81
#define T_MsgsSimToClient_msg_msg_log_maintenance_handling_update 82
#define T_MsgsSimToClient_msg_msg_log_maintenance_handling_destruction 83
#define T_MsgsSimToClient_msg_msg_log_maintenance_state 84
#define T_MsgsSimToClient_msg_msg_log_supply_handling_creation 85
#define T_MsgsSimToClient_msg_msg_log_supply_handling_update 86
#define T_MsgsSimToClient_msg_msg_log_supply_handling_destruction 87
#define T_MsgsSimToClient_msg_msg_log_supply_state 88
#define T_MsgsSimToClient_msg_msg_log_supply_quotas 89
#define T_MsgsSimToClient_msg_msg_population_creation 90
#define T_MsgsSimToClient_msg_msg_population_update 91
#define T_MsgsSimToClient_msg_msg_population_concentration_creation 92
#define T_MsgsSimToClient_msg_msg_population_concentration_destruction 93
#define T_MsgsSimToClient_msg_msg_population_concentration_update 94
#define T_MsgsSimToClient_msg_msg_population_flow_creation 95
#define T_MsgsSimToClient_msg_msg_population_flow_destruction 96
#define T_MsgsSimToClient_msg_msg_population_flow_update 97
#define T_MsgsSimToClient_msg_msg_population_knowledge_creation 98
#define T_MsgsSimToClient_msg_msg_population_knowledge_update 99
#define T_MsgsSimToClient_msg_msg_population_knowledge_destruction 100
#define T_MsgsSimToClient_msg_msg_population_concentration_knowledge_creation 101
#define T_MsgsSimToClient_msg_msg_population_concentration_knowledge_destruction 102
#define T_MsgsSimToClient_msg_msg_population_concentration_knowledge_update 103
#define T_MsgsSimToClient_msg_msg_population_flow_knowledge_creation 104
#define T_MsgsSimToClient_msg_msg_population_flow_knowledge_destruction 105
#define T_MsgsSimToClient_msg_msg_population_flow_knowledge_update 106
#define T_MsgsSimToClient_msg_msg_folk_creation 107
#define T_MsgsSimToClient_msg_msg_folk_graph_update 108

typedef struct EXTERN ASN1T_MsgsSimToClient_msg {
   int t;
   union {
      /* t = 1 */
      ASN1T_MsgUnitOrderAck *msg_unit_order_ack;
      /* t = 2 */
      ASN1T_MsgAutomatOrderAck *msg_automat_order_ack;
      /* t = 3 */
      ASN1T_MsgPopulationOrderAck *msg_population_order_ack;
      /* t = 4 */
      ASN1T_MsgFragOrderAck *msg_frag_order_ack;
      /* t = 5 */
      ASN1T_MsgSetAutomatModeAck *msg_set_automat_mode_ack;
      /* t = 6 */
      ASN1T_MsgUnitCreationRequestAck  msg_unit_creation_request_ack;
      /* t = 7 */
      ASN1T_MsgUnitMagicActionAck *msg_unit_magic_action_ack;
      /* t = 8 */
      ASN1T_MsgObjectMagicActionAck *msg_object_magic_action_ack;
      /* t = 9 */
      ASN1T_MsgPopulationMagicActionAck *msg_population_magic_action_ack;
      /* t = 10 */
      ASN1T_MsgChangeDiplomacyAck *msg_change_diplomacy_ack;
      /* t = 11 */
      ASN1T_MsgAutomatChangeKnowledgeGroupAck  msg_automat_change_knowledge_group_ack;
      /* t = 12 */
      ASN1T_MsgAutomatChangeLogisticLinksAck  msg_automat_change_logistic_links_ack;
      /* t = 13 */
      ASN1T_MsgAutomatChangeSuperiorAck  msg_automat_change_superior_ack;
      /* t = 14 */
      ASN1T_MsgUnitChangeSuperiorAck  msg_unit_change_superior_ack;
      /* t = 15 */
      ASN1T_MsgLogSupplyPushFlowAck  msg_log_supply_push_flow_ack;
      /* t = 16 */
      ASN1T_MsgLogSupplyChangeQuotasAck  msg_log_supply_change_quotas_ack;
      /* t = 17 */
      ASN1T_MsgControlInformation *msg_control_information;
      /* t = 18 */
      ASN1T_MsgControlProfilingInformation *msg_control_profiling_information;
      /* t = 19 */
      ASN1T_MsgControlBeginTick *msg_control_begin_tick;
      /* t = 20 */
      ASN1T_MsgControlEndTick *msg_control_end_tick;
      /* t = 21 */
      ASN1T_MsgControlStopAck  msg_control_stop_ack;
      /* t = 22 */
      ASN1T_MsgControlPauseAck  msg_control_pause_ack;
      /* t = 23 */
      ASN1T_MsgControlResumeAck  msg_control_resume_ack;
      /* t = 24 */
      ASN1T_MsgControlChangeTimeFactorAck *msg_control_change_time_factor_ack;
      /* t = 25 */
      /* t = 26 */
      /* t = 27 */
      /* t = 28 */
      ASN1T_MsgControlCheckPointSaveEnd *msg_control_checkpoint_save_end;
      /* t = 29 */
      /* t = 30 */
      /* t = 31 */
      /* t = 32 */
      /* t = 33 */
      ASN1T_MsgFormationCreation *msg_formation_creation;
      /* t = 34 */
      ASN1T_MsgKnowledgeGroupCreation *msg_knowledge_group_creation;
      /* t = 35 */
      ASN1T_MsgTeamCreation *msg_side_creation;
      /* t = 36 */
      ASN1T_MsgAutomatCreation *msg_automat_creation;
      /* t = 37 */
      ASN1T_MsgAutomatAttributes *msg_automat_attributes;
      /* t = 38 */
      ASN1T_MsgUnitCreation *msg_unit_creation;
      /* t = 39 */
      ASN1T_MsgUnitAttributes *msg_unit_attributes;
      /* t = 40 */
      ASN1T_MsgUnitPathFind *msg_unit_pathfind;
      /* t = 41 */
      ASN1T_MsgUnitDestruction  msg_unit_destruction;
      /* t = 42 */
      ASN1T_MsgUnitEnvironmentType *msg_unit_environment_type;
      /* t = 43 */
      ASN1T_MsgChangeDiplomacy *msg_change_diplomacy;
      /* t = 44 */
      ASN1T_MsgUnitChangeSuperior *msg_unit_change_superior;
      /* t = 45 */
      ASN1T_MsgAutomatChangeLogisticLinks *msg_automat_change_logistic_links;
      /* t = 46 */
      ASN1T_MsgAutomatChangeKnowledgeGroup *msg_automat_change_knowledge_group;
      /* t = 47 */
      ASN1T_MsgAutomatChangeSuperior *msg_automat_change_superior;
      /* t = 48 */
      ASN1T_MsgUnitKnowledgeCreation *msg_unit_knowledge_creation;
      /* t = 49 */
      ASN1T_MsgUnitKnowledgeUpdate *msg_unit_knowledge_update;
      /* t = 50 */
      ASN1T_MsgUnitKnowledgeDestruction *msg_unit_knowledge_destruction;
      /* t = 51 */
      ASN1T_MsgStartUnitFire *msg_start_unit_fire;
      /* t = 52 */
      ASN1T_MsgStopUnitFire *msg_stop_unit_fire;
      /* t = 53 */
      ASN1T_MsgStartPopulationFire *msg_start_population_fire;
      /* t = 54 */
      ASN1T_MsgStopPopulationFire *msg_stop_population_fire;
      /* t = 55 */
      ASN1T_MsgExplosion *msg_explosion;
      /* t = 56 */
      ASN1T_MsgStartFireEffect *msg_start_fire_effect;
      /* t = 57 */
      ASN1T_MsgStopFireEffect  msg_stop_fire_effect;
      /* t = 58 */
      ASN1T_MsgReport *msg_report;
      /* t = 59 */
      ASN1T_MsgInvalidateReport *msg_invalidate_report;
      /* t = 60 */
      ASN1T_MsgTrace *msg_trace;
      /* t = 61 */
      ASN1T_MsgDecisionalState *msg_decisional_state;
      /* t = 62 */
      ASN1T_MsgDebugPoints *msg_debug_points;
      /* t = 63 */
      ASN1T_MsgUnitVisionCones *msg_unit_vision_cones;
      /* t = 64 */
      ASN1T_MsgUnitDetection *msg_unit_detection;
      /* t = 65 */
      ASN1T_MsgObjectDetection *msg_object_detection;
      /* t = 66 */
      ASN1T_MsgPopulationConcentrationDetection *msg_population_concentration_detection;
      /* t = 67 */
      ASN1T_MsgPopulationFlowDetection *msg_population_flow_detection;
      /* t = 68 */
      ASN1T_MsgUnitOrder *msg_unit_order;
      /* t = 69 */
      ASN1T_MsgAutomatOrder *msg_automat_order;
      /* t = 70 */
      ASN1T_MsgPopulationOrder *msg_population_order;
      /* t = 71 */
      ASN1T_MsgObjectCreation *msg_object_creation;
      /* t = 72 */
      ASN1T_MsgObjectDestruction  msg_object_destruction;
      /* t = 73 */
      ASN1T_MsgObjectUpdate *msg_object_update;
      /* t = 74 */
      ASN1T_MsgObjectKnowledgeCreation *msg_object_knowledge_creation;
      /* t = 75 */
      ASN1T_MsgObjectKnowledgeUpdate *msg_object_knowledge_update;
      /* t = 76 */
      ASN1T_MsgObjectKnowledgeDestruction *msg_object_knowledge_destruction;
      /* t = 77 */
      ASN1T_MsgLogMedicalHandlingCreation *msg_log_medical_handling_creation;
      /* t = 78 */
      ASN1T_MsgLogMedicalHandlingUpdate *msg_log_medical_handling_update;
      /* t = 79 */
      ASN1T_MsgLogMedicalHandlingDestruction *msg_log_medical_handling_destruction;
      /* t = 80 */
      ASN1T_MsgLogMedicalState *msg_log_medical_state;
      /* t = 81 */
      ASN1T_MsgLogMaintenanceHandlingCreation *msg_log_maintenance_handling_creation;
      /* t = 82 */
      ASN1T_MsgLogMaintenanceHandlingUpdate *msg_log_maintenance_handling_update;
      /* t = 83 */
      ASN1T_MsgLogMaintenanceHandlingDestruction *msg_log_maintenance_handling_destruction;
      /* t = 84 */
      ASN1T_MsgLogMaintenanceState *msg_log_maintenance_state;
      /* t = 85 */
      ASN1T_MsgLogSupplyHandlingCreation *msg_log_supply_handling_creation;
      /* t = 86 */
      ASN1T_MsgLogSupplyHandlingUpdate *msg_log_supply_handling_update;
      /* t = 87 */
      ASN1T_MsgLogSupplyHandlingDestruction *msg_log_supply_handling_destruction;
      /* t = 88 */
      ASN1T_MsgLogSupplyState *msg_log_supply_state;
      /* t = 89 */
      ASN1T_MsgLogSupplyQuotas *msg_log_supply_quotas;
      /* t = 90 */
      ASN1T_MsgPopulationCreation *msg_population_creation;
      /* t = 91 */
      ASN1T_MsgPopulationUpdate *msg_population_update;
      /* t = 92 */
      ASN1T_MsgPopulationConcentrationCreation *msg_population_concentration_creation;
      /* t = 93 */
      ASN1T_MsgPopulationConcentrationDestruction *msg_population_concentration_destruction;
      /* t = 94 */
      ASN1T_MsgPopulationConcentrationUpdate *msg_population_concentration_update;
      /* t = 95 */
      ASN1T_MsgPopulationFlowCreation *msg_population_flow_creation;
      /* t = 96 */
      ASN1T_MsgPopulationFlowDestruction *msg_population_flow_destruction;
      /* t = 97 */
      ASN1T_MsgPopulationFlowUpdate *msg_population_flow_update;
      /* t = 98 */
      ASN1T_MsgPopulationKnowledgeCreation *msg_population_knowledge_creation;
      /* t = 99 */
      ASN1T_MsgPopulationKnowledgeUpdate *msg_population_knowledge_update;
      /* t = 100 */
      ASN1T_MsgPopulationKnowledgeDestruction *msg_population_knowledge_destruction;
      /* t = 101 */
      ASN1T_MsgPopulationConcentrationKnowledgeCreation *msg_population_concentration_knowledge_creation;
      /* t = 102 */
      ASN1T_MsgPopulationConcentrationKnowledgeDestruction *msg_population_concentration_knowledge_destruction;
      /* t = 103 */
      ASN1T_MsgPopulationConcentrationKnowledgeUpdate *msg_population_concentration_knowledge_update;
      /* t = 104 */
      ASN1T_MsgPopulationFlowKnowledgeCreation *msg_population_flow_knowledge_creation;
      /* t = 105 */
      ASN1T_MsgPopulationFlowKnowledgeDestruction *msg_population_flow_knowledge_destruction;
      /* t = 106 */
      ASN1T_MsgPopulationFlowKnowledgeUpdate *msg_population_flow_knowledge_update;
      /* t = 107 */
      ASN1T_MsgFolkCreation *msg_folk_creation;
      /* t = 108 */
      ASN1T_MsgFolkGraphUpdate *msg_folk_graph_update;
   } u;
} ASN1T_MsgsSimToClient_msg;

class EXTERN ASN1C_MsgsSimToClient_msg : public ASN1CType {
public:
   ASN1T_MsgsSimToClient_msg& msgData;
   ASN1C_MsgsSimToClient_msg (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgsSimToClient_msg& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgsSimToClient_msg (ASN1CTXT* ctxt_p, ASN1T_MsgsSimToClient_msg* pvalue);
EXTERN int asn1PD_MsgsSimToClient_msg (ASN1CTXT* ctxt_p, ASN1T_MsgsSimToClient_msg* pvalue);

EXTERN void asn1Print_MsgsSimToClient_msg (ASN1ConstCharPtr name, ASN1T_MsgsSimToClient_msg* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgsSimToClient                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgsSimToClient {
   ASN1INT   context;
   ASN1T_MsgsSimToClient_msg  msg;

   ASN1T_MsgsSimToClient () {
      context = 0;
   }
} ASN1T_MsgsSimToClient;

class EXTERN ASN1C_MsgsSimToClient : public ASN1CType {
public:
   ASN1T_MsgsSimToClient& msgData;
   ASN1C_MsgsSimToClient (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgsSimToClient& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgsSimToClient (ASN1CTXT* ctxt_p, ASN1T_MsgsSimToClient* pvalue);
EXTERN int asn1PD_MsgsSimToClient (ASN1CTXT* ctxt_p, ASN1T_MsgsSimToClient* pvalue);

EXTERN void asn1Print_MsgsSimToClient (ASN1ConstCharPtr name, ASN1T_MsgsSimToClient* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgsClientToSim_msg                                       */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_MsgsClientToSim_msg_msg_control_stop 1
#define T_MsgsClientToSim_msg_msg_control_pause 2
#define T_MsgsClientToSim_msg_msg_control_resume 3
#define T_MsgsClientToSim_msg_msg_control_change_time_factor 4
#define T_MsgsClientToSim_msg_msg_control_global_meteo 5
#define T_MsgsClientToSim_msg_msg_control_local_meteo 6
#define T_MsgsClientToSim_msg_msg_control_checkpoint_save_now 7
#define T_MsgsClientToSim_msg_msg_control_checkpoint_set_frequency 8
#define T_MsgsClientToSim_msg_msg_control_toggle_vision_cones 9
#define T_MsgsClientToSim_msg_msg_unit_order 10
#define T_MsgsClientToSim_msg_msg_automat_order 11
#define T_MsgsClientToSim_msg_msg_population_order 12
#define T_MsgsClientToSim_msg_msg_frag_order 13
#define T_MsgsClientToSim_msg_msg_set_automat_mode 14
#define T_MsgsClientToSim_msg_msg_unit_creation_request 15
#define T_MsgsClientToSim_msg_msg_unit_magic_action 16
#define T_MsgsClientToSim_msg_msg_object_magic_action 17
#define T_MsgsClientToSim_msg_msg_population_magic_action 18
#define T_MsgsClientToSim_msg_msg_change_diplomacy 19
#define T_MsgsClientToSim_msg_msg_automat_change_knowledge_group 20
#define T_MsgsClientToSim_msg_msg_automat_change_logistic_links 21
#define T_MsgsClientToSim_msg_msg_automat_change_superior 22
#define T_MsgsClientToSim_msg_msg_unit_change_superior 23
#define T_MsgsClientToSim_msg_msg_log_supply_push_flow 24
#define T_MsgsClientToSim_msg_msg_log_supply_change_quotas 25

typedef struct EXTERN ASN1T_MsgsClientToSim_msg {
   int t;
   union {
      /* t = 1 */
      /* t = 2 */
      /* t = 3 */
      /* t = 4 */
      ASN1T_MsgControlChangeTimeFactor  msg_control_change_time_factor;
      /* t = 5 */
      ASN1T_MsgControlGlobalMeteo *msg_control_global_meteo;
      /* t = 6 */
      ASN1T_MsgControlLocalMeteo *msg_control_local_meteo;
      /* t = 7 */
      ASN1T_MsgControlCheckPointSaveNow *msg_control_checkpoint_save_now;
      /* t = 8 */
      ASN1T_MsgControlCheckPointSetFrequency  msg_control_checkpoint_set_frequency;
      /* t = 9 */
      ASN1T_MsgControlToggleVisionCones  msg_control_toggle_vision_cones;
      /* t = 10 */
      ASN1T_MsgUnitOrder *msg_unit_order;
      /* t = 11 */
      ASN1T_MsgAutomatOrder *msg_automat_order;
      /* t = 12 */
      ASN1T_MsgPopulationOrder *msg_population_order;
      /* t = 13 */
      ASN1T_MsgFragOrder *msg_frag_order;
      /* t = 14 */
      ASN1T_MsgSetAutomatMode *msg_set_automat_mode;
      /* t = 15 */
      ASN1T_MsgUnitCreationRequest *msg_unit_creation_request;
      /* t = 16 */
      ASN1T_MsgUnitMagicAction *msg_unit_magic_action;
      /* t = 17 */
      ASN1T_MsgObjectMagicAction *msg_object_magic_action;
      /* t = 18 */
      ASN1T_MsgPopulationMagicAction *msg_population_magic_action;
      /* t = 19 */
      ASN1T_MsgChangeDiplomacy *msg_change_diplomacy;
      /* t = 20 */
      ASN1T_MsgAutomatChangeKnowledgeGroup *msg_automat_change_knowledge_group;
      /* t = 21 */
      ASN1T_MsgAutomatChangeLogisticLinks *msg_automat_change_logistic_links;
      /* t = 22 */
      ASN1T_MsgAutomatChangeSuperior *msg_automat_change_superior;
      /* t = 23 */
      ASN1T_MsgUnitChangeSuperior *msg_unit_change_superior;
      /* t = 24 */
      ASN1T_MsgLogSupplyPushFlow *msg_log_supply_push_flow;
      /* t = 25 */
      ASN1T_MsgLogSupplyChangeQuotas *msg_log_supply_change_quotas;
   } u;
} ASN1T_MsgsClientToSim_msg;

class EXTERN ASN1C_MsgsClientToSim_msg : public ASN1CType {
public:
   ASN1T_MsgsClientToSim_msg& msgData;
   ASN1C_MsgsClientToSim_msg (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgsClientToSim_msg& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgsClientToSim_msg (ASN1CTXT* ctxt_p, ASN1T_MsgsClientToSim_msg* pvalue);
EXTERN int asn1PD_MsgsClientToSim_msg (ASN1CTXT* ctxt_p, ASN1T_MsgsClientToSim_msg* pvalue);

EXTERN void asn1Print_MsgsClientToSim_msg (ASN1ConstCharPtr name, ASN1T_MsgsClientToSim_msg* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgsClientToSim                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgsClientToSim {
   ASN1INT   context;
   ASN1T_MsgsClientToSim_msg  msg;

   ASN1T_MsgsClientToSim () {
      context = 0;
   }
} ASN1T_MsgsClientToSim;

class EXTERN ASN1C_MsgsClientToSim : public ASN1CType {
public:
   ASN1T_MsgsClientToSim& msgData;
   ASN1C_MsgsClientToSim (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgsClientToSim& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgsClientToSim (ASN1CTXT* ctxt_p, ASN1T_MsgsClientToSim* pvalue);
EXTERN int asn1PD_MsgsClientToSim (ASN1CTXT* ctxt_p, ASN1T_MsgsClientToSim* pvalue);

EXTERN void asn1Print_MsgsClientToSim (ASN1ConstCharPtr name, ASN1T_MsgsClientToSim* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgsMiddleToSim_msg                                       */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_MsgsMiddleToSim_msg_msg_control_client_announcement 1

typedef struct EXTERN ASN1T_MsgsMiddleToSim_msg {
   int t;
} ASN1T_MsgsMiddleToSim_msg;

class EXTERN ASN1C_MsgsMiddleToSim_msg : public ASN1CType {
public:
   ASN1T_MsgsMiddleToSim_msg& msgData;
   ASN1C_MsgsMiddleToSim_msg (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgsMiddleToSim_msg& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgsMiddleToSim_msg (ASN1CTXT* ctxt_p, ASN1T_MsgsMiddleToSim_msg* pvalue);
EXTERN int asn1PD_MsgsMiddleToSim_msg (ASN1CTXT* ctxt_p, ASN1T_MsgsMiddleToSim_msg* pvalue);

EXTERN void asn1Print_MsgsMiddleToSim_msg (ASN1ConstCharPtr name, ASN1T_MsgsMiddleToSim_msg* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgsMiddleToSim                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgsMiddleToSim {
   ASN1INT   context;
   ASN1T_MsgsMiddleToSim_msg  msg;

   ASN1T_MsgsMiddleToSim () {
      context = 0;
   }
} ASN1T_MsgsMiddleToSim;

class EXTERN ASN1C_MsgsMiddleToSim : public ASN1CType {
public:
   ASN1T_MsgsMiddleToSim& msgData;
   ASN1C_MsgsMiddleToSim (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgsMiddleToSim& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgsMiddleToSim (ASN1CTXT* ctxt_p, ASN1T_MsgsMiddleToSim* pvalue);
EXTERN int asn1PD_MsgsMiddleToSim (ASN1CTXT* ctxt_p, ASN1T_MsgsMiddleToSim* pvalue);

EXTERN void asn1Print_MsgsMiddleToSim (ASN1ConstCharPtr name, ASN1T_MsgsMiddleToSim* pvalue);

#endif

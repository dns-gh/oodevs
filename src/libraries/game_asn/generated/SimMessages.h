/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 28-Dec-2009.
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
#include "Urban.h"
#include "UrbanKnowledge.h"

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
#define T_MsgsSimToClient_msg_msg_knowledge_group_change_superior_ack 17
#define T_MsgsSimToClient_msg_msg_knowledge_group_delete_ack 18
#define T_MsgsSimToClient_msg_msg_knowledge_group_set_type_ack 19
#define T_MsgsSimToClient_msg_msg_knowledge_group_creation_ack 20
#define T_MsgsSimToClient_msg_msg_control_information 21
#define T_MsgsSimToClient_msg_msg_control_profiling_information 22
#define T_MsgsSimToClient_msg_msg_control_begin_tick 23
#define T_MsgsSimToClient_msg_msg_control_end_tick 24
#define T_MsgsSimToClient_msg_msg_control_stop_ack 25
#define T_MsgsSimToClient_msg_msg_control_pause_ack 26
#define T_MsgsSimToClient_msg_msg_control_resume_ack 27
#define T_MsgsSimToClient_msg_msg_control_change_time_factor_ack 28
#define T_MsgsSimToClient_msg_msg_control_date_time_change_ack 29
#define T_MsgsSimToClient_msg_msg_control_global_meteo_ack 30
#define T_MsgsSimToClient_msg_msg_control_local_meteo_ack 31
#define T_MsgsSimToClient_msg_msg_control_checkpoint_save_begin 32
#define T_MsgsSimToClient_msg_msg_control_checkpoint_save_end 33
#define T_MsgsSimToClient_msg_msg_control_checkpoint_set_frequency_ack 34
#define T_MsgsSimToClient_msg_msg_control_checkpoint_save_now_ack 35
#define T_MsgsSimToClient_msg_msg_control_send_current_state_begin 36
#define T_MsgsSimToClient_msg_msg_control_send_current_state_end 37
#define T_MsgsSimToClient_msg_msg_formation_creation 38
#define T_MsgsSimToClient_msg_msg_knowledge_group_creation 39
#define T_MsgsSimToClient_msg_msg_side_creation 40
#define T_MsgsSimToClient_msg_msg_automat_creation 41
#define T_MsgsSimToClient_msg_msg_automat_attributes 42
#define T_MsgsSimToClient_msg_msg_unit_creation 43
#define T_MsgsSimToClient_msg_msg_unit_attributes 44
#define T_MsgsSimToClient_msg_msg_unit_pathfind 45
#define T_MsgsSimToClient_msg_msg_unit_destruction 46
#define T_MsgsSimToClient_msg_msg_unit_environment_type 47
#define T_MsgsSimToClient_msg_msg_change_diplomacy 48
#define T_MsgsSimToClient_msg_msg_unit_change_superior 49
#define T_MsgsSimToClient_msg_msg_automat_change_logistic_links 50
#define T_MsgsSimToClient_msg_msg_automat_change_knowledge_group 51
#define T_MsgsSimToClient_msg_msg_automat_change_superior 52
#define T_MsgsSimToClient_msg_msg_unit_knowledge_creation 53
#define T_MsgsSimToClient_msg_msg_unit_knowledge_update 54
#define T_MsgsSimToClient_msg_msg_unit_knowledge_destruction 55
#define T_MsgsSimToClient_msg_msg_start_unit_fire 56
#define T_MsgsSimToClient_msg_msg_stop_unit_fire 57
#define T_MsgsSimToClient_msg_msg_start_population_fire 58
#define T_MsgsSimToClient_msg_msg_stop_population_fire 59
#define T_MsgsSimToClient_msg_msg_explosion 60
#define T_MsgsSimToClient_msg_msg_start_fire_effect 61
#define T_MsgsSimToClient_msg_msg_stop_fire_effect 62
#define T_MsgsSimToClient_msg_msg_report 63
#define T_MsgsSimToClient_msg_msg_invalidate_report 64
#define T_MsgsSimToClient_msg_msg_trace 65
#define T_MsgsSimToClient_msg_msg_decisional_state 66
#define T_MsgsSimToClient_msg_msg_debug_points 67
#define T_MsgsSimToClient_msg_msg_unit_vision_cones 68
#define T_MsgsSimToClient_msg_msg_unit_detection 69
#define T_MsgsSimToClient_msg_msg_object_detection 70
#define T_MsgsSimToClient_msg_msg_urban_detection 71
#define T_MsgsSimToClient_msg_msg_population_concentration_detection 72
#define T_MsgsSimToClient_msg_msg_population_flow_detection 73
#define T_MsgsSimToClient_msg_msg_unit_order 74
#define T_MsgsSimToClient_msg_msg_automat_order 75
#define T_MsgsSimToClient_msg_msg_population_order 76
#define T_MsgsSimToClient_msg_msg_object_creation 77
#define T_MsgsSimToClient_msg_msg_object_destruction 78
#define T_MsgsSimToClient_msg_msg_object_update 79
#define T_MsgsSimToClient_msg_msg_object_knowledge_creation 80
#define T_MsgsSimToClient_msg_msg_object_knowledge_update 81
#define T_MsgsSimToClient_msg_msg_object_knowledge_destruction 82
#define T_MsgsSimToClient_msg_msg_log_medical_handling_creation 83
#define T_MsgsSimToClient_msg_msg_log_medical_handling_update 84
#define T_MsgsSimToClient_msg_msg_log_medical_handling_destruction 85
#define T_MsgsSimToClient_msg_msg_log_medical_state 86
#define T_MsgsSimToClient_msg_msg_log_maintenance_handling_creation 87
#define T_MsgsSimToClient_msg_msg_log_maintenance_handling_update 88
#define T_MsgsSimToClient_msg_msg_log_maintenance_handling_destruction 89
#define T_MsgsSimToClient_msg_msg_log_maintenance_state 90
#define T_MsgsSimToClient_msg_msg_log_supply_handling_creation 91
#define T_MsgsSimToClient_msg_msg_log_supply_handling_update 92
#define T_MsgsSimToClient_msg_msg_log_supply_handling_destruction 93
#define T_MsgsSimToClient_msg_msg_log_supply_state 94
#define T_MsgsSimToClient_msg_msg_log_supply_quotas 95
#define T_MsgsSimToClient_msg_msg_population_creation 96
#define T_MsgsSimToClient_msg_msg_population_update 97
#define T_MsgsSimToClient_msg_msg_population_concentration_creation 98
#define T_MsgsSimToClient_msg_msg_population_concentration_destruction 99
#define T_MsgsSimToClient_msg_msg_population_concentration_update 100
#define T_MsgsSimToClient_msg_msg_population_flow_creation 101
#define T_MsgsSimToClient_msg_msg_population_flow_destruction 102
#define T_MsgsSimToClient_msg_msg_population_flow_update 103
#define T_MsgsSimToClient_msg_msg_population_knowledge_creation 104
#define T_MsgsSimToClient_msg_msg_population_knowledge_update 105
#define T_MsgsSimToClient_msg_msg_population_knowledge_destruction 106
#define T_MsgsSimToClient_msg_msg_population_concentration_knowledge_creation 107
#define T_MsgsSimToClient_msg_msg_population_concentration_knowledge_destruction 108
#define T_MsgsSimToClient_msg_msg_population_concentration_knowledge_update 109
#define T_MsgsSimToClient_msg_msg_population_flow_knowledge_creation 110
#define T_MsgsSimToClient_msg_msg_population_flow_knowledge_destruction 111
#define T_MsgsSimToClient_msg_msg_population_flow_knowledge_update 112
#define T_MsgsSimToClient_msg_msg_folk_creation 113
#define T_MsgsSimToClient_msg_msg_folk_graph_update 114
#define T_MsgsSimToClient_msg_msg_urban_creation 115
#define T_MsgsSimToClient_msg_msg_urban_knowledge_creation 116
#define T_MsgsSimToClient_msg_msg_urban_knowledge_update 117
#define T_MsgsSimToClient_msg_msg_urban_knowledge_destruction 118
#define T_MsgsSimToClient_msg_msg_knowledge_group_update 119
#define T_MsgsSimToClient_msg_msg_knowledge_group_delete 120

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
      ASN1T_MsgKnowledgeGroupChangeSuperiorAck *msg_knowledge_group_change_superior_ack;
      /* t = 18 */
      ASN1T_MsgKnowledgeGroupDeleteAck *msg_knowledge_group_delete_ack;
      /* t = 19 */
      ASN1T_MsgKnowledgeGroupSetTypeAck *msg_knowledge_group_set_type_ack;
      /* t = 20 */
      ASN1T_MsgKnowledgeGroupCreationAck *msg_knowledge_group_creation_ack;
      /* t = 21 */
      ASN1T_MsgControlInformation *msg_control_information;
      /* t = 22 */
      ASN1T_MsgControlProfilingInformation *msg_control_profiling_information;
      /* t = 23 */
      ASN1T_MsgControlBeginTick *msg_control_begin_tick;
      /* t = 24 */
      ASN1T_MsgControlEndTick *msg_control_end_tick;
      /* t = 25 */
      ASN1T_MsgControlStopAck  msg_control_stop_ack;
      /* t = 26 */
      ASN1T_MsgControlPauseAck  msg_control_pause_ack;
      /* t = 27 */
      ASN1T_MsgControlResumeAck  msg_control_resume_ack;
      /* t = 28 */
      ASN1T_MsgControlChangeTimeFactorAck *msg_control_change_time_factor_ack;
      /* t = 29 */
      ASN1T_MsgControlDatetimeChangeAck  msg_control_date_time_change_ack;
      /* t = 30 */
      /* t = 31 */
      /* t = 32 */
      /* t = 33 */
      ASN1T_MsgControlCheckPointSaveEnd *msg_control_checkpoint_save_end;
      /* t = 34 */
      /* t = 35 */
      /* t = 36 */
      /* t = 37 */
      /* t = 38 */
      ASN1T_MsgFormationCreation *msg_formation_creation;
      /* t = 39 */
      ASN1T_MsgKnowledgeGroupCreation *msg_knowledge_group_creation;
      /* t = 40 */
      ASN1T_MsgTeamCreation *msg_side_creation;
      /* t = 41 */
      ASN1T_MsgAutomatCreation *msg_automat_creation;
      /* t = 42 */
      ASN1T_MsgAutomatAttributes *msg_automat_attributes;
      /* t = 43 */
      ASN1T_MsgUnitCreation *msg_unit_creation;
      /* t = 44 */
      ASN1T_MsgUnitAttributes *msg_unit_attributes;
      /* t = 45 */
      ASN1T_MsgUnitPathFind *msg_unit_pathfind;
      /* t = 46 */
      ASN1T_MsgUnitDestruction  msg_unit_destruction;
      /* t = 47 */
      ASN1T_MsgUnitEnvironmentType *msg_unit_environment_type;
      /* t = 48 */
      ASN1T_MsgChangeDiplomacy *msg_change_diplomacy;
      /* t = 49 */
      ASN1T_MsgUnitChangeSuperior *msg_unit_change_superior;
      /* t = 50 */
      ASN1T_MsgAutomatChangeLogisticLinks *msg_automat_change_logistic_links;
      /* t = 51 */
      ASN1T_MsgAutomatChangeKnowledgeGroup *msg_automat_change_knowledge_group;
      /* t = 52 */
      ASN1T_MsgAutomatChangeSuperior *msg_automat_change_superior;
      /* t = 53 */
      ASN1T_MsgUnitKnowledgeCreation *msg_unit_knowledge_creation;
      /* t = 54 */
      ASN1T_MsgUnitKnowledgeUpdate *msg_unit_knowledge_update;
      /* t = 55 */
      ASN1T_MsgUnitKnowledgeDestruction *msg_unit_knowledge_destruction;
      /* t = 56 */
      ASN1T_MsgStartUnitFire *msg_start_unit_fire;
      /* t = 57 */
      ASN1T_MsgStopUnitFire *msg_stop_unit_fire;
      /* t = 58 */
      ASN1T_MsgStartPopulationFire *msg_start_population_fire;
      /* t = 59 */
      ASN1T_MsgStopPopulationFire *msg_stop_population_fire;
      /* t = 60 */
      ASN1T_MsgExplosion *msg_explosion;
      /* t = 61 */
      ASN1T_MsgStartFireEffect *msg_start_fire_effect;
      /* t = 62 */
      ASN1T_MsgStopFireEffect  msg_stop_fire_effect;
      /* t = 63 */
      ASN1T_MsgReport *msg_report;
      /* t = 64 */
      ASN1T_MsgInvalidateReport *msg_invalidate_report;
      /* t = 65 */
      ASN1T_MsgTrace *msg_trace;
      /* t = 66 */
      ASN1T_MsgDecisionalState *msg_decisional_state;
      /* t = 67 */
      ASN1T_MsgDebugPoints *msg_debug_points;
      /* t = 68 */
      ASN1T_MsgUnitVisionCones *msg_unit_vision_cones;
      /* t = 69 */
      ASN1T_MsgUnitDetection *msg_unit_detection;
      /* t = 70 */
      ASN1T_MsgObjectDetection *msg_object_detection;
      /* t = 71 */
      ASN1T_MsgUrbanDetection *msg_urban_detection;
      /* t = 72 */
      ASN1T_MsgPopulationConcentrationDetection *msg_population_concentration_detection;
      /* t = 73 */
      ASN1T_MsgPopulationFlowDetection *msg_population_flow_detection;
      /* t = 74 */
      ASN1T_MsgUnitOrder *msg_unit_order;
      /* t = 75 */
      ASN1T_MsgAutomatOrder *msg_automat_order;
      /* t = 76 */
      ASN1T_MsgPopulationOrder *msg_population_order;
      /* t = 77 */
      ASN1T_MsgObjectCreation *msg_object_creation;
      /* t = 78 */
      ASN1T_MsgObjectDestruction  msg_object_destruction;
      /* t = 79 */
      ASN1T_MsgObjectUpdate *msg_object_update;
      /* t = 80 */
      ASN1T_MsgObjectKnowledgeCreation *msg_object_knowledge_creation;
      /* t = 81 */
      ASN1T_MsgObjectKnowledgeUpdate *msg_object_knowledge_update;
      /* t = 82 */
      ASN1T_MsgObjectKnowledgeDestruction *msg_object_knowledge_destruction;
      /* t = 83 */
      ASN1T_MsgLogMedicalHandlingCreation *msg_log_medical_handling_creation;
      /* t = 84 */
      ASN1T_MsgLogMedicalHandlingUpdate *msg_log_medical_handling_update;
      /* t = 85 */
      ASN1T_MsgLogMedicalHandlingDestruction *msg_log_medical_handling_destruction;
      /* t = 86 */
      ASN1T_MsgLogMedicalState *msg_log_medical_state;
      /* t = 87 */
      ASN1T_MsgLogMaintenanceHandlingCreation *msg_log_maintenance_handling_creation;
      /* t = 88 */
      ASN1T_MsgLogMaintenanceHandlingUpdate *msg_log_maintenance_handling_update;
      /* t = 89 */
      ASN1T_MsgLogMaintenanceHandlingDestruction *msg_log_maintenance_handling_destruction;
      /* t = 90 */
      ASN1T_MsgLogMaintenanceState *msg_log_maintenance_state;
      /* t = 91 */
      ASN1T_MsgLogSupplyHandlingCreation *msg_log_supply_handling_creation;
      /* t = 92 */
      ASN1T_MsgLogSupplyHandlingUpdate *msg_log_supply_handling_update;
      /* t = 93 */
      ASN1T_MsgLogSupplyHandlingDestruction *msg_log_supply_handling_destruction;
      /* t = 94 */
      ASN1T_MsgLogSupplyState *msg_log_supply_state;
      /* t = 95 */
      ASN1T_MsgLogSupplyQuotas *msg_log_supply_quotas;
      /* t = 96 */
      ASN1T_MsgPopulationCreation *msg_population_creation;
      /* t = 97 */
      ASN1T_MsgPopulationUpdate *msg_population_update;
      /* t = 98 */
      ASN1T_MsgPopulationConcentrationCreation *msg_population_concentration_creation;
      /* t = 99 */
      ASN1T_MsgPopulationConcentrationDestruction *msg_population_concentration_destruction;
      /* t = 100 */
      ASN1T_MsgPopulationConcentrationUpdate *msg_population_concentration_update;
      /* t = 101 */
      ASN1T_MsgPopulationFlowCreation *msg_population_flow_creation;
      /* t = 102 */
      ASN1T_MsgPopulationFlowDestruction *msg_population_flow_destruction;
      /* t = 103 */
      ASN1T_MsgPopulationFlowUpdate *msg_population_flow_update;
      /* t = 104 */
      ASN1T_MsgPopulationKnowledgeCreation *msg_population_knowledge_creation;
      /* t = 105 */
      ASN1T_MsgPopulationKnowledgeUpdate *msg_population_knowledge_update;
      /* t = 106 */
      ASN1T_MsgPopulationKnowledgeDestruction *msg_population_knowledge_destruction;
      /* t = 107 */
      ASN1T_MsgPopulationConcentrationKnowledgeCreation *msg_population_concentration_knowledge_creation;
      /* t = 108 */
      ASN1T_MsgPopulationConcentrationKnowledgeDestruction *msg_population_concentration_knowledge_destruction;
      /* t = 109 */
      ASN1T_MsgPopulationConcentrationKnowledgeUpdate *msg_population_concentration_knowledge_update;
      /* t = 110 */
      ASN1T_MsgPopulationFlowKnowledgeCreation *msg_population_flow_knowledge_creation;
      /* t = 111 */
      ASN1T_MsgPopulationFlowKnowledgeDestruction *msg_population_flow_knowledge_destruction;
      /* t = 112 */
      ASN1T_MsgPopulationFlowKnowledgeUpdate *msg_population_flow_knowledge_update;
      /* t = 113 */
      ASN1T_MsgFolkCreation *msg_folk_creation;
      /* t = 114 */
      ASN1T_MsgFolkGraphUpdate *msg_folk_graph_update;
      /* t = 115 */
      ASN1T_MsgUrbanCreation *msg_urban_creation;
      /* t = 116 */
      ASN1T_MsgUrbanKnowledgeCreation *msg_urban_knowledge_creation;
      /* t = 117 */
      ASN1T_MsgUrbanKnowledgeUpdate *msg_urban_knowledge_update;
      /* t = 118 */
      ASN1T_MsgUrbanKnowledgeDestruction *msg_urban_knowledge_destruction;
      /* t = 119 */
      ASN1T_MsgKnowledgeGroupUpdate *msg_knowledge_group_update;
      /* t = 120 */
      ASN1T_MsgKnowledgeGroupDelete *msg_knowledge_group_delete;
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
#define T_MsgsClientToSim_msg_msg_control_date_time_change 5
#define T_MsgsClientToSim_msg_msg_control_global_meteo 6
#define T_MsgsClientToSim_msg_msg_control_local_meteo 7
#define T_MsgsClientToSim_msg_msg_control_checkpoint_save_now 8
#define T_MsgsClientToSim_msg_msg_control_checkpoint_set_frequency 9
#define T_MsgsClientToSim_msg_msg_control_toggle_vision_cones 10
#define T_MsgsClientToSim_msg_msg_unit_order 11
#define T_MsgsClientToSim_msg_msg_automat_order 12
#define T_MsgsClientToSim_msg_msg_population_order 13
#define T_MsgsClientToSim_msg_msg_frag_order 14
#define T_MsgsClientToSim_msg_msg_set_automat_mode 15
#define T_MsgsClientToSim_msg_msg_unit_creation_request 16
#define T_MsgsClientToSim_msg_msg_unit_magic_action 17
#define T_MsgsClientToSim_msg_msg_object_magic_action 18
#define T_MsgsClientToSim_msg_msg_population_magic_action 19
#define T_MsgsClientToSim_msg_msg_change_diplomacy 20
#define T_MsgsClientToSim_msg_msg_automat_change_knowledge_group 21
#define T_MsgsClientToSim_msg_msg_automat_change_logistic_links 22
#define T_MsgsClientToSim_msg_msg_automat_change_superior 23
#define T_MsgsClientToSim_msg_msg_unit_change_superior 24
#define T_MsgsClientToSim_msg_msg_log_supply_push_flow 25
#define T_MsgsClientToSim_msg_msg_log_supply_change_quotas 26
#define T_MsgsClientToSim_msg_msg_knowledge_group_change_superior 27
#define T_MsgsClientToSim_msg_msg_knowledge_group_creation 28
#define T_MsgsClientToSim_msg_msg_knowledge_group_delete 29
#define T_MsgsClientToSim_msg_msg_knowledge_group_set_type 30
#define T_MsgsClientToSim_msg_msg_knowledge_group_enable 31

typedef struct EXTERN ASN1T_MsgsClientToSim_msg {
   int t;
   union {
      /* t = 1 */
      /* t = 2 */
      /* t = 3 */
      /* t = 4 */
      ASN1T_MsgControlChangeTimeFactor  msg_control_change_time_factor;
      /* t = 5 */
      ASN1T_MsgControlDatetimeChange *msg_control_date_time_change;
      /* t = 6 */
      ASN1T_MsgControlGlobalMeteo *msg_control_global_meteo;
      /* t = 7 */
      ASN1T_MsgControlLocalMeteo *msg_control_local_meteo;
      /* t = 8 */
      ASN1T_MsgControlCheckPointSaveNow *msg_control_checkpoint_save_now;
      /* t = 9 */
      ASN1T_MsgControlCheckPointSetFrequency  msg_control_checkpoint_set_frequency;
      /* t = 10 */
      ASN1T_MsgControlToggleVisionCones  msg_control_toggle_vision_cones;
      /* t = 11 */
      ASN1T_MsgUnitOrder *msg_unit_order;
      /* t = 12 */
      ASN1T_MsgAutomatOrder *msg_automat_order;
      /* t = 13 */
      ASN1T_MsgPopulationOrder *msg_population_order;
      /* t = 14 */
      ASN1T_MsgFragOrder *msg_frag_order;
      /* t = 15 */
      ASN1T_MsgSetAutomatMode *msg_set_automat_mode;
      /* t = 16 */
      ASN1T_MsgUnitCreationRequest *msg_unit_creation_request;
      /* t = 17 */
      ASN1T_MsgUnitMagicAction *msg_unit_magic_action;
      /* t = 18 */
      ASN1T_MsgObjectMagicAction *msg_object_magic_action;
      /* t = 19 */
      ASN1T_MsgPopulationMagicAction *msg_population_magic_action;
      /* t = 20 */
      ASN1T_MsgChangeDiplomacy *msg_change_diplomacy;
      /* t = 21 */
      ASN1T_MsgAutomatChangeKnowledgeGroup *msg_automat_change_knowledge_group;
      /* t = 22 */
      ASN1T_MsgAutomatChangeLogisticLinks *msg_automat_change_logistic_links;
      /* t = 23 */
      ASN1T_MsgAutomatChangeSuperior *msg_automat_change_superior;
      /* t = 24 */
      ASN1T_MsgUnitChangeSuperior *msg_unit_change_superior;
      /* t = 25 */
      ASN1T_MsgLogSupplyPushFlow *msg_log_supply_push_flow;
      /* t = 26 */
      ASN1T_MsgLogSupplyChangeQuotas *msg_log_supply_change_quotas;
      /* t = 27 */
      ASN1T_MsgKnowledgeGroupChangeSuperior *msg_knowledge_group_change_superior;
      /* t = 28 */
      ASN1T_MsgKnowledgeGroupCreation *msg_knowledge_group_creation;
      /* t = 29 */
      ASN1T_MsgKnowledgeGroupDelete *msg_knowledge_group_delete;
      /* t = 30 */
      ASN1T_MsgKnowledgeGroupSetType *msg_knowledge_group_set_type;
      /* t = 31 */
      ASN1T_MsgKnowledgeGroupEnable *msg_knowledge_group_enable;
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

#endif

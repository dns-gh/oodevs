/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 31-May-2007.
 */
#ifndef MESSAGES_H
#define MESSAGES_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "Base.h"
#include "LimaLimite.h"
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
#include "Automate.h"
#include "Army.h"
#include "Auth.h"

/**************************************************************/
/*                                                            */
/*  MessagesIncludes                                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MessagesIncludes {
} ASN1T_MessagesIncludes;

class EXTERN ASN1C_MessagesIncludes : public ASN1CType {
public:
   ASN1T_MessagesIncludes& msgData;
   ASN1C_MessagesIncludes (
      ASN1MessageBuffer& msgBuf, ASN1T_MessagesIncludes& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MessagesIncludes (ASN1CTXT* ctxt_p, ASN1T_MessagesIncludes* pvalue);
EXTERN int asn1PD_MessagesIncludes (ASN1CTXT* ctxt_p, ASN1T_MessagesIncludes* pvalue);

EXTERN void asn1Print_MessagesIncludes (ASN1ConstCharPtr name, ASN1T_MessagesIncludes* pvalue);

/**************************************************************/
/*                                                            */
/*  MessagesExports                                           */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MessagesExports : public ASN1CType {
public:
   ASN1C_MessagesExports (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MessagesExports (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MessagesExports (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MessagesExports (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgsSimToClient_msg                                       */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_MsgsSimToClient_msg_msg_limit_creation_request_ack 1
#define T_MsgsSimToClient_msg_msg_limit_destruction_request_ack 2
#define T_MsgsSimToClient_msg_msg_limit_update_request_ack 3
#define T_MsgsSimToClient_msg_msg_lima_creation_request_ack 4
#define T_MsgsSimToClient_msg_msg_lima_destruction_request_ack 5
#define T_MsgsSimToClient_msg_msg_lima_update_request_ack 6
#define T_MsgsSimToClient_msg_msg_pion_order_ack 7
#define T_MsgsSimToClient_msg_msg_automate_order_ack 8
#define T_MsgsSimToClient_msg_msg_population_order_ack 9
#define T_MsgsSimToClient_msg_msg_frag_order_ack 10
#define T_MsgsSimToClient_msg_msg_set_automate_mode_ack 11
#define T_MsgsSimToClient_msg_msg_unit_magic_action_ack 12
#define T_MsgsSimToClient_msg_msg_object_magic_action_ack 13
#define T_MsgsSimToClient_msg_msg_population_magic_action_ack 14
#define T_MsgsSimToClient_msg_msg_change_diplomatie_ack 15
#define T_MsgsSimToClient_msg_msg_automate_change_groupe_connaissance_ack 16
#define T_MsgsSimToClient_msg_msg_automate_change_liens_logistiques_ack 17
#define T_MsgsSimToClient_msg_msg_pion_change_superior_ack 18
#define T_MsgsSimToClient_msg_msg_log_ravitaillement_pousser_flux_ack 19
#define T_MsgsSimToClient_msg_msg_log_ravitaillement_change_quotas_ack 20
#define T_MsgsSimToClient_msg_msg_ctrl_info 21
#define T_MsgsSimToClient_msg_msg_ctrl_begin_tick 22
#define T_MsgsSimToClient_msg_msg_ctrl_end_tick 23
#define T_MsgsSimToClient_msg_msg_ctrl_stop_ack 24
#define T_MsgsSimToClient_msg_msg_ctrl_pause_ack 25
#define T_MsgsSimToClient_msg_msg_ctrl_resume_ack 26
#define T_MsgsSimToClient_msg_msg_ctrl_change_time_factor_ack 27
#define T_MsgsSimToClient_msg_msg_ctrl_meteo_globale_ack 28
#define T_MsgsSimToClient_msg_msg_ctrl_meteo_locale_ack 29
#define T_MsgsSimToClient_msg_msg_ctrl_checkpoint_save_begin 30
#define T_MsgsSimToClient_msg_msg_ctrl_checkpoint_save_end 31
#define T_MsgsSimToClient_msg_msg_ctrl_checkpoint_set_frequency_ack 32
#define T_MsgsSimToClient_msg_msg_ctrl_checkpoint_save_now_ack 33
#define T_MsgsSimToClient_msg_msg_ctrl_send_current_state_begin 34
#define T_MsgsSimToClient_msg_msg_ctrl_send_current_state_end 35
#define T_MsgsSimToClient_msg_msg_limit_creation 36
#define T_MsgsSimToClient_msg_msg_limit_update 37
#define T_MsgsSimToClient_msg_msg_limit_destruction 38
#define T_MsgsSimToClient_msg_msg_lima_creation 39
#define T_MsgsSimToClient_msg_msg_lima_update 40
#define T_MsgsSimToClient_msg_msg_lima_destruction 41
#define T_MsgsSimToClient_msg_msg_formation_creation 42
#define T_MsgsSimToClient_msg_msg_knowledge_group_creation 43
#define T_MsgsSimToClient_msg_msg_side_creation 44
#define T_MsgsSimToClient_msg_msg_automate_creation 45
#define T_MsgsSimToClient_msg_msg_automate_attributes 46
#define T_MsgsSimToClient_msg_msg_pion_creation 47
#define T_MsgsSimToClient_msg_msg_unit_attributes 48
#define T_MsgsSimToClient_msg_msg_unit_pathfind 49
#define T_MsgsSimToClient_msg_msg_pion_destruction 50
#define T_MsgsSimToClient_msg_msg_change_diplomatie 51
#define T_MsgsSimToClient_msg_msg_pion_change_superior 52
#define T_MsgsSimToClient_msg_msg_automate_change_liens_logistiques 53
#define T_MsgsSimToClient_msg_msg_unit_knowledge_creation 54
#define T_MsgsSimToClient_msg_msg_unit_knowledge_update 55
#define T_MsgsSimToClient_msg_msg_unit_knowledge_destruction 56
#define T_MsgsSimToClient_msg_msg_start_pion_fire 57
#define T_MsgsSimToClient_msg_msg_stop_pion_fire 58
#define T_MsgsSimToClient_msg_msg_start_population_fire 59
#define T_MsgsSimToClient_msg_msg_stop_population_fire 60
#define T_MsgsSimToClient_msg_msg_explosion 61
#define T_MsgsSimToClient_msg_msg_start_fire_effect 62
#define T_MsgsSimToClient_msg_msg_stop_fire_effect 63
#define T_MsgsSimToClient_msg_msg_cr    64
#define T_MsgsSimToClient_msg_msg_trace 65
#define T_MsgsSimToClient_msg_msg_decisional_state 66
#define T_MsgsSimToClient_msg_msg_pion_order 67
#define T_MsgsSimToClient_msg_msg_automate_order 68
#define T_MsgsSimToClient_msg_msg_population_order 69
#define T_MsgsSimToClient_msg_msg_object_creation 70
#define T_MsgsSimToClient_msg_msg_object_destruction 71
#define T_MsgsSimToClient_msg_msg_object_update 72
#define T_MsgsSimToClient_msg_msg_object_knowledge_creation 73
#define T_MsgsSimToClient_msg_msg_object_knowledge_update 74
#define T_MsgsSimToClient_msg_msg_object_knowledge_destruction 75
#define T_MsgsSimToClient_msg_msg_log_sante_traitement_humain_creation 76
#define T_MsgsSimToClient_msg_msg_log_sante_traitement_humain_update 77
#define T_MsgsSimToClient_msg_msg_log_sante_traitement_humain_destruction 78
#define T_MsgsSimToClient_msg_msg_log_sante_etat 79
#define T_MsgsSimToClient_msg_msg_log_maintenance_traitement_equipement_creation 80
#define T_MsgsSimToClient_msg_msg_log_maintenance_traitement_equipement_update 81
#define T_MsgsSimToClient_msg_msg_log_maintenance_traitement_equipement_destruction 82
#define T_MsgsSimToClient_msg_msg_log_maintenance_etat 83
#define T_MsgsSimToClient_msg_msg_log_ravitaillement_traitement_creation 84
#define T_MsgsSimToClient_msg_msg_log_ravitaillement_traitement_update 85
#define T_MsgsSimToClient_msg_msg_log_ravitaillement_traitement_destruction 86
#define T_MsgsSimToClient_msg_msg_log_ravitaillement_etat 87
#define T_MsgsSimToClient_msg_msg_log_ravitaillement_quotas 88
#define T_MsgsSimToClient_msg_msg_population_creation 89
#define T_MsgsSimToClient_msg_msg_population_update 90
#define T_MsgsSimToClient_msg_msg_population_concentration_creation 91
#define T_MsgsSimToClient_msg_msg_population_concentration_destruction 92
#define T_MsgsSimToClient_msg_msg_population_concentration_update 93
#define T_MsgsSimToClient_msg_msg_population_flux_creation 94
#define T_MsgsSimToClient_msg_msg_population_flux_destruction 95
#define T_MsgsSimToClient_msg_msg_population_flux_update 96
#define T_MsgsSimToClient_msg_msg_population_knowledge_creation 97
#define T_MsgsSimToClient_msg_msg_population_knowledge_update 98
#define T_MsgsSimToClient_msg_msg_population_knowledge_destruction 99
#define T_MsgsSimToClient_msg_msg_population_concentration_knowledge_creation 100
#define T_MsgsSimToClient_msg_msg_population_concentration_knowledge_destruction 101
#define T_MsgsSimToClient_msg_msg_population_concentration_knowledge_update 102
#define T_MsgsSimToClient_msg_msg_population_flux_knowledge_creation 103
#define T_MsgsSimToClient_msg_msg_population_flux_knowledge_destruction 104
#define T_MsgsSimToClient_msg_msg_population_flux_knowledge_update 105

typedef struct EXTERN ASN1T_MsgsSimToClient_msg {
   int t;
   union {
      /* t = 1 */
      ASN1T_MsgLimitCreationRequestAck  msg_limit_creation_request_ack;
      /* t = 2 */
      ASN1T_MsgLimitDestructionRequestAck  msg_limit_destruction_request_ack;
      /* t = 3 */
      ASN1T_MsgLimitUpdateRequestAck  msg_limit_update_request_ack;
      /* t = 4 */
      ASN1T_MsgLimaCreationRequestAck  msg_lima_creation_request_ack;
      /* t = 5 */
      ASN1T_MsgLimaDestructionRequestAck  msg_lima_destruction_request_ack;
      /* t = 6 */
      ASN1T_MsgLimaUpdateRequestAck  msg_lima_update_request_ack;
      /* t = 7 */
      ASN1T_MsgPionOrderAck *msg_pion_order_ack;
      /* t = 8 */
      ASN1T_MsgAutomateOrderAck *msg_automate_order_ack;
      /* t = 9 */
      ASN1T_MsgPopulationOrderAck *msg_population_order_ack;
      /* t = 10 */
      ASN1T_MsgFragOrderAck *msg_frag_order_ack;
      /* t = 11 */
      ASN1T_MsgSetAutomateModeAck *msg_set_automate_mode_ack;
      /* t = 12 */
      ASN1T_MsgUnitMagicActionAck *msg_unit_magic_action_ack;
      /* t = 13 */
      ASN1T_MsgObjectMagicActionAck *msg_object_magic_action_ack;
      /* t = 14 */
      ASN1T_MsgPopulationMagicActionAck *msg_population_magic_action_ack;
      /* t = 15 */
      ASN1T_MsgChangeDiplomatieAck *msg_change_diplomatie_ack;
      /* t = 16 */
      ASN1T_MsgAutomateChangeGroupeConnaissanceAck *msg_automate_change_groupe_connaissance_ack;
      /* t = 17 */
      ASN1T_MsgAutomateChangeLiensLogistiquesAck *msg_automate_change_liens_logistiques_ack;
      /* t = 18 */
      ASN1T_MsgPionChangeSuperiorAck *msg_pion_change_superior_ack;
      /* t = 19 */
      ASN1T_MsgLogRavitaillementPousserFluxAck  msg_log_ravitaillement_pousser_flux_ack;
      /* t = 20 */
      ASN1T_MsgLogRavitaillementChangeQuotasAck  msg_log_ravitaillement_change_quotas_ack;
      /* t = 21 */
      ASN1T_MsgCtrlInfo *msg_ctrl_info;
      /* t = 22 */
      ASN1T_MsgCtrlBeginTick  msg_ctrl_begin_tick;
      /* t = 23 */
      ASN1T_MsgCtrlEndTick *msg_ctrl_end_tick;
      /* t = 24 */
      ASN1T_MsgCtrlStopAck  msg_ctrl_stop_ack;
      /* t = 25 */
      ASN1T_MsgCtrlPauseAck  msg_ctrl_pause_ack;
      /* t = 26 */
      ASN1T_MsgCtrlResumeAck  msg_ctrl_resume_ack;
      /* t = 27 */
      ASN1T_MsgCtrlChangeTimeFactorAck *msg_ctrl_change_time_factor_ack;
      /* t = 28 */
      /* t = 29 */
      /* t = 30 */
      /* t = 31 */
      /* t = 32 */
      /* t = 33 */
      /* t = 34 */
      /* t = 35 */
      /* t = 36 */
      ASN1T_MsgLimitCreation *msg_limit_creation;
      /* t = 37 */
      ASN1T_MsgLimitUpdate *msg_limit_update;
      /* t = 38 */
      ASN1T_MsgLimitDestruction  msg_limit_destruction;
      /* t = 39 */
      ASN1T_MsgLimaCreation *msg_lima_creation;
      /* t = 40 */
      ASN1T_MsgLimaUpdate *msg_lima_update;
      /* t = 41 */
      ASN1T_MsgLimaDestruction  msg_lima_destruction;
      /* t = 42 */
      ASN1T_MsgFormationCreation *msg_formation_creation;
      /* t = 43 */
      ASN1T_MsgKnowledgeGroupCreation *msg_knowledge_group_creation;
      /* t = 44 */
      ASN1T_MsgSideCreation *msg_side_creation;
      /* t = 45 */
      ASN1T_MsgAutomateCreation *msg_automate_creation;
      /* t = 46 */
      ASN1T_MsgAutomateAttributes *msg_automate_attributes;
      /* t = 47 */
      ASN1T_MsgPionCreation *msg_pion_creation;
      /* t = 48 */
      ASN1T_MsgUnitAttributes *msg_unit_attributes;
      /* t = 49 */
      ASN1T_MsgUnitPathFind *msg_unit_pathfind;
      /* t = 50 */
      ASN1T_MsgPionDestruction  msg_pion_destruction;
      /* t = 51 */
      ASN1T_MsgChangeDiplomatie *msg_change_diplomatie;
      /* t = 52 */
      ASN1T_MsgPionChangeSuperior *msg_pion_change_superior;
      /* t = 53 */
      ASN1T_MsgAutomateChangeLiensLogistiques *msg_automate_change_liens_logistiques;
      /* t = 54 */
      ASN1T_MsgUnitKnowledgeCreation *msg_unit_knowledge_creation;
      /* t = 55 */
      ASN1T_MsgUnitKnowledgeUpdate *msg_unit_knowledge_update;
      /* t = 56 */
      ASN1T_MsgUnitKnowledgeDestruction *msg_unit_knowledge_destruction;
      /* t = 57 */
      ASN1T_MsgStartPionFire *msg_start_pion_fire;
      /* t = 58 */
      ASN1T_MsgStopPionFire *msg_stop_pion_fire;
      /* t = 59 */
      ASN1T_MsgStartPopulationFire *msg_start_population_fire;
      /* t = 60 */
      ASN1T_MsgStopPopulationFire *msg_stop_population_fire;
      /* t = 61 */
      ASN1T_MsgExplosion *msg_explosion;
      /* t = 62 */
      ASN1T_MsgStartFireEffect *msg_start_fire_effect;
      /* t = 63 */
      ASN1T_MsgStopFireEffect  msg_stop_fire_effect;
      /* t = 64 */
      ASN1T_MsgCR *msg_cr;
      /* t = 65 */
      ASN1T_MsgTrace *msg_trace;
      /* t = 66 */
      ASN1T_MsgDecisionalState *msg_decisional_state;
      /* t = 67 */
      ASN1T_MsgPionOrder *msg_pion_order;
      /* t = 68 */
      ASN1T_MsgAutomateOrder *msg_automate_order;
      /* t = 69 */
      ASN1T_MsgPopulationOrder *msg_population_order;
      /* t = 70 */
      ASN1T_MsgObjectCreation *msg_object_creation;
      /* t = 71 */
      ASN1T_MsgObjectDestruction  msg_object_destruction;
      /* t = 72 */
      ASN1T_MsgObjectUpdate *msg_object_update;
      /* t = 73 */
      ASN1T_MsgObjectKnowledgeCreation *msg_object_knowledge_creation;
      /* t = 74 */
      ASN1T_MsgObjectKnowledgeUpdate *msg_object_knowledge_update;
      /* t = 75 */
      ASN1T_MsgObjectKnowledgeDestruction *msg_object_knowledge_destruction;
      /* t = 76 */
      ASN1T_MsgLogSanteTraitementHumainCreation *msg_log_sante_traitement_humain_creation;
      /* t = 77 */
      ASN1T_MsgLogSanteTraitementHumainUpdate *msg_log_sante_traitement_humain_update;
      /* t = 78 */
      ASN1T_MsgLogSanteTraitementHumainDestruction *msg_log_sante_traitement_humain_destruction;
      /* t = 79 */
      ASN1T_MsgLogSanteEtat *msg_log_sante_etat;
      /* t = 80 */
      ASN1T_MsgLogMaintenanceTraitementEquipementCreation *msg_log_maintenance_traitement_equipement_creation;
      /* t = 81 */
      ASN1T_MsgLogMaintenanceTraitementEquipementUpdate *msg_log_maintenance_traitement_equipement_update;
      /* t = 82 */
      ASN1T_MsgLogMaintenanceTraitementEquipementDestruction *msg_log_maintenance_traitement_equipement_destruction;
      /* t = 83 */
      ASN1T_MsgLogMaintenanceEtat *msg_log_maintenance_etat;
      /* t = 84 */
      ASN1T_MsgLogRavitaillementTraitementCreation *msg_log_ravitaillement_traitement_creation;
      /* t = 85 */
      ASN1T_MsgLogRavitaillementTraitementUpdate *msg_log_ravitaillement_traitement_update;
      /* t = 86 */
      ASN1T_MsgLogRavitaillementTraitementDestruction *msg_log_ravitaillement_traitement_destruction;
      /* t = 87 */
      ASN1T_MsgLogRavitaillementEtat *msg_log_ravitaillement_etat;
      /* t = 88 */
      ASN1T_MsgLogRavitaillementQuotas *msg_log_ravitaillement_quotas;
      /* t = 89 */
      ASN1T_MsgPopulationCreation *msg_population_creation;
      /* t = 90 */
      ASN1T_MsgPopulationUpdate *msg_population_update;
      /* t = 91 */
      ASN1T_MsgPopulationConcentrationCreation *msg_population_concentration_creation;
      /* t = 92 */
      ASN1T_MsgPopulationConcentrationDestruction *msg_population_concentration_destruction;
      /* t = 93 */
      ASN1T_MsgPopulationConcentrationUpdate *msg_population_concentration_update;
      /* t = 94 */
      ASN1T_MsgPopulationFluxCreation *msg_population_flux_creation;
      /* t = 95 */
      ASN1T_MsgPopulationFluxDestruction *msg_population_flux_destruction;
      /* t = 96 */
      ASN1T_MsgPopulationFluxUpdate *msg_population_flux_update;
      /* t = 97 */
      ASN1T_MsgPopulationKnowledgeCreation *msg_population_knowledge_creation;
      /* t = 98 */
      ASN1T_MsgPopulationKnowledgeUpdate *msg_population_knowledge_update;
      /* t = 99 */
      ASN1T_MsgPopulationKnowledgeDestruction *msg_population_knowledge_destruction;
      /* t = 100 */
      ASN1T_MsgPopulationConcentrationKnowledgeCreation *msg_population_concentration_knowledge_creation;
      /* t = 101 */
      ASN1T_MsgPopulationConcentrationKnowledgeDestruction *msg_population_concentration_knowledge_destruction;
      /* t = 102 */
      ASN1T_MsgPopulationConcentrationKnowledgeUpdate *msg_population_concentration_knowledge_update;
      /* t = 103 */
      ASN1T_MsgPopulationFluxKnowledgeCreation *msg_population_flux_knowledge_creation;
      /* t = 104 */
      ASN1T_MsgPopulationFluxKnowledgeDestruction *msg_population_flux_knowledge_destruction;
      /* t = 105 */
      ASN1T_MsgPopulationFluxKnowledgeUpdate *msg_population_flux_knowledge_update;
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

#define T_MsgsClientToSim_msg_msg_ctrl_stop 1
#define T_MsgsClientToSim_msg_msg_ctrl_pause 2
#define T_MsgsClientToSim_msg_msg_ctrl_resume 3
#define T_MsgsClientToSim_msg_msg_ctrl_change_time_factor 4
#define T_MsgsClientToSim_msg_msg_ctrl_meteo_globale 5
#define T_MsgsClientToSim_msg_msg_ctrl_meteo_locale 6
#define T_MsgsClientToSim_msg_msg_ctrl_checkpoint_save_now 7
#define T_MsgsClientToSim_msg_msg_ctrl_checkpoint_set_frequency 8
#define T_MsgsClientToSim_msg_msg_limit_creation_request 9
#define T_MsgsClientToSim_msg_msg_limit_destruction_request 10
#define T_MsgsClientToSim_msg_msg_limit_update_request 11
#define T_MsgsClientToSim_msg_msg_lima_creation_request 12
#define T_MsgsClientToSim_msg_msg_lima_destruction_request 13
#define T_MsgsClientToSim_msg_msg_lima_update_request 14
#define T_MsgsClientToSim_msg_msg_pion_order 15
#define T_MsgsClientToSim_msg_msg_automate_order 16
#define T_MsgsClientToSim_msg_msg_population_order 17
#define T_MsgsClientToSim_msg_msg_frag_order 18
#define T_MsgsClientToSim_msg_msg_set_automate_mode 19
#define T_MsgsClientToSim_msg_msg_unit_magic_action 20
#define T_MsgsClientToSim_msg_msg_object_magic_action 21
#define T_MsgsClientToSim_msg_msg_population_magic_action 22
#define T_MsgsClientToSim_msg_msg_change_diplomatie 23
#define T_MsgsClientToSim_msg_msg_automate_change_groupe_connaissance 24
#define T_MsgsClientToSim_msg_msg_automate_change_liens_logistiques 25
#define T_MsgsClientToSim_msg_msg_pion_change_superior 26
#define T_MsgsClientToSim_msg_msg_log_ravitaillement_pousser_flux 27
#define T_MsgsClientToSim_msg_msg_log_ravitaillement_change_quotas 28

typedef struct EXTERN ASN1T_MsgsClientToSim_msg {
   int t;
   union {
      /* t = 1 */
      /* t = 2 */
      /* t = 3 */
      /* t = 4 */
      ASN1T_MsgCtrlChangeTimeFactor  msg_ctrl_change_time_factor;
      /* t = 5 */
      ASN1T_MsgCtrlMeteoGlobale *msg_ctrl_meteo_globale;
      /* t = 6 */
      ASN1T_MsgCtrlMeteoLocale *msg_ctrl_meteo_locale;
      /* t = 7 */
      ASN1T_MsgCtrlCheckPointSaveNow *msg_ctrl_checkpoint_save_now;
      /* t = 8 */
      ASN1T_MsgCtrlCheckPointSetFrequency  msg_ctrl_checkpoint_set_frequency;
      /* t = 9 */
      ASN1T_MsgLimitCreationRequest *msg_limit_creation_request;
      /* t = 10 */
      ASN1T_MsgLimitDestructionRequest  msg_limit_destruction_request;
      /* t = 11 */
      ASN1T_MsgLimitUpdateRequest *msg_limit_update_request;
      /* t = 12 */
      ASN1T_MsgLimaCreationRequest *msg_lima_creation_request;
      /* t = 13 */
      ASN1T_MsgLimaDestruction  msg_lima_destruction_request;
      /* t = 14 */
      ASN1T_MsgLimaUpdateRequest *msg_lima_update_request;
      /* t = 15 */
      ASN1T_MsgPionOrder *msg_pion_order;
      /* t = 16 */
      ASN1T_MsgAutomateOrder *msg_automate_order;
      /* t = 17 */
      ASN1T_MsgPopulationOrder *msg_population_order;
      /* t = 18 */
      ASN1T_MsgFragOrder *msg_frag_order;
      /* t = 19 */
      ASN1T_MsgSetAutomateMode *msg_set_automate_mode;
      /* t = 20 */
      ASN1T_MsgUnitMagicAction *msg_unit_magic_action;
      /* t = 21 */
      ASN1T_MsgObjectMagicAction *msg_object_magic_action;
      /* t = 22 */
      ASN1T_MsgPopulationMagicAction *msg_population_magic_action;
      /* t = 23 */
      ASN1T_MsgChangeDiplomatie *msg_change_diplomatie;
      /* t = 24 */
      ASN1T_MsgAutomateChangeGroupeConnaissance *msg_automate_change_groupe_connaissance;
      /* t = 25 */
      ASN1T_MsgAutomateChangeLiensLogistiques *msg_automate_change_liens_logistiques;
      /* t = 26 */
      ASN1T_MsgPionChangeSuperior *msg_pion_change_superior;
      /* t = 27 */
      ASN1T_MsgLogRavitaillementPousserFlux *msg_log_ravitaillement_pousser_flux;
      /* t = 28 */
      ASN1T_MsgLogRavitaillementChangeQuotas *msg_log_ravitaillement_change_quotas;
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
/*  MsgsSimToMiddle                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgsSimToMiddle {
   ASN1INT   context;

   ASN1T_MsgsSimToMiddle () {
      context = 0;
   }
} ASN1T_MsgsSimToMiddle;

class EXTERN ASN1C_MsgsSimToMiddle : public ASN1CType {
public:
   ASN1T_MsgsSimToMiddle& msgData;
   ASN1C_MsgsSimToMiddle (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgsSimToMiddle& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgsSimToMiddle (ASN1CTXT* ctxt_p, ASN1T_MsgsSimToMiddle* pvalue);
EXTERN int asn1PD_MsgsSimToMiddle (ASN1CTXT* ctxt_p, ASN1T_MsgsSimToMiddle* pvalue);

EXTERN void asn1Print_MsgsSimToMiddle (ASN1ConstCharPtr name, ASN1T_MsgsSimToMiddle* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgsMiddleToSim_msg                                       */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_MsgsMiddleToSim_msg_msg_ctrl_client_announcement 1

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

/**************************************************************/
/*                                                            */
/*  MsgsMiddleToClient_msg                                    */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_MsgsMiddleToClient_msg_msg_ctrl_replay_info 1
#define T_MsgsMiddleToClient_msg_msg_ctrl_skip_to_tick_ack 2
#define T_MsgsMiddleToClient_msg_msg_authentication_response 3
#define T_MsgsMiddleToClient_msg_msg_profile_creation 4
#define T_MsgsMiddleToClient_msg_msg_profile_creation_request_ack 5
#define T_MsgsMiddleToClient_msg_msg_profile_update 6
#define T_MsgsMiddleToClient_msg_msg_profile_update_request_ack 7
#define T_MsgsMiddleToClient_msg_msg_profile_destruction 8
#define T_MsgsMiddleToClient_msg_msg_profile_destruction_request_ack 9

typedef struct EXTERN ASN1T_MsgsMiddleToClient_msg {
   int t;
   union {
      /* t = 1 */
      ASN1T_MsgCtrlReplayInfo *msg_ctrl_replay_info;
      /* t = 2 */
      ASN1T_MsgCtrlSkipToTickAck *msg_ctrl_skip_to_tick_ack;
      /* t = 3 */
      ASN1T_MsgAuthenticationResponse *msg_authentication_response;
      /* t = 4 */
      ASN1T_MsgProfileCreation *msg_profile_creation;
      /* t = 5 */
      ASN1T_MsgProfileCreationRequestAck *msg_profile_creation_request_ack;
      /* t = 6 */
      ASN1T_MsgProfileUpdate *msg_profile_update;
      /* t = 7 */
      ASN1T_MsgProfileUpdateRequestAck *msg_profile_update_request_ack;
      /* t = 8 */
      ASN1T_MsgProfileDestruction  msg_profile_destruction;
      /* t = 9 */
      ASN1T_MsgProfileDestructionRequestAck *msg_profile_destruction_request_ack;
   } u;
} ASN1T_MsgsMiddleToClient_msg;

class EXTERN ASN1C_MsgsMiddleToClient_msg : public ASN1CType {
public:
   ASN1T_MsgsMiddleToClient_msg& msgData;
   ASN1C_MsgsMiddleToClient_msg (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgsMiddleToClient_msg& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgsMiddleToClient_msg (ASN1CTXT* ctxt_p, ASN1T_MsgsMiddleToClient_msg* pvalue);
EXTERN int asn1PD_MsgsMiddleToClient_msg (ASN1CTXT* ctxt_p, ASN1T_MsgsMiddleToClient_msg* pvalue);

EXTERN void asn1Print_MsgsMiddleToClient_msg (ASN1ConstCharPtr name, ASN1T_MsgsMiddleToClient_msg* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgsMiddleToClient                                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgsMiddleToClient {
   ASN1INT   context;
   ASN1T_MsgsMiddleToClient_msg  msg;

   ASN1T_MsgsMiddleToClient () {
      context = 0;
   }
} ASN1T_MsgsMiddleToClient;

class EXTERN ASN1C_MsgsMiddleToClient : public ASN1CType {
public:
   ASN1T_MsgsMiddleToClient& msgData;
   ASN1C_MsgsMiddleToClient (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgsMiddleToClient& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgsMiddleToClient (ASN1CTXT* ctxt_p, ASN1T_MsgsMiddleToClient* pvalue);
EXTERN int asn1PD_MsgsMiddleToClient (ASN1CTXT* ctxt_p, ASN1T_MsgsMiddleToClient* pvalue);

EXTERN void asn1Print_MsgsMiddleToClient (ASN1ConstCharPtr name, ASN1T_MsgsMiddleToClient* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgsClientToMiddle_msg                                    */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_MsgsClientToMiddle_msg_msg_ctrl_skip_to_tick 1
#define T_MsgsClientToMiddle_msg_msg_authentication_request 2
#define T_MsgsClientToMiddle_msg_msg_profile_creation_request 3
#define T_MsgsClientToMiddle_msg_msg_profile_update_request 4
#define T_MsgsClientToMiddle_msg_msg_profile_destruction_request 5

typedef struct EXTERN ASN1T_MsgsClientToMiddle_msg {
   int t;
   union {
      /* t = 1 */
      ASN1T_MsgCtrlSkipToTick  msg_ctrl_skip_to_tick;
      /* t = 2 */
      ASN1T_MsgAuthenticationRequest *msg_authentication_request;
      /* t = 3 */
      ASN1T_MsgProfileCreationRequest *msg_profile_creation_request;
      /* t = 4 */
      ASN1T_MsgProfileUpdateRequest *msg_profile_update_request;
      /* t = 5 */
      ASN1T_MsgProfileDestructionRequest  msg_profile_destruction_request;
   } u;
} ASN1T_MsgsClientToMiddle_msg;

class EXTERN ASN1C_MsgsClientToMiddle_msg : public ASN1CType {
public:
   ASN1T_MsgsClientToMiddle_msg& msgData;
   ASN1C_MsgsClientToMiddle_msg (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgsClientToMiddle_msg& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgsClientToMiddle_msg (ASN1CTXT* ctxt_p, ASN1T_MsgsClientToMiddle_msg* pvalue);
EXTERN int asn1PD_MsgsClientToMiddle_msg (ASN1CTXT* ctxt_p, ASN1T_MsgsClientToMiddle_msg* pvalue);

EXTERN void asn1Print_MsgsClientToMiddle_msg (ASN1ConstCharPtr name, ASN1T_MsgsClientToMiddle_msg* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgsClientToMiddle                                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgsClientToMiddle {
   ASN1INT   context;
   ASN1T_MsgsClientToMiddle_msg  msg;

   ASN1T_MsgsClientToMiddle () {
      context = 0;
   }
} ASN1T_MsgsClientToMiddle;

class EXTERN ASN1C_MsgsClientToMiddle : public ASN1CType {
public:
   ASN1T_MsgsClientToMiddle& msgData;
   ASN1C_MsgsClientToMiddle (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgsClientToMiddle& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgsClientToMiddle (ASN1CTXT* ctxt_p, ASN1T_MsgsClientToMiddle* pvalue);
EXTERN int asn1PD_MsgsClientToMiddle (ASN1CTXT* ctxt_p, ASN1T_MsgsClientToMiddle* pvalue);

EXTERN void asn1Print_MsgsClientToMiddle (ASN1ConstCharPtr name, ASN1T_MsgsClientToMiddle* pvalue);

#endif

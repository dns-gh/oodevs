/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 17-Aug-2005.
 */
#ifndef SCIPIOMESSAGES_H
#define SCIPIOMESSAGES_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioControl.h"
#include "ScipioLimaLimite.h"
#include "ScipioUnitKnowledge.h"
#include "ScipioFire.h"
#include "ScipioUnitAttr.h"
#include "ScipioPionOrder.h"
#include "ScipioAutomateOrder.h"
#include "ScipioOrder.h"
#include "ScipioOrderConduite.h"
#include "ScipioLauncher.h"
#include "ScipioCR.h"
#include "ScipioObjectKnowledge.h"
#include "ScipioObject.h"
#include "ScipioHierarchie.h"
#include "ScipioLogSante.h"
#include "ScipioLogMaintenance.h"
#include "ScipioLogRavitaillement.h"

/**************************************************************/
/*                                                            */
/*  NbChoice                                                  */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_NbChoice : public ASN1CType {
public:
   ASN1C_NbChoice (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_NbChoice (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_NbChoice (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_NbChoice (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgsSimMos                                                */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_MsgsSimMos_msg_ctrl_info      1
#define T_MsgsSimMos_msg_ctrl_begin_tick 2
#define T_MsgsSimMos_msg_ctrl_end_tick  3
#define T_MsgsSimMos_msg_ctrl_stop_ack  4
#define T_MsgsSimMos_msg_ctrl_pause_ack 5
#define T_MsgsSimMos_msg_ctrl_resume_ack 6
#define T_MsgsSimMos_msg_ctrl_change_time_factor_ack 7
#define T_MsgsSimMos_msg_ctrl_meteo_globale_ack 8
#define T_MsgsSimMos_msg_ctrl_meteo_locale_ack 9
#define T_MsgsSimMos_msg_ctrl_checkpoint_save_begin 10
#define T_MsgsSimMos_msg_ctrl_checkpoint_save_end 11
#define T_MsgsSimMos_msg_ctrl_checkpoint_load_begin 12
#define T_MsgsSimMos_msg_ctrl_checkpoint_load_end 13
#define T_MsgsSimMos_msg_ctrl_checkpoint_set_frequency_ack 14
#define T_MsgsSimMos_msg_ctrl_checkpoint_save_now_ack 15
#define T_MsgsSimMos_msg_ctrl_send_current_state_begin 16
#define T_MsgsSimMos_msg_ctrl_send_current_state_end 17
#define T_MsgsSimMos_msg_limit_creation 18
#define T_MsgsSimMos_msg_limit_destruction 19
#define T_MsgsSimMos_msg_lima_creation  20
#define T_MsgsSimMos_msg_lima_destruction 21
#define T_MsgsSimMos_msg_automate_creation 22
#define T_MsgsSimMos_msg_pion_creation  23
#define T_MsgsSimMos_msg_change_diplomatie 24
#define T_MsgsSimMos_msg_unit_knowledge_creation 25
#define T_MsgsSimMos_msg_unit_knowledge_update 26
#define T_MsgsSimMos_msg_unit_knowledge_destruction 27
#define T_MsgsSimMos_msg_unit_attributes 28
#define T_MsgsSimMos_msg_unit_dotations 29
#define T_MsgsSimMos_msg_unit_pathfind  30
#define T_MsgsSimMos_msg_start_direct_fire 31
#define T_MsgsSimMos_msg_stop_direct_fire 32
#define T_MsgsSimMos_msg_start_indirect_fire 33
#define T_MsgsSimMos_msg_stop_indirect_fire 34
#define T_MsgsSimMos_msg_explosion      35
#define T_MsgsSimMos_msg_start_fire_effect 36
#define T_MsgsSimMos_msg_stop_fire_effect 37
#define T_MsgsSimMos_msg_cr             38
#define T_MsgsSimMos_msg_order_management 39
#define T_MsgsSimMos_msg_attente_ordre_conduite 40
#define T_MsgsSimMos_msg_annule_attente_ordre_conduite 41
#define T_MsgsSimMos_msg_automate_mrt   42
#define T_MsgsSimMos_msg_pion_order     43
#define T_MsgsSimMos_msg_automate_order 44
#define T_MsgsSimMos_msg_object_creation 45
#define T_MsgsSimMos_msg_object_destruction 46
#define T_MsgsSimMos_msg_object_update  47
#define T_MsgsSimMos_msg_object_knowledge_creation 48
#define T_MsgsSimMos_msg_object_knowledge_update 49
#define T_MsgsSimMos_msg_object_knowledge_destruction 50
#define T_MsgsSimMos_msg_change_automate 51
#define T_MsgsSimMos_msg_log_sante_traitement_humain_creation 52
#define T_MsgsSimMos_msg_log_sante_traitement_humain_update 53
#define T_MsgsSimMos_msg_log_sante_traitement_humain_destruction 54
#define T_MsgsSimMos_msg_log_sante_etat 55
#define T_MsgsSimMos_msg_log_maintenance_traitement_equipement_creation 56
#define T_MsgsSimMos_msg_log_maintenance_traitement_equipement_update 57
#define T_MsgsSimMos_msg_log_maintenance_traitement_equipement_destruction 58
#define T_MsgsSimMos_msg_log_maintenance_etat 59
#define T_MsgsSimMos_msg_log_ravitaillement_traitement_creation 60
#define T_MsgsSimMos_msg_log_ravitaillement_traitement_update 61
#define T_MsgsSimMos_msg_log_ravitaillement_traitement_destruction 62
#define T_MsgsSimMos_msg_log_ravitaillement_etat 63
#define T_MsgsSimMos_msg_log_ravitaillement_quotas 64
#define T_MsgsSimMos_nb_choice          65

typedef struct EXTERN ASN1T_MsgsSimMos {
   int t;
   union {
      /* t = 1 */
      ASN1T_MsgCtrlInfo *msg_ctrl_info;
      /* t = 2 */
      ASN1T_MsgCtrlBeginTick  msg_ctrl_begin_tick;
      /* t = 3 */
      ASN1T_MsgCtrlEndTick *msg_ctrl_end_tick;
      /* t = 4 */
      ASN1T_MsgCtrlStopAck  msg_ctrl_stop_ack;
      /* t = 5 */
      ASN1T_MsgCtrlPauseAck  msg_ctrl_pause_ack;
      /* t = 6 */
      ASN1T_MsgCtrlResumeAck  msg_ctrl_resume_ack;
      /* t = 7 */
      ASN1T_MsgCtrlChangeTimeFactorAck *msg_ctrl_change_time_factor_ack;
      /* t = 8 */
      /* t = 9 */
      /* t = 10 */
      /* t = 11 */
      /* t = 12 */
      /* t = 13 */
      /* t = 14 */
      /* t = 15 */
      /* t = 16 */
      /* t = 17 */
      /* t = 18 */
      ASN1T_MsgLimitCreation *msg_limit_creation;
      /* t = 19 */
      ASN1T_MsgLimitDestruction  msg_limit_destruction;
      /* t = 20 */
      ASN1T_MsgLimaCreation *msg_lima_creation;
      /* t = 21 */
      ASN1T_MsgLimaDestruction  msg_lima_destruction;
      /* t = 22 */
      ASN1T_MsgAutomateCreation *msg_automate_creation;
      /* t = 23 */
      ASN1T_MsgPionCreation *msg_pion_creation;
      /* t = 24 */
      ASN1T_MsgChangeDiplomatie *msg_change_diplomatie;
      /* t = 25 */
      ASN1T_MsgUnitKnowledgeCreation *msg_unit_knowledge_creation;
      /* t = 26 */
      ASN1T_MsgUnitKnowledgeUpdate *msg_unit_knowledge_update;
      /* t = 27 */
      ASN1T_MsgUnitKnowledgeDestruction *msg_unit_knowledge_destruction;
      /* t = 28 */
      ASN1T_MsgUnitAttributes *msg_unit_attributes;
      /* t = 29 */
      ASN1T_MsgUnitDotations *msg_unit_dotations;
      /* t = 30 */
      ASN1T_MsgUnitPathFind *msg_unit_pathfind;
      /* t = 31 */
      ASN1T_MsgStartDirectFire *msg_start_direct_fire;
      /* t = 32 */
      ASN1T_MsgStopDirectFire *msg_stop_direct_fire;
      /* t = 33 */
      ASN1T_MsgStartIndirectFire *msg_start_indirect_fire;
      /* t = 34 */
      ASN1T_MsgStopIndirectFire *msg_stop_indirect_fire;
      /* t = 35 */
      ASN1T_MsgExplosion *msg_explosion;
      /* t = 36 */
      ASN1T_MsgStartFireEffect *msg_start_fire_effect;
      /* t = 37 */
      ASN1T_MsgStopFireEffect  msg_stop_fire_effect;
      /* t = 38 */
      ASN1T_MsgCR *msg_cr;
      /* t = 39 */
      ASN1T_MsgOrderManagement *msg_order_management;
      /* t = 40 */
      ASN1T_MsgAttenteOrdreConduite *msg_attente_ordre_conduite;
      /* t = 41 */
      ASN1T_MsgAnnuleAttenteOrdreConduite *msg_annule_attente_ordre_conduite;
      /* t = 42 */
      ASN1T_MsgAutomateMRT *msg_automate_mrt;
      /* t = 43 */
      ASN1T_MsgPionOrder *msg_pion_order;
      /* t = 44 */
      ASN1T_MsgAutomateOrder *msg_automate_order;
      /* t = 45 */
      ASN1T_MsgObjectCreation *msg_object_creation;
      /* t = 46 */
      ASN1T_MsgObjectDestruction  msg_object_destruction;
      /* t = 47 */
      ASN1T_MsgObjectUpdate *msg_object_update;
      /* t = 48 */
      ASN1T_MsgObjectKnowledgeCreation *msg_object_knowledge_creation;
      /* t = 49 */
      ASN1T_MsgObjectKnowledgeUpdate *msg_object_knowledge_update;
      /* t = 50 */
      ASN1T_MsgObjectKnowledgeDestruction *msg_object_knowledge_destruction;
      /* t = 51 */
      ASN1T_MsgChangeAutomate *msg_change_automate;
      /* t = 52 */
      ASN1T_MsgLogSanteTraitementHumainCreation *msg_log_sante_traitement_humain_creation;
      /* t = 53 */
      ASN1T_MsgLogSanteTraitementHumainUpdate *msg_log_sante_traitement_humain_update;
      /* t = 54 */
      ASN1T_MsgLogSanteTraitementHumainDestruction *msg_log_sante_traitement_humain_destruction;
      /* t = 55 */
      ASN1T_MsgLogSanteEtat *msg_log_sante_etat;
      /* t = 56 */
      ASN1T_MsgLogMaintenanceTraitementEquipementCreation *msg_log_maintenance_traitement_equipement_creation;
      /* t = 57 */
      ASN1T_MsgLogMaintenanceTraitementEquipementUpdate *msg_log_maintenance_traitement_equipement_update;
      /* t = 58 */
      ASN1T_MsgLogMaintenanceTraitementEquipementDestruction *msg_log_maintenance_traitement_equipement_destruction;
      /* t = 59 */
      ASN1T_MsgLogMaintenanceEtat *msg_log_maintenance_etat;
      /* t = 60 */
      ASN1T_MsgLogRavitaillementTraitementCreation *msg_log_ravitaillement_traitement_creation;
      /* t = 61 */
      ASN1T_MsgLogRavitaillementTraitementUpdate *msg_log_ravitaillement_traitement_update;
      /* t = 62 */
      ASN1T_MsgLogRavitaillementTraitementDestruction *msg_log_ravitaillement_traitement_destruction;
      /* t = 63 */
      ASN1T_MsgLogRavitaillementEtat *msg_log_ravitaillement_etat;
      /* t = 64 */
      ASN1T_MsgLogRavitaillementQuotas *msg_log_ravitaillement_quotas;
      /* t = 65 */
   } u;
} ASN1T_MsgsSimMos;

class EXTERN ASN1C_MsgsSimMos : public ASN1CType {
public:
   ASN1T_MsgsSimMos& msgData;
   ASN1C_MsgsSimMos (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgsSimMos& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgsSimMos (ASN1CTXT* ctxt_p, ASN1T_MsgsSimMos* pvalue);
EXTERN int asn1PD_MsgsSimMos (ASN1CTXT* ctxt_p, ASN1T_MsgsSimMos* pvalue);

EXTERN void asn1Print_MsgsSimMos (ASN1ConstCharPtr name, ASN1T_MsgsSimMos* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgsSimMosWithContext                                     */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_MsgsSimMosWithContext_msg_limit_creation_ack 1
#define T_MsgsSimMosWithContext_msg_limit_destruction_ack 2
#define T_MsgsSimMosWithContext_msg_limit_update_ack 3
#define T_MsgsSimMosWithContext_msg_lima_creation_ack 4
#define T_MsgsSimMosWithContext_msg_lima_destruction_ack 5
#define T_MsgsSimMosWithContext_msg_lima_update_ack 6
#define T_MsgsSimMosWithContext_msg_pion_order_ack 7
#define T_MsgsSimMosWithContext_msg_order_conduite_ack 8
#define T_MsgsSimMosWithContext_msg_automate_order_ack 9
#define T_MsgsSimMosWithContext_msg_set_automate_mode_ack 10
#define T_MsgsSimMosWithContext_msg_unit_magic_action_ack 11
#define T_MsgsSimMosWithContext_msg_object_magic_action_ack 12
#define T_MsgsSimMosWithContext_msg_change_diplomatie_ack 13
#define T_MsgsSimMosWithContext_msg_change_groupe_connaissance_ack 14
#define T_MsgsSimMosWithContext_msg_change_liens_logistiques_ack 15
#define T_MsgsSimMosWithContext_msg_change_automate_ack 16
#define T_MsgsSimMosWithContext_msg_log_ravitaillement_pousser_flux_ack 17
#define T_MsgsSimMosWithContext_msg_log_ravitaillement_change_quotas_ack 18
#define T_MsgsSimMosWithContext_nb_choice 19

typedef struct EXTERN ASN1T_MsgsSimMosWithContext {
   int t;
   union {
      /* t = 1 */
      ASN1T_MsgLimitCreationAck *msg_limit_creation_ack;
      /* t = 2 */
      ASN1T_MsgLimitDestructionAck *msg_limit_destruction_ack;
      /* t = 3 */
      ASN1T_MsgLimitUpdateAck *msg_limit_update_ack;
      /* t = 4 */
      ASN1T_MsgLimaCreationAck *msg_lima_creation_ack;
      /* t = 5 */
      ASN1T_MsgLimaDestructionAck *msg_lima_destruction_ack;
      /* t = 6 */
      ASN1T_MsgLimaUpdateAck *msg_lima_update_ack;
      /* t = 7 */
      ASN1T_MsgPionOrderAck *msg_pion_order_ack;
      /* t = 8 */
      ASN1T_MsgOrderConduiteAck *msg_order_conduite_ack;
      /* t = 9 */
      ASN1T_MsgAutomateOrderAck *msg_automate_order_ack;
      /* t = 10 */
      ASN1T_MsgSetAutomateModeAck *msg_set_automate_mode_ack;
      /* t = 11 */
      ASN1T_MsgUnitMagicActionAck *msg_unit_magic_action_ack;
      /* t = 12 */
      ASN1T_MsgObjectMagicActionAck *msg_object_magic_action_ack;
      /* t = 13 */
      ASN1T_MsgChangeDiplomatieAck *msg_change_diplomatie_ack;
      /* t = 14 */
      ASN1T_MsgChangeGroupeConnaissanceAck *msg_change_groupe_connaissance_ack;
      /* t = 15 */
      ASN1T_MsgChangeLiensLogistiquesAck *msg_change_liens_logistiques_ack;
      /* t = 16 */
      ASN1T_MsgChangeAutomateAck *msg_change_automate_ack;
      /* t = 17 */
      ASN1T_MsgLogRavitaillementPousserFluxAck  msg_log_ravitaillement_pousser_flux_ack;
      /* t = 18 */
      ASN1T_MsgLogRavitaillementChangeQuotasAck  msg_log_ravitaillement_change_quotas_ack;
      /* t = 19 */
   } u;
} ASN1T_MsgsSimMosWithContext;

class EXTERN ASN1C_MsgsSimMosWithContext : public ASN1CType {
public:
   ASN1T_MsgsSimMosWithContext& msgData;
   ASN1C_MsgsSimMosWithContext (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgsSimMosWithContext& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgsSimMosWithContext (ASN1CTXT* ctxt_p, ASN1T_MsgsSimMosWithContext* pvalue);
EXTERN int asn1PD_MsgsSimMosWithContext (ASN1CTXT* ctxt_p, ASN1T_MsgsSimMosWithContext* pvalue);

EXTERN void asn1Print_MsgsSimMosWithContext (ASN1ConstCharPtr name, ASN1T_MsgsSimMosWithContext* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgsMosSim                                                */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_MsgsMosSim_msg_ctrl_client_announcement 1
#define T_MsgsMosSim_msg_ctrl_stop      2
#define T_MsgsMosSim_msg_ctrl_pause     3
#define T_MsgsMosSim_msg_ctrl_resume    4
#define T_MsgsMosSim_msg_ctrl_change_time_factor 5
#define T_MsgsMosSim_msg_ctrl_meteo_globale 6
#define T_MsgsMosSim_msg_ctrl_meteo_locale 7
#define T_MsgsMosSim_msg_ctrl_checkpoint_save_now 8
#define T_MsgsMosSim_msg_ctrl_checkpoint_set_frequency 9
#define T_MsgsMosSim_nb_choice          10

typedef struct EXTERN ASN1T_MsgsMosSim {
   int t;
   union {
      /* t = 1 */
      ASN1T_MsgCtrlClientAnnouncement  msg_ctrl_client_announcement;
      /* t = 2 */
      /* t = 3 */
      /* t = 4 */
      /* t = 5 */
      ASN1T_MsgCtrlChangeTimeFactor  msg_ctrl_change_time_factor;
      /* t = 6 */
      ASN1T_MsgCtrlMeteoGlobale *msg_ctrl_meteo_globale;
      /* t = 7 */
      ASN1T_MsgCtrlMeteoLocale *msg_ctrl_meteo_locale;
      /* t = 8 */
      ASN1T_MsgCtrlCheckPointSaveNow *msg_ctrl_checkpoint_save_now;
      /* t = 9 */
      ASN1T_MsgCtrlCheckPointSetFrequency  msg_ctrl_checkpoint_set_frequency;
      /* t = 10 */
   } u;
} ASN1T_MsgsMosSim;

class EXTERN ASN1C_MsgsMosSim : public ASN1CType {
public:
   ASN1T_MsgsMosSim& msgData;
   ASN1C_MsgsMosSim (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgsMosSim& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgsMosSim (ASN1CTXT* ctxt_p, ASN1T_MsgsMosSim* pvalue);
EXTERN int asn1PD_MsgsMosSim (ASN1CTXT* ctxt_p, ASN1T_MsgsMosSim* pvalue);

EXTERN void asn1Print_MsgsMosSim (ASN1ConstCharPtr name, ASN1T_MsgsMosSim* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgsMosSimWithContext                                     */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_MsgsMosSimWithContext_msg_limit_creation 1
#define T_MsgsMosSimWithContext_msg_limit_destruction 2
#define T_MsgsMosSimWithContext_msg_limit_update 3
#define T_MsgsMosSimWithContext_msg_lima_creation 4
#define T_MsgsMosSimWithContext_msg_lima_destruction 5
#define T_MsgsMosSimWithContext_msg_lima_update 6
#define T_MsgsMosSimWithContext_msg_pion_order 7
#define T_MsgsMosSimWithContext_msg_order_conduite 8
#define T_MsgsMosSimWithContext_msg_automate_order 9
#define T_MsgsMosSimWithContext_msg_set_automate_mode 10
#define T_MsgsMosSimWithContext_msg_unit_magic_action 11
#define T_MsgsMosSimWithContext_msg_object_magic_action 12
#define T_MsgsMosSimWithContext_msg_change_diplomatie 13
#define T_MsgsMosSimWithContext_msg_change_groupe_connaissance 14
#define T_MsgsMosSimWithContext_msg_change_liens_logistiques 15
#define T_MsgsMosSimWithContext_msg_change_automate 16
#define T_MsgsMosSimWithContext_msg_log_ravitaillement_pousser_flux 17
#define T_MsgsMosSimWithContext_msg_log_ravitaillement_change_quotas 18
#define T_MsgsMosSimWithContext_nb_choice 19

typedef struct EXTERN ASN1T_MsgsMosSimWithContext {
   int t;
   union {
      /* t = 1 */
      ASN1T_MsgLimitCreation *msg_limit_creation;
      /* t = 2 */
      ASN1T_MsgLimitDestruction  msg_limit_destruction;
      /* t = 3 */
      ASN1T_MsgLimitUpdate *msg_limit_update;
      /* t = 4 */
      ASN1T_MsgLimaCreation *msg_lima_creation;
      /* t = 5 */
      ASN1T_MsgLimaDestruction  msg_lima_destruction;
      /* t = 6 */
      ASN1T_MsgLimaUpdate *msg_lima_update;
      /* t = 7 */
      ASN1T_MsgPionOrder *msg_pion_order;
      /* t = 8 */
      ASN1T_MsgOrderConduite *msg_order_conduite;
      /* t = 9 */
      ASN1T_MsgAutomateOrder *msg_automate_order;
      /* t = 10 */
      ASN1T_MsgSetAutomateMode *msg_set_automate_mode;
      /* t = 11 */
      ASN1T_MsgUnitMagicAction *msg_unit_magic_action;
      /* t = 12 */
      ASN1T_MsgObjectMagicAction *msg_object_magic_action;
      /* t = 13 */
      ASN1T_MsgChangeDiplomatie *msg_change_diplomatie;
      /* t = 14 */
      ASN1T_MsgChangeGroupeConnaissance *msg_change_groupe_connaissance;
      /* t = 15 */
      ASN1T_MsgChangeLiensLogistiques *msg_change_liens_logistiques;
      /* t = 16 */
      ASN1T_MsgChangeAutomate *msg_change_automate;
      /* t = 17 */
      ASN1T_MsgLogRavitaillementPousserFlux *msg_log_ravitaillement_pousser_flux;
      /* t = 18 */
      ASN1T_MsgLogRavitaillementChangeQuotas *msg_log_ravitaillement_change_quotas;
      /* t = 19 */
   } u;
} ASN1T_MsgsMosSimWithContext;

class EXTERN ASN1C_MsgsMosSimWithContext : public ASN1CType {
public:
   ASN1T_MsgsMosSimWithContext& msgData;
   ASN1C_MsgsMosSimWithContext (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgsMosSimWithContext& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgsMosSimWithContext (ASN1CTXT* ctxt_p, ASN1T_MsgsMosSimWithContext* pvalue);
EXTERN int asn1PD_MsgsMosSimWithContext (ASN1CTXT* ctxt_p, ASN1T_MsgsMosSimWithContext* pvalue);

EXTERN void asn1Print_MsgsMosSimWithContext (ASN1ConstCharPtr name, ASN1T_MsgsMosSimWithContext* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgsMosLauncher                                           */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_MsgsMosLauncher_msg_request_list_mission 1
#define T_MsgsMosLauncher_msg_launch_sim 2
#define T_MsgsMosLauncher_msg_request_list_sim 3
#define T_MsgsMosLauncher_msg_request_list_checkpoint 4
#define T_MsgsMosLauncher_msg_launch_sim_from_checkpoint 5
#define T_MsgsMosLauncher_msg_stop_sim  6
#define T_MsgsMosLauncher_nb_choice     7

typedef struct EXTERN ASN1T_MsgsMosLauncher {
   int t;
   union {
      /* t = 1 */
      /* t = 2 */
      ASN1T_MsgMosLau_LaunchSim *msg_launch_sim;
      /* t = 3 */
      /* t = 4 */
      ASN1T_MsgMosLau_RequestListCheckpoint  msg_request_list_checkpoint;
      /* t = 5 */
      ASN1T_MsgMosLau_LaunchSimFromCheckpoint *msg_launch_sim_from_checkpoint;
      /* t = 6 */
      ASN1T_MsgMosLau_StopSim  msg_stop_sim;
      /* t = 7 */
   } u;
} ASN1T_MsgsMosLauncher;

class EXTERN ASN1C_MsgsMosLauncher : public ASN1CType {
public:
   ASN1T_MsgsMosLauncher& msgData;
   ASN1C_MsgsMosLauncher (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgsMosLauncher& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgsMosLauncher (ASN1CTXT* ctxt_p, ASN1T_MsgsMosLauncher* pvalue);
EXTERN int asn1PD_MsgsMosLauncher (ASN1CTXT* ctxt_p, ASN1T_MsgsMosLauncher* pvalue);

EXTERN void asn1Print_MsgsMosLauncher (ASN1ConstCharPtr name, ASN1T_MsgsMosLauncher* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgsLauncherMos                                           */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_MsgsLauncherMos_msg_list_mission 1
#define T_MsgsLauncherMos_msg_sim_launched 2
#define T_MsgsLauncherMos_msg_list_sim  3
#define T_MsgsLauncherMos_msg_sim_failure 4
#define T_MsgsLauncherMos_msg_list_checkpoint 5
#define T_MsgsLauncherMos_msg_exec_success 6
#define T_MsgsLauncherMos_nb_choice     7

typedef struct EXTERN ASN1T_MsgsLauncherMos {
   int t;
   union {
      /* t = 1 */
      ASN1T_MsgLauMos_ListMission *msg_list_mission;
      /* t = 2 */
      ASN1T_MsgLauMos_SimLaunched *msg_sim_launched;
      /* t = 3 */
      ASN1T_MsgLauMos_ListSim *msg_list_sim;
      /* t = 4 */
      ASN1T_MsgLauMos_SimFailure  msg_sim_failure;
      /* t = 5 */
      ASN1T_MsgLauMos_ListCheckpoint *msg_list_checkpoint;
      /* t = 6 */
      ASN1T_MsgLauMos_ExecSuccess *msg_exec_success;
      /* t = 7 */
   } u;
} ASN1T_MsgsLauncherMos;

class EXTERN ASN1C_MsgsLauncherMos : public ASN1CType {
public:
   ASN1T_MsgsLauncherMos& msgData;
   ASN1C_MsgsLauncherMos (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgsLauncherMos& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgsLauncherMos (ASN1CTXT* ctxt_p, ASN1T_MsgsLauncherMos* pvalue);
EXTERN int asn1PD_MsgsLauncherMos (ASN1CTXT* ctxt_p, ASN1T_MsgsLauncherMos* pvalue);

EXTERN void asn1Print_MsgsLauncherMos (ASN1ConstCharPtr name, ASN1T_MsgsLauncherMos* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgsLauncherSim                                           */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_MsgsLauncherSim_msg_ping      1
#define T_MsgsLauncherSim_msg_stop_sim  2
#define T_MsgsLauncherSim_nb_choice     3

typedef struct EXTERN ASN1T_MsgsLauncherSim {
   int t;
   union {
      /* t = 1 */
      /* t = 2 */
      ASN1T_MsgLauSim_StopSim  msg_stop_sim;
      /* t = 3 */
   } u;
} ASN1T_MsgsLauncherSim;

class EXTERN ASN1C_MsgsLauncherSim : public ASN1CType {
public:
   ASN1T_MsgsLauncherSim& msgData;
   ASN1C_MsgsLauncherSim (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgsLauncherSim& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgsLauncherSim (ASN1CTXT* ctxt_p, ASN1T_MsgsLauncherSim* pvalue);
EXTERN int asn1PD_MsgsLauncherSim (ASN1CTXT* ctxt_p, ASN1T_MsgsLauncherSim* pvalue);

EXTERN void asn1Print_MsgsLauncherSim (ASN1ConstCharPtr name, ASN1T_MsgsLauncherSim* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgsSimLauncher                                           */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_MsgsSimLauncher_msg_sim_launched 1
#define T_MsgsSimLauncher_msg_pong      2
#define T_MsgsSimLauncher_msg_exec_success 3
#define T_MsgsSimLauncher_nb_choice     4

typedef struct EXTERN ASN1T_MsgsSimLauncher {
   int t;
   union {
      /* t = 1 */
      ASN1T_MsgSimLau_SimLaunched *msg_sim_launched;
      /* t = 2 */
      ASN1T_MsgSimLau_Pong  msg_pong;
      /* t = 3 */
      ASN1T_MsgSimLau_ExecSuccess *msg_exec_success;
      /* t = 4 */
   } u;
} ASN1T_MsgsSimLauncher;

class EXTERN ASN1C_MsgsSimLauncher : public ASN1CType {
public:
   ASN1T_MsgsSimLauncher& msgData;
   ASN1C_MsgsSimLauncher (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgsSimLauncher& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgsSimLauncher (ASN1CTXT* ctxt_p, ASN1T_MsgsSimLauncher* pvalue);
EXTERN int asn1PD_MsgsSimLauncher (ASN1CTXT* ctxt_p, ASN1T_MsgsSimLauncher* pvalue);

EXTERN void asn1Print_MsgsSimLauncher (ASN1ConstCharPtr name, ASN1T_MsgsSimLauncher* pvalue);

#endif

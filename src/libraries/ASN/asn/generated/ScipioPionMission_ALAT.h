/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 04-Oct-2005.
 */
#ifndef SCIPIOPIONMISSION_ALAT_H
#define SCIPIOPIONMISSION_ALAT_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  EnumMissionALAT_PorteeAction                              */
/*                                                            */
/**************************************************************/

struct EXTERN EnumMissionALAT_PorteeAction {
   enum Root {
      courte_portee = 0,
      moyenne_portee = 1,
      longue_portee = 2,
      sans_munitions = 3
   } ;
} ;

typedef EnumMissionALAT_PorteeAction::Root ASN1T_EnumMissionALAT_PorteeAction;

class EXTERN ASN1C_EnumMissionALAT_PorteeAction : public ASN1CType {
public:
   ASN1T_EnumMissionALAT_PorteeAction& msgData;
   ASN1C_EnumMissionALAT_PorteeAction (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumMissionALAT_PorteeAction& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumMissionALAT_PorteeAction (ASN1CTXT* ctxt_p, ASN1T_EnumMissionALAT_PorteeAction value);
EXTERN int asn1PD_EnumMissionALAT_PorteeAction (ASN1CTXT* ctxt_p, ASN1T_EnumMissionALAT_PorteeAction* pvalue);

EXTERN void asn1Print_EnumMissionALAT_PorteeAction (ASN1ConstCharPtr name, ASN1T_EnumMissionALAT_PorteeAction* pvalue);

/**************************************************************/
/*                                                            */
/*  EnumMissionALAT_AmbianceMission                           */
/*                                                            */
/**************************************************************/

struct EXTERN EnumMissionALAT_AmbianceMission {
   enum Root {
      aa = 0,
      as = 1,
      polyvalent = 2
   } ;
} ;

typedef EnumMissionALAT_AmbianceMission::Root ASN1T_EnumMissionALAT_AmbianceMission;

class EXTERN ASN1C_EnumMissionALAT_AmbianceMission : public ASN1CType {
public:
   ASN1T_EnumMissionALAT_AmbianceMission& msgData;
   ASN1C_EnumMissionALAT_AmbianceMission (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumMissionALAT_AmbianceMission& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumMissionALAT_AmbianceMission (ASN1CTXT* ctxt_p, ASN1T_EnumMissionALAT_AmbianceMission value);
EXTERN int asn1PD_EnumMissionALAT_AmbianceMission (ASN1CTXT* ctxt_p, ASN1T_EnumMissionALAT_AmbianceMission* pvalue);

EXTERN void asn1Print_EnumMissionALAT_AmbianceMission (ASN1ConstCharPtr name, ASN1T_EnumMissionALAT_AmbianceMission* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ALAT_EvacuationSanitaire                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ALAT_EvacuationSanitaire {
   ASN1T_Point  point_debarquement;
   ASN1T_Polygon  zone_extraction;
   ASN1T_Point  point_regroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;

   ASN1T_Mission_Pion_ALAT_EvacuationSanitaire () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Pion_ALAT_EvacuationSanitaire;

class EXTERN ASN1C_Mission_Pion_ALAT_EvacuationSanitaire : public ASN1CType {
public:
   ASN1T_Mission_Pion_ALAT_EvacuationSanitaire& msgData;
   ASN1C_Mission_Pion_ALAT_EvacuationSanitaire (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ALAT_EvacuationSanitaire& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ALAT_EvacuationSanitaire (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_EvacuationSanitaire* pvalue);
EXTERN int asn1PD_Mission_Pion_ALAT_EvacuationSanitaire (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_EvacuationSanitaire* pvalue);

EXTERN void asn1Print_Mission_Pion_ALAT_EvacuationSanitaire (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ALAT_EvacuationSanitaire* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ALAT_ReconnaitreDansLaProfondeur             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ALAT_ReconnaitreDansLaProfondeur {
   ASN1T_Point  point_regroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1T_EnumMissionALAT_AmbianceMission  ambiance_mission;
   ASN1T_Point  point_dislocation;
   ASN1T_Localisation  zone;

   ASN1T_Mission_Pion_ALAT_ReconnaitreDansLaProfondeur () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Pion_ALAT_ReconnaitreDansLaProfondeur;

class EXTERN ASN1C_Mission_Pion_ALAT_ReconnaitreDansLaProfondeur : public ASN1CType {
public:
   ASN1T_Mission_Pion_ALAT_ReconnaitreDansLaProfondeur& msgData;
   ASN1C_Mission_Pion_ALAT_ReconnaitreDansLaProfondeur (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ALAT_ReconnaitreDansLaProfondeur& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ALAT_ReconnaitreDansLaProfondeur (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_ReconnaitreDansLaProfondeur* pvalue);
EXTERN int asn1PD_Mission_Pion_ALAT_ReconnaitreDansLaProfondeur (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_ReconnaitreDansLaProfondeur* pvalue);

EXTERN void asn1Print_Mission_Pion_ALAT_ReconnaitreDansLaProfondeur (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ALAT_ReconnaitreDansLaProfondeur* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ALAT_ReconnaitreContourEnnemi                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ALAT_ReconnaitreContourEnnemi {
   ASN1T_Point  point_regroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1T_EnumMissionALAT_AmbianceMission  ambiance_mission;

   ASN1T_Mission_Pion_ALAT_ReconnaitreContourEnnemi () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Pion_ALAT_ReconnaitreContourEnnemi;

class EXTERN ASN1C_Mission_Pion_ALAT_ReconnaitreContourEnnemi : public ASN1CType {
public:
   ASN1T_Mission_Pion_ALAT_ReconnaitreContourEnnemi& msgData;
   ASN1C_Mission_Pion_ALAT_ReconnaitreContourEnnemi (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ALAT_ReconnaitreContourEnnemi& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ALAT_ReconnaitreContourEnnemi (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_ReconnaitreContourEnnemi* pvalue);
EXTERN int asn1PD_Mission_Pion_ALAT_ReconnaitreContourEnnemi (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_ReconnaitreContourEnnemi* pvalue);

EXTERN void asn1Print_Mission_Pion_ALAT_ReconnaitreContourEnnemi (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ALAT_ReconnaitreContourEnnemi* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ALAT_DetruireNeutraliserDansZone             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ALAT_DetruireNeutraliserDansZone {
   ASN1T_Polygon  zone;
   ASN1T_Point  point_dislocation;
   ASN1T_NatureAtlas  cibles_prioritaires;
   ASN1T_Point  point_regroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1T_EnumMissionALAT_AmbianceMission  ambiance_mission;
   ASN1BOOL  neutraliser;

   ASN1T_Mission_Pion_ALAT_DetruireNeutraliserDansZone () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Pion_ALAT_DetruireNeutraliserDansZone;

class EXTERN ASN1C_Mission_Pion_ALAT_DetruireNeutraliserDansZone : public ASN1CType {
public:
   ASN1T_Mission_Pion_ALAT_DetruireNeutraliserDansZone& msgData;
   ASN1C_Mission_Pion_ALAT_DetruireNeutraliserDansZone (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ALAT_DetruireNeutraliserDansZone& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ALAT_DetruireNeutraliserDansZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_DetruireNeutraliserDansZone* pvalue);
EXTERN int asn1PD_Mission_Pion_ALAT_DetruireNeutraliserDansZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_DetruireNeutraliserDansZone* pvalue);

EXTERN void asn1Print_Mission_Pion_ALAT_DetruireNeutraliserDansZone (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ALAT_DetruireNeutraliserDansZone* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ALAT_DetruireAeromobile                      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ALAT_DetruireAeromobile {
   ASN1T_ListKnowledgeAgent  unite_a_detruire;
   ASN1T_Point  point_regroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1T_EnumMissionALAT_AmbianceMission  ambiance_mission;

   ASN1T_Mission_Pion_ALAT_DetruireAeromobile () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Pion_ALAT_DetruireAeromobile;

class EXTERN ASN1C_Mission_Pion_ALAT_DetruireAeromobile : public ASN1CType {
public:
   ASN1T_Mission_Pion_ALAT_DetruireAeromobile& msgData;
   ASN1C_Mission_Pion_ALAT_DetruireAeromobile (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ALAT_DetruireAeromobile& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ALAT_DetruireAeromobile (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_DetruireAeromobile* pvalue);
EXTERN int asn1PD_Mission_Pion_ALAT_DetruireAeromobile (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_DetruireAeromobile* pvalue);

EXTERN void asn1Print_Mission_Pion_ALAT_DetruireAeromobile (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ALAT_DetruireAeromobile* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ALAT_Freiner                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ALAT_Freiner {
   ASN1T_NatureAtlas  cibles_prioritaires;
   ASN1T_Point  point_regroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1T_EnumMissionALAT_AmbianceMission  ambiance_mission;

   ASN1T_Mission_Pion_ALAT_Freiner () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Pion_ALAT_Freiner;

class EXTERN ASN1C_Mission_Pion_ALAT_Freiner : public ASN1CType {
public:
   ASN1T_Mission_Pion_ALAT_Freiner& msgData;
   ASN1C_Mission_Pion_ALAT_Freiner (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ALAT_Freiner& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ALAT_Freiner (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_Freiner* pvalue);
EXTERN int asn1PD_Mission_Pion_ALAT_Freiner (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_Freiner* pvalue);

EXTERN void asn1Print_Mission_Pion_ALAT_Freiner (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ALAT_Freiner* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ALAT_Jalonner                                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ALAT_Jalonner {
   ASN1T_Point  point_regroupement;
   ASN1T_NatureAtlas  cibles_prioritaires;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1T_EnumMissionALAT_AmbianceMission  ambiance_mission;

   ASN1T_Mission_Pion_ALAT_Jalonner () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Pion_ALAT_Jalonner;

class EXTERN ASN1C_Mission_Pion_ALAT_Jalonner : public ASN1CType {
public:
   ASN1T_Mission_Pion_ALAT_Jalonner& msgData;
   ASN1C_Mission_Pion_ALAT_Jalonner (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ALAT_Jalonner& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ALAT_Jalonner (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_Jalonner* pvalue);
EXTERN int asn1PD_Mission_Pion_ALAT_Jalonner (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_Jalonner* pvalue);

EXTERN void asn1Print_Mission_Pion_ALAT_Jalonner (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ALAT_Jalonner* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ALAT_Escorter                                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ALAT_Escorter {
   ASN1T_ListAgent  unites_a_escorter;
   ASN1T_Point  point_regroupement;
   ASN1T_NatureAtlas  cibles_prioritaires;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1T_EnumMissionALAT_AmbianceMission  ambiance_mission;

   ASN1T_Mission_Pion_ALAT_Escorter () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Pion_ALAT_Escorter;

class EXTERN ASN1C_Mission_Pion_ALAT_Escorter : public ASN1CType {
public:
   ASN1T_Mission_Pion_ALAT_Escorter& msgData;
   ASN1C_Mission_Pion_ALAT_Escorter (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ALAT_Escorter& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ALAT_Escorter (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_Escorter* pvalue);
EXTERN int asn1PD_Mission_Pion_ALAT_Escorter (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_Escorter* pvalue);

EXTERN void asn1Print_Mission_Pion_ALAT_Escorter (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ALAT_Escorter* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ALAT_Heliporter                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ALAT_Heliporter {
   ASN1T_ListAgent  unites_a_heliporter;
   ASN1T_Point  point_debarquement;
   ASN1T_Point  point_embarquement;
   ASN1T_Point  point_regroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1BOOL  attendre_unite;
   ASN1BOOL  avec_materiel;

   ASN1T_Mission_Pion_ALAT_Heliporter () {
      ravitaillement_debut_mission = TRUE;
      avec_materiel = FALSE;
   }
} ASN1T_Mission_Pion_ALAT_Heliporter;

class EXTERN ASN1C_Mission_Pion_ALAT_Heliporter : public ASN1CType {
public:
   ASN1T_Mission_Pion_ALAT_Heliporter& msgData;
   ASN1C_Mission_Pion_ALAT_Heliporter (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ALAT_Heliporter& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ALAT_Heliporter (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_Heliporter* pvalue);
EXTERN int asn1PD_Mission_Pion_ALAT_Heliporter (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_Heliporter* pvalue);

EXTERN void asn1Print_Mission_Pion_ALAT_Heliporter (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ALAT_Heliporter* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ALAT_HeliporterHelitransporterPlotRavitaill  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement {
   ASN1T_Point  point_debarquement;
   ASN1T_Point  position_regroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;

   ASN1T_Mission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement;

class EXTERN ASN1C_Mission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement : public ASN1CType {
public:
   ASN1T_Mission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement& msgData;
   ASN1C_Mission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement* pvalue);
EXTERN int asn1PD_Mission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement* pvalue);

EXTERN void asn1Print_Mission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ALAT_Helitransporter                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ALAT_Helitransporter {
   ASN1T_ListAgent  unites_a_helitransporter;
   ASN1T_Point  point_debarquement;
   ASN1T_Point  point_embarquement;
   ASN1T_Point  point_regroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1BOOL  avec_materiel;

   ASN1T_Mission_Pion_ALAT_Helitransporter () {
      ravitaillement_debut_mission = TRUE;
      avec_materiel = FALSE;
   }
} ASN1T_Mission_Pion_ALAT_Helitransporter;

class EXTERN ASN1C_Mission_Pion_ALAT_Helitransporter : public ASN1CType {
public:
   ASN1T_Mission_Pion_ALAT_Helitransporter& msgData;
   ASN1C_Mission_Pion_ALAT_Helitransporter (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ALAT_Helitransporter& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ALAT_Helitransporter (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_Helitransporter* pvalue);
EXTERN int asn1PD_Mission_Pion_ALAT_Helitransporter (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_Helitransporter* pvalue);

EXTERN void asn1Print_Mission_Pion_ALAT_Helitransporter (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ALAT_Helitransporter* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ALAT_EffectuerRechercheEtSauvetage           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ALAT_EffectuerRechercheEtSauvetage {
   ASN1T_ListAgent  unites_a_secourir;
   ASN1T_Point  point_regroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;

   ASN1T_Mission_Pion_ALAT_EffectuerRechercheEtSauvetage () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Pion_ALAT_EffectuerRechercheEtSauvetage;

class EXTERN ASN1C_Mission_Pion_ALAT_EffectuerRechercheEtSauvetage : public ASN1CType {
public:
   ASN1T_Mission_Pion_ALAT_EffectuerRechercheEtSauvetage& msgData;
   ASN1C_Mission_Pion_ALAT_EffectuerRechercheEtSauvetage (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ALAT_EffectuerRechercheEtSauvetage& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ALAT_EffectuerRechercheEtSauvetage (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_EffectuerRechercheEtSauvetage* pvalue);
EXTERN int asn1PD_Mission_Pion_ALAT_EffectuerRechercheEtSauvetage (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_EffectuerRechercheEtSauvetage* pvalue);

EXTERN void asn1Print_Mission_Pion_ALAT_EffectuerRechercheEtSauvetage (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ALAT_EffectuerRechercheEtSauvetage* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ALAT_IMEX                                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ALAT_IMEX {
   ASN1T_ListAgent  unites_a_appuyer;
   ASN1T_Point  point_regroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;

   ASN1T_Mission_Pion_ALAT_IMEX () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Pion_ALAT_IMEX;

class EXTERN ASN1C_Mission_Pion_ALAT_IMEX : public ASN1CType {
public:
   ASN1T_Mission_Pion_ALAT_IMEX& msgData;
   ASN1C_Mission_Pion_ALAT_IMEX (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ALAT_IMEX& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ALAT_IMEX (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_IMEX* pvalue);
EXTERN int asn1PD_Mission_Pion_ALAT_IMEX (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_IMEX* pvalue);

EXTERN void asn1Print_Mission_Pion_ALAT_IMEX (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ALAT_IMEX* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ALAT_Eclairer                                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ALAT_Eclairer {
   ASN1T_Point  point_regroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1T_EnumMissionALAT_AmbianceMission  ambiance_mission;

   ASN1T_Mission_Pion_ALAT_Eclairer () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Pion_ALAT_Eclairer;

class EXTERN ASN1C_Mission_Pion_ALAT_Eclairer : public ASN1CType {
public:
   ASN1T_Mission_Pion_ALAT_Eclairer& msgData;
   ASN1C_Mission_Pion_ALAT_Eclairer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ALAT_Eclairer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ALAT_Eclairer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_Eclairer* pvalue);
EXTERN int asn1PD_Mission_Pion_ALAT_Eclairer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_Eclairer* pvalue);

EXTERN void asn1Print_Mission_Pion_ALAT_Eclairer (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ALAT_Eclairer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ALAT_Surveiller                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ALAT_Surveiller {
   ASN1T_Polygon  zone;
   ASN1T_Point  point_regroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1T_EnumMissionALAT_AmbianceMission  ambiance_mission;

   ASN1T_Mission_Pion_ALAT_Surveiller () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Pion_ALAT_Surveiller;

class EXTERN ASN1C_Mission_Pion_ALAT_Surveiller : public ASN1CType {
public:
   ASN1T_Mission_Pion_ALAT_Surveiller& msgData;
   ASN1C_Mission_Pion_ALAT_Surveiller (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ALAT_Surveiller& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ALAT_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_Surveiller* pvalue);
EXTERN int asn1PD_Mission_Pion_ALAT_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_Surveiller* pvalue);

EXTERN void asn1Print_Mission_Pion_ALAT_Surveiller (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ALAT_Surveiller* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ALAT_AppuyerDirectAuContact                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ALAT_AppuyerDirectAuContact {
   ASN1T_NatureAtlas  cibles_prioritaires;
   ASN1T_ListAgent  unites_a_appuyer;
   ASN1T_Point  point_regroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1T_EnumMissionALAT_AmbianceMission  ambiance_mission;

   ASN1T_Mission_Pion_ALAT_AppuyerDirectAuContact () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Pion_ALAT_AppuyerDirectAuContact;

class EXTERN ASN1C_Mission_Pion_ALAT_AppuyerDirectAuContact : public ASN1CType {
public:
   ASN1T_Mission_Pion_ALAT_AppuyerDirectAuContact& msgData;
   ASN1C_Mission_Pion_ALAT_AppuyerDirectAuContact (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ALAT_AppuyerDirectAuContact& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ALAT_AppuyerDirectAuContact (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_AppuyerDirectAuContact* pvalue);
EXTERN int asn1PD_Mission_Pion_ALAT_AppuyerDirectAuContact (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_AppuyerDirectAuContact* pvalue);

EXTERN void asn1Print_Mission_Pion_ALAT_AppuyerDirectAuContact (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ALAT_AppuyerDirectAuContact* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ALAT_Couvrir                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ALAT_Couvrir {
   ASN1T_NatureAtlas  cibles_prioritaires;
   ASN1T_ListAgent  unites_a_couvrir;
   ASN1T_Point  point_regroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1T_EnumMissionALAT_AmbianceMission  ambiance_mission;

   ASN1T_Mission_Pion_ALAT_Couvrir () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Pion_ALAT_Couvrir;

class EXTERN ASN1C_Mission_Pion_ALAT_Couvrir : public ASN1CType {
public:
   ASN1T_Mission_Pion_ALAT_Couvrir& msgData;
   ASN1C_Mission_Pion_ALAT_Couvrir (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ALAT_Couvrir& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ALAT_Couvrir (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_Couvrir* pvalue);
EXTERN int asn1PD_Mission_Pion_ALAT_Couvrir (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_Couvrir* pvalue);

EXTERN void asn1Print_Mission_Pion_ALAT_Couvrir (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ALAT_Couvrir* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ALAT_Attaquer                                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ALAT_Attaquer {
   ASN1T_NatureAtlas  cibles_prioritaires;
   ASN1T_Point  point_regroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1T_EnumMissionALAT_AmbianceMission  ambiance_mission;

   ASN1T_Mission_Pion_ALAT_Attaquer () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Pion_ALAT_Attaquer;

class EXTERN ASN1C_Mission_Pion_ALAT_Attaquer : public ASN1CType {
public:
   ASN1T_Mission_Pion_ALAT_Attaquer& msgData;
   ASN1C_Mission_Pion_ALAT_Attaquer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ALAT_Attaquer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ALAT_Attaquer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_Attaquer* pvalue);
EXTERN int asn1PD_Mission_Pion_ALAT_Attaquer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ALAT_Attaquer* pvalue);

EXTERN void asn1Print_Mission_Pion_ALAT_Attaquer (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ALAT_Attaquer* pvalue);

#endif

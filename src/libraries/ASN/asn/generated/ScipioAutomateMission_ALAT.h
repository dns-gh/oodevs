/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 13-Dec-2005.
 */
#ifndef SCIPIOAUTOMATEMISSION_ALAT_H
#define SCIPIOAUTOMATEMISSION_ALAT_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"
#include "ScipioPionMission_ALAT.h"

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ALAT_Heliporter                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ALAT_Heliporter {
   ASN1T_ListAgent  unites_a_heliporter;
   ASN1T_Point  point_debarquement;
   ASN1T_Point  point_embarquement;
   ASN1T_Point  point_regroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1T_EnumMissionALAT_AmbianceMission  ambiance_mission;
   ASN1BOOL  attendre_unites;
   ASN1BOOL  avec_materiel;

   ASN1T_Mission_Automate_ALAT_Heliporter () {
      ravitaillement_debut_mission = TRUE;
      attendre_unites = TRUE;
      avec_materiel = FALSE;
   }
} ASN1T_Mission_Automate_ALAT_Heliporter;

class EXTERN ASN1C_Mission_Automate_ALAT_Heliporter : public ASN1CType {
public:
   ASN1T_Mission_Automate_ALAT_Heliporter& msgData;
   ASN1C_Mission_Automate_ALAT_Heliporter (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ALAT_Heliporter& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ALAT_Heliporter (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_Heliporter* pvalue);
EXTERN int asn1PD_Mission_Automate_ALAT_Heliporter (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_Heliporter* pvalue);

EXTERN void asn1Print_Mission_Automate_ALAT_Heliporter (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ALAT_Heliporter* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ALAT_Jalonner                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ALAT_Jalonner {
   ASN1T_NatureAtlas  cibles_prioritaires;
   ASN1T_Point  point_regroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1T_EnumMissionALAT_AmbianceMission  ambiance_mission;

   ASN1T_Mission_Automate_ALAT_Jalonner () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Automate_ALAT_Jalonner;

class EXTERN ASN1C_Mission_Automate_ALAT_Jalonner : public ASN1CType {
public:
   ASN1T_Mission_Automate_ALAT_Jalonner& msgData;
   ASN1C_Mission_Automate_ALAT_Jalonner (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ALAT_Jalonner& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ALAT_Jalonner (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_Jalonner* pvalue);
EXTERN int asn1PD_Mission_Automate_ALAT_Jalonner (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_Jalonner* pvalue);

EXTERN void asn1Print_Mission_Automate_ALAT_Jalonner (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ALAT_Jalonner* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ALAT_Helitransporter                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ALAT_Helitransporter {
   ASN1T_ListAgent  unites_a_helitransporter;
   ASN1T_Point  point_debarquement;
   ASN1T_Point  point_embarquement;
   ASN1T_Point  point_regroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1T_EnumMissionALAT_AmbianceMission  ambiance_mission;
   ASN1BOOL  avec_materiel;

   ASN1T_Mission_Automate_ALAT_Helitransporter () {
      ravitaillement_debut_mission = TRUE;
      avec_materiel = FALSE;
   }
} ASN1T_Mission_Automate_ALAT_Helitransporter;

class EXTERN ASN1C_Mission_Automate_ALAT_Helitransporter : public ASN1CType {
public:
   ASN1T_Mission_Automate_ALAT_Helitransporter& msgData;
   ASN1C_Mission_Automate_ALAT_Helitransporter (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ALAT_Helitransporter& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ALAT_Helitransporter (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_Helitransporter* pvalue);
EXTERN int asn1PD_Mission_Automate_ALAT_Helitransporter (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_Helitransporter* pvalue);

EXTERN void asn1Print_Mission_Automate_ALAT_Helitransporter (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ALAT_Helitransporter* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ALAT_ExtrairePersonnel                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ALAT_ExtrairePersonnel {
   ASN1T_ListAgent  unites_a_helitransporter;
   ASN1T_Point  point_debarquement;
   ASN1T_Point  point_embarquement;
   ASN1T_Point  point_regroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1T_EnumMissionALAT_AmbianceMission  ambiance_mission;
   ASN1BOOL  avec_materiel;

   ASN1T_Mission_Automate_ALAT_ExtrairePersonnel () {
      ravitaillement_debut_mission = TRUE;
      avec_materiel = FALSE;
   }
} ASN1T_Mission_Automate_ALAT_ExtrairePersonnel;

class EXTERN ASN1C_Mission_Automate_ALAT_ExtrairePersonnel : public ASN1CType {
public:
   ASN1T_Mission_Automate_ALAT_ExtrairePersonnel& msgData;
   ASN1C_Mission_Automate_ALAT_ExtrairePersonnel (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ALAT_ExtrairePersonnel& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ALAT_ExtrairePersonnel (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_ExtrairePersonnel* pvalue);
EXTERN int asn1PD_Mission_Automate_ALAT_ExtrairePersonnel (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_ExtrairePersonnel* pvalue);

EXTERN void asn1Print_Mission_Automate_ALAT_ExtrairePersonnel (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ALAT_ExtrairePersonnel* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ALAT_Appuyer                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ALAT_Appuyer {
   ASN1T_ListAgent  unites_a_appuyer;
   ASN1T_Point  point_regroupement;
   ASN1T_NatureAtlas  cibles_prioritaires;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_Point  point_dislocation;
   ASN1T_Polygon  zone;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1T_EnumMissionALAT_AmbianceMission  ambiance_mission;

   ASN1T_Mission_Automate_ALAT_Appuyer () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Automate_ALAT_Appuyer;

class EXTERN ASN1C_Mission_Automate_ALAT_Appuyer : public ASN1CType {
public:
   ASN1T_Mission_Automate_ALAT_Appuyer& msgData;
   ASN1C_Mission_Automate_ALAT_Appuyer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ALAT_Appuyer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ALAT_Appuyer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_Appuyer* pvalue);
EXTERN int asn1PD_Mission_Automate_ALAT_Appuyer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_Appuyer* pvalue);

EXTERN void asn1Print_Mission_Automate_ALAT_Appuyer (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ALAT_Appuyer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ALAT_Couvrir                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ALAT_Couvrir {
   ASN1T_ListAgent  unites_a_appuyer;
   ASN1T_Point  point_regroupement;
   ASN1T_NatureAtlas  cibles_prioritaires;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1T_EnumMissionALAT_AmbianceMission  ambiance_mission;
} ASN1T_Mission_Automate_ALAT_Couvrir;

class EXTERN ASN1C_Mission_Automate_ALAT_Couvrir : public ASN1CType {
public:
   ASN1T_Mission_Automate_ALAT_Couvrir& msgData;
   ASN1C_Mission_Automate_ALAT_Couvrir (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ALAT_Couvrir& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ALAT_Couvrir (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_Couvrir* pvalue);
EXTERN int asn1PD_Mission_Automate_ALAT_Couvrir (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_Couvrir* pvalue);

EXTERN void asn1Print_Mission_Automate_ALAT_Couvrir (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ALAT_Couvrir* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ALAT_Surveiller                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ALAT_Surveiller {
   ASN1T_Polygon  zone;
   ASN1T_Point  point_regroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1T_EnumMissionALAT_AmbianceMission  ambiance_mission;
   ASN1T_Point  point_logistique;

   ASN1T_Mission_Automate_ALAT_Surveiller () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Automate_ALAT_Surveiller;

class EXTERN ASN1C_Mission_Automate_ALAT_Surveiller : public ASN1CType {
public:
   ASN1T_Mission_Automate_ALAT_Surveiller& msgData;
   ASN1C_Mission_Automate_ALAT_Surveiller (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ALAT_Surveiller& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ALAT_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_Surveiller* pvalue);
EXTERN int asn1PD_Mission_Automate_ALAT_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_Surveiller* pvalue);

EXTERN void asn1Print_Mission_Automate_ALAT_Surveiller (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ALAT_Surveiller* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ALAT_Reconnaitre                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ALAT_Reconnaitre {
   ASN1T_Point  pointRegroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1T_EnumMissionALAT_AmbianceMission  ambiance_mission;

   ASN1T_Mission_Automate_ALAT_Reconnaitre () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Automate_ALAT_Reconnaitre;

class EXTERN ASN1C_Mission_Automate_ALAT_Reconnaitre : public ASN1CType {
public:
   ASN1T_Mission_Automate_ALAT_Reconnaitre& msgData;
   ASN1C_Mission_Automate_ALAT_Reconnaitre (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ALAT_Reconnaitre& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ALAT_Reconnaitre (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_Reconnaitre* pvalue);
EXTERN int asn1PD_Mission_Automate_ALAT_Reconnaitre (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_Reconnaitre* pvalue);

EXTERN void asn1Print_Mission_Automate_ALAT_Reconnaitre (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ALAT_Reconnaitre* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ALAT_Freiner                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ALAT_Freiner {
   ASN1T_NatureAtlas  cibles_prioritaires;
   ASN1T_Point  point_regroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1T_EnumMissionALAT_AmbianceMission  ambiance_mission;

   ASN1T_Mission_Automate_ALAT_Freiner () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Automate_ALAT_Freiner;

class EXTERN ASN1C_Mission_Automate_ALAT_Freiner : public ASN1CType {
public:
   ASN1T_Mission_Automate_ALAT_Freiner& msgData;
   ASN1C_Mission_Automate_ALAT_Freiner (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ALAT_Freiner& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ALAT_Freiner (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_Freiner* pvalue);
EXTERN int asn1PD_Mission_Automate_ALAT_Freiner (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_Freiner* pvalue);

EXTERN void asn1Print_Mission_Automate_ALAT_Freiner (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ALAT_Freiner* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ALAT_Escorter                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ALAT_Escorter {
   ASN1T_ListAgent  unites_a_escorter;
   ASN1T_NatureAtlas  cibles_prioritaires;
   ASN1T_Point  point_regroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1T_EnumMissionALAT_AmbianceMission  ambiance_mission;

   ASN1T_Mission_Automate_ALAT_Escorter () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Automate_ALAT_Escorter;

class EXTERN ASN1C_Mission_Automate_ALAT_Escorter : public ASN1CType {
public:
   ASN1T_Mission_Automate_ALAT_Escorter& msgData;
   ASN1C_Mission_Automate_ALAT_Escorter (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ALAT_Escorter& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ALAT_Escorter (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_Escorter* pvalue);
EXTERN int asn1PD_Mission_Automate_ALAT_Escorter (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_Escorter* pvalue);

EXTERN void asn1Print_Mission_Automate_ALAT_Escorter (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ALAT_Escorter* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ALAT_Attaquer                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ALAT_Attaquer {
   ASN1T_NatureAtlas  cibles_prioritaires;
   ASN1T_Point  point_regroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1T_EnumMissionALAT_AmbianceMission  ambiance_mission;

   ASN1T_Mission_Automate_ALAT_Attaquer () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Automate_ALAT_Attaquer;

class EXTERN ASN1C_Mission_Automate_ALAT_Attaquer : public ASN1CType {
public:
   ASN1T_Mission_Automate_ALAT_Attaquer& msgData;
   ASN1C_Mission_Automate_ALAT_Attaquer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ALAT_Attaquer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ALAT_Attaquer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_Attaquer* pvalue);
EXTERN int asn1PD_Mission_Automate_ALAT_Attaquer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_Attaquer* pvalue);

EXTERN void asn1Print_Mission_Automate_ALAT_Attaquer (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ALAT_Attaquer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ALAT_RecueillirFormationAeromobile       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ALAT_RecueillirFormationAeromobile {
   ASN1T_Polygon  zone;
   ASN1T_NatureAtlas  cibles_prioritaires;
   ASN1T_Point  point_regroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1T_EnumMissionALAT_AmbianceMission  ambiance_mission;

   ASN1T_Mission_Automate_ALAT_RecueillirFormationAeromobile () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Automate_ALAT_RecueillirFormationAeromobile;

class EXTERN ASN1C_Mission_Automate_ALAT_RecueillirFormationAeromobile : public ASN1CType {
public:
   ASN1T_Mission_Automate_ALAT_RecueillirFormationAeromobile& msgData;
   ASN1C_Mission_Automate_ALAT_RecueillirFormationAeromobile (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ALAT_RecueillirFormationAeromobile& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ALAT_RecueillirFormationAeromobile (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_RecueillirFormationAeromobile* pvalue);
EXTERN int asn1PD_Mission_Automate_ALAT_RecueillirFormationAeromobile (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_RecueillirFormationAeromobile* pvalue);

EXTERN void asn1Print_Mission_Automate_ALAT_RecueillirFormationAeromobile (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ALAT_RecueillirFormationAeromobile* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ALAT_AttaquerFormationAeromobile         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ALAT_AttaquerFormationAeromobile {
   ASN1T_ListKnowledgeAgent  unites_a_attaquer;
   ASN1T_Point  point_regroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1T_EnumMissionALAT_AmbianceMission  ambiance_mission;

   ASN1T_Mission_Automate_ALAT_AttaquerFormationAeromobile () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Automate_ALAT_AttaquerFormationAeromobile;

class EXTERN ASN1C_Mission_Automate_ALAT_AttaquerFormationAeromobile : public ASN1CType {
public:
   ASN1T_Mission_Automate_ALAT_AttaquerFormationAeromobile& msgData;
   ASN1C_Mission_Automate_ALAT_AttaquerFormationAeromobile (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ALAT_AttaquerFormationAeromobile& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ALAT_AttaquerFormationAeromobile (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_AttaquerFormationAeromobile* pvalue);
EXTERN int asn1PD_Mission_Automate_ALAT_AttaquerFormationAeromobile (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_AttaquerFormationAeromobile* pvalue);

EXTERN void asn1Print_Mission_Automate_ALAT_AttaquerFormationAeromobile (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ALAT_AttaquerFormationAeromobile* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ALAT_DonnerCoupArret                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ALAT_DonnerCoupArret {
   ASN1T_Polygon  zone;
   ASN1T_NatureAtlas  cibles_prioritaires;
   ASN1T_Point  point_regroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1T_EnumMissionALAT_AmbianceMission  ambiance_mission;

   ASN1T_Mission_Automate_ALAT_DonnerCoupArret () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Automate_ALAT_DonnerCoupArret;

class EXTERN ASN1C_Mission_Automate_ALAT_DonnerCoupArret : public ASN1CType {
public:
   ASN1T_Mission_Automate_ALAT_DonnerCoupArret& msgData;
   ASN1C_Mission_Automate_ALAT_DonnerCoupArret (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ALAT_DonnerCoupArret& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ALAT_DonnerCoupArret (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_DonnerCoupArret* pvalue);
EXTERN int asn1PD_Mission_Automate_ALAT_DonnerCoupArret (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_DonnerCoupArret* pvalue);

EXTERN void asn1Print_Mission_Automate_ALAT_DonnerCoupArret (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ALAT_DonnerCoupArret* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ALAT_DetruireNeutraliserDansProfondeur   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ALAT_DetruireNeutraliserDansProfondeur {
   ASN1T_Polygon  zone;
   ASN1T_NatureAtlas  cibles_prioritaires;
   ASN1T_Point  point_regroupement;
   ASN1T_Point  point_dislocation;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1T_EnumMissionALAT_AmbianceMission  ambiance_mission;
   ASN1BOOL  neutraliser;

   ASN1T_Mission_Automate_ALAT_DetruireNeutraliserDansProfondeur () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Automate_ALAT_DetruireNeutraliserDansProfondeur;

class EXTERN ASN1C_Mission_Automate_ALAT_DetruireNeutraliserDansProfondeur : public ASN1CType {
public:
   ASN1T_Mission_Automate_ALAT_DetruireNeutraliserDansProfondeur& msgData;
   ASN1C_Mission_Automate_ALAT_DetruireNeutraliserDansProfondeur (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ALAT_DetruireNeutraliserDansProfondeur& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ALAT_DetruireNeutraliserDansProfondeur (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_DetruireNeutraliserDansProfondeur* pvalue);
EXTERN int asn1PD_Mission_Automate_ALAT_DetruireNeutraliserDansProfondeur (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_DetruireNeutraliserDansProfondeur* pvalue);

EXTERN void asn1Print_Mission_Automate_ALAT_DetruireNeutraliserDansProfondeur (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ALAT_DetruireNeutraliserDansProfondeur* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ALAT_DetruireNeutraliserDansZone         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ALAT_DetruireNeutraliserDansZone {
   ASN1T_Polygon  zone;
   ASN1T_NatureAtlas  cibles_prioritaires;
   ASN1T_Point  point_regroupement;
   ASN1T_Point  point_dislocation;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;
   ASN1T_EnumMissionALAT_AmbianceMission  ambiance_mission;
   ASN1BOOL  neutraliser;

   ASN1T_Mission_Automate_ALAT_DetruireNeutraliserDansZone () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Automate_ALAT_DetruireNeutraliserDansZone;

class EXTERN ASN1C_Mission_Automate_ALAT_DetruireNeutraliserDansZone : public ASN1CType {
public:
   ASN1T_Mission_Automate_ALAT_DetruireNeutraliserDansZone& msgData;
   ASN1C_Mission_Automate_ALAT_DetruireNeutraliserDansZone (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ALAT_DetruireNeutraliserDansZone& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ALAT_DetruireNeutraliserDansZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_DetruireNeutraliserDansZone* pvalue);
EXTERN int asn1PD_Mission_Automate_ALAT_DetruireNeutraliserDansZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_DetruireNeutraliserDansZone* pvalue);

EXTERN void asn1Print_Mission_Automate_ALAT_DetruireNeutraliserDansZone (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ALAT_DetruireNeutraliserDansZone* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ALAT_EffectuerRechercheEtSauvetage       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ALAT_EffectuerRechercheEtSauvetage {
   ASN1T_ListAgent  unites_a_secourir;
   ASN1T_Point  point_regroupement;
   ASN1T_ListKnowledgeObject  plots_ravitaillement;
   ASN1BOOL  ravitaillement_debut_mission;
   ASN1T_EnumMissionALAT_PorteeAction  portee_action;

   ASN1T_Mission_Automate_ALAT_EffectuerRechercheEtSauvetage () {
      ravitaillement_debut_mission = TRUE;
   }
} ASN1T_Mission_Automate_ALAT_EffectuerRechercheEtSauvetage;

class EXTERN ASN1C_Mission_Automate_ALAT_EffectuerRechercheEtSauvetage : public ASN1CType {
public:
   ASN1T_Mission_Automate_ALAT_EffectuerRechercheEtSauvetage& msgData;
   ASN1C_Mission_Automate_ALAT_EffectuerRechercheEtSauvetage (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ALAT_EffectuerRechercheEtSauvetage& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ALAT_EffectuerRechercheEtSauvetage (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_EffectuerRechercheEtSauvetage* pvalue);
EXTERN int asn1PD_Mission_Automate_ALAT_EffectuerRechercheEtSauvetage (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ALAT_EffectuerRechercheEtSauvetage* pvalue);

EXTERN void asn1Print_Mission_Automate_ALAT_EffectuerRechercheEtSauvetage (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ALAT_EffectuerRechercheEtSauvetage* pvalue);

#endif

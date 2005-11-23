/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 22-Nov-2005.
 */
#ifndef SCIPIOUNITKNOWLEDGE_H
#define SCIPIOUNITKNOWLEDGE_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  MsgUnitKnowledgeCreation                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgUnitKnowledgeCreation {
   ASN1T_OID  oid_connaissance;
   ASN1T_OID  oid_groupe_possesseur;
   ASN1T_Agent  oid_unite_reelle;
} ASN1T_MsgUnitKnowledgeCreation;

class EXTERN ASN1C_MsgUnitKnowledgeCreation : public ASN1CType {
public:
   ASN1T_MsgUnitKnowledgeCreation& msgData;
   ASN1C_MsgUnitKnowledgeCreation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgUnitKnowledgeCreation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgUnitKnowledgeCreation (ASN1CTXT* ctxt_p, ASN1T_MsgUnitKnowledgeCreation* pvalue);
EXTERN int asn1PD_MsgUnitKnowledgeCreation (ASN1CTXT* ctxt_p, ASN1T_MsgUnitKnowledgeCreation* pvalue);

EXTERN void asn1Print_MsgUnitKnowledgeCreation (ASN1ConstCharPtr name, ASN1T_MsgUnitKnowledgeCreation* pvalue);

/**************************************************************/
/*                                                            */
/*  AutomatePerception                                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_AutomatePerception {
   ASN1T_OID  oid_compagnie;
   ASN1T_EnumUnitIdentificationLevel  identification_level;
} ASN1T_AutomatePerception;

class EXTERN ASN1C_AutomatePerception : public ASN1CType {
public:
   ASN1T_AutomatePerception& msgData;
   ASN1C_AutomatePerception (
      ASN1MessageBuffer& msgBuf, ASN1T_AutomatePerception& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_AutomatePerception (ASN1CTXT* ctxt_p, ASN1T_AutomatePerception* pvalue);
EXTERN int asn1PD_AutomatePerception (ASN1CTXT* ctxt_p, ASN1T_AutomatePerception* pvalue);

EXTERN void asn1Print_AutomatePerception (ASN1ConstCharPtr name, ASN1T_AutomatePerception* pvalue);

/**************************************************************/
/*                                                            */
/*  _SeqOfAutomatePerception                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T__SeqOfAutomatePerception {
   ASN1UINT n;
   ASN1T_AutomatePerception *elem;
} ASN1T__SeqOfAutomatePerception;

class EXTERN ASN1C__SeqOfAutomatePerception : public ASN1CType {
public:
   ASN1T__SeqOfAutomatePerception& msgData;
   ASN1C__SeqOfAutomatePerception (
      ASN1MessageBuffer& msgBuf, ASN1T__SeqOfAutomatePerception& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE__SeqOfAutomatePerception (ASN1CTXT* ctxt_p, ASN1T__SeqOfAutomatePerception* pvalue);
EXTERN int asn1PD__SeqOfAutomatePerception (ASN1CTXT* ctxt_p, ASN1T__SeqOfAutomatePerception* pvalue);

EXTERN void asn1Print__SeqOfAutomatePerception (ASN1ConstCharPtr name, ASN1T__SeqOfAutomatePerception* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgUnitKnowledgeUpdate                                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgUnitKnowledgeUpdate {
   struct {
      unsigned pertinencePresent : 1;
      unsigned identification_levelPresent : 1;
      unsigned max_identification_levelPresent : 1;
      unsigned etat_opPresent : 1;
      unsigned positionPresent : 1;
      unsigned directionPresent : 1;
      unsigned speedPresent : 1;
      unsigned campPresent : 1;
      unsigned nature_armePresent : 1;
      unsigned nature_niveauPresent : 1;
      unsigned nature_pcPresent : 1;
      unsigned nature_specialisationPresent : 1;
      unsigned nature_qualificationPresent : 1;
      unsigned nature_categoriePresent : 1;
      unsigned nature_mobilitePresent : 1;
      unsigned capacite_missionPresent : 1;
      unsigned perception_par_compagniePresent : 1;
      unsigned renduPresent : 1;
      unsigned prisonnierPresent : 1;
      unsigned refugie_pris_en_comptePresent : 1;
   } m;
   ASN1T_OID  oid_connaissance;
   ASN1T_OID  oid_groupe_possesseur;
   ASN1T_Pourcentage  pertinence;
   ASN1T_EnumUnitIdentificationLevel  identification_level;
   ASN1T_EnumUnitIdentificationLevel  max_identification_level;
   ASN1T_PourcentageLibre  etat_op;
   ASN1T_CoordUTM  position;
   ASN1T_Direction  direction;
   ASN1T_Vitesse  speed;
   ASN1T_OID  camp;
   ASN1T_EnumUnitNatureWeapon  nature_arme;
   ASN1T_EnumNatureLevel  nature_niveau;
   ASN1BOOL  nature_pc;
   ASN1T_EnumUnitNatureSpecialization  nature_specialisation;
   ASN1T_EnumUnitNatureQualifier  nature_qualification;
   ASN1T_EnumUnitNatureCategory  nature_categorie;
   ASN1T_EnumUnitNatureMobility  nature_mobilite;
   ASN1T_EnumUnitCapaciteMission  capacite_mission;
   ASN1T__SeqOfAutomatePerception  perception_par_compagnie;
   ASN1BOOL  rendu;
   ASN1BOOL  prisonnier;
   ASN1BOOL  refugie_pris_en_compte;

   ASN1T_MsgUnitKnowledgeUpdate () {
      m.pertinencePresent = 0;
      m.identification_levelPresent = 0;
      m.max_identification_levelPresent = 0;
      m.etat_opPresent = 0;
      m.positionPresent = 0;
      m.directionPresent = 0;
      m.speedPresent = 0;
      m.campPresent = 0;
      m.nature_armePresent = 0;
      m.nature_niveauPresent = 0;
      m.nature_pcPresent = 0;
      m.nature_specialisationPresent = 0;
      m.nature_qualificationPresent = 0;
      m.nature_categoriePresent = 0;
      m.nature_mobilitePresent = 0;
      m.capacite_missionPresent = 0;
      m.perception_par_compagniePresent = 0;
      m.renduPresent = 0;
      m.prisonnierPresent = 0;
      m.refugie_pris_en_comptePresent = 0;
   }
} ASN1T_MsgUnitKnowledgeUpdate;

class EXTERN ASN1C_MsgUnitKnowledgeUpdate : public ASN1CType {
public:
   ASN1T_MsgUnitKnowledgeUpdate& msgData;
   ASN1C_MsgUnitKnowledgeUpdate (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgUnitKnowledgeUpdate& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgUnitKnowledgeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgUnitKnowledgeUpdate* pvalue);
EXTERN int asn1PD_MsgUnitKnowledgeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgUnitKnowledgeUpdate* pvalue);

EXTERN void asn1Print_MsgUnitKnowledgeUpdate (ASN1ConstCharPtr name, ASN1T_MsgUnitKnowledgeUpdate* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgUnitKnowledgeDestruction                               */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgUnitKnowledgeDestruction {
   ASN1T_OID  oid_connaissance;
   ASN1T_OID  oid_groupe_possesseur;
} ASN1T_MsgUnitKnowledgeDestruction;

class EXTERN ASN1C_MsgUnitKnowledgeDestruction : public ASN1CType {
public:
   ASN1T_MsgUnitKnowledgeDestruction& msgData;
   ASN1C_MsgUnitKnowledgeDestruction (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgUnitKnowledgeDestruction& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgUnitKnowledgeDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgUnitKnowledgeDestruction* pvalue);
EXTERN int asn1PD_MsgUnitKnowledgeDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgUnitKnowledgeDestruction* pvalue);

EXTERN void asn1Print_MsgUnitKnowledgeDestruction (ASN1ConstCharPtr name, ASN1T_MsgUnitKnowledgeDestruction* pvalue);

#endif

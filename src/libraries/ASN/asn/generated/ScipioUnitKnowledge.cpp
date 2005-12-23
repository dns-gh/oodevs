/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 23-Dec-2005.
 */
#include "ScipioUnitKnowledge.h"

/**************************************************************/
/*                                                            */
/*  MsgUnitKnowledgeCreation                                  */
/*                                                            */
/**************************************************************/

ASN1C_MsgUnitKnowledgeCreation::ASN1C_MsgUnitKnowledgeCreation (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgUnitKnowledgeCreation& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgUnitKnowledgeCreation (ASN1CTXT* ctxt_p, ASN1T_MsgUnitKnowledgeCreation* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgUnitKnowledgeCreation: start\n");

   /* encode oid_connaissance */

   PU_PUSHNAME (ctxt_p, "oid_connaissance");

   stat = asn1PE_OID (ctxt_p, pvalue->oid_connaissance);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode oid_groupe_possesseur */

   PU_PUSHNAME (ctxt_p, "oid_groupe_possesseur");

   stat = asn1PE_OID (ctxt_p, pvalue->oid_groupe_possesseur);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode oid_unite_reelle */

   PU_PUSHNAME (ctxt_p, "oid_unite_reelle");

   stat = asn1PE_Agent (ctxt_p, pvalue->oid_unite_reelle);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_MsgUnitKnowledgeCreation: end\n");
   return (stat);
}

int ASN1C_MsgUnitKnowledgeCreation::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgUnitKnowledgeCreation (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgUnitKnowledgeCreation (ASN1CTXT* ctxt_p, ASN1T_MsgUnitKnowledgeCreation* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgUnitKnowledgeCreation: start\n");

   /* decode oid_connaissance */

   PU_PUSHNAME (ctxt_p, "oid_connaissance");

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_connaissance);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode oid_groupe_possesseur */

   PU_PUSHNAME (ctxt_p, "oid_groupe_possesseur");

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_groupe_possesseur);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode oid_unite_reelle */

   PU_PUSHNAME (ctxt_p, "oid_unite_reelle");

   stat = asn1PD_Agent (ctxt_p, &pvalue->oid_unite_reelle);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_MsgUnitKnowledgeCreation: end\n");

   return (stat);
}

int ASN1C_MsgUnitKnowledgeCreation::Decode ()
{
   return asn1PD_MsgUnitKnowledgeCreation (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  AutomatePerception                                        */
/*                                                            */
/**************************************************************/

ASN1C_AutomatePerception::ASN1C_AutomatePerception (
   ASN1MessageBuffer& msgBuf, ASN1T_AutomatePerception& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_AutomatePerception (ASN1CTXT* ctxt_p, ASN1T_AutomatePerception* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_AutomatePerception: start\n");

   /* encode oid_compagnie */

   PU_PUSHNAME (ctxt_p, "oid_compagnie");

   stat = asn1PE_OID (ctxt_p, pvalue->oid_compagnie);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode identification_level */

   PU_PUSHNAME (ctxt_p, "identification_level");

   stat = asn1PE_EnumUnitIdentificationLevel (ctxt_p, pvalue->identification_level);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_AutomatePerception: end\n");
   return (stat);
}

int ASN1C_AutomatePerception::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_AutomatePerception (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_AutomatePerception (ASN1CTXT* ctxt_p, ASN1T_AutomatePerception* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_AutomatePerception: start\n");

   /* decode oid_compagnie */

   PU_PUSHNAME (ctxt_p, "oid_compagnie");

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_compagnie);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode identification_level */

   PU_PUSHNAME (ctxt_p, "identification_level");

   stat = asn1PD_EnumUnitIdentificationLevel (ctxt_p, &pvalue->identification_level);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_AutomatePerception: end\n");

   return (stat);
}

int ASN1C_AutomatePerception::Decode ()
{
   return asn1PD_AutomatePerception (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfAutomatePerception                                  */
/*                                                            */
/**************************************************************/

ASN1C__SeqOfAutomatePerception::ASN1C__SeqOfAutomatePerception (
   ASN1MessageBuffer& msgBuf, ASN1T__SeqOfAutomatePerception& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE__SeqOfAutomatePerception (ASN1CTXT* ctxt_p, ASN1T__SeqOfAutomatePerception* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   rtdiag ("asn1PE__SeqOfAutomatePerception: start\n");

   /* encode length determinant */

   PU_PUSHNAME (ctxt_p, "n");

   stat = pe_Length (ctxt_p, pvalue->n);
   if (stat < 0) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* encode elements */

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      PU_PUSHELEMNAME (ctxt_p, xx1);

      stat = asn1PE_AutomatePerception (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   rtdiag ("asn1PE__SeqOfAutomatePerception: end\n");
   return (stat);
}

int ASN1C__SeqOfAutomatePerception::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE__SeqOfAutomatePerception (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD__SeqOfAutomatePerception (ASN1CTXT* ctxt_p, ASN1T__SeqOfAutomatePerception* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   rtdiag ("asn1PD__SeqOfAutomatePerception: start\n");

   /* decode length determinant */

   PU_PUSHNAME (ctxt_p, "n");

   stat = pd_Length (ctxt_p, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode elements */

   ALLOC_ASN1ARRAY (ctxt_p, pvalue, ASN1T_AutomatePerception);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      PU_PUSHELEMNAME (ctxt_p, xx1);

      stat = asn1PD_AutomatePerception (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }


   rtdiag ("asn1PD__SeqOfAutomatePerception: end\n");

   return (stat);
}

int ASN1C__SeqOfAutomatePerception::Decode ()
{
   return asn1PD__SeqOfAutomatePerception (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgUnitKnowledgeUpdate                                    */
/*                                                            */
/**************************************************************/

ASN1C_MsgUnitKnowledgeUpdate::ASN1C_MsgUnitKnowledgeUpdate (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgUnitKnowledgeUpdate& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgUnitKnowledgeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgUnitKnowledgeUpdate* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgUnitKnowledgeUpdate: start\n");

   PU_NEWFIELD (ctxt_p, "pertinencePresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.pertinencePresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "identification_levelPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.identification_levelPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "max_identification_levelPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.max_identification_levelPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "etat_opPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.etat_opPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "positionPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.positionPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "directionPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.directionPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "speedPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.speedPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "campPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.campPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "nature_armePresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.nature_armePresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "nature_niveauPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.nature_niveauPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "nature_pcPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.nature_pcPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "nature_specialisationPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.nature_specialisationPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "nature_qualificationPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.nature_qualificationPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "nature_categoriePresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.nature_categoriePresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "nature_mobilitePresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.nature_mobilitePresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "capacite_missionPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.capacite_missionPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "perception_par_compagniePresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.perception_par_compagniePresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "renduPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.renduPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "prisonnierPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.prisonnierPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "refugie_pris_en_comptePresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.refugie_pris_en_comptePresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   /* encode oid_connaissance */

   PU_PUSHNAME (ctxt_p, "oid_connaissance");

   stat = asn1PE_OID (ctxt_p, pvalue->oid_connaissance);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode oid_groupe_possesseur */

   PU_PUSHNAME (ctxt_p, "oid_groupe_possesseur");

   stat = asn1PE_OID (ctxt_p, pvalue->oid_groupe_possesseur);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode pertinence */

   if (pvalue->m.pertinencePresent) {
      PU_PUSHNAME (ctxt_p, "pertinence");

      stat = asn1PE_Pourcentage (ctxt_p, pvalue->pertinence);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode identification_level */

   if (pvalue->m.identification_levelPresent) {
      PU_PUSHNAME (ctxt_p, "identification_level");

      stat = asn1PE_EnumUnitIdentificationLevel (ctxt_p, pvalue->identification_level);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode max_identification_level */

   if (pvalue->m.max_identification_levelPresent) {
      PU_PUSHNAME (ctxt_p, "max_identification_level");

      stat = asn1PE_EnumUnitIdentificationLevel (ctxt_p, pvalue->max_identification_level);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode etat_op */

   if (pvalue->m.etat_opPresent) {
      PU_PUSHNAME (ctxt_p, "etat_op");

      stat = asn1PE_PourcentageLibre (ctxt_p, pvalue->etat_op);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode position */

   if (pvalue->m.positionPresent) {
      PU_PUSHNAME (ctxt_p, "position");

      stat = asn1PE_CoordUTM (ctxt_p, pvalue->position);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode direction */

   if (pvalue->m.directionPresent) {
      PU_PUSHNAME (ctxt_p, "direction");

      stat = asn1PE_Direction (ctxt_p, pvalue->direction);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode speed */

   if (pvalue->m.speedPresent) {
      PU_PUSHNAME (ctxt_p, "speed");

      stat = asn1PE_Vitesse (ctxt_p, pvalue->speed);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode camp */

   if (pvalue->m.campPresent) {
      PU_PUSHNAME (ctxt_p, "camp");

      stat = asn1PE_OID (ctxt_p, pvalue->camp);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode nature_arme */

   if (pvalue->m.nature_armePresent) {
      PU_PUSHNAME (ctxt_p, "nature_arme");

      stat = asn1PE_EnumUnitNatureWeapon (ctxt_p, pvalue->nature_arme);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode nature_niveau */

   if (pvalue->m.nature_niveauPresent) {
      PU_PUSHNAME (ctxt_p, "nature_niveau");

      stat = asn1PE_EnumNatureLevel (ctxt_p, pvalue->nature_niveau);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode nature_pc */

   if (pvalue->m.nature_pcPresent) {
      PU_PUSHNAME (ctxt_p, "nature_pc");

      PU_NEWFIELD (ctxt_p, "boolean");

      stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->nature_pc);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_SETBITCOUNT (ctxt_p);
      PU_POPNAME (ctxt_p);
   }

   /* encode nature_specialisation */

   if (pvalue->m.nature_specialisationPresent) {
      PU_PUSHNAME (ctxt_p, "nature_specialisation");

      stat = asn1PE_EnumUnitNatureSpecialization (ctxt_p, pvalue->nature_specialisation);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode nature_qualification */

   if (pvalue->m.nature_qualificationPresent) {
      PU_PUSHNAME (ctxt_p, "nature_qualification");

      stat = asn1PE_EnumUnitNatureQualifier (ctxt_p, pvalue->nature_qualification);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode nature_categorie */

   if (pvalue->m.nature_categoriePresent) {
      PU_PUSHNAME (ctxt_p, "nature_categorie");

      stat = asn1PE_EnumUnitNatureCategory (ctxt_p, pvalue->nature_categorie);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode nature_mobilite */

   if (pvalue->m.nature_mobilitePresent) {
      PU_PUSHNAME (ctxt_p, "nature_mobilite");

      stat = asn1PE_EnumUnitNatureMobility (ctxt_p, pvalue->nature_mobilite);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode capacite_mission */

   if (pvalue->m.capacite_missionPresent) {
      PU_PUSHNAME (ctxt_p, "capacite_mission");

      stat = asn1PE_EnumUnitCapaciteMission (ctxt_p, pvalue->capacite_mission);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode perception_par_compagnie */

   if (pvalue->m.perception_par_compagniePresent) {
      PU_PUSHNAME (ctxt_p, "perception_par_compagnie");

      stat = asn1PE__SeqOfAutomatePerception (ctxt_p, &pvalue->perception_par_compagnie);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode rendu */

   if (pvalue->m.renduPresent) {
      PU_PUSHNAME (ctxt_p, "rendu");

      PU_NEWFIELD (ctxt_p, "boolean");

      stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->rendu);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_SETBITCOUNT (ctxt_p);
      PU_POPNAME (ctxt_p);
   }

   /* encode prisonnier */

   if (pvalue->m.prisonnierPresent) {
      PU_PUSHNAME (ctxt_p, "prisonnier");

      PU_NEWFIELD (ctxt_p, "boolean");

      stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->prisonnier);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_SETBITCOUNT (ctxt_p);
      PU_POPNAME (ctxt_p);
   }

   /* encode refugie_pris_en_compte */

   if (pvalue->m.refugie_pris_en_comptePresent) {
      PU_PUSHNAME (ctxt_p, "refugie_pris_en_compte");

      PU_NEWFIELD (ctxt_p, "boolean");

      stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->refugie_pris_en_compte);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_SETBITCOUNT (ctxt_p);
      PU_POPNAME (ctxt_p);
   }


   rtdiag ("asn1PE_MsgUnitKnowledgeUpdate: end\n");
   return (stat);
}

int ASN1C_MsgUnitKnowledgeUpdate::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgUnitKnowledgeUpdate (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgUnitKnowledgeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgUnitKnowledgeUpdate* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL optbit;

   rtdiag ("asn1PD_MsgUnitKnowledgeUpdate: start\n");

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   PU_NEWFIELD (ctxt_p, "pertinencePresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.pertinencePresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "identification_levelPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.identification_levelPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "max_identification_levelPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.max_identification_levelPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "etat_opPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.etat_opPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "positionPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.positionPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "directionPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.directionPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "speedPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.speedPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "campPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.campPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "nature_armePresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.nature_armePresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "nature_niveauPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.nature_niveauPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "nature_pcPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.nature_pcPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "nature_specialisationPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.nature_specialisationPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "nature_qualificationPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.nature_qualificationPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "nature_categoriePresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.nature_categoriePresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "nature_mobilitePresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.nature_mobilitePresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "capacite_missionPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.capacite_missionPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "perception_par_compagniePresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.perception_par_compagniePresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "renduPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.renduPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "prisonnierPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.prisonnierPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "refugie_pris_en_comptePresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.refugie_pris_en_comptePresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   /* decode oid_connaissance */

   PU_PUSHNAME (ctxt_p, "oid_connaissance");

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_connaissance);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode oid_groupe_possesseur */

   PU_PUSHNAME (ctxt_p, "oid_groupe_possesseur");

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_groupe_possesseur);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode pertinence */

   if (pvalue->m.pertinencePresent) {
      PU_PUSHNAME (ctxt_p, "pertinence");

      stat = asn1PD_Pourcentage (ctxt_p, &pvalue->pertinence);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode identification_level */

   if (pvalue->m.identification_levelPresent) {
      PU_PUSHNAME (ctxt_p, "identification_level");

      stat = asn1PD_EnumUnitIdentificationLevel (ctxt_p, &pvalue->identification_level);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode max_identification_level */

   if (pvalue->m.max_identification_levelPresent) {
      PU_PUSHNAME (ctxt_p, "max_identification_level");

      stat = asn1PD_EnumUnitIdentificationLevel (ctxt_p, &pvalue->max_identification_level);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode etat_op */

   if (pvalue->m.etat_opPresent) {
      PU_PUSHNAME (ctxt_p, "etat_op");

      stat = asn1PD_PourcentageLibre (ctxt_p, &pvalue->etat_op);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode position */

   if (pvalue->m.positionPresent) {
      PU_PUSHNAME (ctxt_p, "position");

      stat = asn1PD_CoordUTM (ctxt_p, &pvalue->position);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode direction */

   if (pvalue->m.directionPresent) {
      PU_PUSHNAME (ctxt_p, "direction");

      stat = asn1PD_Direction (ctxt_p, &pvalue->direction);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode speed */

   if (pvalue->m.speedPresent) {
      PU_PUSHNAME (ctxt_p, "speed");

      stat = asn1PD_Vitesse (ctxt_p, &pvalue->speed);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode camp */

   if (pvalue->m.campPresent) {
      PU_PUSHNAME (ctxt_p, "camp");

      stat = asn1PD_OID (ctxt_p, &pvalue->camp);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode nature_arme */

   if (pvalue->m.nature_armePresent) {
      PU_PUSHNAME (ctxt_p, "nature_arme");

      stat = asn1PD_EnumUnitNatureWeapon (ctxt_p, &pvalue->nature_arme);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode nature_niveau */

   if (pvalue->m.nature_niveauPresent) {
      PU_PUSHNAME (ctxt_p, "nature_niveau");

      stat = asn1PD_EnumNatureLevel (ctxt_p, &pvalue->nature_niveau);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode nature_pc */

   if (pvalue->m.nature_pcPresent) {
      PU_PUSHNAME (ctxt_p, "nature_pc");

      PU_NEWFIELD (ctxt_p, "boolean");

      stat = pd_bit (ctxt_p, &pvalue->nature_pc);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_SETBITCOUNT (ctxt_p);

      PU_POPNAME (ctxt_p);
   }

   /* decode nature_specialisation */

   if (pvalue->m.nature_specialisationPresent) {
      PU_PUSHNAME (ctxt_p, "nature_specialisation");

      stat = asn1PD_EnumUnitNatureSpecialization (ctxt_p, &pvalue->nature_specialisation);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode nature_qualification */

   if (pvalue->m.nature_qualificationPresent) {
      PU_PUSHNAME (ctxt_p, "nature_qualification");

      stat = asn1PD_EnumUnitNatureQualifier (ctxt_p, &pvalue->nature_qualification);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode nature_categorie */

   if (pvalue->m.nature_categoriePresent) {
      PU_PUSHNAME (ctxt_p, "nature_categorie");

      stat = asn1PD_EnumUnitNatureCategory (ctxt_p, &pvalue->nature_categorie);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode nature_mobilite */

   if (pvalue->m.nature_mobilitePresent) {
      PU_PUSHNAME (ctxt_p, "nature_mobilite");

      stat = asn1PD_EnumUnitNatureMobility (ctxt_p, &pvalue->nature_mobilite);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode capacite_mission */

   if (pvalue->m.capacite_missionPresent) {
      PU_PUSHNAME (ctxt_p, "capacite_mission");

      stat = asn1PD_EnumUnitCapaciteMission (ctxt_p, &pvalue->capacite_mission);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode perception_par_compagnie */

   if (pvalue->m.perception_par_compagniePresent) {
      PU_PUSHNAME (ctxt_p, "perception_par_compagnie");

      stat = asn1PD__SeqOfAutomatePerception (ctxt_p, &pvalue->perception_par_compagnie);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode rendu */

   if (pvalue->m.renduPresent) {
      PU_PUSHNAME (ctxt_p, "rendu");

      PU_NEWFIELD (ctxt_p, "boolean");

      stat = pd_bit (ctxt_p, &pvalue->rendu);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_SETBITCOUNT (ctxt_p);

      PU_POPNAME (ctxt_p);
   }

   /* decode prisonnier */

   if (pvalue->m.prisonnierPresent) {
      PU_PUSHNAME (ctxt_p, "prisonnier");

      PU_NEWFIELD (ctxt_p, "boolean");

      stat = pd_bit (ctxt_p, &pvalue->prisonnier);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_SETBITCOUNT (ctxt_p);

      PU_POPNAME (ctxt_p);
   }

   /* decode refugie_pris_en_compte */

   if (pvalue->m.refugie_pris_en_comptePresent) {
      PU_PUSHNAME (ctxt_p, "refugie_pris_en_compte");

      PU_NEWFIELD (ctxt_p, "boolean");

      stat = pd_bit (ctxt_p, &pvalue->refugie_pris_en_compte);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_SETBITCOUNT (ctxt_p);

      PU_POPNAME (ctxt_p);
   }


   rtdiag ("asn1PD_MsgUnitKnowledgeUpdate: end\n");

   return (stat);
}

int ASN1C_MsgUnitKnowledgeUpdate::Decode ()
{
   return asn1PD_MsgUnitKnowledgeUpdate (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgUnitKnowledgeDestruction                               */
/*                                                            */
/**************************************************************/

ASN1C_MsgUnitKnowledgeDestruction::ASN1C_MsgUnitKnowledgeDestruction (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgUnitKnowledgeDestruction& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgUnitKnowledgeDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgUnitKnowledgeDestruction* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgUnitKnowledgeDestruction: start\n");

   /* encode oid_connaissance */

   PU_PUSHNAME (ctxt_p, "oid_connaissance");

   stat = asn1PE_OID (ctxt_p, pvalue->oid_connaissance);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode oid_groupe_possesseur */

   PU_PUSHNAME (ctxt_p, "oid_groupe_possesseur");

   stat = asn1PE_OID (ctxt_p, pvalue->oid_groupe_possesseur);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_MsgUnitKnowledgeDestruction: end\n");
   return (stat);
}

int ASN1C_MsgUnitKnowledgeDestruction::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgUnitKnowledgeDestruction (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgUnitKnowledgeDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgUnitKnowledgeDestruction* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgUnitKnowledgeDestruction: start\n");

   /* decode oid_connaissance */

   PU_PUSHNAME (ctxt_p, "oid_connaissance");

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_connaissance);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode oid_groupe_possesseur */

   PU_PUSHNAME (ctxt_p, "oid_groupe_possesseur");

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_groupe_possesseur);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_MsgUnitKnowledgeDestruction: end\n");

   return (stat);
}

int ASN1C_MsgUnitKnowledgeDestruction::Decode ()
{
   return asn1PD_MsgUnitKnowledgeDestruction (mpContext->GetPtr(), &msgData);
}


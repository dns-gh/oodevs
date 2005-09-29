/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 29-Sep-2005.
 */
#include "ScipioObjectKnowledge.h"

/**************************************************************/
/*                                                            */
/*  MsgObjectKnowledgeUpdate                                  */
/*                                                            */
/**************************************************************/

ASN1C_MsgObjectKnowledgeUpdate::ASN1C_MsgObjectKnowledgeUpdate (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgObjectKnowledgeUpdate& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgObjectKnowledgeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgObjectKnowledgeUpdate* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgObjectKnowledgeUpdate: start\n");

   PU_NEWFIELD (ctxt_p, "oid_objet_reelPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.oid_objet_reelPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "pertinencePresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.pertinencePresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "localisationPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.localisationPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "pourcentage_constructionPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.pourcentage_constructionPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "pourcentage_valorisationPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.pourcentage_valorisationPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "pourcentage_contournementPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.pourcentage_contournementPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "en_preparationPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.en_preparationPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "est_percuPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.est_percuPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "attributs_specifiquesPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.attributs_specifiquesPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "perception_par_compagniePresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.perception_par_compagniePresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   /* encode oid_connaissance */

   PU_PUSHNAME (ctxt_p, "oid_connaissance");

   stat = asn1PE_OID (ctxt_p, pvalue->oid_connaissance);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode oid_camp_possesseur */

   PU_PUSHNAME (ctxt_p, "oid_camp_possesseur");

   stat = asn1PE_OID (ctxt_p, pvalue->oid_camp_possesseur);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode oid_objet_reel */

   if (pvalue->m.oid_objet_reelPresent) {
      PU_PUSHNAME (ctxt_p, "oid_objet_reel");

      stat = asn1PE_OID (ctxt_p, pvalue->oid_objet_reel);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode pertinence */

   if (pvalue->m.pertinencePresent) {
      PU_PUSHNAME (ctxt_p, "pertinence");

      if ( (pvalue->pertinence >= 0 && pvalue->pertinence <= 100) ) {
         stat = pe_ConsInteger (ctxt_p, pvalue->pertinence, 0, 100);
         if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      }
      else
         return LOG_ASN1ERR (ctxt_p, ASN_E_CONSVIO);

      PU_POPNAME (ctxt_p);
   }

   /* encode localisation */

   if (pvalue->m.localisationPresent) {
      PU_PUSHNAME (ctxt_p, "localisation");

      stat = asn1PE_Localisation (ctxt_p, &pvalue->localisation);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode pourcentage_construction */

   if (pvalue->m.pourcentage_constructionPresent) {
      PU_PUSHNAME (ctxt_p, "pourcentage_construction");

      if ( (pvalue->pourcentage_construction >= 0 && pvalue->pourcentage_construction <= 100) ) {
         stat = pe_ConsInteger (ctxt_p, pvalue->pourcentage_construction, 0, 100);
         if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      }
      else
         return LOG_ASN1ERR (ctxt_p, ASN_E_CONSVIO);

      PU_POPNAME (ctxt_p);
   }

   /* encode pourcentage_valorisation */

   if (pvalue->m.pourcentage_valorisationPresent) {
      PU_PUSHNAME (ctxt_p, "pourcentage_valorisation");

      if ( (pvalue->pourcentage_valorisation >= 0 && pvalue->pourcentage_valorisation <= 100) ) {
         stat = pe_ConsInteger (ctxt_p, pvalue->pourcentage_valorisation, 0, 100);
         if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      }
      else
         return LOG_ASN1ERR (ctxt_p, ASN_E_CONSVIO);

      PU_POPNAME (ctxt_p);
   }

   /* encode pourcentage_contournement */

   if (pvalue->m.pourcentage_contournementPresent) {
      PU_PUSHNAME (ctxt_p, "pourcentage_contournement");

      if ( (pvalue->pourcentage_contournement >= 0 && pvalue->pourcentage_contournement <= 100) ) {
         stat = pe_ConsInteger (ctxt_p, pvalue->pourcentage_contournement, 0, 100);
         if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      }
      else
         return LOG_ASN1ERR (ctxt_p, ASN_E_CONSVIO);

      PU_POPNAME (ctxt_p);
   }

   /* encode en_preparation */

   if (pvalue->m.en_preparationPresent) {
      PU_PUSHNAME (ctxt_p, "en_preparation");

      PU_NEWFIELD (ctxt_p, "boolean");

      stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->en_preparation);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_SETBITCOUNT (ctxt_p);
      PU_POPNAME (ctxt_p);
   }

   /* encode est_percu */

   if (pvalue->m.est_percuPresent) {
      PU_PUSHNAME (ctxt_p, "est_percu");

      PU_NEWFIELD (ctxt_p, "boolean");

      stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->est_percu);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_SETBITCOUNT (ctxt_p);
      PU_POPNAME (ctxt_p);
   }

   /* encode attributs_specifiques */

   if (pvalue->m.attributs_specifiquesPresent) {
      PU_PUSHNAME (ctxt_p, "attributs_specifiques");

      stat = asn1PE_AttrObjectSpecific (ctxt_p, &pvalue->attributs_specifiques);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode perception_par_compagnie */

   if (pvalue->m.perception_par_compagniePresent) {
      PU_PUSHNAME (ctxt_p, "perception_par_compagnie");

      stat = asn1PE__SeqOfOID (ctxt_p, &pvalue->perception_par_compagnie);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }


   rtdiag ("asn1PE_MsgObjectKnowledgeUpdate: end\n");
   return (stat);
}

int ASN1C_MsgObjectKnowledgeUpdate::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgObjectKnowledgeUpdate (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgObjectKnowledgeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgObjectKnowledgeUpdate* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL optbit;

   rtdiag ("asn1PD_MsgObjectKnowledgeUpdate: start\n");

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   PU_NEWFIELD (ctxt_p, "oid_objet_reelPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.oid_objet_reelPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "pertinencePresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.pertinencePresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "localisationPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.localisationPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "pourcentage_constructionPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.pourcentage_constructionPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "pourcentage_valorisationPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.pourcentage_valorisationPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "pourcentage_contournementPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.pourcentage_contournementPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "en_preparationPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.en_preparationPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "est_percuPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.est_percuPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "attributs_specifiquesPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.attributs_specifiquesPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "perception_par_compagniePresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.perception_par_compagniePresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   /* decode oid_connaissance */

   PU_PUSHNAME (ctxt_p, "oid_connaissance");

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_connaissance);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode oid_camp_possesseur */

   PU_PUSHNAME (ctxt_p, "oid_camp_possesseur");

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_camp_possesseur);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode oid_objet_reel */

   if (pvalue->m.oid_objet_reelPresent) {
      PU_PUSHNAME (ctxt_p, "oid_objet_reel");

      stat = asn1PD_OID (ctxt_p, &pvalue->oid_objet_reel);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode pertinence */

   if (pvalue->m.pertinencePresent) {
      PU_PUSHNAME (ctxt_p, "pertinence");

      stat = pd_ConsInteger (ctxt_p, &pvalue->pertinence, 0, 100);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode localisation */

   if (pvalue->m.localisationPresent) {
      PU_PUSHNAME (ctxt_p, "localisation");

      stat = asn1PD_Localisation (ctxt_p, &pvalue->localisation);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode pourcentage_construction */

   if (pvalue->m.pourcentage_constructionPresent) {
      PU_PUSHNAME (ctxt_p, "pourcentage_construction");

      stat = pd_ConsInteger (ctxt_p, &pvalue->pourcentage_construction, 0, 100);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode pourcentage_valorisation */

   if (pvalue->m.pourcentage_valorisationPresent) {
      PU_PUSHNAME (ctxt_p, "pourcentage_valorisation");

      stat = pd_ConsInteger (ctxt_p, &pvalue->pourcentage_valorisation, 0, 100);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode pourcentage_contournement */

   if (pvalue->m.pourcentage_contournementPresent) {
      PU_PUSHNAME (ctxt_p, "pourcentage_contournement");

      stat = pd_ConsInteger (ctxt_p, &pvalue->pourcentage_contournement, 0, 100);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode en_preparation */

   if (pvalue->m.en_preparationPresent) {
      PU_PUSHNAME (ctxt_p, "en_preparation");

      PU_NEWFIELD (ctxt_p, "boolean");

      stat = pd_bit (ctxt_p, &pvalue->en_preparation);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_SETBITCOUNT (ctxt_p);

      PU_POPNAME (ctxt_p);
   }

   /* decode est_percu */

   if (pvalue->m.est_percuPresent) {
      PU_PUSHNAME (ctxt_p, "est_percu");

      PU_NEWFIELD (ctxt_p, "boolean");

      stat = pd_bit (ctxt_p, &pvalue->est_percu);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_SETBITCOUNT (ctxt_p);

      PU_POPNAME (ctxt_p);
   }

   /* decode attributs_specifiques */

   if (pvalue->m.attributs_specifiquesPresent) {
      PU_PUSHNAME (ctxt_p, "attributs_specifiques");

      stat = asn1PD_AttrObjectSpecific (ctxt_p, &pvalue->attributs_specifiques);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode perception_par_compagnie */

   if (pvalue->m.perception_par_compagniePresent) {
      PU_PUSHNAME (ctxt_p, "perception_par_compagnie");

      stat = asn1PD__SeqOfOID (ctxt_p, &pvalue->perception_par_compagnie);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }


   rtdiag ("asn1PD_MsgObjectKnowledgeUpdate: end\n");

   return (stat);
}

int ASN1C_MsgObjectKnowledgeUpdate::Decode ()
{
   return asn1PD_MsgObjectKnowledgeUpdate (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgObjectKnowledgeDestruction                             */
/*                                                            */
/**************************************************************/

ASN1C_MsgObjectKnowledgeDestruction::ASN1C_MsgObjectKnowledgeDestruction (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgObjectKnowledgeDestruction& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgObjectKnowledgeDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgObjectKnowledgeDestruction* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgObjectKnowledgeDestruction: start\n");

   /* encode oid_connaissance */

   PU_PUSHNAME (ctxt_p, "oid_connaissance");

   stat = asn1PE_OID (ctxt_p, pvalue->oid_connaissance);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode oid_camp_possesseur */

   PU_PUSHNAME (ctxt_p, "oid_camp_possesseur");

   stat = asn1PE_OID (ctxt_p, pvalue->oid_camp_possesseur);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_MsgObjectKnowledgeDestruction: end\n");
   return (stat);
}

int ASN1C_MsgObjectKnowledgeDestruction::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgObjectKnowledgeDestruction (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgObjectKnowledgeDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgObjectKnowledgeDestruction* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgObjectKnowledgeDestruction: start\n");

   /* decode oid_connaissance */

   PU_PUSHNAME (ctxt_p, "oid_connaissance");

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_connaissance);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode oid_camp_possesseur */

   PU_PUSHNAME (ctxt_p, "oid_camp_possesseur");

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_camp_possesseur);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_MsgObjectKnowledgeDestruction: end\n");

   return (stat);
}

int ASN1C_MsgObjectKnowledgeDestruction::Decode ()
{
   return asn1PD_MsgObjectKnowledgeDestruction (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgObjectKnowledgeCreation                                */
/*                                                            */
/**************************************************************/

ASN1C_MsgObjectKnowledgeCreation::ASN1C_MsgObjectKnowledgeCreation (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgObjectKnowledgeCreation& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgObjectKnowledgeCreation (ASN1CTXT* ctxt_p, ASN1T_MsgObjectKnowledgeCreation* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgObjectKnowledgeCreation: start\n");

   /* encode oid_connaissance */

   PU_PUSHNAME (ctxt_p, "oid_connaissance");

   stat = asn1PE_OID (ctxt_p, pvalue->oid_connaissance);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode oid_camp_possesseur */

   PU_PUSHNAME (ctxt_p, "oid_camp_possesseur");

   stat = asn1PE_OID (ctxt_p, pvalue->oid_camp_possesseur);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode oid_objet_reel */

   PU_PUSHNAME (ctxt_p, "oid_objet_reel");

   stat = asn1PE_OID (ctxt_p, pvalue->oid_objet_reel);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode type */

   PU_PUSHNAME (ctxt_p, "type");

   stat = asn1PE_EnumObjectType (ctxt_p, pvalue->type);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_MsgObjectKnowledgeCreation: end\n");
   return (stat);
}

int ASN1C_MsgObjectKnowledgeCreation::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgObjectKnowledgeCreation (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgObjectKnowledgeCreation (ASN1CTXT* ctxt_p, ASN1T_MsgObjectKnowledgeCreation* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgObjectKnowledgeCreation: start\n");

   /* decode oid_connaissance */

   PU_PUSHNAME (ctxt_p, "oid_connaissance");

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_connaissance);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode oid_camp_possesseur */

   PU_PUSHNAME (ctxt_p, "oid_camp_possesseur");

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_camp_possesseur);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode oid_objet_reel */

   PU_PUSHNAME (ctxt_p, "oid_objet_reel");

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_objet_reel);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode type */

   PU_PUSHNAME (ctxt_p, "type");

   stat = asn1PD_EnumObjectType (ctxt_p, &pvalue->type);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_MsgObjectKnowledgeCreation: end\n");

   return (stat);
}

int ASN1C_MsgObjectKnowledgeCreation::Decode ()
{
   return asn1PD_MsgObjectKnowledgeCreation (mpContext->GetPtr(), &msgData);
}


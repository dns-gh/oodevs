/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 02-Apr-2008.
 */
#include "PopulationKnowledge.h"

/**************************************************************/
/*                                                            */
/*  PopulationKnowledgeIncludes                               */
/*                                                            */
/**************************************************************/

ASN1C_PopulationKnowledgeIncludes::ASN1C_PopulationKnowledgeIncludes (
   ASN1MessageBuffer& msgBuf, ASN1T_PopulationKnowledgeIncludes& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_PopulationKnowledgeIncludes (ASN1CTXT* ctxt_p, ASN1T_PopulationKnowledgeIncludes* pvalue)
{
   int stat = ASN_OK;

   /* encode baseExports */

   stat = asn1PE_BaseExports (ctxt_p);
   if (stat != ASN_OK) return stat;

   /* encode populationExports */

   stat = asn1PE_PopulationExports (ctxt_p);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_PopulationKnowledgeIncludes::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_PopulationKnowledgeIncludes (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_PopulationKnowledgeIncludes (ASN1CTXT* ctxt_p, ASN1T_PopulationKnowledgeIncludes* pvalue)
{
   int stat = ASN_OK;

   /* decode baseExports */

   stat = asn1PD_BaseExports (ctxt_p);
   if (stat != ASN_OK) return stat;

   /* decode populationExports */

   stat = asn1PD_PopulationExports (ctxt_p);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_PopulationKnowledgeIncludes::Decode ()
{
   return asn1PD_PopulationKnowledgeIncludes (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  PopulationKnowledgeExports                                */
/*                                                            */
/**************************************************************/

ASN1C_PopulationKnowledgeExports::ASN1C_PopulationKnowledgeExports (
   ASN1MessageBuffer& msgBuf) :
   ASN1CType(msgBuf)
{}

EXTERN int asn1PE_PopulationKnowledgeExports (ASN1CTXT* ctxt_p)
{
   int stat = ASN_OK;

   /* NULL */
   return (stat);
}

int ASN1C_PopulationKnowledgeExports::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_PopulationKnowledgeExports (mpContext->GetPtr());
   return stat;
}

EXTERN int asn1PD_PopulationKnowledgeExports (ASN1CTXT* ctxt_p)
{
   int stat = ASN_OK;

   /* NULL */

   return (stat);
}

int ASN1C_PopulationKnowledgeExports::Decode ()
{
   return asn1PD_PopulationKnowledgeExports (mpContext->GetPtr());
}

/**************************************************************/
/*                                                            */
/*  MsgPopulationKnowledgeCreation                            */
/*                                                            */
/**************************************************************/

ASN1C_MsgPopulationKnowledgeCreation::ASN1C_MsgPopulationKnowledgeCreation (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationKnowledgeCreation& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgPopulationKnowledgeCreation (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationKnowledgeCreation* pvalue)
{
   int stat = ASN_OK;

   /* encode oid_connaissance */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_connaissance);
   if (stat != ASN_OK) return stat;

   /* encode oid_groupe_possesseur */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_groupe_possesseur);
   if (stat != ASN_OK) return stat;

   /* encode oid_population_reelle */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_population_reelle);
   if (stat != ASN_OK) return stat;

   /* encode camp */

   stat = asn1PE_OID (ctxt_p, pvalue->camp);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgPopulationKnowledgeCreation::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgPopulationKnowledgeCreation (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgPopulationKnowledgeCreation (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationKnowledgeCreation* pvalue)
{
   int stat = ASN_OK;

   /* decode oid_connaissance */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_connaissance);
   if (stat != ASN_OK) return stat;

   /* decode oid_groupe_possesseur */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_groupe_possesseur);
   if (stat != ASN_OK) return stat;

   /* decode oid_population_reelle */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_population_reelle);
   if (stat != ASN_OK) return stat;

   /* decode camp */

   stat = asn1PD_OID (ctxt_p, &pvalue->camp);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgPopulationKnowledgeCreation::Decode ()
{
   return asn1PD_MsgPopulationKnowledgeCreation (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgPopulationKnowledgeUpdate                              */
/*                                                            */
/**************************************************************/

ASN1C_MsgPopulationKnowledgeUpdate::ASN1C_MsgPopulationKnowledgeUpdate (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationKnowledgeUpdate& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgPopulationKnowledgeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationKnowledgeUpdate* pvalue)
{
   int stat = ASN_OK;

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.etat_dominationPresent);

   /* encode oid_connaissance */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_connaissance);
   if (stat != ASN_OK) return stat;

   /* encode oid_groupe_possesseur */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_groupe_possesseur);
   if (stat != ASN_OK) return stat;

   /* encode etat_domination */

   if (pvalue->m.etat_dominationPresent) {
      stat = asn1PE_Percentage (ctxt_p, pvalue->etat_domination);
      if (stat != ASN_OK) return stat;
   }

   return (stat);
}

int ASN1C_MsgPopulationKnowledgeUpdate::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgPopulationKnowledgeUpdate (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgPopulationKnowledgeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationKnowledgeUpdate* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL optbit;

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   pd_bit (ctxt_p, &optbit);
   pvalue->m.etat_dominationPresent = optbit;

   /* decode oid_connaissance */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_connaissance);
   if (stat != ASN_OK) return stat;

   /* decode oid_groupe_possesseur */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_groupe_possesseur);
   if (stat != ASN_OK) return stat;

   /* decode etat_domination */

   if (pvalue->m.etat_dominationPresent) {
      stat = asn1PD_Percentage (ctxt_p, &pvalue->etat_domination);
      if (stat != ASN_OK) return stat;
   }


   return (stat);
}

int ASN1C_MsgPopulationKnowledgeUpdate::Decode ()
{
   return asn1PD_MsgPopulationKnowledgeUpdate (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgPopulationKnowledgeDestruction                         */
/*                                                            */
/**************************************************************/

ASN1C_MsgPopulationKnowledgeDestruction::ASN1C_MsgPopulationKnowledgeDestruction (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationKnowledgeDestruction& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgPopulationKnowledgeDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationKnowledgeDestruction* pvalue)
{
   int stat = ASN_OK;

   /* encode oid_connaissance */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_connaissance);
   if (stat != ASN_OK) return stat;

   /* encode oid_groupe_possesseur */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_groupe_possesseur);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgPopulationKnowledgeDestruction::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgPopulationKnowledgeDestruction (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgPopulationKnowledgeDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationKnowledgeDestruction* pvalue)
{
   int stat = ASN_OK;

   /* decode oid_connaissance */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_connaissance);
   if (stat != ASN_OK) return stat;

   /* decode oid_groupe_possesseur */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_groupe_possesseur);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgPopulationKnowledgeDestruction::Decode ()
{
   return asn1PD_MsgPopulationKnowledgeDestruction (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgPopulationConcentrationKnowledgeCreation               */
/*                                                            */
/**************************************************************/

ASN1C_MsgPopulationConcentrationKnowledgeCreation::ASN1C_MsgPopulationConcentrationKnowledgeCreation (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationConcentrationKnowledgeCreation& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgPopulationConcentrationKnowledgeCreation (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationConcentrationKnowledgeCreation* pvalue)
{
   int stat = ASN_OK;

   /* encode oid_connaissance_concentration */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_connaissance_concentration);
   if (stat != ASN_OK) return stat;

   /* encode oid_connaissance_population */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_connaissance_population);
   if (stat != ASN_OK) return stat;

   /* encode oid_groupe_possesseur */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_groupe_possesseur);
   if (stat != ASN_OK) return stat;

   /* encode oid_concentration_reelle */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_concentration_reelle);
   if (stat != ASN_OK) return stat;

   /* encode position */

   stat = asn1PE_CoordLatLong (ctxt_p, &pvalue->position);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgPopulationConcentrationKnowledgeCreation::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgPopulationConcentrationKnowledgeCreation (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgPopulationConcentrationKnowledgeCreation (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationConcentrationKnowledgeCreation* pvalue)
{
   int stat = ASN_OK;

   /* decode oid_connaissance_concentration */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_connaissance_concentration);
   if (stat != ASN_OK) return stat;

   /* decode oid_connaissance_population */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_connaissance_population);
   if (stat != ASN_OK) return stat;

   /* decode oid_groupe_possesseur */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_groupe_possesseur);
   if (stat != ASN_OK) return stat;

   /* decode oid_concentration_reelle */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_concentration_reelle);
   if (stat != ASN_OK) return stat;

   /* decode position */

   stat = asn1PD_CoordLatLong (ctxt_p, &pvalue->position);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgPopulationConcentrationKnowledgeCreation::Decode ()
{
   return asn1PD_MsgPopulationConcentrationKnowledgeCreation (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgPopulationConcentrationKnowledgeUpdate                 */
/*                                                            */
/**************************************************************/

ASN1C_MsgPopulationConcentrationKnowledgeUpdate::ASN1C_MsgPopulationConcentrationKnowledgeUpdate (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationConcentrationKnowledgeUpdate& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgPopulationConcentrationKnowledgeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationConcentrationKnowledgeUpdate* pvalue)
{
   int stat = ASN_OK;

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.oid_concentration_reellePresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.nb_humains_vivantsPresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.nb_humains_mortsPresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.attitudePresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.pertinencePresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.est_percuPresent);

   /* encode oid_connaissance_concentration */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_connaissance_concentration);
   if (stat != ASN_OK) return stat;

   /* encode oid_connaissance_population */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_connaissance_population);
   if (stat != ASN_OK) return stat;

   /* encode oid_groupe_possesseur */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_groupe_possesseur);
   if (stat != ASN_OK) return stat;

   /* encode oid_concentration_reelle */

   if (pvalue->m.oid_concentration_reellePresent) {
      stat = asn1PE_OID (ctxt_p, pvalue->oid_concentration_reelle);
      if (stat != ASN_OK) return stat;
   }

   /* encode nb_humains_vivants */

   if (pvalue->m.nb_humains_vivantsPresent) {
      stat = pe_UnconsInteger (ctxt_p, pvalue->nb_humains_vivants);
      if (stat != ASN_OK) return stat;
   }

   /* encode nb_humains_morts */

   if (pvalue->m.nb_humains_mortsPresent) {
      stat = pe_UnconsInteger (ctxt_p, pvalue->nb_humains_morts);
      if (stat != ASN_OK) return stat;
   }

   /* encode attitude */

   if (pvalue->m.attitudePresent) {
      stat = asn1PE_EnumPopulationAttitude (ctxt_p, pvalue->attitude);
      if (stat != ASN_OK) return stat;
   }

   /* encode pertinence */

   if (pvalue->m.pertinencePresent) {
      stat = asn1PE_Percentage (ctxt_p, pvalue->pertinence);
      if (stat != ASN_OK) return stat;
   }

   /* encode est_percu */

   if (pvalue->m.est_percuPresent) {
      stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->est_percu);
      if (stat != ASN_OK) return stat;
   }

   return (stat);
}

int ASN1C_MsgPopulationConcentrationKnowledgeUpdate::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgPopulationConcentrationKnowledgeUpdate (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgPopulationConcentrationKnowledgeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationConcentrationKnowledgeUpdate* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL optbit;

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   pd_bit (ctxt_p, &optbit);
   pvalue->m.oid_concentration_reellePresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.nb_humains_vivantsPresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.nb_humains_mortsPresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.attitudePresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.pertinencePresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.est_percuPresent = optbit;

   /* decode oid_connaissance_concentration */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_connaissance_concentration);
   if (stat != ASN_OK) return stat;

   /* decode oid_connaissance_population */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_connaissance_population);
   if (stat != ASN_OK) return stat;

   /* decode oid_groupe_possesseur */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_groupe_possesseur);
   if (stat != ASN_OK) return stat;

   /* decode oid_concentration_reelle */

   if (pvalue->m.oid_concentration_reellePresent) {
      stat = asn1PD_OID (ctxt_p, &pvalue->oid_concentration_reelle);
      if (stat != ASN_OK) return stat;
   }

   /* decode nb_humains_vivants */

   if (pvalue->m.nb_humains_vivantsPresent) {
      stat = pd_UnconsInteger (ctxt_p, &pvalue->nb_humains_vivants);
      if (stat != ASN_OK) return stat;
   }

   /* decode nb_humains_morts */

   if (pvalue->m.nb_humains_mortsPresent) {
      stat = pd_UnconsInteger (ctxt_p, &pvalue->nb_humains_morts);
      if (stat != ASN_OK) return stat;
   }

   /* decode attitude */

   if (pvalue->m.attitudePresent) {
      stat = asn1PD_EnumPopulationAttitude (ctxt_p, &pvalue->attitude);
      if (stat != ASN_OK) return stat;
   }

   /* decode pertinence */

   if (pvalue->m.pertinencePresent) {
      stat = asn1PD_Percentage (ctxt_p, &pvalue->pertinence);
      if (stat != ASN_OK) return stat;
   }

   /* decode est_percu */

   if (pvalue->m.est_percuPresent) {
      stat = pd_bit (ctxt_p, &pvalue->est_percu);
      if (stat != ASN_OK) return stat;
   }


   return (stat);
}

int ASN1C_MsgPopulationConcentrationKnowledgeUpdate::Decode ()
{
   return asn1PD_MsgPopulationConcentrationKnowledgeUpdate (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgPopulationConcentrationKnowledgeDestruction            */
/*                                                            */
/**************************************************************/

ASN1C_MsgPopulationConcentrationKnowledgeDestruction::ASN1C_MsgPopulationConcentrationKnowledgeDestruction (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationConcentrationKnowledgeDestruction& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgPopulationConcentrationKnowledgeDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationConcentrationKnowledgeDestruction* pvalue)
{
   int stat = ASN_OK;

   /* encode oid_connaissance_concentration */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_connaissance_concentration);
   if (stat != ASN_OK) return stat;

   /* encode oid_connaissance_population */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_connaissance_population);
   if (stat != ASN_OK) return stat;

   /* encode oid_groupe_possesseur */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_groupe_possesseur);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgPopulationConcentrationKnowledgeDestruction::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgPopulationConcentrationKnowledgeDestruction (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgPopulationConcentrationKnowledgeDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationConcentrationKnowledgeDestruction* pvalue)
{
   int stat = ASN_OK;

   /* decode oid_connaissance_concentration */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_connaissance_concentration);
   if (stat != ASN_OK) return stat;

   /* decode oid_connaissance_population */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_connaissance_population);
   if (stat != ASN_OK) return stat;

   /* decode oid_groupe_possesseur */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_groupe_possesseur);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgPopulationConcentrationKnowledgeDestruction::Decode ()
{
   return asn1PD_MsgPopulationConcentrationKnowledgeDestruction (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgPopulationFlowKnowledgeCreation                        */
/*                                                            */
/**************************************************************/

ASN1C_MsgPopulationFlowKnowledgeCreation::ASN1C_MsgPopulationFlowKnowledgeCreation (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationFlowKnowledgeCreation& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgPopulationFlowKnowledgeCreation (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationFlowKnowledgeCreation* pvalue)
{
   int stat = ASN_OK;

   /* encode oid_connaissance_flux */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_connaissance_flux);
   if (stat != ASN_OK) return stat;

   /* encode oid_connaissance_population */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_connaissance_population);
   if (stat != ASN_OK) return stat;

   /* encode oid_groupe_possesseur */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_groupe_possesseur);
   if (stat != ASN_OK) return stat;

   /* encode oid_flux_reel */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_flux_reel);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgPopulationFlowKnowledgeCreation::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgPopulationFlowKnowledgeCreation (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgPopulationFlowKnowledgeCreation (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationFlowKnowledgeCreation* pvalue)
{
   int stat = ASN_OK;

   /* decode oid_connaissance_flux */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_connaissance_flux);
   if (stat != ASN_OK) return stat;

   /* decode oid_connaissance_population */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_connaissance_population);
   if (stat != ASN_OK) return stat;

   /* decode oid_groupe_possesseur */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_groupe_possesseur);
   if (stat != ASN_OK) return stat;

   /* decode oid_flux_reel */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_flux_reel);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgPopulationFlowKnowledgeCreation::Decode ()
{
   return asn1PD_MsgPopulationFlowKnowledgeCreation (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  FlowPart                                                  */
/*                                                            */
/**************************************************************/

ASN1C_FlowPart::ASN1C_FlowPart (
   ASN1MessageBuffer& msgBuf, ASN1T_FlowPart& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_FlowPart (ASN1CTXT* ctxt_p, ASN1T_FlowPart* pvalue)
{
   int stat = ASN_OK;

   /* encode forme */

   stat = asn1PE_Path (ctxt_p, &pvalue->forme);
   if (stat != ASN_OK) return stat;

   /* encode pertinence */

   stat = asn1PE_Percentage (ctxt_p, pvalue->pertinence);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_FlowPart::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_FlowPart (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_FlowPart (ASN1CTXT* ctxt_p, ASN1T_FlowPart* pvalue)
{
   int stat = ASN_OK;

   /* decode forme */

   stat = asn1PD_Path (ctxt_p, &pvalue->forme);
   if (stat != ASN_OK) return stat;

   /* decode pertinence */

   stat = asn1PD_Percentage (ctxt_p, &pvalue->pertinence);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_FlowPart::Decode ()
{
   return asn1PD_FlowPart (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfFlowPart                                            */
/*                                                            */
/**************************************************************/

ASN1C__SeqOfFlowPart::ASN1C__SeqOfFlowPart (
   ASN1MessageBuffer& msgBuf, ASN1T__SeqOfFlowPart& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE__SeqOfFlowPart (ASN1CTXT* ctxt_p, ASN1T__SeqOfFlowPart* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* encode length determinant */

   stat = pe_Length (ctxt_p, pvalue->n);
   if (stat < 0) return stat;

   /* encode elements */

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      stat = asn1PE_FlowPart (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return stat;
   }
   return (stat);
}

int ASN1C__SeqOfFlowPart::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE__SeqOfFlowPart (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD__SeqOfFlowPart (ASN1CTXT* ctxt_p, ASN1T__SeqOfFlowPart* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = pd_Length (ctxt_p, &pvalue->n);
   if (stat != ASN_OK) return stat;

   /* decode elements */

   ALLOC_ASN1ARRAY (ctxt_p, pvalue, ASN1T_FlowPart);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      stat = asn1PD_FlowPart (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return stat;
   }


   return (stat);
}

int ASN1C__SeqOfFlowPart::Decode ()
{
   return asn1PD__SeqOfFlowPart (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgPopulationFlowKnowledgeUpdate                          */
/*                                                            */
/**************************************************************/

ASN1C_MsgPopulationFlowKnowledgeUpdate::ASN1C_MsgPopulationFlowKnowledgeUpdate (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationFlowKnowledgeUpdate& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgPopulationFlowKnowledgeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationFlowKnowledgeUpdate* pvalue)
{
   int stat = ASN_OK;

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.oid_flux_reelPresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.portions_fluxPresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.directionPresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.vitessePresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.nb_humains_vivantsPresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.nb_humains_mortsPresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.attitudePresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.est_percuPresent);

   /* encode oid_connaissance_flux */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_connaissance_flux);
   if (stat != ASN_OK) return stat;

   /* encode oid_connaissance_population */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_connaissance_population);
   if (stat != ASN_OK) return stat;

   /* encode oid_groupe_possesseur */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_groupe_possesseur);
   if (stat != ASN_OK) return stat;

   /* encode oid_flux_reel */

   if (pvalue->m.oid_flux_reelPresent) {
      stat = asn1PE_OID (ctxt_p, pvalue->oid_flux_reel);
      if (stat != ASN_OK) return stat;
   }

   /* encode portions_flux */

   if (pvalue->m.portions_fluxPresent) {
      stat = asn1PE__SeqOfFlowPart (ctxt_p, &pvalue->portions_flux);
      if (stat != ASN_OK) return stat;
   }

   /* encode direction */

   if (pvalue->m.directionPresent) {
      stat = asn1PE_Heading (ctxt_p, pvalue->direction);
      if (stat != ASN_OK) return stat;
   }

   /* encode vitesse */

   if (pvalue->m.vitessePresent) {
      stat = asn1PE_Speed (ctxt_p, pvalue->vitesse);
      if (stat != ASN_OK) return stat;
   }

   /* encode nb_humains_vivants */

   if (pvalue->m.nb_humains_vivantsPresent) {
      stat = pe_UnconsInteger (ctxt_p, pvalue->nb_humains_vivants);
      if (stat != ASN_OK) return stat;
   }

   /* encode nb_humains_morts */

   if (pvalue->m.nb_humains_mortsPresent) {
      stat = pe_UnconsInteger (ctxt_p, pvalue->nb_humains_morts);
      if (stat != ASN_OK) return stat;
   }

   /* encode attitude */

   if (pvalue->m.attitudePresent) {
      stat = asn1PE_EnumPopulationAttitude (ctxt_p, pvalue->attitude);
      if (stat != ASN_OK) return stat;
   }

   /* encode est_percu */

   if (pvalue->m.est_percuPresent) {
      stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->est_percu);
      if (stat != ASN_OK) return stat;
   }

   return (stat);
}

int ASN1C_MsgPopulationFlowKnowledgeUpdate::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgPopulationFlowKnowledgeUpdate (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgPopulationFlowKnowledgeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationFlowKnowledgeUpdate* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL optbit;

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   pd_bit (ctxt_p, &optbit);
   pvalue->m.oid_flux_reelPresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.portions_fluxPresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.directionPresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.vitessePresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.nb_humains_vivantsPresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.nb_humains_mortsPresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.attitudePresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.est_percuPresent = optbit;

   /* decode oid_connaissance_flux */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_connaissance_flux);
   if (stat != ASN_OK) return stat;

   /* decode oid_connaissance_population */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_connaissance_population);
   if (stat != ASN_OK) return stat;

   /* decode oid_groupe_possesseur */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_groupe_possesseur);
   if (stat != ASN_OK) return stat;

   /* decode oid_flux_reel */

   if (pvalue->m.oid_flux_reelPresent) {
      stat = asn1PD_OID (ctxt_p, &pvalue->oid_flux_reel);
      if (stat != ASN_OK) return stat;
   }

   /* decode portions_flux */

   if (pvalue->m.portions_fluxPresent) {
      stat = asn1PD__SeqOfFlowPart (ctxt_p, &pvalue->portions_flux);
      if (stat != ASN_OK) return stat;
   }

   /* decode direction */

   if (pvalue->m.directionPresent) {
      stat = asn1PD_Heading (ctxt_p, &pvalue->direction);
      if (stat != ASN_OK) return stat;
   }

   /* decode vitesse */

   if (pvalue->m.vitessePresent) {
      stat = asn1PD_Speed (ctxt_p, &pvalue->vitesse);
      if (stat != ASN_OK) return stat;
   }

   /* decode nb_humains_vivants */

   if (pvalue->m.nb_humains_vivantsPresent) {
      stat = pd_UnconsInteger (ctxt_p, &pvalue->nb_humains_vivants);
      if (stat != ASN_OK) return stat;
   }

   /* decode nb_humains_morts */

   if (pvalue->m.nb_humains_mortsPresent) {
      stat = pd_UnconsInteger (ctxt_p, &pvalue->nb_humains_morts);
      if (stat != ASN_OK) return stat;
   }

   /* decode attitude */

   if (pvalue->m.attitudePresent) {
      stat = asn1PD_EnumPopulationAttitude (ctxt_p, &pvalue->attitude);
      if (stat != ASN_OK) return stat;
   }

   /* decode est_percu */

   if (pvalue->m.est_percuPresent) {
      stat = pd_bit (ctxt_p, &pvalue->est_percu);
      if (stat != ASN_OK) return stat;
   }


   return (stat);
}

int ASN1C_MsgPopulationFlowKnowledgeUpdate::Decode ()
{
   return asn1PD_MsgPopulationFlowKnowledgeUpdate (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgPopulationFlowKnowledgeDestruction                     */
/*                                                            */
/**************************************************************/

ASN1C_MsgPopulationFlowKnowledgeDestruction::ASN1C_MsgPopulationFlowKnowledgeDestruction (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationFlowKnowledgeDestruction& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgPopulationFlowKnowledgeDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationFlowKnowledgeDestruction* pvalue)
{
   int stat = ASN_OK;

   /* encode oid_connaissance_flux */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_connaissance_flux);
   if (stat != ASN_OK) return stat;

   /* encode oid_connaissance_population */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_connaissance_population);
   if (stat != ASN_OK) return stat;

   /* encode oid_groupe_possesseur */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_groupe_possesseur);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgPopulationFlowKnowledgeDestruction::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgPopulationFlowKnowledgeDestruction (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgPopulationFlowKnowledgeDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationFlowKnowledgeDestruction* pvalue)
{
   int stat = ASN_OK;

   /* decode oid_connaissance_flux */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_connaissance_flux);
   if (stat != ASN_OK) return stat;

   /* decode oid_connaissance_population */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_connaissance_population);
   if (stat != ASN_OK) return stat;

   /* decode oid_groupe_possesseur */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_groupe_possesseur);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgPopulationFlowKnowledgeDestruction::Decode ()
{
   return asn1PD_MsgPopulationFlowKnowledgeDestruction (mpContext->GetPtr(), &msgData);
}



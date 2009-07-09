/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 09-Jul-2009.
 */
#include "LogMaintenance.h"

/**************************************************************/
/*                                                            */
/*  LogMaintenanceIncludes                                    */
/*                                                            */
/**************************************************************/

ASN1C_LogMaintenanceIncludes::ASN1C_LogMaintenanceIncludes (
   ASN1MessageBuffer& msgBuf, ASN1T_LogMaintenanceIncludes& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_LogMaintenanceIncludes (ASN1CTXT* ctxt_p, ASN1T_LogMaintenanceIncludes* pvalue)
{
   int stat = ASN_OK;

   /* encode baseExports */

   stat = asn1PE_BaseExports (ctxt_p);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_LogMaintenanceIncludes::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_LogMaintenanceIncludes (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_LogMaintenanceIncludes (ASN1CTXT* ctxt_p, ASN1T_LogMaintenanceIncludes* pvalue)
{
   int stat = ASN_OK;

   /* decode baseExports */

   stat = asn1PD_BaseExports (ctxt_p);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_LogMaintenanceIncludes::Decode ()
{
   return asn1PD_LogMaintenanceIncludes (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  LogMaintenanceExports                                     */
/*                                                            */
/**************************************************************/

ASN1C_LogMaintenanceExports::ASN1C_LogMaintenanceExports (
   ASN1MessageBuffer& msgBuf) :
   ASN1CType(msgBuf)
{}

EXTERN int asn1PE_LogMaintenanceExports (ASN1CTXT* ctxt_p)
{
   int stat = ASN_OK;

   /* NULL */
   return (stat);
}

int ASN1C_LogMaintenanceExports::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_LogMaintenanceExports (mpContext->GetPtr());
   return stat;
}

EXTERN int asn1PD_LogMaintenanceExports (ASN1CTXT* ctxt_p)
{
   int stat = ASN_OK;

   /* NULL */

   return (stat);
}

int ASN1C_LogMaintenanceExports::Decode ()
{
   return asn1PD_LogMaintenanceExports (mpContext->GetPtr());
}

/**************************************************************/
/*                                                            */
/*  EnumLogMaintenanceRegimeTravail                           */
/*                                                            */
/**************************************************************/

ASN1C_EnumLogMaintenanceRegimeTravail::ASN1C_EnumLogMaintenanceRegimeTravail (
   ASN1MessageBuffer& msgBuf, ASN1T_EnumLogMaintenanceRegimeTravail& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_EnumLogMaintenanceRegimeTravail (ASN1CTXT* ctxt_p, ASN1T_EnumLogMaintenanceRegimeTravail value)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   switch (value) {
      case EnumLogMaintenanceRegimeTravail::regime_1: ui = 0; break;
      case EnumLogMaintenanceRegimeTravail::regime_2: ui = 1; break;
      case EnumLogMaintenanceRegimeTravail::regime_3: ui = 2; break;
      case EnumLogMaintenanceRegimeTravail::regime_4: ui = 3; break;
      default: return ASN_E_INVENUM;
   }

   stat = pe_ConsUnsigned (ctxt_p, ui, 0, 3);
   if (stat != ASN_OK) return stat;
   return (stat);
}

int ASN1C_EnumLogMaintenanceRegimeTravail::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_EnumLogMaintenanceRegimeTravail (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_EnumLogMaintenanceRegimeTravail (ASN1CTXT* ctxt_p, ASN1T_EnumLogMaintenanceRegimeTravail* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   stat = pd_ConsUnsigned (ctxt_p, &ui, 0, 3);
   if (stat != ASN_OK) return stat;

   switch (ui) {
      case 0: *pvalue = EnumLogMaintenanceRegimeTravail::regime_1; break;
      case 1: *pvalue = EnumLogMaintenanceRegimeTravail::regime_2; break;
      case 2: *pvalue = EnumLogMaintenanceRegimeTravail::regime_3; break;
      case 3: *pvalue = EnumLogMaintenanceRegimeTravail::regime_4; break;
      default: return ASN_E_INVENUM;
   }

   return (stat);
}

int ASN1C_EnumLogMaintenanceRegimeTravail::Decode ()
{
   return asn1PD_EnumLogMaintenanceRegimeTravail (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgLogMaintenanceHandlingCreation                         */
/*                                                            */
/**************************************************************/

ASN1C_MsgLogMaintenanceHandlingCreation::ASN1C_MsgLogMaintenanceHandlingCreation (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgLogMaintenanceHandlingCreation& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgLogMaintenanceHandlingCreation (ASN1CTXT* ctxt_p, ASN1T_MsgLogMaintenanceHandlingCreation* pvalue)
{
   int stat = ASN_OK;

   /* encode oid_consigne */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_consigne);
   if (stat != ASN_OK) return stat;

   /* encode oid_pion */

   stat = asn1PE_Unit (ctxt_p, pvalue->oid_pion);
   if (stat != ASN_OK) return stat;

   /* encode tick_creation */

   stat = pe_UnconsInteger (ctxt_p, pvalue->tick_creation);
   if (stat != ASN_OK) return stat;

   /* encode type_equipement */

   stat = asn1PE_EquipmentType (ctxt_p, pvalue->type_equipement);
   if (stat != ASN_OK) return stat;

   /* encode type_panne */

   stat = asn1PE_BreakdownType (ctxt_p, pvalue->type_panne);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgLogMaintenanceHandlingCreation::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgLogMaintenanceHandlingCreation (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgLogMaintenanceHandlingCreation (ASN1CTXT* ctxt_p, ASN1T_MsgLogMaintenanceHandlingCreation* pvalue)
{
   int stat = ASN_OK;

   /* decode oid_consigne */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_consigne);
   if (stat != ASN_OK) return stat;

   /* decode oid_pion */

   stat = asn1PD_Unit (ctxt_p, &pvalue->oid_pion);
   if (stat != ASN_OK) return stat;

   /* decode tick_creation */

   stat = pd_UnconsInteger (ctxt_p, &pvalue->tick_creation);
   if (stat != ASN_OK) return stat;

   /* decode type_equipement */

   stat = asn1PD_EquipmentType (ctxt_p, &pvalue->type_equipement);
   if (stat != ASN_OK) return stat;

   /* decode type_panne */

   stat = asn1PD_BreakdownType (ctxt_p, &pvalue->type_panne);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgLogMaintenanceHandlingCreation::Decode ()
{
   return asn1PD_MsgLogMaintenanceHandlingCreation (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  EnumLogMaintenanceHandlingStatus                          */
/*                                                            */
/**************************************************************/

ASN1C_EnumLogMaintenanceHandlingStatus::ASN1C_EnumLogMaintenanceHandlingStatus (
   ASN1MessageBuffer& msgBuf, ASN1T_EnumLogMaintenanceHandlingStatus& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_EnumLogMaintenanceHandlingStatus (ASN1CTXT* ctxt_p, ASN1T_EnumLogMaintenanceHandlingStatus value)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   switch (value) {
      case EnumLogMaintenanceHandlingStatus::deplacement_vers_chaine: ui = 0; break;
      case EnumLogMaintenanceHandlingStatus::attente_disponibilite_remorqueur: ui = 1; break;
      case EnumLogMaintenanceHandlingStatus::remorqueur_deplacement_aller: ui = 2; break;
      case EnumLogMaintenanceHandlingStatus::remorqueur_chargement: ui = 3; break;
      case EnumLogMaintenanceHandlingStatus::remorqueur_deplacement_retour: ui = 4; break;
      case EnumLogMaintenanceHandlingStatus::remorqueur_dechargement: ui = 5; break;
      case EnumLogMaintenanceHandlingStatus::diagnostique: ui = 6; break;
      case EnumLogMaintenanceHandlingStatus::attente_prise_en_charge_par_niveau_superieur: ui = 7; break;
      case EnumLogMaintenanceHandlingStatus::attente_disponibilite_pieces: ui = 8; break;
      case EnumLogMaintenanceHandlingStatus::attente_disponibilite_reparateur: ui = 9; break;
      case EnumLogMaintenanceHandlingStatus::reparation: ui = 10; break;
      case EnumLogMaintenanceHandlingStatus::retour_pion: ui = 11; break;
      case EnumLogMaintenanceHandlingStatus::termine: ui = 12; break;
      default: return ASN_E_INVENUM;
   }

   stat = pe_ConsUnsigned (ctxt_p, ui, 0, 12);
   if (stat != ASN_OK) return stat;
   return (stat);
}

int ASN1C_EnumLogMaintenanceHandlingStatus::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_EnumLogMaintenanceHandlingStatus (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_EnumLogMaintenanceHandlingStatus (ASN1CTXT* ctxt_p, ASN1T_EnumLogMaintenanceHandlingStatus* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   stat = pd_ConsUnsigned (ctxt_p, &ui, 0, 12);
   if (stat != ASN_OK) return stat;

   switch (ui) {
      case 0: *pvalue = EnumLogMaintenanceHandlingStatus::deplacement_vers_chaine; break;
      case 1: *pvalue = EnumLogMaintenanceHandlingStatus::attente_disponibilite_remorqueur; break;
      case 2: *pvalue = EnumLogMaintenanceHandlingStatus::remorqueur_deplacement_aller; break;
      case 3: *pvalue = EnumLogMaintenanceHandlingStatus::remorqueur_chargement; break;
      case 4: *pvalue = EnumLogMaintenanceHandlingStatus::remorqueur_deplacement_retour; break;
      case 5: *pvalue = EnumLogMaintenanceHandlingStatus::remorqueur_dechargement; break;
      case 6: *pvalue = EnumLogMaintenanceHandlingStatus::diagnostique; break;
      case 7: *pvalue = EnumLogMaintenanceHandlingStatus::attente_prise_en_charge_par_niveau_superieur; break;
      case 8: *pvalue = EnumLogMaintenanceHandlingStatus::attente_disponibilite_pieces; break;
      case 9: *pvalue = EnumLogMaintenanceHandlingStatus::attente_disponibilite_reparateur; break;
      case 10: *pvalue = EnumLogMaintenanceHandlingStatus::reparation; break;
      case 11: *pvalue = EnumLogMaintenanceHandlingStatus::retour_pion; break;
      case 12: *pvalue = EnumLogMaintenanceHandlingStatus::termine; break;
      default: return ASN_E_INVENUM;
   }

   return (stat);
}

int ASN1C_EnumLogMaintenanceHandlingStatus::Decode ()
{
   return asn1PD_EnumLogMaintenanceHandlingStatus (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgLogMaintenanceHandlingUpdate                           */
/*                                                            */
/**************************************************************/

ASN1C_MsgLogMaintenanceHandlingUpdate::ASN1C_MsgLogMaintenanceHandlingUpdate (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgLogMaintenanceHandlingUpdate& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgLogMaintenanceHandlingUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgLogMaintenanceHandlingUpdate* pvalue)
{
   int stat = ASN_OK;

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.etatPresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.diagnostique_effectuePresent);

   /* encode oid_consigne */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_consigne);
   if (stat != ASN_OK) return stat;

   /* encode oid_pion */

   stat = asn1PE_Unit (ctxt_p, pvalue->oid_pion);
   if (stat != ASN_OK) return stat;

   /* encode oid_pion_log_traitant */

   stat = asn1PE_Unit (ctxt_p, pvalue->oid_pion_log_traitant);
   if (stat != ASN_OK) return stat;

   /* encode etat */

   if (pvalue->m.etatPresent) {
      stat = asn1PE_EnumLogMaintenanceHandlingStatus (ctxt_p, pvalue->etat);
      if (stat != ASN_OK) return stat;
   }

   /* encode diagnostique_effectue */

   if (pvalue->m.diagnostique_effectuePresent) {
      stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->diagnostique_effectue);
      if (stat != ASN_OK) return stat;
   }

   return (stat);
}

int ASN1C_MsgLogMaintenanceHandlingUpdate::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgLogMaintenanceHandlingUpdate (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgLogMaintenanceHandlingUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgLogMaintenanceHandlingUpdate* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL optbit;

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   pd_bit (ctxt_p, &optbit);
   pvalue->m.etatPresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.diagnostique_effectuePresent = optbit;

   /* decode oid_consigne */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_consigne);
   if (stat != ASN_OK) return stat;

   /* decode oid_pion */

   stat = asn1PD_Unit (ctxt_p, &pvalue->oid_pion);
   if (stat != ASN_OK) return stat;

   /* decode oid_pion_log_traitant */

   stat = asn1PD_Unit (ctxt_p, &pvalue->oid_pion_log_traitant);
   if (stat != ASN_OK) return stat;

   /* decode etat */

   if (pvalue->m.etatPresent) {
      stat = asn1PD_EnumLogMaintenanceHandlingStatus (ctxt_p, &pvalue->etat);
      if (stat != ASN_OK) return stat;
   }

   /* decode diagnostique_effectue */

   if (pvalue->m.diagnostique_effectuePresent) {
      stat = pd_bit (ctxt_p, &pvalue->diagnostique_effectue);
      if (stat != ASN_OK) return stat;
   }


   return (stat);
}

int ASN1C_MsgLogMaintenanceHandlingUpdate::Decode ()
{
   return asn1PD_MsgLogMaintenanceHandlingUpdate (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgLogMaintenanceHandlingDestruction                      */
/*                                                            */
/**************************************************************/

ASN1C_MsgLogMaintenanceHandlingDestruction::ASN1C_MsgLogMaintenanceHandlingDestruction (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgLogMaintenanceHandlingDestruction& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgLogMaintenanceHandlingDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgLogMaintenanceHandlingDestruction* pvalue)
{
   int stat = ASN_OK;

   /* encode oid_consigne */

   stat = asn1PE_OID (ctxt_p, pvalue->oid_consigne);
   if (stat != ASN_OK) return stat;

   /* encode oid_pion */

   stat = asn1PE_Unit (ctxt_p, pvalue->oid_pion);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgLogMaintenanceHandlingDestruction::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgLogMaintenanceHandlingDestruction (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgLogMaintenanceHandlingDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgLogMaintenanceHandlingDestruction* pvalue)
{
   int stat = ASN_OK;

   /* decode oid_consigne */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_consigne);
   if (stat != ASN_OK) return stat;

   /* decode oid_pion */

   stat = asn1PD_Unit (ctxt_p, &pvalue->oid_pion);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgLogMaintenanceHandlingDestruction::Decode ()
{
   return asn1PD_MsgLogMaintenanceHandlingDestruction (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  LogMaintenanceEquipmentAvailability                       */
/*                                                            */
/**************************************************************/

ASN1C_LogMaintenanceEquipmentAvailability::ASN1C_LogMaintenanceEquipmentAvailability (
   ASN1MessageBuffer& msgBuf, ASN1T_LogMaintenanceEquipmentAvailability& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_LogMaintenanceEquipmentAvailability (ASN1CTXT* ctxt_p, ASN1T_LogMaintenanceEquipmentAvailability* pvalue)
{
   int stat = ASN_OK;

   pe_bit (ctxt_p, (ASN1BOOL)(pvalue->nbr_pretes != 0));

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.nbr_au_reposPresent);

   /* encode type_equipement */

   stat = asn1PE_EquipmentType (ctxt_p, pvalue->type_equipement);
   if (stat != ASN_OK) return stat;

   /* encode nbr_total */

   stat = pe_UnconsInteger (ctxt_p, pvalue->nbr_total);
   if (stat != ASN_OK) return stat;

   /* encode nbr_disponibles */

   stat = pe_UnconsInteger (ctxt_p, pvalue->nbr_disponibles);
   if (stat != ASN_OK) return stat;

   /* encode nbr_au_travail */

   stat = pe_UnconsInteger (ctxt_p, pvalue->nbr_au_travail);
   if (stat != ASN_OK) return stat;

   /* encode nbr_pretes */

   if (pvalue->nbr_pretes != 0) {
      stat = pe_UnconsInteger (ctxt_p, pvalue->nbr_pretes);
      if (stat != ASN_OK) return stat;
   }

   /* encode nbr_au_repos */

   if (pvalue->m.nbr_au_reposPresent) {
      stat = pe_UnconsInteger (ctxt_p, pvalue->nbr_au_repos);
      if (stat != ASN_OK) return stat;
   }

   return (stat);
}

int ASN1C_LogMaintenanceEquipmentAvailability::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_LogMaintenanceEquipmentAvailability (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_LogMaintenanceEquipmentAvailability (ASN1CTXT* ctxt_p, ASN1T_LogMaintenanceEquipmentAvailability* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL optbit;
   ASN1BOOL nbr_pretesPresent;

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   pd_bit (ctxt_p, &optbit);
   nbr_pretesPresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.nbr_au_reposPresent = optbit;

   /* decode type_equipement */

   stat = asn1PD_EquipmentType (ctxt_p, &pvalue->type_equipement);
   if (stat != ASN_OK) return stat;

   /* decode nbr_total */

   stat = pd_UnconsInteger (ctxt_p, &pvalue->nbr_total);
   if (stat != ASN_OK) return stat;

   /* decode nbr_disponibles */

   stat = pd_UnconsInteger (ctxt_p, &pvalue->nbr_disponibles);
   if (stat != ASN_OK) return stat;

   /* decode nbr_au_travail */

   stat = pd_UnconsInteger (ctxt_p, &pvalue->nbr_au_travail);
   if (stat != ASN_OK) return stat;

   /* decode nbr_pretes */

   if (nbr_pretesPresent) {
      stat = pd_UnconsInteger (ctxt_p, &pvalue->nbr_pretes);
      if (stat != ASN_OK) return stat;
   }
   else {
      pvalue->nbr_pretes = 0;
   }

   /* decode nbr_au_repos */

   if (pvalue->m.nbr_au_reposPresent) {
      stat = pd_UnconsInteger (ctxt_p, &pvalue->nbr_au_repos);
      if (stat != ASN_OK) return stat;
   }


   return (stat);
}

int ASN1C_LogMaintenanceEquipmentAvailability::Decode ()
{
   return asn1PD_LogMaintenanceEquipmentAvailability (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  LogMaintenancePriorities                                  */
/*                                                            */
/**************************************************************/

ASN1C_LogMaintenancePriorities::ASN1C_LogMaintenancePriorities (
   ASN1MessageBuffer& msgBuf, ASN1T_LogMaintenancePriorities& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_LogMaintenancePriorities (ASN1CTXT* ctxt_p, ASN1T_LogMaintenancePriorities* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* encode length determinant */

   stat = pe_Length (ctxt_p, pvalue->n);
   if (stat < 0) return stat;

   /* encode elements */

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      stat = asn1PE_EquipmentType (ctxt_p, pvalue->elem[xx1]);
      if (stat != ASN_OK) return stat;
   }
   return (stat);
}

int ASN1C_LogMaintenancePriorities::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_LogMaintenancePriorities (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_LogMaintenancePriorities (ASN1CTXT* ctxt_p, ASN1T_LogMaintenancePriorities* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = pd_Length (ctxt_p, &pvalue->n);
   if (stat != ASN_OK) return stat;

   /* decode elements */

   ALLOC_ASN1ARRAY (ctxt_p, pvalue, ASN1T_EquipmentType);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      stat = asn1PD_EquipmentType (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return stat;
   }


   return (stat);
}

int ASN1C_LogMaintenancePriorities::Decode ()
{
   return asn1PD_LogMaintenancePriorities (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfLogMaintenanceEquipmentAvailability                 */
/*                                                            */
/**************************************************************/

ASN1C__SeqOfLogMaintenanceEquipmentAvailability::ASN1C__SeqOfLogMaintenanceEquipmentAvailability (
   ASN1MessageBuffer& msgBuf, ASN1T__SeqOfLogMaintenanceEquipmentAvailability& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE__SeqOfLogMaintenanceEquipmentAvailability (ASN1CTXT* ctxt_p, ASN1T__SeqOfLogMaintenanceEquipmentAvailability* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* encode length determinant */

   stat = pe_Length (ctxt_p, pvalue->n);
   if (stat < 0) return stat;

   /* encode elements */

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      stat = asn1PE_LogMaintenanceEquipmentAvailability (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return stat;
   }
   return (stat);
}

int ASN1C__SeqOfLogMaintenanceEquipmentAvailability::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE__SeqOfLogMaintenanceEquipmentAvailability (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD__SeqOfLogMaintenanceEquipmentAvailability (ASN1CTXT* ctxt_p, ASN1T__SeqOfLogMaintenanceEquipmentAvailability* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = pd_Length (ctxt_p, &pvalue->n);
   if (stat != ASN_OK) return stat;

   /* decode elements */

   ALLOC_ASN1ARRAY (ctxt_p, pvalue, ASN1T_LogMaintenanceEquipmentAvailability);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      stat = asn1PD_LogMaintenanceEquipmentAvailability (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return stat;
   }


   return (stat);
}

int ASN1C__SeqOfLogMaintenanceEquipmentAvailability::Decode ()
{
   return asn1PD__SeqOfLogMaintenanceEquipmentAvailability (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgLogMaintenanceState                                    */
/*                                                            */
/**************************************************************/

ASN1C_MsgLogMaintenanceState::ASN1C_MsgLogMaintenanceState (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgLogMaintenanceState& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgLogMaintenanceState (ASN1CTXT* ctxt_p, ASN1T_MsgLogMaintenanceState* pvalue)
{
   int stat = ASN_OK;

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.chaine_activeePresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.regime_travailPresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.prioritesPresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.priorites_tactiquesPresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.disponibilites_remorqueursPresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.disponibilites_reparateursPresent);

   /* encode oid_pion */

   stat = asn1PE_Unit (ctxt_p, pvalue->oid_pion);
   if (stat != ASN_OK) return stat;

   /* encode chaine_activee */

   if (pvalue->m.chaine_activeePresent) {
      stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->chaine_activee);
      if (stat != ASN_OK) return stat;
   }

   /* encode regime_travail */

   if (pvalue->m.regime_travailPresent) {
      stat = asn1PE_EnumLogMaintenanceRegimeTravail (ctxt_p, pvalue->regime_travail);
      if (stat != ASN_OK) return stat;
   }

   /* encode priorites */

   if (pvalue->m.prioritesPresent) {
      stat = asn1PE_LogMaintenancePriorities (ctxt_p, &pvalue->priorites);
      if (stat != ASN_OK) return stat;
   }

   /* encode priorites_tactiques */

   if (pvalue->m.priorites_tactiquesPresent) {
      stat = asn1PE_AutomatList (ctxt_p, &pvalue->priorites_tactiques);
      if (stat != ASN_OK) return stat;
   }

   /* encode disponibilites_remorqueurs */

   if (pvalue->m.disponibilites_remorqueursPresent) {
      stat = asn1PE__SeqOfLogMaintenanceEquipmentAvailability (ctxt_p, &pvalue->disponibilites_remorqueurs);
      if (stat != ASN_OK) return stat;
   }

   /* encode disponibilites_reparateurs */

   if (pvalue->m.disponibilites_reparateursPresent) {
      stat = asn1PE__SeqOfLogMaintenanceEquipmentAvailability (ctxt_p, &pvalue->disponibilites_reparateurs);
      if (stat != ASN_OK) return stat;
   }

   return (stat);
}

int ASN1C_MsgLogMaintenanceState::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgLogMaintenanceState (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgLogMaintenanceState (ASN1CTXT* ctxt_p, ASN1T_MsgLogMaintenanceState* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL optbit;

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   pd_bit (ctxt_p, &optbit);
   pvalue->m.chaine_activeePresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.regime_travailPresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.prioritesPresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.priorites_tactiquesPresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.disponibilites_remorqueursPresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.disponibilites_reparateursPresent = optbit;

   /* decode oid_pion */

   stat = asn1PD_Unit (ctxt_p, &pvalue->oid_pion);
   if (stat != ASN_OK) return stat;

   /* decode chaine_activee */

   if (pvalue->m.chaine_activeePresent) {
      stat = pd_bit (ctxt_p, &pvalue->chaine_activee);
      if (stat != ASN_OK) return stat;
   }

   /* decode regime_travail */

   if (pvalue->m.regime_travailPresent) {
      stat = asn1PD_EnumLogMaintenanceRegimeTravail (ctxt_p, &pvalue->regime_travail);
      if (stat != ASN_OK) return stat;
   }

   /* decode priorites */

   if (pvalue->m.prioritesPresent) {
      stat = asn1PD_LogMaintenancePriorities (ctxt_p, &pvalue->priorites);
      if (stat != ASN_OK) return stat;
   }

   /* decode priorites_tactiques */

   if (pvalue->m.priorites_tactiquesPresent) {
      stat = asn1PD_AutomatList (ctxt_p, &pvalue->priorites_tactiques);
      if (stat != ASN_OK) return stat;
   }

   /* decode disponibilites_remorqueurs */

   if (pvalue->m.disponibilites_remorqueursPresent) {
      stat = asn1PD__SeqOfLogMaintenanceEquipmentAvailability (ctxt_p, &pvalue->disponibilites_remorqueurs);
      if (stat != ASN_OK) return stat;
   }

   /* decode disponibilites_reparateurs */

   if (pvalue->m.disponibilites_reparateursPresent) {
      stat = asn1PD__SeqOfLogMaintenanceEquipmentAvailability (ctxt_p, &pvalue->disponibilites_reparateurs);
      if (stat != ASN_OK) return stat;
   }


   return (stat);
}

int ASN1C_MsgLogMaintenanceState::Decode ()
{
   return asn1PD_MsgLogMaintenanceState (mpContext->GetPtr(), &msgData);
}


/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 19-Aug-2005.
 */
#include "ScipioLogSante.h"

/**************************************************************/
/*                                                            */
/*  EnumLogSanteTraitementEtat                                */
/*                                                            */
/**************************************************************/

ASN1C_EnumLogSanteTraitementEtat::ASN1C_EnumLogSanteTraitementEtat (
   ASN1MessageBuffer& msgBuf, ASN1T_EnumLogSanteTraitementEtat& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_EnumLogSanteTraitementEtat (ASN1CTXT* ctxt_p, ASN1T_EnumLogSanteTraitementEtat value)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   rtdiag ("asn1PE_EnumLogSanteTraitementEtat: start\n");

   switch (value) {
      case EnumLogSanteTraitementEtat::attente_disponibilite_ambulance_releve: ui = 0; break;
      case EnumLogSanteTraitementEtat::ambulance_releve_deplacement_aller: ui = 1; break;
      case EnumLogSanteTraitementEtat::ambulance_releve_chargement: ui = 2; break;
      case EnumLogSanteTraitementEtat::attente_chargement_complet_ambulance_releve: ui = 3; break;
      case EnumLogSanteTraitementEtat::ambulance_releve_deplacement_retour: ui = 4; break;
      case EnumLogSanteTraitementEtat::ambulance_releve_dechargement: ui = 5; break;
      case EnumLogSanteTraitementEtat::attente_disponibilite_medecin_pour_diagnostique: ui = 6; break;
      case EnumLogSanteTraitementEtat::diagnostique: ui = 7; break;
      case EnumLogSanteTraitementEtat::recherche_secteur_tri: ui = 8; break;
      case EnumLogSanteTraitementEtat::attente_disponibilite_medecin_pour_tri: ui = 9; break;
      case EnumLogSanteTraitementEtat::tri: ui = 10; break;
      case EnumLogSanteTraitementEtat::recherche_secteur_soin: ui = 11; break;
      case EnumLogSanteTraitementEtat::attente_disponibilite_medecin_pour_soin: ui = 12; break;
      case EnumLogSanteTraitementEtat::soin: ui = 13; break;
      case EnumLogSanteTraitementEtat::attente_disponibilite_ambulance_ramassage: ui = 14; break;
      case EnumLogSanteTraitementEtat::ambulance_ramassage_chargement: ui = 15; break;
      case EnumLogSanteTraitementEtat::attente_chargement_complet_ambulance_ramassage: ui = 16; break;
      case EnumLogSanteTraitementEtat::ambulance_ramassage_deplacement_aller: ui = 17; break;
      case EnumLogSanteTraitementEtat::ambulance_ramassage_dechargement: ui = 18; break;
      case EnumLogSanteTraitementEtat::termine: ui = 19; break;
      default: return LOG_ASN1ERR (ctxt_p, ASN_E_INVENUM);
   }

   stat = pe_ConsUnsigned (ctxt_p, ui, 0, 19);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   rtdiag ("asn1PE_EnumLogSanteTraitementEtat: end\n");
   return (stat);
}

int ASN1C_EnumLogSanteTraitementEtat::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_EnumLogSanteTraitementEtat (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_EnumLogSanteTraitementEtat (ASN1CTXT* ctxt_p, ASN1T_EnumLogSanteTraitementEtat* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   rtdiag ("asn1PD_EnumLogSanteTraitementEtat: start\n");

   stat = pd_ConsUnsigned (ctxt_p, &ui, 0, 19);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   switch (ui) {
      case 0: *pvalue = EnumLogSanteTraitementEtat::attente_disponibilite_ambulance_releve; break;
      case 1: *pvalue = EnumLogSanteTraitementEtat::ambulance_releve_deplacement_aller; break;
      case 2: *pvalue = EnumLogSanteTraitementEtat::ambulance_releve_chargement; break;
      case 3: *pvalue = EnumLogSanteTraitementEtat::attente_chargement_complet_ambulance_releve; break;
      case 4: *pvalue = EnumLogSanteTraitementEtat::ambulance_releve_deplacement_retour; break;
      case 5: *pvalue = EnumLogSanteTraitementEtat::ambulance_releve_dechargement; break;
      case 6: *pvalue = EnumLogSanteTraitementEtat::attente_disponibilite_medecin_pour_diagnostique; break;
      case 7: *pvalue = EnumLogSanteTraitementEtat::diagnostique; break;
      case 8: *pvalue = EnumLogSanteTraitementEtat::recherche_secteur_tri; break;
      case 9: *pvalue = EnumLogSanteTraitementEtat::attente_disponibilite_medecin_pour_tri; break;
      case 10: *pvalue = EnumLogSanteTraitementEtat::tri; break;
      case 11: *pvalue = EnumLogSanteTraitementEtat::recherche_secteur_soin; break;
      case 12: *pvalue = EnumLogSanteTraitementEtat::attente_disponibilite_medecin_pour_soin; break;
      case 13: *pvalue = EnumLogSanteTraitementEtat::soin; break;
      case 14: *pvalue = EnumLogSanteTraitementEtat::attente_disponibilite_ambulance_ramassage; break;
      case 15: *pvalue = EnumLogSanteTraitementEtat::ambulance_ramassage_chargement; break;
      case 16: *pvalue = EnumLogSanteTraitementEtat::attente_chargement_complet_ambulance_ramassage; break;
      case 17: *pvalue = EnumLogSanteTraitementEtat::ambulance_ramassage_deplacement_aller; break;
      case 18: *pvalue = EnumLogSanteTraitementEtat::ambulance_ramassage_dechargement; break;
      case 19: *pvalue = EnumLogSanteTraitementEtat::termine; break;
      default: return LOG_ASN1ERR (ctxt_p, ASN_E_INVENUM);
   }

   rtdiag ("asn1PD_EnumLogSanteTraitementEtat: end\n");

   return (stat);
}

int ASN1C_EnumLogSanteTraitementEtat::Decode ()
{
   return asn1PD_EnumLogSanteTraitementEtat (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgLogSanteTraitementHumainCreation                       */
/*                                                            */
/**************************************************************/

ASN1C_MsgLogSanteTraitementHumainCreation::ASN1C_MsgLogSanteTraitementHumainCreation (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgLogSanteTraitementHumainCreation& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgLogSanteTraitementHumainCreation (ASN1CTXT* ctxt_p, ASN1T_MsgLogSanteTraitementHumainCreation* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgLogSanteTraitementHumainCreation: start\n");

   PU_NEWFIELD (ctxt_p, "blesse_mentalPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)(pvalue->blesse_mental != FALSE));
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "contamine_nbcPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)(pvalue->contamine_nbc != FALSE));
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   /* encode oid_consigne */

   PU_PUSHNAME (ctxt_p, "oid_consigne");

   stat = asn1PE_OID (ctxt_p, pvalue->oid_consigne);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode oid_pion */

   PU_PUSHNAME (ctxt_p, "oid_pion");

   stat = asn1PE_Agent (ctxt_p, pvalue->oid_pion);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode rang */

   PU_PUSHNAME (ctxt_p, "rang");

   stat = asn1PE_EnumHumanRank (ctxt_p, pvalue->rang);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode blessure */

   PU_PUSHNAME (ctxt_p, "blessure");

   stat = asn1PE_EnumHumanWound (ctxt_p, pvalue->blessure);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode blesse_mental */

   if (pvalue->blesse_mental != FALSE) {
      PU_PUSHNAME (ctxt_p, "blesse_mental");

      PU_NEWFIELD (ctxt_p, "boolean");

      stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->blesse_mental);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_SETBITCOUNT (ctxt_p);
      PU_POPNAME (ctxt_p);
   }

   /* encode contamine_nbc */

   if (pvalue->contamine_nbc != FALSE) {
      PU_PUSHNAME (ctxt_p, "contamine_nbc");

      PU_NEWFIELD (ctxt_p, "boolean");

      stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->contamine_nbc);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_SETBITCOUNT (ctxt_p);
      PU_POPNAME (ctxt_p);
   }


   rtdiag ("asn1PE_MsgLogSanteTraitementHumainCreation: end\n");
   return (stat);
}

int ASN1C_MsgLogSanteTraitementHumainCreation::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgLogSanteTraitementHumainCreation (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgLogSanteTraitementHumainCreation (ASN1CTXT* ctxt_p, ASN1T_MsgLogSanteTraitementHumainCreation* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL optbit;
   ASN1BOOL blesse_mentalPresent;
   ASN1BOOL contamine_nbcPresent;

   rtdiag ("asn1PD_MsgLogSanteTraitementHumainCreation: start\n");

   /* optional bits */

   PU_NEWFIELD (ctxt_p, "blesse_mentalPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else blesse_mentalPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "contamine_nbcPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else contamine_nbcPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   /* decode oid_consigne */

   PU_PUSHNAME (ctxt_p, "oid_consigne");

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_consigne);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode oid_pion */

   PU_PUSHNAME (ctxt_p, "oid_pion");

   stat = asn1PD_Agent (ctxt_p, &pvalue->oid_pion);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode rang */

   PU_PUSHNAME (ctxt_p, "rang");

   stat = asn1PD_EnumHumanRank (ctxt_p, &pvalue->rang);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode blessure */

   PU_PUSHNAME (ctxt_p, "blessure");

   stat = asn1PD_EnumHumanWound (ctxt_p, &pvalue->blessure);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode blesse_mental */

   if (blesse_mentalPresent) {
      PU_PUSHNAME (ctxt_p, "blesse_mental");

      PU_NEWFIELD (ctxt_p, "boolean");

      stat = pd_bit (ctxt_p, &pvalue->blesse_mental);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_SETBITCOUNT (ctxt_p);

      PU_POPNAME (ctxt_p);
   }
   else {
      pvalue->blesse_mental = FALSE;
   }

   /* decode contamine_nbc */

   if (contamine_nbcPresent) {
      PU_PUSHNAME (ctxt_p, "contamine_nbc");

      PU_NEWFIELD (ctxt_p, "boolean");

      stat = pd_bit (ctxt_p, &pvalue->contamine_nbc);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_SETBITCOUNT (ctxt_p);

      PU_POPNAME (ctxt_p);
   }
   else {
      pvalue->contamine_nbc = FALSE;
   }


   rtdiag ("asn1PD_MsgLogSanteTraitementHumainCreation: end\n");

   return (stat);
}

int ASN1C_MsgLogSanteTraitementHumainCreation::Decode ()
{
   return asn1PD_MsgLogSanteTraitementHumainCreation (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgLogSanteTraitementHumainUpdate                         */
/*                                                            */
/**************************************************************/

ASN1C_MsgLogSanteTraitementHumainUpdate::ASN1C_MsgLogSanteTraitementHumainUpdate (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgLogSanteTraitementHumainUpdate& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgLogSanteTraitementHumainUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgLogSanteTraitementHumainUpdate* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgLogSanteTraitementHumainUpdate: start\n");

   PU_NEWFIELD (ctxt_p, "oid_pion_log_traitantPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.oid_pion_log_traitantPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "blessurePresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.blessurePresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "blesse_mentalPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.blesse_mentalPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "contamine_nbcPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.contamine_nbcPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "etatPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.etatPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   /* encode oid_consigne */

   PU_PUSHNAME (ctxt_p, "oid_consigne");

   stat = asn1PE_OID (ctxt_p, pvalue->oid_consigne);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode oid_pion */

   PU_PUSHNAME (ctxt_p, "oid_pion");

   stat = asn1PE_Agent (ctxt_p, pvalue->oid_pion);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode oid_pion_log_traitant */

   if (pvalue->m.oid_pion_log_traitantPresent) {
      PU_PUSHNAME (ctxt_p, "oid_pion_log_traitant");

      stat = asn1PE_Agent (ctxt_p, pvalue->oid_pion_log_traitant);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode blessure */

   if (pvalue->m.blessurePresent) {
      PU_PUSHNAME (ctxt_p, "blessure");

      stat = asn1PE_EnumHumanWound (ctxt_p, pvalue->blessure);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode blesse_mental */

   if (pvalue->m.blesse_mentalPresent) {
      PU_PUSHNAME (ctxt_p, "blesse_mental");

      PU_NEWFIELD (ctxt_p, "boolean");

      stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->blesse_mental);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_SETBITCOUNT (ctxt_p);
      PU_POPNAME (ctxt_p);
   }

   /* encode contamine_nbc */

   if (pvalue->m.contamine_nbcPresent) {
      PU_PUSHNAME (ctxt_p, "contamine_nbc");

      PU_NEWFIELD (ctxt_p, "boolean");

      stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->contamine_nbc);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_SETBITCOUNT (ctxt_p);
      PU_POPNAME (ctxt_p);
   }

   /* encode etat */

   if (pvalue->m.etatPresent) {
      PU_PUSHNAME (ctxt_p, "etat");

      stat = asn1PE_EnumLogSanteTraitementEtat (ctxt_p, pvalue->etat);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }


   rtdiag ("asn1PE_MsgLogSanteTraitementHumainUpdate: end\n");
   return (stat);
}

int ASN1C_MsgLogSanteTraitementHumainUpdate::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgLogSanteTraitementHumainUpdate (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgLogSanteTraitementHumainUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgLogSanteTraitementHumainUpdate* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL optbit;

   rtdiag ("asn1PD_MsgLogSanteTraitementHumainUpdate: start\n");

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   PU_NEWFIELD (ctxt_p, "oid_pion_log_traitantPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.oid_pion_log_traitantPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "blessurePresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.blessurePresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "blesse_mentalPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.blesse_mentalPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "contamine_nbcPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.contamine_nbcPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "etatPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.etatPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   /* decode oid_consigne */

   PU_PUSHNAME (ctxt_p, "oid_consigne");

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_consigne);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode oid_pion */

   PU_PUSHNAME (ctxt_p, "oid_pion");

   stat = asn1PD_Agent (ctxt_p, &pvalue->oid_pion);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode oid_pion_log_traitant */

   if (pvalue->m.oid_pion_log_traitantPresent) {
      PU_PUSHNAME (ctxt_p, "oid_pion_log_traitant");

      stat = asn1PD_Agent (ctxt_p, &pvalue->oid_pion_log_traitant);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode blessure */

   if (pvalue->m.blessurePresent) {
      PU_PUSHNAME (ctxt_p, "blessure");

      stat = asn1PD_EnumHumanWound (ctxt_p, &pvalue->blessure);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode blesse_mental */

   if (pvalue->m.blesse_mentalPresent) {
      PU_PUSHNAME (ctxt_p, "blesse_mental");

      PU_NEWFIELD (ctxt_p, "boolean");

      stat = pd_bit (ctxt_p, &pvalue->blesse_mental);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_SETBITCOUNT (ctxt_p);

      PU_POPNAME (ctxt_p);
   }

   /* decode contamine_nbc */

   if (pvalue->m.contamine_nbcPresent) {
      PU_PUSHNAME (ctxt_p, "contamine_nbc");

      PU_NEWFIELD (ctxt_p, "boolean");

      stat = pd_bit (ctxt_p, &pvalue->contamine_nbc);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_SETBITCOUNT (ctxt_p);

      PU_POPNAME (ctxt_p);
   }

   /* decode etat */

   if (pvalue->m.etatPresent) {
      PU_PUSHNAME (ctxt_p, "etat");

      stat = asn1PD_EnumLogSanteTraitementEtat (ctxt_p, &pvalue->etat);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }


   rtdiag ("asn1PD_MsgLogSanteTraitementHumainUpdate: end\n");

   return (stat);
}

int ASN1C_MsgLogSanteTraitementHumainUpdate::Decode ()
{
   return asn1PD_MsgLogSanteTraitementHumainUpdate (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgLogSanteTraitementHumainDestruction                    */
/*                                                            */
/**************************************************************/

ASN1C_MsgLogSanteTraitementHumainDestruction::ASN1C_MsgLogSanteTraitementHumainDestruction (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgLogSanteTraitementHumainDestruction& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgLogSanteTraitementHumainDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgLogSanteTraitementHumainDestruction* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgLogSanteTraitementHumainDestruction: start\n");

   /* encode oid_consigne */

   PU_PUSHNAME (ctxt_p, "oid_consigne");

   stat = asn1PE_OID (ctxt_p, pvalue->oid_consigne);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode oid_pion */

   PU_PUSHNAME (ctxt_p, "oid_pion");

   stat = asn1PE_Agent (ctxt_p, pvalue->oid_pion);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_MsgLogSanteTraitementHumainDestruction: end\n");
   return (stat);
}

int ASN1C_MsgLogSanteTraitementHumainDestruction::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgLogSanteTraitementHumainDestruction (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgLogSanteTraitementHumainDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgLogSanteTraitementHumainDestruction* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgLogSanteTraitementHumainDestruction: start\n");

   /* decode oid_consigne */

   PU_PUSHNAME (ctxt_p, "oid_consigne");

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_consigne);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode oid_pion */

   PU_PUSHNAME (ctxt_p, "oid_pion");

   stat = asn1PD_Agent (ctxt_p, &pvalue->oid_pion);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_MsgLogSanteTraitementHumainDestruction: end\n");

   return (stat);
}

int ASN1C_MsgLogSanteTraitementHumainDestruction::Decode ()
{
   return asn1PD_MsgLogSanteTraitementHumainDestruction (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  SanteDisponibiliteMoyens                                  */
/*                                                            */
/**************************************************************/

ASN1C_SanteDisponibiliteMoyens::ASN1C_SanteDisponibiliteMoyens (
   ASN1MessageBuffer& msgBuf, ASN1T_SanteDisponibiliteMoyens& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_SanteDisponibiliteMoyens (ASN1CTXT* ctxt_p, ASN1T_SanteDisponibiliteMoyens* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_SanteDisponibiliteMoyens: start\n");

   /* encode type_equipement */

   PU_PUSHNAME (ctxt_p, "type_equipement");

   stat = asn1PE_TypeEquipement (ctxt_p, pvalue->type_equipement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode pourcentage_disponibilite */

   PU_PUSHNAME (ctxt_p, "pourcentage_disponibilite");

   stat = asn1PE_Pourcentage (ctxt_p, pvalue->pourcentage_disponibilite);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_SanteDisponibiliteMoyens: end\n");
   return (stat);
}

int ASN1C_SanteDisponibiliteMoyens::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_SanteDisponibiliteMoyens (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_SanteDisponibiliteMoyens (ASN1CTXT* ctxt_p, ASN1T_SanteDisponibiliteMoyens* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_SanteDisponibiliteMoyens: start\n");

   /* decode type_equipement */

   PU_PUSHNAME (ctxt_p, "type_equipement");

   stat = asn1PD_TypeEquipement (ctxt_p, &pvalue->type_equipement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode pourcentage_disponibilite */

   PU_PUSHNAME (ctxt_p, "pourcentage_disponibilite");

   stat = asn1PD_Pourcentage (ctxt_p, &pvalue->pourcentage_disponibilite);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_SanteDisponibiliteMoyens: end\n");

   return (stat);
}

int ASN1C_SanteDisponibiliteMoyens::Decode ()
{
   return asn1PD_SanteDisponibiliteMoyens (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  SantePriorites                                            */
/*                                                            */
/**************************************************************/

ASN1C_SantePriorites::ASN1C_SantePriorites (
   ASN1MessageBuffer& msgBuf, ASN1T_SantePriorites& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_SantePriorites (ASN1CTXT* ctxt_p, ASN1T_SantePriorites* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   rtdiag ("asn1PE_SantePriorites: start\n");

   /* encode length determinant */

   PU_PUSHNAME (ctxt_p, "n");

   stat = pe_Length (ctxt_p, pvalue->n);
   if (stat < 0) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* encode elements */

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      PU_PUSHELEMNAME (ctxt_p, xx1);

      stat = asn1PE_EnumHumanWound (ctxt_p, pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   rtdiag ("asn1PE_SantePriorites: end\n");
   return (stat);
}

int ASN1C_SantePriorites::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_SantePriorites (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_SantePriorites (ASN1CTXT* ctxt_p, ASN1T_SantePriorites* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   rtdiag ("asn1PD_SantePriorites: start\n");

   /* decode length determinant */

   PU_PUSHNAME (ctxt_p, "n");

   stat = pd_Length (ctxt_p, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode elements */

   ALLOC_ASN1ARRAY (ctxt_p, pvalue, ASN1T_EnumHumanWound);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      PU_PUSHELEMNAME (ctxt_p, xx1);

      stat = asn1PD_EnumHumanWound (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }


   rtdiag ("asn1PD_SantePriorites: end\n");

   return (stat);
}

int ASN1C_SantePriorites::Decode ()
{
   return asn1PD_SantePriorites (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfSanteDisponibiliteMoyens                            */
/*                                                            */
/**************************************************************/

ASN1C__SeqOfSanteDisponibiliteMoyens::ASN1C__SeqOfSanteDisponibiliteMoyens (
   ASN1MessageBuffer& msgBuf, ASN1T__SeqOfSanteDisponibiliteMoyens& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE__SeqOfSanteDisponibiliteMoyens (ASN1CTXT* ctxt_p, ASN1T__SeqOfSanteDisponibiliteMoyens* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   rtdiag ("asn1PE__SeqOfSanteDisponibiliteMoyens: start\n");

   /* encode length determinant */

   PU_PUSHNAME (ctxt_p, "n");

   stat = pe_Length (ctxt_p, pvalue->n);
   if (stat < 0) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* encode elements */

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      PU_PUSHELEMNAME (ctxt_p, xx1);

      stat = asn1PE_SanteDisponibiliteMoyens (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   rtdiag ("asn1PE__SeqOfSanteDisponibiliteMoyens: end\n");
   return (stat);
}

int ASN1C__SeqOfSanteDisponibiliteMoyens::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE__SeqOfSanteDisponibiliteMoyens (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD__SeqOfSanteDisponibiliteMoyens (ASN1CTXT* ctxt_p, ASN1T__SeqOfSanteDisponibiliteMoyens* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   rtdiag ("asn1PD__SeqOfSanteDisponibiliteMoyens: start\n");

   /* decode length determinant */

   PU_PUSHNAME (ctxt_p, "n");

   stat = pd_Length (ctxt_p, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode elements */

   ALLOC_ASN1ARRAY (ctxt_p, pvalue, ASN1T_SanteDisponibiliteMoyens);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      PU_PUSHELEMNAME (ctxt_p, xx1);

      stat = asn1PD_SanteDisponibiliteMoyens (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }


   rtdiag ("asn1PD__SeqOfSanteDisponibiliteMoyens: end\n");

   return (stat);
}

int ASN1C__SeqOfSanteDisponibiliteMoyens::Decode ()
{
   return asn1PD__SeqOfSanteDisponibiliteMoyens (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgLogSanteEtat                                           */
/*                                                            */
/**************************************************************/

ASN1C_MsgLogSanteEtat::ASN1C_MsgLogSanteEtat (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgLogSanteEtat& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgLogSanteEtat (ASN1CTXT* ctxt_p, ASN1T_MsgLogSanteEtat* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgLogSanteEtat: start\n");

   PU_NEWFIELD (ctxt_p, "chaine_activeePresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.chaine_activeePresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "temps_de_bordeePresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.temps_de_bordeePresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "prioritesPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.prioritesPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "priorites_tactiquesPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.priorites_tactiquesPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "disponibilites_ambulances_relevePresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.disponibilites_ambulances_relevePresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "disponibilites_ambulances_ramassagePresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.disponibilites_ambulances_ramassagePresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "disponibilites_medecinsPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.disponibilites_medecinsPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   /* encode oid_pion */

   PU_PUSHNAME (ctxt_p, "oid_pion");

   stat = asn1PE_Agent (ctxt_p, pvalue->oid_pion);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode chaine_activee */

   if (pvalue->m.chaine_activeePresent) {
      PU_PUSHNAME (ctxt_p, "chaine_activee");

      PU_NEWFIELD (ctxt_p, "boolean");

      stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->chaine_activee);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_SETBITCOUNT (ctxt_p);
      PU_POPNAME (ctxt_p);
   }

   /* encode temps_de_bordee */

   if (pvalue->m.temps_de_bordeePresent) {
      PU_PUSHNAME (ctxt_p, "temps_de_bordee");

      stat = asn1PE_EnumTempsBordee (ctxt_p, pvalue->temps_de_bordee);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode priorites */

   if (pvalue->m.prioritesPresent) {
      PU_PUSHNAME (ctxt_p, "priorites");

      stat = asn1PE_SantePriorites (ctxt_p, &pvalue->priorites);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode priorites_tactiques */

   if (pvalue->m.priorites_tactiquesPresent) {
      PU_PUSHNAME (ctxt_p, "priorites_tactiques");

      stat = asn1PE_ListAutomate (ctxt_p, &pvalue->priorites_tactiques);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode disponibilites_ambulances_releve */

   if (pvalue->m.disponibilites_ambulances_relevePresent) {
      PU_PUSHNAME (ctxt_p, "disponibilites_ambulances_releve");

      stat = asn1PE__SeqOfSanteDisponibiliteMoyens (ctxt_p, &pvalue->disponibilites_ambulances_releve);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode disponibilites_ambulances_ramassage */

   if (pvalue->m.disponibilites_ambulances_ramassagePresent) {
      PU_PUSHNAME (ctxt_p, "disponibilites_ambulances_ramassage");

      stat = asn1PE__SeqOfSanteDisponibiliteMoyens (ctxt_p, &pvalue->disponibilites_ambulances_ramassage);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode disponibilites_medecins */

   if (pvalue->m.disponibilites_medecinsPresent) {
      PU_PUSHNAME (ctxt_p, "disponibilites_medecins");

      stat = asn1PE__SeqOfSanteDisponibiliteMoyens (ctxt_p, &pvalue->disponibilites_medecins);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }


   rtdiag ("asn1PE_MsgLogSanteEtat: end\n");
   return (stat);
}

int ASN1C_MsgLogSanteEtat::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgLogSanteEtat (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgLogSanteEtat (ASN1CTXT* ctxt_p, ASN1T_MsgLogSanteEtat* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL optbit;

   rtdiag ("asn1PD_MsgLogSanteEtat: start\n");

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   PU_NEWFIELD (ctxt_p, "chaine_activeePresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.chaine_activeePresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "temps_de_bordeePresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.temps_de_bordeePresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "prioritesPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.prioritesPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "priorites_tactiquesPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.priorites_tactiquesPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "disponibilites_ambulances_relevePresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.disponibilites_ambulances_relevePresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "disponibilites_ambulances_ramassagePresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.disponibilites_ambulances_ramassagePresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "disponibilites_medecinsPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.disponibilites_medecinsPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   /* decode oid_pion */

   PU_PUSHNAME (ctxt_p, "oid_pion");

   stat = asn1PD_Agent (ctxt_p, &pvalue->oid_pion);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode chaine_activee */

   if (pvalue->m.chaine_activeePresent) {
      PU_PUSHNAME (ctxt_p, "chaine_activee");

      PU_NEWFIELD (ctxt_p, "boolean");

      stat = pd_bit (ctxt_p, &pvalue->chaine_activee);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_SETBITCOUNT (ctxt_p);

      PU_POPNAME (ctxt_p);
   }

   /* decode temps_de_bordee */

   if (pvalue->m.temps_de_bordeePresent) {
      PU_PUSHNAME (ctxt_p, "temps_de_bordee");

      stat = asn1PD_EnumTempsBordee (ctxt_p, &pvalue->temps_de_bordee);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode priorites */

   if (pvalue->m.prioritesPresent) {
      PU_PUSHNAME (ctxt_p, "priorites");

      stat = asn1PD_SantePriorites (ctxt_p, &pvalue->priorites);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode priorites_tactiques */

   if (pvalue->m.priorites_tactiquesPresent) {
      PU_PUSHNAME (ctxt_p, "priorites_tactiques");

      stat = asn1PD_ListAutomate (ctxt_p, &pvalue->priorites_tactiques);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode disponibilites_ambulances_releve */

   if (pvalue->m.disponibilites_ambulances_relevePresent) {
      PU_PUSHNAME (ctxt_p, "disponibilites_ambulances_releve");

      stat = asn1PD__SeqOfSanteDisponibiliteMoyens (ctxt_p, &pvalue->disponibilites_ambulances_releve);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode disponibilites_ambulances_ramassage */

   if (pvalue->m.disponibilites_ambulances_ramassagePresent) {
      PU_PUSHNAME (ctxt_p, "disponibilites_ambulances_ramassage");

      stat = asn1PD__SeqOfSanteDisponibiliteMoyens (ctxt_p, &pvalue->disponibilites_ambulances_ramassage);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode disponibilites_medecins */

   if (pvalue->m.disponibilites_medecinsPresent) {
      PU_PUSHNAME (ctxt_p, "disponibilites_medecins");

      stat = asn1PD__SeqOfSanteDisponibiliteMoyens (ctxt_p, &pvalue->disponibilites_medecins);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }


   rtdiag ("asn1PD_MsgLogSanteEtat: end\n");

   return (stat);
}

int ASN1C_MsgLogSanteEtat::Decode ()
{
   return asn1PD_MsgLogSanteEtat (mpContext->GetPtr(), &msgData);
}


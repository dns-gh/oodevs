/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 17-Aug-2005.
 */
#include "ScipioLogMaintenance.h"

/**************************************************************/
/*                                                            */
/*  MsgLogMaintenanceTraitementEquipementCreation             */
/*                                                            */
/**************************************************************/

ASN1C_MsgLogMaintenanceTraitementEquipementCreation::ASN1C_MsgLogMaintenanceTraitementEquipementCreation (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgLogMaintenanceTraitementEquipementCreation& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgLogMaintenanceTraitementEquipementCreation (ASN1CTXT* ctxt_p, ASN1T_MsgLogMaintenanceTraitementEquipementCreation* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgLogMaintenanceTraitementEquipementCreation: start\n");

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

   /* encode type_equipement */

   PU_PUSHNAME (ctxt_p, "type_equipement");

   stat = asn1PE_TypeEquipement (ctxt_p, pvalue->type_equipement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode type_panne */

   PU_PUSHNAME (ctxt_p, "type_panne");

   stat = asn1PE_TypePanne (ctxt_p, pvalue->type_panne);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_MsgLogMaintenanceTraitementEquipementCreation: end\n");
   return (stat);
}

int ASN1C_MsgLogMaintenanceTraitementEquipementCreation::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgLogMaintenanceTraitementEquipementCreation (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgLogMaintenanceTraitementEquipementCreation (ASN1CTXT* ctxt_p, ASN1T_MsgLogMaintenanceTraitementEquipementCreation* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgLogMaintenanceTraitementEquipementCreation: start\n");

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

   /* decode type_equipement */

   PU_PUSHNAME (ctxt_p, "type_equipement");

   stat = asn1PD_TypeEquipement (ctxt_p, &pvalue->type_equipement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode type_panne */

   PU_PUSHNAME (ctxt_p, "type_panne");

   stat = asn1PD_TypePanne (ctxt_p, &pvalue->type_panne);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_MsgLogMaintenanceTraitementEquipementCreation: end\n");

   return (stat);
}

int ASN1C_MsgLogMaintenanceTraitementEquipementCreation::Decode ()
{
   return asn1PD_MsgLogMaintenanceTraitementEquipementCreation (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  EnumLogMaintenanceTraitementEtat                          */
/*                                                            */
/**************************************************************/

ASN1C_EnumLogMaintenanceTraitementEtat::ASN1C_EnumLogMaintenanceTraitementEtat (
   ASN1MessageBuffer& msgBuf, ASN1T_EnumLogMaintenanceTraitementEtat& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_EnumLogMaintenanceTraitementEtat (ASN1CTXT* ctxt_p, ASN1T_EnumLogMaintenanceTraitementEtat value)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   rtdiag ("asn1PE_EnumLogMaintenanceTraitementEtat: start\n");

   switch (value) {
      case EnumLogMaintenanceTraitementEtat::deplacement_vers_chaine: ui = 0; break;
      case EnumLogMaintenanceTraitementEtat::attente_disponibilite_remorqueur: ui = 1; break;
      case EnumLogMaintenanceTraitementEtat::remorqueur_deplacement_aller: ui = 2; break;
      case EnumLogMaintenanceTraitementEtat::remorqueur_chargement: ui = 3; break;
      case EnumLogMaintenanceTraitementEtat::remorqueur_deplacement_retour: ui = 4; break;
      case EnumLogMaintenanceTraitementEtat::remorqueur_dechargement: ui = 5; break;
      case EnumLogMaintenanceTraitementEtat::diagnostique: ui = 6; break;
      case EnumLogMaintenanceTraitementEtat::attente_prise_en_charge_par_niveau_superieur: ui = 7; break;
      case EnumLogMaintenanceTraitementEtat::attente_disponibilite_pieces: ui = 8; break;
      case EnumLogMaintenanceTraitementEtat::attente_disponibilite_reparateur: ui = 9; break;
      case EnumLogMaintenanceTraitementEtat::reparation: ui = 10; break;
      case EnumLogMaintenanceTraitementEtat::attente_retour_pion: ui = 11; break;
      case EnumLogMaintenanceTraitementEtat::retour_pion: ui = 12; break;
      case EnumLogMaintenanceTraitementEtat::termine: ui = 13; break;
      default: return LOG_ASN1ERR (ctxt_p, ASN_E_INVENUM);
   }

   stat = pe_ConsUnsigned (ctxt_p, ui, 0, 13);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   rtdiag ("asn1PE_EnumLogMaintenanceTraitementEtat: end\n");
   return (stat);
}

int ASN1C_EnumLogMaintenanceTraitementEtat::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_EnumLogMaintenanceTraitementEtat (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_EnumLogMaintenanceTraitementEtat (ASN1CTXT* ctxt_p, ASN1T_EnumLogMaintenanceTraitementEtat* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   rtdiag ("asn1PD_EnumLogMaintenanceTraitementEtat: start\n");

   stat = pd_ConsUnsigned (ctxt_p, &ui, 0, 13);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   switch (ui) {
      case 0: *pvalue = EnumLogMaintenanceTraitementEtat::deplacement_vers_chaine; break;
      case 1: *pvalue = EnumLogMaintenanceTraitementEtat::attente_disponibilite_remorqueur; break;
      case 2: *pvalue = EnumLogMaintenanceTraitementEtat::remorqueur_deplacement_aller; break;
      case 3: *pvalue = EnumLogMaintenanceTraitementEtat::remorqueur_chargement; break;
      case 4: *pvalue = EnumLogMaintenanceTraitementEtat::remorqueur_deplacement_retour; break;
      case 5: *pvalue = EnumLogMaintenanceTraitementEtat::remorqueur_dechargement; break;
      case 6: *pvalue = EnumLogMaintenanceTraitementEtat::diagnostique; break;
      case 7: *pvalue = EnumLogMaintenanceTraitementEtat::attente_prise_en_charge_par_niveau_superieur; break;
      case 8: *pvalue = EnumLogMaintenanceTraitementEtat::attente_disponibilite_pieces; break;
      case 9: *pvalue = EnumLogMaintenanceTraitementEtat::attente_disponibilite_reparateur; break;
      case 10: *pvalue = EnumLogMaintenanceTraitementEtat::reparation; break;
      case 11: *pvalue = EnumLogMaintenanceTraitementEtat::attente_retour_pion; break;
      case 12: *pvalue = EnumLogMaintenanceTraitementEtat::retour_pion; break;
      case 13: *pvalue = EnumLogMaintenanceTraitementEtat::termine; break;
      default: return LOG_ASN1ERR (ctxt_p, ASN_E_INVENUM);
   }

   rtdiag ("asn1PD_EnumLogMaintenanceTraitementEtat: end\n");

   return (stat);
}

int ASN1C_EnumLogMaintenanceTraitementEtat::Decode ()
{
   return asn1PD_EnumLogMaintenanceTraitementEtat (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgLogMaintenanceTraitementEquipementUpdate               */
/*                                                            */
/**************************************************************/

ASN1C_MsgLogMaintenanceTraitementEquipementUpdate::ASN1C_MsgLogMaintenanceTraitementEquipementUpdate (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgLogMaintenanceTraitementEquipementUpdate& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgLogMaintenanceTraitementEquipementUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgLogMaintenanceTraitementEquipementUpdate* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgLogMaintenanceTraitementEquipementUpdate: start\n");

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

   PU_PUSHNAME (ctxt_p, "oid_pion_log_traitant");

   stat = asn1PE_Agent (ctxt_p, pvalue->oid_pion_log_traitant);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode etat */

   PU_PUSHNAME (ctxt_p, "etat");

   stat = asn1PE_EnumLogMaintenanceTraitementEtat (ctxt_p, pvalue->etat);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_MsgLogMaintenanceTraitementEquipementUpdate: end\n");
   return (stat);
}

int ASN1C_MsgLogMaintenanceTraitementEquipementUpdate::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgLogMaintenanceTraitementEquipementUpdate (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgLogMaintenanceTraitementEquipementUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgLogMaintenanceTraitementEquipementUpdate* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgLogMaintenanceTraitementEquipementUpdate: start\n");

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

   PU_PUSHNAME (ctxt_p, "oid_pion_log_traitant");

   stat = asn1PD_Agent (ctxt_p, &pvalue->oid_pion_log_traitant);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode etat */

   PU_PUSHNAME (ctxt_p, "etat");

   stat = asn1PD_EnumLogMaintenanceTraitementEtat (ctxt_p, &pvalue->etat);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_MsgLogMaintenanceTraitementEquipementUpdate: end\n");

   return (stat);
}

int ASN1C_MsgLogMaintenanceTraitementEquipementUpdate::Decode ()
{
   return asn1PD_MsgLogMaintenanceTraitementEquipementUpdate (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgLogMaintenanceTraitementEquipementDestruction          */
/*                                                            */
/**************************************************************/

ASN1C_MsgLogMaintenanceTraitementEquipementDestruction::ASN1C_MsgLogMaintenanceTraitementEquipementDestruction (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgLogMaintenanceTraitementEquipementDestruction& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgLogMaintenanceTraitementEquipementDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgLogMaintenanceTraitementEquipementDestruction* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgLogMaintenanceTraitementEquipementDestruction: start\n");

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


   rtdiag ("asn1PE_MsgLogMaintenanceTraitementEquipementDestruction: end\n");
   return (stat);
}

int ASN1C_MsgLogMaintenanceTraitementEquipementDestruction::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgLogMaintenanceTraitementEquipementDestruction (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgLogMaintenanceTraitementEquipementDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgLogMaintenanceTraitementEquipementDestruction* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgLogMaintenanceTraitementEquipementDestruction: start\n");

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


   rtdiag ("asn1PD_MsgLogMaintenanceTraitementEquipementDestruction: end\n");

   return (stat);
}

int ASN1C_MsgLogMaintenanceTraitementEquipementDestruction::Decode ()
{
   return asn1PD_MsgLogMaintenanceTraitementEquipementDestruction (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MaintenanceDisponibiliteMoyens                            */
/*                                                            */
/**************************************************************/

ASN1C_MaintenanceDisponibiliteMoyens::ASN1C_MaintenanceDisponibiliteMoyens (
   ASN1MessageBuffer& msgBuf, ASN1T_MaintenanceDisponibiliteMoyens& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MaintenanceDisponibiliteMoyens (ASN1CTXT* ctxt_p, ASN1T_MaintenanceDisponibiliteMoyens* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MaintenanceDisponibiliteMoyens: start\n");

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


   rtdiag ("asn1PE_MaintenanceDisponibiliteMoyens: end\n");
   return (stat);
}

int ASN1C_MaintenanceDisponibiliteMoyens::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MaintenanceDisponibiliteMoyens (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MaintenanceDisponibiliteMoyens (ASN1CTXT* ctxt_p, ASN1T_MaintenanceDisponibiliteMoyens* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MaintenanceDisponibiliteMoyens: start\n");

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


   rtdiag ("asn1PD_MaintenanceDisponibiliteMoyens: end\n");

   return (stat);
}

int ASN1C_MaintenanceDisponibiliteMoyens::Decode ()
{
   return asn1PD_MaintenanceDisponibiliteMoyens (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MaintenancePriorites                                      */
/*                                                            */
/**************************************************************/

ASN1C_MaintenancePriorites::ASN1C_MaintenancePriorites (
   ASN1MessageBuffer& msgBuf, ASN1T_MaintenancePriorites& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MaintenancePriorites (ASN1CTXT* ctxt_p, ASN1T_MaintenancePriorites* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   rtdiag ("asn1PE_MaintenancePriorites: start\n");

   /* encode length determinant */

   PU_PUSHNAME (ctxt_p, "n");

   stat = pe_Length (ctxt_p, pvalue->n);
   if (stat < 0) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* encode elements */

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      PU_PUSHELEMNAME (ctxt_p, xx1);

      stat = asn1PE_TypeEquipement (ctxt_p, pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   rtdiag ("asn1PE_MaintenancePriorites: end\n");
   return (stat);
}

int ASN1C_MaintenancePriorites::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MaintenancePriorites (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MaintenancePriorites (ASN1CTXT* ctxt_p, ASN1T_MaintenancePriorites* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   rtdiag ("asn1PD_MaintenancePriorites: start\n");

   /* decode length determinant */

   PU_PUSHNAME (ctxt_p, "n");

   stat = pd_Length (ctxt_p, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode elements */

   ALLOC_ASN1ARRAY (ctxt_p, pvalue, ASN1T_TypeEquipement);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      PU_PUSHELEMNAME (ctxt_p, xx1);

      stat = asn1PD_TypeEquipement (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }


   rtdiag ("asn1PD_MaintenancePriorites: end\n");

   return (stat);
}

int ASN1C_MaintenancePriorites::Decode ()
{
   return asn1PD_MaintenancePriorites (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfMaintenanceDisponibiliteMoyens                      */
/*                                                            */
/**************************************************************/

ASN1C__SeqOfMaintenanceDisponibiliteMoyens::ASN1C__SeqOfMaintenanceDisponibiliteMoyens (
   ASN1MessageBuffer& msgBuf, ASN1T__SeqOfMaintenanceDisponibiliteMoyens& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE__SeqOfMaintenanceDisponibiliteMoyens (ASN1CTXT* ctxt_p, ASN1T__SeqOfMaintenanceDisponibiliteMoyens* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   rtdiag ("asn1PE__SeqOfMaintenanceDisponibiliteMoyens: start\n");

   /* encode length determinant */

   PU_PUSHNAME (ctxt_p, "n");

   stat = pe_Length (ctxt_p, pvalue->n);
   if (stat < 0) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* encode elements */

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      PU_PUSHELEMNAME (ctxt_p, xx1);

      stat = asn1PE_MaintenanceDisponibiliteMoyens (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   rtdiag ("asn1PE__SeqOfMaintenanceDisponibiliteMoyens: end\n");
   return (stat);
}

int ASN1C__SeqOfMaintenanceDisponibiliteMoyens::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE__SeqOfMaintenanceDisponibiliteMoyens (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD__SeqOfMaintenanceDisponibiliteMoyens (ASN1CTXT* ctxt_p, ASN1T__SeqOfMaintenanceDisponibiliteMoyens* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   rtdiag ("asn1PD__SeqOfMaintenanceDisponibiliteMoyens: start\n");

   /* decode length determinant */

   PU_PUSHNAME (ctxt_p, "n");

   stat = pd_Length (ctxt_p, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode elements */

   ALLOC_ASN1ARRAY (ctxt_p, pvalue, ASN1T_MaintenanceDisponibiliteMoyens);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      PU_PUSHELEMNAME (ctxt_p, xx1);

      stat = asn1PD_MaintenanceDisponibiliteMoyens (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }


   rtdiag ("asn1PD__SeqOfMaintenanceDisponibiliteMoyens: end\n");

   return (stat);
}

int ASN1C__SeqOfMaintenanceDisponibiliteMoyens::Decode ()
{
   return asn1PD__SeqOfMaintenanceDisponibiliteMoyens (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgLogMaintenanceEtat                                     */
/*                                                            */
/**************************************************************/

ASN1C_MsgLogMaintenanceEtat::ASN1C_MsgLogMaintenanceEtat (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgLogMaintenanceEtat& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgLogMaintenanceEtat (ASN1CTXT* ctxt_p, ASN1T_MsgLogMaintenanceEtat* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgLogMaintenanceEtat: start\n");

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

   PU_NEWFIELD (ctxt_p, "disponibilites_remorqueursPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.disponibilites_remorqueursPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "disponibilites_reparateursPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.disponibilites_reparateursPresent);
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

      stat = asn1PE_MaintenancePriorites (ctxt_p, &pvalue->priorites);
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

   /* encode disponibilites_remorqueurs */

   if (pvalue->m.disponibilites_remorqueursPresent) {
      PU_PUSHNAME (ctxt_p, "disponibilites_remorqueurs");

      stat = asn1PE__SeqOfMaintenanceDisponibiliteMoyens (ctxt_p, &pvalue->disponibilites_remorqueurs);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode disponibilites_reparateurs */

   if (pvalue->m.disponibilites_reparateursPresent) {
      PU_PUSHNAME (ctxt_p, "disponibilites_reparateurs");

      stat = asn1PE__SeqOfMaintenanceDisponibiliteMoyens (ctxt_p, &pvalue->disponibilites_reparateurs);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }


   rtdiag ("asn1PE_MsgLogMaintenanceEtat: end\n");
   return (stat);
}

int ASN1C_MsgLogMaintenanceEtat::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgLogMaintenanceEtat (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgLogMaintenanceEtat (ASN1CTXT* ctxt_p, ASN1T_MsgLogMaintenanceEtat* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL optbit;

   rtdiag ("asn1PD_MsgLogMaintenanceEtat: start\n");

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

   PU_NEWFIELD (ctxt_p, "disponibilites_remorqueursPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.disponibilites_remorqueursPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "disponibilites_reparateursPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.disponibilites_reparateursPresent = optbit;

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

      stat = asn1PD_MaintenancePriorites (ctxt_p, &pvalue->priorites);
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

   /* decode disponibilites_remorqueurs */

   if (pvalue->m.disponibilites_remorqueursPresent) {
      PU_PUSHNAME (ctxt_p, "disponibilites_remorqueurs");

      stat = asn1PD__SeqOfMaintenanceDisponibiliteMoyens (ctxt_p, &pvalue->disponibilites_remorqueurs);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode disponibilites_reparateurs */

   if (pvalue->m.disponibilites_reparateursPresent) {
      PU_PUSHNAME (ctxt_p, "disponibilites_reparateurs");

      stat = asn1PD__SeqOfMaintenanceDisponibiliteMoyens (ctxt_p, &pvalue->disponibilites_reparateurs);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }


   rtdiag ("asn1PD_MsgLogMaintenanceEtat: end\n");

   return (stat);
}

int ASN1C_MsgLogMaintenanceEtat::Decode ()
{
   return asn1PD_MsgLogMaintenanceEtat (mpContext->GetPtr(), &msgData);
}


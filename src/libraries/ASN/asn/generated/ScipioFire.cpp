/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 29-Sep-2005.
 */
#include "ScipioFire.h"

/**************************************************************/
/*                                                            */
/*  FireResultHuman                                           */
/*                                                            */
/**************************************************************/

ASN1C_FireResultHuman::ASN1C_FireResultHuman (
   ASN1MessageBuffer& msgBuf, ASN1T_FireResultHuman& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_FireResultHuman (ASN1CTXT* ctxt_p, ASN1T_FireResultHuman* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_FireResultHuman: start\n");

   /* encode rang */

   PU_PUSHNAME (ctxt_p, "rang");

   stat = asn1PE_EnumHumanRank (ctxt_p, pvalue->rang);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode nb_non_blesses */

   PU_PUSHNAME (ctxt_p, "nb_non_blesses");

   stat = pe_UnconsInteger (ctxt_p, pvalue->nb_non_blesses);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode nb_morts */

   PU_PUSHNAME (ctxt_p, "nb_morts");

   stat = pe_UnconsInteger (ctxt_p, pvalue->nb_morts);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode nb_blesses_urgence_1 */

   PU_PUSHNAME (ctxt_p, "nb_blesses_urgence_1");

   stat = pe_UnconsInteger (ctxt_p, pvalue->nb_blesses_urgence_1);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode nb_blesses_urgence_2 */

   PU_PUSHNAME (ctxt_p, "nb_blesses_urgence_2");

   stat = pe_UnconsInteger (ctxt_p, pvalue->nb_blesses_urgence_2);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode nb_blesses_urgence_3 */

   PU_PUSHNAME (ctxt_p, "nb_blesses_urgence_3");

   stat = pe_UnconsInteger (ctxt_p, pvalue->nb_blesses_urgence_3);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode nb_blesses_urgence_extreme */

   PU_PUSHNAME (ctxt_p, "nb_blesses_urgence_extreme");

   stat = pe_UnconsInteger (ctxt_p, pvalue->nb_blesses_urgence_extreme);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_FireResultHuman: end\n");
   return (stat);
}

int ASN1C_FireResultHuman::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_FireResultHuman (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_FireResultHuman (ASN1CTXT* ctxt_p, ASN1T_FireResultHuman* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_FireResultHuman: start\n");

   /* decode rang */

   PU_PUSHNAME (ctxt_p, "rang");

   stat = asn1PD_EnumHumanRank (ctxt_p, &pvalue->rang);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode nb_non_blesses */

   PU_PUSHNAME (ctxt_p, "nb_non_blesses");

   stat = pd_UnconsInteger (ctxt_p, &pvalue->nb_non_blesses);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode nb_morts */

   PU_PUSHNAME (ctxt_p, "nb_morts");

   stat = pd_UnconsInteger (ctxt_p, &pvalue->nb_morts);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode nb_blesses_urgence_1 */

   PU_PUSHNAME (ctxt_p, "nb_blesses_urgence_1");

   stat = pd_UnconsInteger (ctxt_p, &pvalue->nb_blesses_urgence_1);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode nb_blesses_urgence_2 */

   PU_PUSHNAME (ctxt_p, "nb_blesses_urgence_2");

   stat = pd_UnconsInteger (ctxt_p, &pvalue->nb_blesses_urgence_2);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode nb_blesses_urgence_3 */

   PU_PUSHNAME (ctxt_p, "nb_blesses_urgence_3");

   stat = pd_UnconsInteger (ctxt_p, &pvalue->nb_blesses_urgence_3);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode nb_blesses_urgence_extreme */

   PU_PUSHNAME (ctxt_p, "nb_blesses_urgence_extreme");

   stat = pd_UnconsInteger (ctxt_p, &pvalue->nb_blesses_urgence_extreme);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_FireResultHuman: end\n");

   return (stat);
}

int ASN1C_FireResultHuman::Decode ()
{
   return asn1PD_FireResultHuman (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  FireResultEquipement                                      */
/*                                                            */
/**************************************************************/

ASN1C_FireResultEquipement::ASN1C_FireResultEquipement (
   ASN1MessageBuffer& msgBuf, ASN1T_FireResultEquipement& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_FireResultEquipement (ASN1CTXT* ctxt_p, ASN1T_FireResultEquipement* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_FireResultEquipement: start\n");

   /* encode type_equipement */

   PU_PUSHNAME (ctxt_p, "type_equipement");

   stat = asn1PE_TypeEquipement (ctxt_p, pvalue->type_equipement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode nb_disponibles */

   PU_PUSHNAME (ctxt_p, "nb_disponibles");

   stat = pe_UnconsInteger (ctxt_p, pvalue->nb_disponibles);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode nb_indisponibles */

   PU_PUSHNAME (ctxt_p, "nb_indisponibles");

   stat = pe_UnconsInteger (ctxt_p, pvalue->nb_indisponibles);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode nb_reparables */

   PU_PUSHNAME (ctxt_p, "nb_reparables");

   stat = pe_UnconsInteger (ctxt_p, pvalue->nb_reparables);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_FireResultEquipement: end\n");
   return (stat);
}

int ASN1C_FireResultEquipement::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_FireResultEquipement (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_FireResultEquipement (ASN1CTXT* ctxt_p, ASN1T_FireResultEquipement* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_FireResultEquipement: start\n");

   /* decode type_equipement */

   PU_PUSHNAME (ctxt_p, "type_equipement");

   stat = asn1PD_TypeEquipement (ctxt_p, &pvalue->type_equipement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode nb_disponibles */

   PU_PUSHNAME (ctxt_p, "nb_disponibles");

   stat = pd_UnconsInteger (ctxt_p, &pvalue->nb_disponibles);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode nb_indisponibles */

   PU_PUSHNAME (ctxt_p, "nb_indisponibles");

   stat = pd_UnconsInteger (ctxt_p, &pvalue->nb_indisponibles);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode nb_reparables */

   PU_PUSHNAME (ctxt_p, "nb_reparables");

   stat = pd_UnconsInteger (ctxt_p, &pvalue->nb_reparables);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_FireResultEquipement: end\n");

   return (stat);
}

int ASN1C_FireResultEquipement::Decode ()
{
   return asn1PD_FireResultEquipement (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfFireResultHuman                                     */
/*                                                            */
/**************************************************************/

ASN1C__SeqOfFireResultHuman::ASN1C__SeqOfFireResultHuman (
   ASN1MessageBuffer& msgBuf, ASN1T__SeqOfFireResultHuman& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE__SeqOfFireResultHuman (ASN1CTXT* ctxt_p, ASN1T__SeqOfFireResultHuman* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   rtdiag ("asn1PE__SeqOfFireResultHuman: start\n");

   /* encode length determinant */

   PU_PUSHNAME (ctxt_p, "n");

   stat = pe_Length (ctxt_p, pvalue->n);
   if (stat < 0) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* encode elements */

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      PU_PUSHELEMNAME (ctxt_p, xx1);

      stat = asn1PE_FireResultHuman (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   rtdiag ("asn1PE__SeqOfFireResultHuman: end\n");
   return (stat);
}

int ASN1C__SeqOfFireResultHuman::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE__SeqOfFireResultHuman (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD__SeqOfFireResultHuman (ASN1CTXT* ctxt_p, ASN1T__SeqOfFireResultHuman* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   rtdiag ("asn1PD__SeqOfFireResultHuman: start\n");

   /* decode length determinant */

   PU_PUSHNAME (ctxt_p, "n");

   stat = pd_Length (ctxt_p, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode elements */

   ALLOC_ASN1ARRAY (ctxt_p, pvalue, ASN1T_FireResultHuman);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      PU_PUSHELEMNAME (ctxt_p, xx1);

      stat = asn1PD_FireResultHuman (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }


   rtdiag ("asn1PD__SeqOfFireResultHuman: end\n");

   return (stat);
}

int ASN1C__SeqOfFireResultHuman::Decode ()
{
   return asn1PD__SeqOfFireResultHuman (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfFireResultEquipement                                */
/*                                                            */
/**************************************************************/

ASN1C__SeqOfFireResultEquipement::ASN1C__SeqOfFireResultEquipement (
   ASN1MessageBuffer& msgBuf, ASN1T__SeqOfFireResultEquipement& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE__SeqOfFireResultEquipement (ASN1CTXT* ctxt_p, ASN1T__SeqOfFireResultEquipement* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   rtdiag ("asn1PE__SeqOfFireResultEquipement: start\n");

   /* encode length determinant */

   PU_PUSHNAME (ctxt_p, "n");

   stat = pe_Length (ctxt_p, pvalue->n);
   if (stat < 0) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* encode elements */

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      PU_PUSHELEMNAME (ctxt_p, xx1);

      stat = asn1PE_FireResultEquipement (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   rtdiag ("asn1PE__SeqOfFireResultEquipement: end\n");
   return (stat);
}

int ASN1C__SeqOfFireResultEquipement::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE__SeqOfFireResultEquipement (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD__SeqOfFireResultEquipement (ASN1CTXT* ctxt_p, ASN1T__SeqOfFireResultEquipement* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   rtdiag ("asn1PD__SeqOfFireResultEquipement: start\n");

   /* decode length determinant */

   PU_PUSHNAME (ctxt_p, "n");

   stat = pd_Length (ctxt_p, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode elements */

   ALLOC_ASN1ARRAY (ctxt_p, pvalue, ASN1T_FireResultEquipement);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      PU_PUSHELEMNAME (ctxt_p, xx1);

      stat = asn1PD_FireResultEquipement (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }


   rtdiag ("asn1PD__SeqOfFireResultEquipement: end\n");

   return (stat);
}

int ASN1C__SeqOfFireResultEquipement::Decode ()
{
   return asn1PD__SeqOfFireResultEquipement (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  FireResult                                                */
/*                                                            */
/**************************************************************/

ASN1C_FireResult::ASN1C_FireResult (
   ASN1MessageBuffer& msgBuf, ASN1T_FireResult& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_FireResult (ASN1CTXT* ctxt_p, ASN1T_FireResult* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_FireResult: start\n");

   /* encode oid_cible */

   PU_PUSHNAME (ctxt_p, "oid_cible");

   stat = asn1PE_Agent (ctxt_p, pvalue->oid_cible);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode resultats_humains */

   PU_PUSHNAME (ctxt_p, "resultats_humains");

   stat = asn1PE__SeqOfFireResultHuman (ctxt_p, &pvalue->resultats_humains);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode resultats_equipements */

   PU_PUSHNAME (ctxt_p, "resultats_equipements");

   stat = asn1PE__SeqOfFireResultEquipement (ctxt_p, &pvalue->resultats_equipements);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_FireResult: end\n");
   return (stat);
}

int ASN1C_FireResult::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_FireResult (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_FireResult (ASN1CTXT* ctxt_p, ASN1T_FireResult* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_FireResult: start\n");

   /* decode oid_cible */

   PU_PUSHNAME (ctxt_p, "oid_cible");

   stat = asn1PD_Agent (ctxt_p, &pvalue->oid_cible);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode resultats_humains */

   PU_PUSHNAME (ctxt_p, "resultats_humains");

   stat = asn1PD__SeqOfFireResultHuman (ctxt_p, &pvalue->resultats_humains);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode resultats_equipements */

   PU_PUSHNAME (ctxt_p, "resultats_equipements");

   stat = asn1PD__SeqOfFireResultEquipement (ctxt_p, &pvalue->resultats_equipements);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_FireResult: end\n");

   return (stat);
}

int ASN1C_FireResult::Decode ()
{
   return asn1PD_FireResult (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgStartDirectFire                                        */
/*                                                            */
/**************************************************************/

ASN1C_MsgStartDirectFire::ASN1C_MsgStartDirectFire (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgStartDirectFire& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgStartDirectFire (ASN1CTXT* ctxt_p, ASN1T_MsgStartDirectFire* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgStartDirectFire: start\n");

   /* encode oid_tir */

   PU_PUSHNAME (ctxt_p, "oid_tir");

   stat = asn1PE_TirDirect (ctxt_p, pvalue->oid_tir);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode oid_src */

   PU_PUSHNAME (ctxt_p, "oid_src");

   stat = asn1PE_Agent (ctxt_p, pvalue->oid_src);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode oid_dst */

   PU_PUSHNAME (ctxt_p, "oid_dst");

   stat = asn1PE_Agent (ctxt_p, pvalue->oid_dst);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_MsgStartDirectFire: end\n");
   return (stat);
}

int ASN1C_MsgStartDirectFire::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgStartDirectFire (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgStartDirectFire (ASN1CTXT* ctxt_p, ASN1T_MsgStartDirectFire* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgStartDirectFire: start\n");

   /* decode oid_tir */

   PU_PUSHNAME (ctxt_p, "oid_tir");

   stat = asn1PD_TirDirect (ctxt_p, &pvalue->oid_tir);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode oid_src */

   PU_PUSHNAME (ctxt_p, "oid_src");

   stat = asn1PD_Agent (ctxt_p, &pvalue->oid_src);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode oid_dst */

   PU_PUSHNAME (ctxt_p, "oid_dst");

   stat = asn1PD_Agent (ctxt_p, &pvalue->oid_dst);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_MsgStartDirectFire: end\n");

   return (stat);
}

int ASN1C_MsgStartDirectFire::Decode ()
{
   return asn1PD_MsgStartDirectFire (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgStopDirectFire                                         */
/*                                                            */
/**************************************************************/

ASN1C_MsgStopDirectFire::ASN1C_MsgStopDirectFire (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgStopDirectFire& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgStopDirectFire (ASN1CTXT* ctxt_p, ASN1T_MsgStopDirectFire* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgStopDirectFire: start\n");

   /* encode oid_tir */

   PU_PUSHNAME (ctxt_p, "oid_tir");

   stat = asn1PE_TirDirect (ctxt_p, pvalue->oid_tir);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode resultat */

   PU_PUSHNAME (ctxt_p, "resultat");

   stat = asn1PE_FireResult (ctxt_p, &pvalue->resultat);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_MsgStopDirectFire: end\n");
   return (stat);
}

int ASN1C_MsgStopDirectFire::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgStopDirectFire (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgStopDirectFire (ASN1CTXT* ctxt_p, ASN1T_MsgStopDirectFire* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgStopDirectFire: start\n");

   /* decode oid_tir */

   PU_PUSHNAME (ctxt_p, "oid_tir");

   stat = asn1PD_TirDirect (ctxt_p, &pvalue->oid_tir);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode resultat */

   PU_PUSHNAME (ctxt_p, "resultat");

   stat = asn1PD_FireResult (ctxt_p, &pvalue->resultat);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_MsgStopDirectFire: end\n");

   return (stat);
}

int ASN1C_MsgStopDirectFire::Decode ()
{
   return asn1PD_MsgStopDirectFire (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgStartIndirectFire                                      */
/*                                                            */
/**************************************************************/

ASN1C_MsgStartIndirectFire::ASN1C_MsgStartIndirectFire (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgStartIndirectFire& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgStartIndirectFire (ASN1CTXT* ctxt_p, ASN1T_MsgStartIndirectFire* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgStartIndirectFire: start\n");

   /* encode oid_tir */

   PU_PUSHNAME (ctxt_p, "oid_tir");

   stat = asn1PE_TirIndirect (ctxt_p, pvalue->oid_tir);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode oid_src */

   PU_PUSHNAME (ctxt_p, "oid_src");

   stat = asn1PE_Agent (ctxt_p, pvalue->oid_src);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode position */

   PU_PUSHNAME (ctxt_p, "position");

   stat = asn1PE_CoordUTM (ctxt_p, pvalue->position);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode munition */

   PU_PUSHNAME (ctxt_p, "munition");

   stat = asn1PE_TypeDotation (ctxt_p, pvalue->munition);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_MsgStartIndirectFire: end\n");
   return (stat);
}

int ASN1C_MsgStartIndirectFire::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgStartIndirectFire (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgStartIndirectFire (ASN1CTXT* ctxt_p, ASN1T_MsgStartIndirectFire* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgStartIndirectFire: start\n");

   /* decode oid_tir */

   PU_PUSHNAME (ctxt_p, "oid_tir");

   stat = asn1PD_TirIndirect (ctxt_p, &pvalue->oid_tir);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode oid_src */

   PU_PUSHNAME (ctxt_p, "oid_src");

   stat = asn1PD_Agent (ctxt_p, &pvalue->oid_src);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode position */

   PU_PUSHNAME (ctxt_p, "position");

   stat = asn1PD_CoordUTM (ctxt_p, &pvalue->position);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode munition */

   PU_PUSHNAME (ctxt_p, "munition");

   stat = asn1PD_TypeDotation (ctxt_p, &pvalue->munition);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_MsgStartIndirectFire: end\n");

   return (stat);
}

int ASN1C_MsgStartIndirectFire::Decode ()
{
   return asn1PD_MsgStartIndirectFire (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfFireResult                                          */
/*                                                            */
/**************************************************************/

ASN1C__SeqOfFireResult::ASN1C__SeqOfFireResult (
   ASN1MessageBuffer& msgBuf, ASN1T__SeqOfFireResult& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE__SeqOfFireResult (ASN1CTXT* ctxt_p, ASN1T__SeqOfFireResult* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   rtdiag ("asn1PE__SeqOfFireResult: start\n");

   /* encode length determinant */

   PU_PUSHNAME (ctxt_p, "n");

   stat = pe_Length (ctxt_p, pvalue->n);
   if (stat < 0) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* encode elements */

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      PU_PUSHELEMNAME (ctxt_p, xx1);

      stat = asn1PE_FireResult (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   rtdiag ("asn1PE__SeqOfFireResult: end\n");
   return (stat);
}

int ASN1C__SeqOfFireResult::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE__SeqOfFireResult (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD__SeqOfFireResult (ASN1CTXT* ctxt_p, ASN1T__SeqOfFireResult* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   rtdiag ("asn1PD__SeqOfFireResult: start\n");

   /* decode length determinant */

   PU_PUSHNAME (ctxt_p, "n");

   stat = pd_Length (ctxt_p, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode elements */

   ALLOC_ASN1ARRAY (ctxt_p, pvalue, ASN1T_FireResult);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      PU_PUSHELEMNAME (ctxt_p, xx1);

      stat = asn1PD_FireResult (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }


   rtdiag ("asn1PD__SeqOfFireResult: end\n");

   return (stat);
}

int ASN1C__SeqOfFireResult::Decode ()
{
   return asn1PD__SeqOfFireResult (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgStopIndirectFire                                       */
/*                                                            */
/**************************************************************/

ASN1C_MsgStopIndirectFire::ASN1C_MsgStopIndirectFire (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgStopIndirectFire& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgStopIndirectFire (ASN1CTXT* ctxt_p, ASN1T_MsgStopIndirectFire* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgStopIndirectFire: start\n");

   /* encode oid_tir */

   PU_PUSHNAME (ctxt_p, "oid_tir");

   stat = asn1PE_TirIndirect (ctxt_p, pvalue->oid_tir);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode resultats */

   PU_PUSHNAME (ctxt_p, "resultats");

   stat = asn1PE__SeqOfFireResult (ctxt_p, &pvalue->resultats);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_MsgStopIndirectFire: end\n");
   return (stat);
}

int ASN1C_MsgStopIndirectFire::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgStopIndirectFire (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgStopIndirectFire (ASN1CTXT* ctxt_p, ASN1T_MsgStopIndirectFire* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgStopIndirectFire: start\n");

   /* decode oid_tir */

   PU_PUSHNAME (ctxt_p, "oid_tir");

   stat = asn1PD_TirIndirect (ctxt_p, &pvalue->oid_tir);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode resultats */

   PU_PUSHNAME (ctxt_p, "resultats");

   stat = asn1PD__SeqOfFireResult (ctxt_p, &pvalue->resultats);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_MsgStopIndirectFire: end\n");

   return (stat);
}

int ASN1C_MsgStopIndirectFire::Decode ()
{
   return asn1PD_MsgStopIndirectFire (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgExplosion                                              */
/*                                                            */
/**************************************************************/

ASN1C_MsgExplosion::ASN1C_MsgExplosion (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgExplosion& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgExplosion (ASN1CTXT* ctxt_p, ASN1T_MsgExplosion* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgExplosion: start\n");

   /* encode oid_objet */

   PU_PUSHNAME (ctxt_p, "oid_objet");

   stat = asn1PE_OID (ctxt_p, pvalue->oid_objet);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode resultats */

   PU_PUSHNAME (ctxt_p, "resultats");

   stat = asn1PE__SeqOfFireResult (ctxt_p, &pvalue->resultats);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_MsgExplosion: end\n");
   return (stat);
}

int ASN1C_MsgExplosion::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgExplosion (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgExplosion (ASN1CTXT* ctxt_p, ASN1T_MsgExplosion* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgExplosion: start\n");

   /* decode oid_objet */

   PU_PUSHNAME (ctxt_p, "oid_objet");

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_objet);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode resultats */

   PU_PUSHNAME (ctxt_p, "resultats");

   stat = asn1PD__SeqOfFireResult (ctxt_p, &pvalue->resultats);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_MsgExplosion: end\n");

   return (stat);
}

int ASN1C_MsgExplosion::Decode ()
{
   return asn1PD_MsgExplosion (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  EnumFireEffectType                                        */
/*                                                            */
/**************************************************************/

ASN1C_EnumFireEffectType::ASN1C_EnumFireEffectType (
   ASN1MessageBuffer& msgBuf, ASN1T_EnumFireEffectType& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_EnumFireEffectType (ASN1CTXT* ctxt_p, ASN1T_EnumFireEffectType value)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   rtdiag ("asn1PE_EnumFireEffectType: start\n");

   switch (value) {
      case EnumFireEffectType::fumigene: ui = 0; break;
      case EnumFireEffectType::eclairant: ui = 1; break;
      default: return LOG_ASN1ERR (ctxt_p, ASN_E_INVENUM);
   }

   stat = pe_ConsUnsigned (ctxt_p, ui, 0, 1);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   rtdiag ("asn1PE_EnumFireEffectType: end\n");
   return (stat);
}

int ASN1C_EnumFireEffectType::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_EnumFireEffectType (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_EnumFireEffectType (ASN1CTXT* ctxt_p, ASN1T_EnumFireEffectType* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   rtdiag ("asn1PD_EnumFireEffectType: start\n");

   stat = pd_ConsUnsigned (ctxt_p, &ui, 0, 1);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   switch (ui) {
      case 0: *pvalue = EnumFireEffectType::fumigene; break;
      case 1: *pvalue = EnumFireEffectType::eclairant; break;
      default: return LOG_ASN1ERR (ctxt_p, ASN_E_INVENUM);
   }

   rtdiag ("asn1PD_EnumFireEffectType: end\n");

   return (stat);
}

int ASN1C_EnumFireEffectType::Decode ()
{
   return asn1PD_EnumFireEffectType (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgStartFireEffect                                        */
/*                                                            */
/**************************************************************/

ASN1C_MsgStartFireEffect::ASN1C_MsgStartFireEffect (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgStartFireEffect& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgStartFireEffect (ASN1CTXT* ctxt_p, ASN1T_MsgStartFireEffect* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgStartFireEffect: start\n");

   /* encode oid_effet */

   PU_PUSHNAME (ctxt_p, "oid_effet");

   stat = asn1PE_OID (ctxt_p, pvalue->oid_effet);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode localisation */

   PU_PUSHNAME (ctxt_p, "localisation");

   stat = asn1PE_Localisation (ctxt_p, &pvalue->localisation);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode type */

   PU_PUSHNAME (ctxt_p, "type");

   stat = asn1PE_EnumFireEffectType (ctxt_p, pvalue->type);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_MsgStartFireEffect: end\n");
   return (stat);
}

int ASN1C_MsgStartFireEffect::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgStartFireEffect (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgStartFireEffect (ASN1CTXT* ctxt_p, ASN1T_MsgStartFireEffect* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgStartFireEffect: start\n");

   /* decode oid_effet */

   PU_PUSHNAME (ctxt_p, "oid_effet");

   stat = asn1PD_OID (ctxt_p, &pvalue->oid_effet);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode localisation */

   PU_PUSHNAME (ctxt_p, "localisation");

   stat = asn1PD_Localisation (ctxt_p, &pvalue->localisation);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode type */

   PU_PUSHNAME (ctxt_p, "type");

   stat = asn1PD_EnumFireEffectType (ctxt_p, &pvalue->type);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_MsgStartFireEffect: end\n");

   return (stat);
}

int ASN1C_MsgStartFireEffect::Decode ()
{
   return asn1PD_MsgStartFireEffect (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgStopFireEffect                                         */
/*                                                            */
/**************************************************************/

ASN1C_MsgStopFireEffect::ASN1C_MsgStopFireEffect (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgStopFireEffect& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgStopFireEffect (ASN1CTXT* ctxt_p, ASN1T_MsgStopFireEffect value)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgStopFireEffect: start\n");

   stat = asn1PE_OID (ctxt_p, value);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   rtdiag ("asn1PE_MsgStopFireEffect: end\n");
   return (stat);
}

int ASN1C_MsgStopFireEffect::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgStopFireEffect (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_MsgStopFireEffect (ASN1CTXT* ctxt_p, ASN1T_MsgStopFireEffect* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgStopFireEffect: start\n");

   stat = asn1PD_OID (ctxt_p, pvalue);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   rtdiag ("asn1PD_MsgStopFireEffect: end\n");

   return (stat);
}

int ASN1C_MsgStopFireEffect::Decode ()
{
   return asn1PD_MsgStopFireEffect (mpContext->GetPtr(), &msgData);
}


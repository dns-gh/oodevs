/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 29-Jul-2005.
 */
#include "ScipioLimaLimite.h"

/**************************************************************/
/*                                                            */
/*  EnumTypeLima                                              */
/*                                                            */
/**************************************************************/

ASN1C_EnumTypeLima::ASN1C_EnumTypeLima (
   ASN1MessageBuffer& msgBuf, ASN1T_EnumTypeLima& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_EnumTypeLima (ASN1CTXT* ctxt_p, ASN1T_EnumTypeLima value)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   rtdiag ("asn1PE_EnumTypeLima: start\n");

   switch (value) {
      case EnumTypeLima::ligne_debouche: ui = 0; break;
      case EnumTypeLima::ligne_changement_attitude: ui = 1; break;
      case EnumTypeLima::ligne_coordination: ui = 2; break;
      case EnumTypeLima::ligne_interdire: ui = 3; break;
      case EnumTypeLima::ligne_objectif: ui = 4; break;
      case EnumTypeLima::ligne_coup_arret: ui = 5; break;
      case EnumTypeLima::ligne_recueil: ui = 6; break;
      case EnumTypeLima::ligne_debut_mission: ui = 7; break;
      case EnumTypeLima::ligne_fin_mission: ui = 8; break;
      case EnumTypeLima::ligne_identification_accueil: ui = 9; break;
      default: return LOG_ASN1ERR (ctxt_p, ASN_E_INVENUM);
   }

   stat = pe_ConsUnsigned (ctxt_p, ui, 0, 9);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   rtdiag ("asn1PE_EnumTypeLima: end\n");
   return (stat);
}

int ASN1C_EnumTypeLima::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_EnumTypeLima (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_EnumTypeLima (ASN1CTXT* ctxt_p, ASN1T_EnumTypeLima* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   rtdiag ("asn1PD_EnumTypeLima: start\n");

   stat = pd_ConsUnsigned (ctxt_p, &ui, 0, 9);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   switch (ui) {
      case 0: *pvalue = EnumTypeLima::ligne_debouche; break;
      case 1: *pvalue = EnumTypeLima::ligne_changement_attitude; break;
      case 2: *pvalue = EnumTypeLima::ligne_coordination; break;
      case 3: *pvalue = EnumTypeLima::ligne_interdire; break;
      case 4: *pvalue = EnumTypeLima::ligne_objectif; break;
      case 5: *pvalue = EnumTypeLima::ligne_coup_arret; break;
      case 6: *pvalue = EnumTypeLima::ligne_recueil; break;
      case 7: *pvalue = EnumTypeLima::ligne_debut_mission; break;
      case 8: *pvalue = EnumTypeLima::ligne_fin_mission; break;
      case 9: *pvalue = EnumTypeLima::ligne_identification_accueil; break;
      default: return LOG_ASN1ERR (ctxt_p, ASN_E_INVENUM);
   }

   rtdiag ("asn1PD_EnumTypeLima: end\n");

   return (stat);
}

int ASN1C_EnumTypeLima::Decode ()
{
   return asn1PD_EnumTypeLima (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgLimitCreation                                          */
/*                                                            */
/**************************************************************/

ASN1C_MsgLimitCreation::ASN1C_MsgLimitCreation (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgLimitCreation& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgLimitCreation (ASN1CTXT* ctxt_p, ASN1T_MsgLimitCreation* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgLimitCreation: start\n");

   PU_NEWFIELD (ctxt_p, "levelPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)(pvalue->level != EnumNatureLevel::ooo));
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   /* encode oid */

   PU_PUSHNAME (ctxt_p, "oid");

   stat = asn1PE_OID (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode level */

   if (pvalue->level != EnumNatureLevel::ooo) {
      PU_PUSHNAME (ctxt_p, "level");

      stat = asn1PE_EnumNatureLevel (ctxt_p, pvalue->level);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode geometrie */

   PU_PUSHNAME (ctxt_p, "geometrie");

   stat = asn1PE_Line (ctxt_p, &pvalue->geometrie);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_MsgLimitCreation: end\n");
   return (stat);
}

int ASN1C_MsgLimitCreation::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgLimitCreation (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgLimitCreation (ASN1CTXT* ctxt_p, ASN1T_MsgLimitCreation* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL optbit;
   ASN1BOOL levelPresent;

   rtdiag ("asn1PD_MsgLimitCreation: start\n");

   /* optional bits */

   PU_NEWFIELD (ctxt_p, "levelPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else levelPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   /* decode oid */

   PU_PUSHNAME (ctxt_p, "oid");

   stat = asn1PD_OID (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode level */

   if (levelPresent) {
      PU_PUSHNAME (ctxt_p, "level");

      stat = asn1PD_EnumNatureLevel (ctxt_p, &pvalue->level);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }
   else {
      pvalue->level = EnumNatureLevel::ooo;
   }

   /* decode geometrie */

   PU_PUSHNAME (ctxt_p, "geometrie");

   stat = asn1PD_Line (ctxt_p, &pvalue->geometrie);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_MsgLimitCreation: end\n");

   return (stat);
}

int ASN1C_MsgLimitCreation::Decode ()
{
   return asn1PD_MsgLimitCreation (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgLimitDestruction                                       */
/*                                                            */
/**************************************************************/

ASN1C_MsgLimitDestruction::ASN1C_MsgLimitDestruction (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgLimitDestruction& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgLimitDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgLimitDestruction value)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgLimitDestruction: start\n");

   stat = asn1PE_OID (ctxt_p, value);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   rtdiag ("asn1PE_MsgLimitDestruction: end\n");
   return (stat);
}

int ASN1C_MsgLimitDestruction::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgLimitDestruction (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_MsgLimitDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgLimitDestruction* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgLimitDestruction: start\n");

   stat = asn1PD_OID (ctxt_p, pvalue);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   rtdiag ("asn1PD_MsgLimitDestruction: end\n");

   return (stat);
}

int ASN1C_MsgLimitDestruction::Decode ()
{
   return asn1PD_MsgLimitDestruction (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgLimitUpdate                                            */
/*                                                            */
/**************************************************************/

ASN1C_MsgLimitUpdate::ASN1C_MsgLimitUpdate (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgLimitUpdate& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgLimitUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgLimitUpdate* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgLimitUpdate: start\n");

   stat = asn1PE_MsgLimitCreation (ctxt_p, pvalue);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   rtdiag ("asn1PE_MsgLimitUpdate: end\n");
   return (stat);
}

int ASN1C_MsgLimitUpdate::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgLimitUpdate (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgLimitUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgLimitUpdate* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgLimitUpdate: start\n");

   stat = asn1PD_MsgLimitCreation (ctxt_p, pvalue);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   rtdiag ("asn1PD_MsgLimitUpdate: end\n");

   return (stat);
}

int ASN1C_MsgLimitUpdate::Decode ()
{
   return asn1PD_MsgLimitUpdate (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  EnumInfoContextErrorCode                                  */
/*                                                            */
/**************************************************************/

ASN1C_EnumInfoContextErrorCode::ASN1C_EnumInfoContextErrorCode (
   ASN1MessageBuffer& msgBuf, ASN1T_EnumInfoContextErrorCode& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_EnumInfoContextErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumInfoContextErrorCode value)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   rtdiag ("asn1PE_EnumInfoContextErrorCode: start\n");

   switch (value) {
      case EnumInfoContextErrorCode::no_error: ui = 0; break;
      case EnumInfoContextErrorCode::error_invalid_id: ui = 1; break;
      case EnumInfoContextErrorCode::error_invalid_geometry: ui = 2; break;
      case EnumInfoContextErrorCode::error_invalid_lima_function: ui = 3; break;
      case EnumInfoContextErrorCode::error_invalid_limit_or_lima_used: ui = 4; break;
      default: return LOG_ASN1ERR (ctxt_p, ASN_E_INVENUM);
   }

   stat = pe_ConsUnsigned (ctxt_p, ui, 0, 4);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   rtdiag ("asn1PE_EnumInfoContextErrorCode: end\n");
   return (stat);
}

int ASN1C_EnumInfoContextErrorCode::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_EnumInfoContextErrorCode (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_EnumInfoContextErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumInfoContextErrorCode* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   rtdiag ("asn1PD_EnumInfoContextErrorCode: start\n");

   stat = pd_ConsUnsigned (ctxt_p, &ui, 0, 4);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   switch (ui) {
      case 0: *pvalue = EnumInfoContextErrorCode::no_error; break;
      case 1: *pvalue = EnumInfoContextErrorCode::error_invalid_id; break;
      case 2: *pvalue = EnumInfoContextErrorCode::error_invalid_geometry; break;
      case 3: *pvalue = EnumInfoContextErrorCode::error_invalid_lima_function; break;
      case 4: *pvalue = EnumInfoContextErrorCode::error_invalid_limit_or_lima_used; break;
      default: return LOG_ASN1ERR (ctxt_p, ASN_E_INVENUM);
   }

   rtdiag ("asn1PD_EnumInfoContextErrorCode: end\n");

   return (stat);
}

int ASN1C_EnumInfoContextErrorCode::Decode ()
{
   return asn1PD_EnumInfoContextErrorCode (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgLimitCreationAck                                       */
/*                                                            */
/**************************************************************/

ASN1C_MsgLimitCreationAck::ASN1C_MsgLimitCreationAck (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgLimitCreationAck& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgLimitCreationAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimitCreationAck* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgLimitCreationAck: start\n");

   /* encode oid */

   PU_PUSHNAME (ctxt_p, "oid");

   stat = asn1PE_OID (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode error_code */

   PU_PUSHNAME (ctxt_p, "error_code");

   stat = asn1PE_EnumInfoContextErrorCode (ctxt_p, pvalue->error_code);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_MsgLimitCreationAck: end\n");
   return (stat);
}

int ASN1C_MsgLimitCreationAck::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgLimitCreationAck (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgLimitCreationAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimitCreationAck* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgLimitCreationAck: start\n");

   /* decode oid */

   PU_PUSHNAME (ctxt_p, "oid");

   stat = asn1PD_OID (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode error_code */

   PU_PUSHNAME (ctxt_p, "error_code");

   stat = asn1PD_EnumInfoContextErrorCode (ctxt_p, &pvalue->error_code);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_MsgLimitCreationAck: end\n");

   return (stat);
}

int ASN1C_MsgLimitCreationAck::Decode ()
{
   return asn1PD_MsgLimitCreationAck (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgLimitDestructionAck                                    */
/*                                                            */
/**************************************************************/

ASN1C_MsgLimitDestructionAck::ASN1C_MsgLimitDestructionAck (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgLimitDestructionAck& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgLimitDestructionAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimitDestructionAck* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgLimitDestructionAck: start\n");

   /* encode oid */

   PU_PUSHNAME (ctxt_p, "oid");

   stat = asn1PE_OID (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode error_code */

   PU_PUSHNAME (ctxt_p, "error_code");

   stat = asn1PE_EnumInfoContextErrorCode (ctxt_p, pvalue->error_code);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_MsgLimitDestructionAck: end\n");
   return (stat);
}

int ASN1C_MsgLimitDestructionAck::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgLimitDestructionAck (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgLimitDestructionAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimitDestructionAck* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgLimitDestructionAck: start\n");

   /* decode oid */

   PU_PUSHNAME (ctxt_p, "oid");

   stat = asn1PD_OID (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode error_code */

   PU_PUSHNAME (ctxt_p, "error_code");

   stat = asn1PD_EnumInfoContextErrorCode (ctxt_p, &pvalue->error_code);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_MsgLimitDestructionAck: end\n");

   return (stat);
}

int ASN1C_MsgLimitDestructionAck::Decode ()
{
   return asn1PD_MsgLimitDestructionAck (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgLimitUpdateAck                                         */
/*                                                            */
/**************************************************************/

ASN1C_MsgLimitUpdateAck::ASN1C_MsgLimitUpdateAck (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgLimitUpdateAck& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgLimitUpdateAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimitUpdateAck* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgLimitUpdateAck: start\n");

   /* encode oid */

   PU_PUSHNAME (ctxt_p, "oid");

   stat = asn1PE_OID (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode error_code */

   PU_PUSHNAME (ctxt_p, "error_code");

   stat = asn1PE_EnumInfoContextErrorCode (ctxt_p, pvalue->error_code);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_MsgLimitUpdateAck: end\n");
   return (stat);
}

int ASN1C_MsgLimitUpdateAck::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgLimitUpdateAck (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgLimitUpdateAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimitUpdateAck* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgLimitUpdateAck: start\n");

   /* decode oid */

   PU_PUSHNAME (ctxt_p, "oid");

   stat = asn1PD_OID (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode error_code */

   PU_PUSHNAME (ctxt_p, "error_code");

   stat = asn1PD_EnumInfoContextErrorCode (ctxt_p, &pvalue->error_code);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_MsgLimitUpdateAck: end\n");

   return (stat);
}

int ASN1C_MsgLimitUpdateAck::Decode ()
{
   return asn1PD_MsgLimitUpdateAck (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgLimaCreation                                           */
/*                                                            */
/**************************************************************/

ASN1C_MsgLimaCreation::ASN1C_MsgLimaCreation (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgLimaCreation& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgLimaCreation (ASN1CTXT* ctxt_p, ASN1T_MsgLimaCreation* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgLimaCreation: start\n");

   /* encode oid */

   PU_PUSHNAME (ctxt_p, "oid");

   stat = asn1PE_OID (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode fonction */

   PU_PUSHNAME (ctxt_p, "fonction");

   stat = asn1PE_EnumTypeLima (ctxt_p, pvalue->fonction);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode geometrie */

   PU_PUSHNAME (ctxt_p, "geometrie");

   stat = asn1PE_Line (ctxt_p, &pvalue->geometrie);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_MsgLimaCreation: end\n");
   return (stat);
}

int ASN1C_MsgLimaCreation::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgLimaCreation (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgLimaCreation (ASN1CTXT* ctxt_p, ASN1T_MsgLimaCreation* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgLimaCreation: start\n");

   /* decode oid */

   PU_PUSHNAME (ctxt_p, "oid");

   stat = asn1PD_OID (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode fonction */

   PU_PUSHNAME (ctxt_p, "fonction");

   stat = asn1PD_EnumTypeLima (ctxt_p, &pvalue->fonction);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode geometrie */

   PU_PUSHNAME (ctxt_p, "geometrie");

   stat = asn1PD_Line (ctxt_p, &pvalue->geometrie);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_MsgLimaCreation: end\n");

   return (stat);
}

int ASN1C_MsgLimaCreation::Decode ()
{
   return asn1PD_MsgLimaCreation (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgLimaDestruction                                        */
/*                                                            */
/**************************************************************/

ASN1C_MsgLimaDestruction::ASN1C_MsgLimaDestruction (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgLimaDestruction& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgLimaDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgLimaDestruction value)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgLimaDestruction: start\n");

   stat = asn1PE_OID (ctxt_p, value);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   rtdiag ("asn1PE_MsgLimaDestruction: end\n");
   return (stat);
}

int ASN1C_MsgLimaDestruction::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgLimaDestruction (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_MsgLimaDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgLimaDestruction* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgLimaDestruction: start\n");

   stat = asn1PD_OID (ctxt_p, pvalue);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   rtdiag ("asn1PD_MsgLimaDestruction: end\n");

   return (stat);
}

int ASN1C_MsgLimaDestruction::Decode ()
{
   return asn1PD_MsgLimaDestruction (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgLimaUpdate                                             */
/*                                                            */
/**************************************************************/

ASN1C_MsgLimaUpdate::ASN1C_MsgLimaUpdate (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgLimaUpdate& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgLimaUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgLimaUpdate* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgLimaUpdate: start\n");

   stat = asn1PE_MsgLimaCreation (ctxt_p, pvalue);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   rtdiag ("asn1PE_MsgLimaUpdate: end\n");
   return (stat);
}

int ASN1C_MsgLimaUpdate::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgLimaUpdate (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgLimaUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgLimaUpdate* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgLimaUpdate: start\n");

   stat = asn1PD_MsgLimaCreation (ctxt_p, pvalue);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   rtdiag ("asn1PD_MsgLimaUpdate: end\n");

   return (stat);
}

int ASN1C_MsgLimaUpdate::Decode ()
{
   return asn1PD_MsgLimaUpdate (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgLimaCreationAck                                        */
/*                                                            */
/**************************************************************/

ASN1C_MsgLimaCreationAck::ASN1C_MsgLimaCreationAck (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgLimaCreationAck& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgLimaCreationAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimaCreationAck* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgLimaCreationAck: start\n");

   /* encode oid */

   PU_PUSHNAME (ctxt_p, "oid");

   stat = asn1PE_OID (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode error_code */

   PU_PUSHNAME (ctxt_p, "error_code");

   stat = asn1PE_EnumInfoContextErrorCode (ctxt_p, pvalue->error_code);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_MsgLimaCreationAck: end\n");
   return (stat);
}

int ASN1C_MsgLimaCreationAck::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgLimaCreationAck (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgLimaCreationAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimaCreationAck* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgLimaCreationAck: start\n");

   /* decode oid */

   PU_PUSHNAME (ctxt_p, "oid");

   stat = asn1PD_OID (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode error_code */

   PU_PUSHNAME (ctxt_p, "error_code");

   stat = asn1PD_EnumInfoContextErrorCode (ctxt_p, &pvalue->error_code);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_MsgLimaCreationAck: end\n");

   return (stat);
}

int ASN1C_MsgLimaCreationAck::Decode ()
{
   return asn1PD_MsgLimaCreationAck (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgLimaDestructionAck                                     */
/*                                                            */
/**************************************************************/

ASN1C_MsgLimaDestructionAck::ASN1C_MsgLimaDestructionAck (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgLimaDestructionAck& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgLimaDestructionAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimaDestructionAck* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgLimaDestructionAck: start\n");

   /* encode oid */

   PU_PUSHNAME (ctxt_p, "oid");

   stat = asn1PE_OID (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode error_code */

   PU_PUSHNAME (ctxt_p, "error_code");

   stat = asn1PE_EnumInfoContextErrorCode (ctxt_p, pvalue->error_code);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_MsgLimaDestructionAck: end\n");
   return (stat);
}

int ASN1C_MsgLimaDestructionAck::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgLimaDestructionAck (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgLimaDestructionAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimaDestructionAck* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgLimaDestructionAck: start\n");

   /* decode oid */

   PU_PUSHNAME (ctxt_p, "oid");

   stat = asn1PD_OID (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode error_code */

   PU_PUSHNAME (ctxt_p, "error_code");

   stat = asn1PD_EnumInfoContextErrorCode (ctxt_p, &pvalue->error_code);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_MsgLimaDestructionAck: end\n");

   return (stat);
}

int ASN1C_MsgLimaDestructionAck::Decode ()
{
   return asn1PD_MsgLimaDestructionAck (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgLimaUpdateAck                                          */
/*                                                            */
/**************************************************************/

ASN1C_MsgLimaUpdateAck::ASN1C_MsgLimaUpdateAck (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgLimaUpdateAck& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgLimaUpdateAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimaUpdateAck* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgLimaUpdateAck: start\n");

   /* encode oid */

   PU_PUSHNAME (ctxt_p, "oid");

   stat = asn1PE_OID (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode error_code */

   PU_PUSHNAME (ctxt_p, "error_code");

   stat = asn1PE_EnumInfoContextErrorCode (ctxt_p, pvalue->error_code);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_MsgLimaUpdateAck: end\n");
   return (stat);
}

int ASN1C_MsgLimaUpdateAck::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgLimaUpdateAck (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgLimaUpdateAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimaUpdateAck* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgLimaUpdateAck: start\n");

   /* decode oid */

   PU_PUSHNAME (ctxt_p, "oid");

   stat = asn1PD_OID (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode error_code */

   PU_PUSHNAME (ctxt_p, "error_code");

   stat = asn1PD_EnumInfoContextErrorCode (ctxt_p, &pvalue->error_code);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_MsgLimaUpdateAck: end\n");

   return (stat);
}

int ASN1C_MsgLimaUpdateAck::Decode ()
{
   return asn1PD_MsgLimaUpdateAck (mpContext->GetPtr(), &msgData);
}


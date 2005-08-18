/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 17-Aug-2005.
 */
#include "ScipioOrder.h"

/**************************************************************/
/*                                                            */
/*  EnumOrderErrorCode                                        */
/*                                                            */
/**************************************************************/

ASN1C_EnumOrderErrorCode::ASN1C_EnumOrderErrorCode (
   ASN1MessageBuffer& msgBuf, ASN1T_EnumOrderErrorCode& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_EnumOrderErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumOrderErrorCode value)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   rtdiag ("asn1PE_EnumOrderErrorCode: start\n");

   switch (value) {
      case EnumOrderErrorCode::no_error: ui = 0; break;
      case EnumOrderErrorCode::error_invalid_unit: ui = 1; break;
      case EnumOrderErrorCode::error_invalid_limit: ui = 2; break;
      case EnumOrderErrorCode::error_invalid_lima: ui = 3; break;
      case EnumOrderErrorCode::error_invalid_mission: ui = 4; break;
      case EnumOrderErrorCode::error_invalid_mission_parameters: ui = 5; break;
      case EnumOrderErrorCode::error_unit_cannot_receive_order: ui = 6; break;
      case EnumOrderErrorCode::error_invalid_order_conduite: ui = 7; break;
      case EnumOrderErrorCode::error_invalid_order_mission: ui = 8; break;
      case EnumOrderErrorCode::error_invalid_order_initial: ui = 9; break;
      case EnumOrderErrorCode::error_invalid_order_conduite_parameters: ui = 10; break;
      case EnumOrderErrorCode::error_unit_surrendered: ui = 11; break;
      default: return LOG_ASN1ERR (ctxt_p, ASN_E_INVENUM);
   }

   stat = pe_ConsUnsigned (ctxt_p, ui, 0, 11);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   rtdiag ("asn1PE_EnumOrderErrorCode: end\n");
   return (stat);
}

int ASN1C_EnumOrderErrorCode::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_EnumOrderErrorCode (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_EnumOrderErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumOrderErrorCode* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   rtdiag ("asn1PD_EnumOrderErrorCode: start\n");

   stat = pd_ConsUnsigned (ctxt_p, &ui, 0, 11);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   switch (ui) {
      case 0: *pvalue = EnumOrderErrorCode::no_error; break;
      case 1: *pvalue = EnumOrderErrorCode::error_invalid_unit; break;
      case 2: *pvalue = EnumOrderErrorCode::error_invalid_limit; break;
      case 3: *pvalue = EnumOrderErrorCode::error_invalid_lima; break;
      case 4: *pvalue = EnumOrderErrorCode::error_invalid_mission; break;
      case 5: *pvalue = EnumOrderErrorCode::error_invalid_mission_parameters; break;
      case 6: *pvalue = EnumOrderErrorCode::error_unit_cannot_receive_order; break;
      case 7: *pvalue = EnumOrderErrorCode::error_invalid_order_conduite; break;
      case 8: *pvalue = EnumOrderErrorCode::error_invalid_order_mission; break;
      case 9: *pvalue = EnumOrderErrorCode::error_invalid_order_initial; break;
      case 10: *pvalue = EnumOrderErrorCode::error_invalid_order_conduite_parameters; break;
      case 11: *pvalue = EnumOrderErrorCode::error_unit_surrendered; break;
      default: return LOG_ASN1ERR (ctxt_p, ASN_E_INVENUM);
   }

   rtdiag ("asn1PD_EnumOrderErrorCode: end\n");

   return (stat);
}

int ASN1C_EnumOrderErrorCode::Decode ()
{
   return asn1PD_EnumOrderErrorCode (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  EnumOrderState                                            */
/*                                                            */
/**************************************************************/

ASN1C_EnumOrderState::ASN1C_EnumOrderState (
   ASN1MessageBuffer& msgBuf, ASN1T_EnumOrderState& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_EnumOrderState (ASN1CTXT* ctxt_p, ASN1T_EnumOrderState value)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   rtdiag ("asn1PE_EnumOrderState: start\n");

   switch (value) {
      case EnumOrderState::started: ui = 0; break;
      case EnumOrderState::cancelled: ui = 1; break;
      case EnumOrderState::stopped: ui = 2; break;
      default: return LOG_ASN1ERR (ctxt_p, ASN_E_INVENUM);
   }

   stat = pe_ConsUnsigned (ctxt_p, ui, 0, 2);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   rtdiag ("asn1PE_EnumOrderState: end\n");
   return (stat);
}

int ASN1C_EnumOrderState::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_EnumOrderState (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_EnumOrderState (ASN1CTXT* ctxt_p, ASN1T_EnumOrderState* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   rtdiag ("asn1PD_EnumOrderState: start\n");

   stat = pd_ConsUnsigned (ctxt_p, &ui, 0, 2);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   switch (ui) {
      case 0: *pvalue = EnumOrderState::started; break;
      case 1: *pvalue = EnumOrderState::cancelled; break;
      case 2: *pvalue = EnumOrderState::stopped; break;
      default: return LOG_ASN1ERR (ctxt_p, ASN_E_INVENUM);
   }

   rtdiag ("asn1PD_EnumOrderState: end\n");

   return (stat);
}

int ASN1C_EnumOrderState::Decode ()
{
   return asn1PD_EnumOrderState (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgOrderManagement                                        */
/*                                                            */
/**************************************************************/

ASN1C_MsgOrderManagement::ASN1C_MsgOrderManagement (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgOrderManagement& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgOrderManagement (ASN1CTXT* ctxt_p, ASN1T_MsgOrderManagement* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgOrderManagement: start\n");

   /* encode order_id */

   PU_PUSHNAME (ctxt_p, "order_id");

   stat = asn1PE_OID (ctxt_p, pvalue->order_id);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode etat */

   PU_PUSHNAME (ctxt_p, "etat");

   stat = asn1PE_EnumOrderState (ctxt_p, pvalue->etat);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_MsgOrderManagement: end\n");
   return (stat);
}

int ASN1C_MsgOrderManagement::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgOrderManagement (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgOrderManagement (ASN1CTXT* ctxt_p, ASN1T_MsgOrderManagement* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgOrderManagement: start\n");

   /* decode order_id */

   PU_PUSHNAME (ctxt_p, "order_id");

   stat = asn1PD_OID (ctxt_p, &pvalue->order_id);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode etat */

   PU_PUSHNAME (ctxt_p, "etat");

   stat = asn1PD_EnumOrderState (ctxt_p, &pvalue->etat);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_MsgOrderManagement: end\n");

   return (stat);
}

int ASN1C_MsgOrderManagement::Decode ()
{
   return asn1PD_MsgOrderManagement (mpContext->GetPtr(), &msgData);
}


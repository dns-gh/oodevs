/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 24-Dec-2009.
 */
#include "Chat.h"

/**************************************************************/
/*                                                            */
/*  ChatIncludes                                              */
/*                                                            */
/**************************************************************/

ASN1C_ChatIncludes::ASN1C_ChatIncludes (
   ASN1MessageBuffer& msgBuf, ASN1T_ChatIncludes& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_ChatIncludes (ASN1CTXT* ctxt_p, ASN1T_ChatIncludes* pvalue)
{
   int stat = ASN_OK;

   /* encode baseExports */

   stat = asn1PE_BaseExports (ctxt_p);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_ChatIncludes::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_ChatIncludes (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_ChatIncludes (ASN1CTXT* ctxt_p, ASN1T_ChatIncludes* pvalue)
{
   int stat = ASN_OK;

   /* decode baseExports */

   stat = asn1PD_BaseExports (ctxt_p);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_ChatIncludes::Decode ()
{
   return asn1PD_ChatIncludes (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  ChatExports                                               */
/*                                                            */
/**************************************************************/

ASN1C_ChatExports::ASN1C_ChatExports (
   ASN1MessageBuffer& msgBuf) :
   ASN1CType(msgBuf)
{}

EXTERN int asn1PE_ChatExports (ASN1CTXT* ctxt_p)
{
   int stat = ASN_OK;

   /* NULL */
   return (stat);
}

int ASN1C_ChatExports::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_ChatExports (mpContext->GetPtr());
   return stat;
}

EXTERN int asn1PD_ChatExports (ASN1CTXT* ctxt_p)
{
   int stat = ASN_OK;

   /* NULL */

   return (stat);
}

int ASN1C_ChatExports::Decode ()
{
   return asn1PD_ChatExports (mpContext->GetPtr());
}

/**************************************************************/
/*                                                            */
/*  ChatTarget                                                */
/*                                                            */
/**************************************************************/

ASN1C_ChatTarget::ASN1C_ChatTarget (
   ASN1MessageBuffer& msgBuf, ASN1T_ChatTarget& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_ChatTarget (ASN1CTXT* ctxt_p, ASN1T_ChatTarget* pvalue)
{
   int stat = ASN_OK;

   /* encode profile */

   stat = pe_VisibleString (ctxt_p, pvalue->profile, 0);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_ChatTarget::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_ChatTarget (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_ChatTarget (ASN1CTXT* ctxt_p, ASN1T_ChatTarget* pvalue)
{
   int stat = ASN_OK;

   /* decode profile */

   stat = pd_VisibleString (ctxt_p, &pvalue->profile, 0);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_ChatTarget::Decode ()
{
   return asn1PD_ChatTarget (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgTextMessage                                            */
/*                                                            */
/**************************************************************/

ASN1C_MsgTextMessage::ASN1C_MsgTextMessage (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgTextMessage& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgTextMessage (ASN1CTXT* ctxt_p, ASN1T_MsgTextMessage* pvalue)
{
   int stat = ASN_OK;

   /* encode source */

   stat = asn1PE_ChatTarget (ctxt_p, &pvalue->source);
   if (stat != ASN_OK) return stat;

   /* encode target */

   stat = asn1PE_ChatTarget (ctxt_p, &pvalue->target);
   if (stat != ASN_OK) return stat;

   /* encode message */

   stat = pe_VisibleString (ctxt_p, pvalue->message, 0);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgTextMessage::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgTextMessage (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgTextMessage (ASN1CTXT* ctxt_p, ASN1T_MsgTextMessage* pvalue)
{
   int stat = ASN_OK;

   /* decode source */

   stat = asn1PD_ChatTarget (ctxt_p, &pvalue->source);
   if (stat != ASN_OK) return stat;

   /* decode target */

   stat = asn1PD_ChatTarget (ctxt_p, &pvalue->target);
   if (stat != ASN_OK) return stat;

   /* decode message */

   stat = pd_VisibleString (ctxt_p, &pvalue->message, 0);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgTextMessage::Decode ()
{
   return asn1PD_MsgTextMessage (mpContext->GetPtr(), &msgData);
}


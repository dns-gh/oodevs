/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 20-Sep-2007.
 */
#include "Profiles.h"

/**************************************************************/
/*                                                            */
/*  ProfilesIncludes                                          */
/*                                                            */
/**************************************************************/

ASN1C_ProfilesIncludes::ASN1C_ProfilesIncludes (
   ASN1MessageBuffer& msgBuf, ASN1T_ProfilesIncludes& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_ProfilesIncludes (ASN1CTXT* ctxt_p, ASN1T_ProfilesIncludes* pvalue)
{
   int stat = ASN_OK;

   return (stat);
}

int ASN1C_ProfilesIncludes::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_ProfilesIncludes (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_ProfilesIncludes (ASN1CTXT* ctxt_p, ASN1T_ProfilesIncludes* pvalue)
{
   int stat = ASN_OK;

   /* decode  */



   return (stat);
}

int ASN1C_ProfilesIncludes::Decode ()
{
   return asn1PD_ProfilesIncludes (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  ProfilesExports                                           */
/*                                                            */
/**************************************************************/

ASN1C_ProfilesExports::ASN1C_ProfilesExports (
   ASN1MessageBuffer& msgBuf) :
   ASN1CType(msgBuf)
{}

EXTERN int asn1PE_ProfilesExports (ASN1CTXT* ctxt_p)
{
   int stat = ASN_OK;

   /* NULL */
   return (stat);
}

int ASN1C_ProfilesExports::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_ProfilesExports (mpContext->GetPtr());
   return stat;
}

EXTERN int asn1PD_ProfilesExports (ASN1CTXT* ctxt_p)
{
   int stat = ASN_OK;

   /* NULL */

   return (stat);
}

int ASN1C_ProfilesExports::Decode ()
{
   return asn1PD_ProfilesExports (mpContext->GetPtr());
}

/**************************************************************/
/*                                                            */
/*  Profile                                                   */
/*                                                            */
/**************************************************************/

ASN1C_Profile::ASN1C_Profile (
   ASN1MessageBuffer& msgBuf, ASN1T_Profile& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Profile (ASN1CTXT* ctxt_p, ASN1T_Profile* pvalue)
{
   int stat = ASN_OK;

   /* encode login */

   stat = pe_VisibleString (ctxt_p, pvalue->login, 0);
   if (stat != ASN_OK) return stat;

   /* encode role_adaptation */

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->role_adaptation);
   if (stat != ASN_OK) return stat;

   /* encode role_preparation */

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->role_preparation);
   if (stat != ASN_OK) return stat;

   /* encode role_game */

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->role_game);
   if (stat != ASN_OK) return stat;

   /* encode role_supervision */

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->role_supervision);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_Profile::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Profile (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Profile (ASN1CTXT* ctxt_p, ASN1T_Profile* pvalue)
{
   int stat = ASN_OK;

   /* decode login */

   stat = pd_VisibleString (ctxt_p, &pvalue->login, 0);
   if (stat != ASN_OK) return stat;

   /* decode role_adaptation */

   stat = pd_bit (ctxt_p, &pvalue->role_adaptation);
   if (stat != ASN_OK) return stat;

   /* decode role_preparation */

   stat = pd_bit (ctxt_p, &pvalue->role_preparation);
   if (stat != ASN_OK) return stat;

   /* decode role_game */

   stat = pd_bit (ctxt_p, &pvalue->role_game);
   if (stat != ASN_OK) return stat;

   /* decode role_supervision */

   stat = pd_bit (ctxt_p, &pvalue->role_supervision);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_Profile::Decode ()
{
   return asn1PD_Profile (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgAuthenticationRequest                                  */
/*                                                            */
/**************************************************************/

ASN1C_MsgAuthenticationRequest::ASN1C_MsgAuthenticationRequest (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgAuthenticationRequest& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgAuthenticationRequest (ASN1CTXT* ctxt_p, ASN1T_MsgAuthenticationRequest* pvalue)
{
   int stat = ASN_OK;

   /* encode login */

   stat = pe_VisibleString (ctxt_p, pvalue->login, 0);
   if (stat != ASN_OK) return stat;

   /* encode password */

   stat = pe_VisibleString (ctxt_p, pvalue->password, 0);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgAuthenticationRequest::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgAuthenticationRequest (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgAuthenticationRequest (ASN1CTXT* ctxt_p, ASN1T_MsgAuthenticationRequest* pvalue)
{
   int stat = ASN_OK;

   /* decode login */

   stat = pd_VisibleString (ctxt_p, &pvalue->login, 0);
   if (stat != ASN_OK) return stat;

   /* decode password */

   stat = pd_VisibleString (ctxt_p, &pvalue->password, 0);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgAuthenticationRequest::Decode ()
{
   return asn1PD_MsgAuthenticationRequest (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgAuthenticationResponse_return_code                     */
/*                                                            */
/**************************************************************/

ASN1C_MsgAuthenticationResponse_return_code::ASN1C_MsgAuthenticationResponse_return_code (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgAuthenticationResponse_return_code& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgAuthenticationResponse_return_code (ASN1CTXT* ctxt_p, ASN1T_MsgAuthenticationResponse_return_code value)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   switch (value) {
      case MsgAuthenticationResponse_return_code::ok: ui = 0; break;
      case MsgAuthenticationResponse_return_code::invalid_login: ui = 1; break;
      default: return ASN_E_INVENUM;
   }

   stat = pe_ConsUnsigned (ctxt_p, ui, 0, 1);
   if (stat != ASN_OK) return stat;
   return (stat);
}

int ASN1C_MsgAuthenticationResponse_return_code::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgAuthenticationResponse_return_code (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_MsgAuthenticationResponse_return_code (ASN1CTXT* ctxt_p, ASN1T_MsgAuthenticationResponse_return_code* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   stat = pd_ConsUnsigned (ctxt_p, &ui, 0, 1);
   if (stat != ASN_OK) return stat;

   switch (ui) {
      case 0: *pvalue = MsgAuthenticationResponse_return_code::ok; break;
      case 1: *pvalue = MsgAuthenticationResponse_return_code::invalid_login; break;
      default: return ASN_E_INVENUM;
   }

   return (stat);
}

int ASN1C_MsgAuthenticationResponse_return_code::Decode ()
{
   return asn1PD_MsgAuthenticationResponse_return_code (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgAuthenticationResponse                                 */
/*                                                            */
/**************************************************************/

ASN1C_MsgAuthenticationResponse::ASN1C_MsgAuthenticationResponse (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgAuthenticationResponse& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgAuthenticationResponse (ASN1CTXT* ctxt_p, ASN1T_MsgAuthenticationResponse* pvalue)
{
   int stat = ASN_OK;

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.profilePresent);

   /* encode return_code */

   stat = asn1PE_MsgAuthenticationResponse_return_code (ctxt_p, pvalue->return_code);
   if (stat != ASN_OK) return stat;

   /* encode profile */

   if (pvalue->m.profilePresent) {
      stat = asn1PE_Profile (ctxt_p, &pvalue->profile);
      if (stat != ASN_OK) return stat;
   }

   return (stat);
}

int ASN1C_MsgAuthenticationResponse::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgAuthenticationResponse (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgAuthenticationResponse (ASN1CTXT* ctxt_p, ASN1T_MsgAuthenticationResponse* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL optbit;

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   pd_bit (ctxt_p, &optbit);
   pvalue->m.profilePresent = optbit;

   /* decode return_code */

   stat = asn1PD_MsgAuthenticationResponse_return_code (ctxt_p, &pvalue->return_code);
   if (stat != ASN_OK) return stat;

   /* decode profile */

   if (pvalue->m.profilePresent) {
      stat = asn1PD_Profile (ctxt_p, &pvalue->profile);
      if (stat != ASN_OK) return stat;
   }


   return (stat);
}

int ASN1C_MsgAuthenticationResponse::Decode ()
{
   return asn1PD_MsgAuthenticationResponse (mpContext->GetPtr(), &msgData);
}


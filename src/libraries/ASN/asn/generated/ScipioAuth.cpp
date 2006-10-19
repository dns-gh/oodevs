/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 19-Oct-2006.
 */
#include "ScipioAuth.h"

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

   rtdiag ("asn1PE_Profile: start\n");

   PU_NEWFIELD (ctxt_p, "passwordPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.passwordPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "read_only_formationsPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.read_only_formationsPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "read_write_formationsPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.read_write_formationsPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "read_only_automatesPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.read_only_automatesPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "read_write_automatesPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.read_write_automatesPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "read_only_campsPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.read_only_campsPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "read_write_campsPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.read_write_campsPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "read_only_populationsPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.read_only_populationsPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "read_write_populationsPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.read_write_populationsPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   /* encode login */

   PU_PUSHNAME (ctxt_p, "login");

   stat = pe_VisibleString (ctxt_p, pvalue->login, 0);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode password */

   if (pvalue->m.passwordPresent) {
      PU_PUSHNAME (ctxt_p, "password");

      stat = pe_VisibleString (ctxt_p, pvalue->password, 0);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode read_only_formations */

   if (pvalue->m.read_only_formationsPresent) {
      PU_PUSHNAME (ctxt_p, "read_only_formations");

      stat = asn1PE_ListFormation (ctxt_p, &pvalue->read_only_formations);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode read_write_formations */

   if (pvalue->m.read_write_formationsPresent) {
      PU_PUSHNAME (ctxt_p, "read_write_formations");

      stat = asn1PE_ListFormation (ctxt_p, &pvalue->read_write_formations);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode read_only_automates */

   if (pvalue->m.read_only_automatesPresent) {
      PU_PUSHNAME (ctxt_p, "read_only_automates");

      stat = asn1PE_ListAutomate (ctxt_p, &pvalue->read_only_automates);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode read_write_automates */

   if (pvalue->m.read_write_automatesPresent) {
      PU_PUSHNAME (ctxt_p, "read_write_automates");

      stat = asn1PE_ListAutomate (ctxt_p, &pvalue->read_write_automates);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode read_only_camps */

   if (pvalue->m.read_only_campsPresent) {
      PU_PUSHNAME (ctxt_p, "read_only_camps");

      stat = asn1PE_ListCamp (ctxt_p, &pvalue->read_only_camps);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode read_write_camps */

   if (pvalue->m.read_write_campsPresent) {
      PU_PUSHNAME (ctxt_p, "read_write_camps");

      stat = asn1PE_ListCamp (ctxt_p, &pvalue->read_write_camps);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode read_only_populations */

   if (pvalue->m.read_only_populationsPresent) {
      PU_PUSHNAME (ctxt_p, "read_only_populations");

      stat = asn1PE_ListPopulation (ctxt_p, &pvalue->read_only_populations);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode read_write_populations */

   if (pvalue->m.read_write_populationsPresent) {
      PU_PUSHNAME (ctxt_p, "read_write_populations");

      stat = asn1PE_ListPopulation (ctxt_p, &pvalue->read_write_populations);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode superviseur */

   PU_PUSHNAME (ctxt_p, "superviseur");

   PU_NEWFIELD (ctxt_p, "boolean");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->superviseur);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Profile: end\n");
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
   ASN1BOOL optbit;

   rtdiag ("asn1PD_Profile: start\n");

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   PU_NEWFIELD (ctxt_p, "passwordPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.passwordPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "read_only_formationsPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.read_only_formationsPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "read_write_formationsPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.read_write_formationsPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "read_only_automatesPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.read_only_automatesPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "read_write_automatesPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.read_write_automatesPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "read_only_campsPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.read_only_campsPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "read_write_campsPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.read_write_campsPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "read_only_populationsPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.read_only_populationsPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   PU_NEWFIELD (ctxt_p, "read_write_populationsPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.read_write_populationsPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   /* decode login */

   PU_PUSHNAME (ctxt_p, "login");

   stat = pd_VisibleString (ctxt_p, &pvalue->login, 0);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode password */

   if (pvalue->m.passwordPresent) {
      PU_PUSHNAME (ctxt_p, "password");

      stat = pd_VisibleString (ctxt_p, &pvalue->password, 0);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode read_only_formations */

   if (pvalue->m.read_only_formationsPresent) {
      PU_PUSHNAME (ctxt_p, "read_only_formations");

      stat = asn1PD_ListFormation (ctxt_p, &pvalue->read_only_formations);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode read_write_formations */

   if (pvalue->m.read_write_formationsPresent) {
      PU_PUSHNAME (ctxt_p, "read_write_formations");

      stat = asn1PD_ListFormation (ctxt_p, &pvalue->read_write_formations);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode read_only_automates */

   if (pvalue->m.read_only_automatesPresent) {
      PU_PUSHNAME (ctxt_p, "read_only_automates");

      stat = asn1PD_ListAutomate (ctxt_p, &pvalue->read_only_automates);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode read_write_automates */

   if (pvalue->m.read_write_automatesPresent) {
      PU_PUSHNAME (ctxt_p, "read_write_automates");

      stat = asn1PD_ListAutomate (ctxt_p, &pvalue->read_write_automates);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode read_only_camps */

   if (pvalue->m.read_only_campsPresent) {
      PU_PUSHNAME (ctxt_p, "read_only_camps");

      stat = asn1PD_ListCamp (ctxt_p, &pvalue->read_only_camps);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode read_write_camps */

   if (pvalue->m.read_write_campsPresent) {
      PU_PUSHNAME (ctxt_p, "read_write_camps");

      stat = asn1PD_ListCamp (ctxt_p, &pvalue->read_write_camps);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode read_only_populations */

   if (pvalue->m.read_only_populationsPresent) {
      PU_PUSHNAME (ctxt_p, "read_only_populations");

      stat = asn1PD_ListPopulation (ctxt_p, &pvalue->read_only_populations);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode read_write_populations */

   if (pvalue->m.read_write_populationsPresent) {
      PU_PUSHNAME (ctxt_p, "read_write_populations");

      stat = asn1PD_ListPopulation (ctxt_p, &pvalue->read_write_populations);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode superviseur */

   PU_PUSHNAME (ctxt_p, "superviseur");

   PU_NEWFIELD (ctxt_p, "boolean");

   stat = pd_bit (ctxt_p, &pvalue->superviseur);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Profile: end\n");

   return (stat);
}

int ASN1C_Profile::Decode ()
{
   return asn1PD_Profile (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgAuthLogin                                              */
/*                                                            */
/**************************************************************/

ASN1C_MsgAuthLogin::ASN1C_MsgAuthLogin (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgAuthLogin& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgAuthLogin (ASN1CTXT* ctxt_p, ASN1T_MsgAuthLogin* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgAuthLogin: start\n");

   /* encode login */

   PU_PUSHNAME (ctxt_p, "login");

   stat = pe_VisibleString (ctxt_p, pvalue->login, 0);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode password */

   PU_PUSHNAME (ctxt_p, "password");

   stat = pe_VisibleString (ctxt_p, pvalue->password, 0);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_MsgAuthLogin: end\n");
   return (stat);
}

int ASN1C_MsgAuthLogin::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgAuthLogin (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgAuthLogin (ASN1CTXT* ctxt_p, ASN1T_MsgAuthLogin* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgAuthLogin: start\n");

   /* decode login */

   PU_PUSHNAME (ctxt_p, "login");

   stat = pd_VisibleString (ctxt_p, &pvalue->login, 0);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode password */

   PU_PUSHNAME (ctxt_p, "password");

   stat = pd_VisibleString (ctxt_p, &pvalue->password, 0);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_MsgAuthLogin: end\n");

   return (stat);
}

int ASN1C_MsgAuthLogin::Decode ()
{
   return asn1PD_MsgAuthLogin (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgAuthLoginAck_etat                                      */
/*                                                            */
/**************************************************************/

ASN1C_MsgAuthLoginAck_etat::ASN1C_MsgAuthLoginAck_etat (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgAuthLoginAck_etat& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgAuthLoginAck_etat (ASN1CTXT* ctxt_p, ASN1T_MsgAuthLoginAck_etat value)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   rtdiag ("asn1PE_MsgAuthLoginAck_etat: start\n");

   switch (value) {
      case MsgAuthLoginAck_etat::success: ui = 0; break;
      case MsgAuthLoginAck_etat::invalid_login: ui = 1; break;
      default: return LOG_ASN1ERR (ctxt_p, ASN_E_INVENUM);
   }

   stat = pe_ConsUnsigned (ctxt_p, ui, 0, 1);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   rtdiag ("asn1PE_MsgAuthLoginAck_etat: end\n");
   return (stat);
}

int ASN1C_MsgAuthLoginAck_etat::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgAuthLoginAck_etat (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_MsgAuthLoginAck_etat (ASN1CTXT* ctxt_p, ASN1T_MsgAuthLoginAck_etat* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   rtdiag ("asn1PD_MsgAuthLoginAck_etat: start\n");

   stat = pd_ConsUnsigned (ctxt_p, &ui, 0, 1);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   switch (ui) {
      case 0: *pvalue = MsgAuthLoginAck_etat::success; break;
      case 1: *pvalue = MsgAuthLoginAck_etat::invalid_login; break;
      default: return LOG_ASN1ERR (ctxt_p, ASN_E_INVENUM);
   }

   rtdiag ("asn1PD_MsgAuthLoginAck_etat: end\n");

   return (stat);
}

int ASN1C_MsgAuthLoginAck_etat::Decode ()
{
   return asn1PD_MsgAuthLoginAck_etat (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgAuthLoginAck                                           */
/*                                                            */
/**************************************************************/

ASN1C_MsgAuthLoginAck::ASN1C_MsgAuthLoginAck (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgAuthLoginAck& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgAuthLoginAck (ASN1CTXT* ctxt_p, ASN1T_MsgAuthLoginAck* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgAuthLoginAck: start\n");

   PU_NEWFIELD (ctxt_p, "profilePresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.profilePresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   /* encode etat */

   PU_PUSHNAME (ctxt_p, "etat");

   stat = asn1PE_MsgAuthLoginAck_etat (ctxt_p, pvalue->etat);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode profile */

   if (pvalue->m.profilePresent) {
      PU_PUSHNAME (ctxt_p, "profile");

      stat = asn1PE_Profile (ctxt_p, &pvalue->profile);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }


   rtdiag ("asn1PE_MsgAuthLoginAck: end\n");
   return (stat);
}

int ASN1C_MsgAuthLoginAck::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgAuthLoginAck (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgAuthLoginAck (ASN1CTXT* ctxt_p, ASN1T_MsgAuthLoginAck* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL optbit;

   rtdiag ("asn1PD_MsgAuthLoginAck: start\n");

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   PU_NEWFIELD (ctxt_p, "profilePresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.profilePresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   /* decode etat */

   PU_PUSHNAME (ctxt_p, "etat");

   stat = asn1PD_MsgAuthLoginAck_etat (ctxt_p, &pvalue->etat);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode profile */

   if (pvalue->m.profilePresent) {
      PU_PUSHNAME (ctxt_p, "profile");

      stat = asn1PD_Profile (ctxt_p, &pvalue->profile);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }


   rtdiag ("asn1PD_MsgAuthLoginAck: end\n");

   return (stat);
}

int ASN1C_MsgAuthLoginAck::Decode ()
{
   return asn1PD_MsgAuthLoginAck (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgCtrlProfileCreation                                    */
/*                                                            */
/**************************************************************/

ASN1C_MsgCtrlProfileCreation::ASN1C_MsgCtrlProfileCreation (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgCtrlProfileCreation& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgCtrlProfileCreation (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlProfileCreation* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgCtrlProfileCreation: start\n");

   stat = asn1PE_Profile (ctxt_p, pvalue);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   rtdiag ("asn1PE_MsgCtrlProfileCreation: end\n");
   return (stat);
}

int ASN1C_MsgCtrlProfileCreation::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgCtrlProfileCreation (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgCtrlProfileCreation (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlProfileCreation* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgCtrlProfileCreation: start\n");

   stat = asn1PD_Profile (ctxt_p, pvalue);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   rtdiag ("asn1PD_MsgCtrlProfileCreation: end\n");

   return (stat);
}

int ASN1C_MsgCtrlProfileCreation::Decode ()
{
   return asn1PD_MsgCtrlProfileCreation (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgCtrlProfileUpdate                                      */
/*                                                            */
/**************************************************************/

ASN1C_MsgCtrlProfileUpdate::ASN1C_MsgCtrlProfileUpdate (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgCtrlProfileUpdate& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgCtrlProfileUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlProfileUpdate* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgCtrlProfileUpdate: start\n");

   stat = asn1PE_Profile (ctxt_p, pvalue);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   rtdiag ("asn1PE_MsgCtrlProfileUpdate: end\n");
   return (stat);
}

int ASN1C_MsgCtrlProfileUpdate::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgCtrlProfileUpdate (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgCtrlProfileUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlProfileUpdate* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgCtrlProfileUpdate: start\n");

   stat = asn1PD_Profile (ctxt_p, pvalue);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   rtdiag ("asn1PD_MsgCtrlProfileUpdate: end\n");

   return (stat);
}

int ASN1C_MsgCtrlProfileUpdate::Decode ()
{
   return asn1PD_MsgCtrlProfileUpdate (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgCtrlProfileDestruction                                 */
/*                                                            */
/**************************************************************/

ASN1C_MsgCtrlProfileDestruction::ASN1C_MsgCtrlProfileDestruction (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgCtrlProfileDestruction& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgCtrlProfileDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlProfileDestruction value)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgCtrlProfileDestruction: start\n");

   stat = pe_VisibleString (ctxt_p, value, 0);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   rtdiag ("asn1PE_MsgCtrlProfileDestruction: end\n");
   return (stat);
}

int ASN1C_MsgCtrlProfileDestruction::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgCtrlProfileDestruction (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_MsgCtrlProfileDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlProfileDestruction* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgCtrlProfileDestruction: start\n");

   stat = pd_VisibleString (ctxt_p, pvalue, 0);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   rtdiag ("asn1PD_MsgCtrlProfileDestruction: end\n");

   return (stat);
}

int ASN1C_MsgCtrlProfileDestruction::Decode ()
{
   return asn1PD_MsgCtrlProfileDestruction (mpContext->GetPtr(), &msgData);
}


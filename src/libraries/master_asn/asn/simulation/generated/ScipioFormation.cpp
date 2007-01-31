/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 25-Jan-2007.
 */
#include "ScipioFormation.h"

/**************************************************************/
/*                                                            */
/*  ScipioFormationIncludes                                   */
/*                                                            */
/**************************************************************/

ASN1C_ScipioFormationIncludes::ASN1C_ScipioFormationIncludes (
   ASN1MessageBuffer& msgBuf, ASN1T_ScipioFormationIncludes& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_ScipioFormationIncludes (ASN1CTXT* ctxt_p, ASN1T_ScipioFormationIncludes* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_ScipioFormationIncludes: start\n");

   /* encode scipioBaseExports */

   PU_PUSHNAME (ctxt_p, "scipioBaseExports");

   stat = asn1PE_ScipioBaseExports (ctxt_p);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_ScipioFormationIncludes: end\n");
   return (stat);
}

int ASN1C_ScipioFormationIncludes::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_ScipioFormationIncludes (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_ScipioFormationIncludes (ASN1CTXT* ctxt_p, ASN1T_ScipioFormationIncludes* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_ScipioFormationIncludes: start\n");

   /* decode scipioBaseExports */

   PU_PUSHNAME (ctxt_p, "scipioBaseExports");

   stat = asn1PD_ScipioBaseExports (ctxt_p);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_ScipioFormationIncludes: end\n");

   return (stat);
}

int ASN1C_ScipioFormationIncludes::Decode ()
{
   return asn1PD_ScipioFormationIncludes (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  ScipioFormationExports                                    */
/*                                                            */
/**************************************************************/

ASN1C_ScipioFormationExports::ASN1C_ScipioFormationExports (
   ASN1MessageBuffer& msgBuf) :
   ASN1CType(msgBuf)
{}

EXTERN int asn1PE_ScipioFormationExports (ASN1CTXT* ctxt_p)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_ScipioFormationExports: start\n");

   /* NULL */

   rtdiag ("asn1PE_ScipioFormationExports: end\n");
   return (stat);
}

int ASN1C_ScipioFormationExports::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_ScipioFormationExports (mpContext->GetPtr());
   return stat;
}

EXTERN int asn1PD_ScipioFormationExports (ASN1CTXT* ctxt_p)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_ScipioFormationExports: start\n");

   /* NULL */

   rtdiag ("asn1PD_ScipioFormationExports: end\n");

   return (stat);
}

int ASN1C_ScipioFormationExports::Decode ()
{
   return asn1PD_ScipioFormationExports (mpContext->GetPtr());
}

/**************************************************************/
/*                                                            */
/*  MsgFormationCreation                                      */
/*                                                            */
/**************************************************************/

ASN1C_MsgFormationCreation::ASN1C_MsgFormationCreation (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgFormationCreation& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgFormationCreation (ASN1CTXT* ctxt_p, ASN1T_MsgFormationCreation* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgFormationCreation: start\n");

   PU_NEWFIELD (ctxt_p, "oid_formation_parentePresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.oid_formation_parentePresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   /* encode oid */

   PU_PUSHNAME (ctxt_p, "oid");

   stat = asn1PE_Formation (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode oid_camp */

   PU_PUSHNAME (ctxt_p, "oid_camp");

   stat = asn1PE_Camp (ctxt_p, pvalue->oid_camp);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode oid_formation_parente */

   if (pvalue->m.oid_formation_parentePresent) {
      PU_PUSHNAME (ctxt_p, "oid_formation_parente");

      stat = asn1PE_Formation (ctxt_p, pvalue->oid_formation_parente);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   /* encode niveau */

   PU_PUSHNAME (ctxt_p, "niveau");

   stat = asn1PE_EnumNatureLevel (ctxt_p, pvalue->niveau);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode nom */

   PU_PUSHNAME (ctxt_p, "nom");

   stat = pe_VisibleString (ctxt_p, pvalue->nom, 0);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_MsgFormationCreation: end\n");
   return (stat);
}

int ASN1C_MsgFormationCreation::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgFormationCreation (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgFormationCreation (ASN1CTXT* ctxt_p, ASN1T_MsgFormationCreation* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL optbit;

   rtdiag ("asn1PD_MsgFormationCreation: start\n");

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   PU_NEWFIELD (ctxt_p, "oid_formation_parentePresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.oid_formation_parentePresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   /* decode oid */

   PU_PUSHNAME (ctxt_p, "oid");

   stat = asn1PD_Formation (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode oid_camp */

   PU_PUSHNAME (ctxt_p, "oid_camp");

   stat = asn1PD_Camp (ctxt_p, &pvalue->oid_camp);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode oid_formation_parente */

   if (pvalue->m.oid_formation_parentePresent) {
      PU_PUSHNAME (ctxt_p, "oid_formation_parente");

      stat = asn1PD_Formation (ctxt_p, &pvalue->oid_formation_parente);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }

   /* decode niveau */

   PU_PUSHNAME (ctxt_p, "niveau");

   stat = asn1PD_EnumNatureLevel (ctxt_p, &pvalue->niveau);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode nom */

   PU_PUSHNAME (ctxt_p, "nom");

   stat = pd_VisibleString (ctxt_p, &pvalue->nom, 0);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_MsgFormationCreation: end\n");

   return (stat);
}

int ASN1C_MsgFormationCreation::Decode ()
{
   return asn1PD_MsgFormationCreation (mpContext->GetPtr(), &msgData);
}


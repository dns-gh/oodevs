/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 31-Jan-2007.
 */
#include "Formation.h"

/**************************************************************/
/*                                                            */
/*  FormationIncludes                                         */
/*                                                            */
/**************************************************************/

ASN1C_FormationIncludes::ASN1C_FormationIncludes (
   ASN1MessageBuffer& msgBuf, ASN1T_FormationIncludes& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_FormationIncludes (ASN1CTXT* ctxt_p, ASN1T_FormationIncludes* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_FormationIncludes: start\n");

   /* encode baseExports */

   PU_PUSHNAME (ctxt_p, "baseExports");

   stat = asn1PE_BaseExports (ctxt_p);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_FormationIncludes: end\n");
   return (stat);
}

int ASN1C_FormationIncludes::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_FormationIncludes (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_FormationIncludes (ASN1CTXT* ctxt_p, ASN1T_FormationIncludes* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_FormationIncludes: start\n");

   /* decode baseExports */

   PU_PUSHNAME (ctxt_p, "baseExports");

   stat = asn1PD_BaseExports (ctxt_p);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_FormationIncludes: end\n");

   return (stat);
}

int ASN1C_FormationIncludes::Decode ()
{
   return asn1PD_FormationIncludes (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  FormationExports                                          */
/*                                                            */
/**************************************************************/

ASN1C_FormationExports::ASN1C_FormationExports (
   ASN1MessageBuffer& msgBuf) :
   ASN1CType(msgBuf)
{}

EXTERN int asn1PE_FormationExports (ASN1CTXT* ctxt_p)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_FormationExports: start\n");

   /* NULL */

   rtdiag ("asn1PE_FormationExports: end\n");
   return (stat);
}

int ASN1C_FormationExports::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_FormationExports (mpContext->GetPtr());
   return stat;
}

EXTERN int asn1PD_FormationExports (ASN1CTXT* ctxt_p)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_FormationExports: start\n");

   /* NULL */

   rtdiag ("asn1PD_FormationExports: end\n");

   return (stat);
}

int ASN1C_FormationExports::Decode ()
{
   return asn1PD_FormationExports (mpContext->GetPtr());
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


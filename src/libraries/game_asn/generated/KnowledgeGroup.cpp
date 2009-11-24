/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 23-Nov-2009.
 */
#include "KnowledgeGroup.h"

/**************************************************************/
/*                                                            */
/*  KnowledgeGroupIncludes                                    */
/*                                                            */
/**************************************************************/

ASN1C_KnowledgeGroupIncludes::ASN1C_KnowledgeGroupIncludes (
   ASN1MessageBuffer& msgBuf, ASN1T_KnowledgeGroupIncludes& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_KnowledgeGroupIncludes (ASN1CTXT* ctxt_p, ASN1T_KnowledgeGroupIncludes* pvalue)
{
   int stat = ASN_OK;

   /* encode baseExports */

   stat = asn1PE_BaseExports (ctxt_p);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_KnowledgeGroupIncludes::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_KnowledgeGroupIncludes (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_KnowledgeGroupIncludes (ASN1CTXT* ctxt_p, ASN1T_KnowledgeGroupIncludes* pvalue)
{
   int stat = ASN_OK;

   /* decode baseExports */

   stat = asn1PD_BaseExports (ctxt_p);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_KnowledgeGroupIncludes::Decode ()
{
   return asn1PD_KnowledgeGroupIncludes (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  KnowledgeGroupExports                                     */
/*                                                            */
/**************************************************************/

ASN1C_KnowledgeGroupExports::ASN1C_KnowledgeGroupExports (
   ASN1MessageBuffer& msgBuf) :
   ASN1CType(msgBuf)
{}

EXTERN int asn1PE_KnowledgeGroupExports (ASN1CTXT* ctxt_p)
{
   int stat = ASN_OK;

   /* NULL */
   return (stat);
}

int ASN1C_KnowledgeGroupExports::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_KnowledgeGroupExports (mpContext->GetPtr());
   return stat;
}

EXTERN int asn1PD_KnowledgeGroupExports (ASN1CTXT* ctxt_p)
{
   int stat = ASN_OK;

   /* NULL */

   return (stat);
}

int ASN1C_KnowledgeGroupExports::Decode ()
{
   return asn1PD_KnowledgeGroupExports (mpContext->GetPtr());
}

/**************************************************************/
/*                                                            */
/*  MsgKnowledgeGroupCreation                                 */
/*                                                            */
/**************************************************************/

ASN1C_MsgKnowledgeGroupCreation::ASN1C_MsgKnowledgeGroupCreation (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgKnowledgeGroupCreation& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgKnowledgeGroupCreation (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupCreation* pvalue)
{
   int stat = ASN_OK;

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.oid_knowledgegroup_parentePresent);

   /* encode oid */

   stat = asn1PE_KnowledgeGroup (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* encode oid_camp */

   stat = asn1PE_Team (ctxt_p, pvalue->oid_camp);
   if (stat != ASN_OK) return stat;

   /* encode oid_knowledgegroup_parente */

   if (pvalue->m.oid_knowledgegroup_parentePresent) {
      stat = asn1PE_KnowledgeGroup (ctxt_p, pvalue->oid_knowledgegroup_parente);
      if (stat != ASN_OK) return stat;
   }

   return (stat);
}

int ASN1C_MsgKnowledgeGroupCreation::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgKnowledgeGroupCreation (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgKnowledgeGroupCreation (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupCreation* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL optbit;

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   pd_bit (ctxt_p, &optbit);
   pvalue->m.oid_knowledgegroup_parentePresent = optbit;

   /* decode oid */

   stat = asn1PD_KnowledgeGroup (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* decode oid_camp */

   stat = asn1PD_Team (ctxt_p, &pvalue->oid_camp);
   if (stat != ASN_OK) return stat;

   /* decode oid_knowledgegroup_parente */

   if (pvalue->m.oid_knowledgegroup_parentePresent) {
      stat = asn1PD_KnowledgeGroup (ctxt_p, &pvalue->oid_knowledgegroup_parente);
      if (stat != ASN_OK) return stat;
   }


   return (stat);
}

int ASN1C_MsgKnowledgeGroupCreation::Decode ()
{
   return asn1PD_MsgKnowledgeGroupCreation (mpContext->GetPtr(), &msgData);
}


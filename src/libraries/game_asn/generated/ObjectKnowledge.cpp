/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 24-Dec-2009.
 */
#include "ObjectKnowledge.h"

/**************************************************************/
/*                                                            */
/*  ObjectKnowledgeIncludes                                   */
/*                                                            */
/**************************************************************/

ASN1C_ObjectKnowledgeIncludes::ASN1C_ObjectKnowledgeIncludes (
   ASN1MessageBuffer& msgBuf, ASN1T_ObjectKnowledgeIncludes& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_ObjectKnowledgeIncludes (ASN1CTXT* ctxt_p, ASN1T_ObjectKnowledgeIncludes* pvalue)
{
   int stat = ASN_OK;

   /* encode baseExports */

   stat = asn1PE_BaseExports (ctxt_p);
   if (stat != ASN_OK) return stat;

   /* encode objectExports */

   stat = asn1PE_ObjectExports (ctxt_p);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_ObjectKnowledgeIncludes::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_ObjectKnowledgeIncludes (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_ObjectKnowledgeIncludes (ASN1CTXT* ctxt_p, ASN1T_ObjectKnowledgeIncludes* pvalue)
{
   int stat = ASN_OK;

   /* decode baseExports */

   stat = asn1PD_BaseExports (ctxt_p);
   if (stat != ASN_OK) return stat;

   /* decode objectExports */

   stat = asn1PD_ObjectExports (ctxt_p);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_ObjectKnowledgeIncludes::Decode ()
{
   return asn1PD_ObjectKnowledgeIncludes (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  ObjectKnowledgeExports                                    */
/*                                                            */
/**************************************************************/

ASN1C_ObjectKnowledgeExports::ASN1C_ObjectKnowledgeExports (
   ASN1MessageBuffer& msgBuf) :
   ASN1CType(msgBuf)
{}

EXTERN int asn1PE_ObjectKnowledgeExports (ASN1CTXT* ctxt_p)
{
   int stat = ASN_OK;

   /* NULL */
   return (stat);
}

int ASN1C_ObjectKnowledgeExports::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_ObjectKnowledgeExports (mpContext->GetPtr());
   return stat;
}

EXTERN int asn1PD_ObjectKnowledgeExports (ASN1CTXT* ctxt_p)
{
   int stat = ASN_OK;

   /* NULL */

   return (stat);
}

int ASN1C_ObjectKnowledgeExports::Decode ()
{
   return asn1PD_ObjectKnowledgeExports (mpContext->GetPtr());
}

/**************************************************************/
/*                                                            */
/*  MsgObjectKnowledgeCreation                                */
/*                                                            */
/**************************************************************/

ASN1C_MsgObjectKnowledgeCreation::ASN1C_MsgObjectKnowledgeCreation (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgObjectKnowledgeCreation& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgObjectKnowledgeCreation (ASN1CTXT* ctxt_p, ASN1T_MsgObjectKnowledgeCreation* pvalue)
{
   int stat = ASN_OK;

   /* encode oid */

   stat = asn1PE_ObjectKnowledge (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* encode team */

   stat = asn1PE_Team (ctxt_p, pvalue->team);
   if (stat != ASN_OK) return stat;

   /* encode real_object */

   stat = asn1PE_OID (ctxt_p, pvalue->real_object);
   if (stat != ASN_OK) return stat;

   /* encode type */

   stat = pe_VisibleString (ctxt_p, pvalue->type, 0);
   if (stat != ASN_OK) return stat;

   /* encode attributes */

   stat = asn1PE_ObjectAttributes (ctxt_p, &pvalue->attributes);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgObjectKnowledgeCreation::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgObjectKnowledgeCreation (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgObjectKnowledgeCreation (ASN1CTXT* ctxt_p, ASN1T_MsgObjectKnowledgeCreation* pvalue)
{
   int stat = ASN_OK;

   /* decode oid */

   stat = asn1PD_ObjectKnowledge (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* decode team */

   stat = asn1PD_Team (ctxt_p, &pvalue->team);
   if (stat != ASN_OK) return stat;

   /* decode real_object */

   stat = asn1PD_OID (ctxt_p, &pvalue->real_object);
   if (stat != ASN_OK) return stat;

   /* decode type */

   stat = pd_VisibleString (ctxt_p, &pvalue->type, 0);
   if (stat != ASN_OK) return stat;

   /* decode attributes */

   stat = asn1PD_ObjectAttributes (ctxt_p, &pvalue->attributes);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgObjectKnowledgeCreation::Decode ()
{
   return asn1PD_MsgObjectKnowledgeCreation (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgObjectKnowledgeUpdate                                  */
/*                                                            */
/**************************************************************/

ASN1C_MsgObjectKnowledgeUpdate::ASN1C_MsgObjectKnowledgeUpdate (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgObjectKnowledgeUpdate& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgObjectKnowledgeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgObjectKnowledgeUpdate* pvalue)
{
   int stat = ASN_OK;

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.real_objectPresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.relevancePresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.locationPresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.attributesPresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.perceivedPresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.automat_perceptionPresent);

   /* encode oid */

   stat = asn1PE_ObjectKnowledge (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* encode team */

   stat = asn1PE_Team (ctxt_p, pvalue->team);
   if (stat != ASN_OK) return stat;

   /* encode real_object */

   if (pvalue->m.real_objectPresent) {
      stat = asn1PE_OID (ctxt_p, pvalue->real_object);
      if (stat != ASN_OK) return stat;
   }

   /* encode relevance */

   if (pvalue->m.relevancePresent) {
      stat = pe_ConsInteger (ctxt_p, pvalue->relevance, 0, 100);
      if (stat != ASN_OK) return stat;

   }

   /* encode location */

   if (pvalue->m.locationPresent) {
      stat = asn1PE_Location (ctxt_p, &pvalue->location);
      if (stat != ASN_OK) return stat;
   }

   /* encode attributes */

   if (pvalue->m.attributesPresent) {
      stat = asn1PE_ObjectAttributes (ctxt_p, &pvalue->attributes);
      if (stat != ASN_OK) return stat;
   }

   /* encode perceived */

   if (pvalue->m.perceivedPresent) {
      stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->perceived);
      if (stat != ASN_OK) return stat;
   }

   /* encode automat_perception */

   if (pvalue->m.automat_perceptionPresent) {
      stat = asn1PE_ListOID (ctxt_p, &pvalue->automat_perception);
      if (stat != ASN_OK) return stat;
   }

   return (stat);
}

int ASN1C_MsgObjectKnowledgeUpdate::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgObjectKnowledgeUpdate (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgObjectKnowledgeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgObjectKnowledgeUpdate* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL optbit;

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   pd_bit (ctxt_p, &optbit);
   pvalue->m.real_objectPresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.relevancePresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.locationPresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.attributesPresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.perceivedPresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.automat_perceptionPresent = optbit;

   /* decode oid */

   stat = asn1PD_ObjectKnowledge (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* decode team */

   stat = asn1PD_Team (ctxt_p, &pvalue->team);
   if (stat != ASN_OK) return stat;

   /* decode real_object */

   if (pvalue->m.real_objectPresent) {
      stat = asn1PD_OID (ctxt_p, &pvalue->real_object);
      if (stat != ASN_OK) return stat;
   }

   /* decode relevance */

   if (pvalue->m.relevancePresent) {
      stat = pd_ConsInteger (ctxt_p, &pvalue->relevance, 0, 100);
      if (stat != ASN_OK) return stat;
   }

   /* decode location */

   if (pvalue->m.locationPresent) {
      stat = asn1PD_Location (ctxt_p, &pvalue->location);
      if (stat != ASN_OK) return stat;
   }

   /* decode attributes */

   if (pvalue->m.attributesPresent) {
      stat = asn1PD_ObjectAttributes (ctxt_p, &pvalue->attributes);
      if (stat != ASN_OK) return stat;
   }

   /* decode perceived */

   if (pvalue->m.perceivedPresent) {
      stat = pd_bit (ctxt_p, &pvalue->perceived);
      if (stat != ASN_OK) return stat;
   }

   /* decode automat_perception */

   if (pvalue->m.automat_perceptionPresent) {
      stat = asn1PD_ListOID (ctxt_p, &pvalue->automat_perception);
      if (stat != ASN_OK) return stat;
   }


   return (stat);
}

int ASN1C_MsgObjectKnowledgeUpdate::Decode ()
{
   return asn1PD_MsgObjectKnowledgeUpdate (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgObjectKnowledgeDestruction                             */
/*                                                            */
/**************************************************************/

ASN1C_MsgObjectKnowledgeDestruction::ASN1C_MsgObjectKnowledgeDestruction (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgObjectKnowledgeDestruction& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgObjectKnowledgeDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgObjectKnowledgeDestruction* pvalue)
{
   int stat = ASN_OK;

   /* encode oid */

   stat = asn1PE_ObjectKnowledge (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* encode team */

   stat = asn1PE_Team (ctxt_p, pvalue->team);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgObjectKnowledgeDestruction::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgObjectKnowledgeDestruction (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgObjectKnowledgeDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgObjectKnowledgeDestruction* pvalue)
{
   int stat = ASN_OK;

   /* decode oid */

   stat = asn1PD_ObjectKnowledge (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* decode team */

   stat = asn1PD_Team (ctxt_p, &pvalue->team);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgObjectKnowledgeDestruction::Decode ()
{
   return asn1PD_MsgObjectKnowledgeDestruction (mpContext->GetPtr(), &msgData);
}


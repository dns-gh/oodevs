/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 08-Jan-2010.
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

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.oid_knowledgegroup_parentPresent);

   /* encode oid */

   stat = asn1PE_KnowledgeGroup (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* encode oid_camp */

   stat = asn1PE_Team (ctxt_p, pvalue->oid_camp);
   if (stat != ASN_OK) return stat;

   /* encode oid_knowledgegroup_parent */

   if (pvalue->m.oid_knowledgegroup_parentPresent) {
      stat = asn1PE_KnowledgeGroup (ctxt_p, pvalue->oid_knowledgegroup_parent);
      if (stat != ASN_OK) return stat;
   }

   /* encode type */

   stat = asn1PE_KnowledgeGroupType (ctxt_p, pvalue->type);
   if (stat != ASN_OK) return stat;

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
   pvalue->m.oid_knowledgegroup_parentPresent = optbit;

   /* decode oid */

   stat = asn1PD_KnowledgeGroup (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* decode oid_camp */

   stat = asn1PD_Team (ctxt_p, &pvalue->oid_camp);
   if (stat != ASN_OK) return stat;

   /* decode oid_knowledgegroup_parent */

   if (pvalue->m.oid_knowledgegroup_parentPresent) {
      stat = asn1PD_KnowledgeGroup (ctxt_p, &pvalue->oid_knowledgegroup_parent);
      if (stat != ASN_OK) return stat;
   }

   /* decode type */

   stat = asn1PD_KnowledgeGroupType (ctxt_p, &pvalue->type);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgKnowledgeGroupCreation::Decode ()
{
   return asn1PD_MsgKnowledgeGroupCreation (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgKnowledgeGroupUpdate                                   */
/*                                                            */
/**************************************************************/

ASN1C_MsgKnowledgeGroupUpdate::ASN1C_MsgKnowledgeGroupUpdate (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgKnowledgeGroupUpdate& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgKnowledgeGroupUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupUpdate* pvalue)
{
   int stat = ASN_OK;

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.oid_knowledgegroup_parentPresent);

   /* encode oid */

   stat = asn1PE_KnowledgeGroup (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* encode oid_knowledgegroup_parent */

   if (pvalue->m.oid_knowledgegroup_parentPresent) {
      stat = asn1PE_KnowledgeGroup (ctxt_p, pvalue->oid_knowledgegroup_parent);
      if (stat != ASN_OK) return stat;
   }

   /* encode enabled */

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->enabled);
   if (stat != ASN_OK) return stat;

   /* encode type */

   stat = asn1PE_KnowledgeGroupType (ctxt_p, pvalue->type);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgKnowledgeGroupUpdate::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgKnowledgeGroupUpdate (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgKnowledgeGroupUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupUpdate* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL optbit;

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   pd_bit (ctxt_p, &optbit);
   pvalue->m.oid_knowledgegroup_parentPresent = optbit;

   /* decode oid */

   stat = asn1PD_KnowledgeGroup (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* decode oid_knowledgegroup_parent */

   if (pvalue->m.oid_knowledgegroup_parentPresent) {
      stat = asn1PD_KnowledgeGroup (ctxt_p, &pvalue->oid_knowledgegroup_parent);
      if (stat != ASN_OK) return stat;
   }

   /* decode enabled */

   stat = pd_bit (ctxt_p, &pvalue->enabled);
   if (stat != ASN_OK) return stat;

   /* decode type */

   stat = asn1PD_KnowledgeGroupType (ctxt_p, &pvalue->type);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgKnowledgeGroupUpdate::Decode ()
{
   return asn1PD_MsgKnowledgeGroupUpdate (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgKnowledgeGroupDelete                                   */
/*                                                            */
/**************************************************************/

ASN1C_MsgKnowledgeGroupDelete::ASN1C_MsgKnowledgeGroupDelete (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgKnowledgeGroupDelete& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgKnowledgeGroupDelete (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupDelete* pvalue)
{
   int stat = ASN_OK;

   /* encode oid */

   stat = asn1PE_KnowledgeGroup (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgKnowledgeGroupDelete::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgKnowledgeGroupDelete (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgKnowledgeGroupDelete (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupDelete* pvalue)
{
   int stat = ASN_OK;

   /* decode oid */

   stat = asn1PD_KnowledgeGroup (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgKnowledgeGroupDelete::Decode ()
{
   return asn1PD_MsgKnowledgeGroupDelete (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgKnowledgeGroupEnable                                   */
/*                                                            */
/**************************************************************/

ASN1C_MsgKnowledgeGroupEnable::ASN1C_MsgKnowledgeGroupEnable (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgKnowledgeGroupEnable& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgKnowledgeGroupEnable (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupEnable* pvalue)
{
   int stat = ASN_OK;

   /* encode oid */

   stat = asn1PE_KnowledgeGroup (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* encode enabled */

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->enabled);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgKnowledgeGroupEnable::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgKnowledgeGroupEnable (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgKnowledgeGroupEnable (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupEnable* pvalue)
{
   int stat = ASN_OK;

   /* decode oid */

   stat = asn1PD_KnowledgeGroup (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* decode enabled */

   stat = pd_bit (ctxt_p, &pvalue->enabled);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgKnowledgeGroupEnable::Decode ()
{
   return asn1PD_MsgKnowledgeGroupEnable (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgKnowledgeGroupSetType                                  */
/*                                                            */
/**************************************************************/

ASN1C_MsgKnowledgeGroupSetType::ASN1C_MsgKnowledgeGroupSetType (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgKnowledgeGroupSetType& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgKnowledgeGroupSetType (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupSetType* pvalue)
{
   int stat = ASN_OK;

   /* encode oid */

   stat = asn1PE_KnowledgeGroup (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* encode type */

   stat = asn1PE_KnowledgeGroupType (ctxt_p, pvalue->type);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgKnowledgeGroupSetType::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgKnowledgeGroupSetType (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgKnowledgeGroupSetType (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupSetType* pvalue)
{
   int stat = ASN_OK;

   /* decode oid */

   stat = asn1PD_KnowledgeGroup (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* decode type */

   stat = asn1PD_KnowledgeGroupType (ctxt_p, &pvalue->type);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgKnowledgeGroupSetType::Decode ()
{
   return asn1PD_MsgKnowledgeGroupSetType (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgKnowledgeGroupChangeSuperior                           */
/*                                                            */
/**************************************************************/

ASN1C_MsgKnowledgeGroupChangeSuperior::ASN1C_MsgKnowledgeGroupChangeSuperior (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgKnowledgeGroupChangeSuperior& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgKnowledgeGroupChangeSuperior (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupChangeSuperior* pvalue)
{
   int stat = ASN_OK;

   /* encode oid */

   stat = asn1PE_KnowledgeGroup (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* encode oid_camp */

   stat = asn1PE_Team (ctxt_p, pvalue->oid_camp);
   if (stat != ASN_OK) return stat;

   /* encode oid_knowledgegroup_parent */

   stat = asn1PE_KnowledgeGroup (ctxt_p, pvalue->oid_knowledgegroup_parent);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgKnowledgeGroupChangeSuperior::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgKnowledgeGroupChangeSuperior (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgKnowledgeGroupChangeSuperior (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupChangeSuperior* pvalue)
{
   int stat = ASN_OK;

   /* decode oid */

   stat = asn1PD_KnowledgeGroup (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* decode oid_camp */

   stat = asn1PD_Team (ctxt_p, &pvalue->oid_camp);
   if (stat != ASN_OK) return stat;

   /* decode oid_knowledgegroup_parent */

   stat = asn1PD_KnowledgeGroup (ctxt_p, &pvalue->oid_knowledgegroup_parent);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgKnowledgeGroupChangeSuperior::Decode ()
{
   return asn1PD_MsgKnowledgeGroupChangeSuperior (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  EnumKnowledgeGroupErrorCode                               */
/*                                                            */
/**************************************************************/

ASN1C_EnumKnowledgeGroupErrorCode::ASN1C_EnumKnowledgeGroupErrorCode (
   ASN1MessageBuffer& msgBuf, ASN1T_EnumKnowledgeGroupErrorCode& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_EnumKnowledgeGroupErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumKnowledgeGroupErrorCode value)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   switch (value) {
      case EnumKnowledgeGroupErrorCode::no_error: ui = 0; break;
      case EnumKnowledgeGroupErrorCode::error_invalid_unit: ui = 1; break;
      case EnumKnowledgeGroupErrorCode::error_invalid_superior: ui = 2; break;
      case EnumKnowledgeGroupErrorCode::error_invalid_camp: ui = 3; break;
      case EnumKnowledgeGroupErrorCode::error_invalid_knowledgegroup: ui = 4; break;
      case EnumKnowledgeGroupErrorCode::error_invalid_type: ui = 5; break;
      default: return ASN_E_INVENUM;
   }

   stat = pe_ConsUnsigned (ctxt_p, ui, 0, 5);
   if (stat != ASN_OK) return stat;
   return (stat);
}

int ASN1C_EnumKnowledgeGroupErrorCode::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_EnumKnowledgeGroupErrorCode (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_EnumKnowledgeGroupErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumKnowledgeGroupErrorCode* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   stat = pd_ConsUnsigned (ctxt_p, &ui, 0, 5);
   if (stat != ASN_OK) return stat;

   switch (ui) {
      case 0: *pvalue = EnumKnowledgeGroupErrorCode::no_error; break;
      case 1: *pvalue = EnumKnowledgeGroupErrorCode::error_invalid_unit; break;
      case 2: *pvalue = EnumKnowledgeGroupErrorCode::error_invalid_superior; break;
      case 3: *pvalue = EnumKnowledgeGroupErrorCode::error_invalid_camp; break;
      case 4: *pvalue = EnumKnowledgeGroupErrorCode::error_invalid_knowledgegroup; break;
      case 5: *pvalue = EnumKnowledgeGroupErrorCode::error_invalid_type; break;
      default: return ASN_E_INVENUM;
   }

   return (stat);
}

int ASN1C_EnumKnowledgeGroupErrorCode::Decode ()
{
   return asn1PD_EnumKnowledgeGroupErrorCode (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgKnowledgeGroupChangeSuperiorAck                        */
/*                                                            */
/**************************************************************/

ASN1C_MsgKnowledgeGroupChangeSuperiorAck::ASN1C_MsgKnowledgeGroupChangeSuperiorAck (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgKnowledgeGroupChangeSuperiorAck& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgKnowledgeGroupChangeSuperiorAck (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupChangeSuperiorAck* pvalue)
{
   int stat = ASN_OK;

   /* encode oid */

   stat = asn1PE_OID (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* encode error_code */

   stat = asn1PE_EnumKnowledgeGroupErrorCode (ctxt_p, pvalue->error_code);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgKnowledgeGroupChangeSuperiorAck::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgKnowledgeGroupChangeSuperiorAck (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgKnowledgeGroupChangeSuperiorAck (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupChangeSuperiorAck* pvalue)
{
   int stat = ASN_OK;

   /* decode oid */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* decode error_code */

   stat = asn1PD_EnumKnowledgeGroupErrorCode (ctxt_p, &pvalue->error_code);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgKnowledgeGroupChangeSuperiorAck::Decode ()
{
   return asn1PD_MsgKnowledgeGroupChangeSuperiorAck (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgKnowledgeGroupDeleteAck                                */
/*                                                            */
/**************************************************************/

ASN1C_MsgKnowledgeGroupDeleteAck::ASN1C_MsgKnowledgeGroupDeleteAck (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgKnowledgeGroupDeleteAck& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgKnowledgeGroupDeleteAck (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupDeleteAck* pvalue)
{
   int stat = ASN_OK;

   /* encode oid */

   stat = asn1PE_OID (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* encode error_code */

   stat = asn1PE_EnumKnowledgeGroupErrorCode (ctxt_p, pvalue->error_code);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgKnowledgeGroupDeleteAck::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgKnowledgeGroupDeleteAck (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgKnowledgeGroupDeleteAck (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupDeleteAck* pvalue)
{
   int stat = ASN_OK;

   /* decode oid */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* decode error_code */

   stat = asn1PD_EnumKnowledgeGroupErrorCode (ctxt_p, &pvalue->error_code);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgKnowledgeGroupDeleteAck::Decode ()
{
   return asn1PD_MsgKnowledgeGroupDeleteAck (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgKnowledgeGroupSetTypeAck                               */
/*                                                            */
/**************************************************************/

ASN1C_MsgKnowledgeGroupSetTypeAck::ASN1C_MsgKnowledgeGroupSetTypeAck (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgKnowledgeGroupSetTypeAck& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgKnowledgeGroupSetTypeAck (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupSetTypeAck* pvalue)
{
   int stat = ASN_OK;

   /* encode oid */

   stat = asn1PE_OID (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* encode error_code */

   stat = asn1PE_EnumKnowledgeGroupErrorCode (ctxt_p, pvalue->error_code);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgKnowledgeGroupSetTypeAck::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgKnowledgeGroupSetTypeAck (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgKnowledgeGroupSetTypeAck (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupSetTypeAck* pvalue)
{
   int stat = ASN_OK;

   /* decode oid */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* decode error_code */

   stat = asn1PD_EnumKnowledgeGroupErrorCode (ctxt_p, &pvalue->error_code);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgKnowledgeGroupSetTypeAck::Decode ()
{
   return asn1PD_MsgKnowledgeGroupSetTypeAck (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgKnowledgeGroupCreationAck                              */
/*                                                            */
/**************************************************************/

ASN1C_MsgKnowledgeGroupCreationAck::ASN1C_MsgKnowledgeGroupCreationAck (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgKnowledgeGroupCreationAck& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgKnowledgeGroupCreationAck (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupCreationAck* pvalue)
{
   int stat = ASN_OK;

   /* encode oid */

   stat = asn1PE_OID (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* encode error_code */

   stat = asn1PE_EnumKnowledgeGroupErrorCode (ctxt_p, pvalue->error_code);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgKnowledgeGroupCreationAck::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgKnowledgeGroupCreationAck (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgKnowledgeGroupCreationAck (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupCreationAck* pvalue)
{
   int stat = ASN_OK;

   /* decode oid */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* decode error_code */

   stat = asn1PD_EnumKnowledgeGroupErrorCode (ctxt_p, &pvalue->error_code);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgKnowledgeGroupCreationAck::Decode ()
{
   return asn1PD_MsgKnowledgeGroupCreationAck (mpContext->GetPtr(), &msgData);
}


/* This file was generated by ASN1C V5.31 on 18-Jun-2007 */

#include "KnowledgeGroup.h"

void asn1Print_KnowledgeGroupIncludes (ASN1ConstCharPtr name, ASN1T_KnowledgeGroupIncludes* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.baseExports", name);

}

void ASN1C_KnowledgeGroupIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_KnowledgeGroupIncludes (name, &msgData);
}

void asn1Print_KnowledgeGroupExports (ASN1ConstCharPtr name)
{
}

void ASN1C_KnowledgeGroupExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_KnowledgeGroupExports (name);
}

void asn1Print_MsgKnowledgeGroupCreation (ASN1ConstCharPtr name, ASN1T_MsgKnowledgeGroupCreation* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_KnowledgeGroup (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.oid_camp", name);
   asn1Print_Team (namebuf, &pvalue->oid_camp);

}

void ASN1C_MsgKnowledgeGroupCreation::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgKnowledgeGroupCreation (name, &msgData);
}


/* This file was generated by ASN1C V5.31 on 06-Jan-2010 */

#include "UrbanKnowledge.h"

void asn1Print_UrbanKnowledgeIncludes (ASN1ConstCharPtr name, ASN1T_UrbanKnowledgeIncludes* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.baseExports", name);

   sprintf (namebuf, "%s.urbanExports", name);

}

void ASN1C_UrbanKnowledgeIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_UrbanKnowledgeIncludes (name, &msgData);
}

void asn1Print_UrbanKnowledgeExports (ASN1ConstCharPtr name)
{
}

void ASN1C_UrbanKnowledgeExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_UrbanKnowledgeExports (name);
}

void asn1Print_MsgUrbanKnowledgeCreation (ASN1ConstCharPtr name, ASN1T_MsgUrbanKnowledgeCreation* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_UrbanKnowledge (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.team", name);
   asn1Print_Team (namebuf, &pvalue->team);

   sprintf (namebuf, "%s.real_urban", name);
   asn1Print_OID (namebuf, &pvalue->real_urban);

}

void ASN1C_MsgUrbanKnowledgeCreation::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgUrbanKnowledgeCreation (name, &msgData);
}

void asn1Print_MsgUrbanKnowledgeUpdate (ASN1ConstCharPtr name, ASN1T_MsgUrbanKnowledgeUpdate* pvalue)
{
   char namebuf[512];

   printf ("%s.m.real_urbanPresent = %d\n", name, (int)pvalue->m.real_urbanPresent);
   printf ("%s.m.relevancePresent = %d\n", name, (int)pvalue->m.relevancePresent);
   printf ("%s.m.identification_levelPresent = %d\n", name, (int)pvalue->m.identification_levelPresent);
   printf ("%s.m.progressPresent = %d\n", name, (int)pvalue->m.progressPresent);
   printf ("%s.m.perceivedPresent = %d\n", name, (int)pvalue->m.perceivedPresent);
   printf ("%s.m.automat_perceptionPresent = %d\n", name, (int)pvalue->m.automat_perceptionPresent);
   sprintf (namebuf, "%s.oid", name);
   asn1Print_UrbanKnowledge (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.team", name);
   asn1Print_Team (namebuf, &pvalue->team);

   if (pvalue->m.real_urbanPresent) {
      sprintf (namebuf, "%s.real_urban", name);
      asn1Print_OID (namebuf, &pvalue->real_urban);
   }

   if (pvalue->m.relevancePresent) {
      sprintf (namebuf, "%s.relevance", name);
      rtPrintInteger (namebuf, pvalue->relevance);
   }

   if (pvalue->m.identification_levelPresent) {
      sprintf (namebuf, "%s.identification_level", name);
      asn1Print_EnumUnitIdentificationLevel (namebuf, &pvalue->identification_level);
   }

   if (pvalue->m.progressPresent) {
      sprintf (namebuf, "%s.progress", name);
      rtPrintInteger (namebuf, pvalue->progress);
   }

   if (pvalue->m.perceivedPresent) {
      sprintf (namebuf, "%s.perceived", name);
      rtPrintBoolean (namebuf, pvalue->perceived);
   }

   if (pvalue->m.automat_perceptionPresent) {
      sprintf (namebuf, "%s.automat_perception", name);
      asn1Print_ListOID (namebuf, &pvalue->automat_perception);
   }

}

void ASN1C_MsgUrbanKnowledgeUpdate::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgUrbanKnowledgeUpdate (name, &msgData);
}

void asn1Print_MsgUrbanKnowledgeDestruction (ASN1ConstCharPtr name, ASN1T_MsgUrbanKnowledgeDestruction* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_UrbanKnowledge (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.team", name);
   asn1Print_Team (namebuf, &pvalue->team);

}

void ASN1C_MsgUrbanKnowledgeDestruction::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgUrbanKnowledgeDestruction (name, &msgData);
}


/* This file was generated by ASN1C V5.31 on 09-Jul-2009 */

#include "Intelligence.h"

void asn1Print_IntelligenceIncludes (ASN1ConstCharPtr name, ASN1T_IntelligenceIncludes* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.baseExports", name);

   sprintf (namebuf, "%s.armyExports", name);

}

void ASN1C_IntelligenceIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_IntelligenceIncludes (name, &msgData);
}

void asn1Print_IntelligenceExports (ASN1ConstCharPtr name)
{
}

void ASN1C_IntelligenceExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_IntelligenceExports (name);
}

void asn1Print_Intelligence (ASN1ConstCharPtr name, ASN1T_Intelligence* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.name", name);
   rtPrintCharStr (namebuf, pvalue->name);

   sprintf (namebuf, "%s.nature", name);
   rtPrintCharStr (namebuf, pvalue->nature);

   sprintf (namebuf, "%s.level", name);
   asn1Print_EnumNatureLevel (namebuf, &pvalue->level);

   sprintf (namebuf, "%s.embarked", name);
   rtPrintBoolean (namebuf, pvalue->embarked);

   sprintf (namebuf, "%s.location", name);
   asn1Print_CoordLatLong (namebuf, &pvalue->location);

   sprintf (namebuf, "%s.diplomacy", name);
   asn1Print_EnumDiplomacy (namebuf, &pvalue->diplomacy);

   sprintf (namebuf, "%s.formation", name);
   asn1Print_Formation (namebuf, &pvalue->formation);

}

void ASN1C_Intelligence::Print (ASN1ConstCharPtr name)

{
   asn1Print_Intelligence (name, &msgData);
}

void asn1Print_IntelligenceList (ASN1ConstCharPtr name, ASN1T_IntelligenceList* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_Intelligence (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C_IntelligenceList::Print (ASN1ConstCharPtr name)

{
   asn1Print_IntelligenceList (name, &msgData);
}

void asn1Print_MsgIntelligenceCreation (ASN1ConstCharPtr name, ASN1T_MsgIntelligenceCreation* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.intelligence", name);
   asn1Print_Intelligence (namebuf, &pvalue->intelligence);

}

void ASN1C_MsgIntelligenceCreation::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgIntelligenceCreation (name, &msgData);
}

void asn1Print_MsgIntelligenceUpdate (ASN1ConstCharPtr name, ASN1T_MsgIntelligenceUpdate* pvalue)
{
   char namebuf[512];

   printf ("%s.m.formationPresent = %d\n", name, (int)pvalue->m.formationPresent);
   printf ("%s.m.namePresent = %d\n", name, (int)pvalue->m.namePresent);
   printf ("%s.m.naturePresent = %d\n", name, (int)pvalue->m.naturePresent);
   printf ("%s.m.levelPresent = %d\n", name, (int)pvalue->m.levelPresent);
   printf ("%s.m.embarkedPresent = %d\n", name, (int)pvalue->m.embarkedPresent);
   printf ("%s.m.locationPresent = %d\n", name, (int)pvalue->m.locationPresent);
   printf ("%s.m.diplomacyPresent = %d\n", name, (int)pvalue->m.diplomacyPresent);
   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   if (pvalue->m.formationPresent) {
      sprintf (namebuf, "%s.formation", name);
      asn1Print_Formation (namebuf, &pvalue->formation);
   }

   if (pvalue->m.namePresent) {
      sprintf (namebuf, "%s.name", name);
      rtPrintCharStr (namebuf, pvalue->name);
   }

   if (pvalue->m.naturePresent) {
      sprintf (namebuf, "%s.nature", name);
      rtPrintCharStr (namebuf, pvalue->nature);
   }

   if (pvalue->m.levelPresent) {
      sprintf (namebuf, "%s.level", name);
      asn1Print_EnumNatureLevel (namebuf, &pvalue->level);
   }

   if (pvalue->m.embarkedPresent) {
      sprintf (namebuf, "%s.embarked", name);
      rtPrintBoolean (namebuf, pvalue->embarked);
   }

   if (pvalue->m.locationPresent) {
      sprintf (namebuf, "%s.location", name);
      asn1Print_CoordLatLong (namebuf, &pvalue->location);
   }

   if (pvalue->m.diplomacyPresent) {
      sprintf (namebuf, "%s.diplomacy", name);
      asn1Print_EnumDiplomacy (namebuf, &pvalue->diplomacy);
   }

}

void ASN1C_MsgIntelligenceUpdate::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgIntelligenceUpdate (name, &msgData);
}

void asn1Print_MsgIntelligenceDestruction (ASN1ConstCharPtr name, ASN1T_MsgIntelligenceDestruction* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

}

void ASN1C_MsgIntelligenceDestruction::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgIntelligenceDestruction (name, &msgData);
}

void asn1Print_MsgIntelligenceCreationRequest (ASN1ConstCharPtr name, ASN1T_MsgIntelligenceCreationRequest* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.intelligence", name);
   asn1Print_Intelligence (namebuf, &pvalue->intelligence);

}

void ASN1C_MsgIntelligenceCreationRequest::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgIntelligenceCreationRequest (name, &msgData);
}

void asn1Print_MsgIntelligenceUpdateRequest (ASN1ConstCharPtr name, ASN1T_MsgIntelligenceUpdateRequest* pvalue)
{
   char namebuf[512];

   printf ("%s.m.formationPresent = %d\n", name, (int)pvalue->m.formationPresent);
   printf ("%s.m.namePresent = %d\n", name, (int)pvalue->m.namePresent);
   printf ("%s.m.naturePresent = %d\n", name, (int)pvalue->m.naturePresent);
   printf ("%s.m.levelPresent = %d\n", name, (int)pvalue->m.levelPresent);
   printf ("%s.m.embarkedPresent = %d\n", name, (int)pvalue->m.embarkedPresent);
   printf ("%s.m.locationPresent = %d\n", name, (int)pvalue->m.locationPresent);
   printf ("%s.m.diplomacyPresent = %d\n", name, (int)pvalue->m.diplomacyPresent);
   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   if (pvalue->m.formationPresent) {
      sprintf (namebuf, "%s.formation", name);
      asn1Print_Formation (namebuf, &pvalue->formation);
   }

   if (pvalue->m.namePresent) {
      sprintf (namebuf, "%s.name", name);
      rtPrintCharStr (namebuf, pvalue->name);
   }

   if (pvalue->m.naturePresent) {
      sprintf (namebuf, "%s.nature", name);
      rtPrintCharStr (namebuf, pvalue->nature);
   }

   if (pvalue->m.levelPresent) {
      sprintf (namebuf, "%s.level", name);
      asn1Print_EnumNatureLevel (namebuf, &pvalue->level);
   }

   if (pvalue->m.embarkedPresent) {
      sprintf (namebuf, "%s.embarked", name);
      rtPrintBoolean (namebuf, pvalue->embarked);
   }

   if (pvalue->m.locationPresent) {
      sprintf (namebuf, "%s.location", name);
      asn1Print_CoordLatLong (namebuf, &pvalue->location);
   }

   if (pvalue->m.diplomacyPresent) {
      sprintf (namebuf, "%s.diplomacy", name);
      asn1Print_EnumDiplomacy (namebuf, &pvalue->diplomacy);
   }

}

void ASN1C_MsgIntelligenceUpdateRequest::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgIntelligenceUpdateRequest (name, &msgData);
}

void asn1Print_MsgIntelligenceDestructionRequest (ASN1ConstCharPtr name, ASN1T_MsgIntelligenceDestructionRequest* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

}

void ASN1C_MsgIntelligenceDestructionRequest::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgIntelligenceDestructionRequest (name, &msgData);
}

void asn1Print_EnumIntelligenceErrorCode (ASN1ConstCharPtr name, ASN1T_EnumIntelligenceErrorCode* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("no_error\n"); break;
      case 1: printf ("error_invalid_oid\n"); break;
      case 2: printf ("error_invalid_geometry\n"); break;
      case 3: printf ("error_invalid_formation\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumIntelligenceErrorCode::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumIntelligenceErrorCode (name, &msgData);
}

void asn1Print_MsgIntelligenceCreationRequestAck (ASN1ConstCharPtr name, ASN1T_MsgIntelligenceCreationRequestAck* pvalue)
{
   asn1Print_EnumIntelligenceErrorCode (name, pvalue);
}

void ASN1C_MsgIntelligenceCreationRequestAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgIntelligenceCreationRequestAck (name, &msgData);
}

void asn1Print_MsgIntelligenceUpdateRequestAck (ASN1ConstCharPtr name, ASN1T_MsgIntelligenceUpdateRequestAck* pvalue)
{
   asn1Print_EnumIntelligenceErrorCode (name, pvalue);
}

void ASN1C_MsgIntelligenceUpdateRequestAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgIntelligenceUpdateRequestAck (name, &msgData);
}

void asn1Print_MsgIntelligenceDestructionRequestAck (ASN1ConstCharPtr name, ASN1T_MsgIntelligenceDestructionRequestAck* pvalue)
{
   asn1Print_EnumIntelligenceErrorCode (name, pvalue);
}

void ASN1C_MsgIntelligenceDestructionRequestAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgIntelligenceDestructionRequestAck (name, &msgData);
}


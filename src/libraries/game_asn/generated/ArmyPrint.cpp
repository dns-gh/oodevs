/* This file was generated by ASN1C V5.31 on 18-Jun-2007 */

#include "Army.h"

void asn1Print_ArmyIncludes (ASN1ConstCharPtr name, ASN1T_ArmyIncludes* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.baseExports", name);

}

void ASN1C_ArmyIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_ArmyIncludes (name, &msgData);
}

void asn1Print_ArmyExports (ASN1ConstCharPtr name)
{
}

void ASN1C_ArmyExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_ArmyExports (name);
}

void asn1Print_EnumDiplomacy (ASN1ConstCharPtr name, ASN1T_EnumDiplomacy* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("inconnu\n"); break;
      case 1: printf ("ami\n"); break;
      case 2: printf ("ennemi\n"); break;
      case 3: printf ("neutre\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumDiplomacy::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumDiplomacy (name, &msgData);
}

void asn1Print_MsgChangeDiplomacy (ASN1ConstCharPtr name, ASN1T_MsgChangeDiplomacy* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_camp1", name);
   asn1Print_Team (namebuf, &pvalue->oid_camp1);

   sprintf (namebuf, "%s.oid_camp2", name);
   asn1Print_Team (namebuf, &pvalue->oid_camp2);

   sprintf (namebuf, "%s.diplomatie", name);
   asn1Print_EnumDiplomacy (namebuf, &pvalue->diplomatie);

}

void ASN1C_MsgChangeDiplomacy::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgChangeDiplomacy (name, &msgData);
}

void asn1Print_EnumChangeDiplomacyErrorCode (ASN1ConstCharPtr name, ASN1T_EnumChangeDiplomacyErrorCode* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("no_error\n"); break;
      case 1: printf ("error_invalid_camp\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumChangeDiplomacyErrorCode::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumChangeDiplomacyErrorCode (name, &msgData);
}

void asn1Print_MsgTeamCreation (ASN1ConstCharPtr name, ASN1T_MsgTeamCreation* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_Team (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.nom", name);
   rtPrintCharStr (namebuf, pvalue->nom);

   sprintf (namebuf, "%s.type", name);
   asn1Print_EnumDiplomacy (namebuf, &pvalue->type);

}

void ASN1C_MsgTeamCreation::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgTeamCreation (name, &msgData);
}

void asn1Print_MsgChangeDiplomacyAck (ASN1ConstCharPtr name, ASN1T_MsgChangeDiplomacyAck* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_camp1", name);
   asn1Print_Team (namebuf, &pvalue->oid_camp1);

   sprintf (namebuf, "%s.oid_camp2", name);
   asn1Print_Team (namebuf, &pvalue->oid_camp2);

   sprintf (namebuf, "%s.diplomatie", name);
   asn1Print_EnumDiplomacy (namebuf, &pvalue->diplomatie);

   sprintf (namebuf, "%s.error_code", name);
   asn1Print_EnumChangeDiplomacyErrorCode (namebuf, &pvalue->error_code);

}

void ASN1C_MsgChangeDiplomacyAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgChangeDiplomacyAck (name, &msgData);
}


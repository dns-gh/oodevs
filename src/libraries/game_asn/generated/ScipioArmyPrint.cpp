/* This file was generated by ASN1C V5.31 on 31-Jan-2007 */

#include "ScipioArmy.h"

void asn1Print_ScipioArmyIncludes (ASN1ConstCharPtr name, ASN1T_ScipioArmyIncludes* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.scipioBaseExports", name);

}

void ASN1C_ScipioArmyIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_ScipioArmyIncludes (name, &msgData);
}

void asn1Print_ScipioArmyExports (ASN1ConstCharPtr name)
{
}

void ASN1C_ScipioArmyExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_ScipioArmyExports (name);
}

void asn1Print_EnumDiplomatie (ASN1ConstCharPtr name, ASN1T_EnumDiplomatie* pvalue)
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

void ASN1C_EnumDiplomatie::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumDiplomatie (name, &msgData);
}

void asn1Print_MsgChangeDiplomatie (ASN1ConstCharPtr name, ASN1T_MsgChangeDiplomatie* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_camp1", name);
   asn1Print_Camp (namebuf, &pvalue->oid_camp1);

   sprintf (namebuf, "%s.oid_camp2", name);
   asn1Print_Camp (namebuf, &pvalue->oid_camp2);

   sprintf (namebuf, "%s.diplomatie", name);
   asn1Print_EnumDiplomatie (namebuf, &pvalue->diplomatie);

}

void ASN1C_MsgChangeDiplomatie::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgChangeDiplomatie (name, &msgData);
}

void asn1Print_EnumChangeDiplomatieErrorCode (ASN1ConstCharPtr name, ASN1T_EnumChangeDiplomatieErrorCode* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("no_error\n"); break;
      case 1: printf ("error_invalid_camp\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumChangeDiplomatieErrorCode::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumChangeDiplomatieErrorCode (name, &msgData);
}

void asn1Print_MsgSideCreation (ASN1ConstCharPtr name, ASN1T_MsgSideCreation* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_Camp (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.nom", name);
   rtPrintCharStr (namebuf, pvalue->nom);

   sprintf (namebuf, "%s.type", name);
   asn1Print_EnumDiplomatie (namebuf, &pvalue->type);

}

void ASN1C_MsgSideCreation::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgSideCreation (name, &msgData);
}

void asn1Print_MsgChangeDiplomatieAck (ASN1ConstCharPtr name, ASN1T_MsgChangeDiplomatieAck* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_camp1", name);
   asn1Print_Camp (namebuf, &pvalue->oid_camp1);

   sprintf (namebuf, "%s.oid_camp2", name);
   asn1Print_Camp (namebuf, &pvalue->oid_camp2);

   sprintf (namebuf, "%s.diplomatie", name);
   asn1Print_EnumDiplomatie (namebuf, &pvalue->diplomatie);

   sprintf (namebuf, "%s.error_code", name);
   asn1Print_EnumChangeDiplomatieErrorCode (namebuf, &pvalue->error_code);

}

void ASN1C_MsgChangeDiplomatieAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgChangeDiplomatieAck (name, &msgData);
}


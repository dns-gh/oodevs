/* This file was generated by ASN1C V5.31 on 25-Jan-2007 */

#include "ScipioAuth.h"

void asn1Print_ScipioAuthIncludes (ASN1ConstCharPtr name, ASN1T_ScipioAuthIncludes* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.scipioBaseExports", name);

}

void ASN1C_ScipioAuthIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_ScipioAuthIncludes (name, &msgData);
}

void asn1Print_ScipioAuthExports (ASN1ConstCharPtr name)
{
}

void ASN1C_ScipioAuthExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_ScipioAuthExports (name);
}

void asn1Print_Profile (ASN1ConstCharPtr name, ASN1T_Profile* pvalue)
{
   char namebuf[512];

   printf ("%s.m.passwordPresent = %d\n", name, (int)pvalue->m.passwordPresent);
   printf ("%s.m.read_only_formationsPresent = %d\n", name, (int)pvalue->m.read_only_formationsPresent);
   printf ("%s.m.read_write_formationsPresent = %d\n", name, (int)pvalue->m.read_write_formationsPresent);
   printf ("%s.m.read_only_automatesPresent = %d\n", name, (int)pvalue->m.read_only_automatesPresent);
   printf ("%s.m.read_write_automatesPresent = %d\n", name, (int)pvalue->m.read_write_automatesPresent);
   printf ("%s.m.read_only_campsPresent = %d\n", name, (int)pvalue->m.read_only_campsPresent);
   printf ("%s.m.read_write_campsPresent = %d\n", name, (int)pvalue->m.read_write_campsPresent);
   printf ("%s.m.read_only_populationsPresent = %d\n", name, (int)pvalue->m.read_only_populationsPresent);
   printf ("%s.m.read_write_populationsPresent = %d\n", name, (int)pvalue->m.read_write_populationsPresent);
   sprintf (namebuf, "%s.login", name);
   rtPrintCharStr (namebuf, pvalue->login);

   if (pvalue->m.passwordPresent) {
      sprintf (namebuf, "%s.password", name);
      rtPrintCharStr (namebuf, pvalue->password);
   }

   if (pvalue->m.read_only_formationsPresent) {
      sprintf (namebuf, "%s.read_only_formations", name);
      asn1Print_ListFormation (namebuf, &pvalue->read_only_formations);
   }

   if (pvalue->m.read_write_formationsPresent) {
      sprintf (namebuf, "%s.read_write_formations", name);
      asn1Print_ListFormation (namebuf, &pvalue->read_write_formations);
   }

   if (pvalue->m.read_only_automatesPresent) {
      sprintf (namebuf, "%s.read_only_automates", name);
      asn1Print_ListAutomate (namebuf, &pvalue->read_only_automates);
   }

   if (pvalue->m.read_write_automatesPresent) {
      sprintf (namebuf, "%s.read_write_automates", name);
      asn1Print_ListAutomate (namebuf, &pvalue->read_write_automates);
   }

   if (pvalue->m.read_only_campsPresent) {
      sprintf (namebuf, "%s.read_only_camps", name);
      asn1Print_ListCamp (namebuf, &pvalue->read_only_camps);
   }

   if (pvalue->m.read_write_campsPresent) {
      sprintf (namebuf, "%s.read_write_camps", name);
      asn1Print_ListCamp (namebuf, &pvalue->read_write_camps);
   }

   if (pvalue->m.read_only_populationsPresent) {
      sprintf (namebuf, "%s.read_only_populations", name);
      asn1Print_ListPopulation (namebuf, &pvalue->read_only_populations);
   }

   if (pvalue->m.read_write_populationsPresent) {
      sprintf (namebuf, "%s.read_write_populations", name);
      asn1Print_ListPopulation (namebuf, &pvalue->read_write_populations);
   }

   sprintf (namebuf, "%s.superviseur", name);
   rtPrintBoolean (namebuf, pvalue->superviseur);

}

void ASN1C_Profile::Print (ASN1ConstCharPtr name)

{
   asn1Print_Profile (name, &msgData);
}

void asn1Print_MsgAuthLogin (ASN1ConstCharPtr name, ASN1T_MsgAuthLogin* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.login", name);
   rtPrintCharStr (namebuf, pvalue->login);

   sprintf (namebuf, "%s.password", name);
   rtPrintCharStr (namebuf, pvalue->password);

}

void ASN1C_MsgAuthLogin::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgAuthLogin (name, &msgData);
}

void asn1Print_MsgAuthLoginAck_etat (ASN1ConstCharPtr name, ASN1T_MsgAuthLoginAck_etat* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("success\n"); break;
      case 1: printf ("invalid_login\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_MsgAuthLoginAck_etat::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgAuthLoginAck_etat (name, &msgData);
}

void asn1Print_MsgAuthLoginAck (ASN1ConstCharPtr name, ASN1T_MsgAuthLoginAck* pvalue)
{
   char namebuf[512];

   printf ("%s.m.profilePresent = %d\n", name, (int)pvalue->m.profilePresent);
   sprintf (namebuf, "%s.etat", name);
   asn1Print_MsgAuthLoginAck_etat (namebuf, &pvalue->etat);

   if (pvalue->m.profilePresent) {
      sprintf (namebuf, "%s.profile", name);
      asn1Print_Profile (namebuf, &pvalue->profile);
   }

}

void ASN1C_MsgAuthLoginAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgAuthLoginAck (name, &msgData);
}

void asn1Print_MsgProfileCreation (ASN1ConstCharPtr name, ASN1T_MsgProfileCreation* pvalue)
{
   asn1Print_Profile (name, pvalue);
}

void ASN1C_MsgProfileCreation::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgProfileCreation (name, &msgData);
}

void asn1Print_MsgProfileCreationRequest (ASN1ConstCharPtr name, ASN1T_MsgProfileCreationRequest* pvalue)
{
   asn1Print_Profile (name, pvalue);
}

void ASN1C_MsgProfileCreationRequest::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgProfileCreationRequest (name, &msgData);
}

void asn1Print_MsgProfileCreationRequestAck_error_code (ASN1ConstCharPtr name, ASN1T_MsgProfileCreationRequestAck_error_code* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("success\n"); break;
      case 1: printf ("invalid_login\n"); break;
      case 2: printf ("invalid_password\n"); break;
      case 3: printf ("duplicate_login\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_MsgProfileCreationRequestAck_error_code::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgProfileCreationRequestAck_error_code (name, &msgData);
}

void asn1Print_MsgProfileCreationRequestAck (ASN1ConstCharPtr name, ASN1T_MsgProfileCreationRequestAck* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.error_code", name);
   asn1Print_MsgProfileCreationRequestAck_error_code (namebuf, &pvalue->error_code);

   sprintf (namebuf, "%s.login", name);
   rtPrintCharStr (namebuf, pvalue->login);

}

void ASN1C_MsgProfileCreationRequestAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgProfileCreationRequestAck (name, &msgData);
}

void asn1Print_MsgProfileUpdate (ASN1ConstCharPtr name, ASN1T_MsgProfileUpdate* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.login", name);
   rtPrintCharStr (namebuf, pvalue->login);

   sprintf (namebuf, "%s.profile", name);
   asn1Print_Profile (namebuf, &pvalue->profile);

}

void ASN1C_MsgProfileUpdate::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgProfileUpdate (name, &msgData);
}

void asn1Print_MsgProfileUpdateRequest (ASN1ConstCharPtr name, ASN1T_MsgProfileUpdateRequest* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.login", name);
   rtPrintCharStr (namebuf, pvalue->login);

   sprintf (namebuf, "%s.profile", name);
   asn1Print_Profile (namebuf, &pvalue->profile);

}

void ASN1C_MsgProfileUpdateRequest::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgProfileUpdateRequest (name, &msgData);
}

void asn1Print_MsgProfileUpdateRequestAck_error_code (ASN1ConstCharPtr name, ASN1T_MsgProfileUpdateRequestAck_error_code* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("success\n"); break;
      case 1: printf ("invalid_profile\n"); break;
      case 2: printf ("invalid_login\n"); break;
      case 3: printf ("invalid_password\n"); break;
      case 4: printf ("duplicate_login\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_MsgProfileUpdateRequestAck_error_code::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgProfileUpdateRequestAck_error_code (name, &msgData);
}

void asn1Print_MsgProfileUpdateRequestAck (ASN1ConstCharPtr name, ASN1T_MsgProfileUpdateRequestAck* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.error_code", name);
   asn1Print_MsgProfileUpdateRequestAck_error_code (namebuf, &pvalue->error_code);

   sprintf (namebuf, "%s.login", name);
   rtPrintCharStr (namebuf, pvalue->login);

}

void ASN1C_MsgProfileUpdateRequestAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgProfileUpdateRequestAck (name, &msgData);
}

void asn1Print_MsgProfileDestruction (ASN1ConstCharPtr name, ASN1T_MsgProfileDestruction* pvalue)
{
   rtPrintCharStr (name, *pvalue);
}

void ASN1C_MsgProfileDestruction::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgProfileDestruction (name, &msgData);
}

void asn1Print_MsgProfileDestructionRequest (ASN1ConstCharPtr name, ASN1T_MsgProfileDestructionRequest* pvalue)
{
   rtPrintCharStr (name, *pvalue);
}

void ASN1C_MsgProfileDestructionRequest::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgProfileDestructionRequest (name, &msgData);
}

void asn1Print_MsgProfileDestructionRequestAck_error_code (ASN1ConstCharPtr name, ASN1T_MsgProfileDestructionRequestAck_error_code* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("success\n"); break;
      case 1: printf ("invalid_profile\n"); break;
      case 2: printf ("failure\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_MsgProfileDestructionRequestAck_error_code::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgProfileDestructionRequestAck_error_code (name, &msgData);
}

void asn1Print_MsgProfileDestructionRequestAck (ASN1ConstCharPtr name, ASN1T_MsgProfileDestructionRequestAck* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.error_code", name);
   asn1Print_MsgProfileDestructionRequestAck_error_code (namebuf, &pvalue->error_code);

   sprintf (namebuf, "%s.login", name);
   rtPrintCharStr (namebuf, pvalue->login);

}

void ASN1C_MsgProfileDestructionRequestAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgProfileDestructionRequestAck (name, &msgData);
}


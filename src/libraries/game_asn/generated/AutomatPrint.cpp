/* This file was generated by ASN1C V5.31 on 19-Jun-2007 */

#include "Automat.h"

void asn1Print_AutomateIncludes (ASN1ConstCharPtr name, ASN1T_AutomateIncludes* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.baseExports", name);

   sprintf (namebuf, "%s.unitExports", name);

}

void ASN1C_AutomateIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_AutomateIncludes (name, &msgData);
}

void asn1Print_AutomateExports (ASN1ConstCharPtr name)
{
}

void ASN1C_AutomateExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_AutomateExports (name);
}

void asn1Print_EnumAutomatMode (ASN1ConstCharPtr name, ASN1T_EnumAutomatMode* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("embraye\n"); break;
      case 1: printf ("debraye\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumAutomatMode::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumAutomatMode (name, &msgData);
}

void asn1Print_MsgAutomatCreation (ASN1ConstCharPtr name, ASN1T_MsgAutomatCreation* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_automate", name);
   asn1Print_Automat (namebuf, &pvalue->oid_automate);

   sprintf (namebuf, "%s.type_automate", name);
   asn1Print_AutomatType (namebuf, &pvalue->type_automate);

   sprintf (namebuf, "%s.nom", name);
   rtPrintCharStr (namebuf, pvalue->nom);

   sprintf (namebuf, "%s.oid_formation", name);
   asn1Print_Formation (namebuf, &pvalue->oid_formation);

   sprintf (namebuf, "%s.oid_camp", name);
   asn1Print_Team (namebuf, &pvalue->oid_camp);

   sprintf (namebuf, "%s.oid_groupe_connaissance", name);
   asn1Print_OID (namebuf, &pvalue->oid_groupe_connaissance);

}

void ASN1C_MsgAutomatCreation::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgAutomatCreation (name, &msgData);
}

void asn1Print_MsgAutomatAttributes (ASN1ConstCharPtr name, ASN1T_MsgAutomatAttributes* pvalue)
{
   char namebuf[512];

   printf ("%s.m.etat_automatePresent = %d\n", name, (int)pvalue->m.etat_automatePresent);
   printf ("%s.m.rapport_de_forcePresent = %d\n", name, (int)pvalue->m.rapport_de_forcePresent);
   printf ("%s.m.combat_de_rencontrePresent = %d\n", name, (int)pvalue->m.combat_de_rencontrePresent);
   printf ("%s.m.etat_operationnelPresent = %d\n", name, (int)pvalue->m.etat_operationnelPresent);
   printf ("%s.m.roePresent = %d\n", name, (int)pvalue->m.roePresent);
   sprintf (namebuf, "%s.oid_automate", name);
   asn1Print_Automat (namebuf, &pvalue->oid_automate);

   if (pvalue->m.etat_automatePresent) {
      sprintf (namebuf, "%s.etat_automate", name);
      asn1Print_EnumAutomatMode (namebuf, &pvalue->etat_automate);
   }

   if (pvalue->m.rapport_de_forcePresent) {
      sprintf (namebuf, "%s.rapport_de_force", name);
      asn1Print_EnumForceRatioStatus (namebuf, &pvalue->rapport_de_force);
   }

   if (pvalue->m.combat_de_rencontrePresent) {
      sprintf (namebuf, "%s.combat_de_rencontre", name);
      asn1Print_EnumMeetingEngagementStatus (namebuf, &pvalue->combat_de_rencontre);
   }

   if (pvalue->m.etat_operationnelPresent) {
      sprintf (namebuf, "%s.etat_operationnel", name);
      asn1Print_EnumOperationalStatus (namebuf, &pvalue->etat_operationnel);
   }

   if (pvalue->m.roePresent) {
      sprintf (namebuf, "%s.roe", name);
      asn1Print_EnumRoe (namebuf, &pvalue->roe);
   }

}

void ASN1C_MsgAutomatAttributes::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgAutomatAttributes (name, &msgData);
}

void asn1Print_EnumSetAutomatModeErrorCode (ASN1ConstCharPtr name, ASN1T_EnumSetAutomatModeErrorCode* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("no_error\n"); break;
      case 1: printf ("error_invalid_unit\n"); break;
      case 2: printf ("error_not_allowed\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumSetAutomatModeErrorCode::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumSetAutomatModeErrorCode (name, &msgData);
}

void asn1Print_MsgSetAutomatMode (ASN1ConstCharPtr name, ASN1T_MsgSetAutomatMode* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.unit_id", name);
   asn1Print_Unit (namebuf, &pvalue->unit_id);

   sprintf (namebuf, "%s.mode", name);
   asn1Print_EnumAutomatMode (namebuf, &pvalue->mode);

}

void ASN1C_MsgSetAutomatMode::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgSetAutomatMode (name, &msgData);
}

void asn1Print_MsgSetAutomatModeAck (ASN1ConstCharPtr name, ASN1T_MsgSetAutomatModeAck* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.unit_id", name);
   asn1Print_Unit (namebuf, &pvalue->unit_id);

   sprintf (namebuf, "%s.error_code", name);
   asn1Print_EnumSetAutomatModeErrorCode (namebuf, &pvalue->error_code);

}

void ASN1C_MsgSetAutomatModeAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgSetAutomatModeAck (name, &msgData);
}

void asn1Print_MsgAutomatChangeKnowledgeGroup (ASN1ConstCharPtr name, ASN1T_MsgAutomatChangeKnowledgeGroup* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_automate", name);
   asn1Print_Automat (namebuf, &pvalue->oid_automate);

   sprintf (namebuf, "%s.oid_camp", name);
   asn1Print_Team (namebuf, &pvalue->oid_camp);

   sprintf (namebuf, "%s.oid_groupe_connaissance", name);
   asn1Print_KnowledgeGroup (namebuf, &pvalue->oid_groupe_connaissance);

}

void ASN1C_MsgAutomatChangeKnowledgeGroup::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgAutomatChangeKnowledgeGroup (name, &msgData);
}

void asn1Print_MsgAutomatChangeKnowledgeGroupAck (ASN1ConstCharPtr name, ASN1T_MsgAutomatChangeKnowledgeGroupAck* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_automate", name);
   asn1Print_Automat (namebuf, &pvalue->oid_automate);

   sprintf (namebuf, "%s.oid_camp", name);
   asn1Print_Team (namebuf, &pvalue->oid_camp);

   sprintf (namebuf, "%s.oid_groupe_connaissance", name);
   asn1Print_KnowledgeGroup (namebuf, &pvalue->oid_groupe_connaissance);

   sprintf (namebuf, "%s.error_code", name);
   asn1Print_EnumChangeHierarchyErrorCode (namebuf, &pvalue->error_code);

}

void ASN1C_MsgAutomatChangeKnowledgeGroupAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgAutomatChangeKnowledgeGroupAck (name, &msgData);
}

void asn1Print_MsgAutomatChangeLogisticLinks (ASN1ConstCharPtr name, ASN1T_MsgAutomatChangeLogisticLinks* pvalue)
{
   char namebuf[512];

   printf ("%s.m.oid_tc2Present = %d\n", name, (int)pvalue->m.oid_tc2Present);
   printf ("%s.m.oid_maintenancePresent = %d\n", name, (int)pvalue->m.oid_maintenancePresent);
   printf ("%s.m.oid_santePresent = %d\n", name, (int)pvalue->m.oid_santePresent);
   printf ("%s.m.oid_ravitaillementPresent = %d\n", name, (int)pvalue->m.oid_ravitaillementPresent);
   sprintf (namebuf, "%s.oid_automate", name);
   asn1Print_Automat (namebuf, &pvalue->oid_automate);

   if (pvalue->m.oid_tc2Present) {
      sprintf (namebuf, "%s.oid_tc2", name);
      asn1Print_Automat (namebuf, &pvalue->oid_tc2);
   }

   if (pvalue->m.oid_maintenancePresent) {
      sprintf (namebuf, "%s.oid_maintenance", name);
      asn1Print_Automat (namebuf, &pvalue->oid_maintenance);
   }

   if (pvalue->m.oid_santePresent) {
      sprintf (namebuf, "%s.oid_sante", name);
      asn1Print_Automat (namebuf, &pvalue->oid_sante);
   }

   if (pvalue->m.oid_ravitaillementPresent) {
      sprintf (namebuf, "%s.oid_ravitaillement", name);
      asn1Print_Automat (namebuf, &pvalue->oid_ravitaillement);
   }

}

void ASN1C_MsgAutomatChangeLogisticLinks::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgAutomatChangeLogisticLinks (name, &msgData);
}

void asn1Print_MsgAutomatChangeLogisticLinksAck (ASN1ConstCharPtr name, ASN1T_MsgAutomatChangeLogisticLinksAck* pvalue)
{
   char namebuf[512];

   printf ("%s.m.oid_tc2Present = %d\n", name, (int)pvalue->m.oid_tc2Present);
   printf ("%s.m.oid_maintenancePresent = %d\n", name, (int)pvalue->m.oid_maintenancePresent);
   printf ("%s.m.oid_santePresent = %d\n", name, (int)pvalue->m.oid_santePresent);
   printf ("%s.m.oid_ravitaillementPresent = %d\n", name, (int)pvalue->m.oid_ravitaillementPresent);
   sprintf (namebuf, "%s.oid_automate", name);
   asn1Print_Automat (namebuf, &pvalue->oid_automate);

   if (pvalue->m.oid_tc2Present) {
      sprintf (namebuf, "%s.oid_tc2", name);
      asn1Print_Automat (namebuf, &pvalue->oid_tc2);
   }

   if (pvalue->m.oid_maintenancePresent) {
      sprintf (namebuf, "%s.oid_maintenance", name);
      asn1Print_Automat (namebuf, &pvalue->oid_maintenance);
   }

   if (pvalue->m.oid_santePresent) {
      sprintf (namebuf, "%s.oid_sante", name);
      asn1Print_Automat (namebuf, &pvalue->oid_sante);
   }

   if (pvalue->m.oid_ravitaillementPresent) {
      sprintf (namebuf, "%s.oid_ravitaillement", name);
      asn1Print_Automat (namebuf, &pvalue->oid_ravitaillement);
   }

   sprintf (namebuf, "%s.error_code", name);
   asn1Print_EnumChangeHierarchyErrorCode (namebuf, &pvalue->error_code);

}

void ASN1C_MsgAutomatChangeLogisticLinksAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgAutomatChangeLogisticLinksAck (name, &msgData);
}


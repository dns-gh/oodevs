/* This file was generated by ASN1C V5.31 on 24-Dec-2009 */

#include "LogSante.h"

void asn1Print_LogSanteIncludes (ASN1ConstCharPtr name, ASN1T_LogSanteIncludes* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.baseExports", name);

}

void ASN1C_LogSanteIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_LogSanteIncludes (name, &msgData);
}

void asn1Print_LogSanteExports (ASN1ConstCharPtr name)
{
}

void ASN1C_LogSanteExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_LogSanteExports (name);
}

void asn1Print_EnumLogMedicalHandlingStatus (ASN1ConstCharPtr name, ASN1T_EnumLogMedicalHandlingStatus* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("attente_disponibilite_ambulance_releve\n"); break;
      case 1: printf ("ambulance_releve_deplacement_aller\n"); break;
      case 2: printf ("ambulance_releve_chargement\n"); break;
      case 3: printf ("attente_chargement_complet_ambulance_releve\n"); break;
      case 4: printf ("ambulance_releve_deplacement_retour\n"); break;
      case 5: printf ("ambulance_releve_dechargement\n"); break;
      case 6: printf ("attente_disponibilite_medecin_pour_diagnostique\n"); break;
      case 7: printf ("diagnostique\n"); break;
      case 8: printf ("recherche_secteur_tri\n"); break;
      case 9: printf ("attente_disponibilite_medecin_pour_tri\n"); break;
      case 10: printf ("tri\n"); break;
      case 11: printf ("recherche_secteur_soin\n"); break;
      case 12: printf ("attente_disponibilite_medecin_pour_soin\n"); break;
      case 13: printf ("soin\n"); break;
      case 14: printf ("hospitalisation\n"); break;
      case 15: printf ("attente_disponibilite_ambulance_ramassage\n"); break;
      case 16: printf ("ambulance_ramassage_chargement\n"); break;
      case 17: printf ("attente_chargement_complet_ambulance_ramassage\n"); break;
      case 18: printf ("ambulance_ramassage_deplacement_aller\n"); break;
      case 19: printf ("ambulance_ramassage_dechargement\n"); break;
      case 20: printf ("termine\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumLogMedicalHandlingStatus::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumLogMedicalHandlingStatus (name, &msgData);
}

void asn1Print_MsgLogMedicalHandlingCreation (ASN1ConstCharPtr name, ASN1T_MsgLogMedicalHandlingCreation* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_consigne", name);
   asn1Print_OID (namebuf, &pvalue->oid_consigne);

   sprintf (namebuf, "%s.oid_pion", name);
   asn1Print_Unit (namebuf, &pvalue->oid_pion);

   sprintf (namebuf, "%s.tick_creation", name);
   rtPrintInteger (namebuf, pvalue->tick_creation);

   sprintf (namebuf, "%s.rang", name);
   asn1Print_EnumHumanRank (namebuf, &pvalue->rang);

   sprintf (namebuf, "%s.blessure", name);
   asn1Print_EnumHumanWound (namebuf, &pvalue->blessure);

   sprintf (namebuf, "%s.blesse_mental", name);
   rtPrintBoolean (namebuf, pvalue->blesse_mental);

   sprintf (namebuf, "%s.contamine_nbc", name);
   rtPrintBoolean (namebuf, pvalue->contamine_nbc);

}

void ASN1C_MsgLogMedicalHandlingCreation::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgLogMedicalHandlingCreation (name, &msgData);
}

void asn1Print_MsgLogMedicalHandlingUpdate (ASN1ConstCharPtr name, ASN1T_MsgLogMedicalHandlingUpdate* pvalue)
{
   char namebuf[512];

   printf ("%s.m.oid_pion_log_traitantPresent = %d\n", name, (int)pvalue->m.oid_pion_log_traitantPresent);
   printf ("%s.m.blessurePresent = %d\n", name, (int)pvalue->m.blessurePresent);
   printf ("%s.m.blesse_mentalPresent = %d\n", name, (int)pvalue->m.blesse_mentalPresent);
   printf ("%s.m.contamine_nbcPresent = %d\n", name, (int)pvalue->m.contamine_nbcPresent);
   printf ("%s.m.etatPresent = %d\n", name, (int)pvalue->m.etatPresent);
   printf ("%s.m.diagnostique_effectuePresent = %d\n", name, (int)pvalue->m.diagnostique_effectuePresent);
   sprintf (namebuf, "%s.oid_consigne", name);
   asn1Print_OID (namebuf, &pvalue->oid_consigne);

   sprintf (namebuf, "%s.oid_pion", name);
   asn1Print_Unit (namebuf, &pvalue->oid_pion);

   if (pvalue->m.oid_pion_log_traitantPresent) {
      sprintf (namebuf, "%s.oid_pion_log_traitant", name);
      asn1Print_Unit (namebuf, &pvalue->oid_pion_log_traitant);
   }

   if (pvalue->m.blessurePresent) {
      sprintf (namebuf, "%s.blessure", name);
      asn1Print_EnumHumanWound (namebuf, &pvalue->blessure);
   }

   if (pvalue->m.blesse_mentalPresent) {
      sprintf (namebuf, "%s.blesse_mental", name);
      rtPrintBoolean (namebuf, pvalue->blesse_mental);
   }

   if (pvalue->m.contamine_nbcPresent) {
      sprintf (namebuf, "%s.contamine_nbc", name);
      rtPrintBoolean (namebuf, pvalue->contamine_nbc);
   }

   if (pvalue->m.etatPresent) {
      sprintf (namebuf, "%s.etat", name);
      asn1Print_EnumLogMedicalHandlingStatus (namebuf, &pvalue->etat);
   }

   if (pvalue->m.diagnostique_effectuePresent) {
      sprintf (namebuf, "%s.diagnostique_effectue", name);
      rtPrintBoolean (namebuf, pvalue->diagnostique_effectue);
   }

}

void ASN1C_MsgLogMedicalHandlingUpdate::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgLogMedicalHandlingUpdate (name, &msgData);
}

void asn1Print_MsgLogMedicalHandlingDestruction (ASN1ConstCharPtr name, ASN1T_MsgLogMedicalHandlingDestruction* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_consigne", name);
   asn1Print_OID (namebuf, &pvalue->oid_consigne);

   sprintf (namebuf, "%s.oid_pion", name);
   asn1Print_Unit (namebuf, &pvalue->oid_pion);

}

void ASN1C_MsgLogMedicalHandlingDestruction::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgLogMedicalHandlingDestruction (name, &msgData);
}

void asn1Print_LogMedicalEquipmentAvailability (ASN1ConstCharPtr name, ASN1T_LogMedicalEquipmentAvailability* pvalue)
{
   char namebuf[512];

   printf ("%s.m.nbr_au_reposPresent = %d\n", name, (int)pvalue->m.nbr_au_reposPresent);
   sprintf (namebuf, "%s.type_equipement", name);
   asn1Print_EquipmentType (namebuf, &pvalue->type_equipement);

   sprintf (namebuf, "%s.nbr_total", name);
   rtPrintInteger (namebuf, pvalue->nbr_total);

   sprintf (namebuf, "%s.nbr_disponibles", name);
   rtPrintInteger (namebuf, pvalue->nbr_disponibles);

   sprintf (namebuf, "%s.nbr_au_travail", name);
   rtPrintInteger (namebuf, pvalue->nbr_au_travail);

   sprintf (namebuf, "%s.nbr_pretes", name);
   rtPrintInteger (namebuf, pvalue->nbr_pretes);

   if (pvalue->m.nbr_au_reposPresent) {
      sprintf (namebuf, "%s.nbr_au_repos", name);
      rtPrintInteger (namebuf, pvalue->nbr_au_repos);
   }

}

void ASN1C_LogMedicalEquipmentAvailability::Print (ASN1ConstCharPtr name)

{
   asn1Print_LogMedicalEquipmentAvailability (name, &msgData);
}

void asn1Print_LogMedicalPriorities (ASN1ConstCharPtr name, ASN1T_LogMedicalPriorities* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_EnumHumanWound (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C_LogMedicalPriorities::Print (ASN1ConstCharPtr name)

{
   asn1Print_LogMedicalPriorities (name, &msgData);
}

void asn1Print__SeqOfLogMedicalEquipmentAvailability (ASN1ConstCharPtr name, ASN1T__SeqOfLogMedicalEquipmentAvailability* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_LogMedicalEquipmentAvailability (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C__SeqOfLogMedicalEquipmentAvailability::Print (ASN1ConstCharPtr name)

{
   asn1Print__SeqOfLogMedicalEquipmentAvailability (name, &msgData);
}

void asn1Print_MsgLogMedicalState (ASN1ConstCharPtr name, ASN1T_MsgLogMedicalState* pvalue)
{
   char namebuf[512];

   printf ("%s.m.chaine_activeePresent = %d\n", name, (int)pvalue->m.chaine_activeePresent);
   printf ("%s.m.prioritesPresent = %d\n", name, (int)pvalue->m.prioritesPresent);
   printf ("%s.m.priorites_tactiquesPresent = %d\n", name, (int)pvalue->m.priorites_tactiquesPresent);
   printf ("%s.m.disponibilites_ambulances_relevePresent = %d\n", name, (int)pvalue->m.disponibilites_ambulances_relevePresent);
   printf ("%s.m.disponibilites_ambulances_ramassagePresent = %d\n", name, (int)pvalue->m.disponibilites_ambulances_ramassagePresent);
   printf ("%s.m.disponibilites_medecinsPresent = %d\n", name, (int)pvalue->m.disponibilites_medecinsPresent);
   sprintf (namebuf, "%s.oid_pion", name);
   asn1Print_Unit (namebuf, &pvalue->oid_pion);

   if (pvalue->m.chaine_activeePresent) {
      sprintf (namebuf, "%s.chaine_activee", name);
      rtPrintBoolean (namebuf, pvalue->chaine_activee);
   }

   if (pvalue->m.prioritesPresent) {
      sprintf (namebuf, "%s.priorites", name);
      asn1Print_LogMedicalPriorities (namebuf, &pvalue->priorites);
   }

   if (pvalue->m.priorites_tactiquesPresent) {
      sprintf (namebuf, "%s.priorites_tactiques", name);
      asn1Print_AutomatList (namebuf, &pvalue->priorites_tactiques);
   }

   if (pvalue->m.disponibilites_ambulances_relevePresent) {
      sprintf (namebuf, "%s.disponibilites_ambulances_releve", name);
      asn1Print__SeqOfLogMedicalEquipmentAvailability (namebuf, &pvalue->disponibilites_ambulances_releve);
   }

   if (pvalue->m.disponibilites_ambulances_ramassagePresent) {
      sprintf (namebuf, "%s.disponibilites_ambulances_ramassage", name);
      asn1Print__SeqOfLogMedicalEquipmentAvailability (namebuf, &pvalue->disponibilites_ambulances_ramassage);
   }

   if (pvalue->m.disponibilites_medecinsPresent) {
      sprintf (namebuf, "%s.disponibilites_medecins", name);
      asn1Print__SeqOfLogMedicalEquipmentAvailability (namebuf, &pvalue->disponibilites_medecins);
   }

}

void ASN1C_MsgLogMedicalState::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgLogMedicalState (name, &msgData);
}


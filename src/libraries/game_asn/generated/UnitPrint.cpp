/* This file was generated by ASN1C V5.31 on 19-Jun-2007 */

#include "Unit.h"

void asn1Print_UnitIncludes (ASN1ConstCharPtr name, ASN1T_UnitIncludes* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.baseExports", name);

}

void ASN1C_UnitIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_UnitIncludes (name, &msgData);
}

void asn1Print_UnitExports (ASN1ConstCharPtr name)
{
}

void ASN1C_UnitExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_UnitExports (name);
}

void asn1Print_LentEquipment (ASN1ConstCharPtr name, ASN1T_LentEquipment* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_pion_emprunteur", name);
   asn1Print_Unit (namebuf, &pvalue->oid_pion_emprunteur);

   sprintf (namebuf, "%s.type_equipement", name);
   asn1Print_EquipmentType (namebuf, &pvalue->type_equipement);

   sprintf (namebuf, "%s.nombre", name);
   rtPrintInteger (namebuf, pvalue->nombre);

}

void ASN1C_LentEquipment::Print (ASN1ConstCharPtr name)

{
   asn1Print_LentEquipment (name, &msgData);
}

void asn1Print_BorrowedEquipment (ASN1ConstCharPtr name, ASN1T_BorrowedEquipment* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_pion_preteur", name);
   asn1Print_Unit (namebuf, &pvalue->oid_pion_preteur);

   sprintf (namebuf, "%s.type_equipement", name);
   asn1Print_EquipmentType (namebuf, &pvalue->type_equipement);

   sprintf (namebuf, "%s.nombre", name);
   rtPrintInteger (namebuf, pvalue->nombre);

}

void ASN1C_BorrowedEquipment::Print (ASN1ConstCharPtr name)

{
   asn1Print_BorrowedEquipment (name, &msgData);
}

void asn1Print_HumanDotations (ASN1ConstCharPtr name, ASN1T_HumanDotations* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.rang", name);
   asn1Print_EnumHumanRank (namebuf, &pvalue->rang);

   sprintf (namebuf, "%s.nb_total", name);
   rtPrintInteger (namebuf, pvalue->nb_total);

   sprintf (namebuf, "%s.nb_operationnels", name);
   rtPrintInteger (namebuf, pvalue->nb_operationnels);

   sprintf (namebuf, "%s.nb_morts", name);
   rtPrintInteger (namebuf, pvalue->nb_morts);

   sprintf (namebuf, "%s.nb_blesses", name);
   rtPrintInteger (namebuf, pvalue->nb_blesses);

   sprintf (namebuf, "%s.nb_blesses_mentaux", name);
   rtPrintInteger (namebuf, pvalue->nb_blesses_mentaux);

   sprintf (namebuf, "%s.nb_contamines_nbc", name);
   rtPrintInteger (namebuf, pvalue->nb_contamines_nbc);

   sprintf (namebuf, "%s.nb_dans_chaine_sante", name);
   rtPrintInteger (namebuf, pvalue->nb_dans_chaine_sante);

   sprintf (namebuf, "%s.nb_utilises_pour_maintenance", name);
   rtPrintInteger (namebuf, pvalue->nb_utilises_pour_maintenance);

}

void ASN1C_HumanDotations::Print (ASN1ConstCharPtr name)

{
   asn1Print_HumanDotations (name, &msgData);
}

void asn1Print_EquipmentDotations (ASN1ConstCharPtr name, ASN1T_EquipmentDotations* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.type_equipement", name);
   asn1Print_EquipmentType (namebuf, &pvalue->type_equipement);

   sprintf (namebuf, "%s.nb_disponibles", name);
   rtPrintInteger (namebuf, pvalue->nb_disponibles);

   sprintf (namebuf, "%s.nb_indisponibles", name);
   rtPrintInteger (namebuf, pvalue->nb_indisponibles);

   sprintf (namebuf, "%s.nb_reparables", name);
   rtPrintInteger (namebuf, pvalue->nb_reparables);

   sprintf (namebuf, "%s.nb_dans_chaine_maintenance", name);
   rtPrintInteger (namebuf, pvalue->nb_dans_chaine_maintenance);

   sprintf (namebuf, "%s.nb_prisonniers", name);
   rtPrintInteger (namebuf, pvalue->nb_prisonniers);

}

void ASN1C_EquipmentDotations::Print (ASN1ConstCharPtr name)

{
   asn1Print_EquipmentDotations (name, &msgData);
}

void asn1Print_ResourceDotations (ASN1ConstCharPtr name, ASN1T_ResourceDotations* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.ressource_id", name);
   asn1Print_DotationType (namebuf, &pvalue->ressource_id);

   sprintf (namebuf, "%s.quantite_disponible", name);
   rtPrintInteger (namebuf, pvalue->quantite_disponible);

}

void ASN1C_ResourceDotations::Print (ASN1ConstCharPtr name)

{
   asn1Print_ResourceDotations (name, &msgData);
}

void asn1Print_EnumUnitTiredness (ASN1ConstCharPtr name, ASN1T_EnumUnitTiredness* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("normal\n"); break;
      case 1: printf ("fatigue\n"); break;
      case 2: printf ("epuise\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumUnitTiredness::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumUnitTiredness (name, &msgData);
}

void asn1Print_EnumUnitMorale (ASN1ConstCharPtr name, ASN1T_EnumUnitMorale* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("fanatique\n"); break;
      case 1: printf ("bon\n"); break;
      case 2: printf ("moyen\n"); break;
      case 3: printf ("mauvais\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumUnitMorale::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumUnitMorale (name, &msgData);
}

void asn1Print_EnumUnitExperience (ASN1ConstCharPtr name, ASN1T_EnumUnitExperience* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("veteran\n"); break;
      case 1: printf ("experimente\n"); break;
      case 2: printf ("conscrit\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumUnitExperience::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumUnitExperience (name, &msgData);
}

void asn1Print_EnumUnitPosture (ASN1ConstCharPtr name, ASN1T_EnumUnitPosture* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("posture_mouvement\n"); break;
      case 1: printf ("posture_mouvement_discret\n"); break;
      case 2: printf ("posture_arret\n"); break;
      case 3: printf ("posture_poste_reflexe\n"); break;
      case 4: printf ("posture_poste\n"); break;
      case 5: printf ("posture_poste_amenage\n"); break;
      case 6: printf ("posture_poste_prepare_genie\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumUnitPosture::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumUnitPosture (name, &msgData);
}

void asn1Print_EnumForceRatioStatus (ASN1ConstCharPtr name, ASN1T_EnumForceRatioStatus* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("none\n"); break;
      case 1: printf ("neutre\n"); break;
      case 2: printf ("favorable\n"); break;
      case 3: printf ("defavorable\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumForceRatioStatus::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumForceRatioStatus (name, &msgData);
}

void asn1Print_EnumRoe (ASN1ConstCharPtr name, ASN1T_EnumRoe* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("none\n"); break;
      case 1: printf ("tir_libre\n"); break;
      case 2: printf ("tir_sur_riposte\n"); break;
      case 3: printf ("tir_interdit\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumRoe::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumRoe (name, &msgData);
}

void asn1Print_EnumPopulationRoe (ASN1ConstCharPtr name, ASN1T_EnumPopulationRoe* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("none\n"); break;
      case 1: printf ("emploi_force_interdit\n"); break;
      case 2: printf ("maintien_a_distance_par_moyens_non_letaux\n"); break;
      case 3: printf ("dispersion_par_moyens_de_defense_actifs\n"); break;
      case 4: printf ("armes_letales_autorisees\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumPopulationRoe::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumPopulationRoe (name, &msgData);
}

void asn1Print_EnumMeetingEngagementStatus (ASN1ConstCharPtr name, ASN1T_EnumMeetingEngagementStatus* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("none\n"); break;
      case 1: printf ("etat_esquive\n"); break;
      case 2: printf ("etat_fixe\n"); break;
      case 3: printf ("etat_poursuite_mission\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumMeetingEngagementStatus::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumMeetingEngagementStatus (name, &msgData);
}

void asn1Print_EnumOperationalStatus (ASN1ConstCharPtr name, ASN1T_EnumOperationalStatus* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("detruit_totalement\n"); break;
      case 1: printf ("detruit_tactiquement\n"); break;
      case 2: printf ("operationnel\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumOperationalStatus::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumOperationalStatus (name, &msgData);
}

void asn1Print_EnumFireAvailability (ASN1ConstCharPtr name, ASN1T_EnumFireAvailability* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("none\n"); break;
      case 1: printf ("pret_au_tir\n"); break;
      case 2: printf ("indisponible\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumFireAvailability::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumFireAvailability (name, &msgData);
}

void asn1Print_MsgUnitCreation (ASN1ConstCharPtr name, ASN1T_MsgUnitCreation* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_pion", name);
   asn1Print_Unit (namebuf, &pvalue->oid_pion);

   sprintf (namebuf, "%s.type_pion", name);
   asn1Print_UnitType (namebuf, &pvalue->type_pion);

   sprintf (namebuf, "%s.nom", name);
   rtPrintCharStr (namebuf, pvalue->nom);

   sprintf (namebuf, "%s.oid_automate", name);
   asn1Print_Automat (namebuf, &pvalue->oid_automate);

   sprintf (namebuf, "%s.pc", name);
   rtPrintBoolean (namebuf, pvalue->pc);

}

void ASN1C_MsgUnitCreation::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgUnitCreation (name, &msgData);
}

void asn1Print__SeqOfHumanDotations (ASN1ConstCharPtr name, ASN1T__SeqOfHumanDotations* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_HumanDotations (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C__SeqOfHumanDotations::Print (ASN1ConstCharPtr name)

{
   asn1Print__SeqOfHumanDotations (name, &msgData);
}

void asn1Print__SeqOfEquipmentDotations (ASN1ConstCharPtr name, ASN1T__SeqOfEquipmentDotations* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_EquipmentDotations (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C__SeqOfEquipmentDotations::Print (ASN1ConstCharPtr name)

{
   asn1Print__SeqOfEquipmentDotations (name, &msgData);
}

void asn1Print__SeqOfResourceDotations (ASN1ConstCharPtr name, ASN1T__SeqOfResourceDotations* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_ResourceDotations (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C__SeqOfResourceDotations::Print (ASN1ConstCharPtr name)

{
   asn1Print__SeqOfResourceDotations (name, &msgData);
}

void asn1Print__SeqOfLentEquipment (ASN1ConstCharPtr name, ASN1T__SeqOfLentEquipment* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_LentEquipment (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C__SeqOfLentEquipment::Print (ASN1ConstCharPtr name)

{
   asn1Print__SeqOfLentEquipment (name, &msgData);
}

void asn1Print__SeqOfBorrowedEquipment (ASN1ConstCharPtr name, ASN1T__SeqOfBorrowedEquipment* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_BorrowedEquipment (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C__SeqOfBorrowedEquipment::Print (ASN1ConstCharPtr name)

{
   asn1Print__SeqOfBorrowedEquipment (name, &msgData);
}

void asn1Print_MsgUnitAttributes (ASN1ConstCharPtr name, ASN1T_MsgUnitAttributes* pvalue)
{
   char namebuf[512];

   printf ("%s.m.dotation_eff_personnelPresent = %d\n", name, (int)pvalue->m.dotation_eff_personnelPresent);
   printf ("%s.m.dotation_eff_materielPresent = %d\n", name, (int)pvalue->m.dotation_eff_materielPresent);
   printf ("%s.m.dotation_eff_ressourcePresent = %d\n", name, (int)pvalue->m.dotation_eff_ressourcePresent);
   printf ("%s.m.equipements_pretesPresent = %d\n", name, (int)pvalue->m.equipements_pretesPresent);
   printf ("%s.m.equipements_empruntesPresent = %d\n", name, (int)pvalue->m.equipements_empruntesPresent);
   printf ("%s.m.positionPresent = %d\n", name, (int)pvalue->m.positionPresent);
   printf ("%s.m.directionPresent = %d\n", name, (int)pvalue->m.directionPresent);
   printf ("%s.m.hauteurPresent = %d\n", name, (int)pvalue->m.hauteurPresent);
   printf ("%s.m.vitessePresent = %d\n", name, (int)pvalue->m.vitessePresent);
   printf ("%s.m.etat_operationnel_brutPresent = %d\n", name, (int)pvalue->m.etat_operationnel_brutPresent);
   printf ("%s.m.pions_renforcantPresent = %d\n", name, (int)pvalue->m.pions_renforcantPresent);
   printf ("%s.m.pion_renforcePresent = %d\n", name, (int)pvalue->m.pion_renforcePresent);
   printf ("%s.m.mortPresent = %d\n", name, (int)pvalue->m.mortPresent);
   printf ("%s.m.neutralisePresent = %d\n", name, (int)pvalue->m.neutralisePresent);
   printf ("%s.m.mode_furtif_actifPresent = %d\n", name, (int)pvalue->m.mode_furtif_actifPresent);
   printf ("%s.m.embarquePresent = %d\n", name, (int)pvalue->m.embarquePresent);
   printf ("%s.m.transporteurs_disponiblesPresent = %d\n", name, (int)pvalue->m.transporteurs_disponiblesPresent);
   printf ("%s.m.posture_oldPresent = %d\n", name, (int)pvalue->m.posture_oldPresent);
   printf ("%s.m.posture_newPresent = %d\n", name, (int)pvalue->m.posture_newPresent);
   printf ("%s.m.posture_pourcentagePresent = %d\n", name, (int)pvalue->m.posture_pourcentagePresent);
   printf ("%s.m.etat_installationPresent = %d\n", name, (int)pvalue->m.etat_installationPresent);
   printf ("%s.m.en_tenue_de_protection_nbcPresent = %d\n", name, (int)pvalue->m.en_tenue_de_protection_nbcPresent);
   printf ("%s.m.contamine_par_agents_nbcPresent = %d\n", name, (int)pvalue->m.contamine_par_agents_nbcPresent);
   printf ("%s.m.etat_contaminationPresent = %d\n", name, (int)pvalue->m.etat_contaminationPresent);
   printf ("%s.m.communications_brouilleesPresent = %d\n", name, (int)pvalue->m.communications_brouilleesPresent);
   printf ("%s.m.silence_radioPresent = %d\n", name, (int)pvalue->m.silence_radioPresent);
   printf ("%s.m.radar_actifPresent = %d\n", name, (int)pvalue->m.radar_actifPresent);
   printf ("%s.m.pions_transportesPresent = %d\n", name, (int)pvalue->m.pions_transportesPresent);
   printf ("%s.m.pion_transporteurPresent = %d\n", name, (int)pvalue->m.pion_transporteurPresent);
   printf ("%s.m.rapport_de_forcePresent = %d\n", name, (int)pvalue->m.rapport_de_forcePresent);
   printf ("%s.m.combat_de_rencontrePresent = %d\n", name, (int)pvalue->m.combat_de_rencontrePresent);
   printf ("%s.m.etat_operationnelPresent = %d\n", name, (int)pvalue->m.etat_operationnelPresent);
   printf ("%s.m.disponibilite_au_tir_indirectPresent = %d\n", name, (int)pvalue->m.disponibilite_au_tir_indirectPresent);
   printf ("%s.m.roePresent = %d\n", name, (int)pvalue->m.roePresent);
   printf ("%s.m.roe_populationPresent = %d\n", name, (int)pvalue->m.roe_populationPresent);
   printf ("%s.m.fatiguePresent = %d\n", name, (int)pvalue->m.fatiguePresent);
   printf ("%s.m.moralPresent = %d\n", name, (int)pvalue->m.moralPresent);
   printf ("%s.m.experiencePresent = %d\n", name, (int)pvalue->m.experiencePresent);
   printf ("%s.m.renduPresent = %d\n", name, (int)pvalue->m.renduPresent);
   printf ("%s.m.prisonnierPresent = %d\n", name, (int)pvalue->m.prisonnierPresent);
   printf ("%s.m.refugie_pris_en_comptePresent = %d\n", name, (int)pvalue->m.refugie_pris_en_comptePresent);
   sprintf (namebuf, "%s.oid_pion", name);
   asn1Print_Unit (namebuf, &pvalue->oid_pion);

   if (pvalue->m.dotation_eff_personnelPresent) {
      sprintf (namebuf, "%s.dotation_eff_personnel", name);
      asn1Print__SeqOfHumanDotations (namebuf, &pvalue->dotation_eff_personnel);
   }

   if (pvalue->m.dotation_eff_materielPresent) {
      sprintf (namebuf, "%s.dotation_eff_materiel", name);
      asn1Print__SeqOfEquipmentDotations (namebuf, &pvalue->dotation_eff_materiel);
   }

   if (pvalue->m.dotation_eff_ressourcePresent) {
      sprintf (namebuf, "%s.dotation_eff_ressource", name);
      asn1Print__SeqOfResourceDotations (namebuf, &pvalue->dotation_eff_ressource);
   }

   if (pvalue->m.equipements_pretesPresent) {
      sprintf (namebuf, "%s.equipements_pretes", name);
      asn1Print__SeqOfLentEquipment (namebuf, &pvalue->equipements_pretes);
   }

   if (pvalue->m.equipements_empruntesPresent) {
      sprintf (namebuf, "%s.equipements_empruntes", name);
      asn1Print__SeqOfBorrowedEquipment (namebuf, &pvalue->equipements_empruntes);
   }

   if (pvalue->m.positionPresent) {
      sprintf (namebuf, "%s.position", name);
      asn1Print_CoordUTM (namebuf, &pvalue->position);
   }

   if (pvalue->m.directionPresent) {
      sprintf (namebuf, "%s.direction", name);
      asn1Print_Heading (namebuf, &pvalue->direction);
   }

   if (pvalue->m.hauteurPresent) {
      sprintf (namebuf, "%s.hauteur", name);
      asn1Print_Height (namebuf, &pvalue->hauteur);
   }

   if (pvalue->m.vitessePresent) {
      sprintf (namebuf, "%s.vitesse", name);
      asn1Print_Speed (namebuf, &pvalue->vitesse);
   }

   if (pvalue->m.etat_operationnel_brutPresent) {
      sprintf (namebuf, "%s.etat_operationnel_brut", name);
      asn1Print_FreePercentage (namebuf, &pvalue->etat_operationnel_brut);
   }

   if (pvalue->m.pions_renforcantPresent) {
      sprintf (namebuf, "%s.pions_renforcant", name);
      asn1Print_UnitList (namebuf, &pvalue->pions_renforcant);
   }

   if (pvalue->m.pion_renforcePresent) {
      sprintf (namebuf, "%s.pion_renforce", name);
      asn1Print_Unit (namebuf, &pvalue->pion_renforce);
   }

   if (pvalue->m.mortPresent) {
      sprintf (namebuf, "%s.mort", name);
      rtPrintBoolean (namebuf, pvalue->mort);
   }

   if (pvalue->m.neutralisePresent) {
      sprintf (namebuf, "%s.neutralise", name);
      rtPrintBoolean (namebuf, pvalue->neutralise);
   }

   if (pvalue->m.mode_furtif_actifPresent) {
      sprintf (namebuf, "%s.mode_furtif_actif", name);
      rtPrintBoolean (namebuf, pvalue->mode_furtif_actif);
   }

   if (pvalue->m.embarquePresent) {
      sprintf (namebuf, "%s.embarque", name);
      rtPrintBoolean (namebuf, pvalue->embarque);
   }

   if (pvalue->m.transporteurs_disponiblesPresent) {
      sprintf (namebuf, "%s.transporteurs_disponibles", name);
      rtPrintBoolean (namebuf, pvalue->transporteurs_disponibles);
   }

   if (pvalue->m.posture_oldPresent) {
      sprintf (namebuf, "%s.posture_old", name);
      asn1Print_EnumUnitPosture (namebuf, &pvalue->posture_old);
   }

   if (pvalue->m.posture_newPresent) {
      sprintf (namebuf, "%s.posture_new", name);
      asn1Print_EnumUnitPosture (namebuf, &pvalue->posture_new);
   }

   if (pvalue->m.posture_pourcentagePresent) {
      sprintf (namebuf, "%s.posture_pourcentage", name);
      asn1Print_Percentage (namebuf, &pvalue->posture_pourcentage);
   }

   if (pvalue->m.etat_installationPresent) {
      sprintf (namebuf, "%s.etat_installation", name);
      asn1Print_Percentage (namebuf, &pvalue->etat_installation);
   }

   if (pvalue->m.en_tenue_de_protection_nbcPresent) {
      sprintf (namebuf, "%s.en_tenue_de_protection_nbc", name);
      rtPrintBoolean (namebuf, pvalue->en_tenue_de_protection_nbc);
   }

   if (pvalue->m.contamine_par_agents_nbcPresent) {
      sprintf (namebuf, "%s.contamine_par_agents_nbc", name);
      asn1Print_ListOID (namebuf, &pvalue->contamine_par_agents_nbc);
   }

   if (pvalue->m.etat_contaminationPresent) {
      sprintf (namebuf, "%s.etat_contamination", name);
      asn1Print_Percentage (namebuf, &pvalue->etat_contamination);
   }

   if (pvalue->m.communications_brouilleesPresent) {
      sprintf (namebuf, "%s.communications_brouillees", name);
      rtPrintBoolean (namebuf, pvalue->communications_brouillees);
   }

   if (pvalue->m.silence_radioPresent) {
      sprintf (namebuf, "%s.silence_radio", name);
      rtPrintBoolean (namebuf, pvalue->silence_radio);
   }

   if (pvalue->m.radar_actifPresent) {
      sprintf (namebuf, "%s.radar_actif", name);
      rtPrintBoolean (namebuf, pvalue->radar_actif);
   }

   if (pvalue->m.pions_transportesPresent) {
      sprintf (namebuf, "%s.pions_transportes", name);
      asn1Print_UnitList (namebuf, &pvalue->pions_transportes);
   }

   if (pvalue->m.pion_transporteurPresent) {
      sprintf (namebuf, "%s.pion_transporteur", name);
      asn1Print_Unit (namebuf, &pvalue->pion_transporteur);
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

   if (pvalue->m.disponibilite_au_tir_indirectPresent) {
      sprintf (namebuf, "%s.disponibilite_au_tir_indirect", name);
      asn1Print_EnumFireAvailability (namebuf, &pvalue->disponibilite_au_tir_indirect);
   }

   if (pvalue->m.roePresent) {
      sprintf (namebuf, "%s.roe", name);
      asn1Print_EnumRoe (namebuf, &pvalue->roe);
   }

   if (pvalue->m.roe_populationPresent) {
      sprintf (namebuf, "%s.roe_population", name);
      asn1Print_EnumPopulationRoe (namebuf, &pvalue->roe_population);
   }

   if (pvalue->m.fatiguePresent) {
      sprintf (namebuf, "%s.fatigue", name);
      asn1Print_EnumUnitTiredness (namebuf, &pvalue->fatigue);
   }

   if (pvalue->m.moralPresent) {
      sprintf (namebuf, "%s.moral", name);
      asn1Print_EnumUnitMorale (namebuf, &pvalue->moral);
   }

   if (pvalue->m.experiencePresent) {
      sprintf (namebuf, "%s.experience", name);
      asn1Print_EnumUnitExperience (namebuf, &pvalue->experience);
   }

   if (pvalue->m.renduPresent) {
      sprintf (namebuf, "%s.rendu", name);
      asn1Print_Team (namebuf, &pvalue->rendu);
   }

   if (pvalue->m.prisonnierPresent) {
      sprintf (namebuf, "%s.prisonnier", name);
      rtPrintBoolean (namebuf, pvalue->prisonnier);
   }

   if (pvalue->m.refugie_pris_en_comptePresent) {
      sprintf (namebuf, "%s.refugie_pris_en_compte", name);
      rtPrintBoolean (namebuf, pvalue->refugie_pris_en_compte);
   }

}

void ASN1C_MsgUnitAttributes::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgUnitAttributes (name, &msgData);
}

void asn1Print_MsgUnitDestruction (ASN1ConstCharPtr name, ASN1T_MsgUnitDestruction* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_MsgUnitDestruction::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgUnitDestruction (name, &msgData);
}

void asn1Print_MagicActionRecoverTransporters (ASN1ConstCharPtr name)
{
}

void ASN1C_MagicActionRecoverTransporters::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionRecoverTransporters (name);
}

void asn1Print_MagicActionSurrender (ASN1ConstCharPtr name, ASN1T_MagicActionSurrender* pvalue)
{
   asn1Print_Team (name, pvalue);
}

void ASN1C_MagicActionSurrender::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionSurrender (name, &msgData);
}

void asn1Print_MagicActionCancelRendition (ASN1ConstCharPtr name)
{
}

void ASN1C_MagicActionCancelRendition::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionCancelRendition (name);
}

void asn1Print_MagicActionMoveTo (ASN1ConstCharPtr name, ASN1T_MagicActionMoveTo* pvalue)
{
   asn1Print_CoordUTM (name, pvalue);
}

void ASN1C_MagicActionMoveTo::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionMoveTo (name, &msgData);
}

void asn1Print_MagicActionTotalDestruction (ASN1ConstCharPtr name)
{
}

void ASN1C_MagicActionTotalDestruction::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionTotalDestruction (name);
}

void asn1Print_MagicActionComponentDestruction (ASN1ConstCharPtr name)
{
}

void ASN1C_MagicActionComponentDestruction::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionComponentDestruction (name);
}

void asn1Print_MagicActionTotalRecovery (ASN1ConstCharPtr name)
{
}

void ASN1C_MagicActionTotalRecovery::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionTotalRecovery (name);
}

void asn1Print_MagicActionHumanRecovery (ASN1ConstCharPtr name)
{
}

void ASN1C_MagicActionHumanRecovery::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionHumanRecovery (name);
}

void asn1Print_MagicActionEquipmentRecovery (ASN1ConstCharPtr name)
{
}

void ASN1C_MagicActionEquipmentRecovery::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionEquipmentRecovery (name);
}

void asn1Print_MagicActionRessourcesRecovery (ASN1ConstCharPtr name)
{
}

void ASN1C_MagicActionRessourcesRecovery::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionRessourcesRecovery (name);
}

void asn1Print_MagicActionChangeHumanFactors (ASN1ConstCharPtr name, ASN1T_MagicActionChangeHumanFactors* pvalue)
{
   char namebuf[512];

   printf ("%s.m.fatiguePresent = %d\n", name, (int)pvalue->m.fatiguePresent);
   printf ("%s.m.moralPresent = %d\n", name, (int)pvalue->m.moralPresent);
   printf ("%s.m.experiencePresent = %d\n", name, (int)pvalue->m.experiencePresent);
   if (pvalue->m.fatiguePresent) {
      sprintf (namebuf, "%s.fatigue", name);
      asn1Print_EnumUnitTiredness (namebuf, &pvalue->fatigue);
   }

   if (pvalue->m.moralPresent) {
      sprintf (namebuf, "%s.moral", name);
      asn1Print_EnumUnitMorale (namebuf, &pvalue->moral);
   }

   if (pvalue->m.experiencePresent) {
      sprintf (namebuf, "%s.experience", name);
      asn1Print_EnumUnitExperience (namebuf, &pvalue->experience);
   }

}

void ASN1C_MagicActionChangeHumanFactors::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionChangeHumanFactors (name, &msgData);
}

void asn1Print_DotationRecovery (ASN1ConstCharPtr name, ASN1T_DotationRecovery* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.famille_dotation", name);
   asn1Print_EnumDotationFamily (namebuf, &pvalue->famille_dotation);

   sprintf (namebuf, "%s.pourcentage", name);
   asn1Print_Percentage (namebuf, &pvalue->pourcentage);

}

void ASN1C_DotationRecovery::Print (ASN1ConstCharPtr name)

{
   asn1Print_DotationRecovery (name, &msgData);
}

void asn1Print_AmmunitionDotationRecovery (ASN1ConstCharPtr name, ASN1T_AmmunitionDotationRecovery* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.famille_munition", name);
   asn1Print_EnumAmmunitionFamily (namebuf, &pvalue->famille_munition);

   sprintf (namebuf, "%s.pourcentage", name);
   asn1Print_Percentage (namebuf, &pvalue->pourcentage);

}

void ASN1C_AmmunitionDotationRecovery::Print (ASN1ConstCharPtr name)

{
   asn1Print_AmmunitionDotationRecovery (name, &msgData);
}

void asn1Print_EquipmentRecovery (ASN1ConstCharPtr name, ASN1T_EquipmentRecovery* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.type_equipement", name);
   asn1Print_EquipmentType (namebuf, &pvalue->type_equipement);

   sprintf (namebuf, "%s.nombre_disponible", name);
   rtPrintInteger (namebuf, pvalue->nombre_disponible);

}

void ASN1C_EquipmentRecovery::Print (ASN1ConstCharPtr name)

{
   asn1Print_EquipmentRecovery (name, &msgData);
}

void asn1Print_HumanRecovery (ASN1ConstCharPtr name, ASN1T_HumanRecovery* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.rang", name);
   asn1Print_EnumHumanRank (namebuf, &pvalue->rang);

   sprintf (namebuf, "%s.nombre_disponible", name);
   rtPrintInteger (namebuf, pvalue->nombre_disponible);

}

void ASN1C_HumanRecovery::Print (ASN1ConstCharPtr name)

{
   asn1Print_HumanRecovery (name, &msgData);
}

void asn1Print_StockRecovery (ASN1ConstCharPtr name, ASN1T_StockRecovery* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.ressource_id", name);
   asn1Print_DotationType (namebuf, &pvalue->ressource_id);

   sprintf (namebuf, "%s.quantite_disponible", name);
   rtPrintInteger (namebuf, pvalue->quantite_disponible);

}

void ASN1C_StockRecovery::Print (ASN1ConstCharPtr name)

{
   asn1Print_StockRecovery (name, &msgData);
}

void asn1Print__SeqOfEquipmentRecovery (ASN1ConstCharPtr name, ASN1T__SeqOfEquipmentRecovery* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_EquipmentRecovery (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C__SeqOfEquipmentRecovery::Print (ASN1ConstCharPtr name)

{
   asn1Print__SeqOfEquipmentRecovery (name, &msgData);
}

void asn1Print__SeqOfHumanRecovery (ASN1ConstCharPtr name, ASN1T__SeqOfHumanRecovery* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_HumanRecovery (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C__SeqOfHumanRecovery::Print (ASN1ConstCharPtr name)

{
   asn1Print__SeqOfHumanRecovery (name, &msgData);
}

void asn1Print__SeqOfDotationRecovery (ASN1ConstCharPtr name, ASN1T__SeqOfDotationRecovery* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_DotationRecovery (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C__SeqOfDotationRecovery::Print (ASN1ConstCharPtr name)

{
   asn1Print__SeqOfDotationRecovery (name, &msgData);
}

void asn1Print__SeqOfAmmunitionDotationRecovery (ASN1ConstCharPtr name, ASN1T__SeqOfAmmunitionDotationRecovery* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_AmmunitionDotationRecovery (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C__SeqOfAmmunitionDotationRecovery::Print (ASN1ConstCharPtr name)

{
   asn1Print__SeqOfAmmunitionDotationRecovery (name, &msgData);
}

void asn1Print__SeqOfStockRecovery (ASN1ConstCharPtr name, ASN1T__SeqOfStockRecovery* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_StockRecovery (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C__SeqOfStockRecovery::Print (ASN1ConstCharPtr name)

{
   asn1Print__SeqOfStockRecovery (name, &msgData);
}

void asn1Print_MagicActionPartialRecovery (ASN1ConstCharPtr name, ASN1T_MagicActionPartialRecovery* pvalue)
{
   char namebuf[512];

   printf ("%s.m.equipementsPresent = %d\n", name, (int)pvalue->m.equipementsPresent);
   printf ("%s.m.personnelsPresent = %d\n", name, (int)pvalue->m.personnelsPresent);
   printf ("%s.m.dotationsPresent = %d\n", name, (int)pvalue->m.dotationsPresent);
   printf ("%s.m.munitionsPresent = %d\n", name, (int)pvalue->m.munitionsPresent);
   printf ("%s.m.stocksPresent = %d\n", name, (int)pvalue->m.stocksPresent);
   if (pvalue->m.equipementsPresent) {
      sprintf (namebuf, "%s.equipements", name);
      asn1Print__SeqOfEquipmentRecovery (namebuf, &pvalue->equipements);
   }

   if (pvalue->m.personnelsPresent) {
      sprintf (namebuf, "%s.personnels", name);
      asn1Print__SeqOfHumanRecovery (namebuf, &pvalue->personnels);
   }

   if (pvalue->m.dotationsPresent) {
      sprintf (namebuf, "%s.dotations", name);
      asn1Print__SeqOfDotationRecovery (namebuf, &pvalue->dotations);
   }

   if (pvalue->m.munitionsPresent) {
      sprintf (namebuf, "%s.munitions", name);
      asn1Print__SeqOfAmmunitionDotationRecovery (namebuf, &pvalue->munitions);
   }

   if (pvalue->m.stocksPresent) {
      sprintf (namebuf, "%s.stocks", name);
      asn1Print__SeqOfStockRecovery (namebuf, &pvalue->stocks);
   }

}

void ASN1C_MagicActionPartialRecovery::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionPartialRecovery (name, &msgData);
}

void asn1Print_MsgUnitMagicAction_action (ASN1ConstCharPtr name, ASN1T_MsgUnitMagicAction_action* pvalue)
{
   char namebuf[512];

   printf ("%s.t = %d\n", name, pvalue->t);
   switch (pvalue->t) {
      case 1:
         sprintf (namebuf, "%s.u.move_to", name);
         asn1Print_MagicActionMoveTo (namebuf, pvalue->u.move_to);
         break;

      case 2:
         sprintf (namebuf, "%s.u.recompletement_total", name);
         break;

      case 3:
         sprintf (namebuf, "%s.u.recompletement_partiel", name);
         asn1Print_MagicActionPartialRecovery (namebuf, pvalue->u.recompletement_partiel);
         break;

      case 4:
         sprintf (namebuf, "%s.u.recompletement_personnel", name);
         break;

      case 5:
         sprintf (namebuf, "%s.u.recompletement_equipement", name);
         break;

      case 6:
         sprintf (namebuf, "%s.u.recompletement_ressources", name);
         break;

      case 7:
         sprintf (namebuf, "%s.u.change_facteurs_humains", name);
         asn1Print_MagicActionChangeHumanFactors (namebuf, pvalue->u.change_facteurs_humains);
         break;

      case 8:
         sprintf (namebuf, "%s.u.destruction_totale", name);
         break;

      case 9:
         sprintf (namebuf, "%s.u.destruction_composante", name);
         break;

      case 10:
         sprintf (namebuf, "%s.u.se_rendre", name);
         asn1Print_MagicActionSurrender (namebuf, &pvalue->u.se_rendre);
         break;

      case 11:
         sprintf (namebuf, "%s.u.annuler_reddition", name);
         break;

      case 12:
         sprintf (namebuf, "%s.u.recuperer_transporteurs", name);
         break;

      default:;
   }
}

void ASN1C_MsgUnitMagicAction_action::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgUnitMagicAction_action (name, &msgData);
}

void asn1Print_MsgUnitMagicAction (ASN1ConstCharPtr name, ASN1T_MsgUnitMagicAction* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.action", name);
   asn1Print_MsgUnitMagicAction_action (namebuf, &pvalue->action);

}

void ASN1C_MsgUnitMagicAction::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgUnitMagicAction (name, &msgData);
}

void asn1Print_EnumUnitErrorCode (ASN1ConstCharPtr name, ASN1T_EnumUnitErrorCode* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("no_error\n"); break;
      case 1: printf ("error_invalid_unit\n"); break;
      case 2: printf ("error_automate_embraye\n"); break;
      case 3: printf ("error_invalid_attribute\n"); break;
      case 4: printf ("error_unit_surrendered\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumUnitErrorCode::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumUnitErrorCode (name, &msgData);
}

void asn1Print_MsgUnitMagicActionAck (ASN1ConstCharPtr name, ASN1T_MsgUnitMagicActionAck* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.error_code", name);
   asn1Print_EnumUnitErrorCode (namebuf, &pvalue->error_code);

}

void ASN1C_MsgUnitMagicActionAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgUnitMagicActionAck (name, &msgData);
}

void asn1Print_MsgUnitCreationRequest (ASN1ConstCharPtr name, ASN1T_MsgUnitCreationRequest* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.type_pion", name);
   asn1Print_UnitType (namebuf, &pvalue->type_pion);

   sprintf (namebuf, "%s.oid_automate", name);
   asn1Print_Automat (namebuf, &pvalue->oid_automate);

   sprintf (namebuf, "%s.position", name);
   asn1Print_CoordUTM (namebuf, &pvalue->position);

}

void ASN1C_MsgUnitCreationRequest::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgUnitCreationRequest (name, &msgData);
}

void asn1Print_MsgUnitCreationRequestAck (ASN1ConstCharPtr name, ASN1T_MsgUnitCreationRequestAck* pvalue)
{
   asn1Print_EnumUnitErrorCode (name, pvalue);
}

void ASN1C_MsgUnitCreationRequestAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgUnitCreationRequestAck (name, &msgData);
}

void asn1Print_MsgUnitPathFind (ASN1ConstCharPtr name, ASN1T_MsgUnitPathFind* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_pion", name);
   asn1Print_OID (namebuf, &pvalue->oid_pion);

   sprintf (namebuf, "%s.itineraire", name);
   asn1Print_Path (namebuf, &pvalue->itineraire);

}

void ASN1C_MsgUnitPathFind::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgUnitPathFind (name, &msgData);
}

void asn1Print_MsgUnitChangeSuperiorAck (ASN1ConstCharPtr name, ASN1T_MsgUnitChangeSuperiorAck* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_pion", name);
   asn1Print_Unit (namebuf, &pvalue->oid_pion);

   sprintf (namebuf, "%s.oid_automate", name);
   asn1Print_Automat (namebuf, &pvalue->oid_automate);

   sprintf (namebuf, "%s.error_code", name);
   asn1Print_EnumChangeHierarchyErrorCode (namebuf, &pvalue->error_code);

}

void ASN1C_MsgUnitChangeSuperiorAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgUnitChangeSuperiorAck (name, &msgData);
}

void asn1Print_MsgUnitChangeSuperior (ASN1ConstCharPtr name, ASN1T_MsgUnitChangeSuperior* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_pion", name);
   asn1Print_Unit (namebuf, &pvalue->oid_pion);

   sprintf (namebuf, "%s.oid_automate", name);
   asn1Print_Automat (namebuf, &pvalue->oid_automate);

}

void ASN1C_MsgUnitChangeSuperior::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgUnitChangeSuperior (name, &msgData);
}


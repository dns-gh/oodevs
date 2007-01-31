/* This file was generated by ASN1C V5.31 on 30-Jan-2007 */

#include "ScipioUnit.h"

void asn1Print_ScipioUnitIncludes (ASN1ConstCharPtr name, ASN1T_ScipioUnitIncludes* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.scipioBaseExports", name);

}

void ASN1C_ScipioUnitIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_ScipioUnitIncludes (name, &msgData);
}

void asn1Print_ScipioUnitExports (ASN1ConstCharPtr name)
{
}

void ASN1C_ScipioUnitExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_ScipioUnitExports (name);
}

void asn1Print_EquipementPrete (ASN1ConstCharPtr name, ASN1T_EquipementPrete* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_pion_emprunteur", name);
   asn1Print_Agent (namebuf, &pvalue->oid_pion_emprunteur);

   sprintf (namebuf, "%s.type_equipement", name);
   asn1Print_TypeEquipement (namebuf, &pvalue->type_equipement);

   sprintf (namebuf, "%s.nombre", name);
   rtPrintInteger (namebuf, pvalue->nombre);

}

void ASN1C_EquipementPrete::Print (ASN1ConstCharPtr name)

{
   asn1Print_EquipementPrete (name, &msgData);
}

void asn1Print_EquipementEmprunte (ASN1ConstCharPtr name, ASN1T_EquipementEmprunte* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_pion_preteur", name);
   asn1Print_Agent (namebuf, &pvalue->oid_pion_preteur);

   sprintf (namebuf, "%s.type_equipement", name);
   asn1Print_TypeEquipement (namebuf, &pvalue->type_equipement);

   sprintf (namebuf, "%s.nombre", name);
   rtPrintInteger (namebuf, pvalue->nombre);

}

void ASN1C_EquipementEmprunte::Print (ASN1ConstCharPtr name)

{
   asn1Print_EquipementEmprunte (name, &msgData);
}

void asn1Print_DotationPersonnel (ASN1ConstCharPtr name, ASN1T_DotationPersonnel* pvalue)
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

void ASN1C_DotationPersonnel::Print (ASN1ConstCharPtr name)

{
   asn1Print_DotationPersonnel (name, &msgData);
}

void asn1Print_DotationEquipement (ASN1ConstCharPtr name, ASN1T_DotationEquipement* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.type_equipement", name);
   asn1Print_TypeEquipement (namebuf, &pvalue->type_equipement);

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

void ASN1C_DotationEquipement::Print (ASN1ConstCharPtr name)

{
   asn1Print_DotationEquipement (name, &msgData);
}

void asn1Print_DotationRessource (ASN1ConstCharPtr name, ASN1T_DotationRessource* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.ressource_id", name);
   asn1Print_TypeDotation (namebuf, &pvalue->ressource_id);

   sprintf (namebuf, "%s.quantite_disponible", name);
   rtPrintInteger (namebuf, pvalue->quantite_disponible);

}

void ASN1C_DotationRessource::Print (ASN1ConstCharPtr name)

{
   asn1Print_DotationRessource (name, &msgData);
}

void asn1Print_EnumUnitFatigue (ASN1ConstCharPtr name, ASN1T_EnumUnitFatigue* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("normal\n"); break;
      case 1: printf ("fatigue\n"); break;
      case 2: printf ("epuise\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumUnitFatigue::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumUnitFatigue (name, &msgData);
}

void asn1Print_EnumUnitMoral (ASN1ConstCharPtr name, ASN1T_EnumUnitMoral* pvalue)
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

void ASN1C_EnumUnitMoral::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumUnitMoral (name, &msgData);
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

void asn1Print_EnumEtatRapFor (ASN1ConstCharPtr name, ASN1T_EnumEtatRapFor* pvalue)
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

void ASN1C_EnumEtatRapFor::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumEtatRapFor (name, &msgData);
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

void asn1Print_EnumRoePopulation (ASN1ConstCharPtr name, ASN1T_EnumRoePopulation* pvalue)
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

void ASN1C_EnumRoePopulation::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumRoePopulation (name, &msgData);
}

void asn1Print_EnumEtatCombatRencontre (ASN1ConstCharPtr name, ASN1T_EnumEtatCombatRencontre* pvalue)
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

void ASN1C_EnumEtatCombatRencontre::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumEtatCombatRencontre (name, &msgData);
}

void asn1Print_EnumEtatOperationnel (ASN1ConstCharPtr name, ASN1T_EnumEtatOperationnel* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("detruit_totalement\n"); break;
      case 1: printf ("detruit_tactiquement\n"); break;
      case 2: printf ("operationnel\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumEtatOperationnel::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumEtatOperationnel (name, &msgData);
}

void asn1Print_EnumDisponibiliteAuTir (ASN1ConstCharPtr name, ASN1T_EnumDisponibiliteAuTir* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("none\n"); break;
      case 1: printf ("pret_au_tir\n"); break;
      case 2: printf ("indisponible\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumDisponibiliteAuTir::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumDisponibiliteAuTir (name, &msgData);
}

void asn1Print_MsgPionCreation (ASN1ConstCharPtr name, ASN1T_MsgPionCreation* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_pion", name);
   asn1Print_Agent (namebuf, &pvalue->oid_pion);

   sprintf (namebuf, "%s.type_pion", name);
   asn1Print_TypePion (namebuf, &pvalue->type_pion);

   sprintf (namebuf, "%s.nom", name);
   rtPrintCharStr (namebuf, pvalue->nom);

   sprintf (namebuf, "%s.oid_automate", name);
   asn1Print_Automate (namebuf, &pvalue->oid_automate);

   sprintf (namebuf, "%s.pc", name);
   rtPrintBoolean (namebuf, pvalue->pc);

}

void ASN1C_MsgPionCreation::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgPionCreation (name, &msgData);
}

void asn1Print__SeqOfDotationPersonnel (ASN1ConstCharPtr name, ASN1T__SeqOfDotationPersonnel* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_DotationPersonnel (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C__SeqOfDotationPersonnel::Print (ASN1ConstCharPtr name)

{
   asn1Print__SeqOfDotationPersonnel (name, &msgData);
}

void asn1Print__SeqOfDotationEquipement (ASN1ConstCharPtr name, ASN1T__SeqOfDotationEquipement* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_DotationEquipement (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C__SeqOfDotationEquipement::Print (ASN1ConstCharPtr name)

{
   asn1Print__SeqOfDotationEquipement (name, &msgData);
}

void asn1Print__SeqOfDotationRessource (ASN1ConstCharPtr name, ASN1T__SeqOfDotationRessource* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_DotationRessource (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C__SeqOfDotationRessource::Print (ASN1ConstCharPtr name)

{
   asn1Print__SeqOfDotationRessource (name, &msgData);
}

void asn1Print__SeqOfEquipementPrete (ASN1ConstCharPtr name, ASN1T__SeqOfEquipementPrete* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_EquipementPrete (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C__SeqOfEquipementPrete::Print (ASN1ConstCharPtr name)

{
   asn1Print__SeqOfEquipementPrete (name, &msgData);
}

void asn1Print__SeqOfEquipementEmprunte (ASN1ConstCharPtr name, ASN1T__SeqOfEquipementEmprunte* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_EquipementEmprunte (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C__SeqOfEquipementEmprunte::Print (ASN1ConstCharPtr name)

{
   asn1Print__SeqOfEquipementEmprunte (name, &msgData);
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
   asn1Print_Agent (namebuf, &pvalue->oid_pion);

   if (pvalue->m.dotation_eff_personnelPresent) {
      sprintf (namebuf, "%s.dotation_eff_personnel", name);
      asn1Print__SeqOfDotationPersonnel (namebuf, &pvalue->dotation_eff_personnel);
   }

   if (pvalue->m.dotation_eff_materielPresent) {
      sprintf (namebuf, "%s.dotation_eff_materiel", name);
      asn1Print__SeqOfDotationEquipement (namebuf, &pvalue->dotation_eff_materiel);
   }

   if (pvalue->m.dotation_eff_ressourcePresent) {
      sprintf (namebuf, "%s.dotation_eff_ressource", name);
      asn1Print__SeqOfDotationRessource (namebuf, &pvalue->dotation_eff_ressource);
   }

   if (pvalue->m.equipements_pretesPresent) {
      sprintf (namebuf, "%s.equipements_pretes", name);
      asn1Print__SeqOfEquipementPrete (namebuf, &pvalue->equipements_pretes);
   }

   if (pvalue->m.equipements_empruntesPresent) {
      sprintf (namebuf, "%s.equipements_empruntes", name);
      asn1Print__SeqOfEquipementEmprunte (namebuf, &pvalue->equipements_empruntes);
   }

   if (pvalue->m.positionPresent) {
      sprintf (namebuf, "%s.position", name);
      asn1Print_CoordUTM (namebuf, &pvalue->position);
   }

   if (pvalue->m.directionPresent) {
      sprintf (namebuf, "%s.direction", name);
      asn1Print_Direction (namebuf, &pvalue->direction);
   }

   if (pvalue->m.hauteurPresent) {
      sprintf (namebuf, "%s.hauteur", name);
      asn1Print_Hauteur (namebuf, &pvalue->hauteur);
   }

   if (pvalue->m.vitessePresent) {
      sprintf (namebuf, "%s.vitesse", name);
      asn1Print_Vitesse (namebuf, &pvalue->vitesse);
   }

   if (pvalue->m.etat_operationnel_brutPresent) {
      sprintf (namebuf, "%s.etat_operationnel_brut", name);
      asn1Print_PourcentageLibre (namebuf, &pvalue->etat_operationnel_brut);
   }

   if (pvalue->m.pions_renforcantPresent) {
      sprintf (namebuf, "%s.pions_renforcant", name);
      asn1Print_ListAgent (namebuf, &pvalue->pions_renforcant);
   }

   if (pvalue->m.pion_renforcePresent) {
      sprintf (namebuf, "%s.pion_renforce", name);
      asn1Print_Agent (namebuf, &pvalue->pion_renforce);
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
      asn1Print_Pourcentage (namebuf, &pvalue->posture_pourcentage);
   }

   if (pvalue->m.etat_installationPresent) {
      sprintf (namebuf, "%s.etat_installation", name);
      asn1Print_Pourcentage (namebuf, &pvalue->etat_installation);
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
      asn1Print_Pourcentage (namebuf, &pvalue->etat_contamination);
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
      asn1Print_ListAgent (namebuf, &pvalue->pions_transportes);
   }

   if (pvalue->m.pion_transporteurPresent) {
      sprintf (namebuf, "%s.pion_transporteur", name);
      asn1Print_Agent (namebuf, &pvalue->pion_transporteur);
   }

   if (pvalue->m.rapport_de_forcePresent) {
      sprintf (namebuf, "%s.rapport_de_force", name);
      asn1Print_EnumEtatRapFor (namebuf, &pvalue->rapport_de_force);
   }

   if (pvalue->m.combat_de_rencontrePresent) {
      sprintf (namebuf, "%s.combat_de_rencontre", name);
      asn1Print_EnumEtatCombatRencontre (namebuf, &pvalue->combat_de_rencontre);
   }

   if (pvalue->m.etat_operationnelPresent) {
      sprintf (namebuf, "%s.etat_operationnel", name);
      asn1Print_EnumEtatOperationnel (namebuf, &pvalue->etat_operationnel);
   }

   if (pvalue->m.disponibilite_au_tir_indirectPresent) {
      sprintf (namebuf, "%s.disponibilite_au_tir_indirect", name);
      asn1Print_EnumDisponibiliteAuTir (namebuf, &pvalue->disponibilite_au_tir_indirect);
   }

   if (pvalue->m.roePresent) {
      sprintf (namebuf, "%s.roe", name);
      asn1Print_EnumRoe (namebuf, &pvalue->roe);
   }

   if (pvalue->m.roe_populationPresent) {
      sprintf (namebuf, "%s.roe_population", name);
      asn1Print_EnumRoePopulation (namebuf, &pvalue->roe_population);
   }

   if (pvalue->m.fatiguePresent) {
      sprintf (namebuf, "%s.fatigue", name);
      asn1Print_EnumUnitFatigue (namebuf, &pvalue->fatigue);
   }

   if (pvalue->m.moralPresent) {
      sprintf (namebuf, "%s.moral", name);
      asn1Print_EnumUnitMoral (namebuf, &pvalue->moral);
   }

   if (pvalue->m.experiencePresent) {
      sprintf (namebuf, "%s.experience", name);
      asn1Print_EnumUnitExperience (namebuf, &pvalue->experience);
   }

   if (pvalue->m.renduPresent) {
      sprintf (namebuf, "%s.rendu", name);
      rtPrintBoolean (namebuf, pvalue->rendu);
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

void asn1Print_MagicActionRecupererTransporteurs (ASN1ConstCharPtr name)
{
}

void ASN1C_MagicActionRecupererTransporteurs::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionRecupererTransporteurs (name);
}

void asn1Print_MagicActionSeRendre (ASN1ConstCharPtr name)
{
}

void ASN1C_MagicActionSeRendre::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionSeRendre (name);
}

void asn1Print_MagicActionMoveTo (ASN1ConstCharPtr name, ASN1T_MagicActionMoveTo* pvalue)
{
   asn1Print_CoordUTM (name, pvalue);
}

void ASN1C_MagicActionMoveTo::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionMoveTo (name, &msgData);
}

void asn1Print_MagicActionDestructionTotale (ASN1ConstCharPtr name)
{
}

void ASN1C_MagicActionDestructionTotale::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionDestructionTotale (name);
}

void asn1Print_MagicActionRecompletementTotal (ASN1ConstCharPtr name)
{
}

void ASN1C_MagicActionRecompletementTotal::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionRecompletementTotal (name);
}

void asn1Print_MagicActionRecompletementPersonnel (ASN1ConstCharPtr name)
{
}

void ASN1C_MagicActionRecompletementPersonnel::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionRecompletementPersonnel (name);
}

void asn1Print_MagicActionRecompletementEquipement (ASN1ConstCharPtr name)
{
}

void ASN1C_MagicActionRecompletementEquipement::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionRecompletementEquipement (name);
}

void asn1Print_MagicActionRecompletementRessources (ASN1ConstCharPtr name)
{
}

void ASN1C_MagicActionRecompletementRessources::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionRecompletementRessources (name);
}

void asn1Print_MagicActionChangeFacteursHumains (ASN1ConstCharPtr name, ASN1T_MagicActionChangeFacteursHumains* pvalue)
{
   char namebuf[512];

   printf ("%s.m.fatiguePresent = %d\n", name, (int)pvalue->m.fatiguePresent);
   printf ("%s.m.moralPresent = %d\n", name, (int)pvalue->m.moralPresent);
   printf ("%s.m.experiencePresent = %d\n", name, (int)pvalue->m.experiencePresent);
   if (pvalue->m.fatiguePresent) {
      sprintf (namebuf, "%s.fatigue", name);
      asn1Print_EnumUnitFatigue (namebuf, &pvalue->fatigue);
   }

   if (pvalue->m.moralPresent) {
      sprintf (namebuf, "%s.moral", name);
      asn1Print_EnumUnitMoral (namebuf, &pvalue->moral);
   }

   if (pvalue->m.experiencePresent) {
      sprintf (namebuf, "%s.experience", name);
      asn1Print_EnumUnitExperience (namebuf, &pvalue->experience);
   }

}

void ASN1C_MagicActionChangeFacteursHumains::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionChangeFacteursHumains (name, &msgData);
}

void asn1Print_RecompletementDotation (ASN1ConstCharPtr name, ASN1T_RecompletementDotation* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.famille_dotation", name);
   asn1Print_EnumFamilleDotation (namebuf, &pvalue->famille_dotation);

   sprintf (namebuf, "%s.pourcentage", name);
   asn1Print_Pourcentage (namebuf, &pvalue->pourcentage);

}

void ASN1C_RecompletementDotation::Print (ASN1ConstCharPtr name)

{
   asn1Print_RecompletementDotation (name, &msgData);
}

void asn1Print_RecompletementDotationMunition (ASN1ConstCharPtr name, ASN1T_RecompletementDotationMunition* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.famille_munition", name);
   asn1Print_EnumFamilleMunition (namebuf, &pvalue->famille_munition);

   sprintf (namebuf, "%s.pourcentage", name);
   asn1Print_Pourcentage (namebuf, &pvalue->pourcentage);

}

void ASN1C_RecompletementDotationMunition::Print (ASN1ConstCharPtr name)

{
   asn1Print_RecompletementDotationMunition (name, &msgData);
}

void asn1Print_RecompletementEquipement (ASN1ConstCharPtr name, ASN1T_RecompletementEquipement* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.type_equipement", name);
   asn1Print_TypeEquipement (namebuf, &pvalue->type_equipement);

   sprintf (namebuf, "%s.nombre_disponible", name);
   rtPrintInteger (namebuf, pvalue->nombre_disponible);

}

void ASN1C_RecompletementEquipement::Print (ASN1ConstCharPtr name)

{
   asn1Print_RecompletementEquipement (name, &msgData);
}

void asn1Print_RecompletementPersonnel (ASN1ConstCharPtr name, ASN1T_RecompletementPersonnel* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.rang", name);
   asn1Print_EnumHumanRank (namebuf, &pvalue->rang);

   sprintf (namebuf, "%s.nombre_disponible", name);
   rtPrintInteger (namebuf, pvalue->nombre_disponible);

}

void ASN1C_RecompletementPersonnel::Print (ASN1ConstCharPtr name)

{
   asn1Print_RecompletementPersonnel (name, &msgData);
}

void asn1Print_RecompletementStock (ASN1ConstCharPtr name, ASN1T_RecompletementStock* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.ressource_id", name);
   asn1Print_TypeDotation (namebuf, &pvalue->ressource_id);

   sprintf (namebuf, "%s.quantite_disponible", name);
   rtPrintInteger (namebuf, pvalue->quantite_disponible);

}

void ASN1C_RecompletementStock::Print (ASN1ConstCharPtr name)

{
   asn1Print_RecompletementStock (name, &msgData);
}

void asn1Print__SeqOfRecompletementEquipement (ASN1ConstCharPtr name, ASN1T__SeqOfRecompletementEquipement* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_RecompletementEquipement (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C__SeqOfRecompletementEquipement::Print (ASN1ConstCharPtr name)

{
   asn1Print__SeqOfRecompletementEquipement (name, &msgData);
}

void asn1Print__SeqOfRecompletementPersonnel (ASN1ConstCharPtr name, ASN1T__SeqOfRecompletementPersonnel* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_RecompletementPersonnel (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C__SeqOfRecompletementPersonnel::Print (ASN1ConstCharPtr name)

{
   asn1Print__SeqOfRecompletementPersonnel (name, &msgData);
}

void asn1Print__SeqOfRecompletementDotation (ASN1ConstCharPtr name, ASN1T__SeqOfRecompletementDotation* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_RecompletementDotation (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C__SeqOfRecompletementDotation::Print (ASN1ConstCharPtr name)

{
   asn1Print__SeqOfRecompletementDotation (name, &msgData);
}

void asn1Print__SeqOfRecompletementDotationMunition (ASN1ConstCharPtr name, ASN1T__SeqOfRecompletementDotationMunition* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_RecompletementDotationMunition (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C__SeqOfRecompletementDotationMunition::Print (ASN1ConstCharPtr name)

{
   asn1Print__SeqOfRecompletementDotationMunition (name, &msgData);
}

void asn1Print__SeqOfRecompletementStock (ASN1ConstCharPtr name, ASN1T__SeqOfRecompletementStock* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_RecompletementStock (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C__SeqOfRecompletementStock::Print (ASN1ConstCharPtr name)

{
   asn1Print__SeqOfRecompletementStock (name, &msgData);
}

void asn1Print_MagicActionRecompletementPartiel (ASN1ConstCharPtr name, ASN1T_MagicActionRecompletementPartiel* pvalue)
{
   char namebuf[512];

   printf ("%s.m.equipementsPresent = %d\n", name, (int)pvalue->m.equipementsPresent);
   printf ("%s.m.personnelsPresent = %d\n", name, (int)pvalue->m.personnelsPresent);
   printf ("%s.m.dotationsPresent = %d\n", name, (int)pvalue->m.dotationsPresent);
   printf ("%s.m.munitionsPresent = %d\n", name, (int)pvalue->m.munitionsPresent);
   printf ("%s.m.stocksPresent = %d\n", name, (int)pvalue->m.stocksPresent);
   if (pvalue->m.equipementsPresent) {
      sprintf (namebuf, "%s.equipements", name);
      asn1Print__SeqOfRecompletementEquipement (namebuf, &pvalue->equipements);
   }

   if (pvalue->m.personnelsPresent) {
      sprintf (namebuf, "%s.personnels", name);
      asn1Print__SeqOfRecompletementPersonnel (namebuf, &pvalue->personnels);
   }

   if (pvalue->m.dotationsPresent) {
      sprintf (namebuf, "%s.dotations", name);
      asn1Print__SeqOfRecompletementDotation (namebuf, &pvalue->dotations);
   }

   if (pvalue->m.munitionsPresent) {
      sprintf (namebuf, "%s.munitions", name);
      asn1Print__SeqOfRecompletementDotationMunition (namebuf, &pvalue->munitions);
   }

   if (pvalue->m.stocksPresent) {
      sprintf (namebuf, "%s.stocks", name);
      asn1Print__SeqOfRecompletementStock (namebuf, &pvalue->stocks);
   }

}

void ASN1C_MagicActionRecompletementPartiel::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionRecompletementPartiel (name, &msgData);
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
         asn1Print_MagicActionRecompletementPartiel (namebuf, pvalue->u.recompletement_partiel);
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
         asn1Print_MagicActionChangeFacteursHumains (namebuf, pvalue->u.change_facteurs_humains);
         break;

      case 8:
         sprintf (namebuf, "%s.u.destruction_totale", name);
         break;

      case 9:
         sprintf (namebuf, "%s.u.se_rendre", name);
         break;

      case 10:
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

void asn1Print_EnumUnitAttrErrorCode (ASN1ConstCharPtr name, ASN1T_EnumUnitAttrErrorCode* pvalue)
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

void ASN1C_EnumUnitAttrErrorCode::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumUnitAttrErrorCode (name, &msgData);
}

void asn1Print_MsgUnitMagicActionAck (ASN1ConstCharPtr name, ASN1T_MsgUnitMagicActionAck* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.error_code", name);
   asn1Print_EnumUnitAttrErrorCode (namebuf, &pvalue->error_code);

}

void ASN1C_MsgUnitMagicActionAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgUnitMagicActionAck (name, &msgData);
}

void asn1Print_MsgUnitPathFind (ASN1ConstCharPtr name, ASN1T_MsgUnitPathFind* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_pion", name);
   asn1Print_OID (namebuf, &pvalue->oid_pion);

   sprintf (namebuf, "%s.itineraire", name);
   asn1Print_Itineraire (namebuf, &pvalue->itineraire);

}

void ASN1C_MsgUnitPathFind::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgUnitPathFind (name, &msgData);
}


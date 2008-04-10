/* This file was generated by ASN1C V5.31 on 07-Apr-2008 */

#include "PopulationKnowledge.h"

void asn1Print_PopulationKnowledgeIncludes (ASN1ConstCharPtr name, ASN1T_PopulationKnowledgeIncludes* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.baseExports", name);

   sprintf (namebuf, "%s.populationExports", name);

}

void ASN1C_PopulationKnowledgeIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_PopulationKnowledgeIncludes (name, &msgData);
}

void asn1Print_PopulationKnowledgeExports (ASN1ConstCharPtr name)
{
}

void ASN1C_PopulationKnowledgeExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_PopulationKnowledgeExports (name);
}

void asn1Print_MsgPopulationKnowledgeCreation (ASN1ConstCharPtr name, ASN1T_MsgPopulationKnowledgeCreation* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_connaissance", name);
   asn1Print_OID (namebuf, &pvalue->oid_connaissance);

   sprintf (namebuf, "%s.oid_groupe_possesseur", name);
   asn1Print_OID (namebuf, &pvalue->oid_groupe_possesseur);

   sprintf (namebuf, "%s.oid_population_reelle", name);
   asn1Print_OID (namebuf, &pvalue->oid_population_reelle);

   sprintf (namebuf, "%s.camp", name);
   asn1Print_OID (namebuf, &pvalue->camp);

}

void ASN1C_MsgPopulationKnowledgeCreation::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgPopulationKnowledgeCreation (name, &msgData);
}

void asn1Print_MsgPopulationKnowledgeUpdate (ASN1ConstCharPtr name, ASN1T_MsgPopulationKnowledgeUpdate* pvalue)
{
   char namebuf[512];

   printf ("%s.m.etat_dominationPresent = %d\n", name, (int)pvalue->m.etat_dominationPresent);
   sprintf (namebuf, "%s.oid_connaissance", name);
   asn1Print_OID (namebuf, &pvalue->oid_connaissance);

   sprintf (namebuf, "%s.oid_groupe_possesseur", name);
   asn1Print_OID (namebuf, &pvalue->oid_groupe_possesseur);

   if (pvalue->m.etat_dominationPresent) {
      sprintf (namebuf, "%s.etat_domination", name);
      asn1Print_Percentage (namebuf, &pvalue->etat_domination);
   }

}

void ASN1C_MsgPopulationKnowledgeUpdate::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgPopulationKnowledgeUpdate (name, &msgData);
}

void asn1Print_MsgPopulationKnowledgeDestruction (ASN1ConstCharPtr name, ASN1T_MsgPopulationKnowledgeDestruction* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_connaissance", name);
   asn1Print_OID (namebuf, &pvalue->oid_connaissance);

   sprintf (namebuf, "%s.oid_groupe_possesseur", name);
   asn1Print_OID (namebuf, &pvalue->oid_groupe_possesseur);

}

void ASN1C_MsgPopulationKnowledgeDestruction::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgPopulationKnowledgeDestruction (name, &msgData);
}

void asn1Print_MsgPopulationConcentrationKnowledgeCreation (ASN1ConstCharPtr name, ASN1T_MsgPopulationConcentrationKnowledgeCreation* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_connaissance_concentration", name);
   asn1Print_OID (namebuf, &pvalue->oid_connaissance_concentration);

   sprintf (namebuf, "%s.oid_connaissance_population", name);
   asn1Print_OID (namebuf, &pvalue->oid_connaissance_population);

   sprintf (namebuf, "%s.oid_groupe_possesseur", name);
   asn1Print_OID (namebuf, &pvalue->oid_groupe_possesseur);

   sprintf (namebuf, "%s.oid_concentration_reelle", name);
   asn1Print_OID (namebuf, &pvalue->oid_concentration_reelle);

   sprintf (namebuf, "%s.position", name);
   asn1Print_CoordLatLong (namebuf, &pvalue->position);

}

void ASN1C_MsgPopulationConcentrationKnowledgeCreation::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgPopulationConcentrationKnowledgeCreation (name, &msgData);
}

void asn1Print_MsgPopulationConcentrationKnowledgeUpdate (ASN1ConstCharPtr name, ASN1T_MsgPopulationConcentrationKnowledgeUpdate* pvalue)
{
   char namebuf[512];

   printf ("%s.m.oid_concentration_reellePresent = %d\n", name, (int)pvalue->m.oid_concentration_reellePresent);
   printf ("%s.m.nb_humains_vivantsPresent = %d\n", name, (int)pvalue->m.nb_humains_vivantsPresent);
   printf ("%s.m.nb_humains_mortsPresent = %d\n", name, (int)pvalue->m.nb_humains_mortsPresent);
   printf ("%s.m.attitudePresent = %d\n", name, (int)pvalue->m.attitudePresent);
   printf ("%s.m.pertinencePresent = %d\n", name, (int)pvalue->m.pertinencePresent);
   printf ("%s.m.est_percuPresent = %d\n", name, (int)pvalue->m.est_percuPresent);
   sprintf (namebuf, "%s.oid_connaissance_concentration", name);
   asn1Print_OID (namebuf, &pvalue->oid_connaissance_concentration);

   sprintf (namebuf, "%s.oid_connaissance_population", name);
   asn1Print_OID (namebuf, &pvalue->oid_connaissance_population);

   sprintf (namebuf, "%s.oid_groupe_possesseur", name);
   asn1Print_OID (namebuf, &pvalue->oid_groupe_possesseur);

   if (pvalue->m.oid_concentration_reellePresent) {
      sprintf (namebuf, "%s.oid_concentration_reelle", name);
      asn1Print_OID (namebuf, &pvalue->oid_concentration_reelle);
   }

   if (pvalue->m.nb_humains_vivantsPresent) {
      sprintf (namebuf, "%s.nb_humains_vivants", name);
      rtPrintInteger (namebuf, pvalue->nb_humains_vivants);
   }

   if (pvalue->m.nb_humains_mortsPresent) {
      sprintf (namebuf, "%s.nb_humains_morts", name);
      rtPrintInteger (namebuf, pvalue->nb_humains_morts);
   }

   if (pvalue->m.attitudePresent) {
      sprintf (namebuf, "%s.attitude", name);
      asn1Print_EnumPopulationAttitude (namebuf, &pvalue->attitude);
   }

   if (pvalue->m.pertinencePresent) {
      sprintf (namebuf, "%s.pertinence", name);
      asn1Print_Percentage (namebuf, &pvalue->pertinence);
   }

   if (pvalue->m.est_percuPresent) {
      sprintf (namebuf, "%s.est_percu", name);
      rtPrintBoolean (namebuf, pvalue->est_percu);
   }

}

void ASN1C_MsgPopulationConcentrationKnowledgeUpdate::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgPopulationConcentrationKnowledgeUpdate (name, &msgData);
}

void asn1Print_MsgPopulationConcentrationKnowledgeDestruction (ASN1ConstCharPtr name, ASN1T_MsgPopulationConcentrationKnowledgeDestruction* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_connaissance_concentration", name);
   asn1Print_OID (namebuf, &pvalue->oid_connaissance_concentration);

   sprintf (namebuf, "%s.oid_connaissance_population", name);
   asn1Print_OID (namebuf, &pvalue->oid_connaissance_population);

   sprintf (namebuf, "%s.oid_groupe_possesseur", name);
   asn1Print_OID (namebuf, &pvalue->oid_groupe_possesseur);

}

void ASN1C_MsgPopulationConcentrationKnowledgeDestruction::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgPopulationConcentrationKnowledgeDestruction (name, &msgData);
}

void asn1Print_MsgPopulationFlowKnowledgeCreation (ASN1ConstCharPtr name, ASN1T_MsgPopulationFlowKnowledgeCreation* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_connaissance_flux", name);
   asn1Print_OID (namebuf, &pvalue->oid_connaissance_flux);

   sprintf (namebuf, "%s.oid_connaissance_population", name);
   asn1Print_OID (namebuf, &pvalue->oid_connaissance_population);

   sprintf (namebuf, "%s.oid_groupe_possesseur", name);
   asn1Print_OID (namebuf, &pvalue->oid_groupe_possesseur);

   sprintf (namebuf, "%s.oid_flux_reel", name);
   asn1Print_OID (namebuf, &pvalue->oid_flux_reel);

}

void ASN1C_MsgPopulationFlowKnowledgeCreation::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgPopulationFlowKnowledgeCreation (name, &msgData);
}

void asn1Print_FlowPart (ASN1ConstCharPtr name, ASN1T_FlowPart* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.forme", name);
   asn1Print_Path (namebuf, &pvalue->forme);

   sprintf (namebuf, "%s.pertinence", name);
   asn1Print_Percentage (namebuf, &pvalue->pertinence);

}

void ASN1C_FlowPart::Print (ASN1ConstCharPtr name)

{
   asn1Print_FlowPart (name, &msgData);
}

void asn1Print__SeqOfFlowPart (ASN1ConstCharPtr name, ASN1T__SeqOfFlowPart* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_FlowPart (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C__SeqOfFlowPart::Print (ASN1ConstCharPtr name)

{
   asn1Print__SeqOfFlowPart (name, &msgData);
}

void asn1Print_MsgPopulationFlowKnowledgeUpdate (ASN1ConstCharPtr name, ASN1T_MsgPopulationFlowKnowledgeUpdate* pvalue)
{
   char namebuf[512];

   printf ("%s.m.oid_flux_reelPresent = %d\n", name, (int)pvalue->m.oid_flux_reelPresent);
   printf ("%s.m.portions_fluxPresent = %d\n", name, (int)pvalue->m.portions_fluxPresent);
   printf ("%s.m.directionPresent = %d\n", name, (int)pvalue->m.directionPresent);
   printf ("%s.m.vitessePresent = %d\n", name, (int)pvalue->m.vitessePresent);
   printf ("%s.m.nb_humains_vivantsPresent = %d\n", name, (int)pvalue->m.nb_humains_vivantsPresent);
   printf ("%s.m.nb_humains_mortsPresent = %d\n", name, (int)pvalue->m.nb_humains_mortsPresent);
   printf ("%s.m.attitudePresent = %d\n", name, (int)pvalue->m.attitudePresent);
   printf ("%s.m.est_percuPresent = %d\n", name, (int)pvalue->m.est_percuPresent);
   sprintf (namebuf, "%s.oid_connaissance_flux", name);
   asn1Print_OID (namebuf, &pvalue->oid_connaissance_flux);

   sprintf (namebuf, "%s.oid_connaissance_population", name);
   asn1Print_OID (namebuf, &pvalue->oid_connaissance_population);

   sprintf (namebuf, "%s.oid_groupe_possesseur", name);
   asn1Print_OID (namebuf, &pvalue->oid_groupe_possesseur);

   if (pvalue->m.oid_flux_reelPresent) {
      sprintf (namebuf, "%s.oid_flux_reel", name);
      asn1Print_OID (namebuf, &pvalue->oid_flux_reel);
   }

   if (pvalue->m.portions_fluxPresent) {
      sprintf (namebuf, "%s.portions_flux", name);
      asn1Print__SeqOfFlowPart (namebuf, &pvalue->portions_flux);
   }

   if (pvalue->m.directionPresent) {
      sprintf (namebuf, "%s.direction", name);
      asn1Print_Heading (namebuf, &pvalue->direction);
   }

   if (pvalue->m.vitessePresent) {
      sprintf (namebuf, "%s.vitesse", name);
      asn1Print_Speed (namebuf, &pvalue->vitesse);
   }

   if (pvalue->m.nb_humains_vivantsPresent) {
      sprintf (namebuf, "%s.nb_humains_vivants", name);
      rtPrintInteger (namebuf, pvalue->nb_humains_vivants);
   }

   if (pvalue->m.nb_humains_mortsPresent) {
      sprintf (namebuf, "%s.nb_humains_morts", name);
      rtPrintInteger (namebuf, pvalue->nb_humains_morts);
   }

   if (pvalue->m.attitudePresent) {
      sprintf (namebuf, "%s.attitude", name);
      asn1Print_EnumPopulationAttitude (namebuf, &pvalue->attitude);
   }

   if (pvalue->m.est_percuPresent) {
      sprintf (namebuf, "%s.est_percu", name);
      rtPrintBoolean (namebuf, pvalue->est_percu);
   }

}

void ASN1C_MsgPopulationFlowKnowledgeUpdate::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgPopulationFlowKnowledgeUpdate (name, &msgData);
}

void asn1Print_MsgPopulationFlowKnowledgeDestruction (ASN1ConstCharPtr name, ASN1T_MsgPopulationFlowKnowledgeDestruction* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_connaissance_flux", name);
   asn1Print_OID (namebuf, &pvalue->oid_connaissance_flux);

   sprintf (namebuf, "%s.oid_connaissance_population", name);
   asn1Print_OID (namebuf, &pvalue->oid_connaissance_population);

   sprintf (namebuf, "%s.oid_groupe_possesseur", name);
   asn1Print_OID (namebuf, &pvalue->oid_groupe_possesseur);

}

void ASN1C_MsgPopulationFlowKnowledgeDestruction::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgPopulationFlowKnowledgeDestruction (name, &msgData);
}


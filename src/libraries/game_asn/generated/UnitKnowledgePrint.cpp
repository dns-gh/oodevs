/* This file was generated by ASN1C V5.31 on 08-Feb-2007 */

#include "UnitKnowledge.h"

void asn1Print_UnitKnowledgeIncludes (ASN1ConstCharPtr name, ASN1T_UnitKnowledgeIncludes* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.baseExports", name);

}

void ASN1C_UnitKnowledgeIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_UnitKnowledgeIncludes (name, &msgData);
}

void asn1Print_UnitKnowledgeExports (ASN1ConstCharPtr name)
{
}

void ASN1C_UnitKnowledgeExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_UnitKnowledgeExports (name);
}

void asn1Print_MsgUnitKnowledgeCreation (ASN1ConstCharPtr name, ASN1T_MsgUnitKnowledgeCreation* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_connaissance", name);
   asn1Print_KnowledgeAgent (namebuf, &pvalue->oid_connaissance);

   sprintf (namebuf, "%s.oid_groupe_possesseur", name);
   asn1Print_KnowledgeGroup (namebuf, &pvalue->oid_groupe_possesseur);

   sprintf (namebuf, "%s.oid_unite_reelle", name);
   asn1Print_Agent (namebuf, &pvalue->oid_unite_reelle);

   sprintf (namebuf, "%s.type_unite", name);
   asn1Print_TypePion (namebuf, &pvalue->type_unite);

}

void ASN1C_MsgUnitKnowledgeCreation::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgUnitKnowledgeCreation (name, &msgData);
}

void asn1Print_AutomatePerception (ASN1ConstCharPtr name, ASN1T_AutomatePerception* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_compagnie", name);
   asn1Print_Automate (namebuf, &pvalue->oid_compagnie);

   sprintf (namebuf, "%s.identification_level", name);
   asn1Print_EnumUnitIdentificationLevel (namebuf, &pvalue->identification_level);

}

void ASN1C_AutomatePerception::Print (ASN1ConstCharPtr name)

{
   asn1Print_AutomatePerception (name, &msgData);
}

void asn1Print__SeqOfAutomatePerception (ASN1ConstCharPtr name, ASN1T__SeqOfAutomatePerception* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_AutomatePerception (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C__SeqOfAutomatePerception::Print (ASN1ConstCharPtr name)

{
   asn1Print__SeqOfAutomatePerception (name, &msgData);
}

void asn1Print_MsgUnitKnowledgeUpdate (ASN1ConstCharPtr name, ASN1T_MsgUnitKnowledgeUpdate* pvalue)
{
   char namebuf[512];

   printf ("%s.m.pertinencePresent = %d\n", name, (int)pvalue->m.pertinencePresent);
   printf ("%s.m.identification_levelPresent = %d\n", name, (int)pvalue->m.identification_levelPresent);
   printf ("%s.m.max_identification_levelPresent = %d\n", name, (int)pvalue->m.max_identification_levelPresent);
   printf ("%s.m.etat_opPresent = %d\n", name, (int)pvalue->m.etat_opPresent);
   printf ("%s.m.mortPresent = %d\n", name, (int)pvalue->m.mortPresent);
   printf ("%s.m.positionPresent = %d\n", name, (int)pvalue->m.positionPresent);
   printf ("%s.m.directionPresent = %d\n", name, (int)pvalue->m.directionPresent);
   printf ("%s.m.speedPresent = %d\n", name, (int)pvalue->m.speedPresent);
   printf ("%s.m.campPresent = %d\n", name, (int)pvalue->m.campPresent);
   printf ("%s.m.nature_pcPresent = %d\n", name, (int)pvalue->m.nature_pcPresent);
   printf ("%s.m.perception_par_compagniePresent = %d\n", name, (int)pvalue->m.perception_par_compagniePresent);
   printf ("%s.m.renduPresent = %d\n", name, (int)pvalue->m.renduPresent);
   printf ("%s.m.prisonnierPresent = %d\n", name, (int)pvalue->m.prisonnierPresent);
   printf ("%s.m.refugie_pris_en_comptePresent = %d\n", name, (int)pvalue->m.refugie_pris_en_comptePresent);
   sprintf (namebuf, "%s.oid_connaissance", name);
   asn1Print_KnowledgeAgent (namebuf, &pvalue->oid_connaissance);

   sprintf (namebuf, "%s.oid_groupe_possesseur", name);
   asn1Print_KnowledgeGroup (namebuf, &pvalue->oid_groupe_possesseur);

   if (pvalue->m.pertinencePresent) {
      sprintf (namebuf, "%s.pertinence", name);
      asn1Print_Pourcentage (namebuf, &pvalue->pertinence);
   }

   if (pvalue->m.identification_levelPresent) {
      sprintf (namebuf, "%s.identification_level", name);
      asn1Print_EnumUnitIdentificationLevel (namebuf, &pvalue->identification_level);
   }

   if (pvalue->m.max_identification_levelPresent) {
      sprintf (namebuf, "%s.max_identification_level", name);
      asn1Print_EnumUnitIdentificationLevel (namebuf, &pvalue->max_identification_level);
   }

   if (pvalue->m.etat_opPresent) {
      sprintf (namebuf, "%s.etat_op", name);
      asn1Print_PourcentageLibre (namebuf, &pvalue->etat_op);
   }

   if (pvalue->m.mortPresent) {
      sprintf (namebuf, "%s.mort", name);
      rtPrintBoolean (namebuf, pvalue->mort);
   }

   if (pvalue->m.positionPresent) {
      sprintf (namebuf, "%s.position", name);
      asn1Print_CoordUTM (namebuf, &pvalue->position);
   }

   if (pvalue->m.directionPresent) {
      sprintf (namebuf, "%s.direction", name);
      asn1Print_Direction (namebuf, &pvalue->direction);
   }

   if (pvalue->m.speedPresent) {
      sprintf (namebuf, "%s.speed", name);
      asn1Print_Vitesse (namebuf, &pvalue->speed);
   }

   if (pvalue->m.campPresent) {
      sprintf (namebuf, "%s.camp", name);
      asn1Print_OID (namebuf, &pvalue->camp);
   }

   if (pvalue->m.nature_pcPresent) {
      sprintf (namebuf, "%s.nature_pc", name);
      rtPrintBoolean (namebuf, pvalue->nature_pc);
   }

   if (pvalue->m.perception_par_compagniePresent) {
      sprintf (namebuf, "%s.perception_par_compagnie", name);
      asn1Print__SeqOfAutomatePerception (namebuf, &pvalue->perception_par_compagnie);
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

void ASN1C_MsgUnitKnowledgeUpdate::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgUnitKnowledgeUpdate (name, &msgData);
}

void asn1Print_MsgUnitKnowledgeDestruction (ASN1ConstCharPtr name, ASN1T_MsgUnitKnowledgeDestruction* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_connaissance", name);
   asn1Print_KnowledgeAgent (namebuf, &pvalue->oid_connaissance);

   sprintf (namebuf, "%s.oid_groupe_possesseur", name);
   asn1Print_KnowledgeGroup (namebuf, &pvalue->oid_groupe_possesseur);

}

void ASN1C_MsgUnitKnowledgeDestruction::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgUnitKnowledgeDestruction (name, &msgData);
}


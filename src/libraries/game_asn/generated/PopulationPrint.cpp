/* This file was generated by ASN1C V5.31 on 09-Jul-2009 */

#include "Population.h"

void asn1Print_PopulationIncludes (ASN1ConstCharPtr name, ASN1T_PopulationIncludes* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.baseExports", name);

}

void ASN1C_PopulationIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_PopulationIncludes (name, &msgData);
}

void asn1Print_PopulationExports (ASN1ConstCharPtr name)
{
}

void ASN1C_PopulationExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_PopulationExports (name);
}

void asn1Print_EnumPopulationAttitude (ASN1ConstCharPtr name, ASN1T_EnumPopulationAttitude* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("calme\n"); break;
      case 1: printf ("agitee\n"); break;
      case 2: printf ("excitee\n"); break;
      case 3: printf ("agressive\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumPopulationAttitude::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumPopulationAttitude (name, &msgData);
}

void asn1Print_MsgPopulationCreation (ASN1ConstCharPtr name, ASN1T_MsgPopulationCreation* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.type_population", name);
   asn1Print_PopulationType (namebuf, &pvalue->type_population);

   sprintf (namebuf, "%s.nom", name);
   rtPrintCharStr (namebuf, pvalue->nom);

   sprintf (namebuf, "%s.oid_camp", name);
   asn1Print_OID (namebuf, &pvalue->oid_camp);

}

void ASN1C_MsgPopulationCreation::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgPopulationCreation (name, &msgData);
}

void asn1Print_MsgPopulationUpdate (ASN1ConstCharPtr name, ASN1T_MsgPopulationUpdate* pvalue)
{
   char namebuf[512];

   printf ("%s.m.etat_dominationPresent = %d\n", name, (int)pvalue->m.etat_dominationPresent);
   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   if (pvalue->m.etat_dominationPresent) {
      sprintf (namebuf, "%s.etat_domination", name);
      asn1Print_Percentage (namebuf, &pvalue->etat_domination);
   }

}

void ASN1C_MsgPopulationUpdate::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgPopulationUpdate (name, &msgData);
}

void asn1Print_MsgPopulationConcentrationCreation (ASN1ConstCharPtr name, ASN1T_MsgPopulationConcentrationCreation* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.oid_population", name);
   asn1Print_OID (namebuf, &pvalue->oid_population);

   sprintf (namebuf, "%s.position", name);
   asn1Print_CoordLatLong (namebuf, &pvalue->position);

}

void ASN1C_MsgPopulationConcentrationCreation::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgPopulationConcentrationCreation (name, &msgData);
}

void asn1Print_MsgPopulationConcentrationUpdate (ASN1ConstCharPtr name, ASN1T_MsgPopulationConcentrationUpdate* pvalue)
{
   char namebuf[512];

   printf ("%s.m.nb_humains_vivantsPresent = %d\n", name, (int)pvalue->m.nb_humains_vivantsPresent);
   printf ("%s.m.nb_humains_mortsPresent = %d\n", name, (int)pvalue->m.nb_humains_mortsPresent);
   printf ("%s.m.attitudePresent = %d\n", name, (int)pvalue->m.attitudePresent);
   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.oid_population", name);
   asn1Print_OID (namebuf, &pvalue->oid_population);

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

}

void ASN1C_MsgPopulationConcentrationUpdate::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgPopulationConcentrationUpdate (name, &msgData);
}

void asn1Print_MsgPopulationConcentrationDestruction (ASN1ConstCharPtr name, ASN1T_MsgPopulationConcentrationDestruction* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.oid_population", name);
   asn1Print_OID (namebuf, &pvalue->oid_population);

}

void ASN1C_MsgPopulationConcentrationDestruction::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgPopulationConcentrationDestruction (name, &msgData);
}

void asn1Print_MsgPopulationFlowCreation (ASN1ConstCharPtr name, ASN1T_MsgPopulationFlowCreation* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.oid_population", name);
   asn1Print_OID (namebuf, &pvalue->oid_population);

}

void ASN1C_MsgPopulationFlowCreation::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgPopulationFlowCreation (name, &msgData);
}

void asn1Print_MsgPopulationFlowUpdate (ASN1ConstCharPtr name, ASN1T_MsgPopulationFlowUpdate* pvalue)
{
   char namebuf[512];

   printf ("%s.m.itinerairePresent = %d\n", name, (int)pvalue->m.itinerairePresent);
   printf ("%s.m.fluxPresent = %d\n", name, (int)pvalue->m.fluxPresent);
   printf ("%s.m.directionPresent = %d\n", name, (int)pvalue->m.directionPresent);
   printf ("%s.m.vitessePresent = %d\n", name, (int)pvalue->m.vitessePresent);
   printf ("%s.m.nb_humains_vivantsPresent = %d\n", name, (int)pvalue->m.nb_humains_vivantsPresent);
   printf ("%s.m.nb_humains_mortsPresent = %d\n", name, (int)pvalue->m.nb_humains_mortsPresent);
   printf ("%s.m.attitudePresent = %d\n", name, (int)pvalue->m.attitudePresent);
   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.oid_population", name);
   asn1Print_OID (namebuf, &pvalue->oid_population);

   if (pvalue->m.itinerairePresent) {
      sprintf (namebuf, "%s.itineraire", name);
      asn1Print_Path (namebuf, &pvalue->itineraire);
   }

   if (pvalue->m.fluxPresent) {
      sprintf (namebuf, "%s.flux", name);
      asn1Print_Path (namebuf, &pvalue->flux);
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

}

void ASN1C_MsgPopulationFlowUpdate::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgPopulationFlowUpdate (name, &msgData);
}

void asn1Print_MsgPopulationFlowDestruction (ASN1ConstCharPtr name, ASN1T_MsgPopulationFlowDestruction* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.oid_population", name);
   asn1Print_OID (namebuf, &pvalue->oid_population);

}

void ASN1C_MsgPopulationFlowDestruction::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgPopulationFlowDestruction (name, &msgData);
}

void asn1Print_MagicActionPopulationChangeAttitude_beneficiaire (ASN1ConstCharPtr name, ASN1T_MagicActionPopulationChangeAttitude_beneficiaire* pvalue)
{
   char namebuf[512];

   printf ("%s.t = %d\n", name, pvalue->t);
   switch (pvalue->t) {
      case 1:
         sprintf (namebuf, "%s.u.flux", name);
         asn1Print_OID (namebuf, &pvalue->u.flux);
         break;

      case 2:
         sprintf (namebuf, "%s.u.concentration", name);
         asn1Print_OID (namebuf, &pvalue->u.concentration);
         break;

      case 3:
         sprintf (namebuf, "%s.u.global", name);
         break;

      default:;
   }
}

void ASN1C_MagicActionPopulationChangeAttitude_beneficiaire::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionPopulationChangeAttitude_beneficiaire (name, &msgData);
}

void asn1Print_MagicActionPopulationChangeAttitude (ASN1ConstCharPtr name, ASN1T_MagicActionPopulationChangeAttitude* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.attitude", name);
   asn1Print_EnumPopulationAttitude (namebuf, &pvalue->attitude);

   sprintf (namebuf, "%s.beneficiaire", name);
   asn1Print_MagicActionPopulationChangeAttitude_beneficiaire (namebuf, &pvalue->beneficiaire);

}

void ASN1C_MagicActionPopulationChangeAttitude::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionPopulationChangeAttitude (name, &msgData);
}

void asn1Print_MagicActionPopulationMoveTo (ASN1ConstCharPtr name, ASN1T_MagicActionPopulationMoveTo* pvalue)
{
   asn1Print_CoordLatLong (name, pvalue);
}

void ASN1C_MagicActionPopulationMoveTo::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionPopulationMoveTo (name, &msgData);
}

void asn1Print_MagicActionPopulationTotalDestruction (ASN1ConstCharPtr name)
{
}

void ASN1C_MagicActionPopulationTotalDestruction::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionPopulationTotalDestruction (name);
}

void asn1Print_MagicActionPopulationKill (ASN1ConstCharPtr name, ASN1T_MagicActionPopulationKill* pvalue)
{
   rtPrintInteger (name, *pvalue);
}

void ASN1C_MagicActionPopulationKill::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionPopulationKill (name, &msgData);
}

void asn1Print_MagicActionPopulationResurrect (ASN1ConstCharPtr name, ASN1T_MagicActionPopulationResurrect* pvalue)
{
   rtPrintInteger (name, *pvalue);
}

void ASN1C_MagicActionPopulationResurrect::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionPopulationResurrect (name, &msgData);
}

void asn1Print_MsgPopulationMagicAction_action (ASN1ConstCharPtr name, ASN1T_MsgPopulationMagicAction_action* pvalue)
{
   char namebuf[512];

   printf ("%s.t = %d\n", name, pvalue->t);
   switch (pvalue->t) {
      case 1:
         sprintf (namebuf, "%s.u.change_attitude", name);
         asn1Print_MagicActionPopulationChangeAttitude (namebuf, pvalue->u.change_attitude);
         break;

      case 2:
         sprintf (namebuf, "%s.u.move_to", name);
         asn1Print_MagicActionPopulationMoveTo (namebuf, pvalue->u.move_to);
         break;

      case 3:
         sprintf (namebuf, "%s.u.destruction_totale", name);
         break;

      case 4:
         sprintf (namebuf, "%s.u.tuer", name);
         asn1Print_MagicActionPopulationKill (namebuf, &pvalue->u.tuer);
         break;

      case 5:
         sprintf (namebuf, "%s.u.ressusciter", name);
         asn1Print_MagicActionPopulationResurrect (namebuf, &pvalue->u.ressusciter);
         break;

      default:;
   }
}

void ASN1C_MsgPopulationMagicAction_action::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgPopulationMagicAction_action (name, &msgData);
}

void asn1Print_MsgPopulationMagicAction (ASN1ConstCharPtr name, ASN1T_MsgPopulationMagicAction* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.action", name);
   asn1Print_MsgPopulationMagicAction_action (namebuf, &pvalue->action);

}

void ASN1C_MsgPopulationMagicAction::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgPopulationMagicAction (name, &msgData);
}

void asn1Print_EnumPopulationErrorCode (ASN1ConstCharPtr name, ASN1T_EnumPopulationErrorCode* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("no_error\n"); break;
      case 1: printf ("error_invalid_unit\n"); break;
      case 2: printf ("error_invalid_attribute\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumPopulationErrorCode::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumPopulationErrorCode (name, &msgData);
}

void asn1Print_MsgPopulationMagicActionAck (ASN1ConstCharPtr name, ASN1T_MsgPopulationMagicActionAck* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.error_code", name);
   asn1Print_EnumPopulationErrorCode (namebuf, &pvalue->error_code);

}

void ASN1C_MsgPopulationMagicActionAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgPopulationMagicActionAck (name, &msgData);
}


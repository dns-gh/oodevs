/* This file was generated by ASN1C V5.31 on 19-Sep-2007 */

#include "Object.h"

void asn1Print_ObjectIncludes (ASN1ConstCharPtr name, ASN1T_ObjectIncludes* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.baseExports", name);

}

void ASN1C_ObjectIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_ObjectIncludes (name, &msgData);
}

void asn1Print_ObjectExports (ASN1ConstCharPtr name)
{
}

void ASN1C_ObjectExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_ObjectExports (name);
}

void asn1Print_EnumObstacleType (ASN1ConstCharPtr name, ASN1T_EnumObstacleType* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("initial\n"); break;
      case 1: printf ("reserved\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumObstacleType::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumObstacleType (name, &msgData);
}

void asn1Print_EnumObjectType (ASN1ConstCharPtr name, ASN1T_EnumObjectType* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("bouchon_mines\n"); break;
      case 1: printf ("zone_minee_lineaire\n"); break;
      case 2: printf ("zone_minee_par_dispersion\n"); break;
      case 3: printf ("fosse_anti_char\n"); break;
      case 4: printf ("abattis\n"); break;
      case 5: printf ("barricade\n"); break;
      case 6: printf ("eboulement\n"); break;
      case 7: printf ("destruction_route\n"); break;
      case 8: printf ("destruction_pont\n"); break;
      case 9: printf ("pont_flottant_continu\n"); break;
      case 10: printf ("poste_tir\n"); break;
      case 11: printf ("zone_protegee\n"); break;
      case 12: printf ("zone_implantation_canon\n"); break;
      case 13: printf ("zone_implantation_cobra\n"); break;
      case 14: printf ("zone_implantation_lrm\n"); break;
      case 15: printf ("site_franchissement\n"); break;
      case 16: printf ("nuage_nbc\n"); break;
      case 17: printf ("plot_ravitaillement\n"); break;
      case 18: printf ("site_decontamination\n"); break;
      case 19: printf ("zone_brouillage_brod\n"); break;
      case 20: printf ("rota\n"); break;
      case 21: printf ("zone_nbc\n"); break;
      case 22: printf ("zone_brouillage_bromure\n"); break;
      case 23: printf ("aire_poser\n"); break;
      case 24: printf ("piste\n"); break;
      case 25: printf ("plateforme\n"); break;
      case 26: printf ("zone_mobilite_amelioree\n"); break;
      case 27: printf ("zone_poser_helicoptere\n"); break;
      case 28: printf ("aire_logistique\n"); break;
      case 29: printf ("itineraire_logistique\n"); break;
      case 30: printf ("camp_prisonniers\n"); break;
      case 31: printf ("camp_refugies\n"); break;
      case 32: printf ("poste_controle\n"); break;
      case 33: printf ("terrain_largage\n"); break;
      case 34: printf ("zone_interdite_mouvement\n"); break;
      case 35: printf ("zone_interdite_tir\n"); break;
      case 36: printf ("zone_implantation_mortier\n"); break;
      case 37: printf ("pont_flottant_discontinu\n"); break;
      case 38: printf ("installation\n"); break;
      case 39: printf ("fire\n"); break;
      case 40: printf ("emergency_shelter\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumObjectType::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumObjectType (name, &msgData);
}

void asn1Print_EnumObjectErrorCode (ASN1ConstCharPtr name, ASN1T_EnumObjectErrorCode* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("no_error\n"); break;
      case 1: printf ("error_invalid_object\n"); break;
      case 2: printf ("error_invalid_id\n"); break;
      case 3: printf ("error_invalid_camp\n"); break;
      case 4: printf ("error_invalid_localisation\n"); break;
      case 5: printf ("error_missing_specific_attributes\n"); break;
      case 6: printf ("error_invalid_specific_attributes\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumObjectErrorCode::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumObjectErrorCode (name, &msgData);
}

void asn1Print_ObjectAttributesCrossingSite (ASN1ConstCharPtr name, ASN1T_ObjectAttributesCrossingSite* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.width", name);
   rtPrintInteger (namebuf, pvalue->width);

   sprintf (namebuf, "%s.depth", name);
   rtPrintInteger (namebuf, pvalue->depth);

   sprintf (namebuf, "%s.flow_rate", name);
   rtPrintInteger (namebuf, pvalue->flow_rate);

   sprintf (namebuf, "%s.banks_require_fitting", name);
   rtPrintBoolean (namebuf, pvalue->banks_require_fitting);

}

void ASN1C_ObjectAttributesCrossingSite::Print (ASN1ConstCharPtr name)

{
   asn1Print_ObjectAttributesCrossingSite (name, &msgData);
}

void asn1Print_ObjectAttributesNbcCloud (ASN1ConstCharPtr name, ASN1T_ObjectAttributesNbcCloud* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.nbc_agent", name);
   asn1Print_OID (namebuf, &pvalue->nbc_agent);

}

void ASN1C_ObjectAttributesNbcCloud::Print (ASN1ConstCharPtr name)

{
   asn1Print_ObjectAttributesNbcCloud (name, &msgData);
}

void asn1Print_ObjectAttributesNbcZone (ASN1ConstCharPtr name, ASN1T_ObjectAttributesNbcZone* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.nbc_agent", name);
   asn1Print_OID (namebuf, &pvalue->nbc_agent);

}

void ASN1C_ObjectAttributesNbcZone::Print (ASN1ConstCharPtr name)

{
   asn1Print_ObjectAttributesNbcZone (name, &msgData);
}

void asn1Print_ObjectAttributesRota (ASN1ConstCharPtr name, ASN1T_ObjectAttributesRota* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.danger_level", name);
   rtPrintInteger (namebuf, pvalue->danger_level);

   sprintf (namebuf, "%s.nbc_agents", name);
   asn1Print_ListOID (namebuf, &pvalue->nbc_agents);

}

void ASN1C_ObjectAttributesRota::Print (ASN1ConstCharPtr name)

{
   asn1Print_ObjectAttributesRota (name, &msgData);
}

void asn1Print_ObjectAttributesLogisticRoute (ASN1ConstCharPtr name, ASN1T_ObjectAttributesLogisticRoute* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.equipped", name);
   rtPrintBoolean (namebuf, pvalue->equipped);

   sprintf (namebuf, "%s.max_weight", name);
   rtPrintInteger (namebuf, pvalue->max_weight);

   sprintf (namebuf, "%s.width", name);
   rtPrintInteger (namebuf, pvalue->width);

   sprintf (namebuf, "%s.length", name);
   rtPrintInteger (namebuf, pvalue->length);

   sprintf (namebuf, "%s.flow_rate", name);
   rtPrintInteger (namebuf, pvalue->flow_rate);

}

void ASN1C_ObjectAttributesLogisticRoute::Print (ASN1ConstCharPtr name)

{
   asn1Print_ObjectAttributesLogisticRoute (name, &msgData);
}

void asn1Print_ObjectAttributesPrisonerCamp (ASN1ConstCharPtr name, ASN1T_ObjectAttributesPrisonerCamp* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.tc2", name);
   asn1Print_Automat (namebuf, &pvalue->tc2);

}

void ASN1C_ObjectAttributesPrisonerCamp::Print (ASN1ConstCharPtr name)

{
   asn1Print_ObjectAttributesPrisonerCamp (name, &msgData);
}

void asn1Print_ObjectAttributesRefugeeCamp (ASN1ConstCharPtr name, ASN1T_ObjectAttributesRefugeeCamp* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.tc2", name);
   asn1Print_Automat (namebuf, &pvalue->tc2);

}

void ASN1C_ObjectAttributesRefugeeCamp::Print (ASN1ConstCharPtr name)

{
   asn1Print_ObjectAttributesRefugeeCamp (name, &msgData);
}

void asn1Print_ObjectAttributesLinearMineArea (ASN1ConstCharPtr name, ASN1T_ObjectAttributesLinearMineArea* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.density", name);
   rtPrintReal (namebuf, pvalue->density);

   sprintf (namebuf, "%s.activity_time", name);
   rtPrintInteger (namebuf, pvalue->activity_time);

}

void ASN1C_ObjectAttributesLinearMineArea::Print (ASN1ConstCharPtr name)

{
   asn1Print_ObjectAttributesLinearMineArea (name, &msgData);
}

void asn1Print_ObjectAttributesDispersedMineArea (ASN1ConstCharPtr name, ASN1T_ObjectAttributesDispersedMineArea* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.density", name);
   rtPrintReal (namebuf, pvalue->density);

   sprintf (namebuf, "%s.activity_time", name);
   rtPrintInteger (namebuf, pvalue->activity_time);

}

void ASN1C_ObjectAttributesDispersedMineArea::Print (ASN1ConstCharPtr name)

{
   asn1Print_ObjectAttributesDispersedMineArea (name, &msgData);
}

void asn1Print_ObjectAttributesMineJam (ASN1ConstCharPtr name, ASN1T_ObjectAttributesMineJam* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.activity_time", name);
   rtPrintInteger (namebuf, pvalue->activity_time);

}

void ASN1C_ObjectAttributesMineJam::Print (ASN1ConstCharPtr name)

{
   asn1Print_ObjectAttributesMineJam (name, &msgData);
}

void asn1Print_ObjectAttributesSpecific (ASN1ConstCharPtr name, ASN1T_ObjectAttributesSpecific* pvalue)
{
   char namebuf[512];

   printf ("%s.t = %d\n", name, pvalue->t);
   switch (pvalue->t) {
      case 1:
         sprintf (namebuf, "%s.u.crossing_site", name);
         asn1Print_ObjectAttributesCrossingSite (namebuf, pvalue->u.crossing_site);
         break;

      case 2:
         sprintf (namebuf, "%s.u.nbc_cloud", name);
         asn1Print_ObjectAttributesNbcCloud (namebuf, pvalue->u.nbc_cloud);
         break;

      case 3:
         sprintf (namebuf, "%s.u.rota", name);
         asn1Print_ObjectAttributesRota (namebuf, pvalue->u.rota);
         break;

      case 4:
         sprintf (namebuf, "%s.u.nbc_zone", name);
         asn1Print_ObjectAttributesNbcZone (namebuf, pvalue->u.nbc_zone);
         break;

      case 5:
         sprintf (namebuf, "%s.u.logistic_route", name);
         asn1Print_ObjectAttributesLogisticRoute (namebuf, pvalue->u.logistic_route);
         break;

      case 6:
         sprintf (namebuf, "%s.u.prisoner_camp", name);
         asn1Print_ObjectAttributesPrisonerCamp (namebuf, pvalue->u.prisoner_camp);
         break;

      case 7:
         sprintf (namebuf, "%s.u.refugee_camp", name);
         asn1Print_ObjectAttributesRefugeeCamp (namebuf, pvalue->u.refugee_camp);
         break;

      case 8:
         sprintf (namebuf, "%s.u.linear_mine_area", name);
         asn1Print_ObjectAttributesLinearMineArea (namebuf, pvalue->u.linear_mine_area);
         break;

      case 9:
         sprintf (namebuf, "%s.u.dispersed_mine_area", name);
         asn1Print_ObjectAttributesDispersedMineArea (namebuf, pvalue->u.dispersed_mine_area);
         break;

      case 10:
         sprintf (namebuf, "%s.u.mine_jam", name);
         asn1Print_ObjectAttributesMineJam (namebuf, pvalue->u.mine_jam);
         break;

      default:;
   }
}

void ASN1C_ObjectAttributesSpecific::Print (ASN1ConstCharPtr name)

{
   asn1Print_ObjectAttributesSpecific (name, &msgData);
}

void asn1Print_MsgObjectCreation (ASN1ConstCharPtr name, ASN1T_MsgObjectCreation* pvalue)
{
   char namebuf[512];

   printf ("%s.m.obstacle_typePresent = %d\n", name, (int)pvalue->m.obstacle_typePresent);
   printf ("%s.m.reserved_obstacle_activatedPresent = %d\n", name, (int)pvalue->m.reserved_obstacle_activatedPresent);
   printf ("%s.m.specific_attributesPresent = %d\n", name, (int)pvalue->m.specific_attributesPresent);
   printf ("%s.m.construction_dotation_typePresent = %d\n", name, (int)pvalue->m.construction_dotation_typePresent);
   printf ("%s.m.mining_dotation_typePresent = %d\n", name, (int)pvalue->m.mining_dotation_typePresent);
   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.type", name);
   asn1Print_EnumObjectType (namebuf, &pvalue->type);

   if (pvalue->m.obstacle_typePresent) {
      sprintf (namebuf, "%s.obstacle_type", name);
      asn1Print_EnumObstacleType (namebuf, &pvalue->obstacle_type);
   }

   if (pvalue->m.reserved_obstacle_activatedPresent) {
      sprintf (namebuf, "%s.reserved_obstacle_activated", name);
      rtPrintBoolean (namebuf, pvalue->reserved_obstacle_activated);
   }

   sprintf (namebuf, "%s.location", name);
   asn1Print_Location (namebuf, &pvalue->location);

   sprintf (namebuf, "%s.name", name);
   rtPrintCharStr (namebuf, pvalue->name);

   sprintf (namebuf, "%s.team", name);
   asn1Print_Team (namebuf, &pvalue->team);

   if (pvalue->m.specific_attributesPresent) {
      sprintf (namebuf, "%s.specific_attributes", name);
      asn1Print_ObjectAttributesSpecific (namebuf, &pvalue->specific_attributes);
   }

   if (pvalue->m.construction_dotation_typePresent) {
      sprintf (namebuf, "%s.construction_dotation_type", name);
      asn1Print_DotationType (namebuf, &pvalue->construction_dotation_type);
   }

   if (pvalue->m.mining_dotation_typePresent) {
      sprintf (namebuf, "%s.mining_dotation_type", name);
      asn1Print_DotationType (namebuf, &pvalue->mining_dotation_type);
   }

}

void ASN1C_MsgObjectCreation::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgObjectCreation (name, &msgData);
}

void asn1Print_MsgObjectUpdate (ASN1ConstCharPtr name, ASN1T_MsgObjectUpdate* pvalue)
{
   char namebuf[512];

   printf ("%s.m.construction_percentagePresent = %d\n", name, (int)pvalue->m.construction_percentagePresent);
   printf ("%s.m.mining_percentagePresent = %d\n", name, (int)pvalue->m.mining_percentagePresent);
   printf ("%s.m.bypass_construction_percentagePresent = %d\n", name, (int)pvalue->m.bypass_construction_percentagePresent);
   printf ("%s.m.reserved_obstacle_activatedPresent = %d\n", name, (int)pvalue->m.reserved_obstacle_activatedPresent);
   printf ("%s.m.construction_dotation_nbrPresent = %d\n", name, (int)pvalue->m.construction_dotation_nbrPresent);
   printf ("%s.m.mining_dotation_nbrPresent = %d\n", name, (int)pvalue->m.mining_dotation_nbrPresent);
   printf ("%s.m.locationPresent = %d\n", name, (int)pvalue->m.locationPresent);
   printf ("%s.m.specific_attributesPresent = %d\n", name, (int)pvalue->m.specific_attributesPresent);
   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   if (pvalue->m.construction_percentagePresent) {
      sprintf (namebuf, "%s.construction_percentage", name);
      rtPrintInteger (namebuf, pvalue->construction_percentage);
   }

   if (pvalue->m.mining_percentagePresent) {
      sprintf (namebuf, "%s.mining_percentage", name);
      rtPrintInteger (namebuf, pvalue->mining_percentage);
   }

   if (pvalue->m.bypass_construction_percentagePresent) {
      sprintf (namebuf, "%s.bypass_construction_percentage", name);
      rtPrintInteger (namebuf, pvalue->bypass_construction_percentage);
   }

   if (pvalue->m.reserved_obstacle_activatedPresent) {
      sprintf (namebuf, "%s.reserved_obstacle_activated", name);
      rtPrintBoolean (namebuf, pvalue->reserved_obstacle_activated);
   }

   if (pvalue->m.construction_dotation_nbrPresent) {
      sprintf (namebuf, "%s.construction_dotation_nbr", name);
      rtPrintInteger (namebuf, pvalue->construction_dotation_nbr);
   }

   if (pvalue->m.mining_dotation_nbrPresent) {
      sprintf (namebuf, "%s.mining_dotation_nbr", name);
      rtPrintInteger (namebuf, pvalue->mining_dotation_nbr);
   }

   if (pvalue->m.locationPresent) {
      sprintf (namebuf, "%s.location", name);
      asn1Print_Location (namebuf, &pvalue->location);
   }

   if (pvalue->m.specific_attributesPresent) {
      sprintf (namebuf, "%s.specific_attributes", name);
      asn1Print_ObjectAttributesSpecific (namebuf, &pvalue->specific_attributes);
   }

}

void ASN1C_MsgObjectUpdate::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgObjectUpdate (name, &msgData);
}

void asn1Print_MsgObjectDestruction (ASN1ConstCharPtr name, ASN1T_MsgObjectDestruction* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_MsgObjectDestruction::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgObjectDestruction (name, &msgData);
}

void asn1Print_MagicActionUpdateObject (ASN1ConstCharPtr name, ASN1T_MagicActionUpdateObject* pvalue)
{
   char namebuf[512];

   printf ("%s.m.construction_percentagePresent = %d\n", name, (int)pvalue->m.construction_percentagePresent);
   printf ("%s.m.mining_percentagePresent = %d\n", name, (int)pvalue->m.mining_percentagePresent);
   printf ("%s.m.bypass_construction_percentagePresent = %d\n", name, (int)pvalue->m.bypass_construction_percentagePresent);
   printf ("%s.m.reserved_obstacle_activatedPresent = %d\n", name, (int)pvalue->m.reserved_obstacle_activatedPresent);
   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   if (pvalue->m.construction_percentagePresent) {
      sprintf (namebuf, "%s.construction_percentage", name);
      rtPrintInteger (namebuf, pvalue->construction_percentage);
   }

   if (pvalue->m.mining_percentagePresent) {
      sprintf (namebuf, "%s.mining_percentage", name);
      rtPrintInteger (namebuf, pvalue->mining_percentage);
   }

   if (pvalue->m.bypass_construction_percentagePresent) {
      sprintf (namebuf, "%s.bypass_construction_percentage", name);
      rtPrintInteger (namebuf, pvalue->bypass_construction_percentage);
   }

   if (pvalue->m.reserved_obstacle_activatedPresent) {
      sprintf (namebuf, "%s.reserved_obstacle_activated", name);
      rtPrintBoolean (namebuf, pvalue->reserved_obstacle_activated);
   }

}

void ASN1C_MagicActionUpdateObject::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionUpdateObject (name, &msgData);
}

void asn1Print_MagicActionCreateObject (ASN1ConstCharPtr name, ASN1T_MagicActionCreateObject* pvalue)
{
   char namebuf[512];

   printf ("%s.m.obstacle_typePresent = %d\n", name, (int)pvalue->m.obstacle_typePresent);
   printf ("%s.m.reserved_obstacle_activatedPresent = %d\n", name, (int)pvalue->m.reserved_obstacle_activatedPresent);
   printf ("%s.m.namePresent = %d\n", name, (int)pvalue->m.namePresent);
   printf ("%s.m.specific_attributesPresent = %d\n", name, (int)pvalue->m.specific_attributesPresent);
   sprintf (namebuf, "%s.team", name);
   asn1Print_Team (namebuf, &pvalue->team);

   sprintf (namebuf, "%s.type", name);
   asn1Print_EnumObjectType (namebuf, &pvalue->type);

   if (pvalue->m.obstacle_typePresent) {
      sprintf (namebuf, "%s.obstacle_type", name);
      asn1Print_EnumObstacleType (namebuf, &pvalue->obstacle_type);
   }

   if (pvalue->m.reserved_obstacle_activatedPresent) {
      sprintf (namebuf, "%s.reserved_obstacle_activated", name);
      rtPrintBoolean (namebuf, pvalue->reserved_obstacle_activated);
   }

   sprintf (namebuf, "%s.location", name);
   asn1Print_Location (namebuf, &pvalue->location);

   if (pvalue->m.namePresent) {
      sprintf (namebuf, "%s.name", name);
      rtPrintCharStr (namebuf, pvalue->name);
   }

   if (pvalue->m.specific_attributesPresent) {
      sprintf (namebuf, "%s.specific_attributes", name);
      asn1Print_ObjectAttributesSpecific (namebuf, &pvalue->specific_attributes);
   }

}

void ASN1C_MagicActionCreateObject::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionCreateObject (name, &msgData);
}

void asn1Print_MagicActionDestroyObjet (ASN1ConstCharPtr name, ASN1T_MagicActionDestroyObjet* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_MagicActionDestroyObjet::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionDestroyObjet (name, &msgData);
}

void asn1Print_MsgObjectMagicAction_action (ASN1ConstCharPtr name, ASN1T_MsgObjectMagicAction_action* pvalue)
{
   char namebuf[512];

   printf ("%s.t = %d\n", name, pvalue->t);
   switch (pvalue->t) {
      case 1:
         sprintf (namebuf, "%s.u.create_object", name);
         asn1Print_MagicActionCreateObject (namebuf, pvalue->u.create_object);
         break;

      case 2:
         sprintf (namebuf, "%s.u.update_object", name);
         asn1Print_MagicActionUpdateObject (namebuf, pvalue->u.update_object);
         break;

      case 3:
         sprintf (namebuf, "%s.u.destroy_object", name);
         asn1Print_MagicActionDestroyObjet (namebuf, &pvalue->u.destroy_object);
         break;

      default:;
   }
}

void ASN1C_MsgObjectMagicAction_action::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgObjectMagicAction_action (name, &msgData);
}

void asn1Print_MsgObjectMagicAction (ASN1ConstCharPtr name, ASN1T_MsgObjectMagicAction* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.action", name);
   asn1Print_MsgObjectMagicAction_action (namebuf, &pvalue->action);

}

void ASN1C_MsgObjectMagicAction::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgObjectMagicAction (name, &msgData);
}

void asn1Print_MsgObjectMagicActionAck (ASN1ConstCharPtr name, ASN1T_MsgObjectMagicActionAck* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.error_code", name);
   asn1Print_EnumObjectErrorCode (namebuf, &pvalue->error_code);

}

void ASN1C_MsgObjectMagicActionAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgObjectMagicActionAck (name, &msgData);
}


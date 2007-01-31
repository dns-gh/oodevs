/* This file was generated by ASN1C V5.31 on 30-Jan-2007 */

#include "ScipioObject.h"

void asn1Print_ScipioObjectIncludes (ASN1ConstCharPtr name, ASN1T_ScipioObjectIncludes* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.scipioBaseExports", name);

}

void ASN1C_ScipioObjectIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_ScipioObjectIncludes (name, &msgData);
}

void asn1Print_ScipioObjectExports (ASN1ConstCharPtr name)
{
}

void ASN1C_ScipioObjectExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_ScipioObjectExports (name);
}

void asn1Print_EnumMissionGenSousTypeObstacle (ASN1ConstCharPtr name, ASN1T_EnumMissionGenSousTypeObstacle* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("preliminaire\n"); break;
      case 1: printf ("de_manoeuvre\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumMissionGenSousTypeObstacle::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumMissionGenSousTypeObstacle (name, &msgData);
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

void asn1Print_AttrObjectSiteFranchissement (ASN1ConstCharPtr name, ASN1T_AttrObjectSiteFranchissement* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.largeur", name);
   rtPrintInteger (namebuf, pvalue->largeur);

   sprintf (namebuf, "%s.profondeur", name);
   rtPrintInteger (namebuf, pvalue->profondeur);

   sprintf (namebuf, "%s.vitesse_courant", name);
   rtPrintInteger (namebuf, pvalue->vitesse_courant);

   sprintf (namebuf, "%s.berges_a_amenager", name);
   rtPrintBoolean (namebuf, pvalue->berges_a_amenager);

}

void ASN1C_AttrObjectSiteFranchissement::Print (ASN1ConstCharPtr name)

{
   asn1Print_AttrObjectSiteFranchissement (name, &msgData);
}

void asn1Print_AttrObjectNuageNBC (ASN1ConstCharPtr name, ASN1T_AttrObjectNuageNBC* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.agent_nbc", name);
   asn1Print_OID (namebuf, &pvalue->agent_nbc);

}

void ASN1C_AttrObjectNuageNBC::Print (ASN1ConstCharPtr name)

{
   asn1Print_AttrObjectNuageNBC (name, &msgData);
}

void asn1Print_AttrObjectZoneNBC (ASN1ConstCharPtr name, ASN1T_AttrObjectZoneNBC* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.agent_nbc", name);
   asn1Print_OID (namebuf, &pvalue->agent_nbc);

}

void ASN1C_AttrObjectZoneNBC::Print (ASN1ConstCharPtr name)

{
   asn1Print_AttrObjectZoneNBC (name, &msgData);
}

void asn1Print_AttrObjectROTA (ASN1ConstCharPtr name, ASN1T_AttrObjectROTA* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.niveau_danger", name);
   rtPrintInteger (namebuf, pvalue->niveau_danger);

   sprintf (namebuf, "%s.agents_nbc", name);
   asn1Print_ListOID (namebuf, &pvalue->agents_nbc);

}

void ASN1C_AttrObjectROTA::Print (ASN1ConstCharPtr name)

{
   asn1Print_AttrObjectROTA (name, &msgData);
}

void asn1Print_AttrObjectItineraireLogistique (ASN1ConstCharPtr name, ASN1T_AttrObjectItineraireLogistique* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.itineraire_equipe", name);
   rtPrintBoolean (namebuf, pvalue->itineraire_equipe);

   sprintf (namebuf, "%s.poids_max_supporte", name);
   rtPrintInteger (namebuf, pvalue->poids_max_supporte);

   sprintf (namebuf, "%s.largeur", name);
   rtPrintInteger (namebuf, pvalue->largeur);

   sprintf (namebuf, "%s.longueur", name);
   rtPrintInteger (namebuf, pvalue->longueur);

   sprintf (namebuf, "%s.debit", name);
   rtPrintInteger (namebuf, pvalue->debit);

}

void ASN1C_AttrObjectItineraireLogistique::Print (ASN1ConstCharPtr name)

{
   asn1Print_AttrObjectItineraireLogistique (name, &msgData);
}

void asn1Print_AttrObjectCampPrisonniers (ASN1ConstCharPtr name, ASN1T_AttrObjectCampPrisonniers* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.tc2", name);
   asn1Print_Automate (namebuf, &pvalue->tc2);

}

void ASN1C_AttrObjectCampPrisonniers::Print (ASN1ConstCharPtr name)

{
   asn1Print_AttrObjectCampPrisonniers (name, &msgData);
}

void asn1Print_AttrObjectCampRefugies (ASN1ConstCharPtr name, ASN1T_AttrObjectCampRefugies* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.tc2", name);
   asn1Print_Automate (namebuf, &pvalue->tc2);

}

void ASN1C_AttrObjectCampRefugies::Print (ASN1ConstCharPtr name)

{
   asn1Print_AttrObjectCampRefugies (name, &msgData);
}

void asn1Print_AttrObjectSpecific (ASN1ConstCharPtr name, ASN1T_AttrObjectSpecific* pvalue)
{
   char namebuf[512];

   printf ("%s.t = %d\n", name, pvalue->t);
   switch (pvalue->t) {
      case 1:
         sprintf (namebuf, "%s.u.site_franchissement", name);
         asn1Print_AttrObjectSiteFranchissement (namebuf, pvalue->u.site_franchissement);
         break;

      case 2:
         sprintf (namebuf, "%s.u.nuage_nbc", name);
         asn1Print_AttrObjectNuageNBC (namebuf, pvalue->u.nuage_nbc);
         break;

      case 3:
         sprintf (namebuf, "%s.u.rota", name);
         asn1Print_AttrObjectROTA (namebuf, pvalue->u.rota);
         break;

      case 4:
         sprintf (namebuf, "%s.u.zone_nbc", name);
         asn1Print_AttrObjectZoneNBC (namebuf, pvalue->u.zone_nbc);
         break;

      case 5:
         sprintf (namebuf, "%s.u.itineraire_logistique", name);
         asn1Print_AttrObjectItineraireLogistique (namebuf, pvalue->u.itineraire_logistique);
         break;

      case 6:
         sprintf (namebuf, "%s.u.camp_prisonniers", name);
         asn1Print_AttrObjectCampPrisonniers (namebuf, pvalue->u.camp_prisonniers);
         break;

      case 7:
         sprintf (namebuf, "%s.u.camp_refugies", name);
         asn1Print_AttrObjectCampRefugies (namebuf, pvalue->u.camp_refugies);
         break;

      default:;
   }
}

void ASN1C_AttrObjectSpecific::Print (ASN1ConstCharPtr name)

{
   asn1Print_AttrObjectSpecific (name, &msgData);
}

void asn1Print_MsgObjectCreation (ASN1ConstCharPtr name, ASN1T_MsgObjectCreation* pvalue)
{
   char namebuf[512];

   printf ("%s.m.attributs_specifiquesPresent = %d\n", name, (int)pvalue->m.attributs_specifiquesPresent);
   printf ("%s.m.type_dotation_constructionPresent = %d\n", name, (int)pvalue->m.type_dotation_constructionPresent);
   printf ("%s.m.type_dotation_valorisationPresent = %d\n", name, (int)pvalue->m.type_dotation_valorisationPresent);
   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.type", name);
   asn1Print_EnumObjectType (namebuf, &pvalue->type);

   sprintf (namebuf, "%s.localisation", name);
   asn1Print_Localisation (namebuf, &pvalue->localisation);

   sprintf (namebuf, "%s.nom", name);
   rtPrintCharStr (namebuf, pvalue->nom);

   sprintf (namebuf, "%s.camp", name);
   asn1Print_Camp (namebuf, &pvalue->camp);

   if (pvalue->m.attributs_specifiquesPresent) {
      sprintf (namebuf, "%s.attributs_specifiques", name);
      asn1Print_AttrObjectSpecific (namebuf, &pvalue->attributs_specifiques);
   }

   if (pvalue->m.type_dotation_constructionPresent) {
      sprintf (namebuf, "%s.type_dotation_construction", name);
      asn1Print_TypeDotation (namebuf, &pvalue->type_dotation_construction);
   }

   if (pvalue->m.type_dotation_valorisationPresent) {
      sprintf (namebuf, "%s.type_dotation_valorisation", name);
      asn1Print_TypeDotation (namebuf, &pvalue->type_dotation_valorisation);
   }

}

void ASN1C_MsgObjectCreation::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgObjectCreation (name, &msgData);
}

void asn1Print_MsgObjectUpdate (ASN1ConstCharPtr name, ASN1T_MsgObjectUpdate* pvalue)
{
   char namebuf[512];

   printf ("%s.m.pourcentage_constructionPresent = %d\n", name, (int)pvalue->m.pourcentage_constructionPresent);
   printf ("%s.m.pourcentage_valorisationPresent = %d\n", name, (int)pvalue->m.pourcentage_valorisationPresent);
   printf ("%s.m.pourcentage_creation_contournementPresent = %d\n", name, (int)pvalue->m.pourcentage_creation_contournementPresent);
   printf ("%s.m.nb_dotation_constructionPresent = %d\n", name, (int)pvalue->m.nb_dotation_constructionPresent);
   printf ("%s.m.nb_dotation_valorisationPresent = %d\n", name, (int)pvalue->m.nb_dotation_valorisationPresent);
   printf ("%s.m.localisationPresent = %d\n", name, (int)pvalue->m.localisationPresent);
   printf ("%s.m.attributs_specifiquesPresent = %d\n", name, (int)pvalue->m.attributs_specifiquesPresent);
   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   if (pvalue->m.pourcentage_constructionPresent) {
      sprintf (namebuf, "%s.pourcentage_construction", name);
      rtPrintInteger (namebuf, pvalue->pourcentage_construction);
   }

   if (pvalue->m.pourcentage_valorisationPresent) {
      sprintf (namebuf, "%s.pourcentage_valorisation", name);
      rtPrintInteger (namebuf, pvalue->pourcentage_valorisation);
   }

   if (pvalue->m.pourcentage_creation_contournementPresent) {
      sprintf (namebuf, "%s.pourcentage_creation_contournement", name);
      rtPrintInteger (namebuf, pvalue->pourcentage_creation_contournement);
   }

   sprintf (namebuf, "%s.en_preparation", name);
   rtPrintBoolean (namebuf, pvalue->en_preparation);

   if (pvalue->m.nb_dotation_constructionPresent) {
      sprintf (namebuf, "%s.nb_dotation_construction", name);
      rtPrintInteger (namebuf, pvalue->nb_dotation_construction);
   }

   if (pvalue->m.nb_dotation_valorisationPresent) {
      sprintf (namebuf, "%s.nb_dotation_valorisation", name);
      rtPrintInteger (namebuf, pvalue->nb_dotation_valorisation);
   }

   if (pvalue->m.localisationPresent) {
      sprintf (namebuf, "%s.localisation", name);
      asn1Print_Localisation (namebuf, &pvalue->localisation);
   }

   if (pvalue->m.attributs_specifiquesPresent) {
      sprintf (namebuf, "%s.attributs_specifiques", name);
      asn1Print_AttrObjectSpecific (namebuf, &pvalue->attributs_specifiques);
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

   printf ("%s.m.pourcentage_constructionPresent = %d\n", name, (int)pvalue->m.pourcentage_constructionPresent);
   printf ("%s.m.pourcentage_valorisationPresent = %d\n", name, (int)pvalue->m.pourcentage_valorisationPresent);
   printf ("%s.m.pourcentage_creation_contournementPresent = %d\n", name, (int)pvalue->m.pourcentage_creation_contournementPresent);
   printf ("%s.m.en_preparationPresent = %d\n", name, (int)pvalue->m.en_preparationPresent);
   if (pvalue->m.pourcentage_constructionPresent) {
      sprintf (namebuf, "%s.pourcentage_construction", name);
      rtPrintInteger (namebuf, pvalue->pourcentage_construction);
   }

   if (pvalue->m.pourcentage_valorisationPresent) {
      sprintf (namebuf, "%s.pourcentage_valorisation", name);
      rtPrintInteger (namebuf, pvalue->pourcentage_valorisation);
   }

   if (pvalue->m.pourcentage_creation_contournementPresent) {
      sprintf (namebuf, "%s.pourcentage_creation_contournement", name);
      rtPrintInteger (namebuf, pvalue->pourcentage_creation_contournement);
   }

   if (pvalue->m.en_preparationPresent) {
      sprintf (namebuf, "%s.en_preparation", name);
      rtPrintBoolean (namebuf, pvalue->en_preparation);
   }

}

void ASN1C_MagicActionUpdateObject::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionUpdateObject (name, &msgData);
}

void asn1Print_MagicActionCreateObject (ASN1ConstCharPtr name, ASN1T_MagicActionCreateObject* pvalue)
{
   char namebuf[512];

   printf ("%s.m.nomPresent = %d\n", name, (int)pvalue->m.nomPresent);
   printf ("%s.m.attributs_specifiquesPresent = %d\n", name, (int)pvalue->m.attributs_specifiquesPresent);
   sprintf (namebuf, "%s.oid_camp", name);
   asn1Print_Camp (namebuf, &pvalue->oid_camp);

   sprintf (namebuf, "%s.type", name);
   asn1Print_EnumObjectType (namebuf, &pvalue->type);

   sprintf (namebuf, "%s.localisation", name);
   asn1Print_Localisation (namebuf, &pvalue->localisation);

   if (pvalue->m.nomPresent) {
      sprintf (namebuf, "%s.nom", name);
      rtPrintCharStr (namebuf, pvalue->nom);
   }

   if (pvalue->m.attributs_specifiquesPresent) {
      sprintf (namebuf, "%s.attributs_specifiques", name);
      asn1Print_AttrObjectSpecific (namebuf, &pvalue->attributs_specifiques);
   }

}

void ASN1C_MagicActionCreateObject::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionCreateObject (name, &msgData);
}

void asn1Print_MagicActionDestroyObjet (ASN1ConstCharPtr name)
{
}

void ASN1C_MagicActionDestroyObjet::Print (ASN1ConstCharPtr name)

{
   asn1Print_MagicActionDestroyObjet (name);
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

   sprintf (namebuf, "%s.oid_objet", name);
   asn1Print_OID (namebuf, &pvalue->oid_objet);

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

   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.error_code", name);
   asn1Print_EnumObjectErrorCode (namebuf, &pvalue->error_code);

}

void ASN1C_MsgObjectMagicActionAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgObjectMagicActionAck (name, &msgData);
}


/* This file was generated by ASN1C V5.31 on 30-Jan-2007 */

#include "ScipioBase.h"

void asn1Print_ScipioBaseIncludes (ASN1ConstCharPtr name, ASN1T_ScipioBaseIncludes* pvalue)
{
}

void ASN1C_ScipioBaseIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_ScipioBaseIncludes (name, &msgData);
}

void asn1Print_ScipioBaseExports (ASN1ConstCharPtr name)
{
}

void ASN1C_ScipioBaseExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_ScipioBaseExports (name);
}

void asn1Print_EnumTypeLocalisation (ASN1ConstCharPtr name, ASN1T_EnumTypeLocalisation* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("circle\n"); break;
      case 1: printf ("ellipse\n"); break;
      case 2: printf ("line\n"); break;
      case 3: printf ("polygon\n"); break;
      case 4: printf ("point\n"); break;
      case 5: printf ("sector\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumTypeLocalisation::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumTypeLocalisation (name, &msgData);
}

void asn1Print_CoordUTM (ASN1ConstCharPtr name, ASN1T_CoordUTM* pvalue)
{
   rtPrintOctStr (name, pvalue->numocts, pvalue->data, ".");
}

void ASN1C_CoordUTM::Print (ASN1ConstCharPtr name)

{
   asn1Print_CoordUTM (name, &msgData);
}

void asn1Print__SeqOfCoordUTM (ASN1ConstCharPtr name, ASN1T__SeqOfCoordUTM* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_CoordUTM (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C__SeqOfCoordUTM::Print (ASN1ConstCharPtr name)

{
   asn1Print__SeqOfCoordUTM (name, &msgData);
}

void asn1Print_Localisation (ASN1ConstCharPtr name, ASN1T_Localisation* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.type", name);
   asn1Print_EnumTypeLocalisation (namebuf, &pvalue->type);

   sprintf (namebuf, "%s.vecteur_point", name);
   asn1Print__SeqOfCoordUTM (namebuf, &pvalue->vecteur_point);

}

void ASN1C_Localisation::Print (ASN1ConstCharPtr name)

{
   asn1Print_Localisation (name, &msgData);
}

void asn1Print_Point (ASN1ConstCharPtr name, ASN1T_Point* pvalue)
{
   asn1Print_Localisation (name, pvalue);
}

void ASN1C_Point::Print (ASN1ConstCharPtr name)

{
   asn1Print_Point (name, &msgData);
}

void asn1Print_Line (ASN1ConstCharPtr name, ASN1T_Line* pvalue)
{
   asn1Print_Localisation (name, pvalue);
}

void ASN1C_Line::Print (ASN1ConstCharPtr name)

{
   asn1Print_Line (name, &msgData);
}

void asn1Print_Itineraire (ASN1ConstCharPtr name, ASN1T_Itineraire* pvalue)
{
   asn1Print_Localisation (name, pvalue);
}

void ASN1C_Itineraire::Print (ASN1ConstCharPtr name)

{
   asn1Print_Itineraire (name, &msgData);
}

void asn1Print_Polygon (ASN1ConstCharPtr name, ASN1T_Polygon* pvalue)
{
   asn1Print_Localisation (name, pvalue);
}

void ASN1C_Polygon::Print (ASN1ConstCharPtr name)

{
   asn1Print_Polygon (name, &msgData);
}

void asn1Print_Segment (ASN1ConstCharPtr name, ASN1T_Segment* pvalue)
{
   asn1Print_Localisation (name, pvalue);
}

void ASN1C_Segment::Print (ASN1ConstCharPtr name)

{
   asn1Print_Segment (name, &msgData);
}

void asn1Print_Circle (ASN1ConstCharPtr name, ASN1T_Circle* pvalue)
{
   asn1Print_Localisation (name, pvalue);
}

void ASN1C_Circle::Print (ASN1ConstCharPtr name)

{
   asn1Print_Circle (name, &msgData);
}

void asn1Print_Ellipse (ASN1ConstCharPtr name, ASN1T_Ellipse* pvalue)
{
   asn1Print_Localisation (name, pvalue);
}

void ASN1C_Ellipse::Print (ASN1ConstCharPtr name)

{
   asn1Print_Ellipse (name, &msgData);
}

void asn1Print_Sector (ASN1ConstCharPtr name, ASN1T_Sector* pvalue)
{
   asn1Print_Localisation (name, pvalue);
}

void ASN1C_Sector::Print (ASN1ConstCharPtr name)

{
   asn1Print_Sector (name, &msgData);
}

void asn1Print_ListLocalisation (ASN1ConstCharPtr name, ASN1T_ListLocalisation* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_Localisation (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C_ListLocalisation::Print (ASN1ConstCharPtr name)

{
   asn1Print_ListLocalisation (name, &msgData);
}

void asn1Print_ListPolygon (ASN1ConstCharPtr name, ASN1T_ListPolygon* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_Polygon (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C_ListPolygon::Print (ASN1ConstCharPtr name)

{
   asn1Print_ListPolygon (name, &msgData);
}

void asn1Print_ListItineraire (ASN1ConstCharPtr name, ASN1T_ListItineraire* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_Itineraire (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C_ListItineraire::Print (ASN1ConstCharPtr name)

{
   asn1Print_ListItineraire (name, &msgData);
}

void asn1Print_ListPoint (ASN1ConstCharPtr name, ASN1T_ListPoint* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_Point (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C_ListPoint::Print (ASN1ConstCharPtr name)

{
   asn1Print_ListPoint (name, &msgData);
}

void asn1Print_OID (ASN1ConstCharPtr name, ASN1T_OID* pvalue)
{
   rtPrintInteger (name, *pvalue);
}

void ASN1C_OID::Print (ASN1ConstCharPtr name)

{
   asn1Print_OID (name, &msgData);
}

void asn1Print_ListOID (ASN1ConstCharPtr name, ASN1T_ListOID* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_OID (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C_ListOID::Print (ASN1ConstCharPtr name)

{
   asn1Print_ListOID (name, &msgData);
}

void asn1Print_TirPion (ASN1ConstCharPtr name, ASN1T_TirPion* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_TirPion::Print (ASN1ConstCharPtr name)

{
   asn1Print_TirPion (name, &msgData);
}

void asn1Print_TirPopulation (ASN1ConstCharPtr name, ASN1T_TirPopulation* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_TirPopulation::Print (ASN1ConstCharPtr name)

{
   asn1Print_TirPopulation (name, &msgData);
}

void asn1Print_Agent (ASN1ConstCharPtr name, ASN1T_Agent* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_Agent::Print (ASN1ConstCharPtr name)

{
   asn1Print_Agent (name, &msgData);
}

void asn1Print_Automate (ASN1ConstCharPtr name, ASN1T_Automate* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_Automate::Print (ASN1ConstCharPtr name)

{
   asn1Print_Automate (name, &msgData);
}

void asn1Print_Population (ASN1ConstCharPtr name, ASN1T_Population* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_Population::Print (ASN1ConstCharPtr name)

{
   asn1Print_Population (name, &msgData);
}

void asn1Print_Camp (ASN1ConstCharPtr name, ASN1T_Camp* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_Camp::Print (ASN1ConstCharPtr name)

{
   asn1Print_Camp (name, &msgData);
}

void asn1Print_Formation (ASN1ConstCharPtr name, ASN1T_Formation* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_Formation::Print (ASN1ConstCharPtr name)

{
   asn1Print_Formation (name, &msgData);
}

void asn1Print_KnowledgeGroup (ASN1ConstCharPtr name, ASN1T_KnowledgeGroup* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_KnowledgeGroup::Print (ASN1ConstCharPtr name)

{
   asn1Print_KnowledgeGroup (name, &msgData);
}

void asn1Print_KnowledgeAgent (ASN1ConstCharPtr name, ASN1T_KnowledgeAgent* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_KnowledgeAgent::Print (ASN1ConstCharPtr name)

{
   asn1Print_KnowledgeAgent (name, &msgData);
}

void asn1Print_KnowledgeObject (ASN1ConstCharPtr name, ASN1T_KnowledgeObject* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_KnowledgeObject::Print (ASN1ConstCharPtr name)

{
   asn1Print_KnowledgeObject (name, &msgData);
}

void asn1Print_KnowledgePopulation (ASN1ConstCharPtr name, ASN1T_KnowledgePopulation* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_KnowledgePopulation::Print (ASN1ConstCharPtr name)

{
   asn1Print_KnowledgePopulation (name, &msgData);
}

void asn1Print_ListAutomate (ASN1ConstCharPtr name, ASN1T_ListAutomate* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_Automate (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C_ListAutomate::Print (ASN1ConstCharPtr name)

{
   asn1Print_ListAutomate (name, &msgData);
}

void asn1Print_ListAgent (ASN1ConstCharPtr name, ASN1T_ListAgent* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_Agent (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C_ListAgent::Print (ASN1ConstCharPtr name)

{
   asn1Print_ListAgent (name, &msgData);
}

void asn1Print_ListKnowledgeAgent (ASN1ConstCharPtr name, ASN1T_ListKnowledgeAgent* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_KnowledgeAgent (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C_ListKnowledgeAgent::Print (ASN1ConstCharPtr name)

{
   asn1Print_ListKnowledgeAgent (name, &msgData);
}

void asn1Print_ListKnowledgeObject (ASN1ConstCharPtr name, ASN1T_ListKnowledgeObject* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_KnowledgeObject (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C_ListKnowledgeObject::Print (ASN1ConstCharPtr name)

{
   asn1Print_ListKnowledgeObject (name, &msgData);
}

void asn1Print_ListCamp (ASN1ConstCharPtr name, ASN1T_ListCamp* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_Camp (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C_ListCamp::Print (ASN1ConstCharPtr name)

{
   asn1Print_ListCamp (name, &msgData);
}

void asn1Print_ListPopulation (ASN1ConstCharPtr name, ASN1T_ListPopulation* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_Population (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C_ListPopulation::Print (ASN1ConstCharPtr name)

{
   asn1Print_ListPopulation (name, &msgData);
}

void asn1Print_ListFormation (ASN1ConstCharPtr name, ASN1T_ListFormation* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_Formation (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C_ListFormation::Print (ASN1ConstCharPtr name)

{
   asn1Print_ListFormation (name, &msgData);
}

void asn1Print_TypeEquipement (ASN1ConstCharPtr name, ASN1T_TypeEquipement* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_TypeEquipement::Print (ASN1ConstCharPtr name)

{
   asn1Print_TypeEquipement (name, &msgData);
}

void asn1Print_TypeDotation (ASN1ConstCharPtr name, ASN1T_TypeDotation* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_TypeDotation::Print (ASN1ConstCharPtr name)

{
   asn1Print_TypeDotation (name, &msgData);
}

void asn1Print_TypeDotationTrancheD (ASN1ConstCharPtr name, ASN1T_TypeDotationTrancheD* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_TypeDotationTrancheD::Print (ASN1ConstCharPtr name)

{
   asn1Print_TypeDotationTrancheD (name, &msgData);
}

void asn1Print_TypePanne (ASN1ConstCharPtr name, ASN1T_TypePanne* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_TypePanne::Print (ASN1ConstCharPtr name)

{
   asn1Print_TypePanne (name, &msgData);
}

void asn1Print_TypePion (ASN1ConstCharPtr name, ASN1T_TypePion* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_TypePion::Print (ASN1ConstCharPtr name)

{
   asn1Print_TypePion (name, &msgData);
}

void asn1Print_TypeAutomate (ASN1ConstCharPtr name, ASN1T_TypeAutomate* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_TypeAutomate::Print (ASN1ConstCharPtr name)

{
   asn1Print_TypeAutomate (name, &msgData);
}

void asn1Print_TypePopulation (ASN1ConstCharPtr name, ASN1T_TypePopulation* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_TypePopulation::Print (ASN1ConstCharPtr name)

{
   asn1Print_TypePopulation (name, &msgData);
}

void asn1Print_Pourcentage (ASN1ConstCharPtr name, ASN1T_Pourcentage* pvalue)
{
   rtPrintInteger (name, *pvalue);
}

void ASN1C_Pourcentage::Print (ASN1ConstCharPtr name)

{
   asn1Print_Pourcentage (name, &msgData);
}

void asn1Print_PourcentageLibre (ASN1ConstCharPtr name, ASN1T_PourcentageLibre* pvalue)
{
   rtPrintInteger (name, *pvalue);
}

void ASN1C_PourcentageLibre::Print (ASN1ConstCharPtr name)

{
   asn1Print_PourcentageLibre (name, &msgData);
}

void asn1Print_Hauteur (ASN1ConstCharPtr name, ASN1T_Hauteur* pvalue)
{
   rtPrintInteger (name, *pvalue);
}

void ASN1C_Hauteur::Print (ASN1ConstCharPtr name)

{
   asn1Print_Hauteur (name, &msgData);
}

void asn1Print_Altitude (ASN1ConstCharPtr name, ASN1T_Altitude* pvalue)
{
   rtPrintInteger (name, *pvalue);
}

void ASN1C_Altitude::Print (ASN1ConstCharPtr name)

{
   asn1Print_Altitude (name, &msgData);
}

void asn1Print_Vitesse (ASN1ConstCharPtr name, ASN1T_Vitesse* pvalue)
{
   rtPrintInteger (name, *pvalue);
}

void ASN1C_Vitesse::Print (ASN1ConstCharPtr name)

{
   asn1Print_Vitesse (name, &msgData);
}

void asn1Print_Angle (ASN1ConstCharPtr name, ASN1T_Angle* pvalue)
{
   rtPrintInteger (name, *pvalue);
}

void ASN1C_Angle::Print (ASN1ConstCharPtr name)

{
   asn1Print_Angle (name, &msgData);
}

void asn1Print_Direction (ASN1ConstCharPtr name, ASN1T_Direction* pvalue)
{
   rtPrintInteger (name, *pvalue);
}

void ASN1C_Direction::Print (ASN1ConstCharPtr name)

{
   asn1Print_Direction (name, &msgData);
}

void asn1Print_EnumGDH_Qualificatif (ASN1ConstCharPtr name, ASN1T_EnumGDH_Qualificatif* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("at\n"); break;
      case 1: printf ("after\n"); break;
      case 2: printf ("as_of\n"); break;
      case 3: printf ("before\n"); break;
      case 4: printf ("cleared\n"); break;
      case 5: printf ("due\n"); break;
      case 6: printf ("end\n"); break;
      case 7: printf ("off\n"); break;
      case 8: printf ("on\n"); break;
      case 9: printf ("since\n"); break;
      case 10: printf ("start\n"); break;
      case 11: printf ("until\n"); break;
      case 12: printf ("wef\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumGDH_Qualificatif::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumGDH_Qualificatif (name, &msgData);
}

void asn1Print_GDH (ASN1ConstCharPtr name, ASN1T_GDH* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.datation", name);
   rtPrintInteger (namebuf, pvalue->datation);

   sprintf (namebuf, "%s.qualificatif", name);
   asn1Print_EnumGDH_Qualificatif (namebuf, &pvalue->qualificatif);

}

void ASN1C_GDH::Print (ASN1ConstCharPtr name)

{
   asn1Print_GDH (name, &msgData);
}

void asn1Print_EnumFamilleDotation (ASN1ConstCharPtr name, ASN1T_EnumFamilleDotation* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("munition\n"); break;
      case 1: printf ("carburant\n"); break;
      case 2: printf ("explosif\n"); break;
      case 3: printf ("mine\n"); break;
      case 4: printf ("barbele\n"); break;
      case 5: printf ("piece\n"); break;
      case 6: printf ("ration\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumFamilleDotation::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumFamilleDotation (name, &msgData);
}

void asn1Print_EnumFamilleMunition (ASN1ConstCharPtr name, ASN1T_EnumFamilleMunition* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("obus\n"); break;
      case 1: printf ("missile_air\n"); break;
      case 2: printf ("missile_sol\n"); break;
      case 3: printf ("mitraille\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumFamilleMunition::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumFamilleMunition (name, &msgData);
}

void asn1Print_EnumUnitIdentificationLevel (ASN1ConstCharPtr name, ASN1T_EnumUnitIdentificationLevel* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("identifiee\n"); break;
      case 1: printf ("reconnue\n"); break;
      case 2: printf ("detectee\n"); break;
      case 3: printf ("signale\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumUnitIdentificationLevel::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumUnitIdentificationLevel (name, &msgData);
}

void asn1Print_EnumNatureLevel (ASN1ConstCharPtr name, ASN1T_EnumNatureLevel* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("none\n"); break;
      case 1: printf ("o\n"); break;
      case 2: printf ("oo\n"); break;
      case 3: printf ("ooo\n"); break;
      case 4: printf ("i\n"); break;
      case 5: printf ("ii\n"); break;
      case 6: printf ("iii\n"); break;
      case 7: printf ("x\n"); break;
      case 8: printf ("xx\n"); break;
      case 9: printf ("xxx\n"); break;
      case 10: printf ("xxxx\n"); break;
      case 11: printf ("xxxxx\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumNatureLevel::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumNatureLevel (name, &msgData);
}

void asn1Print_NatureAtlas (ASN1ConstCharPtr name, ASN1T_NatureAtlas* pvalue)
{
   rtPrintBitStr (name, pvalue->numbits, pvalue->data, ".");
}

void ASN1C_NatureAtlas::Print (ASN1ConstCharPtr name)

{
   asn1Print_NatureAtlas (name, &msgData);
}

void asn1Print_EnumHumanWound (ASN1ConstCharPtr name, ASN1T_EnumHumanWound* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("non_blesse\n"); break;
      case 1: printf ("mort\n"); break;
      case 2: printf ("blesse_urgence_1\n"); break;
      case 3: printf ("blesse_urgence_2\n"); break;
      case 4: printf ("blesse_urgence_3\n"); break;
      case 5: printf ("blesse_urgence_extreme\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumHumanWound::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumHumanWound (name, &msgData);
}

void asn1Print_EnumHumanRank (ASN1ConstCharPtr name, ASN1T_EnumHumanRank* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("officier\n"); break;
      case 1: printf ("sous_officer\n"); break;
      case 2: printf ("mdr\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumHumanRank::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumHumanRank (name, &msgData);
}


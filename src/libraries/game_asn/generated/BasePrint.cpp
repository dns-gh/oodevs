/* This file was generated by ASN1C V5.31 on 13-Sep-2007 */

#include "Base.h"

void asn1Print_BaseIncludes (ASN1ConstCharPtr name, ASN1T_BaseIncludes* pvalue)
{
}

void ASN1C_BaseIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_BaseIncludes (name, &msgData);
}

void asn1Print_BaseExports (ASN1ConstCharPtr name)
{
}

void ASN1C_BaseExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_BaseExports (name);
}

void asn1Print_EnumLocationType (ASN1ConstCharPtr name, ASN1T_EnumLocationType* pvalue)
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

void ASN1C_EnumLocationType::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumLocationType (name, &msgData);
}

void asn1Print_CoordUTM (ASN1ConstCharPtr name, ASN1T_CoordUTM* pvalue)
{
   rtPrintOctStr (name, pvalue->numocts, pvalue->data, ".");
}

void ASN1C_CoordUTM::Print (ASN1ConstCharPtr name)

{
   asn1Print_CoordUTM (name, &msgData);
}

void asn1Print_CoordUTMList (ASN1ConstCharPtr name, ASN1T_CoordUTMList* pvalue)
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

void ASN1C_CoordUTMList::Print (ASN1ConstCharPtr name)

{
   asn1Print_CoordUTMList (name, &msgData);
}

void asn1Print_Location (ASN1ConstCharPtr name, ASN1T_Location* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.type", name);
   asn1Print_EnumLocationType (namebuf, &pvalue->type);

   sprintf (namebuf, "%s.coordinates", name);
   asn1Print_CoordUTMList (namebuf, &pvalue->coordinates);

}

void ASN1C_Location::Print (ASN1ConstCharPtr name)

{
   asn1Print_Location (name, &msgData);
}

void asn1Print_Point (ASN1ConstCharPtr name, ASN1T_Point* pvalue)
{
   asn1Print_Location (name, pvalue);
}

void ASN1C_Point::Print (ASN1ConstCharPtr name)

{
   asn1Print_Point (name, &msgData);
}

void asn1Print_Line (ASN1ConstCharPtr name, ASN1T_Line* pvalue)
{
   asn1Print_Location (name, pvalue);
}

void ASN1C_Line::Print (ASN1ConstCharPtr name)

{
   asn1Print_Line (name, &msgData);
}

void asn1Print_Path (ASN1ConstCharPtr name, ASN1T_Path* pvalue)
{
   asn1Print_Location (name, pvalue);
}

void ASN1C_Path::Print (ASN1ConstCharPtr name)

{
   asn1Print_Path (name, &msgData);
}

void asn1Print_Polygon (ASN1ConstCharPtr name, ASN1T_Polygon* pvalue)
{
   asn1Print_Location (name, pvalue);
}

void ASN1C_Polygon::Print (ASN1ConstCharPtr name)

{
   asn1Print_Polygon (name, &msgData);
}

void asn1Print_Segment (ASN1ConstCharPtr name, ASN1T_Segment* pvalue)
{
   asn1Print_Location (name, pvalue);
}

void ASN1C_Segment::Print (ASN1ConstCharPtr name)

{
   asn1Print_Segment (name, &msgData);
}

void asn1Print_Circle (ASN1ConstCharPtr name, ASN1T_Circle* pvalue)
{
   asn1Print_Location (name, pvalue);
}

void ASN1C_Circle::Print (ASN1ConstCharPtr name)

{
   asn1Print_Circle (name, &msgData);
}

void asn1Print_Ellipse (ASN1ConstCharPtr name, ASN1T_Ellipse* pvalue)
{
   asn1Print_Location (name, pvalue);
}

void ASN1C_Ellipse::Print (ASN1ConstCharPtr name)

{
   asn1Print_Ellipse (name, &msgData);
}

void asn1Print_Sector (ASN1ConstCharPtr name, ASN1T_Sector* pvalue)
{
   asn1Print_Location (name, pvalue);
}

void ASN1C_Sector::Print (ASN1ConstCharPtr name)

{
   asn1Print_Sector (name, &msgData);
}

void asn1Print_LocationList (ASN1ConstCharPtr name, ASN1T_LocationList* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_Location (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C_LocationList::Print (ASN1ConstCharPtr name)

{
   asn1Print_LocationList (name, &msgData);
}

void asn1Print_PolygonList (ASN1ConstCharPtr name, ASN1T_PolygonList* pvalue)
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

void ASN1C_PolygonList::Print (ASN1ConstCharPtr name)

{
   asn1Print_PolygonList (name, &msgData);
}

void asn1Print_PathList (ASN1ConstCharPtr name, ASN1T_PathList* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_Path (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C_PathList::Print (ASN1ConstCharPtr name)

{
   asn1Print_PathList (name, &msgData);
}

void asn1Print_PointList (ASN1ConstCharPtr name, ASN1T_PointList* pvalue)
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

void ASN1C_PointList::Print (ASN1ConstCharPtr name)

{
   asn1Print_PointList (name, &msgData);
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

void asn1Print_UnitFire (ASN1ConstCharPtr name, ASN1T_UnitFire* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_UnitFire::Print (ASN1ConstCharPtr name)

{
   asn1Print_UnitFire (name, &msgData);
}

void asn1Print_PopulationFire (ASN1ConstCharPtr name, ASN1T_PopulationFire* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_PopulationFire::Print (ASN1ConstCharPtr name)

{
   asn1Print_PopulationFire (name, &msgData);
}

void asn1Print_Unit (ASN1ConstCharPtr name, ASN1T_Unit* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_Unit::Print (ASN1ConstCharPtr name)

{
   asn1Print_Unit (name, &msgData);
}

void asn1Print_Automat (ASN1ConstCharPtr name, ASN1T_Automat* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_Automat::Print (ASN1ConstCharPtr name)

{
   asn1Print_Automat (name, &msgData);
}

void asn1Print_Population (ASN1ConstCharPtr name, ASN1T_Population* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_Population::Print (ASN1ConstCharPtr name)

{
   asn1Print_Population (name, &msgData);
}

void asn1Print_Team (ASN1ConstCharPtr name, ASN1T_Team* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_Team::Print (ASN1ConstCharPtr name)

{
   asn1Print_Team (name, &msgData);
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

void asn1Print_UnitKnowledge (ASN1ConstCharPtr name, ASN1T_UnitKnowledge* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_UnitKnowledge::Print (ASN1ConstCharPtr name)

{
   asn1Print_UnitKnowledge (name, &msgData);
}

void asn1Print_ObjectKnowledge (ASN1ConstCharPtr name, ASN1T_ObjectKnowledge* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_ObjectKnowledge::Print (ASN1ConstCharPtr name)

{
   asn1Print_ObjectKnowledge (name, &msgData);
}

void asn1Print_PopulationKnowledge (ASN1ConstCharPtr name, ASN1T_PopulationKnowledge* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_PopulationKnowledge::Print (ASN1ConstCharPtr name)

{
   asn1Print_PopulationKnowledge (name, &msgData);
}

void asn1Print_AutomatList (ASN1ConstCharPtr name, ASN1T_AutomatList* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_Automat (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C_AutomatList::Print (ASN1ConstCharPtr name)

{
   asn1Print_AutomatList (name, &msgData);
}

void asn1Print_UnitList (ASN1ConstCharPtr name, ASN1T_UnitList* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_Unit (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C_UnitList::Print (ASN1ConstCharPtr name)

{
   asn1Print_UnitList (name, &msgData);
}

void asn1Print_UnitKnowledgeList (ASN1ConstCharPtr name, ASN1T_UnitKnowledgeList* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_UnitKnowledge (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C_UnitKnowledgeList::Print (ASN1ConstCharPtr name)

{
   asn1Print_UnitKnowledgeList (name, &msgData);
}

void asn1Print_ObjectKnowledgeList (ASN1ConstCharPtr name, ASN1T_ObjectKnowledgeList* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_ObjectKnowledge (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C_ObjectKnowledgeList::Print (ASN1ConstCharPtr name)

{
   asn1Print_ObjectKnowledgeList (name, &msgData);
}

void asn1Print_TeamList (ASN1ConstCharPtr name, ASN1T_TeamList* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_Team (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C_TeamList::Print (ASN1ConstCharPtr name)

{
   asn1Print_TeamList (name, &msgData);
}

void asn1Print_PopulationList (ASN1ConstCharPtr name, ASN1T_PopulationList* pvalue)
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

void ASN1C_PopulationList::Print (ASN1ConstCharPtr name)

{
   asn1Print_PopulationList (name, &msgData);
}

void asn1Print_FormationList (ASN1ConstCharPtr name, ASN1T_FormationList* pvalue)
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

void ASN1C_FormationList::Print (ASN1ConstCharPtr name)

{
   asn1Print_FormationList (name, &msgData);
}

void asn1Print_EquipmentType (ASN1ConstCharPtr name, ASN1T_EquipmentType* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_EquipmentType::Print (ASN1ConstCharPtr name)

{
   asn1Print_EquipmentType (name, &msgData);
}

void asn1Print_DotationType (ASN1ConstCharPtr name, ASN1T_DotationType* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_DotationType::Print (ASN1ConstCharPtr name)

{
   asn1Print_DotationType (name, &msgData);
}

void asn1Print_BreakdownType (ASN1ConstCharPtr name, ASN1T_BreakdownType* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_BreakdownType::Print (ASN1ConstCharPtr name)

{
   asn1Print_BreakdownType (name, &msgData);
}

void asn1Print_UnitType (ASN1ConstCharPtr name, ASN1T_UnitType* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_UnitType::Print (ASN1ConstCharPtr name)

{
   asn1Print_UnitType (name, &msgData);
}

void asn1Print_AutomatType (ASN1ConstCharPtr name, ASN1T_AutomatType* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_AutomatType::Print (ASN1ConstCharPtr name)

{
   asn1Print_AutomatType (name, &msgData);
}

void asn1Print_PopulationType (ASN1ConstCharPtr name, ASN1T_PopulationType* pvalue)
{
   asn1Print_OID (name, pvalue);
}

void ASN1C_PopulationType::Print (ASN1ConstCharPtr name)

{
   asn1Print_PopulationType (name, &msgData);
}

void asn1Print_Percentage (ASN1ConstCharPtr name, ASN1T_Percentage* pvalue)
{
   rtPrintInteger (name, *pvalue);
}

void ASN1C_Percentage::Print (ASN1ConstCharPtr name)

{
   asn1Print_Percentage (name, &msgData);
}

void asn1Print_FreePercentage (ASN1ConstCharPtr name, ASN1T_FreePercentage* pvalue)
{
   rtPrintInteger (name, *pvalue);
}

void ASN1C_FreePercentage::Print (ASN1ConstCharPtr name)

{
   asn1Print_FreePercentage (name, &msgData);
}

void asn1Print_Height (ASN1ConstCharPtr name, ASN1T_Height* pvalue)
{
   rtPrintInteger (name, *pvalue);
}

void ASN1C_Height::Print (ASN1ConstCharPtr name)

{
   asn1Print_Height (name, &msgData);
}

void asn1Print_Elevation (ASN1ConstCharPtr name, ASN1T_Elevation* pvalue)
{
   rtPrintInteger (name, *pvalue);
}

void ASN1C_Elevation::Print (ASN1ConstCharPtr name)

{
   asn1Print_Elevation (name, &msgData);
}

void asn1Print_Speed (ASN1ConstCharPtr name, ASN1T_Speed* pvalue)
{
   rtPrintInteger (name, *pvalue);
}

void ASN1C_Speed::Print (ASN1ConstCharPtr name)

{
   asn1Print_Speed (name, &msgData);
}

void asn1Print_Heading (ASN1ConstCharPtr name, ASN1T_Heading* pvalue)
{
   rtPrintInteger (name, *pvalue);
}

void ASN1C_Heading::Print (ASN1ConstCharPtr name)

{
   asn1Print_Heading (name, &msgData);
}

void asn1Print_DateTime (ASN1ConstCharPtr name, ASN1T_DateTime* pvalue)
{
   rtPrintInteger (name, *pvalue);
}

void ASN1C_DateTime::Print (ASN1ConstCharPtr name)

{
   asn1Print_DateTime (name, &msgData);
}

void asn1Print_EnumDotationFamily (ASN1ConstCharPtr name, ASN1T_EnumDotationFamily* pvalue)
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

void ASN1C_EnumDotationFamily::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumDotationFamily (name, &msgData);
}

void asn1Print_EnumAmmunitionFamily (ASN1ConstCharPtr name, ASN1T_EnumAmmunitionFamily* pvalue)
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

void ASN1C_EnumAmmunitionFamily::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumAmmunitionFamily (name, &msgData);
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

void asn1Print_EnumUnitVisibility (ASN1ConstCharPtr name, ASN1T_EnumUnitVisibility* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("invisible\n"); break;
      case 1: printf ("detected\n"); break;
      case 2: printf ("recognized\n"); break;
      case 3: printf ("identified\n"); break;
      case 4: printf ("recorded\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumUnitVisibility::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumUnitVisibility (name, &msgData);
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

void asn1Print_AtlasNature (ASN1ConstCharPtr name, ASN1T_AtlasNature* pvalue)
{
   rtPrintBitStr (name, pvalue->numbits, pvalue->data, ".");
}

void ASN1C_AtlasNature::Print (ASN1ConstCharPtr name)

{
   asn1Print_AtlasNature (name, &msgData);
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

void asn1Print_EnumChangeHierarchyErrorCode (ASN1ConstCharPtr name, ASN1T_EnumChangeHierarchyErrorCode* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("no_error\n"); break;
      case 1: printf ("error_invalid_pion\n"); break;
      case 2: printf ("error_invalid_automate\n"); break;
      case 3: printf ("error_invalid_formation\n"); break;
      case 4: printf ("error_invalid_automate_tc2\n"); break;
      case 5: printf ("error_invalid_automate_maintenance\n"); break;
      case 6: printf ("error_invalid_automate_sante\n"); break;
      case 7: printf ("error_invalid_automate_ravitaillement\n"); break;
      case 8: printf ("error_unit_surrendered\n"); break;
      case 9: printf ("error_invalid_camp\n"); break;
      case 10: printf ("error_invalid_groupe_connaissance\n"); break;
      case 11: printf ("error_camps_incompatibles\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumChangeHierarchyErrorCode::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumChangeHierarchyErrorCode (name, &msgData);
}


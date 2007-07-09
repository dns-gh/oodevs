/* This file was generated by ASN1C V5.31 on 09-Jul-2007 */

#include "Order.h"

void asn1Print_OrderIncludes (ASN1ConstCharPtr name, ASN1T_OrderIncludes* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.baseExports", name);

   sprintf (namebuf, "%s.limaLimiteExports", name);

   sprintf (namebuf, "%s.logMaintenanceExports", name);

   sprintf (namebuf, "%s.logSanteExports", name);

   sprintf (namebuf, "%s.objectExports", name);

}

void ASN1C_OrderIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_OrderIncludes (name, &msgData);
}

void asn1Print_OrderExports (ASN1ConstCharPtr name)
{
}

void ASN1C_OrderExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_OrderExports (name);
}

void asn1Print_EnumOrderErrorCode (ASN1ConstCharPtr name, ASN1T_EnumOrderErrorCode* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("no_error\n"); break;
      case 1: printf ("error_invalid_unit\n"); break;
      case 2: printf ("error_invalid_limit\n"); break;
      case 3: printf ("error_invalid_lima\n"); break;
      case 4: printf ("error_invalid_mission\n"); break;
      case 5: printf ("error_invalid_mission_parameters\n"); break;
      case 6: printf ("error_unit_cannot_receive_order\n"); break;
      case 7: printf ("error_invalid_order_conduite\n"); break;
      case 8: printf ("error_invalid_order_mission\n"); break;
      case 9: printf ("error_invalid_order_initial\n"); break;
      case 10: printf ("error_invalid_order_conduite_parameters\n"); break;
      case 11: printf ("error_unit_surrendered\n"); break;
      case 12: printf ("error_invalid_lima_function\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumOrderErrorCode::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumOrderErrorCode (name, &msgData);
}

void asn1Print_OrderContext (ASN1ConstCharPtr name, ASN1T_OrderContext* pvalue)
{
   char namebuf[512];

   printf ("%s.m.limite_gauchePresent = %d\n", name, (int)pvalue->m.limite_gauchePresent);
   printf ("%s.m.limite_droitePresent = %d\n", name, (int)pvalue->m.limite_droitePresent);
   if (pvalue->m.limite_gauchePresent) {
      sprintf (namebuf, "%s.limite_gauche", name);
      asn1Print_Line (namebuf, &pvalue->limite_gauche);
   }

   if (pvalue->m.limite_droitePresent) {
      sprintf (namebuf, "%s.limite_droite", name);
      asn1Print_Line (namebuf, &pvalue->limite_droite);
   }

   sprintf (namebuf, "%s.limas", name);
   asn1Print_LimasOrder (namebuf, &pvalue->limas);

   sprintf (namebuf, "%s.direction_dangereuse", name);
   asn1Print_Heading (namebuf, &pvalue->direction_dangereuse);

}

void ASN1C_OrderContext::Print (ASN1ConstCharPtr name)

{
   asn1Print_OrderContext (name, &msgData);
}

void asn1Print_PlannedWork (ASN1ConstCharPtr name, ASN1T_PlannedWork* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.type", name);
   asn1Print_EnumObjectType (namebuf, &pvalue->type);

   sprintf (namebuf, "%s.position", name);
   asn1Print_Location (namebuf, &pvalue->position);

   sprintf (namebuf, "%s.type_obstacle", name);
   asn1Print_EnumObstacleType (namebuf, &pvalue->type_obstacle);

   sprintf (namebuf, "%s.densite", name);
   rtPrintReal (namebuf, pvalue->densite);

   sprintf (namebuf, "%s.tc2", name);
   asn1Print_Automat (namebuf, &pvalue->tc2);

   sprintf (namebuf, "%s.activity_time", name);
   rtPrintInteger (namebuf, pvalue->activity_time);

}

void ASN1C_PlannedWork::Print (ASN1ConstCharPtr name)

{
   asn1Print_PlannedWork (name, &msgData);
}

void asn1Print_PlannedWorkList (ASN1ConstCharPtr name, ASN1T_PlannedWorkList* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_PlannedWork (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C_PlannedWorkList::Print (ASN1ConstCharPtr name)

{
   asn1Print_PlannedWorkList (name, &msgData);
}

void asn1Print_MissionParameter_value (ASN1ConstCharPtr name, ASN1T_MissionParameter_value* pvalue)
{
   char namebuf[512];

   printf ("%s.t = %d\n", name, pvalue->t);
   switch (pvalue->t) {
      case 1:
         sprintf (namebuf, "%s.u.aBool", name);
         rtPrintBoolean (namebuf, pvalue->u.aBool);
         break;

      case 2:
         sprintf (namebuf, "%s.u.aReal", name);
         rtPrintReal (namebuf, pvalue->u.aReal);
         break;

      case 3:
         sprintf (namebuf, "%s.u.enumeration", name);
         rtPrintInteger (namebuf, pvalue->u.enumeration);
         break;

      case 4:
         sprintf (namebuf, "%s.u.path", name);
         asn1Print_Path (namebuf, pvalue->u.path);
         break;

      case 5:
         sprintf (namebuf, "%s.u.pathList", name);
         asn1Print_PathList (namebuf, pvalue->u.pathList);
         break;

      case 6:
         sprintf (namebuf, "%s.u.point", name);
         asn1Print_Point (namebuf, pvalue->u.point);
         break;

      case 7:
         sprintf (namebuf, "%s.u.pointList", name);
         asn1Print_PointList (namebuf, pvalue->u.pointList);
         break;

      case 8:
         sprintf (namebuf, "%s.u.polygon", name);
         asn1Print_Polygon (namebuf, pvalue->u.polygon);
         break;

      case 9:
         sprintf (namebuf, "%s.u.polygonList", name);
         asn1Print_PolygonList (namebuf, pvalue->u.polygonList);
         break;

      case 10:
         sprintf (namebuf, "%s.u.location", name);
         asn1Print_Location (namebuf, pvalue->u.location);
         break;

      case 11:
         sprintf (namebuf, "%s.u.locationList", name);
         asn1Print_LocationList (namebuf, pvalue->u.locationList);
         break;

      case 12:
         sprintf (namebuf, "%s.u.heading", name);
         asn1Print_Heading (namebuf, &pvalue->u.heading);
         break;

      case 13:
         sprintf (namebuf, "%s.u.atlasNature", name);
         asn1Print_AtlasNature (namebuf, pvalue->u.atlasNature);
         break;

      case 14:
         sprintf (namebuf, "%s.u.unit", name);
         asn1Print_Unit (namebuf, &pvalue->u.unit);
         break;

      case 15:
         sprintf (namebuf, "%s.u.unitList", name);
         asn1Print_UnitList (namebuf, pvalue->u.unitList);
         break;

      case 16:
         sprintf (namebuf, "%s.u.automat", name);
         asn1Print_Automat (namebuf, &pvalue->u.automat);
         break;

      case 17:
         sprintf (namebuf, "%s.u.automatList", name);
         asn1Print_AutomatList (namebuf, pvalue->u.automatList);
         break;

      case 18:
         sprintf (namebuf, "%s.u.unitKnowledge", name);
         asn1Print_UnitKnowledge (namebuf, &pvalue->u.unitKnowledge);
         break;

      case 19:
         sprintf (namebuf, "%s.u.unitKnowledgeList", name);
         asn1Print_UnitKnowledgeList (namebuf, pvalue->u.unitKnowledgeList);
         break;

      case 20:
         sprintf (namebuf, "%s.u.objectKnowledge", name);
         asn1Print_ObjectKnowledge (namebuf, &pvalue->u.objectKnowledge);
         break;

      case 21:
         sprintf (namebuf, "%s.u.objectKnowledgeList", name);
         asn1Print_ObjectKnowledgeList (namebuf, pvalue->u.objectKnowledgeList);
         break;

      case 22:
         sprintf (namebuf, "%s.u.populationKnowledge", name);
         asn1Print_PopulationKnowledge (namebuf, &pvalue->u.populationKnowledge);
         break;

      case 23:
         sprintf (namebuf, "%s.u.plannedWork", name);
         asn1Print_PlannedWork (namebuf, pvalue->u.plannedWork);
         break;

      case 24:
         sprintf (namebuf, "%s.u.plannedWorkList", name);
         asn1Print_PlannedWorkList (namebuf, pvalue->u.plannedWorkList);
         break;

      case 25:
         sprintf (namebuf, "%s.u.dotationType", name);
         asn1Print_DotationType (namebuf, &pvalue->u.dotationType);
         break;

      case 26:
         sprintf (namebuf, "%s.u.equipmentType", name);
         asn1Print_EquipmentType (namebuf, &pvalue->u.equipmentType);
         break;

      case 27:
         sprintf (namebuf, "%s.u.tirIndirect", name);
         asn1Print_UnitFire (namebuf, &pvalue->u.tirIndirect);
         break;

      case 28:
         sprintf (namebuf, "%s.u.dateTime", name);
         asn1Print_DateTime (namebuf, &pvalue->u.dateTime);
         break;

      case 29:
         sprintf (namebuf, "%s.u.logMaintenancePriorities", name);
         asn1Print_LogMaintenancePriorities (namebuf, pvalue->u.logMaintenancePriorities);
         break;

      case 30:
         sprintf (namebuf, "%s.u.logMedicalPriorities", name);
         asn1Print_LogMedicalPriorities (namebuf, pvalue->u.logMedicalPriorities);
         break;

      case 31:
         sprintf (namebuf, "%s.u.aCharStr", name);
         rtPrintCharStr (namebuf, pvalue->u.aCharStr);
         break;

      default:;
   }
}

void ASN1C_MissionParameter_value::Print (ASN1ConstCharPtr name)

{
   asn1Print_MissionParameter_value (name, &msgData);
}

void asn1Print_MissionParameter (ASN1ConstCharPtr name, ASN1T_MissionParameter* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.null_value", name);
   rtPrintBoolean (namebuf, pvalue->null_value);

   sprintf (namebuf, "%s.value", name);
   asn1Print_MissionParameter_value (namebuf, &pvalue->value);

}

void ASN1C_MissionParameter::Print (ASN1ConstCharPtr name)

{
   asn1Print_MissionParameter (name, &msgData);
}

void asn1Print_MissionParameters (ASN1ConstCharPtr name, ASN1T_MissionParameters* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_MissionParameter (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C_MissionParameters::Print (ASN1ConstCharPtr name)

{
   asn1Print_MissionParameters (name, &msgData);
}

void asn1Print_MsgUnitOrder (ASN1ConstCharPtr name, ASN1T_MsgUnitOrder* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_Unit (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.order_context", name);
   asn1Print_OrderContext (namebuf, &pvalue->order_context);

   sprintf (namebuf, "%s.mission", name);
   asn1Print_OID (namebuf, &pvalue->mission);

   sprintf (namebuf, "%s.parametres", name);
   asn1Print_MissionParameters (namebuf, &pvalue->parametres);

}

void ASN1C_MsgUnitOrder::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgUnitOrder (name, &msgData);
}

void asn1Print_MsgUnitOrderAck (ASN1ConstCharPtr name, ASN1T_MsgUnitOrderAck* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_Unit (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.error_code", name);
   asn1Print_EnumOrderErrorCode (namebuf, &pvalue->error_code);

}

void ASN1C_MsgUnitOrderAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgUnitOrderAck (name, &msgData);
}

void asn1Print_EnumAutomatOrderFormation (ASN1ConstCharPtr name, ASN1T_EnumAutomatOrderFormation* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("un_echelon\n"); break;
      case 1: printf ("deux_echelons\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumAutomatOrderFormation::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumAutomatOrderFormation (name, &msgData);
}

void asn1Print_MsgAutomatOrder (ASN1ConstCharPtr name, ASN1T_MsgAutomatOrder* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_Automat (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.order_context", name);
   asn1Print_OrderContext (namebuf, &pvalue->order_context);

   sprintf (namebuf, "%s.mission", name);
   asn1Print_OID (namebuf, &pvalue->mission);

   sprintf (namebuf, "%s.parametres", name);
   asn1Print_MissionParameters (namebuf, &pvalue->parametres);

   sprintf (namebuf, "%s.formation", name);
   asn1Print_EnumAutomatOrderFormation (namebuf, &pvalue->formation);

}

void ASN1C_MsgAutomatOrder::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgAutomatOrder (name, &msgData);
}

void asn1Print_MsgAutomatOrderAck (ASN1ConstCharPtr name, ASN1T_MsgAutomatOrderAck* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_Automat (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.error_code", name);
   asn1Print_EnumOrderErrorCode (namebuf, &pvalue->error_code);

}

void ASN1C_MsgAutomatOrderAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgAutomatOrderAck (name, &msgData);
}

void asn1Print_MsgPopulationOrder (ASN1ConstCharPtr name, ASN1T_MsgPopulationOrder* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_Population (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.mission", name);
   asn1Print_OID (namebuf, &pvalue->mission);

   sprintf (namebuf, "%s.parametres", name);
   asn1Print_MissionParameters (namebuf, &pvalue->parametres);

}

void ASN1C_MsgPopulationOrder::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgPopulationOrder (name, &msgData);
}

void asn1Print_MsgPopulationOrderAck (ASN1ConstCharPtr name, ASN1T_MsgPopulationOrderAck* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_Population (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.error_code", name);
   asn1Print_EnumOrderErrorCode (namebuf, &pvalue->error_code);

}

void ASN1C_MsgPopulationOrderAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgPopulationOrderAck (name, &msgData);
}

void asn1Print_MsgFragOrder (ASN1ConstCharPtr name, ASN1T_MsgFragOrder* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.frag_order", name);
   asn1Print_OID (namebuf, &pvalue->frag_order);

   sprintf (namebuf, "%s.parametres", name);
   asn1Print_MissionParameters (namebuf, &pvalue->parametres);

}

void ASN1C_MsgFragOrder::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgFragOrder (name, &msgData);
}

void asn1Print_MsgFragOrderAck (ASN1ConstCharPtr name, ASN1T_MsgFragOrderAck* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.error_code", name);
   asn1Print_EnumOrderErrorCode (namebuf, &pvalue->error_code);

}

void ASN1C_MsgFragOrderAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgFragOrderAck (name, &msgData);
}

void asn1Print_EnumReportType (ASN1ConstCharPtr name, ASN1T_EnumReportType* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("message\n"); break;
      case 1: printf ("operationel\n"); break;
      case 2: printf ("evenement_exceptionnel\n"); break;
      case 3: printf ("warning\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumReportType::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumReportType (name, &msgData);
}

void asn1Print_MsgReport (ASN1ConstCharPtr name, ASN1T_MsgReport* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.cr", name);
   asn1Print_OID (namebuf, &pvalue->cr);

   sprintf (namebuf, "%s.type", name);
   asn1Print_EnumReportType (namebuf, &pvalue->type);

   sprintf (namebuf, "%s.parametres", name);
   asn1Print_MissionParameters (namebuf, &pvalue->parametres);

}

void ASN1C_MsgReport::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgReport (name, &msgData);
}

void asn1Print_MsgTrace (ASN1ConstCharPtr name, ASN1T_MsgTrace* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.message", name);
   rtPrintCharStr (namebuf, pvalue->message);

}

void ASN1C_MsgTrace::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgTrace (name, &msgData);
}

void asn1Print_MsgDecisionalState (ASN1ConstCharPtr name, ASN1T_MsgDecisionalState* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.key", name);
   rtPrintCharStr (namebuf, pvalue->key);

   sprintf (namebuf, "%s.value", name);
   rtPrintCharStr (namebuf, pvalue->value);

}

void ASN1C_MsgDecisionalState::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgDecisionalState (name, &msgData);
}

void asn1Print_MsgDebugPoints (ASN1ConstCharPtr name, ASN1T_MsgDebugPoints* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.coordinates", name);
   asn1Print_CoordUTMList (namebuf, &pvalue->coordinates);

}

void ASN1C_MsgDebugPoints::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgDebugPoints (name, &msgData);
}


/* This file was generated by ASN1C V5.31 on 21-May-2008 */

#include "Drawing.h"

void asn1Print_DrawingIncludes (ASN1ConstCharPtr name, ASN1T_DrawingIncludes* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.baseExports", name);

}

void ASN1C_DrawingIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_DrawingIncludes (name, &msgData);
}

void asn1Print_DrawingExports (ASN1ConstCharPtr name)
{
}

void ASN1C_DrawingExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_DrawingExports (name);
}

void asn1Print_Shape (ASN1ConstCharPtr name, ASN1T_Shape* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.category", name);
   rtPrintCharStr (namebuf, pvalue->category);

   sprintf (namebuf, "%s.color", name);
   rtPrintCharStr (namebuf, pvalue->color);

   sprintf (namebuf, "%s.template_", name);
   rtPrintCharStr (namebuf, pvalue->template_);

   sprintf (namebuf, "%s.points", name);
   asn1Print_CoordLatLongList (namebuf, &pvalue->points);

}

void ASN1C_Shape::Print (ASN1ConstCharPtr name)

{
   asn1Print_Shape (name, &msgData);
}

void asn1Print_ShapeList (ASN1ConstCharPtr name, ASN1T_ShapeList* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_Shape (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C_ShapeList::Print (ASN1ConstCharPtr name)

{
   asn1Print_ShapeList (name, &msgData);
}

void asn1Print_MsgShapeCreation (ASN1ConstCharPtr name, ASN1T_MsgShapeCreation* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.shape", name);
   asn1Print_Shape (namebuf, &pvalue->shape);

}

void ASN1C_MsgShapeCreation::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgShapeCreation (name, &msgData);
}

void asn1Print_MsgShapeUpdate (ASN1ConstCharPtr name, ASN1T_MsgShapeUpdate* pvalue)
{
   char namebuf[512];

   printf ("%s.m.categoryPresent = %d\n", name, (int)pvalue->m.categoryPresent);
   printf ("%s.m.colorPresent = %d\n", name, (int)pvalue->m.colorPresent);
   printf ("%s.m.template_Present = %d\n", name, (int)pvalue->m.template_Present);
   printf ("%s.m.typePresent = %d\n", name, (int)pvalue->m.typePresent);
   printf ("%s.m.pointsPresent = %d\n", name, (int)pvalue->m.pointsPresent);
   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   if (pvalue->m.categoryPresent) {
      sprintf (namebuf, "%s.category", name);
      rtPrintCharStr (namebuf, pvalue->category);
   }

   if (pvalue->m.colorPresent) {
      sprintf (namebuf, "%s.color", name);
      rtPrintCharStr (namebuf, pvalue->color);
   }

   if (pvalue->m.template_Present) {
      sprintf (namebuf, "%s.template_", name);
      rtPrintCharStr (namebuf, pvalue->template_);
   }

   if (pvalue->m.typePresent) {
      sprintf (namebuf, "%s.type", name);
      asn1Print_EnumLocationType (namebuf, &pvalue->type);
   }

   if (pvalue->m.pointsPresent) {
      sprintf (namebuf, "%s.points", name);
      asn1Print_CoordLatLongList (namebuf, &pvalue->points);
   }

}

void ASN1C_MsgShapeUpdate::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgShapeUpdate (name, &msgData);
}

void asn1Print_MsgShapeDestruction (ASN1ConstCharPtr name, ASN1T_MsgShapeDestruction* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

}

void ASN1C_MsgShapeDestruction::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgShapeDestruction (name, &msgData);
}

void asn1Print_MsgShapeCreationRequest (ASN1ConstCharPtr name, ASN1T_MsgShapeCreationRequest* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.shape", name);
   asn1Print_Shape (namebuf, &pvalue->shape);

}

void ASN1C_MsgShapeCreationRequest::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgShapeCreationRequest (name, &msgData);
}

void asn1Print_MsgShapeUpdateRequest (ASN1ConstCharPtr name, ASN1T_MsgShapeUpdateRequest* pvalue)
{
   char namebuf[512];

   printf ("%s.m.categoryPresent = %d\n", name, (int)pvalue->m.categoryPresent);
   printf ("%s.m.colorPresent = %d\n", name, (int)pvalue->m.colorPresent);
   printf ("%s.m.template_Present = %d\n", name, (int)pvalue->m.template_Present);
   printf ("%s.m.typePresent = %d\n", name, (int)pvalue->m.typePresent);
   printf ("%s.m.pointsPresent = %d\n", name, (int)pvalue->m.pointsPresent);
   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   if (pvalue->m.categoryPresent) {
      sprintf (namebuf, "%s.category", name);
      rtPrintCharStr (namebuf, pvalue->category);
   }

   if (pvalue->m.colorPresent) {
      sprintf (namebuf, "%s.color", name);
      rtPrintCharStr (namebuf, pvalue->color);
   }

   if (pvalue->m.template_Present) {
      sprintf (namebuf, "%s.template_", name);
      rtPrintCharStr (namebuf, pvalue->template_);
   }

   if (pvalue->m.typePresent) {
      sprintf (namebuf, "%s.type", name);
      asn1Print_EnumLocationType (namebuf, &pvalue->type);
   }

   if (pvalue->m.pointsPresent) {
      sprintf (namebuf, "%s.points", name);
      asn1Print_CoordLatLongList (namebuf, &pvalue->points);
   }

}

void ASN1C_MsgShapeUpdateRequest::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgShapeUpdateRequest (name, &msgData);
}

void asn1Print_MsgShapeDestructionRequest (ASN1ConstCharPtr name, ASN1T_MsgShapeDestructionRequest* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

}

void ASN1C_MsgShapeDestructionRequest::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgShapeDestructionRequest (name, &msgData);
}

void asn1Print_EnumShapeErrorCode (ASN1ConstCharPtr name, ASN1T_EnumShapeErrorCode* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("no_error\n"); break;
      case 1: printf ("error_invalid_oid\n"); break;
      case 2: printf ("error_invalid_geometry\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumShapeErrorCode::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumShapeErrorCode (name, &msgData);
}

void asn1Print_MsgShapeCreationRequestAck (ASN1ConstCharPtr name, ASN1T_MsgShapeCreationRequestAck* pvalue)
{
   asn1Print_EnumShapeErrorCode (name, pvalue);
}

void ASN1C_MsgShapeCreationRequestAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgShapeCreationRequestAck (name, &msgData);
}

void asn1Print_MsgShapeUpdateRequestAck (ASN1ConstCharPtr name, ASN1T_MsgShapeUpdateRequestAck* pvalue)
{
   asn1Print_EnumShapeErrorCode (name, pvalue);
}

void ASN1C_MsgShapeUpdateRequestAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgShapeUpdateRequestAck (name, &msgData);
}

void asn1Print_MsgShapeDestructionRequestAck (ASN1ConstCharPtr name, ASN1T_MsgShapeDestructionRequestAck* pvalue)
{
   asn1Print_EnumShapeErrorCode (name, pvalue);
}

void ASN1C_MsgShapeDestructionRequestAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgShapeDestructionRequestAck (name, &msgData);
}


/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 21-May-2008.
 */
#include "Drawing.h"

/**************************************************************/
/*                                                            */
/*  DrawingIncludes                                           */
/*                                                            */
/**************************************************************/

ASN1C_DrawingIncludes::ASN1C_DrawingIncludes (
   ASN1MessageBuffer& msgBuf, ASN1T_DrawingIncludes& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_DrawingIncludes (ASN1CTXT* ctxt_p, ASN1T_DrawingIncludes* pvalue)
{
   int stat = ASN_OK;

   /* encode baseExports */

   stat = asn1PE_BaseExports (ctxt_p);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_DrawingIncludes::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_DrawingIncludes (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_DrawingIncludes (ASN1CTXT* ctxt_p, ASN1T_DrawingIncludes* pvalue)
{
   int stat = ASN_OK;

   /* decode baseExports */

   stat = asn1PD_BaseExports (ctxt_p);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_DrawingIncludes::Decode ()
{
   return asn1PD_DrawingIncludes (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  DrawingExports                                            */
/*                                                            */
/**************************************************************/

ASN1C_DrawingExports::ASN1C_DrawingExports (
   ASN1MessageBuffer& msgBuf) :
   ASN1CType(msgBuf)
{}

EXTERN int asn1PE_DrawingExports (ASN1CTXT* ctxt_p)
{
   int stat = ASN_OK;

   /* NULL */
   return (stat);
}

int ASN1C_DrawingExports::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_DrawingExports (mpContext->GetPtr());
   return stat;
}

EXTERN int asn1PD_DrawingExports (ASN1CTXT* ctxt_p)
{
   int stat = ASN_OK;

   /* NULL */

   return (stat);
}

int ASN1C_DrawingExports::Decode ()
{
   return asn1PD_DrawingExports (mpContext->GetPtr());
}

/**************************************************************/
/*                                                            */
/*  Shape                                                     */
/*                                                            */
/**************************************************************/

ASN1C_Shape::ASN1C_Shape (
   ASN1MessageBuffer& msgBuf, ASN1T_Shape& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Shape (ASN1CTXT* ctxt_p, ASN1T_Shape* pvalue)
{
   int stat = ASN_OK;

   /* encode category */

   stat = pe_VisibleString (ctxt_p, pvalue->category, 0);
   if (stat != ASN_OK) return stat;

   /* encode color */

   stat = pe_VisibleString (ctxt_p, pvalue->color, 0);
   if (stat != ASN_OK) return stat;

   /* encode template_ */

   stat = pe_VisibleString (ctxt_p, pvalue->template_, 0);
   if (stat != ASN_OK) return stat;

   /* encode points */

   stat = asn1PE_CoordLatLongList (ctxt_p, &pvalue->points);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_Shape::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Shape (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Shape (ASN1CTXT* ctxt_p, ASN1T_Shape* pvalue)
{
   int stat = ASN_OK;

   /* decode category */

   stat = pd_VisibleString (ctxt_p, &pvalue->category, 0);
   if (stat != ASN_OK) return stat;

   /* decode color */

   stat = pd_VisibleString (ctxt_p, &pvalue->color, 0);
   if (stat != ASN_OK) return stat;

   /* decode template_ */

   stat = pd_VisibleString (ctxt_p, &pvalue->template_, 0);
   if (stat != ASN_OK) return stat;

   /* decode points */

   stat = asn1PD_CoordLatLongList (ctxt_p, &pvalue->points);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_Shape::Decode ()
{
   return asn1PD_Shape (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  ShapeList                                                 */
/*                                                            */
/**************************************************************/

ASN1C_ShapeList::ASN1C_ShapeList (
   ASN1MessageBuffer& msgBuf, ASN1T_ShapeList& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_ShapeList (ASN1CTXT* ctxt_p, ASN1T_ShapeList* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* encode length determinant */

   stat = pe_Length (ctxt_p, pvalue->n);
   if (stat < 0) return stat;

   /* encode elements */

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      stat = asn1PE_Shape (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return stat;
   }
   return (stat);
}

int ASN1C_ShapeList::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_ShapeList (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_ShapeList (ASN1CTXT* ctxt_p, ASN1T_ShapeList* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = pd_Length (ctxt_p, &pvalue->n);
   if (stat != ASN_OK) return stat;

   /* decode elements */

   ALLOC_ASN1ARRAY (ctxt_p, pvalue, ASN1T_Shape);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      stat = asn1PD_Shape (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return stat;
   }


   return (stat);
}

int ASN1C_ShapeList::Decode ()
{
   return asn1PD_ShapeList (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgShapeCreation                                          */
/*                                                            */
/**************************************************************/

ASN1C_MsgShapeCreation::ASN1C_MsgShapeCreation (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgShapeCreation& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgShapeCreation (ASN1CTXT* ctxt_p, ASN1T_MsgShapeCreation* pvalue)
{
   int stat = ASN_OK;

   /* encode oid */

   stat = asn1PE_OID (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* encode shape */

   stat = asn1PE_Shape (ctxt_p, &pvalue->shape);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgShapeCreation::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgShapeCreation (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgShapeCreation (ASN1CTXT* ctxt_p, ASN1T_MsgShapeCreation* pvalue)
{
   int stat = ASN_OK;

   /* decode oid */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* decode shape */

   stat = asn1PD_Shape (ctxt_p, &pvalue->shape);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgShapeCreation::Decode ()
{
   return asn1PD_MsgShapeCreation (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgShapeUpdate                                            */
/*                                                            */
/**************************************************************/

ASN1C_MsgShapeUpdate::ASN1C_MsgShapeUpdate (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgShapeUpdate& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgShapeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgShapeUpdate* pvalue)
{
   int stat = ASN_OK;

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.categoryPresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.colorPresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.template_Present);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.typePresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.pointsPresent);

   /* encode oid */

   stat = asn1PE_OID (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* encode category */

   if (pvalue->m.categoryPresent) {
      stat = pe_VisibleString (ctxt_p, pvalue->category, 0);
      if (stat != ASN_OK) return stat;
   }

   /* encode color */

   if (pvalue->m.colorPresent) {
      stat = pe_VisibleString (ctxt_p, pvalue->color, 0);
      if (stat != ASN_OK) return stat;
   }

   /* encode template_ */

   if (pvalue->m.template_Present) {
      stat = pe_VisibleString (ctxt_p, pvalue->template_, 0);
      if (stat != ASN_OK) return stat;
   }

   /* encode type */

   if (pvalue->m.typePresent) {
      stat = asn1PE_EnumLocationType (ctxt_p, pvalue->type);
      if (stat != ASN_OK) return stat;
   }

   /* encode points */

   if (pvalue->m.pointsPresent) {
      stat = asn1PE_CoordLatLongList (ctxt_p, &pvalue->points);
      if (stat != ASN_OK) return stat;
   }

   return (stat);
}

int ASN1C_MsgShapeUpdate::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgShapeUpdate (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgShapeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgShapeUpdate* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL optbit;

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   pd_bit (ctxt_p, &optbit);
   pvalue->m.categoryPresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.colorPresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.template_Present = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.typePresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.pointsPresent = optbit;

   /* decode oid */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* decode category */

   if (pvalue->m.categoryPresent) {
      stat = pd_VisibleString (ctxt_p, &pvalue->category, 0);
      if (stat != ASN_OK) return stat;
   }

   /* decode color */

   if (pvalue->m.colorPresent) {
      stat = pd_VisibleString (ctxt_p, &pvalue->color, 0);
      if (stat != ASN_OK) return stat;
   }

   /* decode template_ */

   if (pvalue->m.template_Present) {
      stat = pd_VisibleString (ctxt_p, &pvalue->template_, 0);
      if (stat != ASN_OK) return stat;
   }

   /* decode type */

   if (pvalue->m.typePresent) {
      stat = asn1PD_EnumLocationType (ctxt_p, &pvalue->type);
      if (stat != ASN_OK) return stat;
   }

   /* decode points */

   if (pvalue->m.pointsPresent) {
      stat = asn1PD_CoordLatLongList (ctxt_p, &pvalue->points);
      if (stat != ASN_OK) return stat;
   }


   return (stat);
}

int ASN1C_MsgShapeUpdate::Decode ()
{
   return asn1PD_MsgShapeUpdate (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgShapeDestruction                                       */
/*                                                            */
/**************************************************************/

ASN1C_MsgShapeDestruction::ASN1C_MsgShapeDestruction (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgShapeDestruction& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgShapeDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgShapeDestruction* pvalue)
{
   int stat = ASN_OK;

   /* encode oid */

   stat = asn1PE_OID (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgShapeDestruction::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgShapeDestruction (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgShapeDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgShapeDestruction* pvalue)
{
   int stat = ASN_OK;

   /* decode oid */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgShapeDestruction::Decode ()
{
   return asn1PD_MsgShapeDestruction (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgShapeCreationRequest                                   */
/*                                                            */
/**************************************************************/

ASN1C_MsgShapeCreationRequest::ASN1C_MsgShapeCreationRequest (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgShapeCreationRequest& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgShapeCreationRequest (ASN1CTXT* ctxt_p, ASN1T_MsgShapeCreationRequest* pvalue)
{
   int stat = ASN_OK;

   /* encode shape */

   stat = asn1PE_Shape (ctxt_p, &pvalue->shape);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgShapeCreationRequest::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgShapeCreationRequest (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgShapeCreationRequest (ASN1CTXT* ctxt_p, ASN1T_MsgShapeCreationRequest* pvalue)
{
   int stat = ASN_OK;

   /* decode shape */

   stat = asn1PD_Shape (ctxt_p, &pvalue->shape);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgShapeCreationRequest::Decode ()
{
   return asn1PD_MsgShapeCreationRequest (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgShapeUpdateRequest                                     */
/*                                                            */
/**************************************************************/

ASN1C_MsgShapeUpdateRequest::ASN1C_MsgShapeUpdateRequest (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgShapeUpdateRequest& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgShapeUpdateRequest (ASN1CTXT* ctxt_p, ASN1T_MsgShapeUpdateRequest* pvalue)
{
   int stat = ASN_OK;

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.categoryPresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.colorPresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.template_Present);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.typePresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.pointsPresent);

   /* encode oid */

   stat = asn1PE_OID (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* encode category */

   if (pvalue->m.categoryPresent) {
      stat = pe_VisibleString (ctxt_p, pvalue->category, 0);
      if (stat != ASN_OK) return stat;
   }

   /* encode color */

   if (pvalue->m.colorPresent) {
      stat = pe_VisibleString (ctxt_p, pvalue->color, 0);
      if (stat != ASN_OK) return stat;
   }

   /* encode template_ */

   if (pvalue->m.template_Present) {
      stat = pe_VisibleString (ctxt_p, pvalue->template_, 0);
      if (stat != ASN_OK) return stat;
   }

   /* encode type */

   if (pvalue->m.typePresent) {
      stat = asn1PE_EnumLocationType (ctxt_p, pvalue->type);
      if (stat != ASN_OK) return stat;
   }

   /* encode points */

   if (pvalue->m.pointsPresent) {
      stat = asn1PE_CoordLatLongList (ctxt_p, &pvalue->points);
      if (stat != ASN_OK) return stat;
   }

   return (stat);
}

int ASN1C_MsgShapeUpdateRequest::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgShapeUpdateRequest (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgShapeUpdateRequest (ASN1CTXT* ctxt_p, ASN1T_MsgShapeUpdateRequest* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL optbit;

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   pd_bit (ctxt_p, &optbit);
   pvalue->m.categoryPresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.colorPresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.template_Present = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.typePresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.pointsPresent = optbit;

   /* decode oid */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* decode category */

   if (pvalue->m.categoryPresent) {
      stat = pd_VisibleString (ctxt_p, &pvalue->category, 0);
      if (stat != ASN_OK) return stat;
   }

   /* decode color */

   if (pvalue->m.colorPresent) {
      stat = pd_VisibleString (ctxt_p, &pvalue->color, 0);
      if (stat != ASN_OK) return stat;
   }

   /* decode template_ */

   if (pvalue->m.template_Present) {
      stat = pd_VisibleString (ctxt_p, &pvalue->template_, 0);
      if (stat != ASN_OK) return stat;
   }

   /* decode type */

   if (pvalue->m.typePresent) {
      stat = asn1PD_EnumLocationType (ctxt_p, &pvalue->type);
      if (stat != ASN_OK) return stat;
   }

   /* decode points */

   if (pvalue->m.pointsPresent) {
      stat = asn1PD_CoordLatLongList (ctxt_p, &pvalue->points);
      if (stat != ASN_OK) return stat;
   }


   return (stat);
}

int ASN1C_MsgShapeUpdateRequest::Decode ()
{
   return asn1PD_MsgShapeUpdateRequest (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgShapeDestructionRequest                                */
/*                                                            */
/**************************************************************/

ASN1C_MsgShapeDestructionRequest::ASN1C_MsgShapeDestructionRequest (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgShapeDestructionRequest& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgShapeDestructionRequest (ASN1CTXT* ctxt_p, ASN1T_MsgShapeDestructionRequest* pvalue)
{
   int stat = ASN_OK;

   /* encode oid */

   stat = asn1PE_OID (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgShapeDestructionRequest::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgShapeDestructionRequest (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgShapeDestructionRequest (ASN1CTXT* ctxt_p, ASN1T_MsgShapeDestructionRequest* pvalue)
{
   int stat = ASN_OK;

   /* decode oid */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgShapeDestructionRequest::Decode ()
{
   return asn1PD_MsgShapeDestructionRequest (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  EnumShapeErrorCode                                        */
/*                                                            */
/**************************************************************/

ASN1C_EnumShapeErrorCode::ASN1C_EnumShapeErrorCode (
   ASN1MessageBuffer& msgBuf, ASN1T_EnumShapeErrorCode& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_EnumShapeErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumShapeErrorCode value)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   switch (value) {
      case EnumShapeErrorCode::no_error: ui = 0; break;
      case EnumShapeErrorCode::error_invalid_oid: ui = 1; break;
      case EnumShapeErrorCode::error_invalid_geometry: ui = 2; break;
      default: return ASN_E_INVENUM;
   }

   stat = pe_ConsUnsigned (ctxt_p, ui, 0, 2);
   if (stat != ASN_OK) return stat;
   return (stat);
}

int ASN1C_EnumShapeErrorCode::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_EnumShapeErrorCode (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_EnumShapeErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumShapeErrorCode* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   stat = pd_ConsUnsigned (ctxt_p, &ui, 0, 2);
   if (stat != ASN_OK) return stat;

   switch (ui) {
      case 0: *pvalue = EnumShapeErrorCode::no_error; break;
      case 1: *pvalue = EnumShapeErrorCode::error_invalid_oid; break;
      case 2: *pvalue = EnumShapeErrorCode::error_invalid_geometry; break;
      default: return ASN_E_INVENUM;
   }

   return (stat);
}

int ASN1C_EnumShapeErrorCode::Decode ()
{
   return asn1PD_EnumShapeErrorCode (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgShapeCreationRequestAck                                */
/*                                                            */
/**************************************************************/

ASN1C_MsgShapeCreationRequestAck::ASN1C_MsgShapeCreationRequestAck (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgShapeCreationRequestAck& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgShapeCreationRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgShapeCreationRequestAck value)
{
   int stat = ASN_OK;

   stat = asn1PE_EnumShapeErrorCode (ctxt_p, value);
   if (stat != ASN_OK) return stat;
   return (stat);
}

int ASN1C_MsgShapeCreationRequestAck::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgShapeCreationRequestAck (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_MsgShapeCreationRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgShapeCreationRequestAck* pvalue)
{
   int stat = ASN_OK;

   stat = asn1PD_EnumShapeErrorCode (ctxt_p, pvalue);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgShapeCreationRequestAck::Decode ()
{
   return asn1PD_MsgShapeCreationRequestAck (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgShapeUpdateRequestAck                                  */
/*                                                            */
/**************************************************************/

ASN1C_MsgShapeUpdateRequestAck::ASN1C_MsgShapeUpdateRequestAck (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgShapeUpdateRequestAck& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgShapeUpdateRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgShapeUpdateRequestAck value)
{
   int stat = ASN_OK;

   stat = asn1PE_EnumShapeErrorCode (ctxt_p, value);
   if (stat != ASN_OK) return stat;
   return (stat);
}

int ASN1C_MsgShapeUpdateRequestAck::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgShapeUpdateRequestAck (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_MsgShapeUpdateRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgShapeUpdateRequestAck* pvalue)
{
   int stat = ASN_OK;

   stat = asn1PD_EnumShapeErrorCode (ctxt_p, pvalue);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgShapeUpdateRequestAck::Decode ()
{
   return asn1PD_MsgShapeUpdateRequestAck (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgShapeDestructionRequestAck                             */
/*                                                            */
/**************************************************************/

ASN1C_MsgShapeDestructionRequestAck::ASN1C_MsgShapeDestructionRequestAck (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgShapeDestructionRequestAck& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgShapeDestructionRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgShapeDestructionRequestAck value)
{
   int stat = ASN_OK;

   stat = asn1PE_EnumShapeErrorCode (ctxt_p, value);
   if (stat != ASN_OK) return stat;
   return (stat);
}

int ASN1C_MsgShapeDestructionRequestAck::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgShapeDestructionRequestAck (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_MsgShapeDestructionRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgShapeDestructionRequestAck* pvalue)
{
   int stat = ASN_OK;

   stat = asn1PD_EnumShapeErrorCode (ctxt_p, pvalue);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgShapeDestructionRequestAck::Decode ()
{
   return asn1PD_MsgShapeDestructionRequestAck (mpContext->GetPtr(), &msgData);
}


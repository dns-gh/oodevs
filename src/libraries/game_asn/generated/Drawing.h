/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 09-Jul-2009.
 */
#ifndef DRAWING_H
#define DRAWING_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "Base.h"

/**************************************************************/
/*                                                            */
/*  DrawingIncludes                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_DrawingIncludes {
} ASN1T_DrawingIncludes;

class EXTERN ASN1C_DrawingIncludes : public ASN1CType {
public:
   ASN1T_DrawingIncludes& msgData;
   ASN1C_DrawingIncludes (
      ASN1MessageBuffer& msgBuf, ASN1T_DrawingIncludes& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_DrawingIncludes (ASN1CTXT* ctxt_p, ASN1T_DrawingIncludes* pvalue);
EXTERN int asn1PD_DrawingIncludes (ASN1CTXT* ctxt_p, ASN1T_DrawingIncludes* pvalue);

EXTERN void asn1Print_DrawingIncludes (ASN1ConstCharPtr name, ASN1T_DrawingIncludes* pvalue);

/**************************************************************/
/*                                                            */
/*  DrawingExports                                            */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_DrawingExports : public ASN1CType {
public:
   ASN1C_DrawingExports (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_DrawingExports (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_DrawingExports (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_DrawingExports (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  Shape                                                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Shape {
   ASN1VisibleString  category;
   ASN1VisibleString  color;
   ASN1VisibleString  template_;
   ASN1T_CoordLatLongList  points;
} ASN1T_Shape;

class EXTERN ASN1C_Shape : public ASN1CType {
public:
   ASN1T_Shape& msgData;
   ASN1C_Shape (
      ASN1MessageBuffer& msgBuf, ASN1T_Shape& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Shape (ASN1CTXT* ctxt_p, ASN1T_Shape* pvalue);
EXTERN int asn1PD_Shape (ASN1CTXT* ctxt_p, ASN1T_Shape* pvalue);

EXTERN void asn1Print_Shape (ASN1ConstCharPtr name, ASN1T_Shape* pvalue);

/**************************************************************/
/*                                                            */
/*  ShapeList                                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ShapeList {
   ASN1UINT n;
   ASN1T_Shape *elem;
} ASN1T_ShapeList;

class EXTERN ASN1C_ShapeList : public ASN1CType {
public:
   ASN1T_ShapeList& msgData;
   ASN1C_ShapeList (
      ASN1MessageBuffer& msgBuf, ASN1T_ShapeList& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ShapeList (ASN1CTXT* ctxt_p, ASN1T_ShapeList* pvalue);
EXTERN int asn1PD_ShapeList (ASN1CTXT* ctxt_p, ASN1T_ShapeList* pvalue);

EXTERN void asn1Print_ShapeList (ASN1ConstCharPtr name, ASN1T_ShapeList* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgShapeCreation                                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgShapeCreation {
   ASN1T_OID  oid;
   ASN1T_Shape  shape;
} ASN1T_MsgShapeCreation;

class EXTERN ASN1C_MsgShapeCreation : public ASN1CType {
public:
   ASN1T_MsgShapeCreation& msgData;
   ASN1C_MsgShapeCreation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgShapeCreation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgShapeCreation (ASN1CTXT* ctxt_p, ASN1T_MsgShapeCreation* pvalue);
EXTERN int asn1PD_MsgShapeCreation (ASN1CTXT* ctxt_p, ASN1T_MsgShapeCreation* pvalue);

EXTERN void asn1Print_MsgShapeCreation (ASN1ConstCharPtr name, ASN1T_MsgShapeCreation* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgShapeUpdate                                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgShapeUpdate {
   struct {
      unsigned categoryPresent : 1;
      unsigned colorPresent : 1;
      unsigned template_Present : 1;
      unsigned pointsPresent : 1;
   } m;
   ASN1T_OID  oid;
   ASN1VisibleString  category;
   ASN1VisibleString  color;
   ASN1VisibleString  template_;
   ASN1T_CoordLatLongList  points;

   ASN1T_MsgShapeUpdate () {
      m.categoryPresent = 0;
      m.colorPresent = 0;
      m.template_Present = 0;
      m.pointsPresent = 0;
   }
} ASN1T_MsgShapeUpdate;

class EXTERN ASN1C_MsgShapeUpdate : public ASN1CType {
public:
   ASN1T_MsgShapeUpdate& msgData;
   ASN1C_MsgShapeUpdate (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgShapeUpdate& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgShapeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgShapeUpdate* pvalue);
EXTERN int asn1PD_MsgShapeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgShapeUpdate* pvalue);

EXTERN void asn1Print_MsgShapeUpdate (ASN1ConstCharPtr name, ASN1T_MsgShapeUpdate* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgShapeDestruction                                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgShapeDestruction {
   ASN1T_OID  oid;
} ASN1T_MsgShapeDestruction;

class EXTERN ASN1C_MsgShapeDestruction : public ASN1CType {
public:
   ASN1T_MsgShapeDestruction& msgData;
   ASN1C_MsgShapeDestruction (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgShapeDestruction& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgShapeDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgShapeDestruction* pvalue);
EXTERN int asn1PD_MsgShapeDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgShapeDestruction* pvalue);

EXTERN void asn1Print_MsgShapeDestruction (ASN1ConstCharPtr name, ASN1T_MsgShapeDestruction* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgShapeCreationRequest                                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgShapeCreationRequest {
   ASN1T_Shape  shape;
} ASN1T_MsgShapeCreationRequest;

class EXTERN ASN1C_MsgShapeCreationRequest : public ASN1CType {
public:
   ASN1T_MsgShapeCreationRequest& msgData;
   ASN1C_MsgShapeCreationRequest (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgShapeCreationRequest& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgShapeCreationRequest (ASN1CTXT* ctxt_p, ASN1T_MsgShapeCreationRequest* pvalue);
EXTERN int asn1PD_MsgShapeCreationRequest (ASN1CTXT* ctxt_p, ASN1T_MsgShapeCreationRequest* pvalue);

EXTERN void asn1Print_MsgShapeCreationRequest (ASN1ConstCharPtr name, ASN1T_MsgShapeCreationRequest* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgShapeUpdateRequest                                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgShapeUpdateRequest {
   struct {
      unsigned categoryPresent : 1;
      unsigned colorPresent : 1;
      unsigned template_Present : 1;
      unsigned pointsPresent : 1;
   } m;
   ASN1T_OID  oid;
   ASN1VisibleString  category;
   ASN1VisibleString  color;
   ASN1VisibleString  template_;
   ASN1T_CoordLatLongList  points;

   ASN1T_MsgShapeUpdateRequest () {
      m.categoryPresent = 0;
      m.colorPresent = 0;
      m.template_Present = 0;
      m.pointsPresent = 0;
   }
} ASN1T_MsgShapeUpdateRequest;

class EXTERN ASN1C_MsgShapeUpdateRequest : public ASN1CType {
public:
   ASN1T_MsgShapeUpdateRequest& msgData;
   ASN1C_MsgShapeUpdateRequest (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgShapeUpdateRequest& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgShapeUpdateRequest (ASN1CTXT* ctxt_p, ASN1T_MsgShapeUpdateRequest* pvalue);
EXTERN int asn1PD_MsgShapeUpdateRequest (ASN1CTXT* ctxt_p, ASN1T_MsgShapeUpdateRequest* pvalue);

EXTERN void asn1Print_MsgShapeUpdateRequest (ASN1ConstCharPtr name, ASN1T_MsgShapeUpdateRequest* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgShapeDestructionRequest                                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgShapeDestructionRequest {
   ASN1T_OID  oid;
} ASN1T_MsgShapeDestructionRequest;

class EXTERN ASN1C_MsgShapeDestructionRequest : public ASN1CType {
public:
   ASN1T_MsgShapeDestructionRequest& msgData;
   ASN1C_MsgShapeDestructionRequest (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgShapeDestructionRequest& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgShapeDestructionRequest (ASN1CTXT* ctxt_p, ASN1T_MsgShapeDestructionRequest* pvalue);
EXTERN int asn1PD_MsgShapeDestructionRequest (ASN1CTXT* ctxt_p, ASN1T_MsgShapeDestructionRequest* pvalue);

EXTERN void asn1Print_MsgShapeDestructionRequest (ASN1ConstCharPtr name, ASN1T_MsgShapeDestructionRequest* pvalue);

/**************************************************************/
/*                                                            */
/*  EnumShapeErrorCode                                        */
/*                                                            */
/**************************************************************/

struct EXTERN EnumShapeErrorCode {
   enum Root {
      no_error = 0,
      error_invalid_oid = 1,
      error_invalid_geometry = 2
   } ;
} ;

typedef EnumShapeErrorCode::Root ASN1T_EnumShapeErrorCode;

class EXTERN ASN1C_EnumShapeErrorCode : public ASN1CType {
public:
   ASN1T_EnumShapeErrorCode& msgData;
   ASN1C_EnumShapeErrorCode (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumShapeErrorCode& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumShapeErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumShapeErrorCode value);
EXTERN int asn1PD_EnumShapeErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumShapeErrorCode* pvalue);

EXTERN void asn1Print_EnumShapeErrorCode (ASN1ConstCharPtr name, ASN1T_EnumShapeErrorCode* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgShapeCreationRequestAck                                */
/*                                                            */
/**************************************************************/

typedef ASN1T_EnumShapeErrorCode ASN1T_MsgShapeCreationRequestAck;

class EXTERN ASN1C_MsgShapeCreationRequestAck : public ASN1CType {
public:
   ASN1T_MsgShapeCreationRequestAck& msgData;
   ASN1C_MsgShapeCreationRequestAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgShapeCreationRequestAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgShapeCreationRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgShapeCreationRequestAck value);
EXTERN int asn1PD_MsgShapeCreationRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgShapeCreationRequestAck* pvalue);

EXTERN void asn1Print_MsgShapeCreationRequestAck (ASN1ConstCharPtr name, ASN1T_MsgShapeCreationRequestAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgShapeUpdateRequestAck                                  */
/*                                                            */
/**************************************************************/

typedef ASN1T_EnumShapeErrorCode ASN1T_MsgShapeUpdateRequestAck;

class EXTERN ASN1C_MsgShapeUpdateRequestAck : public ASN1CType {
public:
   ASN1T_MsgShapeUpdateRequestAck& msgData;
   ASN1C_MsgShapeUpdateRequestAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgShapeUpdateRequestAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgShapeUpdateRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgShapeUpdateRequestAck value);
EXTERN int asn1PD_MsgShapeUpdateRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgShapeUpdateRequestAck* pvalue);

EXTERN void asn1Print_MsgShapeUpdateRequestAck (ASN1ConstCharPtr name, ASN1T_MsgShapeUpdateRequestAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgShapeDestructionRequestAck                             */
/*                                                            */
/**************************************************************/

typedef ASN1T_EnumShapeErrorCode ASN1T_MsgShapeDestructionRequestAck;

class EXTERN ASN1C_MsgShapeDestructionRequestAck : public ASN1CType {
public:
   ASN1T_MsgShapeDestructionRequestAck& msgData;
   ASN1C_MsgShapeDestructionRequestAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgShapeDestructionRequestAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgShapeDestructionRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgShapeDestructionRequestAck value);
EXTERN int asn1PD_MsgShapeDestructionRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgShapeDestructionRequestAck* pvalue);

EXTERN void asn1Print_MsgShapeDestructionRequestAck (ASN1ConstCharPtr name, ASN1T_MsgShapeDestructionRequestAck* pvalue);

#endif

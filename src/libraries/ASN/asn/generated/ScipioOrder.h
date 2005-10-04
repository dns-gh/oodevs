/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 04-Oct-2005.
 */
#ifndef SCIPIOORDER_H
#define SCIPIOORDER_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  EnumOrderErrorCode                                        */
/*                                                            */
/**************************************************************/

struct EXTERN EnumOrderErrorCode {
   enum Root {
      no_error = 0,
      error_invalid_unit = 1,
      error_invalid_limit = 2,
      error_invalid_lima = 3,
      error_invalid_mission = 4,
      error_invalid_mission_parameters = 5,
      error_unit_cannot_receive_order = 6,
      error_invalid_order_conduite = 7,
      error_invalid_order_mission = 8,
      error_invalid_order_initial = 9,
      error_invalid_order_conduite_parameters = 10,
      error_unit_surrendered = 11
   } ;
} ;

typedef EnumOrderErrorCode::Root ASN1T_EnumOrderErrorCode;

class EXTERN ASN1C_EnumOrderErrorCode : public ASN1CType {
public:
   ASN1T_EnumOrderErrorCode& msgData;
   ASN1C_EnumOrderErrorCode (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumOrderErrorCode& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumOrderErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumOrderErrorCode value);
EXTERN int asn1PD_EnumOrderErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumOrderErrorCode* pvalue);

EXTERN void asn1Print_EnumOrderErrorCode (ASN1ConstCharPtr name, ASN1T_EnumOrderErrorCode* pvalue);

/**************************************************************/
/*                                                            */
/*  EnumOrderState                                            */
/*                                                            */
/**************************************************************/

struct EXTERN EnumOrderState {
   enum Root {
      started = 0,
      cancelled = 1,
      stopped = 2
   } ;
} ;

typedef EnumOrderState::Root ASN1T_EnumOrderState;

class EXTERN ASN1C_EnumOrderState : public ASN1CType {
public:
   ASN1T_EnumOrderState& msgData;
   ASN1C_EnumOrderState (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumOrderState& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumOrderState (ASN1CTXT* ctxt_p, ASN1T_EnumOrderState value);
EXTERN int asn1PD_EnumOrderState (ASN1CTXT* ctxt_p, ASN1T_EnumOrderState* pvalue);

EXTERN void asn1Print_EnumOrderState (ASN1ConstCharPtr name, ASN1T_EnumOrderState* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgOrderManagement                                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgOrderManagement {
   ASN1T_OID  order_id;
   ASN1T_EnumOrderState  etat;
} ASN1T_MsgOrderManagement;

class EXTERN ASN1C_MsgOrderManagement : public ASN1CType {
public:
   ASN1T_MsgOrderManagement& msgData;
   ASN1C_MsgOrderManagement (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgOrderManagement& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgOrderManagement (ASN1CTXT* ctxt_p, ASN1T_MsgOrderManagement* pvalue);
EXTERN int asn1PD_MsgOrderManagement (ASN1CTXT* ctxt_p, ASN1T_MsgOrderManagement* pvalue);

EXTERN void asn1Print_MsgOrderManagement (ASN1ConstCharPtr name, ASN1T_MsgOrderManagement* pvalue);

#endif

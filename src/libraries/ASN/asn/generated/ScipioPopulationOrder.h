/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 03-Nov-2005.
 */
#ifndef SCIPIOPOPULATIONORDER_H
#define SCIPIOPOPULATIONORDER_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"
#include "ScipioOrder.h"
#include "ScipioPopulationMission.h"

/**************************************************************/
/*                                                            */
/*  MsgPopulationOrderAck                                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationOrderAck {
   ASN1T_Population  oid_unite_executante;
   ASN1T_OID  order_id;
   ASN1T_EnumOrderErrorCode  error_code;
} ASN1T_MsgPopulationOrderAck;

class EXTERN ASN1C_MsgPopulationOrderAck : public ASN1CType {
public:
   ASN1T_MsgPopulationOrderAck& msgData;
   ASN1C_MsgPopulationOrderAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationOrderAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPopulationOrderAck (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationOrderAck* pvalue);
EXTERN int asn1PD_MsgPopulationOrderAck (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationOrderAck* pvalue);

EXTERN void asn1Print_MsgPopulationOrderAck (ASN1ConstCharPtr name, ASN1T_MsgPopulationOrderAck* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Population                                        */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_Mission_Population_mission_population_faire_mouvement 1

typedef struct EXTERN ASN1T_Mission_Population {
   int t;
   union {
      /* t = 1 */
      ASN1T_Mission_Population_FaireMouvement *mission_population_faire_mouvement;
   } u;
} ASN1T_Mission_Population;

class EXTERN ASN1C_Mission_Population : public ASN1CType {
public:
   ASN1T_Mission_Population& msgData;
   ASN1C_Mission_Population (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Population& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Population (ASN1CTXT* ctxt_p, ASN1T_Mission_Population* pvalue);
EXTERN int asn1PD_Mission_Population (ASN1CTXT* ctxt_p, ASN1T_Mission_Population* pvalue);

EXTERN void asn1Print_Mission_Population (ASN1ConstCharPtr name, ASN1T_Mission_Population* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPopulationOrder                                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationOrder {
   ASN1T_Population  oid_unite_executante;
   ASN1T_OID  order_id;
   ASN1T_Mission_Population  mission;
} ASN1T_MsgPopulationOrder;

class EXTERN ASN1C_MsgPopulationOrder : public ASN1CType {
public:
   ASN1T_MsgPopulationOrder& msgData;
   ASN1C_MsgPopulationOrder (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationOrder& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPopulationOrder (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationOrder* pvalue);
EXTERN int asn1PD_MsgPopulationOrder (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationOrder* pvalue);

EXTERN void asn1Print_MsgPopulationOrder (ASN1ConstCharPtr name, ASN1T_MsgPopulationOrder* pvalue);

#endif

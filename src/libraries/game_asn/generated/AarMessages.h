/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 28-Feb-2008.
 */
#ifndef AARMESSAGES_H
#define AARMESSAGES_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "Aar.h"

/**************************************************************/
/*                                                            */
/*  AarMessagesIncludes                                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_AarMessagesIncludes {
} ASN1T_AarMessagesIncludes;

class EXTERN ASN1C_AarMessagesIncludes : public ASN1CType {
public:
   ASN1T_AarMessagesIncludes& msgData;
   ASN1C_AarMessagesIncludes (
      ASN1MessageBuffer& msgBuf, ASN1T_AarMessagesIncludes& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_AarMessagesIncludes (ASN1CTXT* ctxt_p, ASN1T_AarMessagesIncludes* pvalue);
EXTERN int asn1PD_AarMessagesIncludes (ASN1CTXT* ctxt_p, ASN1T_AarMessagesIncludes* pvalue);

EXTERN void asn1Print_AarMessagesIncludes (ASN1ConstCharPtr name, ASN1T_AarMessagesIncludes* pvalue);

/**************************************************************/
/*                                                            */
/*  AarMessagesExports                                        */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_AarMessagesExports : public ASN1CType {
public:
   ASN1C_AarMessagesExports (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_AarMessagesExports (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_AarMessagesExports (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_AarMessagesExports (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgsAarToClient_msg                                       */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_MsgsAarToClient_msg_msg_aar_information 1
#define T_MsgsAarToClient_msg_msg_indicator_result 2

typedef struct EXTERN ASN1T_MsgsAarToClient_msg {
   int t;
   union {
      /* t = 1 */
      ASN1T_MsgAarInformation *msg_aar_information;
      /* t = 2 */
      ASN1T_MsgIndicatorResult *msg_indicator_result;
   } u;
} ASN1T_MsgsAarToClient_msg;

class EXTERN ASN1C_MsgsAarToClient_msg : public ASN1CType {
public:
   ASN1T_MsgsAarToClient_msg& msgData;
   ASN1C_MsgsAarToClient_msg (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgsAarToClient_msg& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgsAarToClient_msg (ASN1CTXT* ctxt_p, ASN1T_MsgsAarToClient_msg* pvalue);
EXTERN int asn1PD_MsgsAarToClient_msg (ASN1CTXT* ctxt_p, ASN1T_MsgsAarToClient_msg* pvalue);

EXTERN void asn1Print_MsgsAarToClient_msg (ASN1ConstCharPtr name, ASN1T_MsgsAarToClient_msg* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgsAarToClient                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgsAarToClient {
   ASN1INT   context;
   ASN1T_MsgsAarToClient_msg  msg;

   ASN1T_MsgsAarToClient () {
      context = 0;
   }
} ASN1T_MsgsAarToClient;

class EXTERN ASN1C_MsgsAarToClient : public ASN1CType {
public:
   ASN1T_MsgsAarToClient& msgData;
   ASN1C_MsgsAarToClient (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgsAarToClient& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgsAarToClient (ASN1CTXT* ctxt_p, ASN1T_MsgsAarToClient* pvalue);
EXTERN int asn1PD_MsgsAarToClient (ASN1CTXT* ctxt_p, ASN1T_MsgsAarToClient* pvalue);

EXTERN void asn1Print_MsgsAarToClient (ASN1ConstCharPtr name, ASN1T_MsgsAarToClient* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgsClientToAar_msg                                       */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_MsgsClientToAar_msg_msg_indicator_request 1

typedef struct EXTERN ASN1T_MsgsClientToAar_msg {
   int t;
   union {
      /* t = 1 */
      ASN1T_MsgIndicatorRequest *msg_indicator_request;
   } u;
} ASN1T_MsgsClientToAar_msg;

class EXTERN ASN1C_MsgsClientToAar_msg : public ASN1CType {
public:
   ASN1T_MsgsClientToAar_msg& msgData;
   ASN1C_MsgsClientToAar_msg (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgsClientToAar_msg& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgsClientToAar_msg (ASN1CTXT* ctxt_p, ASN1T_MsgsClientToAar_msg* pvalue);
EXTERN int asn1PD_MsgsClientToAar_msg (ASN1CTXT* ctxt_p, ASN1T_MsgsClientToAar_msg* pvalue);

EXTERN void asn1Print_MsgsClientToAar_msg (ASN1ConstCharPtr name, ASN1T_MsgsClientToAar_msg* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgsClientToAar                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgsClientToAar {
   ASN1INT   context;
   ASN1T_MsgsClientToAar_msg  msg;

   ASN1T_MsgsClientToAar () {
      context = 0;
   }
} ASN1T_MsgsClientToAar;

class EXTERN ASN1C_MsgsClientToAar : public ASN1CType {
public:
   ASN1T_MsgsClientToAar& msgData;
   ASN1C_MsgsClientToAar (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgsClientToAar& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgsClientToAar (ASN1CTXT* ctxt_p, ASN1T_MsgsClientToAar* pvalue);
EXTERN int asn1PD_MsgsClientToAar (ASN1CTXT* ctxt_p, ASN1T_MsgsClientToAar* pvalue);

EXTERN void asn1Print_MsgsClientToAar (ASN1ConstCharPtr name, ASN1T_MsgsClientToAar* pvalue);

#endif

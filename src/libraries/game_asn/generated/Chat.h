/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 06-Jan-2010.
 */
#ifndef CHAT_H
#define CHAT_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "Base.h"

/**************************************************************/
/*                                                            */
/*  ChatIncludes                                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ChatIncludes {
} ASN1T_ChatIncludes;

class EXTERN ASN1C_ChatIncludes : public ASN1CType {
public:
   ASN1T_ChatIncludes& msgData;
   ASN1C_ChatIncludes (
      ASN1MessageBuffer& msgBuf, ASN1T_ChatIncludes& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ChatIncludes (ASN1CTXT* ctxt_p, ASN1T_ChatIncludes* pvalue);
EXTERN int asn1PD_ChatIncludes (ASN1CTXT* ctxt_p, ASN1T_ChatIncludes* pvalue);

EXTERN void asn1Print_ChatIncludes (ASN1ConstCharPtr name, ASN1T_ChatIncludes* pvalue);

/**************************************************************/
/*                                                            */
/*  ChatExports                                               */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_ChatExports : public ASN1CType {
public:
   ASN1C_ChatExports (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ChatExports (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_ChatExports (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_ChatExports (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  ChatTarget                                                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ChatTarget {
   ASN1VisibleString  profile;
} ASN1T_ChatTarget;

class EXTERN ASN1C_ChatTarget : public ASN1CType {
public:
   ASN1T_ChatTarget& msgData;
   ASN1C_ChatTarget (
      ASN1MessageBuffer& msgBuf, ASN1T_ChatTarget& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ChatTarget (ASN1CTXT* ctxt_p, ASN1T_ChatTarget* pvalue);
EXTERN int asn1PD_ChatTarget (ASN1CTXT* ctxt_p, ASN1T_ChatTarget* pvalue);

EXTERN void asn1Print_ChatTarget (ASN1ConstCharPtr name, ASN1T_ChatTarget* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgTextMessage                                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgTextMessage {
   ASN1T_ChatTarget  source;
   ASN1T_ChatTarget  target;
   ASN1VisibleString  message;
} ASN1T_MsgTextMessage;

class EXTERN ASN1C_MsgTextMessage : public ASN1CType {
public:
   ASN1T_MsgTextMessage& msgData;
   ASN1C_MsgTextMessage (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgTextMessage& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgTextMessage (ASN1CTXT* ctxt_p, ASN1T_MsgTextMessage* pvalue);
EXTERN int asn1PD_MsgTextMessage (ASN1CTXT* ctxt_p, ASN1T_MsgTextMessage* pvalue);

EXTERN void asn1Print_MsgTextMessage (ASN1ConstCharPtr name, ASN1T_MsgTextMessage* pvalue);

#endif

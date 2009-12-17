/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 17-Dec-2009.
 */
#ifndef KNOWLEDGEGROUP_H
#define KNOWLEDGEGROUP_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "Base.h"

/**************************************************************/
/*                                                            */
/*  KnowledgeGroupIncludes                                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_KnowledgeGroupIncludes {
} ASN1T_KnowledgeGroupIncludes;

class EXTERN ASN1C_KnowledgeGroupIncludes : public ASN1CType {
public:
   ASN1T_KnowledgeGroupIncludes& msgData;
   ASN1C_KnowledgeGroupIncludes (
      ASN1MessageBuffer& msgBuf, ASN1T_KnowledgeGroupIncludes& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_KnowledgeGroupIncludes (ASN1CTXT* ctxt_p, ASN1T_KnowledgeGroupIncludes* pvalue);
EXTERN int asn1PD_KnowledgeGroupIncludes (ASN1CTXT* ctxt_p, ASN1T_KnowledgeGroupIncludes* pvalue);

EXTERN void asn1Print_KnowledgeGroupIncludes (ASN1ConstCharPtr name, ASN1T_KnowledgeGroupIncludes* pvalue);

/**************************************************************/
/*                                                            */
/*  KnowledgeGroupExports                                     */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_KnowledgeGroupExports : public ASN1CType {
public:
   ASN1C_KnowledgeGroupExports (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_KnowledgeGroupExports (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_KnowledgeGroupExports (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_KnowledgeGroupExports (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgKnowledgeGroupCreation                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgKnowledgeGroupCreation {
   struct {
      unsigned oid_knowledgegroup_parentPresent : 1;
   } m;
   ASN1T_KnowledgeGroup  oid;
   ASN1T_Team  oid_camp;
   ASN1T_KnowledgeGroup  oid_knowledgegroup_parent;

   ASN1T_MsgKnowledgeGroupCreation () {
      m.oid_knowledgegroup_parentPresent = 0;
   }
} ASN1T_MsgKnowledgeGroupCreation;

class EXTERN ASN1C_MsgKnowledgeGroupCreation : public ASN1CType {
public:
   ASN1T_MsgKnowledgeGroupCreation& msgData;
   ASN1C_MsgKnowledgeGroupCreation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgKnowledgeGroupCreation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgKnowledgeGroupCreation (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupCreation* pvalue);
EXTERN int asn1PD_MsgKnowledgeGroupCreation (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupCreation* pvalue);

EXTERN void asn1Print_MsgKnowledgeGroupCreation (ASN1ConstCharPtr name, ASN1T_MsgKnowledgeGroupCreation* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgKnowledgeGroupDelete                                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgKnowledgeGroupDelete {
   ASN1T_KnowledgeGroup  oid;
} ASN1T_MsgKnowledgeGroupDelete;

class EXTERN ASN1C_MsgKnowledgeGroupDelete : public ASN1CType {
public:
   ASN1T_MsgKnowledgeGroupDelete& msgData;
   ASN1C_MsgKnowledgeGroupDelete (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgKnowledgeGroupDelete& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgKnowledgeGroupDelete (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupDelete* pvalue);
EXTERN int asn1PD_MsgKnowledgeGroupDelete (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupDelete* pvalue);

EXTERN void asn1Print_MsgKnowledgeGroupDelete (ASN1ConstCharPtr name, ASN1T_MsgKnowledgeGroupDelete* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgKnowledgeGroupEnable                                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgKnowledgeGroupEnable {
   ASN1T_KnowledgeGroup  oid;
   ASN1BOOL  enabled;
} ASN1T_MsgKnowledgeGroupEnable;

class EXTERN ASN1C_MsgKnowledgeGroupEnable : public ASN1CType {
public:
   ASN1T_MsgKnowledgeGroupEnable& msgData;
   ASN1C_MsgKnowledgeGroupEnable (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgKnowledgeGroupEnable& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgKnowledgeGroupEnable (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupEnable* pvalue);
EXTERN int asn1PD_MsgKnowledgeGroupEnable (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupEnable* pvalue);

EXTERN void asn1Print_MsgKnowledgeGroupEnable (ASN1ConstCharPtr name, ASN1T_MsgKnowledgeGroupEnable* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgKnowledgeGroupSetType                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgKnowledgeGroupSetType {
   ASN1T_KnowledgeGroup  oid;
   ASN1T_KnowledgeGroupType  type;
} ASN1T_MsgKnowledgeGroupSetType;

class EXTERN ASN1C_MsgKnowledgeGroupSetType : public ASN1CType {
public:
   ASN1T_MsgKnowledgeGroupSetType& msgData;
   ASN1C_MsgKnowledgeGroupSetType (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgKnowledgeGroupSetType& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgKnowledgeGroupSetType (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupSetType* pvalue);
EXTERN int asn1PD_MsgKnowledgeGroupSetType (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupSetType* pvalue);

EXTERN void asn1Print_MsgKnowledgeGroupSetType (ASN1ConstCharPtr name, ASN1T_MsgKnowledgeGroupSetType* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgKnowledgeGroupChangeSuperior                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgKnowledgeGroupChangeSuperior {
   ASN1T_KnowledgeGroup  oid;
   ASN1T_Team  oid_camp;
   ASN1T_KnowledgeGroup  oid_knowledgegroup_parent;
} ASN1T_MsgKnowledgeGroupChangeSuperior;

class EXTERN ASN1C_MsgKnowledgeGroupChangeSuperior : public ASN1CType {
public:
   ASN1T_MsgKnowledgeGroupChangeSuperior& msgData;
   ASN1C_MsgKnowledgeGroupChangeSuperior (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgKnowledgeGroupChangeSuperior& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgKnowledgeGroupChangeSuperior (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupChangeSuperior* pvalue);
EXTERN int asn1PD_MsgKnowledgeGroupChangeSuperior (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupChangeSuperior* pvalue);

EXTERN void asn1Print_MsgKnowledgeGroupChangeSuperior (ASN1ConstCharPtr name, ASN1T_MsgKnowledgeGroupChangeSuperior* pvalue);

/**************************************************************/
/*                                                            */
/*  EnumKnowledgeGroupErrorCode                               */
/*                                                            */
/**************************************************************/

struct EXTERN EnumKnowledgeGroupErrorCode {
   enum Root {
      no_error = 0,
      error_invalid_unit = 1,
      error_invalid_superior = 2,
      error_invalid_camp = 3,
      error_invalid_knowledgegroup = 4,
      error_invalid_type = 5
   } ;
} ;

typedef EnumKnowledgeGroupErrorCode::Root ASN1T_EnumKnowledgeGroupErrorCode;

class EXTERN ASN1C_EnumKnowledgeGroupErrorCode : public ASN1CType {
public:
   ASN1T_EnumKnowledgeGroupErrorCode& msgData;
   ASN1C_EnumKnowledgeGroupErrorCode (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumKnowledgeGroupErrorCode& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumKnowledgeGroupErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumKnowledgeGroupErrorCode value);
EXTERN int asn1PD_EnumKnowledgeGroupErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumKnowledgeGroupErrorCode* pvalue);

EXTERN void asn1Print_EnumKnowledgeGroupErrorCode (ASN1ConstCharPtr name, ASN1T_EnumKnowledgeGroupErrorCode* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgKnowledgeGroupChangeSuperiorAck                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgKnowledgeGroupChangeSuperiorAck {
   ASN1T_OID  oid;
   ASN1T_EnumKnowledgeGroupErrorCode  error_code;
} ASN1T_MsgKnowledgeGroupChangeSuperiorAck;

class EXTERN ASN1C_MsgKnowledgeGroupChangeSuperiorAck : public ASN1CType {
public:
   ASN1T_MsgKnowledgeGroupChangeSuperiorAck& msgData;
   ASN1C_MsgKnowledgeGroupChangeSuperiorAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgKnowledgeGroupChangeSuperiorAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgKnowledgeGroupChangeSuperiorAck (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupChangeSuperiorAck* pvalue);
EXTERN int asn1PD_MsgKnowledgeGroupChangeSuperiorAck (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupChangeSuperiorAck* pvalue);

EXTERN void asn1Print_MsgKnowledgeGroupChangeSuperiorAck (ASN1ConstCharPtr name, ASN1T_MsgKnowledgeGroupChangeSuperiorAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgKnowledgeGroupDeleteAck                                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgKnowledgeGroupDeleteAck {
   ASN1T_OID  oid;
   ASN1T_EnumKnowledgeGroupErrorCode  error_code;
} ASN1T_MsgKnowledgeGroupDeleteAck;

class EXTERN ASN1C_MsgKnowledgeGroupDeleteAck : public ASN1CType {
public:
   ASN1T_MsgKnowledgeGroupDeleteAck& msgData;
   ASN1C_MsgKnowledgeGroupDeleteAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgKnowledgeGroupDeleteAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgKnowledgeGroupDeleteAck (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupDeleteAck* pvalue);
EXTERN int asn1PD_MsgKnowledgeGroupDeleteAck (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupDeleteAck* pvalue);

EXTERN void asn1Print_MsgKnowledgeGroupDeleteAck (ASN1ConstCharPtr name, ASN1T_MsgKnowledgeGroupDeleteAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgKnowledgeGroupSetTypeAck                               */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgKnowledgeGroupSetTypeAck {
   ASN1T_OID  oid;
   ASN1T_EnumKnowledgeGroupErrorCode  error_code;
} ASN1T_MsgKnowledgeGroupSetTypeAck;

class EXTERN ASN1C_MsgKnowledgeGroupSetTypeAck : public ASN1CType {
public:
   ASN1T_MsgKnowledgeGroupSetTypeAck& msgData;
   ASN1C_MsgKnowledgeGroupSetTypeAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgKnowledgeGroupSetTypeAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgKnowledgeGroupSetTypeAck (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupSetTypeAck* pvalue);
EXTERN int asn1PD_MsgKnowledgeGroupSetTypeAck (ASN1CTXT* ctxt_p, ASN1T_MsgKnowledgeGroupSetTypeAck* pvalue);

EXTERN void asn1Print_MsgKnowledgeGroupSetTypeAck (ASN1ConstCharPtr name, ASN1T_MsgKnowledgeGroupSetTypeAck* pvalue);

#endif

/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 31-Jan-2007.
 */
#ifndef EXERCISES_H
#define EXERCISES_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"

/**************************************************************/
/*                                                            */
/*  ExercisesIncludes                                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ExercisesIncludes {
   ASN1OCTET __dummy__;
} ASN1T_ExercisesIncludes;

class EXTERN ASN1C_ExercisesIncludes : public ASN1CType {
public:
   ASN1T_ExercisesIncludes& msgData;
   ASN1C_ExercisesIncludes (
      ASN1MessageBuffer& msgBuf, ASN1T_ExercisesIncludes& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ExercisesIncludes (ASN1CTXT* ctxt_p, ASN1T_ExercisesIncludes* pvalue);
EXTERN int asn1PD_ExercisesIncludes (ASN1CTXT* ctxt_p, ASN1T_ExercisesIncludes* pvalue);

EXTERN void asn1Print_ExercisesIncludes (ASN1ConstCharPtr name, ASN1T_ExercisesIncludes* pvalue);

/**************************************************************/
/*                                                            */
/*  ExercisesExports                                          */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_ExercisesExports : public ASN1CType {
public:
   ASN1C_ExercisesExports (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ExercisesExports (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_ExercisesExports (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_ExercisesExports (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  Exercise                                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Exercise {
   ASN1VisibleString  name;
   ASN1VisibleString  terrain;
   ASN1VisibleString  dataset;
   ASN1VisibleString  physical;
} ASN1T_Exercise;

class EXTERN ASN1C_Exercise : public ASN1CType {
public:
   ASN1T_Exercise& msgData;
   ASN1C_Exercise (
      ASN1MessageBuffer& msgBuf, ASN1T_Exercise& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Exercise (ASN1CTXT* ctxt_p, ASN1T_Exercise* pvalue);
EXTERN int asn1PD_Exercise (ASN1CTXT* ctxt_p, ASN1T_Exercise* pvalue);

EXTERN void asn1Print_Exercise (ASN1ConstCharPtr name, ASN1T_Exercise* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgExerciseCreation                                       */
/*                                                            */
/**************************************************************/

typedef ASN1T_Exercise ASN1T_MsgExerciseCreation;

class EXTERN ASN1C_MsgExerciseCreation : public ASN1CType {
public:
   ASN1T_MsgExerciseCreation& msgData;
   ASN1C_MsgExerciseCreation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgExerciseCreation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgExerciseCreation (ASN1CTXT* ctxt_p, ASN1T_MsgExerciseCreation* pvalue);
EXTERN int asn1PD_MsgExerciseCreation (ASN1CTXT* ctxt_p, ASN1T_MsgExerciseCreation* pvalue);

EXTERN void asn1Print_MsgExerciseCreation (ASN1ConstCharPtr name, ASN1T_MsgExerciseCreation* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgExerciseUpdate                                         */
/*                                                            */
/**************************************************************/

typedef ASN1T_Exercise ASN1T_MsgExerciseUpdate;

class EXTERN ASN1C_MsgExerciseUpdate : public ASN1CType {
public:
   ASN1T_MsgExerciseUpdate& msgData;
   ASN1C_MsgExerciseUpdate (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgExerciseUpdate& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgExerciseUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgExerciseUpdate* pvalue);
EXTERN int asn1PD_MsgExerciseUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgExerciseUpdate* pvalue);

EXTERN void asn1Print_MsgExerciseUpdate (ASN1ConstCharPtr name, ASN1T_MsgExerciseUpdate* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgExerciseDestruction                                    */
/*                                                            */
/**************************************************************/

typedef ASN1VisibleString ASN1T_MsgExerciseDestruction;

class EXTERN ASN1C_MsgExerciseDestruction : public ASN1CType {
public:
   ASN1T_MsgExerciseDestruction& msgData;
   ASN1C_MsgExerciseDestruction (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgExerciseDestruction& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgExerciseDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgExerciseDestruction value);
EXTERN int asn1PD_MsgExerciseDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgExerciseDestruction* pvalue);

EXTERN void asn1Print_MsgExerciseDestruction (ASN1ConstCharPtr name, ASN1T_MsgExerciseDestruction* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgExerciseCreationRequest                                */
/*                                                            */
/**************************************************************/

typedef ASN1T_Exercise ASN1T_MsgExerciseCreationRequest;

class EXTERN ASN1C_MsgExerciseCreationRequest : public ASN1CType {
public:
   ASN1T_MsgExerciseCreationRequest& msgData;
   ASN1C_MsgExerciseCreationRequest (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgExerciseCreationRequest& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgExerciseCreationRequest (ASN1CTXT* ctxt_p, ASN1T_MsgExerciseCreationRequest* pvalue);
EXTERN int asn1PD_MsgExerciseCreationRequest (ASN1CTXT* ctxt_p, ASN1T_MsgExerciseCreationRequest* pvalue);

EXTERN void asn1Print_MsgExerciseCreationRequest (ASN1ConstCharPtr name, ASN1T_MsgExerciseCreationRequest* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgExerciseCreationRequestAck_return_code                 */
/*                                                            */
/**************************************************************/

struct EXTERN MsgExerciseCreationRequestAck_return_code {
   enum Root {
      success = 0,
      forbidden = 1,
      invalid_name = 2
   } ;
} ;

typedef MsgExerciseCreationRequestAck_return_code::Root ASN1T_MsgExerciseCreationRequestAck_return_code;

class EXTERN ASN1C_MsgExerciseCreationRequestAck_return_code : public ASN1CType {
public:
   ASN1T_MsgExerciseCreationRequestAck_return_code& msgData;
   ASN1C_MsgExerciseCreationRequestAck_return_code (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgExerciseCreationRequestAck_return_code& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgExerciseCreationRequestAck_return_code (ASN1CTXT* ctxt_p, ASN1T_MsgExerciseCreationRequestAck_return_code value);
EXTERN int asn1PD_MsgExerciseCreationRequestAck_return_code (ASN1CTXT* ctxt_p, ASN1T_MsgExerciseCreationRequestAck_return_code* pvalue);

EXTERN void asn1Print_MsgExerciseCreationRequestAck_return_code (ASN1ConstCharPtr name, ASN1T_MsgExerciseCreationRequestAck_return_code* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgExerciseCreationRequestAck                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgExerciseCreationRequestAck {
   ASN1T_MsgExerciseCreationRequestAck_return_code  return_code;
   ASN1VisibleString  name;
} ASN1T_MsgExerciseCreationRequestAck;

class EXTERN ASN1C_MsgExerciseCreationRequestAck : public ASN1CType {
public:
   ASN1T_MsgExerciseCreationRequestAck& msgData;
   ASN1C_MsgExerciseCreationRequestAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgExerciseCreationRequestAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgExerciseCreationRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgExerciseCreationRequestAck* pvalue);
EXTERN int asn1PD_MsgExerciseCreationRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgExerciseCreationRequestAck* pvalue);

EXTERN void asn1Print_MsgExerciseCreationRequestAck (ASN1ConstCharPtr name, ASN1T_MsgExerciseCreationRequestAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgExerciseUpdateRequest                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgExerciseUpdateRequest {
   ASN1T_Exercise  exercise;
   ASN1VisibleString  name;
} ASN1T_MsgExerciseUpdateRequest;

class EXTERN ASN1C_MsgExerciseUpdateRequest : public ASN1CType {
public:
   ASN1T_MsgExerciseUpdateRequest& msgData;
   ASN1C_MsgExerciseUpdateRequest (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgExerciseUpdateRequest& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgExerciseUpdateRequest (ASN1CTXT* ctxt_p, ASN1T_MsgExerciseUpdateRequest* pvalue);
EXTERN int asn1PD_MsgExerciseUpdateRequest (ASN1CTXT* ctxt_p, ASN1T_MsgExerciseUpdateRequest* pvalue);

EXTERN void asn1Print_MsgExerciseUpdateRequest (ASN1ConstCharPtr name, ASN1T_MsgExerciseUpdateRequest* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgExerciseUpdateRequestAck_return_code                   */
/*                                                            */
/**************************************************************/

struct EXTERN MsgExerciseUpdateRequestAck_return_code {
   enum Root {
      success = 0,
      forbidden = 1,
      not_found = 2,
      invalid_name = 3
   } ;
} ;

typedef MsgExerciseUpdateRequestAck_return_code::Root ASN1T_MsgExerciseUpdateRequestAck_return_code;

class EXTERN ASN1C_MsgExerciseUpdateRequestAck_return_code : public ASN1CType {
public:
   ASN1T_MsgExerciseUpdateRequestAck_return_code& msgData;
   ASN1C_MsgExerciseUpdateRequestAck_return_code (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgExerciseUpdateRequestAck_return_code& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgExerciseUpdateRequestAck_return_code (ASN1CTXT* ctxt_p, ASN1T_MsgExerciseUpdateRequestAck_return_code value);
EXTERN int asn1PD_MsgExerciseUpdateRequestAck_return_code (ASN1CTXT* ctxt_p, ASN1T_MsgExerciseUpdateRequestAck_return_code* pvalue);

EXTERN void asn1Print_MsgExerciseUpdateRequestAck_return_code (ASN1ConstCharPtr name, ASN1T_MsgExerciseUpdateRequestAck_return_code* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgExerciseUpdateRequestAck                               */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgExerciseUpdateRequestAck {
   ASN1T_MsgExerciseUpdateRequestAck_return_code  return_code;
   ASN1VisibleString  name;
} ASN1T_MsgExerciseUpdateRequestAck;

class EXTERN ASN1C_MsgExerciseUpdateRequestAck : public ASN1CType {
public:
   ASN1T_MsgExerciseUpdateRequestAck& msgData;
   ASN1C_MsgExerciseUpdateRequestAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgExerciseUpdateRequestAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgExerciseUpdateRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgExerciseUpdateRequestAck* pvalue);
EXTERN int asn1PD_MsgExerciseUpdateRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgExerciseUpdateRequestAck* pvalue);

EXTERN void asn1Print_MsgExerciseUpdateRequestAck (ASN1ConstCharPtr name, ASN1T_MsgExerciseUpdateRequestAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgExerciseDestructionRequest                             */
/*                                                            */
/**************************************************************/

typedef ASN1VisibleString ASN1T_MsgExerciseDestructionRequest;

class EXTERN ASN1C_MsgExerciseDestructionRequest : public ASN1CType {
public:
   ASN1T_MsgExerciseDestructionRequest& msgData;
   ASN1C_MsgExerciseDestructionRequest (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgExerciseDestructionRequest& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgExerciseDestructionRequest (ASN1CTXT* ctxt_p, ASN1T_MsgExerciseDestructionRequest value);
EXTERN int asn1PD_MsgExerciseDestructionRequest (ASN1CTXT* ctxt_p, ASN1T_MsgExerciseDestructionRequest* pvalue);

EXTERN void asn1Print_MsgExerciseDestructionRequest (ASN1ConstCharPtr name, ASN1T_MsgExerciseDestructionRequest* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgExerciseDestructionRequestAck_return_code              */
/*                                                            */
/**************************************************************/

struct EXTERN MsgExerciseDestructionRequestAck_return_code {
   enum Root {
      success = 0,
      forbidden = 1,
      not_found = 2
   } ;
} ;

typedef MsgExerciseDestructionRequestAck_return_code::Root ASN1T_MsgExerciseDestructionRequestAck_return_code;

class EXTERN ASN1C_MsgExerciseDestructionRequestAck_return_code : public ASN1CType {
public:
   ASN1T_MsgExerciseDestructionRequestAck_return_code& msgData;
   ASN1C_MsgExerciseDestructionRequestAck_return_code (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgExerciseDestructionRequestAck_return_code& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgExerciseDestructionRequestAck_return_code (ASN1CTXT* ctxt_p, ASN1T_MsgExerciseDestructionRequestAck_return_code value);
EXTERN int asn1PD_MsgExerciseDestructionRequestAck_return_code (ASN1CTXT* ctxt_p, ASN1T_MsgExerciseDestructionRequestAck_return_code* pvalue);

EXTERN void asn1Print_MsgExerciseDestructionRequestAck_return_code (ASN1ConstCharPtr name, ASN1T_MsgExerciseDestructionRequestAck_return_code* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgExerciseDestructionRequestAck                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgExerciseDestructionRequestAck {
   ASN1T_MsgExerciseDestructionRequestAck_return_code  return_code;
   ASN1VisibleString  name;
} ASN1T_MsgExerciseDestructionRequestAck;

class EXTERN ASN1C_MsgExerciseDestructionRequestAck : public ASN1CType {
public:
   ASN1T_MsgExerciseDestructionRequestAck& msgData;
   ASN1C_MsgExerciseDestructionRequestAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgExerciseDestructionRequestAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgExerciseDestructionRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgExerciseDestructionRequestAck* pvalue);
EXTERN int asn1PD_MsgExerciseDestructionRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgExerciseDestructionRequestAck* pvalue);

EXTERN void asn1Print_MsgExerciseDestructionRequestAck (ASN1ConstCharPtr name, ASN1T_MsgExerciseDestructionRequestAck* pvalue);

#endif

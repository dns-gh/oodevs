/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 25-Oct-2006.
 */
#ifndef SCIPIOLIMALIMITE_H
#define SCIPIOLIMALIMITE_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  EnumTypeLima                                              */
/*                                                            */
/**************************************************************/

struct EXTERN EnumTypeLima {
   enum Root {
      ligne_debouche = 0,
      ligne_changement_attitude = 1,
      ligne_coordination = 2,
      ligne_interdire = 3,
      ligne_objectif = 4,
      ligne_coup_arret = 5,
      ligne_recueil = 6,
      ligne_debut_mission = 7,
      ligne_fin_mission = 8,
      ligne_identification_accueil = 9
   } ;
} ;

typedef EnumTypeLima::Root ASN1T_EnumTypeLima;

class EXTERN ASN1C_EnumTypeLima : public ASN1CType {
public:
   ASN1T_EnumTypeLima& msgData;
   ASN1C_EnumTypeLima (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumTypeLima& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumTypeLima (ASN1CTXT* ctxt_p, ASN1T_EnumTypeLima value);
EXTERN int asn1PD_EnumTypeLima (ASN1CTXT* ctxt_p, ASN1T_EnumTypeLima* pvalue);

EXTERN void asn1Print_EnumTypeLima (ASN1ConstCharPtr name, ASN1T_EnumTypeLima* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLimitCreation                                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLimitCreation {
   ASN1T_OID  oid;
   ASN1T_EnumNatureLevel  level;
   ASN1T_Line  geometrie;

   ASN1T_MsgLimitCreation () {
      level = EnumNatureLevel::ooo;
   }
} ASN1T_MsgLimitCreation;

class EXTERN ASN1C_MsgLimitCreation : public ASN1CType {
public:
   ASN1T_MsgLimitCreation& msgData;
   ASN1C_MsgLimitCreation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLimitCreation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLimitCreation (ASN1CTXT* ctxt_p, ASN1T_MsgLimitCreation* pvalue);
EXTERN int asn1PD_MsgLimitCreation (ASN1CTXT* ctxt_p, ASN1T_MsgLimitCreation* pvalue);

EXTERN void asn1Print_MsgLimitCreation (ASN1ConstCharPtr name, ASN1T_MsgLimitCreation* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLimitDestruction                                       */
/*                                                            */
/**************************************************************/

typedef ASN1T_OID ASN1T_MsgLimitDestruction;

class EXTERN ASN1C_MsgLimitDestruction : public ASN1CType {
public:
   ASN1T_MsgLimitDestruction& msgData;
   ASN1C_MsgLimitDestruction (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLimitDestruction& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLimitDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgLimitDestruction value);
EXTERN int asn1PD_MsgLimitDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgLimitDestruction* pvalue);

EXTERN void asn1Print_MsgLimitDestruction (ASN1ConstCharPtr name, ASN1T_MsgLimitDestruction* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLimitUpdate                                            */
/*                                                            */
/**************************************************************/

typedef ASN1T_MsgLimitCreation ASN1T_MsgLimitUpdate;

class EXTERN ASN1C_MsgLimitUpdate : public ASN1CType {
public:
   ASN1T_MsgLimitUpdate& msgData;
   ASN1C_MsgLimitUpdate (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLimitUpdate& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLimitUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgLimitUpdate* pvalue);
EXTERN int asn1PD_MsgLimitUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgLimitUpdate* pvalue);

EXTERN void asn1Print_MsgLimitUpdate (ASN1ConstCharPtr name, ASN1T_MsgLimitUpdate* pvalue);

/**************************************************************/
/*                                                            */
/*  EnumInfoContextErrorCode                                  */
/*                                                            */
/**************************************************************/

struct EXTERN EnumInfoContextErrorCode {
   enum Root {
      no_error = 0,
      error_invalid_id = 1,
      error_invalid_geometry = 2,
      error_invalid_lima_function = 3,
      error_invalid_limit_or_lima_used = 4
   } ;
} ;

typedef EnumInfoContextErrorCode::Root ASN1T_EnumInfoContextErrorCode;

class EXTERN ASN1C_EnumInfoContextErrorCode : public ASN1CType {
public:
   ASN1T_EnumInfoContextErrorCode& msgData;
   ASN1C_EnumInfoContextErrorCode (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumInfoContextErrorCode& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumInfoContextErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumInfoContextErrorCode value);
EXTERN int asn1PD_EnumInfoContextErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumInfoContextErrorCode* pvalue);

EXTERN void asn1Print_EnumInfoContextErrorCode (ASN1ConstCharPtr name, ASN1T_EnumInfoContextErrorCode* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLimitCreationAck                                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLimitCreationAck {
   ASN1T_OID  oid;
   ASN1T_EnumInfoContextErrorCode  error_code;
} ASN1T_MsgLimitCreationAck;

class EXTERN ASN1C_MsgLimitCreationAck : public ASN1CType {
public:
   ASN1T_MsgLimitCreationAck& msgData;
   ASN1C_MsgLimitCreationAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLimitCreationAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLimitCreationAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimitCreationAck* pvalue);
EXTERN int asn1PD_MsgLimitCreationAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimitCreationAck* pvalue);

EXTERN void asn1Print_MsgLimitCreationAck (ASN1ConstCharPtr name, ASN1T_MsgLimitCreationAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLimitDestructionAck                                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLimitDestructionAck {
   ASN1T_OID  oid;
   ASN1T_EnumInfoContextErrorCode  error_code;
} ASN1T_MsgLimitDestructionAck;

class EXTERN ASN1C_MsgLimitDestructionAck : public ASN1CType {
public:
   ASN1T_MsgLimitDestructionAck& msgData;
   ASN1C_MsgLimitDestructionAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLimitDestructionAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLimitDestructionAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimitDestructionAck* pvalue);
EXTERN int asn1PD_MsgLimitDestructionAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimitDestructionAck* pvalue);

EXTERN void asn1Print_MsgLimitDestructionAck (ASN1ConstCharPtr name, ASN1T_MsgLimitDestructionAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLimitUpdateAck                                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLimitUpdateAck {
   ASN1T_OID  oid;
   ASN1T_EnumInfoContextErrorCode  error_code;
} ASN1T_MsgLimitUpdateAck;

class EXTERN ASN1C_MsgLimitUpdateAck : public ASN1CType {
public:
   ASN1T_MsgLimitUpdateAck& msgData;
   ASN1C_MsgLimitUpdateAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLimitUpdateAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLimitUpdateAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimitUpdateAck* pvalue);
EXTERN int asn1PD_MsgLimitUpdateAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimitUpdateAck* pvalue);

EXTERN void asn1Print_MsgLimitUpdateAck (ASN1ConstCharPtr name, ASN1T_MsgLimitUpdateAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLimaCreation                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLimaCreation {
   ASN1T_OID  oid;
   ASN1T_EnumTypeLima  fonction;
   ASN1T_Line  geometrie;
} ASN1T_MsgLimaCreation;

class EXTERN ASN1C_MsgLimaCreation : public ASN1CType {
public:
   ASN1T_MsgLimaCreation& msgData;
   ASN1C_MsgLimaCreation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLimaCreation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLimaCreation (ASN1CTXT* ctxt_p, ASN1T_MsgLimaCreation* pvalue);
EXTERN int asn1PD_MsgLimaCreation (ASN1CTXT* ctxt_p, ASN1T_MsgLimaCreation* pvalue);

EXTERN void asn1Print_MsgLimaCreation (ASN1ConstCharPtr name, ASN1T_MsgLimaCreation* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLimaDestruction                                        */
/*                                                            */
/**************************************************************/

typedef ASN1T_OID ASN1T_MsgLimaDestruction;

class EXTERN ASN1C_MsgLimaDestruction : public ASN1CType {
public:
   ASN1T_MsgLimaDestruction& msgData;
   ASN1C_MsgLimaDestruction (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLimaDestruction& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLimaDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgLimaDestruction value);
EXTERN int asn1PD_MsgLimaDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgLimaDestruction* pvalue);

EXTERN void asn1Print_MsgLimaDestruction (ASN1ConstCharPtr name, ASN1T_MsgLimaDestruction* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLimaUpdate                                             */
/*                                                            */
/**************************************************************/

typedef ASN1T_MsgLimaCreation ASN1T_MsgLimaUpdate;

class EXTERN ASN1C_MsgLimaUpdate : public ASN1CType {
public:
   ASN1T_MsgLimaUpdate& msgData;
   ASN1C_MsgLimaUpdate (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLimaUpdate& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLimaUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgLimaUpdate* pvalue);
EXTERN int asn1PD_MsgLimaUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgLimaUpdate* pvalue);

EXTERN void asn1Print_MsgLimaUpdate (ASN1ConstCharPtr name, ASN1T_MsgLimaUpdate* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLimaCreationAck                                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLimaCreationAck {
   ASN1T_OID  oid;
   ASN1T_EnumInfoContextErrorCode  error_code;
} ASN1T_MsgLimaCreationAck;

class EXTERN ASN1C_MsgLimaCreationAck : public ASN1CType {
public:
   ASN1T_MsgLimaCreationAck& msgData;
   ASN1C_MsgLimaCreationAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLimaCreationAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLimaCreationAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimaCreationAck* pvalue);
EXTERN int asn1PD_MsgLimaCreationAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimaCreationAck* pvalue);

EXTERN void asn1Print_MsgLimaCreationAck (ASN1ConstCharPtr name, ASN1T_MsgLimaCreationAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLimaDestructionAck                                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLimaDestructionAck {
   ASN1T_OID  oid;
   ASN1T_EnumInfoContextErrorCode  error_code;
} ASN1T_MsgLimaDestructionAck;

class EXTERN ASN1C_MsgLimaDestructionAck : public ASN1CType {
public:
   ASN1T_MsgLimaDestructionAck& msgData;
   ASN1C_MsgLimaDestructionAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLimaDestructionAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLimaDestructionAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimaDestructionAck* pvalue);
EXTERN int asn1PD_MsgLimaDestructionAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimaDestructionAck* pvalue);

EXTERN void asn1Print_MsgLimaDestructionAck (ASN1ConstCharPtr name, ASN1T_MsgLimaDestructionAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLimaUpdateAck                                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLimaUpdateAck {
   ASN1T_OID  oid;
   ASN1T_EnumInfoContextErrorCode  error_code;
} ASN1T_MsgLimaUpdateAck;

class EXTERN ASN1C_MsgLimaUpdateAck : public ASN1CType {
public:
   ASN1T_MsgLimaUpdateAck& msgData;
   ASN1C_MsgLimaUpdateAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLimaUpdateAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLimaUpdateAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimaUpdateAck* pvalue);
EXTERN int asn1PD_MsgLimaUpdateAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimaUpdateAck* pvalue);

EXTERN void asn1Print_MsgLimaUpdateAck (ASN1ConstCharPtr name, ASN1T_MsgLimaUpdateAck* pvalue);

#endif

/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 13-Sep-2007.
 */
#ifndef LIMALIMITE_H
#define LIMALIMITE_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "Base.h"

/**************************************************************/
/*                                                            */
/*  LimaLimiteIncludes                                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_LimaLimiteIncludes {
} ASN1T_LimaLimiteIncludes;

class EXTERN ASN1C_LimaLimiteIncludes : public ASN1CType {
public:
   ASN1T_LimaLimiteIncludes& msgData;
   ASN1C_LimaLimiteIncludes (
      ASN1MessageBuffer& msgBuf, ASN1T_LimaLimiteIncludes& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_LimaLimiteIncludes (ASN1CTXT* ctxt_p, ASN1T_LimaLimiteIncludes* pvalue);
EXTERN int asn1PD_LimaLimiteIncludes (ASN1CTXT* ctxt_p, ASN1T_LimaLimiteIncludes* pvalue);

EXTERN void asn1Print_LimaLimiteIncludes (ASN1ConstCharPtr name, ASN1T_LimaLimiteIncludes* pvalue);

/**************************************************************/
/*                                                            */
/*  LimaLimiteExports                                         */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_LimaLimiteExports : public ASN1CType {
public:
   ASN1C_LimaLimiteExports (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_LimaLimiteExports (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_LimaLimiteExports (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_LimaLimiteExports (ASN1ConstCharPtr name);

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
      error_invalid_diffusion = 4
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
/*  EnumLimaType                                              */
/*                                                            */
/**************************************************************/

struct EXTERN EnumLimaType {
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

typedef EnumLimaType::Root ASN1T_EnumLimaType;

class EXTERN ASN1C_EnumLimaType : public ASN1CType {
public:
   ASN1T_EnumLimaType& msgData;
   ASN1C_EnumLimaType (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumLimaType& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumLimaType (ASN1CTXT* ctxt_p, ASN1T_EnumLimaType value);
EXTERN int asn1PD_EnumLimaType (ASN1CTXT* ctxt_p, ASN1T_EnumLimaType* pvalue);

EXTERN void asn1Print_EnumLimaType (ASN1ConstCharPtr name, ASN1T_EnumLimaType* pvalue);

/**************************************************************/
/*                                                            */
/*  TacticalLinesDiffusion                                    */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_TacticalLinesDiffusion_automat 1
#define T_TacticalLinesDiffusion_formation 2

typedef struct EXTERN ASN1T_TacticalLinesDiffusion {
   int t;
   union {
      /* t = 1 */
      ASN1T_Automat  automat;
      /* t = 2 */
      ASN1T_Formation  formation;
   } u;
} ASN1T_TacticalLinesDiffusion;

class EXTERN ASN1C_TacticalLinesDiffusion : public ASN1CType {
public:
   ASN1T_TacticalLinesDiffusion& msgData;
   ASN1C_TacticalLinesDiffusion (
      ASN1MessageBuffer& msgBuf, ASN1T_TacticalLinesDiffusion& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_TacticalLinesDiffusion (ASN1CTXT* ctxt_p, ASN1T_TacticalLinesDiffusion* pvalue);
EXTERN int asn1PD_TacticalLinesDiffusion (ASN1CTXT* ctxt_p, ASN1T_TacticalLinesDiffusion* pvalue);

EXTERN void asn1Print_TacticalLinesDiffusion (ASN1ConstCharPtr name, ASN1T_TacticalLinesDiffusion* pvalue);

/**************************************************************/
/*                                                            */
/*  _SeqOfEnumLimaType                                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T__SeqOfEnumLimaType {
   ASN1UINT n;
   ASN1T_EnumLimaType *elem;
} ASN1T__SeqOfEnumLimaType;

class EXTERN ASN1C__SeqOfEnumLimaType : public ASN1CType {
public:
   ASN1T__SeqOfEnumLimaType& msgData;
   ASN1C__SeqOfEnumLimaType (
      ASN1MessageBuffer& msgBuf, ASN1T__SeqOfEnumLimaType& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE__SeqOfEnumLimaType (ASN1CTXT* ctxt_p, ASN1T__SeqOfEnumLimaType* pvalue);
EXTERN int asn1PD__SeqOfEnumLimaType (ASN1CTXT* ctxt_p, ASN1T__SeqOfEnumLimaType* pvalue);

EXTERN void asn1Print__SeqOfEnumLimaType (ASN1ConstCharPtr name, ASN1T__SeqOfEnumLimaType* pvalue);

/**************************************************************/
/*                                                            */
/*  LimaOrder                                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_LimaOrder {
   ASN1T_Line  lima;
   ASN1INT   horaire;
   ASN1T__SeqOfEnumLimaType  fonctions;

   ASN1T_LimaOrder () {
      horaire = 0;
   }
} ASN1T_LimaOrder;

class EXTERN ASN1C_LimaOrder : public ASN1CType {
public:
   ASN1T_LimaOrder& msgData;
   ASN1C_LimaOrder (
      ASN1MessageBuffer& msgBuf, ASN1T_LimaOrder& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_LimaOrder (ASN1CTXT* ctxt_p, ASN1T_LimaOrder* pvalue);
EXTERN int asn1PD_LimaOrder (ASN1CTXT* ctxt_p, ASN1T_LimaOrder* pvalue);

EXTERN void asn1Print_LimaOrder (ASN1ConstCharPtr name, ASN1T_LimaOrder* pvalue);

/**************************************************************/
/*                                                            */
/*  LimasOrder                                                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_LimasOrder {
   ASN1UINT n;
   ASN1T_LimaOrder *elem;
} ASN1T_LimasOrder;

class EXTERN ASN1C_LimasOrder : public ASN1CType {
public:
   ASN1T_LimasOrder& msgData;
   ASN1C_LimasOrder (
      ASN1MessageBuffer& msgBuf, ASN1T_LimasOrder& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_LimasOrder (ASN1CTXT* ctxt_p, ASN1T_LimasOrder* pvalue);
EXTERN int asn1PD_LimasOrder (ASN1CTXT* ctxt_p, ASN1T_LimasOrder* pvalue);

EXTERN void asn1Print_LimasOrder (ASN1ConstCharPtr name, ASN1T_LimasOrder* pvalue);

/**************************************************************/
/*                                                            */
/*  TacticalLine                                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_TacticalLine {
   ASN1VisibleString  name;
   ASN1T_Line  geometry;
   ASN1T_TacticalLinesDiffusion  diffusion;
} ASN1T_TacticalLine;

class EXTERN ASN1C_TacticalLine : public ASN1CType {
public:
   ASN1T_TacticalLine& msgData;
   ASN1C_TacticalLine (
      ASN1MessageBuffer& msgBuf, ASN1T_TacticalLine& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_TacticalLine (ASN1CTXT* ctxt_p, ASN1T_TacticalLine* pvalue);
EXTERN int asn1PD_TacticalLine (ASN1CTXT* ctxt_p, ASN1T_TacticalLine* pvalue);

EXTERN void asn1Print_TacticalLine (ASN1ConstCharPtr name, ASN1T_TacticalLine* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLimitCreationRequestAck                                */
/*                                                            */
/**************************************************************/

typedef ASN1T_EnumInfoContextErrorCode ASN1T_MsgLimitCreationRequestAck;

class EXTERN ASN1C_MsgLimitCreationRequestAck : public ASN1CType {
public:
   ASN1T_MsgLimitCreationRequestAck& msgData;
   ASN1C_MsgLimitCreationRequestAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLimitCreationRequestAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLimitCreationRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimitCreationRequestAck value);
EXTERN int asn1PD_MsgLimitCreationRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimitCreationRequestAck* pvalue);

EXTERN void asn1Print_MsgLimitCreationRequestAck (ASN1ConstCharPtr name, ASN1T_MsgLimitCreationRequestAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLimitCreationRequest                                   */
/*                                                            */
/**************************************************************/

typedef ASN1T_TacticalLine ASN1T_MsgLimitCreationRequest;

class EXTERN ASN1C_MsgLimitCreationRequest : public ASN1CType {
public:
   ASN1T_MsgLimitCreationRequest& msgData;
   ASN1C_MsgLimitCreationRequest (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLimitCreationRequest& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLimitCreationRequest (ASN1CTXT* ctxt_p, ASN1T_MsgLimitCreationRequest* pvalue);
EXTERN int asn1PD_MsgLimitCreationRequest (ASN1CTXT* ctxt_p, ASN1T_MsgLimitCreationRequest* pvalue);

EXTERN void asn1Print_MsgLimitCreationRequest (ASN1ConstCharPtr name, ASN1T_MsgLimitCreationRequest* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLimitCreation                                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLimitCreation {
   ASN1T_OID  oid;
   ASN1T_TacticalLine  tactical_line;
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
/*  MsgLimitUpdateRequestAck                                  */
/*                                                            */
/**************************************************************/

typedef ASN1T_EnumInfoContextErrorCode ASN1T_MsgLimitUpdateRequestAck;

class EXTERN ASN1C_MsgLimitUpdateRequestAck : public ASN1CType {
public:
   ASN1T_MsgLimitUpdateRequestAck& msgData;
   ASN1C_MsgLimitUpdateRequestAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLimitUpdateRequestAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLimitUpdateRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimitUpdateRequestAck value);
EXTERN int asn1PD_MsgLimitUpdateRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimitUpdateRequestAck* pvalue);

EXTERN void asn1Print_MsgLimitUpdateRequestAck (ASN1ConstCharPtr name, ASN1T_MsgLimitUpdateRequestAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLimitUpdateRequest                                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLimitUpdateRequest {
   ASN1T_OID  oid;
   ASN1T_TacticalLine  tactical_line;
} ASN1T_MsgLimitUpdateRequest;

class EXTERN ASN1C_MsgLimitUpdateRequest : public ASN1CType {
public:
   ASN1T_MsgLimitUpdateRequest& msgData;
   ASN1C_MsgLimitUpdateRequest (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLimitUpdateRequest& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLimitUpdateRequest (ASN1CTXT* ctxt_p, ASN1T_MsgLimitUpdateRequest* pvalue);
EXTERN int asn1PD_MsgLimitUpdateRequest (ASN1CTXT* ctxt_p, ASN1T_MsgLimitUpdateRequest* pvalue);

EXTERN void asn1Print_MsgLimitUpdateRequest (ASN1ConstCharPtr name, ASN1T_MsgLimitUpdateRequest* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLimitUpdate                                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLimitUpdate {
   ASN1T_OID  oid;
   ASN1T_TacticalLine  tactical_line;
} ASN1T_MsgLimitUpdate;

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
/*  MsgLimitDestructionRequestAck                             */
/*                                                            */
/**************************************************************/

typedef ASN1T_EnumInfoContextErrorCode ASN1T_MsgLimitDestructionRequestAck;

class EXTERN ASN1C_MsgLimitDestructionRequestAck : public ASN1CType {
public:
   ASN1T_MsgLimitDestructionRequestAck& msgData;
   ASN1C_MsgLimitDestructionRequestAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLimitDestructionRequestAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLimitDestructionRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimitDestructionRequestAck value);
EXTERN int asn1PD_MsgLimitDestructionRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimitDestructionRequestAck* pvalue);

EXTERN void asn1Print_MsgLimitDestructionRequestAck (ASN1ConstCharPtr name, ASN1T_MsgLimitDestructionRequestAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLimitDestructionRequest                                */
/*                                                            */
/**************************************************************/

typedef ASN1T_OID ASN1T_MsgLimitDestructionRequest;

class EXTERN ASN1C_MsgLimitDestructionRequest : public ASN1CType {
public:
   ASN1T_MsgLimitDestructionRequest& msgData;
   ASN1C_MsgLimitDestructionRequest (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLimitDestructionRequest& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLimitDestructionRequest (ASN1CTXT* ctxt_p, ASN1T_MsgLimitDestructionRequest value);
EXTERN int asn1PD_MsgLimitDestructionRequest (ASN1CTXT* ctxt_p, ASN1T_MsgLimitDestructionRequest* pvalue);

EXTERN void asn1Print_MsgLimitDestructionRequest (ASN1ConstCharPtr name, ASN1T_MsgLimitDestructionRequest* pvalue);

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
/*  MsgLimaCreationRequestAck                                 */
/*                                                            */
/**************************************************************/

typedef ASN1T_EnumInfoContextErrorCode ASN1T_MsgLimaCreationRequestAck;

class EXTERN ASN1C_MsgLimaCreationRequestAck : public ASN1CType {
public:
   ASN1T_MsgLimaCreationRequestAck& msgData;
   ASN1C_MsgLimaCreationRequestAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLimaCreationRequestAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLimaCreationRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimaCreationRequestAck value);
EXTERN int asn1PD_MsgLimaCreationRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimaCreationRequestAck* pvalue);

EXTERN void asn1Print_MsgLimaCreationRequestAck (ASN1ConstCharPtr name, ASN1T_MsgLimaCreationRequestAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLimaCreationRequest                                    */
/*                                                            */
/**************************************************************/

typedef ASN1T_TacticalLine ASN1T_MsgLimaCreationRequest;

class EXTERN ASN1C_MsgLimaCreationRequest : public ASN1CType {
public:
   ASN1T_MsgLimaCreationRequest& msgData;
   ASN1C_MsgLimaCreationRequest (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLimaCreationRequest& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLimaCreationRequest (ASN1CTXT* ctxt_p, ASN1T_MsgLimaCreationRequest* pvalue);
EXTERN int asn1PD_MsgLimaCreationRequest (ASN1CTXT* ctxt_p, ASN1T_MsgLimaCreationRequest* pvalue);

EXTERN void asn1Print_MsgLimaCreationRequest (ASN1ConstCharPtr name, ASN1T_MsgLimaCreationRequest* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLimaCreation                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLimaCreation {
   ASN1T_OID  oid;
   ASN1T_TacticalLine  tactical_line;
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
/*  MsgLimaUpdateRequestAck                                   */
/*                                                            */
/**************************************************************/

typedef ASN1T_EnumInfoContextErrorCode ASN1T_MsgLimaUpdateRequestAck;

class EXTERN ASN1C_MsgLimaUpdateRequestAck : public ASN1CType {
public:
   ASN1T_MsgLimaUpdateRequestAck& msgData;
   ASN1C_MsgLimaUpdateRequestAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLimaUpdateRequestAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLimaUpdateRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimaUpdateRequestAck value);
EXTERN int asn1PD_MsgLimaUpdateRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimaUpdateRequestAck* pvalue);

EXTERN void asn1Print_MsgLimaUpdateRequestAck (ASN1ConstCharPtr name, ASN1T_MsgLimaUpdateRequestAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLimaUpdateRequest                                      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLimaUpdateRequest {
   ASN1T_OID  oid;
   ASN1T_TacticalLine  tactical_line;
} ASN1T_MsgLimaUpdateRequest;

class EXTERN ASN1C_MsgLimaUpdateRequest : public ASN1CType {
public:
   ASN1T_MsgLimaUpdateRequest& msgData;
   ASN1C_MsgLimaUpdateRequest (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLimaUpdateRequest& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLimaUpdateRequest (ASN1CTXT* ctxt_p, ASN1T_MsgLimaUpdateRequest* pvalue);
EXTERN int asn1PD_MsgLimaUpdateRequest (ASN1CTXT* ctxt_p, ASN1T_MsgLimaUpdateRequest* pvalue);

EXTERN void asn1Print_MsgLimaUpdateRequest (ASN1ConstCharPtr name, ASN1T_MsgLimaUpdateRequest* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLimaUpdate                                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLimaUpdate {
   ASN1T_OID  oid;
   ASN1T_TacticalLine  tactical_line;
} ASN1T_MsgLimaUpdate;

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
/*  MsgLimaDestructionRequestAck                              */
/*                                                            */
/**************************************************************/

typedef ASN1T_EnumInfoContextErrorCode ASN1T_MsgLimaDestructionRequestAck;

class EXTERN ASN1C_MsgLimaDestructionRequestAck : public ASN1CType {
public:
   ASN1T_MsgLimaDestructionRequestAck& msgData;
   ASN1C_MsgLimaDestructionRequestAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLimaDestructionRequestAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLimaDestructionRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimaDestructionRequestAck value);
EXTERN int asn1PD_MsgLimaDestructionRequestAck (ASN1CTXT* ctxt_p, ASN1T_MsgLimaDestructionRequestAck* pvalue);

EXTERN void asn1Print_MsgLimaDestructionRequestAck (ASN1ConstCharPtr name, ASN1T_MsgLimaDestructionRequestAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLimaDestructionRequest                                 */
/*                                                            */
/**************************************************************/

typedef ASN1T_OID ASN1T_MsgLimaDestructionRequest;

class EXTERN ASN1C_MsgLimaDestructionRequest : public ASN1CType {
public:
   ASN1T_MsgLimaDestructionRequest& msgData;
   ASN1C_MsgLimaDestructionRequest (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLimaDestructionRequest& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLimaDestructionRequest (ASN1CTXT* ctxt_p, ASN1T_MsgLimaDestructionRequest value);
EXTERN int asn1PD_MsgLimaDestructionRequest (ASN1CTXT* ctxt_p, ASN1T_MsgLimaDestructionRequest* pvalue);

EXTERN void asn1Print_MsgLimaDestructionRequest (ASN1ConstCharPtr name, ASN1T_MsgLimaDestructionRequest* pvalue);

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

#endif

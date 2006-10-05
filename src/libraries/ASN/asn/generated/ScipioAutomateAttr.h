/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 04-Oct-2006.
 */
#ifndef SCIPIOAUTOMATEATTR_H
#define SCIPIOAUTOMATEATTR_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"
#include "ScipioUnitAttr.h"

/**************************************************************/
/*                                                            */
/*  EnumAutomateState                                         */
/*                                                            */
/**************************************************************/

struct EXTERN EnumAutomateState {
   enum Root {
      embraye = 0,
      debraye = 1
   } ;
} ;

typedef EnumAutomateState::Root ASN1T_EnumAutomateState;

class EXTERN ASN1C_EnumAutomateState : public ASN1CType {
public:
   ASN1T_EnumAutomateState& msgData;
   ASN1C_EnumAutomateState (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumAutomateState& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumAutomateState (ASN1CTXT* ctxt_p, ASN1T_EnumAutomateState value);
EXTERN int asn1PD_EnumAutomateState (ASN1CTXT* ctxt_p, ASN1T_EnumAutomateState* pvalue);

EXTERN void asn1Print_EnumAutomateState (ASN1ConstCharPtr name, ASN1T_EnumAutomateState* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgAutomateAttributes                                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgAutomateAttributes {
   struct {
      unsigned etat_automatePresent : 1;
      unsigned rapport_de_forcePresent : 1;
      unsigned combat_de_rencontrePresent : 1;
      unsigned etat_operationnelPresent : 1;
      unsigned disponibilite_au_tir_indirectPresent : 1;
      unsigned roePresent : 1;
   } m;
   ASN1T_Automate  oid_automate;
   ASN1T_EnumAutomateState  etat_automate;
   ASN1T_EnumEtatRapFor  rapport_de_force;
   ASN1T_EnumEtatCombatRencontre  combat_de_rencontre;
   ASN1T_EnumEtatOperationnel  etat_operationnel;
   ASN1T_EnumDisponibiliteAuTir  disponibilite_au_tir_indirect;
   ASN1T_EnumRoe  roe;

   ASN1T_MsgAutomateAttributes () {
      m.etat_automatePresent = 0;
      m.rapport_de_forcePresent = 0;
      m.combat_de_rencontrePresent = 0;
      m.etat_operationnelPresent = 0;
      m.disponibilite_au_tir_indirectPresent = 0;
      m.roePresent = 0;
   }
} ASN1T_MsgAutomateAttributes;

class EXTERN ASN1C_MsgAutomateAttributes : public ASN1CType {
public:
   ASN1T_MsgAutomateAttributes& msgData;
   ASN1C_MsgAutomateAttributes (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgAutomateAttributes& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgAutomateAttributes (ASN1CTXT* ctxt_p, ASN1T_MsgAutomateAttributes* pvalue);
EXTERN int asn1PD_MsgAutomateAttributes (ASN1CTXT* ctxt_p, ASN1T_MsgAutomateAttributes* pvalue);

EXTERN void asn1Print_MsgAutomateAttributes (ASN1ConstCharPtr name, ASN1T_MsgAutomateAttributes* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgSetAutomateMode                                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgSetAutomateMode {
   ASN1T_Agent  unit_id;
   ASN1T_EnumAutomateState  mode;
} ASN1T_MsgSetAutomateMode;

class EXTERN ASN1C_MsgSetAutomateMode : public ASN1CType {
public:
   ASN1T_MsgSetAutomateMode& msgData;
   ASN1C_MsgSetAutomateMode (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgSetAutomateMode& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgSetAutomateMode (ASN1CTXT* ctxt_p, ASN1T_MsgSetAutomateMode* pvalue);
EXTERN int asn1PD_MsgSetAutomateMode (ASN1CTXT* ctxt_p, ASN1T_MsgSetAutomateMode* pvalue);

EXTERN void asn1Print_MsgSetAutomateMode (ASN1ConstCharPtr name, ASN1T_MsgSetAutomateMode* pvalue);

/**************************************************************/
/*                                                            */
/*  EnumSetAutomateModeErrorCode                              */
/*                                                            */
/**************************************************************/

struct EXTERN EnumSetAutomateModeErrorCode {
   enum Root {
      no_error = 0,
      error_invalid_unit = 1,
      error_unit_not_automate = 2,
      error_already_embraye = 3,
      error_already_debraye = 4,
      error_unit_surrendered = 5
   } ;
} ;

typedef EnumSetAutomateModeErrorCode::Root ASN1T_EnumSetAutomateModeErrorCode;

class EXTERN ASN1C_EnumSetAutomateModeErrorCode : public ASN1CType {
public:
   ASN1T_EnumSetAutomateModeErrorCode& msgData;
   ASN1C_EnumSetAutomateModeErrorCode (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumSetAutomateModeErrorCode& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumSetAutomateModeErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumSetAutomateModeErrorCode value);
EXTERN int asn1PD_EnumSetAutomateModeErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumSetAutomateModeErrorCode* pvalue);

EXTERN void asn1Print_EnumSetAutomateModeErrorCode (ASN1ConstCharPtr name, ASN1T_EnumSetAutomateModeErrorCode* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgSetAutomateModeAck                                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgSetAutomateModeAck {
   ASN1T_Agent  unit_id;
   ASN1T_EnumSetAutomateModeErrorCode  error_code;
} ASN1T_MsgSetAutomateModeAck;

class EXTERN ASN1C_MsgSetAutomateModeAck : public ASN1CType {
public:
   ASN1T_MsgSetAutomateModeAck& msgData;
   ASN1C_MsgSetAutomateModeAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgSetAutomateModeAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgSetAutomateModeAck (ASN1CTXT* ctxt_p, ASN1T_MsgSetAutomateModeAck* pvalue);
EXTERN int asn1PD_MsgSetAutomateModeAck (ASN1CTXT* ctxt_p, ASN1T_MsgSetAutomateModeAck* pvalue);

EXTERN void asn1Print_MsgSetAutomateModeAck (ASN1ConstCharPtr name, ASN1T_MsgSetAutomateModeAck* pvalue);

#endif

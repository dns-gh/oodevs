/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 16-Nov-2005.
 */
#ifndef SCIPIOHIERARCHIE_H
#define SCIPIOHIERARCHIE_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  EnumDiplomatie                                            */
/*                                                            */
/**************************************************************/

struct EXTERN EnumDiplomatie {
   enum Root {
      inconnu = 0,
      ami = 1,
      ennemi = 2,
      neutre = 3
   } ;
} ;

typedef EnumDiplomatie::Root ASN1T_EnumDiplomatie;

class EXTERN ASN1C_EnumDiplomatie : public ASN1CType {
public:
   ASN1T_EnumDiplomatie& msgData;
   ASN1C_EnumDiplomatie (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumDiplomatie& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumDiplomatie (ASN1CTXT* ctxt_p, ASN1T_EnumDiplomatie value);
EXTERN int asn1PD_EnumDiplomatie (ASN1CTXT* ctxt_p, ASN1T_EnumDiplomatie* pvalue);

EXTERN void asn1Print_EnumDiplomatie (ASN1ConstCharPtr name, ASN1T_EnumDiplomatie* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgChangeDiplomatie                                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgChangeDiplomatie {
   ASN1T_OID  oid_camp1;
   ASN1T_OID  oid_camp2;
   ASN1T_EnumDiplomatie  diplomatie;
} ASN1T_MsgChangeDiplomatie;

class EXTERN ASN1C_MsgChangeDiplomatie : public ASN1CType {
public:
   ASN1T_MsgChangeDiplomatie& msgData;
   ASN1C_MsgChangeDiplomatie (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgChangeDiplomatie& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgChangeDiplomatie (ASN1CTXT* ctxt_p, ASN1T_MsgChangeDiplomatie* pvalue);
EXTERN int asn1PD_MsgChangeDiplomatie (ASN1CTXT* ctxt_p, ASN1T_MsgChangeDiplomatie* pvalue);

EXTERN void asn1Print_MsgChangeDiplomatie (ASN1ConstCharPtr name, ASN1T_MsgChangeDiplomatie* pvalue);

/**************************************************************/
/*                                                            */
/*  EnumChangeDiplomatieErrorCode                             */
/*                                                            */
/**************************************************************/

struct EXTERN EnumChangeDiplomatieErrorCode {
   enum Root {
      no_error = 0,
      error_invalid_camp = 1
   } ;
} ;

typedef EnumChangeDiplomatieErrorCode::Root ASN1T_EnumChangeDiplomatieErrorCode;

class EXTERN ASN1C_EnumChangeDiplomatieErrorCode : public ASN1CType {
public:
   ASN1T_EnumChangeDiplomatieErrorCode& msgData;
   ASN1C_EnumChangeDiplomatieErrorCode (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumChangeDiplomatieErrorCode& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumChangeDiplomatieErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumChangeDiplomatieErrorCode value);
EXTERN int asn1PD_EnumChangeDiplomatieErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumChangeDiplomatieErrorCode* pvalue);

EXTERN void asn1Print_EnumChangeDiplomatieErrorCode (ASN1ConstCharPtr name, ASN1T_EnumChangeDiplomatieErrorCode* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgChangeDiplomatieAck                                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgChangeDiplomatieAck {
   ASN1T_OID  oid_camp1;
   ASN1T_OID  oid_camp2;
   ASN1T_EnumDiplomatie  diplomatie;
   ASN1T_EnumChangeDiplomatieErrorCode  error_code;
} ASN1T_MsgChangeDiplomatieAck;

class EXTERN ASN1C_MsgChangeDiplomatieAck : public ASN1CType {
public:
   ASN1T_MsgChangeDiplomatieAck& msgData;
   ASN1C_MsgChangeDiplomatieAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgChangeDiplomatieAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgChangeDiplomatieAck (ASN1CTXT* ctxt_p, ASN1T_MsgChangeDiplomatieAck* pvalue);
EXTERN int asn1PD_MsgChangeDiplomatieAck (ASN1CTXT* ctxt_p, ASN1T_MsgChangeDiplomatieAck* pvalue);

EXTERN void asn1Print_MsgChangeDiplomatieAck (ASN1ConstCharPtr name, ASN1T_MsgChangeDiplomatieAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgChangeGroupeConnaissance                               */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgChangeGroupeConnaissance {
   ASN1T_Automate  oid_automate;
   ASN1T_OID  oid_camp;
   ASN1T_OID  oid_groupe_connaissance;
} ASN1T_MsgChangeGroupeConnaissance;

class EXTERN ASN1C_MsgChangeGroupeConnaissance : public ASN1CType {
public:
   ASN1T_MsgChangeGroupeConnaissance& msgData;
   ASN1C_MsgChangeGroupeConnaissance (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgChangeGroupeConnaissance& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgChangeGroupeConnaissance (ASN1CTXT* ctxt_p, ASN1T_MsgChangeGroupeConnaissance* pvalue);
EXTERN int asn1PD_MsgChangeGroupeConnaissance (ASN1CTXT* ctxt_p, ASN1T_MsgChangeGroupeConnaissance* pvalue);

EXTERN void asn1Print_MsgChangeGroupeConnaissance (ASN1ConstCharPtr name, ASN1T_MsgChangeGroupeConnaissance* pvalue);

/**************************************************************/
/*                                                            */
/*  EnumChangeGroupeConnaissanceErrorCode                     */
/*                                                            */
/**************************************************************/

struct EXTERN EnumChangeGroupeConnaissanceErrorCode {
   enum Root {
      no_error = 0,
      error_invalid_automate = 1,
      error_invalid_camp = 2,
      error_invalid_groupe_connaissance = 3
   } ;
} ;

typedef EnumChangeGroupeConnaissanceErrorCode::Root ASN1T_EnumChangeGroupeConnaissanceErrorCode;

class EXTERN ASN1C_EnumChangeGroupeConnaissanceErrorCode : public ASN1CType {
public:
   ASN1T_EnumChangeGroupeConnaissanceErrorCode& msgData;
   ASN1C_EnumChangeGroupeConnaissanceErrorCode (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumChangeGroupeConnaissanceErrorCode& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumChangeGroupeConnaissanceErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumChangeGroupeConnaissanceErrorCode value);
EXTERN int asn1PD_EnumChangeGroupeConnaissanceErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumChangeGroupeConnaissanceErrorCode* pvalue);

EXTERN void asn1Print_EnumChangeGroupeConnaissanceErrorCode (ASN1ConstCharPtr name, ASN1T_EnumChangeGroupeConnaissanceErrorCode* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgChangeGroupeConnaissanceAck                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgChangeGroupeConnaissanceAck {
   ASN1T_Automate  oid_automate;
   ASN1T_OID  oid_camp;
   ASN1T_OID  oid_groupe_connaissance;
   ASN1T_EnumChangeGroupeConnaissanceErrorCode  error_code;
} ASN1T_MsgChangeGroupeConnaissanceAck;

class EXTERN ASN1C_MsgChangeGroupeConnaissanceAck : public ASN1CType {
public:
   ASN1T_MsgChangeGroupeConnaissanceAck& msgData;
   ASN1C_MsgChangeGroupeConnaissanceAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgChangeGroupeConnaissanceAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgChangeGroupeConnaissanceAck (ASN1CTXT* ctxt_p, ASN1T_MsgChangeGroupeConnaissanceAck* pvalue);
EXTERN int asn1PD_MsgChangeGroupeConnaissanceAck (ASN1CTXT* ctxt_p, ASN1T_MsgChangeGroupeConnaissanceAck* pvalue);

EXTERN void asn1Print_MsgChangeGroupeConnaissanceAck (ASN1ConstCharPtr name, ASN1T_MsgChangeGroupeConnaissanceAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgChangeLiensLogistiques                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgChangeLiensLogistiques {
   struct {
      unsigned oid_tc2Present : 1;
      unsigned oid_maintenancePresent : 1;
      unsigned oid_santePresent : 1;
      unsigned oid_ravitaillementPresent : 1;
   } m;
   ASN1T_Automate  oid_automate;
   ASN1T_Automate  oid_tc2;
   ASN1T_Automate  oid_maintenance;
   ASN1T_Automate  oid_sante;
   ASN1T_Automate  oid_ravitaillement;

   ASN1T_MsgChangeLiensLogistiques () {
      m.oid_tc2Present = 0;
      m.oid_maintenancePresent = 0;
      m.oid_santePresent = 0;
      m.oid_ravitaillementPresent = 0;
   }
} ASN1T_MsgChangeLiensLogistiques;

class EXTERN ASN1C_MsgChangeLiensLogistiques : public ASN1CType {
public:
   ASN1T_MsgChangeLiensLogistiques& msgData;
   ASN1C_MsgChangeLiensLogistiques (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgChangeLiensLogistiques& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgChangeLiensLogistiques (ASN1CTXT* ctxt_p, ASN1T_MsgChangeLiensLogistiques* pvalue);
EXTERN int asn1PD_MsgChangeLiensLogistiques (ASN1CTXT* ctxt_p, ASN1T_MsgChangeLiensLogistiques* pvalue);

EXTERN void asn1Print_MsgChangeLiensLogistiques (ASN1ConstCharPtr name, ASN1T_MsgChangeLiensLogistiques* pvalue);

/**************************************************************/
/*                                                            */
/*  EnumChangeLiensLogistiquesErrorCode                       */
/*                                                            */
/**************************************************************/

struct EXTERN EnumChangeLiensLogistiquesErrorCode {
   enum Root {
      no_error = 0,
      error_invalid_automate = 1,
      error_invalid_automate_tc2 = 2,
      error_invalid_automate_maintenance = 3,
      error_invalid_automate_sante = 4,
      error_invalid_automate_ravitaillement = 5,
      error_unit_surrendered = 6
   } ;
} ;

typedef EnumChangeLiensLogistiquesErrorCode::Root ASN1T_EnumChangeLiensLogistiquesErrorCode;

class EXTERN ASN1C_EnumChangeLiensLogistiquesErrorCode : public ASN1CType {
public:
   ASN1T_EnumChangeLiensLogistiquesErrorCode& msgData;
   ASN1C_EnumChangeLiensLogistiquesErrorCode (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumChangeLiensLogistiquesErrorCode& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumChangeLiensLogistiquesErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumChangeLiensLogistiquesErrorCode value);
EXTERN int asn1PD_EnumChangeLiensLogistiquesErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumChangeLiensLogistiquesErrorCode* pvalue);

EXTERN void asn1Print_EnumChangeLiensLogistiquesErrorCode (ASN1ConstCharPtr name, ASN1T_EnumChangeLiensLogistiquesErrorCode* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgChangeLiensLogistiquesAck                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgChangeLiensLogistiquesAck {
   struct {
      unsigned oid_tc2Present : 1;
      unsigned oid_maintenancePresent : 1;
      unsigned oid_santePresent : 1;
      unsigned oid_ravitaillementPresent : 1;
   } m;
   ASN1T_Automate  oid_automate;
   ASN1T_Automate  oid_tc2;
   ASN1T_Automate  oid_maintenance;
   ASN1T_Automate  oid_sante;
   ASN1T_Automate  oid_ravitaillement;
   ASN1T_EnumChangeLiensLogistiquesErrorCode  error_code;

   ASN1T_MsgChangeLiensLogistiquesAck () {
      m.oid_tc2Present = 0;
      m.oid_maintenancePresent = 0;
      m.oid_santePresent = 0;
      m.oid_ravitaillementPresent = 0;
   }
} ASN1T_MsgChangeLiensLogistiquesAck;

class EXTERN ASN1C_MsgChangeLiensLogistiquesAck : public ASN1CType {
public:
   ASN1T_MsgChangeLiensLogistiquesAck& msgData;
   ASN1C_MsgChangeLiensLogistiquesAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgChangeLiensLogistiquesAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgChangeLiensLogistiquesAck (ASN1CTXT* ctxt_p, ASN1T_MsgChangeLiensLogistiquesAck* pvalue);
EXTERN int asn1PD_MsgChangeLiensLogistiquesAck (ASN1CTXT* ctxt_p, ASN1T_MsgChangeLiensLogistiquesAck* pvalue);

EXTERN void asn1Print_MsgChangeLiensLogistiquesAck (ASN1ConstCharPtr name, ASN1T_MsgChangeLiensLogistiquesAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgChangeAutomate                                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgChangeAutomate {
   ASN1T_Agent  oid_pion;
   ASN1T_Automate  oid_automate;
} ASN1T_MsgChangeAutomate;

class EXTERN ASN1C_MsgChangeAutomate : public ASN1CType {
public:
   ASN1T_MsgChangeAutomate& msgData;
   ASN1C_MsgChangeAutomate (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgChangeAutomate& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgChangeAutomate (ASN1CTXT* ctxt_p, ASN1T_MsgChangeAutomate* pvalue);
EXTERN int asn1PD_MsgChangeAutomate (ASN1CTXT* ctxt_p, ASN1T_MsgChangeAutomate* pvalue);

EXTERN void asn1Print_MsgChangeAutomate (ASN1ConstCharPtr name, ASN1T_MsgChangeAutomate* pvalue);

/**************************************************************/
/*                                                            */
/*  EnumChangeAutomateErrorCode                               */
/*                                                            */
/**************************************************************/

struct EXTERN EnumChangeAutomateErrorCode {
   enum Root {
      no_error = 0,
      error_invalid_pion = 1,
      error_invalid_automate = 2,
      error_camps_incompatibles = 3
   } ;
} ;

typedef EnumChangeAutomateErrorCode::Root ASN1T_EnumChangeAutomateErrorCode;

class EXTERN ASN1C_EnumChangeAutomateErrorCode : public ASN1CType {
public:
   ASN1T_EnumChangeAutomateErrorCode& msgData;
   ASN1C_EnumChangeAutomateErrorCode (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumChangeAutomateErrorCode& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumChangeAutomateErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumChangeAutomateErrorCode value);
EXTERN int asn1PD_EnumChangeAutomateErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumChangeAutomateErrorCode* pvalue);

EXTERN void asn1Print_EnumChangeAutomateErrorCode (ASN1ConstCharPtr name, ASN1T_EnumChangeAutomateErrorCode* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgChangeAutomateAck                                      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgChangeAutomateAck {
   ASN1T_Agent  oid_pion;
   ASN1T_Automate  oid_automate;
   ASN1T_EnumChangeAutomateErrorCode  error_code;
} ASN1T_MsgChangeAutomateAck;

class EXTERN ASN1C_MsgChangeAutomateAck : public ASN1CType {
public:
   ASN1T_MsgChangeAutomateAck& msgData;
   ASN1C_MsgChangeAutomateAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgChangeAutomateAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgChangeAutomateAck (ASN1CTXT* ctxt_p, ASN1T_MsgChangeAutomateAck* pvalue);
EXTERN int asn1PD_MsgChangeAutomateAck (ASN1CTXT* ctxt_p, ASN1T_MsgChangeAutomateAck* pvalue);

EXTERN void asn1Print_MsgChangeAutomateAck (ASN1ConstCharPtr name, ASN1T_MsgChangeAutomateAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgAutomateCreation                                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgAutomateCreation {
   struct {
      unsigned oid_tc2Present : 1;
      unsigned oid_maintenancePresent : 1;
      unsigned oid_santePresent : 1;
      unsigned oid_ravitaillementPresent : 1;
   } m;
   ASN1T_Automate  oid_automate;
   ASN1T_TypeAutomate  type_automate;
   ASN1VisibleString  nom;
   ASN1T_OID  oid_camp;
   ASN1T_OID  oid_groupe_connaissance;
   ASN1T_Automate  oid_tc2;
   ASN1T_Automate  oid_maintenance;
   ASN1T_Automate  oid_sante;
   ASN1T_Automate  oid_ravitaillement;

   ASN1T_MsgAutomateCreation () {
      m.oid_tc2Present = 0;
      m.oid_maintenancePresent = 0;
      m.oid_santePresent = 0;
      m.oid_ravitaillementPresent = 0;
   }
} ASN1T_MsgAutomateCreation;

class EXTERN ASN1C_MsgAutomateCreation : public ASN1CType {
public:
   ASN1T_MsgAutomateCreation& msgData;
   ASN1C_MsgAutomateCreation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgAutomateCreation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgAutomateCreation (ASN1CTXT* ctxt_p, ASN1T_MsgAutomateCreation* pvalue);
EXTERN int asn1PD_MsgAutomateCreation (ASN1CTXT* ctxt_p, ASN1T_MsgAutomateCreation* pvalue);

EXTERN void asn1Print_MsgAutomateCreation (ASN1ConstCharPtr name, ASN1T_MsgAutomateCreation* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPionCreation                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPionCreation {
   ASN1T_Agent  oid_pion;
   ASN1T_TypePion  type_pion;
   ASN1VisibleString  nom;
   ASN1T_Automate  oid_automate;
} ASN1T_MsgPionCreation;

class EXTERN ASN1C_MsgPionCreation : public ASN1CType {
public:
   ASN1T_MsgPionCreation& msgData;
   ASN1C_MsgPionCreation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPionCreation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPionCreation (ASN1CTXT* ctxt_p, ASN1T_MsgPionCreation* pvalue);
EXTERN int asn1PD_MsgPionCreation (ASN1CTXT* ctxt_p, ASN1T_MsgPionCreation* pvalue);

EXTERN void asn1Print_MsgPionCreation (ASN1ConstCharPtr name, ASN1T_MsgPionCreation* pvalue);

#endif

/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 16-Aug-2005.
 */
#ifndef SCIPIOAUTOMATEMISSION_ASA_H
#define SCIPIOAUTOMATEMISSION_ASA_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  EnumModeDeploiement                                       */
/*                                                            */
/**************************************************************/

struct EXTERN EnumModeDeploiement {
   enum Root {
      en_carre = 0,
      en_triangle = 1,
      nasse_trois_sections = 2,
      nasse_quatre_sections = 3,
      double_rideau = 4,
      simple_rideau = 5
   } ;
} ;

typedef EnumModeDeploiement::Root ASN1T_EnumModeDeploiement;

class EXTERN ASN1C_EnumModeDeploiement : public ASN1CType {
public:
   ASN1T_EnumModeDeploiement& msgData;
   ASN1C_EnumModeDeploiement (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumModeDeploiement& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumModeDeploiement (ASN1CTXT* ctxt_p, ASN1T_EnumModeDeploiement value);
EXTERN int asn1PD_EnumModeDeploiement (ASN1CTXT* ctxt_p, ASN1T_EnumModeDeploiement* pvalue);

EXTERN void asn1Print_EnumModeDeploiement (ASN1ConstCharPtr name, ASN1T_EnumModeDeploiement* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ASA_DefendreZone                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ASA_DefendreZone {
   ASN1T_Polygon  zone;
   ASN1T_Angle  angle;
} ASN1T_Mission_Automate_ASA_DefendreZone;

class EXTERN ASN1C_Mission_Automate_ASA_DefendreZone : public ASN1CType {
public:
   ASN1T_Mission_Automate_ASA_DefendreZone& msgData;
   ASN1C_Mission_Automate_ASA_DefendreZone (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ASA_DefendreZone& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ASA_DefendreZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_DefendreZone* pvalue);
EXTERN int asn1PD_Mission_Automate_ASA_DefendreZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_DefendreZone* pvalue);

EXTERN void asn1Print_Mission_Automate_ASA_DefendreZone (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ASA_DefendreZone* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ASA_DefendreSite                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ASA_DefendreSite {
   ASN1T_Polygon  zone;
   ASN1T_Angle  angle;
} ASN1T_Mission_Automate_ASA_DefendreSite;

class EXTERN ASN1C_Mission_Automate_ASA_DefendreSite : public ASN1CType {
public:
   ASN1T_Mission_Automate_ASA_DefendreSite& msgData;
   ASN1C_Mission_Automate_ASA_DefendreSite (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ASA_DefendreSite& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ASA_DefendreSite (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_DefendreSite* pvalue);
EXTERN int asn1PD_Mission_Automate_ASA_DefendreSite (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_DefendreSite* pvalue);

EXTERN void asn1Print_Mission_Automate_ASA_DefendreSite (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ASA_DefendreSite* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ASA_Surveiller                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ASA_Surveiller {
   ASN1T_Polygon  zone;
   ASN1T_Angle  angle;
} ASN1T_Mission_Automate_ASA_Surveiller;

class EXTERN ASN1C_Mission_Automate_ASA_Surveiller : public ASN1CType {
public:
   ASN1T_Mission_Automate_ASA_Surveiller& msgData;
   ASN1C_Mission_Automate_ASA_Surveiller (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ASA_Surveiller& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ASA_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_Surveiller* pvalue);
EXTERN int asn1PD_Mission_Automate_ASA_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_Surveiller* pvalue);

EXTERN void asn1Print_Mission_Automate_ASA_Surveiller (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ASA_Surveiller* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ASA_DefendreItineraire                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ASA_DefendreItineraire {
   ASN1T_Itineraire  itineraire;
} ASN1T_Mission_Automate_ASA_DefendreItineraire;

class EXTERN ASN1C_Mission_Automate_ASA_DefendreItineraire : public ASN1CType {
public:
   ASN1T_Mission_Automate_ASA_DefendreItineraire& msgData;
   ASN1C_Mission_Automate_ASA_DefendreItineraire (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ASA_DefendreItineraire& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ASA_DefendreItineraire (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_DefendreItineraire* pvalue);
EXTERN int asn1PD_Mission_Automate_ASA_DefendreItineraire (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_DefendreItineraire* pvalue);

EXTERN void asn1Print_Mission_Automate_ASA_DefendreItineraire (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ASA_DefendreItineraire* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ASA_MISTRAL_DefendreZone                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ASA_MISTRAL_DefendreZone {
   ASN1T_Polygon  zone;
   ASN1T_EnumModeDeploiement  mode_deploiement;
   ASN1T_Angle  angle;
} ASN1T_Mission_Automate_ASA_MISTRAL_DefendreZone;

class EXTERN ASN1C_Mission_Automate_ASA_MISTRAL_DefendreZone : public ASN1CType {
public:
   ASN1T_Mission_Automate_ASA_MISTRAL_DefendreZone& msgData;
   ASN1C_Mission_Automate_ASA_MISTRAL_DefendreZone (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ASA_MISTRAL_DefendreZone& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ASA_MISTRAL_DefendreZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_MISTRAL_DefendreZone* pvalue);
EXTERN int asn1PD_Mission_Automate_ASA_MISTRAL_DefendreZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_MISTRAL_DefendreZone* pvalue);

EXTERN void asn1Print_Mission_Automate_ASA_MISTRAL_DefendreZone (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ASA_MISTRAL_DefendreZone* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ASA_MISTRAL_DefendreSite                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ASA_MISTRAL_DefendreSite {
   ASN1T_Polygon  zone;
   ASN1T_EnumModeDeploiement  mode_deploiement;
   ASN1T_Angle  angle;
} ASN1T_Mission_Automate_ASA_MISTRAL_DefendreSite;

class EXTERN ASN1C_Mission_Automate_ASA_MISTRAL_DefendreSite : public ASN1CType {
public:
   ASN1T_Mission_Automate_ASA_MISTRAL_DefendreSite& msgData;
   ASN1C_Mission_Automate_ASA_MISTRAL_DefendreSite (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ASA_MISTRAL_DefendreSite& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ASA_MISTRAL_DefendreSite (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_MISTRAL_DefendreSite* pvalue);
EXTERN int asn1PD_Mission_Automate_ASA_MISTRAL_DefendreSite (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_MISTRAL_DefendreSite* pvalue);

EXTERN void asn1Print_Mission_Automate_ASA_MISTRAL_DefendreSite (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ASA_MISTRAL_DefendreSite* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ASA_MISTRAL_Surveiller                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ASA_MISTRAL_Surveiller {
   ASN1T_Polygon  zone;
   ASN1T_EnumModeDeploiement  mode_deploiement;
   ASN1T_Angle  angle;
} ASN1T_Mission_Automate_ASA_MISTRAL_Surveiller;

class EXTERN ASN1C_Mission_Automate_ASA_MISTRAL_Surveiller : public ASN1CType {
public:
   ASN1T_Mission_Automate_ASA_MISTRAL_Surveiller& msgData;
   ASN1C_Mission_Automate_ASA_MISTRAL_Surveiller (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ASA_MISTRAL_Surveiller& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ASA_MISTRAL_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_MISTRAL_Surveiller* pvalue);
EXTERN int asn1PD_Mission_Automate_ASA_MISTRAL_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_MISTRAL_Surveiller* pvalue);

EXTERN void asn1Print_Mission_Automate_ASA_MISTRAL_Surveiller (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ASA_MISTRAL_Surveiller* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ASA_HAWK_DefendreZone                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ASA_HAWK_DefendreZone {
   ASN1T_Polygon  zone;
   ASN1T_ListPoint  positions_sections;
} ASN1T_Mission_Automate_ASA_HAWK_DefendreZone;

class EXTERN ASN1C_Mission_Automate_ASA_HAWK_DefendreZone : public ASN1CType {
public:
   ASN1T_Mission_Automate_ASA_HAWK_DefendreZone& msgData;
   ASN1C_Mission_Automate_ASA_HAWK_DefendreZone (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ASA_HAWK_DefendreZone& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ASA_HAWK_DefendreZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_HAWK_DefendreZone* pvalue);
EXTERN int asn1PD_Mission_Automate_ASA_HAWK_DefendreZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_HAWK_DefendreZone* pvalue);

EXTERN void asn1Print_Mission_Automate_ASA_HAWK_DefendreZone (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ASA_HAWK_DefendreZone* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ASA_HAWK_DefendreSite                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ASA_HAWK_DefendreSite {
   ASN1T_Polygon  zone;
   ASN1T_ListPoint  positions_sections;
} ASN1T_Mission_Automate_ASA_HAWK_DefendreSite;

class EXTERN ASN1C_Mission_Automate_ASA_HAWK_DefendreSite : public ASN1CType {
public:
   ASN1T_Mission_Automate_ASA_HAWK_DefendreSite& msgData;
   ASN1C_Mission_Automate_ASA_HAWK_DefendreSite (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ASA_HAWK_DefendreSite& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ASA_HAWK_DefendreSite (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_HAWK_DefendreSite* pvalue);
EXTERN int asn1PD_Mission_Automate_ASA_HAWK_DefendreSite (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_HAWK_DefendreSite* pvalue);

EXTERN void asn1Print_Mission_Automate_ASA_HAWK_DefendreSite (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ASA_HAWK_DefendreSite* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ASA_HAWK_Surveiller                      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ASA_HAWK_Surveiller {
   ASN1T_Polygon  zone;
   ASN1T_ListPoint  positions_sections;
} ASN1T_Mission_Automate_ASA_HAWK_Surveiller;

class EXTERN ASN1C_Mission_Automate_ASA_HAWK_Surveiller : public ASN1CType {
public:
   ASN1T_Mission_Automate_ASA_HAWK_Surveiller& msgData;
   ASN1C_Mission_Automate_ASA_HAWK_Surveiller (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ASA_HAWK_Surveiller& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ASA_HAWK_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_HAWK_Surveiller* pvalue);
EXTERN int asn1PD_Mission_Automate_ASA_HAWK_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_HAWK_Surveiller* pvalue);

EXTERN void asn1Print_Mission_Automate_ASA_HAWK_Surveiller (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ASA_HAWK_Surveiller* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ASA_DefendreUnites                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ASA_DefendreUnites {
   ASN1T_ListAgent  unites;
} ASN1T_Mission_Automate_ASA_DefendreUnites;

class EXTERN ASN1C_Mission_Automate_ASA_DefendreUnites : public ASN1CType {
public:
   ASN1T_Mission_Automate_ASA_DefendreUnites& msgData;
   ASN1C_Mission_Automate_ASA_DefendreUnites (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ASA_DefendreUnites& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ASA_DefendreUnites (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_DefendreUnites* pvalue);
EXTERN int asn1PD_Mission_Automate_ASA_DefendreUnites (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_DefendreUnites* pvalue);

EXTERN void asn1Print_Mission_Automate_ASA_DefendreUnites (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ASA_DefendreUnites* pvalue);

#endif

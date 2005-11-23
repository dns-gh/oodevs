/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 22-Nov-2005.
 */
#ifndef SCIPIOPIONMISSION_ASA_H
#define SCIPIOPIONMISSION_ASA_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASA_DefendreSite                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ASA_DefendreSite {
   ASN1T_Polygon  site;
} ASN1T_Mission_Pion_ASA_DefendreSite;

class EXTERN ASN1C_Mission_Pion_ASA_DefendreSite : public ASN1CType {
public:
   ASN1T_Mission_Pion_ASA_DefendreSite& msgData;
   ASN1C_Mission_Pion_ASA_DefendreSite (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASA_DefendreSite& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ASA_DefendreSite (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_DefendreSite* pvalue);
EXTERN int asn1PD_Mission_Pion_ASA_DefendreSite (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_DefendreSite* pvalue);

EXTERN void asn1Print_Mission_Pion_ASA_DefendreSite (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ASA_DefendreSite* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASA_DefendreSiteAPartirPosition              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ASA_DefendreSiteAPartirPosition {
   ASN1T_Polygon  site;
   ASN1T_Point  point_de_deploiement;
} ASN1T_Mission_Pion_ASA_DefendreSiteAPartirPosition;

class EXTERN ASN1C_Mission_Pion_ASA_DefendreSiteAPartirPosition : public ASN1CType {
public:
   ASN1T_Mission_Pion_ASA_DefendreSiteAPartirPosition& msgData;
   ASN1C_Mission_Pion_ASA_DefendreSiteAPartirPosition (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASA_DefendreSiteAPartirPosition& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ASA_DefendreSiteAPartirPosition (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_DefendreSiteAPartirPosition* pvalue);
EXTERN int asn1PD_Mission_Pion_ASA_DefendreSiteAPartirPosition (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_DefendreSiteAPartirPosition* pvalue);

EXTERN void asn1Print_Mission_Pion_ASA_DefendreSiteAPartirPosition (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ASA_DefendreSiteAPartirPosition* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASA_DefendreZone                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ASA_DefendreZone {
   ASN1T_Polygon  zone;
} ASN1T_Mission_Pion_ASA_DefendreZone;

class EXTERN ASN1C_Mission_Pion_ASA_DefendreZone : public ASN1CType {
public:
   ASN1T_Mission_Pion_ASA_DefendreZone& msgData;
   ASN1C_Mission_Pion_ASA_DefendreZone (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASA_DefendreZone& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ASA_DefendreZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_DefendreZone* pvalue);
EXTERN int asn1PD_Mission_Pion_ASA_DefendreZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_DefendreZone* pvalue);

EXTERN void asn1Print_Mission_Pion_ASA_DefendreZone (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ASA_DefendreZone* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASA_DefendreZoneAPartirPosition              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ASA_DefendreZoneAPartirPosition {
   ASN1T_Polygon  zone;
   ASN1T_Point  point_de_deploiement;
} ASN1T_Mission_Pion_ASA_DefendreZoneAPartirPosition;

class EXTERN ASN1C_Mission_Pion_ASA_DefendreZoneAPartirPosition : public ASN1CType {
public:
   ASN1T_Mission_Pion_ASA_DefendreZoneAPartirPosition& msgData;
   ASN1C_Mission_Pion_ASA_DefendreZoneAPartirPosition (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASA_DefendreZoneAPartirPosition& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ASA_DefendreZoneAPartirPosition (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_DefendreZoneAPartirPosition* pvalue);
EXTERN int asn1PD_Mission_Pion_ASA_DefendreZoneAPartirPosition (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_DefendreZoneAPartirPosition* pvalue);

EXTERN void asn1Print_Mission_Pion_ASA_DefendreZoneAPartirPosition (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ASA_DefendreZoneAPartirPosition* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASA_Surveiller                               */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ASA_Surveiller {
   ASN1T_Point  point_de_deploiement;
} ASN1T_Mission_Pion_ASA_Surveiller;

class EXTERN ASN1C_Mission_Pion_ASA_Surveiller : public ASN1CType {
public:
   ASN1T_Mission_Pion_ASA_Surveiller& msgData;
   ASN1C_Mission_Pion_ASA_Surveiller (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASA_Surveiller& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ASA_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_Surveiller* pvalue);
EXTERN int asn1PD_Mission_Pion_ASA_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_Surveiller* pvalue);

EXTERN void asn1Print_Mission_Pion_ASA_Surveiller (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ASA_Surveiller* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASA_DefendreUnites                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ASA_DefendreUnites {
   ASN1T_ListAgent  unites;
} ASN1T_Mission_Pion_ASA_DefendreUnites;

class EXTERN ASN1C_Mission_Pion_ASA_DefendreUnites : public ASN1CType {
public:
   ASN1T_Mission_Pion_ASA_DefendreUnites& msgData;
   ASN1C_Mission_Pion_ASA_DefendreUnites (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASA_DefendreUnites& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ASA_DefendreUnites (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_DefendreUnites* pvalue);
EXTERN int asn1PD_Mission_Pion_ASA_DefendreUnites (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_DefendreUnites* pvalue);

EXTERN void asn1Print_Mission_Pion_ASA_DefendreUnites (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ASA_DefendreUnites* pvalue);

/**************************************************************/
/*                                                            */
/*  EnumModeDeploiementSectionMistral                         */
/*                                                            */
/**************************************************************/

struct EXTERN EnumModeDeploiementSectionMistral {
   enum Root {
      omnidirectionnel = 0,
      directionnel = 1
   } ;
} ;

typedef EnumModeDeploiementSectionMistral::Root ASN1T_EnumModeDeploiementSectionMistral;

class EXTERN ASN1C_EnumModeDeploiementSectionMistral : public ASN1CType {
public:
   ASN1T_EnumModeDeploiementSectionMistral& msgData;
   ASN1C_EnumModeDeploiementSectionMistral (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumModeDeploiementSectionMistral& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumModeDeploiementSectionMistral (ASN1CTXT* ctxt_p, ASN1T_EnumModeDeploiementSectionMistral value);
EXTERN int asn1PD_EnumModeDeploiementSectionMistral (ASN1CTXT* ctxt_p, ASN1T_EnumModeDeploiementSectionMistral* pvalue);

EXTERN void asn1Print_EnumModeDeploiementSectionMistral (ASN1ConstCharPtr name, ASN1T_EnumModeDeploiementSectionMistral* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASA_MISTRAL_DefendreSite                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSite {
   ASN1T_Polygon  site;
   ASN1T_EnumModeDeploiementSectionMistral  mode_deploiement;
} ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSite;

class EXTERN ASN1C_Mission_Pion_ASA_MISTRAL_DefendreSite : public ASN1CType {
public:
   ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSite& msgData;
   ASN1C_Mission_Pion_ASA_MISTRAL_DefendreSite (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSite& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ASA_MISTRAL_DefendreSite (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSite* pvalue);
EXTERN int asn1PD_Mission_Pion_ASA_MISTRAL_DefendreSite (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSite* pvalue);

EXTERN void asn1Print_Mission_Pion_ASA_MISTRAL_DefendreSite (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSite* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition {
   ASN1T_Polygon  site;
   ASN1T_Point  point_de_deploiement;
   ASN1T_EnumModeDeploiementSectionMistral  mode_deploiement;
} ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition;

class EXTERN ASN1C_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition : public ASN1CType {
public:
   ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition& msgData;
   ASN1C_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition* pvalue);
EXTERN int asn1PD_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition* pvalue);

EXTERN void asn1Print_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASA_MISTRAL_DefendreZone                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZone {
   ASN1T_Polygon  zone;
   ASN1T_EnumModeDeploiementSectionMistral  mode_deploiement;
} ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZone;

class EXTERN ASN1C_Mission_Pion_ASA_MISTRAL_DefendreZone : public ASN1CType {
public:
   ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZone& msgData;
   ASN1C_Mission_Pion_ASA_MISTRAL_DefendreZone (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZone& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ASA_MISTRAL_DefendreZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZone* pvalue);
EXTERN int asn1PD_Mission_Pion_ASA_MISTRAL_DefendreZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZone* pvalue);

EXTERN void asn1Print_Mission_Pion_ASA_MISTRAL_DefendreZone (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZone* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition {
   ASN1T_Polygon  zone;
   ASN1T_Point  point_de_deploiement;
   ASN1T_EnumModeDeploiementSectionMistral  mode_deploiement;
} ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition;

class EXTERN ASN1C_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition : public ASN1CType {
public:
   ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition& msgData;
   ASN1C_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition* pvalue);
EXTERN int asn1PD_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition* pvalue);

EXTERN void asn1Print_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASA_MISTRAL_Surveiller                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ASA_MISTRAL_Surveiller {
   ASN1T_Point  point_de_deploiement;
   ASN1T_EnumModeDeploiementSectionMistral  mode_deploiement;
} ASN1T_Mission_Pion_ASA_MISTRAL_Surveiller;

class EXTERN ASN1C_Mission_Pion_ASA_MISTRAL_Surveiller : public ASN1CType {
public:
   ASN1T_Mission_Pion_ASA_MISTRAL_Surveiller& msgData;
   ASN1C_Mission_Pion_ASA_MISTRAL_Surveiller (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASA_MISTRAL_Surveiller& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ASA_MISTRAL_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_MISTRAL_Surveiller* pvalue);
EXTERN int asn1PD_Mission_Pion_ASA_MISTRAL_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_MISTRAL_Surveiller* pvalue);

EXTERN void asn1Print_Mission_Pion_ASA_MISTRAL_Surveiller (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ASA_MISTRAL_Surveiller* pvalue);

#endif

/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 19-Dec-2005.
 */
#ifndef SCIPIOPIONMISSION_ASS_H
#define SCIPIOPIONMISSION_ASS_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASS_AssurerMiseEnOeuvre                      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvre {
   ASN1T_KnowledgeObject  zone_implantation;
   ASN1BOOL  presence_contre_batterie;
} ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvre;

class EXTERN ASN1C_Mission_Pion_ASS_AssurerMiseEnOeuvre : public ASN1CType {
public:
   ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvre& msgData;
   ASN1C_Mission_Pion_ASS_AssurerMiseEnOeuvre (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvre& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ASS_AssurerMiseEnOeuvre (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvre* pvalue);
EXTERN int asn1PD_Mission_Pion_ASS_AssurerMiseEnOeuvre (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvre* pvalue);

EXTERN void asn1Print_Mission_Pion_ASS_AssurerMiseEnOeuvre (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvre* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition {
   ASN1T_Point  position_implantation;
   ASN1BOOL  presence_contre_batterie;
} ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition;

class EXTERN ASN1C_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition : public ASN1CType {
public:
   ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition& msgData;
   ASN1C_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition* pvalue);
EXTERN int asn1PD_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition* pvalue);

EXTERN void asn1Print_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASS_ReconnaitreZoneDeploiement               */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ASS_ReconnaitreZoneDeploiement {
   ASN1T_Point  position_deploiement;
} ASN1T_Mission_Pion_ASS_ReconnaitreZoneDeploiement;

class EXTERN ASN1C_Mission_Pion_ASS_ReconnaitreZoneDeploiement : public ASN1CType {
public:
   ASN1T_Mission_Pion_ASS_ReconnaitreZoneDeploiement& msgData;
   ASN1C_Mission_Pion_ASS_ReconnaitreZoneDeploiement (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASS_ReconnaitreZoneDeploiement& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ASS_ReconnaitreZoneDeploiement (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASS_ReconnaitreZoneDeploiement* pvalue);
EXTERN int asn1PD_Mission_Pion_ASS_ReconnaitreZoneDeploiement (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASS_ReconnaitreZoneDeploiement* pvalue);

EXTERN void asn1Print_Mission_Pion_ASS_ReconnaitreZoneDeploiement (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ASS_ReconnaitreZoneDeploiement* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASS_AcquerirLanceursEni                      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ASS_AcquerirLanceursEni {
   ASN1T_ListPolygon  zones_a_observer;
   ASN1T_KnowledgeObject  zone_implantation;
   ASN1BOOL  menance_roem;
} ASN1T_Mission_Pion_ASS_AcquerirLanceursEni;

class EXTERN ASN1C_Mission_Pion_ASS_AcquerirLanceursEni : public ASN1CType {
public:
   ASN1T_Mission_Pion_ASS_AcquerirLanceursEni& msgData;
   ASN1C_Mission_Pion_ASS_AcquerirLanceursEni (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASS_AcquerirLanceursEni& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ASS_AcquerirLanceursEni (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASS_AcquerirLanceursEni* pvalue);
EXTERN int asn1PD_Mission_Pion_ASS_AcquerirLanceursEni (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASS_AcquerirLanceursEni* pvalue);

EXTERN void asn1Print_Mission_Pion_ASS_AcquerirLanceursEni (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ASS_AcquerirLanceursEni* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASS_EO_AcquerirObjectifs                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ASS_EO_AcquerirObjectifs {
   ASN1T_Polygon  zone_a_observer;
   ASN1T_Automate  unite_a_appuyer;
   ASN1T_NatureAtlas  categories;
} ASN1T_Mission_Pion_ASS_EO_AcquerirObjectifs;

class EXTERN ASN1C_Mission_Pion_ASS_EO_AcquerirObjectifs : public ASN1CType {
public:
   ASN1T_Mission_Pion_ASS_EO_AcquerirObjectifs& msgData;
   ASN1C_Mission_Pion_ASS_EO_AcquerirObjectifs (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASS_EO_AcquerirObjectifs& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ASS_EO_AcquerirObjectifs (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASS_EO_AcquerirObjectifs* pvalue);
EXTERN int asn1PD_Mission_Pion_ASS_EO_AcquerirObjectifs (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASS_EO_AcquerirObjectifs* pvalue);

EXTERN void asn1Print_Mission_Pion_ASS_EO_AcquerirObjectifs (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ASS_EO_AcquerirObjectifs* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASS_EOP_AcquerirObjectifs                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ASS_EOP_AcquerirObjectifs {
   ASN1T_Polygon  zone_a_observer;
   ASN1T_Point  position_installation;
   ASN1T_Point  position_debarquement;
} ASN1T_Mission_Pion_ASS_EOP_AcquerirObjectifs;

class EXTERN ASN1C_Mission_Pion_ASS_EOP_AcquerirObjectifs : public ASN1CType {
public:
   ASN1T_Mission_Pion_ASS_EOP_AcquerirObjectifs& msgData;
   ASN1C_Mission_Pion_ASS_EOP_AcquerirObjectifs (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASS_EOP_AcquerirObjectifs& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ASS_EOP_AcquerirObjectifs (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASS_EOP_AcquerirObjectifs* pvalue);
EXTERN int asn1PD_Mission_Pion_ASS_EOP_AcquerirObjectifs (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASS_EOP_AcquerirObjectifs* pvalue);

EXTERN void asn1Print_Mission_Pion_ASS_EOP_AcquerirObjectifs (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ASS_EOP_AcquerirObjectifs* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASS_RATAC_AcquerirObjectifs                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ASS_RATAC_AcquerirObjectifs {
   ASN1T_Polygon  zone_a_observer;
   ASN1T_NatureAtlas  categories;
   ASN1T_Point  position_deploiement;
} ASN1T_Mission_Pion_ASS_RATAC_AcquerirObjectifs;

class EXTERN ASN1C_Mission_Pion_ASS_RATAC_AcquerirObjectifs : public ASN1CType {
public:
   ASN1T_Mission_Pion_ASS_RATAC_AcquerirObjectifs& msgData;
   ASN1C_Mission_Pion_ASS_RATAC_AcquerirObjectifs (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASS_RATAC_AcquerirObjectifs& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ASS_RATAC_AcquerirObjectifs (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASS_RATAC_AcquerirObjectifs* pvalue);
EXTERN int asn1PD_Mission_Pion_ASS_RATAC_AcquerirObjectifs (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASS_RATAC_AcquerirObjectifs* pvalue);

EXTERN void asn1Print_Mission_Pion_ASS_RATAC_AcquerirObjectifs (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ASS_RATAC_AcquerirObjectifs* pvalue);

#endif

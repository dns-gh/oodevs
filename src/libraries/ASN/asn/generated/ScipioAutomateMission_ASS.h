/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 23-Dec-2005.
 */
#ifndef SCIPIOAUTOMATEMISSION_ASS_H
#define SCIPIOAUTOMATEMISSION_ASS_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ASS_SeDeployer                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ASS_SeDeployer {
   ASN1T_Polygon  zone_deploiement;
} ASN1T_Mission_Automate_ASS_SeDeployer;

class EXTERN ASN1C_Mission_Automate_ASS_SeDeployer : public ASN1CType {
public:
   ASN1T_Mission_Automate_ASS_SeDeployer& msgData;
   ASN1C_Mission_Automate_ASS_SeDeployer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ASS_SeDeployer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ASS_SeDeployer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASS_SeDeployer* pvalue);
EXTERN int asn1PD_Mission_Automate_ASS_SeDeployer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASS_SeDeployer* pvalue);

EXTERN void asn1Print_Mission_Automate_ASS_SeDeployer (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ASS_SeDeployer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ASS_ReconnaitreZonesDeploiement          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ASS_ReconnaitreZonesDeploiement {
   ASN1T_ListPoint  positions_a_reconnaitre;
} ASN1T_Mission_Automate_ASS_ReconnaitreZonesDeploiement;

class EXTERN ASN1C_Mission_Automate_ASS_ReconnaitreZonesDeploiement : public ASN1CType {
public:
   ASN1T_Mission_Automate_ASS_ReconnaitreZonesDeploiement& msgData;
   ASN1C_Mission_Automate_ASS_ReconnaitreZonesDeploiement (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ASS_ReconnaitreZonesDeploiement& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ASS_ReconnaitreZonesDeploiement (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASS_ReconnaitreZonesDeploiement* pvalue);
EXTERN int asn1PD_Mission_Automate_ASS_ReconnaitreZonesDeploiement (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASS_ReconnaitreZonesDeploiement* pvalue);

EXTERN void asn1Print_Mission_Automate_ASS_ReconnaitreZonesDeploiement (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ASS_ReconnaitreZonesDeploiement* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ASS_AcquerirObjectifs                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ASS_AcquerirObjectifs {
   ASN1T_ListPolygon  zones_a_observer;
   ASN1T_ListPoint  positions_deploiement;
   ASN1T_NatureAtlas  categories;
} ASN1T_Mission_Automate_ASS_AcquerirObjectifs;

class EXTERN ASN1C_Mission_Automate_ASS_AcquerirObjectifs : public ASN1CType {
public:
   ASN1T_Mission_Automate_ASS_AcquerirObjectifs& msgData;
   ASN1C_Mission_Automate_ASS_AcquerirObjectifs (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ASS_AcquerirObjectifs& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ASS_AcquerirObjectifs (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASS_AcquerirObjectifs* pvalue);
EXTERN int asn1PD_Mission_Automate_ASS_AcquerirObjectifs (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASS_AcquerirObjectifs* pvalue);

EXTERN void asn1Print_Mission_Automate_ASS_AcquerirObjectifs (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ASS_AcquerirObjectifs* pvalue);

#endif

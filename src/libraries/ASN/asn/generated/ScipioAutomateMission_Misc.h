/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 17-Aug-2005.
 */
#ifndef SCIPIOAUTOMATEMISSION_MISC_H
#define SCIPIOAUTOMATEMISSION_MISC_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  Mission_Automate_Test_MoveTo                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_Test_MoveTo {
   ASN1T_Itineraire  itineraire;
} ASN1T_Mission_Automate_Test_MoveTo;

class EXTERN ASN1C_Mission_Automate_Test_MoveTo : public ASN1CType {
public:
   ASN1T_Mission_Automate_Test_MoveTo& msgData;
   ASN1C_Mission_Automate_Test_MoveTo (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_Test_MoveTo& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_Test_MoveTo (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_Test_MoveTo* pvalue);
EXTERN int asn1PD_Mission_Automate_Test_MoveTo (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_Test_MoveTo* pvalue);

EXTERN void asn1Print_Mission_Automate_Test_MoveTo (ASN1ConstCharPtr name, ASN1T_Mission_Automate_Test_MoveTo* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_Decrocher                                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_Decrocher {
   ASN1T_Polygon  point_regroupement;
} ASN1T_Mission_Automate_Decrocher;

class EXTERN ASN1C_Mission_Automate_Decrocher : public ASN1CType {
public:
   ASN1T_Mission_Automate_Decrocher& msgData;
   ASN1C_Mission_Automate_Decrocher (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_Decrocher& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_Decrocher (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_Decrocher* pvalue);
EXTERN int asn1PD_Mission_Automate_Decrocher (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_Decrocher* pvalue);

EXTERN void asn1Print_Mission_Automate_Decrocher (ASN1ConstCharPtr name, ASN1T_Mission_Automate_Decrocher* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ReleverSurPlace                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ReleverSurPlace {
   ASN1T_Automate  compagnie_a_relever;
   ASN1T_Polygon  zone_installation;
} ASN1T_Mission_Automate_ReleverSurPlace;

class EXTERN ASN1C_Mission_Automate_ReleverSurPlace : public ASN1CType {
public:
   ASN1T_Mission_Automate_ReleverSurPlace& msgData;
   ASN1C_Mission_Automate_ReleverSurPlace (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ReleverSurPlace& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ReleverSurPlace (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ReleverSurPlace* pvalue);
EXTERN int asn1PD_Mission_Automate_ReleverSurPlace (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ReleverSurPlace* pvalue);

EXTERN void asn1Print_Mission_Automate_ReleverSurPlace (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ReleverSurPlace* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_FaireMouvement                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_FaireMouvement {
   ASN1T_Itineraire  itineraire;
} ASN1T_Mission_Automate_FaireMouvement;

class EXTERN ASN1C_Mission_Automate_FaireMouvement : public ASN1CType {
public:
   ASN1T_Mission_Automate_FaireMouvement& msgData;
   ASN1C_Mission_Automate_FaireMouvement (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_FaireMouvement& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_FaireMouvement (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_FaireMouvement* pvalue);
EXTERN int asn1PD_Mission_Automate_FaireMouvement (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_FaireMouvement* pvalue);

EXTERN void asn1Print_Mission_Automate_FaireMouvement (ASN1ConstCharPtr name, ASN1T_Mission_Automate_FaireMouvement* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_SeFaireDecontaminer                      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_SeFaireDecontaminer {
   ASN1T_Polygon  zone_arrivee;
   ASN1T_KnowledgeObject  site_decontamination;
} ASN1T_Mission_Automate_SeFaireDecontaminer;

class EXTERN ASN1C_Mission_Automate_SeFaireDecontaminer : public ASN1CType {
public:
   ASN1T_Mission_Automate_SeFaireDecontaminer& msgData;
   ASN1C_Mission_Automate_SeFaireDecontaminer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_SeFaireDecontaminer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_SeFaireDecontaminer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_SeFaireDecontaminer* pvalue);
EXTERN int asn1PD_Mission_Automate_SeFaireDecontaminer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_SeFaireDecontaminer* pvalue);

EXTERN void asn1Print_Mission_Automate_SeFaireDecontaminer (ASN1ConstCharPtr name, ASN1T_Mission_Automate_SeFaireDecontaminer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_Franchir                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_Franchir {
   ASN1T_KnowledgeObject  pont_flottant;
   ASN1T_Polygon  zone_arrivee;
} ASN1T_Mission_Automate_Franchir;

class EXTERN ASN1C_Mission_Automate_Franchir : public ASN1CType {
public:
   ASN1T_Mission_Automate_Franchir& msgData;
   ASN1C_Mission_Automate_Franchir (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_Franchir& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_Franchir (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_Franchir* pvalue);
EXTERN int asn1PD_Mission_Automate_Franchir (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_Franchir* pvalue);

EXTERN void asn1Print_Mission_Automate_Franchir (ASN1ConstCharPtr name, ASN1T_Mission_Automate_Franchir* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_Stationner                               */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_Stationner {
   ASN1T_Polygon  zone_attente;
} ASN1T_Mission_Automate_Stationner;

class EXTERN ASN1C_Mission_Automate_Stationner : public ASN1CType {
public:
   ASN1T_Mission_Automate_Stationner& msgData;
   ASN1C_Mission_Automate_Stationner (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_Stationner& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_Stationner (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_Stationner* pvalue);
EXTERN int asn1PD_Mission_Automate_Stationner (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_Stationner* pvalue);

EXTERN void asn1Print_Mission_Automate_Stationner (ASN1ConstCharPtr name, ASN1T_Mission_Automate_Stationner* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_SeFaireTransporter                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_SeFaireTransporter {
   ASN1T_Polygon  zone_embarquement;
} ASN1T_Mission_Automate_SeFaireTransporter;

class EXTERN ASN1C_Mission_Automate_SeFaireTransporter : public ASN1CType {
public:
   ASN1T_Mission_Automate_SeFaireTransporter& msgData;
   ASN1C_Mission_Automate_SeFaireTransporter (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_SeFaireTransporter& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_SeFaireTransporter (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_SeFaireTransporter* pvalue);
EXTERN int asn1PD_Mission_Automate_SeFaireTransporter (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_SeFaireTransporter* pvalue);

EXTERN void asn1Print_Mission_Automate_SeFaireTransporter (ASN1ConstCharPtr name, ASN1T_Mission_Automate_SeFaireTransporter* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_SeRecompleter                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_SeRecompleter {
   ASN1T_Polygon  zone_ravitaillement;
} ASN1T_Mission_Automate_SeRecompleter;

class EXTERN ASN1C_Mission_Automate_SeRecompleter : public ASN1CType {
public:
   ASN1T_Mission_Automate_SeRecompleter& msgData;
   ASN1C_Mission_Automate_SeRecompleter (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_SeRecompleter& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_SeRecompleter (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_SeRecompleter* pvalue);
EXTERN int asn1PD_Mission_Automate_SeRecompleter (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_SeRecompleter* pvalue);

EXTERN void asn1Print_Mission_Automate_SeRecompleter (ASN1ConstCharPtr name, ASN1T_Mission_Automate_SeRecompleter* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_SeRendre                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_SeRendre {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Automate_SeRendre;

class EXTERN ASN1C_Mission_Automate_SeRendre : public ASN1CType {
public:
   ASN1T_Mission_Automate_SeRendre& msgData;
   ASN1C_Mission_Automate_SeRendre (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_SeRendre& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_SeRendre (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_SeRendre* pvalue);
EXTERN int asn1PD_Mission_Automate_SeRendre (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_SeRendre* pvalue);

EXTERN void asn1Print_Mission_Automate_SeRendre (ASN1ConstCharPtr name, ASN1T_Mission_Automate_SeRendre* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_FaireMouvementVersCampRefugies           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_FaireMouvementVersCampRefugies {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Automate_FaireMouvementVersCampRefugies;

class EXTERN ASN1C_Mission_Automate_FaireMouvementVersCampRefugies : public ASN1CType {
public:
   ASN1T_Mission_Automate_FaireMouvementVersCampRefugies& msgData;
   ASN1C_Mission_Automate_FaireMouvementVersCampRefugies (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_FaireMouvementVersCampRefugies& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_FaireMouvementVersCampRefugies (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_FaireMouvementVersCampRefugies* pvalue);
EXTERN int asn1PD_Mission_Automate_FaireMouvementVersCampRefugies (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_FaireMouvementVersCampRefugies* pvalue);

EXTERN void asn1Print_Mission_Automate_FaireMouvementVersCampRefugies (ASN1ConstCharPtr name, ASN1T_Mission_Automate_FaireMouvementVersCampRefugies* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_Generique                                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_Generique {
   ASN1T_ListPolygon  zones;
   ASN1T_ListPoint  points;
   ASN1T_ListItineraire  itineraires;
   ASN1T_ListAgent  unites;
   ASN1T_ListAutomate  automates;
   ASN1T_ListKnowledgeAgent  connaissances_agents;
   ASN1T_ListKnowledgeObject  connaissances_objets;
} ASN1T_Mission_Automate_Generique;

class EXTERN ASN1C_Mission_Automate_Generique : public ASN1CType {
public:
   ASN1T_Mission_Automate_Generique& msgData;
   ASN1C_Mission_Automate_Generique (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_Generique& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_Generique (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_Generique* pvalue);
EXTERN int asn1PD_Mission_Automate_Generique (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_Generique* pvalue);

EXTERN void asn1Print_Mission_Automate_Generique (ASN1ConstCharPtr name, ASN1T_Mission_Automate_Generique* pvalue);

#endif

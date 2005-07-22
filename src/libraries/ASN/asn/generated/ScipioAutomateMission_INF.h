/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 20-Jul-2005.
 */
#ifndef SCIPIOAUTOMATEMISSION_INF_H
#define SCIPIOAUTOMATEMISSION_INF_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  Mission_Automate_INF_AttaquerSouplesse                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_INF_AttaquerSouplesse {
   ASN1T_ListPoint  objectifs;
} ASN1T_Mission_Automate_INF_AttaquerSouplesse;

class EXTERN ASN1C_Mission_Automate_INF_AttaquerSouplesse : public ASN1CType {
public:
   ASN1T_Mission_Automate_INF_AttaquerSouplesse& msgData;
   ASN1C_Mission_Automate_INF_AttaquerSouplesse (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_INF_AttaquerSouplesse& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_INF_AttaquerSouplesse (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_AttaquerSouplesse* pvalue);
EXTERN int asn1PD_Mission_Automate_INF_AttaquerSouplesse (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_AttaquerSouplesse* pvalue);

EXTERN void asn1Print_Mission_Automate_INF_AttaquerSouplesse (ASN1ConstCharPtr name, ASN1T_Mission_Automate_INF_AttaquerSouplesse* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_INF_Appuyer                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_INF_Appuyer {
   ASN1T_Automate  compagnie;
} ASN1T_Mission_Automate_INF_Appuyer;

class EXTERN ASN1C_Mission_Automate_INF_Appuyer : public ASN1CType {
public:
   ASN1T_Mission_Automate_INF_Appuyer& msgData;
   ASN1C_Mission_Automate_INF_Appuyer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_INF_Appuyer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_INF_Appuyer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_Appuyer* pvalue);
EXTERN int asn1PD_Mission_Automate_INF_Appuyer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_Appuyer* pvalue);

EXTERN void asn1Print_Mission_Automate_INF_Appuyer (ASN1ConstCharPtr name, ASN1T_Mission_Automate_INF_Appuyer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_INF_Controler                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_INF_Controler {
   ASN1T_Polygon  zone;
   ASN1BOOL  preparer_terrain;
} ASN1T_Mission_Automate_INF_Controler;

class EXTERN ASN1C_Mission_Automate_INF_Controler : public ASN1CType {
public:
   ASN1T_Mission_Automate_INF_Controler& msgData;
   ASN1C_Mission_Automate_INF_Controler (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_INF_Controler& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_INF_Controler (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_Controler* pvalue);
EXTERN int asn1PD_Mission_Automate_INF_Controler (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_Controler* pvalue);

EXTERN void asn1Print_Mission_Automate_INF_Controler (ASN1ConstCharPtr name, ASN1T_Mission_Automate_INF_Controler* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_INF_Couvrir                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_INF_Couvrir {
   ASN1T_Automate  compagnie;
   ASN1T_ListPoint  objectifs;
} ASN1T_Mission_Automate_INF_Couvrir;

class EXTERN ASN1C_Mission_Automate_INF_Couvrir : public ASN1CType {
public:
   ASN1T_Mission_Automate_INF_Couvrir& msgData;
   ASN1C_Mission_Automate_INF_Couvrir (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_INF_Couvrir& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_INF_Couvrir (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_Couvrir* pvalue);
EXTERN int asn1PD_Mission_Automate_INF_Couvrir (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_Couvrir* pvalue);

EXTERN void asn1Print_Mission_Automate_INF_Couvrir (ASN1ConstCharPtr name, ASN1T_Mission_Automate_INF_Couvrir* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_INF_DonnerCoupArret                      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_INF_DonnerCoupArret {
   ASN1T_Polygon  zone_regroupement;
   ASN1BOOL  preparer_terrain;

   ASN1T_Mission_Automate_INF_DonnerCoupArret () {
      preparer_terrain = FALSE;
   }
} ASN1T_Mission_Automate_INF_DonnerCoupArret;

class EXTERN ASN1C_Mission_Automate_INF_DonnerCoupArret : public ASN1CType {
public:
   ASN1T_Mission_Automate_INF_DonnerCoupArret& msgData;
   ASN1C_Mission_Automate_INF_DonnerCoupArret (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_INF_DonnerCoupArret& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_INF_DonnerCoupArret (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_DonnerCoupArret* pvalue);
EXTERN int asn1PD_Mission_Automate_INF_DonnerCoupArret (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_DonnerCoupArret* pvalue);

EXTERN void asn1Print_Mission_Automate_INF_DonnerCoupArret (ASN1ConstCharPtr name, ASN1T_Mission_Automate_INF_DonnerCoupArret* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_INF_Eclairer                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_INF_Eclairer {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Automate_INF_Eclairer;

class EXTERN ASN1C_Mission_Automate_INF_Eclairer : public ASN1CType {
public:
   ASN1T_Mission_Automate_INF_Eclairer& msgData;
   ASN1C_Mission_Automate_INF_Eclairer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_INF_Eclairer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_INF_Eclairer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_Eclairer* pvalue);
EXTERN int asn1PD_Mission_Automate_INF_Eclairer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_Eclairer* pvalue);

EXTERN void asn1Print_Mission_Automate_INF_Eclairer (ASN1ConstCharPtr name, ASN1T_Mission_Automate_INF_Eclairer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_INF_FlancGarder                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_INF_FlancGarder {
   ASN1T_Automate  compagnie;
} ASN1T_Mission_Automate_INF_FlancGarder;

class EXTERN ASN1C_Mission_Automate_INF_FlancGarder : public ASN1CType {
public:
   ASN1T_Mission_Automate_INF_FlancGarder& msgData;
   ASN1C_Mission_Automate_INF_FlancGarder (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_INF_FlancGarder& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_INF_FlancGarder (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_FlancGarder* pvalue);
EXTERN int asn1PD_Mission_Automate_INF_FlancGarder (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_FlancGarder* pvalue);

EXTERN void asn1Print_Mission_Automate_INF_FlancGarder (ASN1ConstCharPtr name, ASN1T_Mission_Automate_INF_FlancGarder* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_INF_Freiner                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_INF_Freiner {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Automate_INF_Freiner;

class EXTERN ASN1C_Mission_Automate_INF_Freiner : public ASN1CType {
public:
   ASN1T_Mission_Automate_INF_Freiner& msgData;
   ASN1C_Mission_Automate_INF_Freiner (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_INF_Freiner& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_INF_Freiner (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_Freiner* pvalue);
EXTERN int asn1PD_Mission_Automate_INF_Freiner (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_Freiner* pvalue);

EXTERN void asn1Print_Mission_Automate_INF_Freiner (ASN1ConstCharPtr name, ASN1T_Mission_Automate_INF_Freiner* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_INF_Harceler                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_INF_Harceler {
   ASN1T_Polygon  zone_surveillance;
   ASN1T_Polygon  point_regroupement;
} ASN1T_Mission_Automate_INF_Harceler;

class EXTERN ASN1C_Mission_Automate_INF_Harceler : public ASN1CType {
public:
   ASN1T_Mission_Automate_INF_Harceler& msgData;
   ASN1C_Mission_Automate_INF_Harceler (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_INF_Harceler& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_INF_Harceler (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_Harceler* pvalue);
EXTERN int asn1PD_Mission_Automate_INF_Harceler (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_Harceler* pvalue);

EXTERN void asn1Print_Mission_Automate_INF_Harceler (ASN1ConstCharPtr name, ASN1T_Mission_Automate_INF_Harceler* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_INF_Interdire                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_INF_Interdire {
   ASN1T_ListPoint  points_a_interdire;
   ASN1BOOL  preparer_terrain;

   ASN1T_Mission_Automate_INF_Interdire () {
      preparer_terrain = FALSE;
   }
} ASN1T_Mission_Automate_INF_Interdire;

class EXTERN ASN1C_Mission_Automate_INF_Interdire : public ASN1CType {
public:
   ASN1T_Mission_Automate_INF_Interdire& msgData;
   ASN1C_Mission_Automate_INF_Interdire (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_INF_Interdire& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_INF_Interdire (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_Interdire* pvalue);
EXTERN int asn1PD_Mission_Automate_INF_Interdire (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_Interdire* pvalue);

EXTERN void asn1Print_Mission_Automate_INF_Interdire (ASN1ConstCharPtr name, ASN1T_Mission_Automate_INF_Interdire* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_INF_MenerDefenseUsure                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_INF_MenerDefenseUsure {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Automate_INF_MenerDefenseUsure;

class EXTERN ASN1C_Mission_Automate_INF_MenerDefenseUsure : public ASN1CType {
public:
   ASN1T_Mission_Automate_INF_MenerDefenseUsure& msgData;
   ASN1C_Mission_Automate_INF_MenerDefenseUsure (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_INF_MenerDefenseUsure& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_INF_MenerDefenseUsure (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_MenerDefenseUsure* pvalue);
EXTERN int asn1PD_Mission_Automate_INF_MenerDefenseUsure (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_MenerDefenseUsure* pvalue);

EXTERN void asn1Print_Mission_Automate_INF_MenerDefenseUsure (ASN1ConstCharPtr name, ASN1T_Mission_Automate_INF_MenerDefenseUsure* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_INF_PrendrePreciserMaintenirContact      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_INF_PrendrePreciserMaintenirContact {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Automate_INF_PrendrePreciserMaintenirContact;

class EXTERN ASN1C_Mission_Automate_INF_PrendrePreciserMaintenirContact : public ASN1CType {
public:
   ASN1T_Mission_Automate_INF_PrendrePreciserMaintenirContact& msgData;
   ASN1C_Mission_Automate_INF_PrendrePreciserMaintenirContact (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_INF_PrendrePreciserMaintenirContact& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_INF_PrendrePreciserMaintenirContact (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_PrendrePreciserMaintenirContact* pvalue);
EXTERN int asn1PD_Mission_Automate_INF_PrendrePreciserMaintenirContact (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_PrendrePreciserMaintenirContact* pvalue);

EXTERN void asn1Print_Mission_Automate_INF_PrendrePreciserMaintenirContact (ASN1ConstCharPtr name, ASN1T_Mission_Automate_INF_PrendrePreciserMaintenirContact* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_INF_ReconnaissanceOffensive              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_INF_ReconnaissanceOffensive {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Automate_INF_ReconnaissanceOffensive;

class EXTERN ASN1C_Mission_Automate_INF_ReconnaissanceOffensive : public ASN1CType {
public:
   ASN1T_Mission_Automate_INF_ReconnaissanceOffensive& msgData;
   ASN1C_Mission_Automate_INF_ReconnaissanceOffensive (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_INF_ReconnaissanceOffensive& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_INF_ReconnaissanceOffensive (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_ReconnaissanceOffensive* pvalue);
EXTERN int asn1PD_Mission_Automate_INF_ReconnaissanceOffensive (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_ReconnaissanceOffensive* pvalue);

EXTERN void asn1Print_Mission_Automate_INF_ReconnaissanceOffensive (ASN1ConstCharPtr name, ASN1T_Mission_Automate_INF_ReconnaissanceOffensive* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_INF_RecueillirUnite                      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_INF_RecueillirUnite {
   ASN1T_Polygon  zone_deploiement;
   ASN1T_Automate  compagnie;
   ASN1T_ListPoint  pias;
} ASN1T_Mission_Automate_INF_RecueillirUnite;

class EXTERN ASN1C_Mission_Automate_INF_RecueillirUnite : public ASN1CType {
public:
   ASN1T_Mission_Automate_INF_RecueillirUnite& msgData;
   ASN1C_Mission_Automate_INF_RecueillirUnite (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_INF_RecueillirUnite& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_INF_RecueillirUnite (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_RecueillirUnite* pvalue);
EXTERN int asn1PD_Mission_Automate_INF_RecueillirUnite (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_RecueillirUnite* pvalue);

EXTERN void asn1Print_Mission_Automate_INF_RecueillirUnite (ASN1ConstCharPtr name, ASN1T_Mission_Automate_INF_RecueillirUnite* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_INF_Reduire                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_INF_Reduire {
   ASN1T_ListPoint  points_resistance;
} ASN1T_Mission_Automate_INF_Reduire;

class EXTERN ASN1C_Mission_Automate_INF_Reduire : public ASN1CType {
public:
   ASN1T_Mission_Automate_INF_Reduire& msgData;
   ASN1C_Mission_Automate_INF_Reduire (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_INF_Reduire& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_INF_Reduire (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_Reduire* pvalue);
EXTERN int asn1PD_Mission_Automate_INF_Reduire (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_Reduire* pvalue);

EXTERN void asn1Print_Mission_Automate_INF_Reduire (ASN1ConstCharPtr name, ASN1T_Mission_Automate_INF_Reduire* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_INF_Relever                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_INF_Relever {
   ASN1T_Automate  compagnie;
} ASN1T_Mission_Automate_INF_Relever;

class EXTERN ASN1C_Mission_Automate_INF_Relever : public ASN1CType {
public:
   ASN1T_Mission_Automate_INF_Relever& msgData;
   ASN1C_Mission_Automate_INF_Relever (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_INF_Relever& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_INF_Relever (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_Relever* pvalue);
EXTERN int asn1PD_Mission_Automate_INF_Relever (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_Relever* pvalue);

EXTERN void asn1Print_Mission_Automate_INF_Relever (ASN1ConstCharPtr name, ASN1T_Mission_Automate_INF_Relever* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_INF_RompreContact                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_INF_RompreContact {
   ASN1T_Polygon  zone_regroupement;
} ASN1T_Mission_Automate_INF_RompreContact;

class EXTERN ASN1C_Mission_Automate_INF_RompreContact : public ASN1CType {
public:
   ASN1T_Mission_Automate_INF_RompreContact& msgData;
   ASN1C_Mission_Automate_INF_RompreContact (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_INF_RompreContact& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_INF_RompreContact (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_RompreContact* pvalue);
EXTERN int asn1PD_Mission_Automate_INF_RompreContact (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_RompreContact* pvalue);

EXTERN void asn1Print_Mission_Automate_INF_RompreContact (ASN1ConstCharPtr name, ASN1T_Mission_Automate_INF_RompreContact* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_INF_Surveiller                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_INF_Surveiller {
   ASN1T_Polygon  zone;
   ASN1T_Polygon  zone_regroupement;
} ASN1T_Mission_Automate_INF_Surveiller;

class EXTERN ASN1C_Mission_Automate_INF_Surveiller : public ASN1CType {
public:
   ASN1T_Mission_Automate_INF_Surveiller& msgData;
   ASN1C_Mission_Automate_INF_Surveiller (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_INF_Surveiller& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_INF_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_Surveiller* pvalue);
EXTERN int asn1PD_Mission_Automate_INF_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_Surveiller* pvalue);

EXTERN void asn1Print_Mission_Automate_INF_Surveiller (ASN1ConstCharPtr name, ASN1T_Mission_Automate_INF_Surveiller* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_INF_Semparer                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_INF_Semparer {
   ASN1T_Polygon  objectif;
} ASN1T_Mission_Automate_INF_Semparer;

class EXTERN ASN1C_Mission_Automate_INF_Semparer : public ASN1CType {
public:
   ASN1T_Mission_Automate_INF_Semparer& msgData;
   ASN1C_Mission_Automate_INF_Semparer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_INF_Semparer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_INF_Semparer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_Semparer* pvalue);
EXTERN int asn1PD_Mission_Automate_INF_Semparer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_Semparer* pvalue);

EXTERN void asn1Print_Mission_Automate_INF_Semparer (ASN1ConstCharPtr name, ASN1T_Mission_Automate_INF_Semparer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_INF_Fixer                                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_INF_Fixer {
   ASN1T_ListKnowledgeAgent  ennemis;
} ASN1T_Mission_Automate_INF_Fixer;

class EXTERN ASN1C_Mission_Automate_INF_Fixer : public ASN1CType {
public:
   ASN1T_Mission_Automate_INF_Fixer& msgData;
   ASN1C_Mission_Automate_INF_Fixer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_INF_Fixer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_INF_Fixer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_Fixer* pvalue);
EXTERN int asn1PD_Mission_Automate_INF_Fixer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_Fixer* pvalue);

EXTERN void asn1Print_Mission_Automate_INF_Fixer (ASN1ConstCharPtr name, ASN1T_Mission_Automate_INF_Fixer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_INF_AttaquerEnTerrainDifficile           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_INF_AttaquerEnTerrainDifficile {
   ASN1T_ListPoint  objectifs;
} ASN1T_Mission_Automate_INF_AttaquerEnTerrainDifficile;

class EXTERN ASN1C_Mission_Automate_INF_AttaquerEnTerrainDifficile : public ASN1CType {
public:
   ASN1T_Mission_Automate_INF_AttaquerEnTerrainDifficile& msgData;
   ASN1C_Mission_Automate_INF_AttaquerEnTerrainDifficile (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_INF_AttaquerEnTerrainDifficile& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_INF_AttaquerEnTerrainDifficile (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_AttaquerEnTerrainDifficile* pvalue);
EXTERN int asn1PD_Mission_Automate_INF_AttaquerEnTerrainDifficile (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_AttaquerEnTerrainDifficile* pvalue);

EXTERN void asn1Print_Mission_Automate_INF_AttaquerEnTerrainDifficile (ASN1ConstCharPtr name, ASN1T_Mission_Automate_INF_AttaquerEnTerrainDifficile* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_INF_BarrerDirection                      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_INF_BarrerDirection {
   ASN1T_Polygon  position;
   ASN1BOOL  preparer_terrain;

   ASN1T_Mission_Automate_INF_BarrerDirection () {
      preparer_terrain = FALSE;
   }
} ASN1T_Mission_Automate_INF_BarrerDirection;

class EXTERN ASN1C_Mission_Automate_INF_BarrerDirection : public ASN1CType {
public:
   ASN1T_Mission_Automate_INF_BarrerDirection& msgData;
   ASN1C_Mission_Automate_INF_BarrerDirection (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_INF_BarrerDirection& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_INF_BarrerDirection (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_BarrerDirection* pvalue);
EXTERN int asn1PD_Mission_Automate_INF_BarrerDirection (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_BarrerDirection* pvalue);

EXTERN void asn1Print_Mission_Automate_INF_BarrerDirection (ASN1ConstCharPtr name, ASN1T_Mission_Automate_INF_BarrerDirection* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_INF_DefendreFerme                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_INF_DefendreFerme {
   ASN1T_Polygon  position;
   ASN1BOOL  preparer_terrain;

   ASN1T_Mission_Automate_INF_DefendreFerme () {
      preparer_terrain = FALSE;
   }
} ASN1T_Mission_Automate_INF_DefendreFerme;

class EXTERN ASN1C_Mission_Automate_INF_DefendreFerme : public ASN1CType {
public:
   ASN1T_Mission_Automate_INF_DefendreFerme& msgData;
   ASN1C_Mission_Automate_INF_DefendreFerme (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_INF_DefendreFerme& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_INF_DefendreFerme (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_DefendreFerme* pvalue);
EXTERN int asn1PD_Mission_Automate_INF_DefendreFerme (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_DefendreFerme* pvalue);

EXTERN void asn1Print_Mission_Automate_INF_DefendreFerme (ASN1ConstCharPtr name, ASN1T_Mission_Automate_INF_DefendreFerme* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_INF_Tenir                                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_INF_Tenir {
   ASN1T_Polygon  position;
   ASN1BOOL  preparer_terrain;

   ASN1T_Mission_Automate_INF_Tenir () {
      preparer_terrain = FALSE;
   }
} ASN1T_Mission_Automate_INF_Tenir;

class EXTERN ASN1C_Mission_Automate_INF_Tenir : public ASN1CType {
public:
   ASN1T_Mission_Automate_INF_Tenir& msgData;
   ASN1C_Mission_Automate_INF_Tenir (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_INF_Tenir& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_INF_Tenir (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_Tenir* pvalue);
EXTERN int asn1PD_Mission_Automate_INF_Tenir (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_Tenir* pvalue);

EXTERN void asn1Print_Mission_Automate_INF_Tenir (ASN1ConstCharPtr name, ASN1T_Mission_Automate_INF_Tenir* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_INF_AppuyerUnFreinage                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_INF_AppuyerUnFreinage {
   ASN1T_Automate  compagnie;
} ASN1T_Mission_Automate_INF_AppuyerUnFreinage;

class EXTERN ASN1C_Mission_Automate_INF_AppuyerUnFreinage : public ASN1CType {
public:
   ASN1T_Mission_Automate_INF_AppuyerUnFreinage& msgData;
   ASN1C_Mission_Automate_INF_AppuyerUnFreinage (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_INF_AppuyerUnFreinage& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_INF_AppuyerUnFreinage (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_AppuyerUnFreinage* pvalue);
EXTERN int asn1PD_Mission_Automate_INF_AppuyerUnFreinage (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_AppuyerUnFreinage* pvalue);

EXTERN void asn1Print_Mission_Automate_INF_AppuyerUnFreinage (ASN1ConstCharPtr name, ASN1T_Mission_Automate_INF_AppuyerUnFreinage* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_INF_AppuyerUneAttaque                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_INF_AppuyerUneAttaque {
   ASN1T_Automate  compagnie;
} ASN1T_Mission_Automate_INF_AppuyerUneAttaque;

class EXTERN ASN1C_Mission_Automate_INF_AppuyerUneAttaque : public ASN1CType {
public:
   ASN1T_Mission_Automate_INF_AppuyerUneAttaque& msgData;
   ASN1C_Mission_Automate_INF_AppuyerUneAttaque (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_INF_AppuyerUneAttaque& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_INF_AppuyerUneAttaque (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_AppuyerUneAttaque* pvalue);
EXTERN int asn1PD_Mission_Automate_INF_AppuyerUneAttaque (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_AppuyerUneAttaque* pvalue);

EXTERN void asn1Print_Mission_Automate_INF_AppuyerUneAttaque (ASN1ConstCharPtr name, ASN1T_Mission_Automate_INF_AppuyerUneAttaque* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_INF_Soutenir                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_INF_Soutenir {
   ASN1T_Automate  compagnie;
} ASN1T_Mission_Automate_INF_Soutenir;

class EXTERN ASN1C_Mission_Automate_INF_Soutenir : public ASN1CType {
public:
   ASN1T_Mission_Automate_INF_Soutenir& msgData;
   ASN1C_Mission_Automate_INF_Soutenir (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_INF_Soutenir& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_INF_Soutenir (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_Soutenir* pvalue);
EXTERN int asn1PD_Mission_Automate_INF_Soutenir (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_INF_Soutenir* pvalue);

EXTERN void asn1Print_Mission_Automate_INF_Soutenir (ASN1ConstCharPtr name, ASN1T_Mission_Automate_INF_Soutenir* pvalue);

#endif

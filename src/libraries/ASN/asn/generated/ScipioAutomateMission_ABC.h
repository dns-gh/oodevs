/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 26-Dec-2005.
 */
#ifndef SCIPIOAUTOMATEMISSION_ABC_H
#define SCIPIOAUTOMATEMISSION_ABC_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_ArmerPointDeControle                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_ArmerPointDeControle {
   ASN1T_Polygon  zone_de_controle;
} ASN1T_Mission_Automate_ABC_ArmerPointDeControle;

class EXTERN ASN1C_Mission_Automate_ABC_ArmerPointDeControle : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_ArmerPointDeControle& msgData;
   ASN1C_Mission_Automate_ABC_ArmerPointDeControle (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_ArmerPointDeControle& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_ArmerPointDeControle (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_ArmerPointDeControle* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_ArmerPointDeControle (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_ArmerPointDeControle* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_ArmerPointDeControle (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_ArmerPointDeControle* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_OrganiserAccueilColonneRefugies      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_OrganiserAccueilColonneRefugies {
   ASN1T_Polygon  zone_installation;
} ASN1T_Mission_Automate_ABC_OrganiserAccueilColonneRefugies;

class EXTERN ASN1C_Mission_Automate_ABC_OrganiserAccueilColonneRefugies : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_OrganiserAccueilColonneRefugies& msgData;
   ASN1C_Mission_Automate_ABC_OrganiserAccueilColonneRefugies (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_OrganiserAccueilColonneRefugies& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_OrganiserAccueilColonneRefugies (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_OrganiserAccueilColonneRefugies* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_OrganiserAccueilColonneRefugies (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_OrganiserAccueilColonneRefugies* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_OrganiserAccueilColonneRefugies (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_OrganiserAccueilColonneRefugies* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_ProtegerPC                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_ProtegerPC {
   ASN1T_Automate  pc;
} ASN1T_Mission_Automate_ABC_ProtegerPC;

class EXTERN ASN1C_Mission_Automate_ABC_ProtegerPC : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_ProtegerPC& msgData;
   ASN1C_Mission_Automate_ABC_ProtegerPC (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_ProtegerPC& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_ProtegerPC (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_ProtegerPC* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_ProtegerPC (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_ProtegerPC* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_ProtegerPC (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_ProtegerPC* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_Appuyer                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_Appuyer {
   ASN1T_Automate  compagnie;
} ASN1T_Mission_Automate_ABC_Appuyer;

class EXTERN ASN1C_Mission_Automate_ABC_Appuyer : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_Appuyer& msgData;
   ASN1C_Mission_Automate_ABC_Appuyer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_Appuyer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_Appuyer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Appuyer* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_Appuyer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Appuyer* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_Appuyer (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_Appuyer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_Attaquer                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_Attaquer {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Automate_ABC_Attaquer;

class EXTERN ASN1C_Mission_Automate_ABC_Attaquer : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_Attaquer& msgData;
   ASN1C_Mission_Automate_ABC_Attaquer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_Attaquer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_Attaquer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Attaquer* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_Attaquer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Attaquer* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_Attaquer (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_Attaquer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_ContreAttaquer                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_ContreAttaquer {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Automate_ABC_ContreAttaquer;

class EXTERN ASN1C_Mission_Automate_ABC_ContreAttaquer : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_ContreAttaquer& msgData;
   ASN1C_Mission_Automate_ABC_ContreAttaquer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_ContreAttaquer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_ContreAttaquer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_ContreAttaquer* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_ContreAttaquer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_ContreAttaquer* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_ContreAttaquer (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_ContreAttaquer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_Controler                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_Controler {
   ASN1T_Polygon  zone_a_controler;
} ASN1T_Mission_Automate_ABC_Controler;

class EXTERN ASN1C_Mission_Automate_ABC_Controler : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_Controler& msgData;
   ASN1C_Mission_Automate_ABC_Controler (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_Controler& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_Controler (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Controler* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_Controler (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Controler* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_Controler (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_Controler* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_Couvrir                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_Couvrir {
   ASN1T_Polygon  position_installation;
   ASN1T_Automate  compagnie;
} ASN1T_Mission_Automate_ABC_Couvrir;

class EXTERN ASN1C_Mission_Automate_ABC_Couvrir : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_Couvrir& msgData;
   ASN1C_Mission_Automate_ABC_Couvrir (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_Couvrir& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_Couvrir (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Couvrir* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_Couvrir (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Couvrir* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_Couvrir (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_Couvrir* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_DonnerCoupArret                      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_DonnerCoupArret {
   ASN1T_Polygon  zone_regroupement;
} ASN1T_Mission_Automate_ABC_DonnerCoupArret;

class EXTERN ASN1C_Mission_Automate_ABC_DonnerCoupArret : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_DonnerCoupArret& msgData;
   ASN1C_Mission_Automate_ABC_DonnerCoupArret (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_DonnerCoupArret& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_DonnerCoupArret (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_DonnerCoupArret* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_DonnerCoupArret (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_DonnerCoupArret* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_DonnerCoupArret (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_DonnerCoupArret* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_Eclairer                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_Eclairer {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Automate_ABC_Eclairer;

class EXTERN ASN1C_Mission_Automate_ABC_Eclairer : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_Eclairer& msgData;
   ASN1C_Mission_Automate_ABC_Eclairer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_Eclairer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_Eclairer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Eclairer* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_Eclairer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Eclairer* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_Eclairer (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_Eclairer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_EffectuerRaid                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_EffectuerRaid {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Automate_ABC_EffectuerRaid;

class EXTERN ASN1C_Mission_Automate_ABC_EffectuerRaid : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_EffectuerRaid& msgData;
   ASN1C_Mission_Automate_ABC_EffectuerRaid (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_EffectuerRaid& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_EffectuerRaid (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_EffectuerRaid* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_EffectuerRaid (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_EffectuerRaid* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_EffectuerRaid (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_EffectuerRaid* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_Interdire                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_Interdire {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Automate_ABC_Interdire;

class EXTERN ASN1C_Mission_Automate_ABC_Interdire : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_Interdire& msgData;
   ASN1C_Mission_Automate_ABC_Interdire (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_Interdire& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_Interdire (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Interdire* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_Interdire (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Interdire* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_Interdire (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_Interdire* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_Jalonner                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_Jalonner {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Automate_ABC_Jalonner;

class EXTERN ASN1C_Mission_Automate_ABC_Jalonner : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_Jalonner& msgData;
   ASN1C_Mission_Automate_ABC_Jalonner (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_Jalonner& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_Jalonner (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Jalonner* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_Jalonner (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Jalonner* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_Jalonner (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_Jalonner* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_PrendrePreciserContact               */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_PrendrePreciserContact {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Automate_ABC_PrendrePreciserContact;

class EXTERN ASN1C_Mission_Automate_ABC_PrendrePreciserContact : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_PrendrePreciserContact& msgData;
   ASN1C_Mission_Automate_ABC_PrendrePreciserContact (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_PrendrePreciserContact& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_PrendrePreciserContact (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_PrendrePreciserContact* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_PrendrePreciserContact (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_PrendrePreciserContact* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_PrendrePreciserContact (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_PrendrePreciserContact* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_Reconnaitre                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_Reconnaitre {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Automate_ABC_Reconnaitre;

class EXTERN ASN1C_Mission_Automate_ABC_Reconnaitre : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_Reconnaitre& msgData;
   ASN1C_Mission_Automate_ABC_Reconnaitre (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_Reconnaitre& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_Reconnaitre (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Reconnaitre* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_Reconnaitre (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Reconnaitre* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_Reconnaitre (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_Reconnaitre* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_ReconnaitrePoint                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_ReconnaitrePoint {
   ASN1T_Polygon  point_a_reconnaitre;
} ASN1T_Mission_Automate_ABC_ReconnaitrePoint;

class EXTERN ASN1C_Mission_Automate_ABC_ReconnaitrePoint : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_ReconnaitrePoint& msgData;
   ASN1C_Mission_Automate_ABC_ReconnaitrePoint (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_ReconnaitrePoint& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_ReconnaitrePoint (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_ReconnaitrePoint* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_ReconnaitrePoint (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_ReconnaitrePoint* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_ReconnaitrePoint (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_ReconnaitrePoint* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_Reduire                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_Reduire {
   ASN1T_Polygon  zone_resistance;
} ASN1T_Mission_Automate_ABC_Reduire;

class EXTERN ASN1C_Mission_Automate_ABC_Reduire : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_Reduire& msgData;
   ASN1C_Mission_Automate_ABC_Reduire (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_Reduire& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_Reduire (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Reduire* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_Reduire (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Reduire* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_Reduire (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_Reduire* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_ReleverEnDepassant                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_ReleverEnDepassant {
   ASN1T_Automate  compagnie;
} ASN1T_Mission_Automate_ABC_ReleverEnDepassant;

class EXTERN ASN1C_Mission_Automate_ABC_ReleverEnDepassant : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_ReleverEnDepassant& msgData;
   ASN1C_Mission_Automate_ABC_ReleverEnDepassant (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_ReleverEnDepassant& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_ReleverEnDepassant (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_ReleverEnDepassant* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_ReleverEnDepassant (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_ReleverEnDepassant* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_ReleverEnDepassant (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_ReleverEnDepassant* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_Semparer                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_Semparer {
   ASN1T_Polygon  objectif;
} ASN1T_Mission_Automate_ABC_Semparer;

class EXTERN ASN1C_Mission_Automate_ABC_Semparer : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_Semparer& msgData;
   ASN1C_Mission_Automate_ABC_Semparer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_Semparer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_Semparer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Semparer* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_Semparer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Semparer* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_Semparer (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_Semparer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_Tenir                                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_Tenir {
   ASN1T_Polygon  position_installation;
} ASN1T_Mission_Automate_ABC_Tenir;

class EXTERN ASN1C_Mission_Automate_ABC_Tenir : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_Tenir& msgData;
   ASN1C_Mission_Automate_ABC_Tenir (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_Tenir& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_Tenir (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Tenir* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_Tenir (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Tenir* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_Tenir (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_Tenir* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_Soutenir                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_Soutenir {
   ASN1T_Automate  compagnie;
} ASN1T_Mission_Automate_ABC_Soutenir;

class EXTERN ASN1C_Mission_Automate_ABC_Soutenir : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_Soutenir& msgData;
   ASN1C_Mission_Automate_ABC_Soutenir (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_Soutenir& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_Soutenir (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Soutenir* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_Soutenir (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Soutenir* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_Soutenir (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_Soutenir* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_Surveiller                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_Surveiller {
   ASN1T_Polygon  zone_installation;
} ASN1T_Mission_Automate_ABC_Surveiller;

class EXTERN ASN1C_Mission_Automate_ABC_Surveiller : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_Surveiller& msgData;
   ASN1C_Mission_Automate_ABC_Surveiller (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_Surveiller& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Surveiller* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Surveiller* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_Surveiller (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_Surveiller* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_FlancGarder                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_FlancGarder {
   ASN1T_Automate  compagnie;
} ASN1T_Mission_Automate_ABC_FlancGarder;

class EXTERN ASN1C_Mission_Automate_ABC_FlancGarder : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_FlancGarder& msgData;
   ASN1C_Mission_Automate_ABC_FlancGarder (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_FlancGarder& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_FlancGarder (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_FlancGarder* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_FlancGarder (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_FlancGarder* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_FlancGarder (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_FlancGarder* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_RenseignerSur                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_RenseignerSur {
   ASN1T_Polygon  zone_a_renseigner;
} ASN1T_Mission_Automate_ABC_RenseignerSur;

class EXTERN ASN1C_Mission_Automate_ABC_RenseignerSur : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_RenseignerSur& msgData;
   ASN1C_Mission_Automate_ABC_RenseignerSur (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_RenseignerSur& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_RenseignerSur (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_RenseignerSur* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_RenseignerSur (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_RenseignerSur* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_RenseignerSur (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_RenseignerSur* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_Fixer                                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_Fixer {
   ASN1T_ListKnowledgeAgent  ennemis;
} ASN1T_Mission_Automate_ABC_Fixer;

class EXTERN ASN1C_Mission_Automate_ABC_Fixer : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_Fixer& msgData;
   ASN1C_Mission_Automate_ABC_Fixer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_Fixer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_Fixer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Fixer* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_Fixer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Fixer* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_Fixer (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_Fixer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_Freiner                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_Freiner {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Automate_ABC_Freiner;

class EXTERN ASN1C_Mission_Automate_ABC_Freiner : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_Freiner& msgData;
   ASN1C_Mission_Automate_ABC_Freiner (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_Freiner& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_Freiner (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Freiner* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_Freiner (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_Freiner* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_Freiner (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_Freiner* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_SurveillerItineraire                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_SurveillerItineraire {
   ASN1T_Itineraire  itineraire;
} ASN1T_Mission_Automate_ABC_SurveillerItineraire;

class EXTERN ASN1C_Mission_Automate_ABC_SurveillerItineraire : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_SurveillerItineraire& msgData;
   ASN1C_Mission_Automate_ABC_SurveillerItineraire (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_SurveillerItineraire& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_SurveillerItineraire (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_SurveillerItineraire* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_SurveillerItineraire (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_SurveillerItineraire* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_SurveillerItineraire (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_SurveillerItineraire* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_RenseignerSurUnAxe                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_RenseignerSurUnAxe {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Automate_ABC_RenseignerSurUnAxe;

class EXTERN ASN1C_Mission_Automate_ABC_RenseignerSurUnAxe : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_RenseignerSurUnAxe& msgData;
   ASN1C_Mission_Automate_ABC_RenseignerSurUnAxe (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_RenseignerSurUnAxe& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_RenseignerSurUnAxe (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_RenseignerSurUnAxe* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_RenseignerSurUnAxe (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_RenseignerSurUnAxe* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_RenseignerSurUnAxe (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_RenseignerSurUnAxe* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_RecueillirUnite                      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_RecueillirUnite {
   ASN1T_Polygon  zone_regroupement;
} ASN1T_Mission_Automate_ABC_RecueillirUnite;

class EXTERN ASN1C_Mission_Automate_ABC_RecueillirUnite : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_RecueillirUnite& msgData;
   ASN1C_Mission_Automate_ABC_RecueillirUnite (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_RecueillirUnite& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_RecueillirUnite (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_RecueillirUnite* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_RecueillirUnite (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_RecueillirUnite* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_RecueillirUnite (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_RecueillirUnite* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ABC_EscorterUnConvoi                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_ABC_EscorterUnConvoi {
   ASN1T_Agent  convoi_a_escorter;
} ASN1T_Mission_Automate_ABC_EscorterUnConvoi;

class EXTERN ASN1C_Mission_Automate_ABC_EscorterUnConvoi : public ASN1CType {
public:
   ASN1T_Mission_Automate_ABC_EscorterUnConvoi& msgData;
   ASN1C_Mission_Automate_ABC_EscorterUnConvoi (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ABC_EscorterUnConvoi& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_ABC_EscorterUnConvoi (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_EscorterUnConvoi* pvalue);
EXTERN int asn1PD_Mission_Automate_ABC_EscorterUnConvoi (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ABC_EscorterUnConvoi* pvalue);

EXTERN void asn1Print_Mission_Automate_ABC_EscorterUnConvoi (ASN1ConstCharPtr name, ASN1T_Mission_Automate_ABC_EscorterUnConvoi* pvalue);

#endif

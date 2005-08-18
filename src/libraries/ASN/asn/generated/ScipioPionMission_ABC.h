/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 17-Aug-2005.
 */
#ifndef SCIPIOPIONMISSION_ABC_H
#define SCIPIOPIONMISSION_ABC_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ABC_ReconnaitreAxe                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ABC_ReconnaitreAxe {
   ASN1T_Itineraire  itineraire;
} ASN1T_Mission_Pion_ABC_ReconnaitreAxe;

class EXTERN ASN1C_Mission_Pion_ABC_ReconnaitreAxe : public ASN1CType {
public:
   ASN1T_Mission_Pion_ABC_ReconnaitreAxe& msgData;
   ASN1C_Mission_Pion_ABC_ReconnaitreAxe (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ABC_ReconnaitreAxe& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ABC_ReconnaitreAxe (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_ReconnaitreAxe* pvalue);
EXTERN int asn1PD_Mission_Pion_ABC_ReconnaitreAxe (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_ReconnaitreAxe* pvalue);

EXTERN void asn1Print_Mission_Pion_ABC_ReconnaitreAxe (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ABC_ReconnaitreAxe* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ABC_Soutenir                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ABC_Soutenir {
   ASN1T_Agent  unite_a_soutenir;
} ASN1T_Mission_Pion_ABC_Soutenir;

class EXTERN ASN1C_Mission_Pion_ABC_Soutenir : public ASN1CType {
public:
   ASN1T_Mission_Pion_ABC_Soutenir& msgData;
   ASN1C_Mission_Pion_ABC_Soutenir (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ABC_Soutenir& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ABC_Soutenir (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_Soutenir* pvalue);
EXTERN int asn1PD_Mission_Pion_ABC_Soutenir (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_Soutenir* pvalue);

EXTERN void asn1Print_Mission_Pion_ABC_Soutenir (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ABC_Soutenir* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ABC_Couvrir                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ABC_Couvrir {
   ASN1T_Point  position;
   ASN1T_ListAgent  unites_a_couvrir;
} ASN1T_Mission_Pion_ABC_Couvrir;

class EXTERN ASN1C_Mission_Pion_ABC_Couvrir : public ASN1CType {
public:
   ASN1T_Mission_Pion_ABC_Couvrir& msgData;
   ASN1C_Mission_Pion_ABC_Couvrir (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ABC_Couvrir& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ABC_Couvrir (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_Couvrir* pvalue);
EXTERN int asn1PD_Mission_Pion_ABC_Couvrir (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_Couvrir* pvalue);

EXTERN void asn1Print_Mission_Pion_ABC_Couvrir (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ABC_Couvrir* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ABC_FlancGarder                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ABC_FlancGarder {
   ASN1T_Itineraire  itineraire;
   ASN1T_ListAgent  unites_a_couvrir;
} ASN1T_Mission_Pion_ABC_FlancGarder;

class EXTERN ASN1C_Mission_Pion_ABC_FlancGarder : public ASN1CType {
public:
   ASN1T_Mission_Pion_ABC_FlancGarder& msgData;
   ASN1C_Mission_Pion_ABC_FlancGarder (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ABC_FlancGarder& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ABC_FlancGarder (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_FlancGarder* pvalue);
EXTERN int asn1PD_Mission_Pion_ABC_FlancGarder (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_FlancGarder* pvalue);

EXTERN void asn1Print_Mission_Pion_ABC_FlancGarder (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ABC_FlancGarder* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ABC_Reduire                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ABC_Reduire {
   ASN1T_KnowledgeAgent  unite_a_reduire;
} ASN1T_Mission_Pion_ABC_Reduire;

class EXTERN ASN1C_Mission_Pion_ABC_Reduire : public ASN1CType {
public:
   ASN1T_Mission_Pion_ABC_Reduire& msgData;
   ASN1C_Mission_Pion_ABC_Reduire (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ABC_Reduire& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ABC_Reduire (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_Reduire* pvalue);
EXTERN int asn1PD_Mission_Pion_ABC_Reduire (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_Reduire* pvalue);

EXTERN void asn1Print_Mission_Pion_ABC_Reduire (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ABC_Reduire* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ABC_Appuyer                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ABC_Appuyer {
   ASN1T_Agent  unite_a_appuyer;
} ASN1T_Mission_Pion_ABC_Appuyer;

class EXTERN ASN1C_Mission_Pion_ABC_Appuyer : public ASN1CType {
public:
   ASN1T_Mission_Pion_ABC_Appuyer& msgData;
   ASN1C_Mission_Pion_ABC_Appuyer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ABC_Appuyer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ABC_Appuyer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_Appuyer* pvalue);
EXTERN int asn1PD_Mission_Pion_ABC_Appuyer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_Appuyer* pvalue);

EXTERN void asn1Print_Mission_Pion_ABC_Appuyer (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ABC_Appuyer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ABC_Fixer                                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ABC_Fixer {
   ASN1T_KnowledgeAgent  unite_a_fixer;
} ASN1T_Mission_Pion_ABC_Fixer;

class EXTERN ASN1C_Mission_Pion_ABC_Fixer : public ASN1CType {
public:
   ASN1T_Mission_Pion_ABC_Fixer& msgData;
   ASN1C_Mission_Pion_ABC_Fixer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ABC_Fixer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ABC_Fixer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_Fixer* pvalue);
EXTERN int asn1PD_Mission_Pion_ABC_Fixer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_Fixer* pvalue);

EXTERN void asn1Print_Mission_Pion_ABC_Fixer (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ABC_Fixer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ABC_RenseignerSur                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ABC_RenseignerSur {
   ASN1T_Point  point_a_renseigner;
} ASN1T_Mission_Pion_ABC_RenseignerSur;

class EXTERN ASN1C_Mission_Pion_ABC_RenseignerSur : public ASN1CType {
public:
   ASN1T_Mission_Pion_ABC_RenseignerSur& msgData;
   ASN1C_Mission_Pion_ABC_RenseignerSur (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ABC_RenseignerSur& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ABC_RenseignerSur (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_RenseignerSur* pvalue);
EXTERN int asn1PD_Mission_Pion_ABC_RenseignerSur (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_RenseignerSur* pvalue);

EXTERN void asn1Print_Mission_Pion_ABC_RenseignerSur (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ABC_RenseignerSur* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ABC_ControlerZone                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ABC_ControlerZone {
   ASN1T_Polygon  zone;
} ASN1T_Mission_Pion_ABC_ControlerZone;

class EXTERN ASN1C_Mission_Pion_ABC_ControlerZone : public ASN1CType {
public:
   ASN1T_Mission_Pion_ABC_ControlerZone& msgData;
   ASN1C_Mission_Pion_ABC_ControlerZone (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ABC_ControlerZone& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ABC_ControlerZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_ControlerZone* pvalue);
EXTERN int asn1PD_Mission_Pion_ABC_ControlerZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_ControlerZone* pvalue);

EXTERN void asn1Print_Mission_Pion_ABC_ControlerZone (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ABC_ControlerZone* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ABC_Surveiller                               */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ABC_Surveiller {
   ASN1T_Point  position;
} ASN1T_Mission_Pion_ABC_Surveiller;

class EXTERN ASN1C_Mission_Pion_ABC_Surveiller : public ASN1CType {
public:
   ASN1T_Mission_Pion_ABC_Surveiller& msgData;
   ASN1C_Mission_Pion_ABC_Surveiller (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ABC_Surveiller& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ABC_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_Surveiller* pvalue);
EXTERN int asn1PD_Mission_Pion_ABC_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_Surveiller* pvalue);

EXTERN void asn1Print_Mission_Pion_ABC_Surveiller (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ABC_Surveiller* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ABC_Barrer                                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ABC_Barrer {
   ASN1T_Point  position_installation;
} ASN1T_Mission_Pion_ABC_Barrer;

class EXTERN ASN1C_Mission_Pion_ABC_Barrer : public ASN1CType {
public:
   ASN1T_Mission_Pion_ABC_Barrer& msgData;
   ASN1C_Mission_Pion_ABC_Barrer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ABC_Barrer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ABC_Barrer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_Barrer* pvalue);
EXTERN int asn1PD_Mission_Pion_ABC_Barrer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_Barrer* pvalue);

EXTERN void asn1Print_Mission_Pion_ABC_Barrer (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ABC_Barrer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ABC_Attaquer                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ABC_Attaquer {
   ASN1T_Itineraire  itineraire_assaut;
} ASN1T_Mission_Pion_ABC_Attaquer;

class EXTERN ASN1C_Mission_Pion_ABC_Attaquer : public ASN1CType {
public:
   ASN1T_Mission_Pion_ABC_Attaquer& msgData;
   ASN1C_Mission_Pion_ABC_Attaquer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ABC_Attaquer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ABC_Attaquer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_Attaquer* pvalue);
EXTERN int asn1PD_Mission_Pion_ABC_Attaquer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_Attaquer* pvalue);

EXTERN void asn1Print_Mission_Pion_ABC_Attaquer (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ABC_Attaquer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ABC_ReconnaitrePoint                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ABC_ReconnaitrePoint {
   ASN1T_Point  point_a_reconnaitre;
} ASN1T_Mission_Pion_ABC_ReconnaitrePoint;

class EXTERN ASN1C_Mission_Pion_ABC_ReconnaitrePoint : public ASN1CType {
public:
   ASN1T_Mission_Pion_ABC_ReconnaitrePoint& msgData;
   ASN1C_Mission_Pion_ABC_ReconnaitrePoint (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ABC_ReconnaitrePoint& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ABC_ReconnaitrePoint (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_ReconnaitrePoint* pvalue);
EXTERN int asn1PD_Mission_Pion_ABC_ReconnaitrePoint (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_ReconnaitrePoint* pvalue);

EXTERN void asn1Print_Mission_Pion_ABC_ReconnaitrePoint (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ABC_ReconnaitrePoint* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ABC_Eclairer                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ABC_Eclairer {
   ASN1T_Itineraire  itineraire;
} ASN1T_Mission_Pion_ABC_Eclairer;

class EXTERN ASN1C_Mission_Pion_ABC_Eclairer : public ASN1CType {
public:
   ASN1T_Mission_Pion_ABC_Eclairer& msgData;
   ASN1C_Mission_Pion_ABC_Eclairer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ABC_Eclairer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ABC_Eclairer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_Eclairer* pvalue);
EXTERN int asn1PD_Mission_Pion_ABC_Eclairer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_Eclairer* pvalue);

EXTERN void asn1Print_Mission_Pion_ABC_Eclairer (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ABC_Eclairer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ABC_Jalonner                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ABC_Jalonner {
   ASN1T_Itineraire  itineraire;
} ASN1T_Mission_Pion_ABC_Jalonner;

class EXTERN ASN1C_Mission_Pion_ABC_Jalonner : public ASN1CType {
public:
   ASN1T_Mission_Pion_ABC_Jalonner& msgData;
   ASN1C_Mission_Pion_ABC_Jalonner (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ABC_Jalonner& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ABC_Jalonner (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_Jalonner* pvalue);
EXTERN int asn1PD_Mission_Pion_ABC_Jalonner (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_Jalonner* pvalue);

EXTERN void asn1Print_Mission_Pion_ABC_Jalonner (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ABC_Jalonner* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ABC_ContreAttaquer                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ABC_ContreAttaquer {
   ASN1T_Itineraire  itineraire_assaut;
} ASN1T_Mission_Pion_ABC_ContreAttaquer;

class EXTERN ASN1C_Mission_Pion_ABC_ContreAttaquer : public ASN1CType {
public:
   ASN1T_Mission_Pion_ABC_ContreAttaquer& msgData;
   ASN1C_Mission_Pion_ABC_ContreAttaquer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ABC_ContreAttaquer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ABC_ContreAttaquer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_ContreAttaquer* pvalue);
EXTERN int asn1PD_Mission_Pion_ABC_ContreAttaquer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_ContreAttaquer* pvalue);

EXTERN void asn1Print_Mission_Pion_ABC_ContreAttaquer (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ABC_ContreAttaquer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ABC_SemparerZone                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ABC_SemparerZone {
   ASN1T_Polygon  zone_a_prendre;
} ASN1T_Mission_Pion_ABC_SemparerZone;

class EXTERN ASN1C_Mission_Pion_ABC_SemparerZone : public ASN1CType {
public:
   ASN1T_Mission_Pion_ABC_SemparerZone& msgData;
   ASN1C_Mission_Pion_ABC_SemparerZone (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ABC_SemparerZone& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ABC_SemparerZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_SemparerZone* pvalue);
EXTERN int asn1PD_Mission_Pion_ABC_SemparerZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_SemparerZone* pvalue);

EXTERN void asn1Print_Mission_Pion_ABC_SemparerZone (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ABC_SemparerZone* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ABC_ReleverUnite                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ABC_ReleverUnite {
   ASN1T_Agent  unite_a_relever;
} ASN1T_Mission_Pion_ABC_ReleverUnite;

class EXTERN ASN1C_Mission_Pion_ABC_ReleverUnite : public ASN1CType {
public:
   ASN1T_Mission_Pion_ABC_ReleverUnite& msgData;
   ASN1C_Mission_Pion_ABC_ReleverUnite (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ABC_ReleverUnite& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ABC_ReleverUnite (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_ReleverUnite* pvalue);
EXTERN int asn1PD_Mission_Pion_ABC_ReleverUnite (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_ReleverUnite* pvalue);

EXTERN void asn1Print_Mission_Pion_ABC_ReleverUnite (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ABC_ReleverUnite* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ABC_ArmerPIA                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ABC_ArmerPIA {
   ASN1T_Point  pia;
} ASN1T_Mission_Pion_ABC_ArmerPIA;

class EXTERN ASN1C_Mission_Pion_ABC_ArmerPIA : public ASN1CType {
public:
   ASN1T_Mission_Pion_ABC_ArmerPIA& msgData;
   ASN1C_Mission_Pion_ABC_ArmerPIA (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ABC_ArmerPIA& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ABC_ArmerPIA (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_ArmerPIA* pvalue);
EXTERN int asn1PD_Mission_Pion_ABC_ArmerPIA (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_ArmerPIA* pvalue);

EXTERN void asn1Print_Mission_Pion_ABC_ArmerPIA (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ABC_ArmerPIA* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ABC_OuvrirItineraire                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ABC_OuvrirItineraire {
   ASN1T_Itineraire  itineraire;
} ASN1T_Mission_Pion_ABC_OuvrirItineraire;

class EXTERN ASN1C_Mission_Pion_ABC_OuvrirItineraire : public ASN1CType {
public:
   ASN1T_Mission_Pion_ABC_OuvrirItineraire& msgData;
   ASN1C_Mission_Pion_ABC_OuvrirItineraire (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ABC_OuvrirItineraire& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ABC_OuvrirItineraire (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_OuvrirItineraire* pvalue);
EXTERN int asn1PD_Mission_Pion_ABC_OuvrirItineraire (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_OuvrirItineraire* pvalue);

EXTERN void asn1Print_Mission_Pion_ABC_OuvrirItineraire (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ABC_OuvrirItineraire* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ABC_Freiner                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ABC_Freiner {
   ASN1T_Itineraire  itineraire;
} ASN1T_Mission_Pion_ABC_Freiner;

class EXTERN ASN1C_Mission_Pion_ABC_Freiner : public ASN1CType {
public:
   ASN1T_Mission_Pion_ABC_Freiner& msgData;
   ASN1C_Mission_Pion_ABC_Freiner (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ABC_Freiner& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ABC_Freiner (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_Freiner* pvalue);
EXTERN int asn1PD_Mission_Pion_ABC_Freiner (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_Freiner* pvalue);

EXTERN void asn1Print_Mission_Pion_ABC_Freiner (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ABC_Freiner* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ABC_EscorterUnConvoi                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ABC_EscorterUnConvoi {
   ASN1T_Agent  convoi_a_escorter;
} ASN1T_Mission_Pion_ABC_EscorterUnConvoi;

class EXTERN ASN1C_Mission_Pion_ABC_EscorterUnConvoi : public ASN1CType {
public:
   ASN1T_Mission_Pion_ABC_EscorterUnConvoi& msgData;
   ASN1C_Mission_Pion_ABC_EscorterUnConvoi (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ABC_EscorterUnConvoi& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ABC_EscorterUnConvoi (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_EscorterUnConvoi* pvalue);
EXTERN int asn1PD_Mission_Pion_ABC_EscorterUnConvoi (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_EscorterUnConvoi* pvalue);

EXTERN void asn1Print_Mission_Pion_ABC_EscorterUnConvoi (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ABC_EscorterUnConvoi* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ABC_ArmerPointDeControle                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ABC_ArmerPointDeControle {
   ASN1T_Point  point_de_controle;
} ASN1T_Mission_Pion_ABC_ArmerPointDeControle;

class EXTERN ASN1C_Mission_Pion_ABC_ArmerPointDeControle : public ASN1CType {
public:
   ASN1T_Mission_Pion_ABC_ArmerPointDeControle& msgData;
   ASN1C_Mission_Pion_ABC_ArmerPointDeControle (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ABC_ArmerPointDeControle& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ABC_ArmerPointDeControle (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_ArmerPointDeControle* pvalue);
EXTERN int asn1PD_Mission_Pion_ABC_ArmerPointDeControle (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_ArmerPointDeControle* pvalue);

EXTERN void asn1Print_Mission_Pion_ABC_ArmerPointDeControle (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ABC_ArmerPointDeControle* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ABC_OrganiserAccueilColonneRefugies          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_ABC_OrganiserAccueilColonneRefugies {
   ASN1T_Point  position_installation;
} ASN1T_Mission_Pion_ABC_OrganiserAccueilColonneRefugies;

class EXTERN ASN1C_Mission_Pion_ABC_OrganiserAccueilColonneRefugies : public ASN1CType {
public:
   ASN1T_Mission_Pion_ABC_OrganiserAccueilColonneRefugies& msgData;
   ASN1C_Mission_Pion_ABC_OrganiserAccueilColonneRefugies (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ABC_OrganiserAccueilColonneRefugies& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_ABC_OrganiserAccueilColonneRefugies (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_OrganiserAccueilColonneRefugies* pvalue);
EXTERN int asn1PD_Mission_Pion_ABC_OrganiserAccueilColonneRefugies (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ABC_OrganiserAccueilColonneRefugies* pvalue);

EXTERN void asn1Print_Mission_Pion_ABC_OrganiserAccueilColonneRefugies (ASN1ConstCharPtr name, ASN1T_Mission_Pion_ABC_OrganiserAccueilColonneRefugies* pvalue);

#endif

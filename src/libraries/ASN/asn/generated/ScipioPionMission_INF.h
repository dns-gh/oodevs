/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 18-Aug-2005.
 */
#ifndef SCIPIOPIONMISSION_INF_H
#define SCIPIOPIONMISSION_INF_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"
#include "ScipioObject.h"

/**************************************************************/
/*                                                            */
/*  Mission_Pion_INF_ReconnaitreAxe                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_INF_ReconnaitreAxe {
   ASN1T_Itineraire  itineraire;
} ASN1T_Mission_Pion_INF_ReconnaitreAxe;

class EXTERN ASN1C_Mission_Pion_INF_ReconnaitreAxe : public ASN1CType {
public:
   ASN1T_Mission_Pion_INF_ReconnaitreAxe& msgData;
   ASN1C_Mission_Pion_INF_ReconnaitreAxe (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_INF_ReconnaitreAxe& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_INF_ReconnaitreAxe (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_ReconnaitreAxe* pvalue);
EXTERN int asn1PD_Mission_Pion_INF_ReconnaitreAxe (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_ReconnaitreAxe* pvalue);

EXTERN void asn1Print_Mission_Pion_INF_ReconnaitreAxe (ASN1ConstCharPtr name, ASN1T_Mission_Pion_INF_ReconnaitreAxe* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_INF_Couvrir                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_INF_Couvrir {
   ASN1T_Point  point_couverture;
   ASN1T_Agent  cible;
} ASN1T_Mission_Pion_INF_Couvrir;

class EXTERN ASN1C_Mission_Pion_INF_Couvrir : public ASN1CType {
public:
   ASN1T_Mission_Pion_INF_Couvrir& msgData;
   ASN1C_Mission_Pion_INF_Couvrir (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_INF_Couvrir& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_INF_Couvrir (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_Couvrir* pvalue);
EXTERN int asn1PD_Mission_Pion_INF_Couvrir (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_Couvrir* pvalue);

EXTERN void asn1Print_Mission_Pion_INF_Couvrir (ASN1ConstCharPtr name, ASN1T_Mission_Pion_INF_Couvrir* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_INF_Reduire                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_INF_Reduire {
   ASN1T_KnowledgeAgent  unite_a_reduire;
} ASN1T_Mission_Pion_INF_Reduire;

class EXTERN ASN1C_Mission_Pion_INF_Reduire : public ASN1CType {
public:
   ASN1T_Mission_Pion_INF_Reduire& msgData;
   ASN1C_Mission_Pion_INF_Reduire (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_INF_Reduire& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_INF_Reduire (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_Reduire* pvalue);
EXTERN int asn1PD_Mission_Pion_INF_Reduire (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_Reduire* pvalue);

EXTERN void asn1Print_Mission_Pion_INF_Reduire (ASN1ConstCharPtr name, ASN1T_Mission_Pion_INF_Reduire* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_INF_Appuyer                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_INF_Appuyer {
   ASN1T_Agent  unite_a_appuyer;
} ASN1T_Mission_Pion_INF_Appuyer;

class EXTERN ASN1C_Mission_Pion_INF_Appuyer : public ASN1CType {
public:
   ASN1T_Mission_Pion_INF_Appuyer& msgData;
   ASN1C_Mission_Pion_INF_Appuyer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_INF_Appuyer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_INF_Appuyer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_Appuyer* pvalue);
EXTERN int asn1PD_Mission_Pion_INF_Appuyer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_Appuyer* pvalue);

EXTERN void asn1Print_Mission_Pion_INF_Appuyer (ASN1ConstCharPtr name, ASN1T_Mission_Pion_INF_Appuyer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_INF_Fixer                                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_INF_Fixer {
   ASN1T_KnowledgeAgent  ennemi;
} ASN1T_Mission_Pion_INF_Fixer;

class EXTERN ASN1C_Mission_Pion_INF_Fixer : public ASN1CType {
public:
   ASN1T_Mission_Pion_INF_Fixer& msgData;
   ASN1C_Mission_Pion_INF_Fixer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_INF_Fixer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_INF_Fixer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_Fixer* pvalue);
EXTERN int asn1PD_Mission_Pion_INF_Fixer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_Fixer* pvalue);

EXTERN void asn1Print_Mission_Pion_INF_Fixer (ASN1ConstCharPtr name, ASN1T_Mission_Pion_INF_Fixer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_INF_ControlerSecteur                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_INF_ControlerSecteur {
   ASN1T_Polygon  secteur;
   ASN1BOOL  preparer_terrain;
} ASN1T_Mission_Pion_INF_ControlerSecteur;

class EXTERN ASN1C_Mission_Pion_INF_ControlerSecteur : public ASN1CType {
public:
   ASN1T_Mission_Pion_INF_ControlerSecteur& msgData;
   ASN1C_Mission_Pion_INF_ControlerSecteur (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_INF_ControlerSecteur& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_INF_ControlerSecteur (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_ControlerSecteur* pvalue);
EXTERN int asn1PD_Mission_Pion_INF_ControlerSecteur (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_ControlerSecteur* pvalue);

EXTERN void asn1Print_Mission_Pion_INF_ControlerSecteur (ASN1ConstCharPtr name, ASN1T_Mission_Pion_INF_ControlerSecteur* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_INF_SurveillerSecteur                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_INF_SurveillerSecteur {
   ASN1T_Point  position_installation;
} ASN1T_Mission_Pion_INF_SurveillerSecteur;

class EXTERN ASN1C_Mission_Pion_INF_SurveillerSecteur : public ASN1CType {
public:
   ASN1T_Mission_Pion_INF_SurveillerSecteur& msgData;
   ASN1C_Mission_Pion_INF_SurveillerSecteur (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_INF_SurveillerSecteur& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_INF_SurveillerSecteur (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_SurveillerSecteur* pvalue);
EXTERN int asn1PD_Mission_Pion_INF_SurveillerSecteur (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_SurveillerSecteur* pvalue);

EXTERN void asn1Print_Mission_Pion_INF_SurveillerSecteur (ASN1ConstCharPtr name, ASN1T_Mission_Pion_INF_SurveillerSecteur* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_INF_Barrer                                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_INF_Barrer {
   ASN1T_Point  position_a_barrer;
} ASN1T_Mission_Pion_INF_Barrer;

class EXTERN ASN1C_Mission_Pion_INF_Barrer : public ASN1CType {
public:
   ASN1T_Mission_Pion_INF_Barrer& msgData;
   ASN1C_Mission_Pion_INF_Barrer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_INF_Barrer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_INF_Barrer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_Barrer* pvalue);
EXTERN int asn1PD_Mission_Pion_INF_Barrer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_Barrer* pvalue);

EXTERN void asn1Print_Mission_Pion_INF_Barrer (ASN1ConstCharPtr name, ASN1T_Mission_Pion_INF_Barrer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_INF_ReconnaitrePoint                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_INF_ReconnaitrePoint {
   ASN1T_Itineraire  itineraire;
   ASN1T_Point  point_a_reconnaitre;
} ASN1T_Mission_Pion_INF_ReconnaitrePoint;

class EXTERN ASN1C_Mission_Pion_INF_ReconnaitrePoint : public ASN1CType {
public:
   ASN1T_Mission_Pion_INF_ReconnaitrePoint& msgData;
   ASN1C_Mission_Pion_INF_ReconnaitrePoint (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_INF_ReconnaitrePoint& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_INF_ReconnaitrePoint (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_ReconnaitrePoint* pvalue);
EXTERN int asn1PD_Mission_Pion_INF_ReconnaitrePoint (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_ReconnaitrePoint* pvalue);

EXTERN void asn1Print_Mission_Pion_INF_ReconnaitrePoint (ASN1ConstCharPtr name, ASN1T_Mission_Pion_INF_ReconnaitrePoint* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_INF_Eclairer                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_INF_Eclairer {
   ASN1T_Itineraire  itineraire;
} ASN1T_Mission_Pion_INF_Eclairer;

class EXTERN ASN1C_Mission_Pion_INF_Eclairer : public ASN1CType {
public:
   ASN1T_Mission_Pion_INF_Eclairer& msgData;
   ASN1C_Mission_Pion_INF_Eclairer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_INF_Eclairer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_INF_Eclairer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_Eclairer* pvalue);
EXTERN int asn1PD_Mission_Pion_INF_Eclairer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_Eclairer* pvalue);

EXTERN void asn1Print_Mission_Pion_INF_Eclairer (ASN1ConstCharPtr name, ASN1T_Mission_Pion_INF_Eclairer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_INF_Interdire                                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_INF_Interdire {
   ASN1T_Point  point_a_interdire;
   ASN1BOOL  preparer_terrain;

   ASN1T_Mission_Pion_INF_Interdire () {
      preparer_terrain = FALSE;
   }
} ASN1T_Mission_Pion_INF_Interdire;

class EXTERN ASN1C_Mission_Pion_INF_Interdire : public ASN1CType {
public:
   ASN1T_Mission_Pion_INF_Interdire& msgData;
   ASN1C_Mission_Pion_INF_Interdire (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_INF_Interdire& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_INF_Interdire (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_Interdire* pvalue);
EXTERN int asn1PD_Mission_Pion_INF_Interdire (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_Interdire* pvalue);

EXTERN void asn1Print_Mission_Pion_INF_Interdire (ASN1ConstCharPtr name, ASN1T_Mission_Pion_INF_Interdire* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_INF_Semparer                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_INF_Semparer {
   ASN1T_Point  objectif;
} ASN1T_Mission_Pion_INF_Semparer;

class EXTERN ASN1C_Mission_Pion_INF_Semparer : public ASN1CType {
public:
   ASN1T_Mission_Pion_INF_Semparer& msgData;
   ASN1C_Mission_Pion_INF_Semparer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_INF_Semparer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_INF_Semparer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_Semparer* pvalue);
EXTERN int asn1PD_Mission_Pion_INF_Semparer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_Semparer* pvalue);

EXTERN void asn1Print_Mission_Pion_INF_Semparer (ASN1ConstCharPtr name, ASN1T_Mission_Pion_INF_Semparer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_INF_PrendrePreciserContact                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_INF_PrendrePreciserContact {
   ASN1T_KnowledgeAgent  ennemi;
} ASN1T_Mission_Pion_INF_PrendrePreciserContact;

class EXTERN ASN1C_Mission_Pion_INF_PrendrePreciserContact : public ASN1CType {
public:
   ASN1T_Mission_Pion_INF_PrendrePreciserContact& msgData;
   ASN1C_Mission_Pion_INF_PrendrePreciserContact (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_INF_PrendrePreciserContact& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_INF_PrendrePreciserContact (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_PrendrePreciserContact* pvalue);
EXTERN int asn1PD_Mission_Pion_INF_PrendrePreciserContact (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_PrendrePreciserContact* pvalue);

EXTERN void asn1Print_Mission_Pion_INF_PrendrePreciserContact (ASN1ConstCharPtr name, ASN1T_Mission_Pion_INF_PrendrePreciserContact* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_INF_Defendre                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_INF_Defendre {
   ASN1T_Point  point_a_defendre;
   ASN1BOOL  preparer_terrain;

   ASN1T_Mission_Pion_INF_Defendre () {
      preparer_terrain = FALSE;
   }
} ASN1T_Mission_Pion_INF_Defendre;

class EXTERN ASN1C_Mission_Pion_INF_Defendre : public ASN1CType {
public:
   ASN1T_Mission_Pion_INF_Defendre& msgData;
   ASN1C_Mission_Pion_INF_Defendre (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_INF_Defendre& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_INF_Defendre (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_Defendre* pvalue);
EXTERN int asn1PD_Mission_Pion_INF_Defendre (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_Defendre* pvalue);

EXTERN void asn1Print_Mission_Pion_INF_Defendre (ASN1ConstCharPtr name, ASN1T_Mission_Pion_INF_Defendre* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_INF_DetruireEmbuscade                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_INF_DetruireEmbuscade {
   ASN1T_Point  position_embuscade;
   ASN1T_Point  position_regroupement;
   ASN1T_KnowledgeAgent  ennemi;
   ASN1T_EnumObjectType  nature_obstacle;
} ASN1T_Mission_Pion_INF_DetruireEmbuscade;

class EXTERN ASN1C_Mission_Pion_INF_DetruireEmbuscade : public ASN1CType {
public:
   ASN1T_Mission_Pion_INF_DetruireEmbuscade& msgData;
   ASN1C_Mission_Pion_INF_DetruireEmbuscade (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_INF_DetruireEmbuscade& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_INF_DetruireEmbuscade (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_DetruireEmbuscade* pvalue);
EXTERN int asn1PD_Mission_Pion_INF_DetruireEmbuscade (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_DetruireEmbuscade* pvalue);

EXTERN void asn1Print_Mission_Pion_INF_DetruireEmbuscade (ASN1ConstCharPtr name, ASN1T_Mission_Pion_INF_DetruireEmbuscade* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_INF_ContreAttaquer                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_INF_ContreAttaquer {
   ASN1T_KnowledgeAgent  ennemi;
} ASN1T_Mission_Pion_INF_ContreAttaquer;

class EXTERN ASN1C_Mission_Pion_INF_ContreAttaquer : public ASN1CType {
public:
   ASN1T_Mission_Pion_INF_ContreAttaquer& msgData;
   ASN1C_Mission_Pion_INF_ContreAttaquer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_INF_ContreAttaquer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_INF_ContreAttaquer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_ContreAttaquer* pvalue);
EXTERN int asn1PD_Mission_Pion_INF_ContreAttaquer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_ContreAttaquer* pvalue);

EXTERN void asn1Print_Mission_Pion_INF_ContreAttaquer (ASN1ConstCharPtr name, ASN1T_Mission_Pion_INF_ContreAttaquer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_INF_Freiner                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_INF_Freiner {
   ASN1T_Itineraire  itineraire;
} ASN1T_Mission_Pion_INF_Freiner;

class EXTERN ASN1C_Mission_Pion_INF_Freiner : public ASN1CType {
public:
   ASN1T_Mission_Pion_INF_Freiner& msgData;
   ASN1C_Mission_Pion_INF_Freiner (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_INF_Freiner& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_INF_Freiner (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_Freiner* pvalue);
EXTERN int asn1PD_Mission_Pion_INF_Freiner (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_Freiner* pvalue);

EXTERN void asn1Print_Mission_Pion_INF_Freiner (ASN1ConstCharPtr name, ASN1T_Mission_Pion_INF_Freiner* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_INF_FlancGarder                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_INF_FlancGarder {
   ASN1T_Itineraire  itineraire;
   ASN1T_Agent  unite_a_flancgarder;
} ASN1T_Mission_Pion_INF_FlancGarder;

class EXTERN ASN1C_Mission_Pion_INF_FlancGarder : public ASN1CType {
public:
   ASN1T_Mission_Pion_INF_FlancGarder& msgData;
   ASN1C_Mission_Pion_INF_FlancGarder (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_INF_FlancGarder& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_INF_FlancGarder (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_FlancGarder* pvalue);
EXTERN int asn1PD_Mission_Pion_INF_FlancGarder (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_FlancGarder* pvalue);

EXTERN void asn1Print_Mission_Pion_INF_FlancGarder (ASN1ConstCharPtr name, ASN1T_Mission_Pion_INF_FlancGarder* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_INF_OrganiserUnCheckPoint                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_INF_OrganiserUnCheckPoint {
   ASN1T_Point  point_installation;
} ASN1T_Mission_Pion_INF_OrganiserUnCheckPoint;

class EXTERN ASN1C_Mission_Pion_INF_OrganiserUnCheckPoint : public ASN1CType {
public:
   ASN1T_Mission_Pion_INF_OrganiserUnCheckPoint& msgData;
   ASN1C_Mission_Pion_INF_OrganiserUnCheckPoint (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_INF_OrganiserUnCheckPoint& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_INF_OrganiserUnCheckPoint (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_OrganiserUnCheckPoint* pvalue);
EXTERN int asn1PD_Mission_Pion_INF_OrganiserUnCheckPoint (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_OrganiserUnCheckPoint* pvalue);

EXTERN void asn1Print_Mission_Pion_INF_OrganiserUnCheckPoint (ASN1ConstCharPtr name, ASN1T_Mission_Pion_INF_OrganiserUnCheckPoint* pvalue);

/**************************************************************/
/*                                                            */
/*  EnumMissionInfNatureObjectif                              */
/*                                                            */
/**************************************************************/

struct EXTERN EnumMissionInfNatureObjectif {
   enum Root {
      elements_terrain = 0,
      objets = 1,
      elements_mobiles = 2
   } ;
} ;

typedef EnumMissionInfNatureObjectif::Root ASN1T_EnumMissionInfNatureObjectif;

class EXTERN ASN1C_EnumMissionInfNatureObjectif : public ASN1CType {
public:
   ASN1T_EnumMissionInfNatureObjectif& msgData;
   ASN1C_EnumMissionInfNatureObjectif (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumMissionInfNatureObjectif& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumMissionInfNatureObjectif (ASN1CTXT* ctxt_p, ASN1T_EnumMissionInfNatureObjectif value);
EXTERN int asn1PD_EnumMissionInfNatureObjectif (ASN1CTXT* ctxt_p, ASN1T_EnumMissionInfNatureObjectif* pvalue);

EXTERN void asn1Print_EnumMissionInfNatureObjectif (ASN1ConstCharPtr name, ASN1T_EnumMissionInfNatureObjectif* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_INF_Harceler                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_INF_Harceler {
   ASN1T_Polygon  secteur;
   ASN1T_Point  point_regroupement;
   ASN1T_EnumMissionInfNatureObjectif  nature_objectif;
} ASN1T_Mission_Pion_INF_Harceler;

class EXTERN ASN1C_Mission_Pion_INF_Harceler : public ASN1CType {
public:
   ASN1T_Mission_Pion_INF_Harceler& msgData;
   ASN1C_Mission_Pion_INF_Harceler (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_INF_Harceler& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_INF_Harceler (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_Harceler* pvalue);
EXTERN int asn1PD_Mission_Pion_INF_Harceler (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_Harceler* pvalue);

EXTERN void asn1Print_Mission_Pion_INF_Harceler (ASN1ConstCharPtr name, ASN1T_Mission_Pion_INF_Harceler* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_INF_DetruireParCoupDeMain                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_INF_DetruireParCoupDeMain {
   ASN1T_Point  objectif;
   ASN1T_Point  point_regroupement;
   ASN1T_GDH  heure_destruction;
   ASN1T_EnumMissionInfNatureObjectif  nature_destruction;
} ASN1T_Mission_Pion_INF_DetruireParCoupDeMain;

class EXTERN ASN1C_Mission_Pion_INF_DetruireParCoupDeMain : public ASN1CType {
public:
   ASN1T_Mission_Pion_INF_DetruireParCoupDeMain& msgData;
   ASN1C_Mission_Pion_INF_DetruireParCoupDeMain (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_INF_DetruireParCoupDeMain& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_INF_DetruireParCoupDeMain (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_DetruireParCoupDeMain* pvalue);
EXTERN int asn1PD_Mission_Pion_INF_DetruireParCoupDeMain (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_DetruireParCoupDeMain* pvalue);

EXTERN void asn1Print_Mission_Pion_INF_DetruireParCoupDeMain (ASN1ConstCharPtr name, ASN1T_Mission_Pion_INF_DetruireParCoupDeMain* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_INF_EscorterUnConvoi                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_INF_EscorterUnConvoi {
   ASN1T_Agent  convoi_a_escorter;
} ASN1T_Mission_Pion_INF_EscorterUnConvoi;

class EXTERN ASN1C_Mission_Pion_INF_EscorterUnConvoi : public ASN1CType {
public:
   ASN1T_Mission_Pion_INF_EscorterUnConvoi& msgData;
   ASN1C_Mission_Pion_INF_EscorterUnConvoi (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_INF_EscorterUnConvoi& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_INF_EscorterUnConvoi (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_EscorterUnConvoi* pvalue);
EXTERN int asn1PD_Mission_Pion_INF_EscorterUnConvoi (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_EscorterUnConvoi* pvalue);

EXTERN void asn1Print_Mission_Pion_INF_EscorterUnConvoi (ASN1ConstCharPtr name, ASN1T_Mission_Pion_INF_EscorterUnConvoi* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_INF_RenseignerSur                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_INF_RenseignerSur {
   ASN1T_Itineraire  itineraire;
} ASN1T_Mission_Pion_INF_RenseignerSur;

class EXTERN ASN1C_Mission_Pion_INF_RenseignerSur : public ASN1CType {
public:
   ASN1T_Mission_Pion_INF_RenseignerSur& msgData;
   ASN1C_Mission_Pion_INF_RenseignerSur (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_INF_RenseignerSur& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_INF_RenseignerSur (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_RenseignerSur* pvalue);
EXTERN int asn1PD_Mission_Pion_INF_RenseignerSur (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_RenseignerSur* pvalue);

EXTERN void asn1Print_Mission_Pion_INF_RenseignerSur (ASN1ConstCharPtr name, ASN1T_Mission_Pion_INF_RenseignerSur* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_INF_Soutenir                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_INF_Soutenir {
   ASN1T_Agent  unite_a_soutenir;
} ASN1T_Mission_Pion_INF_Soutenir;

class EXTERN ASN1C_Mission_Pion_INF_Soutenir : public ASN1CType {
public:
   ASN1T_Mission_Pion_INF_Soutenir& msgData;
   ASN1C_Mission_Pion_INF_Soutenir (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_INF_Soutenir& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_INF_Soutenir (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_Soutenir* pvalue);
EXTERN int asn1PD_Mission_Pion_INF_Soutenir (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_Soutenir* pvalue);

EXTERN void asn1Print_Mission_Pion_INF_Soutenir (ASN1ConstCharPtr name, ASN1T_Mission_Pion_INF_Soutenir* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_INF_Securiser                                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_INF_Securiser {
   ASN1T_Polygon  zone;
} ASN1T_Mission_Pion_INF_Securiser;

class EXTERN ASN1C_Mission_Pion_INF_Securiser : public ASN1CType {
public:
   ASN1T_Mission_Pion_INF_Securiser& msgData;
   ASN1C_Mission_Pion_INF_Securiser (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_INF_Securiser& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_INF_Securiser (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_Securiser* pvalue);
EXTERN int asn1PD_Mission_Pion_INF_Securiser (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_Securiser* pvalue);

EXTERN void asn1Print_Mission_Pion_INF_Securiser (ASN1ConstCharPtr name, ASN1T_Mission_Pion_INF_Securiser* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_INF_Recueillir                               */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_INF_Recueillir {
   ASN1T_Point  position_installation;
   ASN1T_Point  pia;
} ASN1T_Mission_Pion_INF_Recueillir;

class EXTERN ASN1C_Mission_Pion_INF_Recueillir : public ASN1CType {
public:
   ASN1T_Mission_Pion_INF_Recueillir& msgData;
   ASN1C_Mission_Pion_INF_Recueillir (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_INF_Recueillir& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_INF_Recueillir (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_Recueillir* pvalue);
EXTERN int asn1PD_Mission_Pion_INF_Recueillir (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_Recueillir* pvalue);

EXTERN void asn1Print_Mission_Pion_INF_Recueillir (ASN1ConstCharPtr name, ASN1T_Mission_Pion_INF_Recueillir* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_INF_RepondreAAggression                      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_INF_RepondreAAggression {
   ASN1T_KnowledgeAgent  aggresseur;
} ASN1T_Mission_Pion_INF_RepondreAAggression;

class EXTERN ASN1C_Mission_Pion_INF_RepondreAAggression : public ASN1CType {
public:
   ASN1T_Mission_Pion_INF_RepondreAAggression& msgData;
   ASN1C_Mission_Pion_INF_RepondreAAggression (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_INF_RepondreAAggression& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_INF_RepondreAAggression (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_RepondreAAggression* pvalue);
EXTERN int asn1PD_Mission_Pion_INF_RepondreAAggression (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_INF_RepondreAAggression* pvalue);

EXTERN void asn1Print_Mission_Pion_INF_RepondreAAggression (ASN1ConstCharPtr name, ASN1T_Mission_Pion_INF_RepondreAAggression* pvalue);

#endif

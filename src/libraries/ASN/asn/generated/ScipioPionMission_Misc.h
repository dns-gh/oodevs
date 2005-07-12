/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 04-Jul-2005.
 */
#ifndef SCIPIOPIONMISSION_MISC_H
#define SCIPIOPIONMISSION_MISC_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"
#include "ScipioObject.h"

/**************************************************************/
/*                                                            */
/*  Mission_Pion_Test_Heliporter                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_Test_Heliporter {
   ASN1T_ListAgent  agents;
   ASN1T_Point  point_embarquement;
   ASN1T_Point  point_debarquement;
} ASN1T_Mission_Pion_Test_Heliporter;

class EXTERN ASN1C_Mission_Pion_Test_Heliporter : public ASN1CType {
public:
   ASN1T_Mission_Pion_Test_Heliporter& msgData;
   ASN1C_Mission_Pion_Test_Heliporter (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_Test_Heliporter& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_Test_Heliporter (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_Heliporter* pvalue);
EXTERN int asn1PD_Mission_Pion_Test_Heliporter (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_Heliporter* pvalue);

EXTERN void asn1Print_Mission_Pion_Test_Heliporter (ASN1ConstCharPtr name, ASN1T_Mission_Pion_Test_Heliporter* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_Test_MoveTo_type_itineraire                  */
/*                                                            */
/**************************************************************/

struct EXTERN Mission_Pion_Test_MoveTo_type_itineraire {
   enum Root {
      movement = 0,
      recon = 1,
      infiltration = 2,
      assault = 3,
      retreat = 4,
      backup = 5
   } ;
} ;

typedef Mission_Pion_Test_MoveTo_type_itineraire::Root ASN1T_Mission_Pion_Test_MoveTo_type_itineraire;

class EXTERN ASN1C_Mission_Pion_Test_MoveTo_type_itineraire : public ASN1CType {
public:
   ASN1T_Mission_Pion_Test_MoveTo_type_itineraire& msgData;
   ASN1C_Mission_Pion_Test_MoveTo_type_itineraire (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_Test_MoveTo_type_itineraire& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_Test_MoveTo_type_itineraire (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_MoveTo_type_itineraire value);
EXTERN int asn1PD_Mission_Pion_Test_MoveTo_type_itineraire (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_MoveTo_type_itineraire* pvalue);

EXTERN void asn1Print_Mission_Pion_Test_MoveTo_type_itineraire (ASN1ConstCharPtr name, ASN1T_Mission_Pion_Test_MoveTo_type_itineraire* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_Test_MoveTo_verrouillage_vision              */
/*                                                            */
/**************************************************************/

struct EXTERN Mission_Pion_Test_MoveTo_verrouillage_vision {
   enum Root {
      rien = 0,
      point = 1,
      direction = 2
   } ;
} ;

typedef Mission_Pion_Test_MoveTo_verrouillage_vision::Root ASN1T_Mission_Pion_Test_MoveTo_verrouillage_vision;

class EXTERN ASN1C_Mission_Pion_Test_MoveTo_verrouillage_vision : public ASN1CType {
public:
   ASN1T_Mission_Pion_Test_MoveTo_verrouillage_vision& msgData;
   ASN1C_Mission_Pion_Test_MoveTo_verrouillage_vision (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_Test_MoveTo_verrouillage_vision& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_Test_MoveTo_verrouillage_vision (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_MoveTo_verrouillage_vision value);
EXTERN int asn1PD_Mission_Pion_Test_MoveTo_verrouillage_vision (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_MoveTo_verrouillage_vision* pvalue);

EXTERN void asn1Print_Mission_Pion_Test_MoveTo_verrouillage_vision (ASN1ConstCharPtr name, ASN1T_Mission_Pion_Test_MoveTo_verrouillage_vision* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_Test_MoveTo                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_Test_MoveTo {
   ASN1T_Itineraire  itineraire;
   ASN1T_Mission_Pion_Test_MoveTo_type_itineraire  type_itineraire;
   ASN1BOOL  debarque;
   ASN1T_Mission_Pion_Test_MoveTo_verrouillage_vision  verrouillage_vision;
   ASN1T_Point  vision_point;
   ASN1T_Direction  vision_direction;
} ASN1T_Mission_Pion_Test_MoveTo;

class EXTERN ASN1C_Mission_Pion_Test_MoveTo : public ASN1CType {
public:
   ASN1T_Mission_Pion_Test_MoveTo& msgData;
   ASN1C_Mission_Pion_Test_MoveTo (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_Test_MoveTo& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_Test_MoveTo (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_MoveTo* pvalue);
EXTERN int asn1PD_Mission_Pion_Test_MoveTo (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_MoveTo* pvalue);

EXTERN void asn1Print_Mission_Pion_Test_MoveTo (ASN1ConstCharPtr name, ASN1T_Mission_Pion_Test_MoveTo* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_Test_CreateObject                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_Test_CreateObject {
   ASN1T_Localisation  forme;
   ASN1T_EnumObjectType  type;
   ASN1INT   param;
} ASN1T_Mission_Pion_Test_CreateObject;

class EXTERN ASN1C_Mission_Pion_Test_CreateObject : public ASN1CType {
public:
   ASN1T_Mission_Pion_Test_CreateObject& msgData;
   ASN1C_Mission_Pion_Test_CreateObject (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_Test_CreateObject& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_Test_CreateObject (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_CreateObject* pvalue);
EXTERN int asn1PD_Mission_Pion_Test_CreateObject (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_CreateObject* pvalue);

EXTERN void asn1Print_Mission_Pion_Test_CreateObject (ASN1ConstCharPtr name, ASN1T_Mission_Pion_Test_CreateObject* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_Test_DestroyObject                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_Test_DestroyObject {
   ASN1T_KnowledgeObject  objet;
} ASN1T_Mission_Pion_Test_DestroyObject;

class EXTERN ASN1C_Mission_Pion_Test_DestroyObject : public ASN1CType {
public:
   ASN1T_Mission_Pion_Test_DestroyObject& msgData;
   ASN1C_Mission_Pion_Test_DestroyObject (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_Test_DestroyObject& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_Test_DestroyObject (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_DestroyObject* pvalue);
EXTERN int asn1PD_Mission_Pion_Test_DestroyObject (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_DestroyObject* pvalue);

EXTERN void asn1Print_Mission_Pion_Test_DestroyObject (ASN1ConstCharPtr name, ASN1T_Mission_Pion_Test_DestroyObject* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_Test_CreateBypass                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_Test_CreateBypass {
   ASN1T_KnowledgeObject  objet;
} ASN1T_Mission_Pion_Test_CreateBypass;

class EXTERN ASN1C_Mission_Pion_Test_CreateBypass : public ASN1CType {
public:
   ASN1T_Mission_Pion_Test_CreateBypass& msgData;
   ASN1C_Mission_Pion_Test_CreateBypass (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_Test_CreateBypass& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_Test_CreateBypass (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_CreateBypass* pvalue);
EXTERN int asn1PD_Mission_Pion_Test_CreateBypass (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_CreateBypass* pvalue);

EXTERN void asn1Print_Mission_Pion_Test_CreateBypass (ASN1ConstCharPtr name, ASN1T_Mission_Pion_Test_CreateBypass* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_Test_Fire_munitions                          */
/*                                                            */
/**************************************************************/

struct EXTERN Mission_Pion_Test_Fire_munitions {
   enum Root {
      mun_obus_explosif = 0,
      mun_obus_grenade = 1,
      mun_obus_aced = 2,
      mun_obus_fumigene = 3,
      mun_obus_eclairant = 4,
      mun_obus_mine = 5
   } ;
} ;

typedef Mission_Pion_Test_Fire_munitions::Root ASN1T_Mission_Pion_Test_Fire_munitions;

class EXTERN ASN1C_Mission_Pion_Test_Fire_munitions : public ASN1CType {
public:
   ASN1T_Mission_Pion_Test_Fire_munitions& msgData;
   ASN1C_Mission_Pion_Test_Fire_munitions (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_Test_Fire_munitions& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_Test_Fire_munitions (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_Fire_munitions value);
EXTERN int asn1PD_Mission_Pion_Test_Fire_munitions (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_Fire_munitions* pvalue);

EXTERN void asn1Print_Mission_Pion_Test_Fire_munitions (ASN1ConstCharPtr name, ASN1T_Mission_Pion_Test_Fire_munitions* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_Test_Fire                                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_Test_Fire {
   ASN1T_Point  point;
   ASN1BOOL  tir_indirect;
   ASN1T_Mission_Pion_Test_Fire_munitions  munitions;
   ASN1INT   nb_obus;
   ASN1T_ListKnowledgeAgent  targets;
} ASN1T_Mission_Pion_Test_Fire;

class EXTERN ASN1C_Mission_Pion_Test_Fire : public ASN1CType {
public:
   ASN1T_Mission_Pion_Test_Fire& msgData;
   ASN1C_Mission_Pion_Test_Fire (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_Test_Fire& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_Test_Fire (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_Fire* pvalue);
EXTERN int asn1PD_Mission_Pion_Test_Fire (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_Fire* pvalue);

EXTERN void asn1Print_Mission_Pion_Test_Fire (ASN1ConstCharPtr name, ASN1T_Mission_Pion_Test_Fire* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_Test_ChangePosture_posture                   */
/*                                                            */
/**************************************************************/

struct EXTERN Mission_Pion_Test_ChangePosture_posture {
   enum Root {
      mouvement = 0,
      arret = 1,
      poste_reflexe = 2,
      poste = 3,
      poste_amenage = 4,
      poste_prepare_genie = 5,
      automatique = 6
   } ;
} ;

typedef Mission_Pion_Test_ChangePosture_posture::Root ASN1T_Mission_Pion_Test_ChangePosture_posture;

class EXTERN ASN1C_Mission_Pion_Test_ChangePosture_posture : public ASN1CType {
public:
   ASN1T_Mission_Pion_Test_ChangePosture_posture& msgData;
   ASN1C_Mission_Pion_Test_ChangePosture_posture (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_Test_ChangePosture_posture& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_Test_ChangePosture_posture (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_ChangePosture_posture value);
EXTERN int asn1PD_Mission_Pion_Test_ChangePosture_posture (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_ChangePosture_posture* pvalue);

EXTERN void asn1Print_Mission_Pion_Test_ChangePosture_posture (ASN1ConstCharPtr name, ASN1T_Mission_Pion_Test_ChangePosture_posture* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_Test_ChangePosture                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_Test_ChangePosture {
   ASN1T_Mission_Pion_Test_ChangePosture_posture  posture;
} ASN1T_Mission_Pion_Test_ChangePosture;

class EXTERN ASN1C_Mission_Pion_Test_ChangePosture : public ASN1CType {
public:
   ASN1T_Mission_Pion_Test_ChangePosture& msgData;
   ASN1C_Mission_Pion_Test_ChangePosture (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_Test_ChangePosture& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_Test_ChangePosture (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_ChangePosture* pvalue);
EXTERN int asn1PD_Mission_Pion_Test_ChangePosture (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_ChangePosture* pvalue);

EXTERN void asn1Print_Mission_Pion_Test_ChangePosture (ASN1ConstCharPtr name, ASN1T_Mission_Pion_Test_ChangePosture* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_Test_PathFind                                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_Test_PathFind {
   ASN1INT   nb_pathfind;
} ASN1T_Mission_Pion_Test_PathFind;

class EXTERN ASN1C_Mission_Pion_Test_PathFind : public ASN1CType {
public:
   ASN1T_Mission_Pion_Test_PathFind& msgData;
   ASN1C_Mission_Pion_Test_PathFind (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_Test_PathFind& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_Test_PathFind (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_PathFind* pvalue);
EXTERN int asn1PD_Mission_Pion_Test_PathFind (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_PathFind* pvalue);

EXTERN void asn1Print_Mission_Pion_Test_PathFind (ASN1ConstCharPtr name, ASN1T_Mission_Pion_Test_PathFind* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_Test_FindPosition_type_objectif              */
/*                                                            */
/**************************************************************/

struct EXTERN Mission_Pion_Test_FindPosition_type_objectif {
   enum Root {
      agent = 0,
      point = 1,
      rep_point = 2,
      objet = 3,
      zone = 4
   } ;
} ;

typedef Mission_Pion_Test_FindPosition_type_objectif::Root ASN1T_Mission_Pion_Test_FindPosition_type_objectif;

class EXTERN ASN1C_Mission_Pion_Test_FindPosition_type_objectif : public ASN1CType {
public:
   ASN1T_Mission_Pion_Test_FindPosition_type_objectif& msgData;
   ASN1C_Mission_Pion_Test_FindPosition_type_objectif (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_Test_FindPosition_type_objectif& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_Test_FindPosition_type_objectif (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_FindPosition_type_objectif value);
EXTERN int asn1PD_Mission_Pion_Test_FindPosition_type_objectif (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_FindPosition_type_objectif* pvalue);

EXTERN void asn1Print_Mission_Pion_Test_FindPosition_type_objectif (ASN1ConstCharPtr name, ASN1T_Mission_Pion_Test_FindPosition_type_objectif* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_Test_FindPosition_type_operation             */
/*                                                            */
/**************************************************************/

struct EXTERN Mission_Pion_Test_FindPosition_type_operation {
   enum Root {
      see = 0,
      shoot = 1,
      add = 2,
      rem = 3,
      safety = 4,
      border = 5,
      appui = 6,
      support = 7,
      ambush = 8,
      random = 9
   } ;
} ;

typedef Mission_Pion_Test_FindPosition_type_operation::Root ASN1T_Mission_Pion_Test_FindPosition_type_operation;

class EXTERN ASN1C_Mission_Pion_Test_FindPosition_type_operation : public ASN1CType {
public:
   ASN1T_Mission_Pion_Test_FindPosition_type_operation& msgData;
   ASN1C_Mission_Pion_Test_FindPosition_type_operation (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_Test_FindPosition_type_operation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_Test_FindPosition_type_operation (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_FindPosition_type_operation value);
EXTERN int asn1PD_Mission_Pion_Test_FindPosition_type_operation (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_FindPosition_type_operation* pvalue);

EXTERN void asn1Print_Mission_Pion_Test_FindPosition_type_operation (ASN1ConstCharPtr name, ASN1T_Mission_Pion_Test_FindPosition_type_operation* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_Test_FindPosition                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_Test_FindPosition {
   ASN1T_Point  point;
   ASN1T_Point  retreat;
   ASN1T_ListAgent  unite;
   ASN1T_Mission_Pion_Test_FindPosition_type_objectif  type_objectif;
   ASN1T_Mission_Pion_Test_FindPosition_type_operation  type_operation;
   ASN1BOOL  avoid;
   ASN1BOOL  reco;
   ASN1BOOL  no_obstacle_avoid;
   ASN1BOOL  debordement;
   ASN1BOOL  dec_points;
   ASN1T_EnumUnitIdentificationLevel  niveau_identification;
   ASN1T_Direction  dgo;
} ASN1T_Mission_Pion_Test_FindPosition;

class EXTERN ASN1C_Mission_Pion_Test_FindPosition : public ASN1CType {
public:
   ASN1T_Mission_Pion_Test_FindPosition& msgData;
   ASN1C_Mission_Pion_Test_FindPosition (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_Test_FindPosition& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_Test_FindPosition (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_FindPosition* pvalue);
EXTERN int asn1PD_Mission_Pion_Test_FindPosition (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_FindPosition* pvalue);

EXTERN void asn1Print_Mission_Pion_Test_FindPosition (ASN1ConstCharPtr name, ASN1T_Mission_Pion_Test_FindPosition* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_Test_Reinforce                               */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_Test_Reinforce {
   ASN1T_Agent  unite;
   ASN1BOOL  renforce;
} ASN1T_Mission_Pion_Test_Reinforce;

class EXTERN ASN1C_Mission_Pion_Test_Reinforce : public ASN1CType {
public:
   ASN1T_Mission_Pion_Test_Reinforce& msgData;
   ASN1C_Mission_Pion_Test_Reinforce (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_Test_Reinforce& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_Test_Reinforce (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_Reinforce* pvalue);
EXTERN int asn1PD_Mission_Pion_Test_Reinforce (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Test_Reinforce* pvalue);

EXTERN void asn1Print_Mission_Pion_Test_Reinforce (ASN1ConstCharPtr name, ASN1T_Mission_Pion_Test_Reinforce* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_PlastronEnnemi_type                          */
/*                                                            */
/**************************************************************/

struct EXTERN Mission_Pion_PlastronEnnemi_type {
   enum Root {
      defense_statique = 0,
      defense_dynamique = 1,
      esquive = 2,
      reco_offensive = 3
   } ;
} ;

typedef Mission_Pion_PlastronEnnemi_type::Root ASN1T_Mission_Pion_PlastronEnnemi_type;

class EXTERN ASN1C_Mission_Pion_PlastronEnnemi_type : public ASN1CType {
public:
   ASN1T_Mission_Pion_PlastronEnnemi_type& msgData;
   ASN1C_Mission_Pion_PlastronEnnemi_type (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_PlastronEnnemi_type& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_PlastronEnnemi_type (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_PlastronEnnemi_type value);
EXTERN int asn1PD_Mission_Pion_PlastronEnnemi_type (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_PlastronEnnemi_type* pvalue);

EXTERN void asn1Print_Mission_Pion_PlastronEnnemi_type (ASN1ConstCharPtr name, ASN1T_Mission_Pion_PlastronEnnemi_type* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_PlastronEnnemi                               */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_PlastronEnnemi {
   ASN1T_Itineraire  itineraire;
   ASN1T_Point  point_mission;
   ASN1T_Mission_Pion_PlastronEnnemi_type  type;
} ASN1T_Mission_Pion_PlastronEnnemi;

class EXTERN ASN1C_Mission_Pion_PlastronEnnemi : public ASN1CType {
public:
   ASN1T_Mission_Pion_PlastronEnnemi& msgData;
   ASN1C_Mission_Pion_PlastronEnnemi (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_PlastronEnnemi& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_PlastronEnnemi (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_PlastronEnnemi* pvalue);
EXTERN int asn1PD_Mission_Pion_PlastronEnnemi (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_PlastronEnnemi* pvalue);

EXTERN void asn1Print_Mission_Pion_PlastronEnnemi (ASN1ConstCharPtr name, ASN1T_Mission_Pion_PlastronEnnemi* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_FaireMouvement                               */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_FaireMouvement {
   ASN1T_Itineraire  itineraire;
} ASN1T_Mission_Pion_FaireMouvement;

class EXTERN ASN1C_Mission_Pion_FaireMouvement : public ASN1CType {
public:
   ASN1T_Mission_Pion_FaireMouvement& msgData;
   ASN1C_Mission_Pion_FaireMouvement (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_FaireMouvement& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_FaireMouvement (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_FaireMouvement* pvalue);
EXTERN int asn1PD_Mission_Pion_FaireMouvement (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_FaireMouvement* pvalue);

EXTERN void asn1Print_Mission_Pion_FaireMouvement (ASN1ConstCharPtr name, ASN1T_Mission_Pion_FaireMouvement* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_Suivre                                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_Suivre {
   ASN1T_Agent  unite_a_suivre;
} ASN1T_Mission_Pion_Suivre;

class EXTERN ASN1C_Mission_Pion_Suivre : public ASN1CType {
public:
   ASN1T_Mission_Pion_Suivre& msgData;
   ASN1C_Mission_Pion_Suivre (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_Suivre& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_Suivre (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Suivre* pvalue);
EXTERN int asn1PD_Mission_Pion_Suivre (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Suivre* pvalue);

EXTERN void asn1Print_Mission_Pion_Suivre (ASN1ConstCharPtr name, ASN1T_Mission_Pion_Suivre* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_SeFaireDecontaminer                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_SeFaireDecontaminer {
   ASN1T_Point  point_arrivee;
   ASN1T_KnowledgeObject  site_decontamination;
} ASN1T_Mission_Pion_SeFaireDecontaminer;

class EXTERN ASN1C_Mission_Pion_SeFaireDecontaminer : public ASN1CType {
public:
   ASN1T_Mission_Pion_SeFaireDecontaminer& msgData;
   ASN1C_Mission_Pion_SeFaireDecontaminer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_SeFaireDecontaminer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_SeFaireDecontaminer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_SeFaireDecontaminer* pvalue);
EXTERN int asn1PD_Mission_Pion_SeFaireDecontaminer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_SeFaireDecontaminer* pvalue);

EXTERN void asn1Print_Mission_Pion_SeFaireDecontaminer (ASN1ConstCharPtr name, ASN1T_Mission_Pion_SeFaireDecontaminer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_Franchir                                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_Franchir {
   ASN1T_KnowledgeObject  pont_flottant;
   ASN1T_Point  point_arrivee;
} ASN1T_Mission_Pion_Franchir;

class EXTERN ASN1C_Mission_Pion_Franchir : public ASN1CType {
public:
   ASN1T_Mission_Pion_Franchir& msgData;
   ASN1C_Mission_Pion_Franchir (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_Franchir& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_Franchir (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Franchir* pvalue);
EXTERN int asn1PD_Mission_Pion_Franchir (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Franchir* pvalue);

EXTERN void asn1Print_Mission_Pion_Franchir (ASN1ConstCharPtr name, ASN1T_Mission_Pion_Franchir* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_Decrocher                                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_Decrocher {
   ASN1T_Point  point_regroupement;
} ASN1T_Mission_Pion_Decrocher;

class EXTERN ASN1C_Mission_Pion_Decrocher : public ASN1CType {
public:
   ASN1T_Mission_Pion_Decrocher& msgData;
   ASN1C_Mission_Pion_Decrocher (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_Decrocher& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_Decrocher (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Decrocher* pvalue);
EXTERN int asn1PD_Mission_Pion_Decrocher (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Decrocher* pvalue);

EXTERN void asn1Print_Mission_Pion_Decrocher (ASN1ConstCharPtr name, ASN1T_Mission_Pion_Decrocher* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_Stationner                                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_Stationner {
   ASN1T_Point  point_attente;
} ASN1T_Mission_Pion_Stationner;

class EXTERN ASN1C_Mission_Pion_Stationner : public ASN1CType {
public:
   ASN1T_Mission_Pion_Stationner& msgData;
   ASN1C_Mission_Pion_Stationner (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_Stationner& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_Stationner (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Stationner* pvalue);
EXTERN int asn1PD_Mission_Pion_Stationner (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Stationner* pvalue);

EXTERN void asn1Print_Mission_Pion_Stationner (ASN1ConstCharPtr name, ASN1T_Mission_Pion_Stationner* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_SeFaireTransporter                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_SeFaireTransporter {
   ASN1T_Point  point_embarquement;
} ASN1T_Mission_Pion_SeFaireTransporter;

class EXTERN ASN1C_Mission_Pion_SeFaireTransporter : public ASN1CType {
public:
   ASN1T_Mission_Pion_SeFaireTransporter& msgData;
   ASN1C_Mission_Pion_SeFaireTransporter (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_SeFaireTransporter& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_SeFaireTransporter (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_SeFaireTransporter* pvalue);
EXTERN int asn1PD_Mission_Pion_SeFaireTransporter (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_SeFaireTransporter* pvalue);

EXTERN void asn1Print_Mission_Pion_SeFaireTransporter (ASN1ConstCharPtr name, ASN1T_Mission_Pion_SeFaireTransporter* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_Generique                                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_Generique {
   ASN1T_ListPolygon  zones;
   ASN1T_ListPoint  points;
   ASN1T_ListItineraire  itineraires;
   ASN1T_ListAgent  unites;
   ASN1T_ListAutomate  automates;
   ASN1T_ListKnowledgeAgent  connaissances_agents;
   ASN1T_ListKnowledgeObject  connaissances_objets;
} ASN1T_Mission_Pion_Generique;

class EXTERN ASN1C_Mission_Pion_Generique : public ASN1CType {
public:
   ASN1T_Mission_Pion_Generique& msgData;
   ASN1C_Mission_Pion_Generique (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_Generique& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_Generique (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Generique* pvalue);
EXTERN int asn1PD_Mission_Pion_Generique (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_Generique* pvalue);

EXTERN void asn1Print_Mission_Pion_Generique (ASN1ConstCharPtr name, ASN1T_Mission_Pion_Generique* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_MILICE_Provoquer                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_MILICE_Provoquer {
   ASN1T_KnowledgeAgent  unite_a_provoquer;
} ASN1T_Mission_Pion_MILICE_Provoquer;

class EXTERN ASN1C_Mission_Pion_MILICE_Provoquer : public ASN1CType {
public:
   ASN1T_Mission_Pion_MILICE_Provoquer& msgData;
   ASN1C_Mission_Pion_MILICE_Provoquer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_MILICE_Provoquer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_MILICE_Provoquer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_MILICE_Provoquer* pvalue);
EXTERN int asn1PD_Mission_Pion_MILICE_Provoquer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_MILICE_Provoquer* pvalue);

EXTERN void asn1Print_Mission_Pion_MILICE_Provoquer (ASN1ConstCharPtr name, ASN1T_Mission_Pion_MILICE_Provoquer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_MILICE_PrendreEtTenir                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_MILICE_PrendreEtTenir {
   ASN1T_Point  point_a_prendre;
} ASN1T_Mission_Pion_MILICE_PrendreEtTenir;

class EXTERN ASN1C_Mission_Pion_MILICE_PrendreEtTenir : public ASN1CType {
public:
   ASN1T_Mission_Pion_MILICE_PrendreEtTenir& msgData;
   ASN1C_Mission_Pion_MILICE_PrendreEtTenir (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_MILICE_PrendreEtTenir& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_MILICE_PrendreEtTenir (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_MILICE_PrendreEtTenir* pvalue);
EXTERN int asn1PD_Mission_Pion_MILICE_PrendreEtTenir (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_MILICE_PrendreEtTenir* pvalue);

EXTERN void asn1Print_Mission_Pion_MILICE_PrendreEtTenir (ASN1ConstCharPtr name, ASN1T_Mission_Pion_MILICE_PrendreEtTenir* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_MILICE_DetruireEmbuscade                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_MILICE_DetruireEmbuscade {
   ASN1T_Point  position_embuscade;
   ASN1T_Point  position_regroupement;
   ASN1T_EnumObjectType  nature_obstacle;
} ASN1T_Mission_Pion_MILICE_DetruireEmbuscade;

class EXTERN ASN1C_Mission_Pion_MILICE_DetruireEmbuscade : public ASN1CType {
public:
   ASN1T_Mission_Pion_MILICE_DetruireEmbuscade& msgData;
   ASN1C_Mission_Pion_MILICE_DetruireEmbuscade (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_MILICE_DetruireEmbuscade& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_MILICE_DetruireEmbuscade (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_MILICE_DetruireEmbuscade* pvalue);
EXTERN int asn1PD_Mission_Pion_MILICE_DetruireEmbuscade (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_MILICE_DetruireEmbuscade* pvalue);

EXTERN void asn1Print_Mission_Pion_MILICE_DetruireEmbuscade (ASN1ConstCharPtr name, ASN1T_Mission_Pion_MILICE_DetruireEmbuscade* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_JOINT_MARINE_Transporter                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_JOINT_MARINE_Transporter {
   ASN1T_ListAgent  unites_a_transporter;
   ASN1T_Point  point_debarquement;
   ASN1T_Point  point_embarquement;
   ASN1BOOL  avec_materiel;

   ASN1T_Mission_Pion_JOINT_MARINE_Transporter () {
      avec_materiel = FALSE;
   }
} ASN1T_Mission_Pion_JOINT_MARINE_Transporter;

class EXTERN ASN1C_Mission_Pion_JOINT_MARINE_Transporter : public ASN1CType {
public:
   ASN1T_Mission_Pion_JOINT_MARINE_Transporter& msgData;
   ASN1C_Mission_Pion_JOINT_MARINE_Transporter (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_JOINT_MARINE_Transporter& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_JOINT_MARINE_Transporter (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_JOINT_MARINE_Transporter* pvalue);
EXTERN int asn1PD_Mission_Pion_JOINT_MARINE_Transporter (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_JOINT_MARINE_Transporter* pvalue);

EXTERN void asn1Print_Mission_Pion_JOINT_MARINE_Transporter (ASN1ConstCharPtr name, ASN1T_Mission_Pion_JOINT_MARINE_Transporter* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_JOINT_AIR_EffectuerRaid                      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_JOINT_AIR_EffectuerRaid {
   ASN1T_Itineraire  itineraire;
   ASN1T_Polygon  zone;
} ASN1T_Mission_Pion_JOINT_AIR_EffectuerRaid;

class EXTERN ASN1C_Mission_Pion_JOINT_AIR_EffectuerRaid : public ASN1CType {
public:
   ASN1T_Mission_Pion_JOINT_AIR_EffectuerRaid& msgData;
   ASN1C_Mission_Pion_JOINT_AIR_EffectuerRaid (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_JOINT_AIR_EffectuerRaid& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_JOINT_AIR_EffectuerRaid (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_JOINT_AIR_EffectuerRaid* pvalue);
EXTERN int asn1PD_Mission_Pion_JOINT_AIR_EffectuerRaid (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_JOINT_AIR_EffectuerRaid* pvalue);

EXTERN void asn1Print_Mission_Pion_JOINT_AIR_EffectuerRaid (ASN1ConstCharPtr name, ASN1T_Mission_Pion_JOINT_AIR_EffectuerRaid* pvalue);

#endif

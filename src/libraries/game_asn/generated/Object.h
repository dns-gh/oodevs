/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 25-Jun-2007.
 */
#ifndef OBJECT_H
#define OBJECT_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "Base.h"

/**************************************************************/
/*                                                            */
/*  ObjectIncludes                                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectIncludes {
} ASN1T_ObjectIncludes;

class EXTERN ASN1C_ObjectIncludes : public ASN1CType {
public:
   ASN1T_ObjectIncludes& msgData;
   ASN1C_ObjectIncludes (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectIncludes& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectIncludes (ASN1CTXT* ctxt_p, ASN1T_ObjectIncludes* pvalue);
EXTERN int asn1PD_ObjectIncludes (ASN1CTXT* ctxt_p, ASN1T_ObjectIncludes* pvalue);

EXTERN void asn1Print_ObjectIncludes (ASN1ConstCharPtr name, ASN1T_ObjectIncludes* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectExports                                             */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_ObjectExports : public ASN1CType {
public:
   ASN1C_ObjectExports (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectExports (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_ObjectExports (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_ObjectExports (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  EnumObstacleType                                          */
/*                                                            */
/**************************************************************/

struct EXTERN EnumObstacleType {
   enum Root {
      initial = 0,
      reserved = 1
   } ;
} ;

typedef EnumObstacleType::Root ASN1T_EnumObstacleType;

class EXTERN ASN1C_EnumObstacleType : public ASN1CType {
public:
   ASN1T_EnumObstacleType& msgData;
   ASN1C_EnumObstacleType (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumObstacleType& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumObstacleType (ASN1CTXT* ctxt_p, ASN1T_EnumObstacleType value);
EXTERN int asn1PD_EnumObstacleType (ASN1CTXT* ctxt_p, ASN1T_EnumObstacleType* pvalue);

EXTERN void asn1Print_EnumObstacleType (ASN1ConstCharPtr name, ASN1T_EnumObstacleType* pvalue);

/**************************************************************/
/*                                                            */
/*  EnumObjectType                                            */
/*                                                            */
/**************************************************************/

struct EXTERN EnumObjectType {
   enum Root {
      bouchon_mines = 0,
      zone_minee_lineaire = 1,
      zone_minee_par_dispersion = 2,
      fosse_anti_char = 3,
      abattis = 4,
      barricade = 5,
      eboulement = 6,
      destruction_route = 7,
      destruction_pont = 8,
      pont_flottant_continu = 9,
      poste_tir = 10,
      zone_protegee = 11,
      zone_implantation_canon = 12,
      zone_implantation_cobra = 13,
      zone_implantation_lrm = 14,
      site_franchissement = 15,
      nuage_nbc = 16,
      plot_ravitaillement = 17,
      site_decontamination = 18,
      zone_brouillage_brod = 19,
      rota = 20,
      zone_nbc = 21,
      zone_brouillage_bromure = 22,
      aire_poser = 23,
      piste = 24,
      plateforme = 25,
      zone_mobilite_amelioree = 26,
      zone_poser_helicoptere = 27,
      aire_logistique = 28,
      itineraire_logistique = 29,
      camp_prisonniers = 30,
      camp_refugies = 31,
      poste_controle = 32,
      terrain_largage = 33,
      zone_interdite_mouvement = 34,
      zone_interdite_tir = 35,
      zone_implantation_mortier = 36,
      pont_flottant_discontinu = 37,
      installation = 38
   } ;
} ;

typedef EnumObjectType::Root ASN1T_EnumObjectType;

class EXTERN ASN1C_EnumObjectType : public ASN1CType {
public:
   ASN1T_EnumObjectType& msgData;
   ASN1C_EnumObjectType (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumObjectType& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumObjectType (ASN1CTXT* ctxt_p, ASN1T_EnumObjectType value);
EXTERN int asn1PD_EnumObjectType (ASN1CTXT* ctxt_p, ASN1T_EnumObjectType* pvalue);

EXTERN void asn1Print_EnumObjectType (ASN1ConstCharPtr name, ASN1T_EnumObjectType* pvalue);

/**************************************************************/
/*                                                            */
/*  EnumObjectErrorCode                                       */
/*                                                            */
/**************************************************************/

struct EXTERN EnumObjectErrorCode {
   enum Root {
      no_error = 0,
      error_invalid_object = 1,
      error_invalid_id = 2,
      error_invalid_camp = 3,
      error_invalid_localisation = 4,
      error_missing_specific_attributes = 5,
      error_invalid_specific_attributes = 6
   } ;
} ;

typedef EnumObjectErrorCode::Root ASN1T_EnumObjectErrorCode;

class EXTERN ASN1C_EnumObjectErrorCode : public ASN1CType {
public:
   ASN1T_EnumObjectErrorCode& msgData;
   ASN1C_EnumObjectErrorCode (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumObjectErrorCode& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumObjectErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumObjectErrorCode value);
EXTERN int asn1PD_EnumObjectErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumObjectErrorCode* pvalue);

EXTERN void asn1Print_EnumObjectErrorCode (ASN1ConstCharPtr name, ASN1T_EnumObjectErrorCode* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributesCrossingSite                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributesCrossingSite {
   ASN1INT   width;
   ASN1INT   depth;
   ASN1INT   flow_rate;
   ASN1BOOL  banks_require_fitting;
} ASN1T_ObjectAttributesCrossingSite;

class EXTERN ASN1C_ObjectAttributesCrossingSite : public ASN1CType {
public:
   ASN1T_ObjectAttributesCrossingSite& msgData;
   ASN1C_ObjectAttributesCrossingSite (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributesCrossingSite& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributesCrossingSite (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributesCrossingSite* pvalue);
EXTERN int asn1PD_ObjectAttributesCrossingSite (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributesCrossingSite* pvalue);

EXTERN void asn1Print_ObjectAttributesCrossingSite (ASN1ConstCharPtr name, ASN1T_ObjectAttributesCrossingSite* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributesNbcCloud                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributesNbcCloud {
   ASN1T_OID  nbc_agent;
} ASN1T_ObjectAttributesNbcCloud;

class EXTERN ASN1C_ObjectAttributesNbcCloud : public ASN1CType {
public:
   ASN1T_ObjectAttributesNbcCloud& msgData;
   ASN1C_ObjectAttributesNbcCloud (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributesNbcCloud& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributesNbcCloud (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributesNbcCloud* pvalue);
EXTERN int asn1PD_ObjectAttributesNbcCloud (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributesNbcCloud* pvalue);

EXTERN void asn1Print_ObjectAttributesNbcCloud (ASN1ConstCharPtr name, ASN1T_ObjectAttributesNbcCloud* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributesNbcZone                                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributesNbcZone {
   ASN1T_OID  nbc_agent;
} ASN1T_ObjectAttributesNbcZone;

class EXTERN ASN1C_ObjectAttributesNbcZone : public ASN1CType {
public:
   ASN1T_ObjectAttributesNbcZone& msgData;
   ASN1C_ObjectAttributesNbcZone (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributesNbcZone& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributesNbcZone (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributesNbcZone* pvalue);
EXTERN int asn1PD_ObjectAttributesNbcZone (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributesNbcZone* pvalue);

EXTERN void asn1Print_ObjectAttributesNbcZone (ASN1ConstCharPtr name, ASN1T_ObjectAttributesNbcZone* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributesRota                                      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributesRota {
   ASN1INT   danger_level;
   ASN1T_ListOID  nbc_agents;
} ASN1T_ObjectAttributesRota;

class EXTERN ASN1C_ObjectAttributesRota : public ASN1CType {
public:
   ASN1T_ObjectAttributesRota& msgData;
   ASN1C_ObjectAttributesRota (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributesRota& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributesRota (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributesRota* pvalue);
EXTERN int asn1PD_ObjectAttributesRota (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributesRota* pvalue);

EXTERN void asn1Print_ObjectAttributesRota (ASN1ConstCharPtr name, ASN1T_ObjectAttributesRota* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributesLogisticRoute                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributesLogisticRoute {
   ASN1BOOL  equipped;
   ASN1INT   max_weight;
   ASN1INT   width;
   ASN1INT   length;
   ASN1INT   flow_rate;
} ASN1T_ObjectAttributesLogisticRoute;

class EXTERN ASN1C_ObjectAttributesLogisticRoute : public ASN1CType {
public:
   ASN1T_ObjectAttributesLogisticRoute& msgData;
   ASN1C_ObjectAttributesLogisticRoute (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributesLogisticRoute& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributesLogisticRoute (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributesLogisticRoute* pvalue);
EXTERN int asn1PD_ObjectAttributesLogisticRoute (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributesLogisticRoute* pvalue);

EXTERN void asn1Print_ObjectAttributesLogisticRoute (ASN1ConstCharPtr name, ASN1T_ObjectAttributesLogisticRoute* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributesPrisonerCamp                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributesPrisonerCamp {
   ASN1T_Automat  tc2;
} ASN1T_ObjectAttributesPrisonerCamp;

class EXTERN ASN1C_ObjectAttributesPrisonerCamp : public ASN1CType {
public:
   ASN1T_ObjectAttributesPrisonerCamp& msgData;
   ASN1C_ObjectAttributesPrisonerCamp (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributesPrisonerCamp& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributesPrisonerCamp (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributesPrisonerCamp* pvalue);
EXTERN int asn1PD_ObjectAttributesPrisonerCamp (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributesPrisonerCamp* pvalue);

EXTERN void asn1Print_ObjectAttributesPrisonerCamp (ASN1ConstCharPtr name, ASN1T_ObjectAttributesPrisonerCamp* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributesRefugeeCamp                               */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributesRefugeeCamp {
   ASN1T_Automat  tc2;
} ASN1T_ObjectAttributesRefugeeCamp;

class EXTERN ASN1C_ObjectAttributesRefugeeCamp : public ASN1CType {
public:
   ASN1T_ObjectAttributesRefugeeCamp& msgData;
   ASN1C_ObjectAttributesRefugeeCamp (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributesRefugeeCamp& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributesRefugeeCamp (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributesRefugeeCamp* pvalue);
EXTERN int asn1PD_ObjectAttributesRefugeeCamp (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributesRefugeeCamp* pvalue);

EXTERN void asn1Print_ObjectAttributesRefugeeCamp (ASN1ConstCharPtr name, ASN1T_ObjectAttributesRefugeeCamp* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributesLinearMineArea                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributesLinearMineArea {
   ASN1REAL  density;
   ASN1INT   activity_time;

   ASN1T_ObjectAttributesLinearMineArea () {
      density = 0;
      activity_time = 0;
   }
} ASN1T_ObjectAttributesLinearMineArea;

class EXTERN ASN1C_ObjectAttributesLinearMineArea : public ASN1CType {
public:
   ASN1T_ObjectAttributesLinearMineArea& msgData;
   ASN1C_ObjectAttributesLinearMineArea (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributesLinearMineArea& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributesLinearMineArea (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributesLinearMineArea* pvalue);
EXTERN int asn1PD_ObjectAttributesLinearMineArea (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributesLinearMineArea* pvalue);

EXTERN void asn1Print_ObjectAttributesLinearMineArea (ASN1ConstCharPtr name, ASN1T_ObjectAttributesLinearMineArea* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributesDispersedMineArea                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributesDispersedMineArea {
   ASN1REAL  density;
   ASN1INT   activity_time;

   ASN1T_ObjectAttributesDispersedMineArea () {
      density = 0;
      activity_time = 0;
   }
} ASN1T_ObjectAttributesDispersedMineArea;

class EXTERN ASN1C_ObjectAttributesDispersedMineArea : public ASN1CType {
public:
   ASN1T_ObjectAttributesDispersedMineArea& msgData;
   ASN1C_ObjectAttributesDispersedMineArea (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributesDispersedMineArea& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributesDispersedMineArea (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributesDispersedMineArea* pvalue);
EXTERN int asn1PD_ObjectAttributesDispersedMineArea (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributesDispersedMineArea* pvalue);

EXTERN void asn1Print_ObjectAttributesDispersedMineArea (ASN1ConstCharPtr name, ASN1T_ObjectAttributesDispersedMineArea* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributesMineJam                                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributesMineJam {
   ASN1INT   activity_time;

   ASN1T_ObjectAttributesMineJam () {
      activity_time = 0;
   }
} ASN1T_ObjectAttributesMineJam;

class EXTERN ASN1C_ObjectAttributesMineJam : public ASN1CType {
public:
   ASN1T_ObjectAttributesMineJam& msgData;
   ASN1C_ObjectAttributesMineJam (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributesMineJam& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributesMineJam (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributesMineJam* pvalue);
EXTERN int asn1PD_ObjectAttributesMineJam (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributesMineJam* pvalue);

EXTERN void asn1Print_ObjectAttributesMineJam (ASN1ConstCharPtr name, ASN1T_ObjectAttributesMineJam* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributesSpecific                                  */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_ObjectAttributesSpecific_crossing_site 1
#define T_ObjectAttributesSpecific_nbc_cloud 2
#define T_ObjectAttributesSpecific_rota 3
#define T_ObjectAttributesSpecific_nbc_zone 4
#define T_ObjectAttributesSpecific_logistic_route 5
#define T_ObjectAttributesSpecific_prisoner_camp 6
#define T_ObjectAttributesSpecific_refugee_camp 7
#define T_ObjectAttributesSpecific_linear_mine_area 8
#define T_ObjectAttributesSpecific_dispersed_mine_area 9
#define T_ObjectAttributesSpecific_mine_jam 10

typedef struct EXTERN ASN1T_ObjectAttributesSpecific {
   int t;
   union {
      /* t = 1 */
      ASN1T_ObjectAttributesCrossingSite *crossing_site;
      /* t = 2 */
      ASN1T_ObjectAttributesNbcCloud *nbc_cloud;
      /* t = 3 */
      ASN1T_ObjectAttributesRota *rota;
      /* t = 4 */
      ASN1T_ObjectAttributesNbcZone *nbc_zone;
      /* t = 5 */
      ASN1T_ObjectAttributesLogisticRoute *logistic_route;
      /* t = 6 */
      ASN1T_ObjectAttributesPrisonerCamp *prisoner_camp;
      /* t = 7 */
      ASN1T_ObjectAttributesRefugeeCamp *refugee_camp;
      /* t = 8 */
      ASN1T_ObjectAttributesLinearMineArea *linear_mine_area;
      /* t = 9 */
      ASN1T_ObjectAttributesDispersedMineArea *dispersed_mine_area;
      /* t = 10 */
      ASN1T_ObjectAttributesMineJam *mine_jam;
   } u;
} ASN1T_ObjectAttributesSpecific;

class EXTERN ASN1C_ObjectAttributesSpecific : public ASN1CType {
public:
   ASN1T_ObjectAttributesSpecific& msgData;
   ASN1C_ObjectAttributesSpecific (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributesSpecific& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributesSpecific (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributesSpecific* pvalue);
EXTERN int asn1PD_ObjectAttributesSpecific (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributesSpecific* pvalue);

EXTERN void asn1Print_ObjectAttributesSpecific (ASN1ConstCharPtr name, ASN1T_ObjectAttributesSpecific* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgObjectCreation                                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgObjectCreation {
   struct {
      unsigned obstacle_typePresent : 1;
      unsigned reserved_obstacle_activatedPresent : 1;
      unsigned specific_attributesPresent : 1;
      unsigned construction_dotation_typePresent : 1;
      unsigned mining_dotation_typePresent : 1;
   } m;
   ASN1T_OID  oid;
   ASN1T_EnumObjectType  type;
   ASN1T_EnumObstacleType  obstacle_type;
   ASN1BOOL  reserved_obstacle_activated;
   ASN1T_Location  location;
   ASN1VisibleString  name;
   ASN1T_Team  team;
   ASN1T_ObjectAttributesSpecific  specific_attributes;
   ASN1T_DotationType  construction_dotation_type;
   ASN1T_DotationType  mining_dotation_type;

   ASN1T_MsgObjectCreation () {
      m.obstacle_typePresent = 0;
      m.reserved_obstacle_activatedPresent = 0;
      m.specific_attributesPresent = 0;
      m.construction_dotation_typePresent = 0;
      m.mining_dotation_typePresent = 0;
   }
} ASN1T_MsgObjectCreation;

class EXTERN ASN1C_MsgObjectCreation : public ASN1CType {
public:
   ASN1T_MsgObjectCreation& msgData;
   ASN1C_MsgObjectCreation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgObjectCreation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgObjectCreation (ASN1CTXT* ctxt_p, ASN1T_MsgObjectCreation* pvalue);
EXTERN int asn1PD_MsgObjectCreation (ASN1CTXT* ctxt_p, ASN1T_MsgObjectCreation* pvalue);

EXTERN void asn1Print_MsgObjectCreation (ASN1ConstCharPtr name, ASN1T_MsgObjectCreation* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgObjectUpdate                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgObjectUpdate {
   struct {
      unsigned construction_percentagePresent : 1;
      unsigned mining_percentagePresent : 1;
      unsigned bypass_construction_percentagePresent : 1;
      unsigned reserved_obstacle_activatedPresent : 1;
      unsigned construction_dotation_nbrPresent : 1;
      unsigned mining_dotation_nbrPresent : 1;
      unsigned locationPresent : 1;
      unsigned specific_attributesPresent : 1;
   } m;
   ASN1T_OID  oid;
   ASN1INT   construction_percentage;
   ASN1INT   mining_percentage;
   ASN1INT   bypass_construction_percentage;
   ASN1BOOL  reserved_obstacle_activated;
   ASN1INT   construction_dotation_nbr;
   ASN1INT   mining_dotation_nbr;
   ASN1T_Location  location;
   ASN1T_ObjectAttributesSpecific  specific_attributes;

   ASN1T_MsgObjectUpdate () {
      m.construction_percentagePresent = 0;
      m.mining_percentagePresent = 0;
      m.bypass_construction_percentagePresent = 0;
      m.reserved_obstacle_activatedPresent = 0;
      m.construction_dotation_nbrPresent = 0;
      m.mining_dotation_nbrPresent = 0;
      m.locationPresent = 0;
      m.specific_attributesPresent = 0;
   }
} ASN1T_MsgObjectUpdate;

class EXTERN ASN1C_MsgObjectUpdate : public ASN1CType {
public:
   ASN1T_MsgObjectUpdate& msgData;
   ASN1C_MsgObjectUpdate (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgObjectUpdate& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgObjectUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgObjectUpdate* pvalue);
EXTERN int asn1PD_MsgObjectUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgObjectUpdate* pvalue);

EXTERN void asn1Print_MsgObjectUpdate (ASN1ConstCharPtr name, ASN1T_MsgObjectUpdate* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgObjectDestruction                                      */
/*                                                            */
/**************************************************************/

typedef ASN1T_OID ASN1T_MsgObjectDestruction;

class EXTERN ASN1C_MsgObjectDestruction : public ASN1CType {
public:
   ASN1T_MsgObjectDestruction& msgData;
   ASN1C_MsgObjectDestruction (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgObjectDestruction& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgObjectDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgObjectDestruction value);
EXTERN int asn1PD_MsgObjectDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgObjectDestruction* pvalue);

EXTERN void asn1Print_MsgObjectDestruction (ASN1ConstCharPtr name, ASN1T_MsgObjectDestruction* pvalue);

/**************************************************************/
/*                                                            */
/*  MagicActionUpdateObject                                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MagicActionUpdateObject {
   struct {
      unsigned construction_percentagePresent : 1;
      unsigned mining_percentagePresent : 1;
      unsigned bypass_construction_percentagePresent : 1;
      unsigned reserved_obstacle_activatedPresent : 1;
   } m;
   ASN1T_OID  oid;
   ASN1INT   construction_percentage;
   ASN1INT   mining_percentage;
   ASN1INT   bypass_construction_percentage;
   ASN1BOOL  reserved_obstacle_activated;

   ASN1T_MagicActionUpdateObject () {
      m.construction_percentagePresent = 0;
      m.mining_percentagePresent = 0;
      m.bypass_construction_percentagePresent = 0;
      m.reserved_obstacle_activatedPresent = 0;
   }
} ASN1T_MagicActionUpdateObject;

class EXTERN ASN1C_MagicActionUpdateObject : public ASN1CType {
public:
   ASN1T_MagicActionUpdateObject& msgData;
   ASN1C_MagicActionUpdateObject (
      ASN1MessageBuffer& msgBuf, ASN1T_MagicActionUpdateObject& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MagicActionUpdateObject (ASN1CTXT* ctxt_p, ASN1T_MagicActionUpdateObject* pvalue);
EXTERN int asn1PD_MagicActionUpdateObject (ASN1CTXT* ctxt_p, ASN1T_MagicActionUpdateObject* pvalue);

EXTERN void asn1Print_MagicActionUpdateObject (ASN1ConstCharPtr name, ASN1T_MagicActionUpdateObject* pvalue);

/**************************************************************/
/*                                                            */
/*  MagicActionCreateObject                                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MagicActionCreateObject {
   struct {
      unsigned obstacle_typePresent : 1;
      unsigned reserved_obstacle_activatedPresent : 1;
      unsigned namePresent : 1;
      unsigned specific_attributesPresent : 1;
   } m;
   ASN1T_Team  team;
   ASN1T_EnumObjectType  type;
   ASN1T_EnumObstacleType  obstacle_type;
   ASN1BOOL  reserved_obstacle_activated;
   ASN1T_Location  location;
   ASN1VisibleString  name;
   ASN1T_ObjectAttributesSpecific  specific_attributes;

   ASN1T_MagicActionCreateObject () {
      m.obstacle_typePresent = 0;
      m.reserved_obstacle_activatedPresent = 0;
      m.namePresent = 0;
      m.specific_attributesPresent = 0;
   }
} ASN1T_MagicActionCreateObject;

class EXTERN ASN1C_MagicActionCreateObject : public ASN1CType {
public:
   ASN1T_MagicActionCreateObject& msgData;
   ASN1C_MagicActionCreateObject (
      ASN1MessageBuffer& msgBuf, ASN1T_MagicActionCreateObject& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MagicActionCreateObject (ASN1CTXT* ctxt_p, ASN1T_MagicActionCreateObject* pvalue);
EXTERN int asn1PD_MagicActionCreateObject (ASN1CTXT* ctxt_p, ASN1T_MagicActionCreateObject* pvalue);

EXTERN void asn1Print_MagicActionCreateObject (ASN1ConstCharPtr name, ASN1T_MagicActionCreateObject* pvalue);

/**************************************************************/
/*                                                            */
/*  MagicActionDestroyObjet                                   */
/*                                                            */
/**************************************************************/

typedef ASN1T_OID ASN1T_MagicActionDestroyObjet;

class EXTERN ASN1C_MagicActionDestroyObjet : public ASN1CType {
public:
   ASN1T_MagicActionDestroyObjet& msgData;
   ASN1C_MagicActionDestroyObjet (
      ASN1MessageBuffer& msgBuf, ASN1T_MagicActionDestroyObjet& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MagicActionDestroyObjet (ASN1CTXT* ctxt_p, ASN1T_MagicActionDestroyObjet value);
EXTERN int asn1PD_MagicActionDestroyObjet (ASN1CTXT* ctxt_p, ASN1T_MagicActionDestroyObjet* pvalue);

EXTERN void asn1Print_MagicActionDestroyObjet (ASN1ConstCharPtr name, ASN1T_MagicActionDestroyObjet* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgObjectMagicAction_action                               */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_MsgObjectMagicAction_action_create_object 1
#define T_MsgObjectMagicAction_action_update_object 2
#define T_MsgObjectMagicAction_action_destroy_object 3

typedef struct EXTERN ASN1T_MsgObjectMagicAction_action {
   int t;
   union {
      /* t = 1 */
      ASN1T_MagicActionCreateObject *create_object;
      /* t = 2 */
      ASN1T_MagicActionUpdateObject *update_object;
      /* t = 3 */
      ASN1T_MagicActionDestroyObjet  destroy_object;
   } u;
} ASN1T_MsgObjectMagicAction_action;

class EXTERN ASN1C_MsgObjectMagicAction_action : public ASN1CType {
public:
   ASN1T_MsgObjectMagicAction_action& msgData;
   ASN1C_MsgObjectMagicAction_action (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgObjectMagicAction_action& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgObjectMagicAction_action (ASN1CTXT* ctxt_p, ASN1T_MsgObjectMagicAction_action* pvalue);
EXTERN int asn1PD_MsgObjectMagicAction_action (ASN1CTXT* ctxt_p, ASN1T_MsgObjectMagicAction_action* pvalue);

EXTERN void asn1Print_MsgObjectMagicAction_action (ASN1ConstCharPtr name, ASN1T_MsgObjectMagicAction_action* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgObjectMagicAction                                      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgObjectMagicAction {
   ASN1T_MsgObjectMagicAction_action  action;
} ASN1T_MsgObjectMagicAction;

class EXTERN ASN1C_MsgObjectMagicAction : public ASN1CType {
public:
   ASN1T_MsgObjectMagicAction& msgData;
   ASN1C_MsgObjectMagicAction (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgObjectMagicAction& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgObjectMagicAction (ASN1CTXT* ctxt_p, ASN1T_MsgObjectMagicAction* pvalue);
EXTERN int asn1PD_MsgObjectMagicAction (ASN1CTXT* ctxt_p, ASN1T_MsgObjectMagicAction* pvalue);

EXTERN void asn1Print_MsgObjectMagicAction (ASN1ConstCharPtr name, ASN1T_MsgObjectMagicAction* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgObjectMagicActionAck                                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgObjectMagicActionAck {
   ASN1T_EnumObjectErrorCode  error_code;
} ASN1T_MsgObjectMagicActionAck;

class EXTERN ASN1C_MsgObjectMagicActionAck : public ASN1CType {
public:
   ASN1T_MsgObjectMagicActionAck& msgData;
   ASN1C_MsgObjectMagicActionAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgObjectMagicActionAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgObjectMagicActionAck (ASN1CTXT* ctxt_p, ASN1T_MsgObjectMagicActionAck* pvalue);
EXTERN int asn1PD_MsgObjectMagicActionAck (ASN1CTXT* ctxt_p, ASN1T_MsgObjectMagicActionAck* pvalue);

EXTERN void asn1Print_MsgObjectMagicActionAck (ASN1ConstCharPtr name, ASN1T_MsgObjectMagicActionAck* pvalue);

#endif

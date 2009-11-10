/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 03-Nov-2009.
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
/*  EnumDemolitionTargetType                                  */
/*                                                            */
/**************************************************************/

struct EXTERN EnumDemolitionTargetType {
   enum Root {
      preliminary = 0,
      reserved = 1
   } ;
} ;

typedef EnumDemolitionTargetType::Root ASN1T_EnumDemolitionTargetType;

class EXTERN ASN1C_EnumDemolitionTargetType : public ASN1CType {
public:
   ASN1T_EnumDemolitionTargetType& msgData;
   ASN1C_EnumDemolitionTargetType (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumDemolitionTargetType& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumDemolitionTargetType (ASN1CTXT* ctxt_p, ASN1T_EnumDemolitionTargetType value);
EXTERN int asn1PD_EnumDemolitionTargetType (ASN1CTXT* ctxt_p, ASN1T_EnumDemolitionTargetType* pvalue);

EXTERN void asn1Print_EnumDemolitionTargetType (ASN1ConstCharPtr name, ASN1T_EnumDemolitionTargetType* pvalue);

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
/*  ObjectAttributeBypass                                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributeBypass {
   struct {
      unsigned percentagePresent : 1;
   } m;
   ASN1INT   percentage;

   ASN1T_ObjectAttributeBypass () {
      m.percentagePresent = 0;
   }
} ASN1T_ObjectAttributeBypass;

class EXTERN ASN1C_ObjectAttributeBypass : public ASN1CType {
public:
   ASN1T_ObjectAttributeBypass& msgData;
   ASN1C_ObjectAttributeBypass (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributeBypass& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributeBypass (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeBypass* pvalue);
EXTERN int asn1PD_ObjectAttributeBypass (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeBypass* pvalue);

EXTERN void asn1Print_ObjectAttributeBypass (ASN1ConstCharPtr name, ASN1T_ObjectAttributeBypass* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributeConstruction                               */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributeConstruction {
   struct {
      unsigned dotation_typePresent : 1;
      unsigned dotation_nbrPresent : 1;
      unsigned densityPresent : 1;
      unsigned percentagePresent : 1;
   } m;
   ASN1T_DotationType  dotation_type;
   ASN1INT   dotation_nbr;
   ASN1REAL  density;
   ASN1INT   percentage;

   ASN1T_ObjectAttributeConstruction () {
      m.dotation_typePresent = 0;
      m.dotation_nbrPresent = 0;
      m.densityPresent = 0;
      m.percentagePresent = 0;
   }
} ASN1T_ObjectAttributeConstruction;

class EXTERN ASN1C_ObjectAttributeConstruction : public ASN1CType {
public:
   ASN1T_ObjectAttributeConstruction& msgData;
   ASN1C_ObjectAttributeConstruction (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributeConstruction& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributeConstruction (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeConstruction* pvalue);
EXTERN int asn1PD_ObjectAttributeConstruction (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeConstruction* pvalue);

EXTERN void asn1Print_ObjectAttributeConstruction (ASN1ConstCharPtr name, ASN1T_ObjectAttributeConstruction* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributeLogistic                                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributeLogistic {
   ASN1T_Automat  tc2;
} ASN1T_ObjectAttributeLogistic;

class EXTERN ASN1C_ObjectAttributeLogistic : public ASN1CType {
public:
   ASN1T_ObjectAttributeLogistic& msgData;
   ASN1C_ObjectAttributeLogistic (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributeLogistic& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributeLogistic (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeLogistic* pvalue);
EXTERN int asn1PD_ObjectAttributeLogistic (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeLogistic* pvalue);

EXTERN void asn1Print_ObjectAttributeLogistic (ASN1ConstCharPtr name, ASN1T_ObjectAttributeLogistic* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributeInteractionHeight                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributeInteractionHeight {
   ASN1REAL  height;
} ASN1T_ObjectAttributeInteractionHeight;

class EXTERN ASN1C_ObjectAttributeInteractionHeight : public ASN1CType {
public:
   ASN1T_ObjectAttributeInteractionHeight& msgData;
   ASN1C_ObjectAttributeInteractionHeight (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributeInteractionHeight& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributeInteractionHeight (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeInteractionHeight* pvalue);
EXTERN int asn1PD_ObjectAttributeInteractionHeight (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeInteractionHeight* pvalue);

EXTERN void asn1Print_ObjectAttributeInteractionHeight (ASN1ConstCharPtr name, ASN1T_ObjectAttributeInteractionHeight* pvalue);

/**************************************************************/
/*                                                            */
/*  LocatedQuantity                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_LocatedQuantity {
   ASN1T_CoordLatLong  coordinate;
   ASN1REAL  quantity;
} ASN1T_LocatedQuantity;

class EXTERN ASN1C_LocatedQuantity : public ASN1CType {
public:
   ASN1T_LocatedQuantity& msgData;
   ASN1C_LocatedQuantity (
      ASN1MessageBuffer& msgBuf, ASN1T_LocatedQuantity& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_LocatedQuantity (ASN1CTXT* ctxt_p, ASN1T_LocatedQuantity* pvalue);
EXTERN int asn1PD_LocatedQuantity (ASN1CTXT* ctxt_p, ASN1T_LocatedQuantity* pvalue);

EXTERN void asn1Print_LocatedQuantity (ASN1ConstCharPtr name, ASN1T_LocatedQuantity* pvalue);

/**************************************************************/
/*                                                            */
/*  LocatedQuantityList                                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_LocatedQuantityList {
   ASN1UINT n;
   ASN1T_LocatedQuantity *elem;
} ASN1T_LocatedQuantityList;

class EXTERN ASN1C_LocatedQuantityList : public ASN1CType {
public:
   ASN1T_LocatedQuantityList& msgData;
   ASN1C_LocatedQuantityList (
      ASN1MessageBuffer& msgBuf, ASN1T_LocatedQuantityList& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_LocatedQuantityList (ASN1CTXT* ctxt_p, ASN1T_LocatedQuantityList* pvalue);
EXTERN int asn1PD_LocatedQuantityList (ASN1CTXT* ctxt_p, ASN1T_LocatedQuantityList* pvalue);

EXTERN void asn1Print_LocatedQuantityList (ASN1ConstCharPtr name, ASN1T_LocatedQuantityList* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributeToxicCloud                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributeToxicCloud {
   ASN1T_LocatedQuantityList  quantities;
} ASN1T_ObjectAttributeToxicCloud;

class EXTERN ASN1C_ObjectAttributeToxicCloud : public ASN1CType {
public:
   ASN1T_ObjectAttributeToxicCloud& msgData;
   ASN1C_ObjectAttributeToxicCloud (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributeToxicCloud& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributeToxicCloud (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeToxicCloud* pvalue);
EXTERN int asn1PD_ObjectAttributeToxicCloud (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeToxicCloud* pvalue);

EXTERN void asn1Print_ObjectAttributeToxicCloud (ASN1ConstCharPtr name, ASN1T_ObjectAttributeToxicCloud* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributeMine                                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributeMine {
   struct {
      unsigned dotation_typePresent : 1;
      unsigned dotation_nbrPresent : 1;
      unsigned densityPresent : 1;
      unsigned percentagePresent : 1;
   } m;
   ASN1T_DotationType  dotation_type;
   ASN1INT   dotation_nbr;
   ASN1REAL  density;
   ASN1INT   percentage;

   ASN1T_ObjectAttributeMine () {
      m.dotation_typePresent = 0;
      m.dotation_nbrPresent = 0;
      m.densityPresent = 0;
      m.percentagePresent = 0;
   }
} ASN1T_ObjectAttributeMine;

class EXTERN ASN1C_ObjectAttributeMine : public ASN1CType {
public:
   ASN1T_ObjectAttributeMine& msgData;
   ASN1C_ObjectAttributeMine (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributeMine& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributeMine (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeMine* pvalue);
EXTERN int asn1PD_ObjectAttributeMine (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeMine* pvalue);

EXTERN void asn1Print_ObjectAttributeMine (ASN1ConstCharPtr name, ASN1T_ObjectAttributeMine* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributeActivityTime                               */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributeActivityTime {
   ASN1INT   value;
} ASN1T_ObjectAttributeActivityTime;

class EXTERN ASN1C_ObjectAttributeActivityTime : public ASN1CType {
public:
   ASN1T_ObjectAttributeActivityTime& msgData;
   ASN1C_ObjectAttributeActivityTime (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributeActivityTime& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributeActivityTime (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeActivityTime* pvalue);
EXTERN int asn1PD_ObjectAttributeActivityTime (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeActivityTime* pvalue);

EXTERN void asn1Print_ObjectAttributeActivityTime (ASN1ConstCharPtr name, ASN1T_ObjectAttributeActivityTime* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributeNBC                                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributeNBC {
   ASN1INT   danger_level;
   ASN1T_ListOID  nbc_agents;
} ASN1T_ObjectAttributeNBC;

class EXTERN ASN1C_ObjectAttributeNBC : public ASN1CType {
public:
   ASN1T_ObjectAttributeNBC& msgData;
   ASN1C_ObjectAttributeNBC (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributeNBC& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributeNBC (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeNBC* pvalue);
EXTERN int asn1PD_ObjectAttributeNBC (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeNBC* pvalue);

EXTERN void asn1Print_ObjectAttributeNBC (ASN1ConstCharPtr name, ASN1T_ObjectAttributeNBC* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributeObstacle                                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributeObstacle {
   ASN1T_EnumDemolitionTargetType  type;
   ASN1BOOL  activated;
} ASN1T_ObjectAttributeObstacle;

class EXTERN ASN1C_ObjectAttributeObstacle : public ASN1CType {
public:
   ASN1T_ObjectAttributeObstacle& msgData;
   ASN1C_ObjectAttributeObstacle (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributeObstacle& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributeObstacle (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeObstacle* pvalue);
EXTERN int asn1PD_ObjectAttributeObstacle (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeObstacle* pvalue);

EXTERN void asn1Print_ObjectAttributeObstacle (ASN1ConstCharPtr name, ASN1T_ObjectAttributeObstacle* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributeCrossingSite                               */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributeCrossingSite {
   ASN1INT   width;
   ASN1INT   depth;
   ASN1INT   flow_rate;
   ASN1BOOL  banks_require_fitting;
} ASN1T_ObjectAttributeCrossingSite;

class EXTERN ASN1C_ObjectAttributeCrossingSite : public ASN1CType {
public:
   ASN1T_ObjectAttributeCrossingSite& msgData;
   ASN1C_ObjectAttributeCrossingSite (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributeCrossingSite& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributeCrossingSite (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeCrossingSite* pvalue);
EXTERN int asn1PD_ObjectAttributeCrossingSite (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeCrossingSite* pvalue);

EXTERN void asn1Print_ObjectAttributeCrossingSite (ASN1ConstCharPtr name, ASN1T_ObjectAttributeCrossingSite* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributeSupplyRoute                                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributeSupplyRoute {
   ASN1BOOL  equipped;
   ASN1INT   max_weight;
   ASN1INT   width;
   ASN1INT   length;
   ASN1INT   flow_rate;
} ASN1T_ObjectAttributeSupplyRoute;

class EXTERN ASN1C_ObjectAttributeSupplyRoute : public ASN1CType {
public:
   ASN1T_ObjectAttributeSupplyRoute& msgData;
   ASN1C_ObjectAttributeSupplyRoute (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributeSupplyRoute& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributeSupplyRoute (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeSupplyRoute* pvalue);
EXTERN int asn1PD_ObjectAttributeSupplyRoute (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeSupplyRoute* pvalue);

EXTERN void asn1Print_ObjectAttributeSupplyRoute (ASN1ConstCharPtr name, ASN1T_ObjectAttributeSupplyRoute* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributeFire                                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributeFire {
   ASN1T_OID  class_id;
   ASN1INT   heat;
} ASN1T_ObjectAttributeFire;

class EXTERN ASN1C_ObjectAttributeFire : public ASN1CType {
public:
   ASN1T_ObjectAttributeFire& msgData;
   ASN1C_ObjectAttributeFire (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributeFire& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributeFire (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeFire* pvalue);
EXTERN int asn1PD_ObjectAttributeFire (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeFire* pvalue);

EXTERN void asn1Print_ObjectAttributeFire (ASN1ConstCharPtr name, ASN1T_ObjectAttributeFire* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributeNBCType                                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributeNBCType {
   ASN1T_OID  agent_id;
   ASN1INT   concentration;
   ASN1INT   source_life_duration;
} ASN1T_ObjectAttributeNBCType;

class EXTERN ASN1C_ObjectAttributeNBCType : public ASN1CType {
public:
   ASN1T_ObjectAttributeNBCType& msgData;
   ASN1C_ObjectAttributeNBCType (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributeNBCType& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributeNBCType (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeNBCType* pvalue);
EXTERN int asn1PD_ObjectAttributeNBCType (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeNBCType* pvalue);

EXTERN void asn1Print_ObjectAttributeNBCType (ASN1ConstCharPtr name, ASN1T_ObjectAttributeNBCType* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributeMedicalTreatment                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributeMedicalTreatment {
   ASN1T_ListOID  type_id;
   ASN1INT   beds;
   ASN1INT   available_beds;
   ASN1INT   doctors;
   ASN1INT   available_doctors;
} ASN1T_ObjectAttributeMedicalTreatment;

class EXTERN ASN1C_ObjectAttributeMedicalTreatment : public ASN1CType {
public:
   ASN1T_ObjectAttributeMedicalTreatment& msgData;
   ASN1C_ObjectAttributeMedicalTreatment (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributeMedicalTreatment& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributeMedicalTreatment (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeMedicalTreatment* pvalue);
EXTERN int asn1PD_ObjectAttributeMedicalTreatment (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeMedicalTreatment* pvalue);

EXTERN void asn1Print_ObjectAttributeMedicalTreatment (ASN1ConstCharPtr name, ASN1T_ObjectAttributeMedicalTreatment* pvalue);

/**************************************************************/
/*                                                            */
/*  StockResource                                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_StockResource {
   struct {
      unsigned maxPresent : 1;
   } m;
   ASN1T_DotationType  dotation_type;
   ASN1INT   current;
   ASN1INT   max;

   ASN1T_StockResource () {
      m.maxPresent = 0;
   }
} ASN1T_StockResource;

class EXTERN ASN1C_StockResource : public ASN1CType {
public:
   ASN1T_StockResource& msgData;
   ASN1C_StockResource (
      ASN1MessageBuffer& msgBuf, ASN1T_StockResource& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_StockResource (ASN1CTXT* ctxt_p, ASN1T_StockResource* pvalue);
EXTERN int asn1PD_StockResource (ASN1CTXT* ctxt_p, ASN1T_StockResource* pvalue);

EXTERN void asn1Print_StockResource (ASN1ConstCharPtr name, ASN1T_StockResource* pvalue);

/**************************************************************/
/*                                                            */
/*  StockResourceList                                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_StockResourceList {
   ASN1UINT n;
   ASN1T_StockResource *elem;
} ASN1T_StockResourceList;

class EXTERN ASN1C_StockResourceList : public ASN1CType {
public:
   ASN1T_StockResourceList& msgData;
   ASN1C_StockResourceList (
      ASN1MessageBuffer& msgBuf, ASN1T_StockResourceList& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_StockResourceList (ASN1CTXT* ctxt_p, ASN1T_StockResourceList* pvalue);
EXTERN int asn1PD_StockResourceList (ASN1CTXT* ctxt_p, ASN1T_StockResourceList* pvalue);

EXTERN void asn1Print_StockResourceList (ASN1ConstCharPtr name, ASN1T_StockResourceList* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributeStock                                      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributeStock {
   ASN1T_StockResourceList  resources;
} ASN1T_ObjectAttributeStock;

class EXTERN ASN1C_ObjectAttributeStock : public ASN1CType {
public:
   ASN1T_ObjectAttributeStock& msgData;
   ASN1C_ObjectAttributeStock (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributeStock& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributeStock (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeStock* pvalue);
EXTERN int asn1PD_ObjectAttributeStock (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeStock* pvalue);

EXTERN void asn1Print_ObjectAttributeStock (ASN1ConstCharPtr name, ASN1T_ObjectAttributeStock* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributes                                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributes {
   struct {
      unsigned constructionPresent : 1;
      unsigned obstaclePresent : 1;
      unsigned minePresent : 1;
      unsigned activity_timePresent : 1;
      unsigned bypassPresent : 1;
      unsigned logisticPresent : 1;
      unsigned nbcPresent : 1;
      unsigned crossing_sitePresent : 1;
      unsigned supply_routePresent : 1;
      unsigned toxic_cloudPresent : 1;
      unsigned firePresent : 1;
      unsigned medical_treatmentPresent : 1;
      unsigned interaction_heightPresent : 1;
      unsigned nbc_agentPresent : 1;
      unsigned stockPresent : 1;
   } m;
   ASN1T_ObjectAttributeConstruction  construction;
   ASN1T_ObjectAttributeObstacle  obstacle;
   ASN1T_ObjectAttributeMine  mine;
   ASN1T_ObjectAttributeActivityTime  activity_time;
   ASN1T_ObjectAttributeBypass  bypass;
   ASN1T_ObjectAttributeLogistic  logistic;
   ASN1T_ObjectAttributeNBC  nbc;
   ASN1T_ObjectAttributeCrossingSite  crossing_site;
   ASN1T_ObjectAttributeSupplyRoute  supply_route;
   ASN1T_ObjectAttributeToxicCloud  toxic_cloud;
   ASN1T_ObjectAttributeFire  fire;
   ASN1T_ObjectAttributeMedicalTreatment  medical_treatment;
   ASN1T_ObjectAttributeInteractionHeight  interaction_height;
   ASN1T_ObjectAttributeNBCType  nbc_agent;
   ASN1T_ObjectAttributeStock  stock;

   ASN1T_ObjectAttributes () {
      m.constructionPresent = 0;
      m.obstaclePresent = 0;
      m.minePresent = 0;
      m.activity_timePresent = 0;
      m.bypassPresent = 0;
      m.logisticPresent = 0;
      m.nbcPresent = 0;
      m.crossing_sitePresent = 0;
      m.supply_routePresent = 0;
      m.toxic_cloudPresent = 0;
      m.firePresent = 0;
      m.medical_treatmentPresent = 0;
      m.interaction_heightPresent = 0;
      m.nbc_agentPresent = 0;
      m.stockPresent = 0;
   }
} ASN1T_ObjectAttributes;

class EXTERN ASN1C_ObjectAttributes : public ASN1CType {
public:
   ASN1T_ObjectAttributes& msgData;
   ASN1C_ObjectAttributes (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributes& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributes (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributes* pvalue);
EXTERN int asn1PD_ObjectAttributes (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributes* pvalue);

EXTERN void asn1Print_ObjectAttributes (ASN1ConstCharPtr name, ASN1T_ObjectAttributes* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgObjectCreation                                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgObjectCreation {
   ASN1T_OID  oid;
   ASN1VisibleString  type;
   ASN1VisibleString  name;
   ASN1T_Team  team;
   ASN1T_Location  location;
   ASN1T_ObjectAttributes  attributes;
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
      unsigned locationPresent : 1;
   } m;
   ASN1T_OID  oid;
   ASN1T_Location  location;
   ASN1T_ObjectAttributes  attributes;

   ASN1T_MsgObjectUpdate () {
      m.locationPresent = 0;
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
   ASN1T_OID  oid;
   ASN1T_ObjectAttributes  attributes;
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
   ASN1VisibleString  type;
   ASN1T_Location  location;
   ASN1VisibleString  name;
   ASN1T_Team  team;
   ASN1T_ObjectAttributes  attributes;
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

/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 31-Jan-2007.
 */
#ifndef ORDER_H
#define ORDER_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "Base.h"
#include "LimaLimite.h"
#include "LogMaintenance.h"
#include "LogSante.h"
#include "Object.h"

/**************************************************************/
/*                                                            */
/*  OrderIncludes                                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_OrderIncludes {
} ASN1T_OrderIncludes;

class EXTERN ASN1C_OrderIncludes : public ASN1CType {
public:
   ASN1T_OrderIncludes& msgData;
   ASN1C_OrderIncludes (
      ASN1MessageBuffer& msgBuf, ASN1T_OrderIncludes& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_OrderIncludes (ASN1CTXT* ctxt_p, ASN1T_OrderIncludes* pvalue);
EXTERN int asn1PD_OrderIncludes (ASN1CTXT* ctxt_p, ASN1T_OrderIncludes* pvalue);

EXTERN void asn1Print_OrderIncludes (ASN1ConstCharPtr name, ASN1T_OrderIncludes* pvalue);

/**************************************************************/
/*                                                            */
/*  OrderExports                                              */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_OrderExports : public ASN1CType {
public:
   ASN1C_OrderExports (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_OrderExports (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_OrderExports (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_OrderExports (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  EnumOrderErrorCode                                        */
/*                                                            */
/**************************************************************/

struct EXTERN EnumOrderErrorCode {
   enum Root {
      no_error = 0,
      error_invalid_unit = 1,
      error_invalid_limit = 2,
      error_invalid_lima = 3,
      error_invalid_mission = 4,
      error_invalid_mission_parameters = 5,
      error_unit_cannot_receive_order = 6,
      error_invalid_order_conduite = 7,
      error_invalid_order_mission = 8,
      error_invalid_order_initial = 9,
      error_invalid_order_conduite_parameters = 10,
      error_unit_surrendered = 11,
      error_invalid_lima_function = 12
   } ;
} ;

typedef EnumOrderErrorCode::Root ASN1T_EnumOrderErrorCode;

class EXTERN ASN1C_EnumOrderErrorCode : public ASN1CType {
public:
   ASN1T_EnumOrderErrorCode& msgData;
   ASN1C_EnumOrderErrorCode (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumOrderErrorCode& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumOrderErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumOrderErrorCode value);
EXTERN int asn1PD_EnumOrderErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumOrderErrorCode* pvalue);

EXTERN void asn1Print_EnumOrderErrorCode (ASN1ConstCharPtr name, ASN1T_EnumOrderErrorCode* pvalue);

/**************************************************************/
/*                                                            */
/*  EnumOrderState                                            */
/*                                                            */
/**************************************************************/

struct EXTERN EnumOrderState {
   enum Root {
      started = 0,
      stopped = 1
   } ;
} ;

typedef EnumOrderState::Root ASN1T_EnumOrderState;

class EXTERN ASN1C_EnumOrderState : public ASN1CType {
public:
   ASN1T_EnumOrderState& msgData;
   ASN1C_EnumOrderState (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumOrderState& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumOrderState (ASN1CTXT* ctxt_p, ASN1T_EnumOrderState value);
EXTERN int asn1PD_EnumOrderState (ASN1CTXT* ctxt_p, ASN1T_EnumOrderState* pvalue);

EXTERN void asn1Print_EnumOrderState (ASN1ConstCharPtr name, ASN1T_EnumOrderState* pvalue);

/**************************************************************/
/*                                                            */
/*  OrderContext                                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_OrderContext {
   struct {
      unsigned limite_gauchePresent : 1;
      unsigned limite_droitePresent : 1;
   } m;
   ASN1T_Line  limite_gauche;
   ASN1T_Line  limite_droite;
   ASN1T_LimasOrder  limas;
   ASN1T_Direction  direction_dangereuse;

   ASN1T_OrderContext () {
      m.limite_gauchePresent = 0;
      m.limite_droitePresent = 0;
   }
} ASN1T_OrderContext;

class EXTERN ASN1C_OrderContext : public ASN1CType {
public:
   ASN1T_OrderContext& msgData;
   ASN1C_OrderContext (
      ASN1MessageBuffer& msgBuf, ASN1T_OrderContext& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_OrderContext (ASN1CTXT* ctxt_p, ASN1T_OrderContext* pvalue);
EXTERN int asn1PD_OrderContext (ASN1CTXT* ctxt_p, ASN1T_OrderContext* pvalue);

EXTERN void asn1Print_OrderContext (ASN1ConstCharPtr name, ASN1T_OrderContext* pvalue);

/**************************************************************/
/*                                                            */
/*  MissionGenObject                                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MissionGenObject {
   ASN1T_EnumObjectType  type;
   ASN1T_Localisation  position;
   ASN1T_OID  oid_planifie;
   ASN1T_EnumMissionGenSousTypeObstacle  preliminaire;
   ASN1REAL  densite;
   ASN1T_Automate  tc2;

   ASN1T_MissionGenObject () {
      preliminaire = EnumMissionGenSousTypeObstacle::preliminaire;
      densite = 0;
      tc2 = 0;
   }
} ASN1T_MissionGenObject;

class EXTERN ASN1C_MissionGenObject : public ASN1CType {
public:
   ASN1T_MissionGenObject& msgData;
   ASN1C_MissionGenObject (
      ASN1MessageBuffer& msgBuf, ASN1T_MissionGenObject& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MissionGenObject (ASN1CTXT* ctxt_p, ASN1T_MissionGenObject* pvalue);
EXTERN int asn1PD_MissionGenObject (ASN1CTXT* ctxt_p, ASN1T_MissionGenObject* pvalue);

EXTERN void asn1Print_MissionGenObject (ASN1ConstCharPtr name, ASN1T_MissionGenObject* pvalue);

/**************************************************************/
/*                                                            */
/*  ListMissionGenObject                                      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ListMissionGenObject {
   ASN1UINT n;
   ASN1T_MissionGenObject *elem;
} ASN1T_ListMissionGenObject;

class EXTERN ASN1C_ListMissionGenObject : public ASN1CType {
public:
   ASN1T_ListMissionGenObject& msgData;
   ASN1C_ListMissionGenObject (
      ASN1MessageBuffer& msgBuf, ASN1T_ListMissionGenObject& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ListMissionGenObject (ASN1CTXT* ctxt_p, ASN1T_ListMissionGenObject* pvalue);
EXTERN int asn1PD_ListMissionGenObject (ASN1CTXT* ctxt_p, ASN1T_ListMissionGenObject* pvalue);

EXTERN void asn1Print_ListMissionGenObject (ASN1ConstCharPtr name, ASN1T_ListMissionGenObject* pvalue);

/**************************************************************/
/*                                                            */
/*  MissionParameter_value                                    */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_MissionParameter_value_aBool  1
#define T_MissionParameter_value_aReal  2
#define T_MissionParameter_value_enumeration 3
#define T_MissionParameter_value_itineraire 4
#define T_MissionParameter_value_listItineraire 5
#define T_MissionParameter_value_point  6
#define T_MissionParameter_value_listPoint 7
#define T_MissionParameter_value_polygon 8
#define T_MissionParameter_value_listPolygon 9
#define T_MissionParameter_value_localisation 10
#define T_MissionParameter_value_listLocalisation 11
#define T_MissionParameter_value_direction 12
#define T_MissionParameter_value_natureAtlas 13
#define T_MissionParameter_value_agent  14
#define T_MissionParameter_value_listAgent 15
#define T_MissionParameter_value_automate 16
#define T_MissionParameter_value_listAutomate 17
#define T_MissionParameter_value_knowledgeAgent 18
#define T_MissionParameter_value_listKnowledgeAgent 19
#define T_MissionParameter_value_knowledgeObject 20
#define T_MissionParameter_value_listKnowledgeObject 21
#define T_MissionParameter_value_knowledgePopulation 22
#define T_MissionParameter_value_missionGenObject 23
#define T_MissionParameter_value_listMissionGenObject 24
#define T_MissionParameter_value_typeDotation 25
#define T_MissionParameter_value_typeEquipement 26
#define T_MissionParameter_value_tirIndirect 27
#define T_MissionParameter_value_gDH    28
#define T_MissionParameter_value_maintenancePriorites 29
#define T_MissionParameter_value_santePriorites 30
#define T_MissionParameter_value_aCharStr 31

typedef struct EXTERN ASN1T_MissionParameter_value {
   int t;
   union {
      /* t = 1 */
      ASN1BOOL  aBool;
      /* t = 2 */
      ASN1REAL  aReal;
      /* t = 3 */
      ASN1INT   enumeration;
      /* t = 4 */
      ASN1T_Itineraire *itineraire;
      /* t = 5 */
      ASN1T_ListItineraire *listItineraire;
      /* t = 6 */
      ASN1T_Point *point;
      /* t = 7 */
      ASN1T_ListPoint *listPoint;
      /* t = 8 */
      ASN1T_Polygon *polygon;
      /* t = 9 */
      ASN1T_ListPolygon *listPolygon;
      /* t = 10 */
      ASN1T_Localisation *localisation;
      /* t = 11 */
      ASN1T_ListLocalisation *listLocalisation;
      /* t = 12 */
      ASN1T_Direction  direction;
      /* t = 13 */
      ASN1T_NatureAtlas *natureAtlas;
      /* t = 14 */
      ASN1T_Agent  agent;
      /* t = 15 */
      ASN1T_ListAgent *listAgent;
      /* t = 16 */
      ASN1T_Automate  automate;
      /* t = 17 */
      ASN1T_ListAutomate *listAutomate;
      /* t = 18 */
      ASN1T_KnowledgeAgent  knowledgeAgent;
      /* t = 19 */
      ASN1T_ListKnowledgeAgent *listKnowledgeAgent;
      /* t = 20 */
      ASN1T_KnowledgeObject  knowledgeObject;
      /* t = 21 */
      ASN1T_ListKnowledgeObject *listKnowledgeObject;
      /* t = 22 */
      ASN1T_KnowledgePopulation  knowledgePopulation;
      /* t = 23 */
      ASN1T_MissionGenObject *missionGenObject;
      /* t = 24 */
      ASN1T_ListMissionGenObject *listMissionGenObject;
      /* t = 25 */
      ASN1T_TypeDotation  typeDotation;
      /* t = 26 */
      ASN1T_TypeEquipement  typeEquipement;
      /* t = 27 */
      ASN1T_TirPion  tirIndirect;
      /* t = 28 */
      ASN1T_GDH *gDH;
      /* t = 29 */
      ASN1T_MaintenancePriorites *maintenancePriorites;
      /* t = 30 */
      ASN1T_SantePriorites *santePriorites;
      /* t = 31 */
      ASN1VisibleString  aCharStr;
   } u;
} ASN1T_MissionParameter_value;

class EXTERN ASN1C_MissionParameter_value : public ASN1CType {
public:
   ASN1T_MissionParameter_value& msgData;
   ASN1C_MissionParameter_value (
      ASN1MessageBuffer& msgBuf, ASN1T_MissionParameter_value& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MissionParameter_value (ASN1CTXT* ctxt_p, ASN1T_MissionParameter_value* pvalue);
EXTERN int asn1PD_MissionParameter_value (ASN1CTXT* ctxt_p, ASN1T_MissionParameter_value* pvalue);

EXTERN void asn1Print_MissionParameter_value (ASN1ConstCharPtr name, ASN1T_MissionParameter_value* pvalue);

/**************************************************************/
/*                                                            */
/*  MissionParameter                                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MissionParameter {
   ASN1BOOL  null_value;
   ASN1T_MissionParameter_value  value;

   ASN1T_MissionParameter () {
      null_value = FALSE;
   }
} ASN1T_MissionParameter;

class EXTERN ASN1C_MissionParameter : public ASN1CType {
public:
   ASN1T_MissionParameter& msgData;
   ASN1C_MissionParameter (
      ASN1MessageBuffer& msgBuf, ASN1T_MissionParameter& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MissionParameter (ASN1CTXT* ctxt_p, ASN1T_MissionParameter* pvalue);
EXTERN int asn1PD_MissionParameter (ASN1CTXT* ctxt_p, ASN1T_MissionParameter* pvalue);

EXTERN void asn1Print_MissionParameter (ASN1ConstCharPtr name, ASN1T_MissionParameter* pvalue);

/**************************************************************/
/*                                                            */
/*  MissionParameters                                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MissionParameters {
   ASN1UINT n;
   ASN1T_MissionParameter *elem;
} ASN1T_MissionParameters;

class EXTERN ASN1C_MissionParameters : public ASN1CType {
public:
   ASN1T_MissionParameters& msgData;
   ASN1C_MissionParameters (
      ASN1MessageBuffer& msgBuf, ASN1T_MissionParameters& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MissionParameters (ASN1CTXT* ctxt_p, ASN1T_MissionParameters* pvalue);
EXTERN int asn1PD_MissionParameters (ASN1CTXT* ctxt_p, ASN1T_MissionParameters* pvalue);

EXTERN void asn1Print_MissionParameters (ASN1ConstCharPtr name, ASN1T_MissionParameters* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPionOrder                                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPionOrder {
   ASN1T_Agent  oid_unite_executante;
   ASN1T_OrderContext  order_context;
   ASN1T_OID  mission;
   ASN1T_MissionParameters  parametres;
} ASN1T_MsgPionOrder;

class EXTERN ASN1C_MsgPionOrder : public ASN1CType {
public:
   ASN1T_MsgPionOrder& msgData;
   ASN1C_MsgPionOrder (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPionOrder& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPionOrder (ASN1CTXT* ctxt_p, ASN1T_MsgPionOrder* pvalue);
EXTERN int asn1PD_MsgPionOrder (ASN1CTXT* ctxt_p, ASN1T_MsgPionOrder* pvalue);

EXTERN void asn1Print_MsgPionOrder (ASN1ConstCharPtr name, ASN1T_MsgPionOrder* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPionOrderAck                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPionOrderAck {
   ASN1T_Agent  oid_unite_executante;
   ASN1T_EnumOrderErrorCode  error_code;
} ASN1T_MsgPionOrderAck;

class EXTERN ASN1C_MsgPionOrderAck : public ASN1CType {
public:
   ASN1T_MsgPionOrderAck& msgData;
   ASN1C_MsgPionOrderAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPionOrderAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPionOrderAck (ASN1CTXT* ctxt_p, ASN1T_MsgPionOrderAck* pvalue);
EXTERN int asn1PD_MsgPionOrderAck (ASN1CTXT* ctxt_p, ASN1T_MsgPionOrderAck* pvalue);

EXTERN void asn1Print_MsgPionOrderAck (ASN1ConstCharPtr name, ASN1T_MsgPionOrderAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPionOrderManagement                                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPionOrderManagement {
   ASN1T_Agent  oid_unite_executante;
   ASN1T_EnumOrderState  etat;
} ASN1T_MsgPionOrderManagement;

class EXTERN ASN1C_MsgPionOrderManagement : public ASN1CType {
public:
   ASN1T_MsgPionOrderManagement& msgData;
   ASN1C_MsgPionOrderManagement (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPionOrderManagement& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPionOrderManagement (ASN1CTXT* ctxt_p, ASN1T_MsgPionOrderManagement* pvalue);
EXTERN int asn1PD_MsgPionOrderManagement (ASN1CTXT* ctxt_p, ASN1T_MsgPionOrderManagement* pvalue);

EXTERN void asn1Print_MsgPionOrderManagement (ASN1ConstCharPtr name, ASN1T_MsgPionOrderManagement* pvalue);

/**************************************************************/
/*                                                            */
/*  EnumAutomateOrderFormation                                */
/*                                                            */
/**************************************************************/

struct EXTERN EnumAutomateOrderFormation {
   enum Root {
      un_echelon = 0,
      deux_echelons = 1
   } ;
} ;

typedef EnumAutomateOrderFormation::Root ASN1T_EnumAutomateOrderFormation;

class EXTERN ASN1C_EnumAutomateOrderFormation : public ASN1CType {
public:
   ASN1T_EnumAutomateOrderFormation& msgData;
   ASN1C_EnumAutomateOrderFormation (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumAutomateOrderFormation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumAutomateOrderFormation (ASN1CTXT* ctxt_p, ASN1T_EnumAutomateOrderFormation value);
EXTERN int asn1PD_EnumAutomateOrderFormation (ASN1CTXT* ctxt_p, ASN1T_EnumAutomateOrderFormation* pvalue);

EXTERN void asn1Print_EnumAutomateOrderFormation (ASN1ConstCharPtr name, ASN1T_EnumAutomateOrderFormation* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgAutomateOrder                                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgAutomateOrder {
   ASN1T_Automate  oid_unite_executante;
   ASN1T_OrderContext  order_context;
   ASN1T_OID  mission;
   ASN1T_MissionParameters  parametres;
   ASN1T_EnumAutomateOrderFormation  formation;
} ASN1T_MsgAutomateOrder;

class EXTERN ASN1C_MsgAutomateOrder : public ASN1CType {
public:
   ASN1T_MsgAutomateOrder& msgData;
   ASN1C_MsgAutomateOrder (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgAutomateOrder& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgAutomateOrder (ASN1CTXT* ctxt_p, ASN1T_MsgAutomateOrder* pvalue);
EXTERN int asn1PD_MsgAutomateOrder (ASN1CTXT* ctxt_p, ASN1T_MsgAutomateOrder* pvalue);

EXTERN void asn1Print_MsgAutomateOrder (ASN1ConstCharPtr name, ASN1T_MsgAutomateOrder* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgAutomateOrderAck                                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgAutomateOrderAck {
   ASN1T_Agent  oid_unite_executante;
   ASN1T_EnumOrderErrorCode  error_code;
} ASN1T_MsgAutomateOrderAck;

class EXTERN ASN1C_MsgAutomateOrderAck : public ASN1CType {
public:
   ASN1T_MsgAutomateOrderAck& msgData;
   ASN1C_MsgAutomateOrderAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgAutomateOrderAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgAutomateOrderAck (ASN1CTXT* ctxt_p, ASN1T_MsgAutomateOrderAck* pvalue);
EXTERN int asn1PD_MsgAutomateOrderAck (ASN1CTXT* ctxt_p, ASN1T_MsgAutomateOrderAck* pvalue);

EXTERN void asn1Print_MsgAutomateOrderAck (ASN1ConstCharPtr name, ASN1T_MsgAutomateOrderAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgAutomateOrderManagement                                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgAutomateOrderManagement {
   ASN1T_Automate  oid_unite_executante;
   ASN1T_EnumOrderState  etat;
} ASN1T_MsgAutomateOrderManagement;

class EXTERN ASN1C_MsgAutomateOrderManagement : public ASN1CType {
public:
   ASN1T_MsgAutomateOrderManagement& msgData;
   ASN1C_MsgAutomateOrderManagement (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgAutomateOrderManagement& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgAutomateOrderManagement (ASN1CTXT* ctxt_p, ASN1T_MsgAutomateOrderManagement* pvalue);
EXTERN int asn1PD_MsgAutomateOrderManagement (ASN1CTXT* ctxt_p, ASN1T_MsgAutomateOrderManagement* pvalue);

EXTERN void asn1Print_MsgAutomateOrderManagement (ASN1ConstCharPtr name, ASN1T_MsgAutomateOrderManagement* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPopulationOrder                                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationOrder {
   ASN1T_Population  oid_unite_executante;
   ASN1T_OID  mission;
   ASN1T_MissionParameters  parametres;
} ASN1T_MsgPopulationOrder;

class EXTERN ASN1C_MsgPopulationOrder : public ASN1CType {
public:
   ASN1T_MsgPopulationOrder& msgData;
   ASN1C_MsgPopulationOrder (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationOrder& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPopulationOrder (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationOrder* pvalue);
EXTERN int asn1PD_MsgPopulationOrder (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationOrder* pvalue);

EXTERN void asn1Print_MsgPopulationOrder (ASN1ConstCharPtr name, ASN1T_MsgPopulationOrder* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPopulationOrderAck                                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationOrderAck {
   ASN1T_Population  oid_unite_executante;
   ASN1T_EnumOrderErrorCode  error_code;
} ASN1T_MsgPopulationOrderAck;

class EXTERN ASN1C_MsgPopulationOrderAck : public ASN1CType {
public:
   ASN1T_MsgPopulationOrderAck& msgData;
   ASN1C_MsgPopulationOrderAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationOrderAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPopulationOrderAck (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationOrderAck* pvalue);
EXTERN int asn1PD_MsgPopulationOrderAck (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationOrderAck* pvalue);

EXTERN void asn1Print_MsgPopulationOrderAck (ASN1ConstCharPtr name, ASN1T_MsgPopulationOrderAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPopulationOrderManagement                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationOrderManagement {
   ASN1T_Population  oid_unite_executante;
   ASN1T_EnumOrderState  etat;
} ASN1T_MsgPopulationOrderManagement;

class EXTERN ASN1C_MsgPopulationOrderManagement : public ASN1CType {
public:
   ASN1T_MsgPopulationOrderManagement& msgData;
   ASN1C_MsgPopulationOrderManagement (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationOrderManagement& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPopulationOrderManagement (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationOrderManagement* pvalue);
EXTERN int asn1PD_MsgPopulationOrderManagement (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationOrderManagement* pvalue);

EXTERN void asn1Print_MsgPopulationOrderManagement (ASN1ConstCharPtr name, ASN1T_MsgPopulationOrderManagement* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgFragOrder                                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgFragOrder {
   ASN1T_OID  oid_unite_executante;
   ASN1T_OID  frag_order;
   ASN1T_MissionParameters  parametres;
} ASN1T_MsgFragOrder;

class EXTERN ASN1C_MsgFragOrder : public ASN1CType {
public:
   ASN1T_MsgFragOrder& msgData;
   ASN1C_MsgFragOrder (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgFragOrder& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgFragOrder (ASN1CTXT* ctxt_p, ASN1T_MsgFragOrder* pvalue);
EXTERN int asn1PD_MsgFragOrder (ASN1CTXT* ctxt_p, ASN1T_MsgFragOrder* pvalue);

EXTERN void asn1Print_MsgFragOrder (ASN1ConstCharPtr name, ASN1T_MsgFragOrder* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgFragOrderAck                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgFragOrderAck {
   ASN1T_OID  oid_unite_executante;
   ASN1T_EnumOrderErrorCode  error_code;
} ASN1T_MsgFragOrderAck;

class EXTERN ASN1C_MsgFragOrderAck : public ASN1CType {
public:
   ASN1T_MsgFragOrderAck& msgData;
   ASN1C_MsgFragOrderAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgFragOrderAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgFragOrderAck (ASN1CTXT* ctxt_p, ASN1T_MsgFragOrderAck* pvalue);
EXTERN int asn1PD_MsgFragOrderAck (ASN1CTXT* ctxt_p, ASN1T_MsgFragOrderAck* pvalue);

EXTERN void asn1Print_MsgFragOrderAck (ASN1ConstCharPtr name, ASN1T_MsgFragOrderAck* pvalue);

/**************************************************************/
/*                                                            */
/*  EnumTypeCR                                                */
/*                                                            */
/**************************************************************/

struct EXTERN EnumTypeCR {
   enum Root {
      message = 0,
      operationel = 1,
      evenement_exceptionnel = 2,
      warning = 3
   } ;
} ;

typedef EnumTypeCR::Root ASN1T_EnumTypeCR;

class EXTERN ASN1C_EnumTypeCR : public ASN1CType {
public:
   ASN1T_EnumTypeCR& msgData;
   ASN1C_EnumTypeCR (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumTypeCR& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumTypeCR (ASN1CTXT* ctxt_p, ASN1T_EnumTypeCR value);
EXTERN int asn1PD_EnumTypeCR (ASN1CTXT* ctxt_p, ASN1T_EnumTypeCR* pvalue);

EXTERN void asn1Print_EnumTypeCR (ASN1ConstCharPtr name, ASN1T_EnumTypeCR* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgCR                                                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgCR {
   ASN1T_OID  unit_id;
   ASN1T_OID  cr;
   ASN1T_EnumTypeCR  type;
   ASN1T_MissionParameters  parametres;
} ASN1T_MsgCR;

class EXTERN ASN1C_MsgCR : public ASN1CType {
public:
   ASN1T_MsgCR& msgData;
   ASN1C_MsgCR (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgCR& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgCR (ASN1CTXT* ctxt_p, ASN1T_MsgCR* pvalue);
EXTERN int asn1PD_MsgCR (ASN1CTXT* ctxt_p, ASN1T_MsgCR* pvalue);

EXTERN void asn1Print_MsgCR (ASN1ConstCharPtr name, ASN1T_MsgCR* pvalue);

#endif

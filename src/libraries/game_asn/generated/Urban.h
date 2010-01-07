/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 06-Jan-2010.
 */
#ifndef URBAN_H
#define URBAN_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "Base.h"

/**************************************************************/
/*                                                            */
/*  UrbanIncludes                                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_UrbanIncludes {
} ASN1T_UrbanIncludes;

class EXTERN ASN1C_UrbanIncludes : public ASN1CType {
public:
   ASN1T_UrbanIncludes& msgData;
   ASN1C_UrbanIncludes (
      ASN1MessageBuffer& msgBuf, ASN1T_UrbanIncludes& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_UrbanIncludes (ASN1CTXT* ctxt_p, ASN1T_UrbanIncludes* pvalue);
EXTERN int asn1PD_UrbanIncludes (ASN1CTXT* ctxt_p, ASN1T_UrbanIncludes* pvalue);

EXTERN void asn1Print_UrbanIncludes (ASN1ConstCharPtr name, ASN1T_UrbanIncludes* pvalue);

/**************************************************************/
/*                                                            */
/*  UrbanExports                                              */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_UrbanExports : public ASN1CType {
public:
   ASN1C_UrbanExports (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_UrbanExports (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_UrbanExports (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_UrbanExports (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  EnumUrbanErrorCode                                        */
/*                                                            */
/**************************************************************/

struct EXTERN EnumUrbanErrorCode {
   enum Root {
      no_error = 0,
      error_invalid_urban = 1,
      error_invalid_id = 2,
      error_invalid_camp = 3,
      error_invalid_localisation = 4,
      error_missing_specific_attributes = 5,
      error_invalid_specific_attributes = 6
   } ;
} ;

typedef EnumUrbanErrorCode::Root ASN1T_EnumUrbanErrorCode;

class EXTERN ASN1C_EnumUrbanErrorCode : public ASN1CType {
public:
   ASN1T_EnumUrbanErrorCode& msgData;
   ASN1C_EnumUrbanErrorCode (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumUrbanErrorCode& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumUrbanErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumUrbanErrorCode value);
EXTERN int asn1PD_EnumUrbanErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumUrbanErrorCode* pvalue);

EXTERN void asn1Print_EnumUrbanErrorCode (ASN1ConstCharPtr name, ASN1T_EnumUrbanErrorCode* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributeVegetation                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributeVegetation {
   ASN1VisibleString  type;
   ASN1INT   height;
   ASN1REAL  density;
} ASN1T_ObjectAttributeVegetation;

class EXTERN ASN1C_ObjectAttributeVegetation : public ASN1CType {
public:
   ASN1T_ObjectAttributeVegetation& msgData;
   ASN1C_ObjectAttributeVegetation (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributeVegetation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributeVegetation (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeVegetation* pvalue);
EXTERN int asn1PD_ObjectAttributeVegetation (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeVegetation* pvalue);

EXTERN void asn1Print_ObjectAttributeVegetation (ASN1ConstCharPtr name, ASN1T_ObjectAttributeVegetation* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributeSoil                                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributeSoil {
   ASN1REAL  occupation;
   ASN1REAL  trafficability;
   ASN1BOOL  multiple;
   ASN1VisibleString  compoundClearing;
} ASN1T_ObjectAttributeSoil;

class EXTERN ASN1C_ObjectAttributeSoil : public ASN1CType {
public:
   ASN1T_ObjectAttributeSoil& msgData;
   ASN1C_ObjectAttributeSoil (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributeSoil& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributeSoil (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeSoil* pvalue);
EXTERN int asn1PD_ObjectAttributeSoil (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeSoil* pvalue);

EXTERN void asn1Print_ObjectAttributeSoil (ASN1ConstCharPtr name, ASN1T_ObjectAttributeSoil* pvalue);

/**************************************************************/
/*                                                            */
/*  ObjectAttributeArchitecture                               */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ObjectAttributeArchitecture {
   ASN1REAL  height;
   ASN1INT   floorNumber;
   ASN1INT   basementLevelNumber;
   ASN1VisibleString  roofShape;
   ASN1VisibleString  material;
   ASN1REAL  innerCluttering;
   ASN1REAL  facadeOpacity;
} ASN1T_ObjectAttributeArchitecture;

class EXTERN ASN1C_ObjectAttributeArchitecture : public ASN1CType {
public:
   ASN1T_ObjectAttributeArchitecture& msgData;
   ASN1C_ObjectAttributeArchitecture (
      ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributeArchitecture& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ObjectAttributeArchitecture (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeArchitecture* pvalue);
EXTERN int asn1PD_ObjectAttributeArchitecture (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeArchitecture* pvalue);

EXTERN void asn1Print_ObjectAttributeArchitecture (ASN1ConstCharPtr name, ASN1T_ObjectAttributeArchitecture* pvalue);

/**************************************************************/
/*                                                            */
/*  UrbanAttributes                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_UrbanAttributes {
   struct {
      unsigned colorPresent : 1;
      unsigned vegetationPresent : 1;
      unsigned soilPresent : 1;
      unsigned architecturePresent : 1;
   } m;
   ASN1T_ColorRGBA  color;
   ASN1T_ObjectAttributeVegetation  vegetation;
   ASN1T_ObjectAttributeSoil  soil;
   ASN1T_ObjectAttributeArchitecture  architecture;

   ASN1T_UrbanAttributes () {
      m.colorPresent = 0;
      m.vegetationPresent = 0;
      m.soilPresent = 0;
      m.architecturePresent = 0;
   }
} ASN1T_UrbanAttributes;

class EXTERN ASN1C_UrbanAttributes : public ASN1CType {
public:
   ASN1T_UrbanAttributes& msgData;
   ASN1C_UrbanAttributes (
      ASN1MessageBuffer& msgBuf, ASN1T_UrbanAttributes& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_UrbanAttributes (ASN1CTXT* ctxt_p, ASN1T_UrbanAttributes* pvalue);
EXTERN int asn1PD_UrbanAttributes (ASN1CTXT* ctxt_p, ASN1T_UrbanAttributes* pvalue);

EXTERN void asn1Print_UrbanAttributes (ASN1ConstCharPtr name, ASN1T_UrbanAttributes* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgUrbanCreation                                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgUrbanCreation {
   ASN1T_OID  oid;
   ASN1VisibleString  name;
   ASN1T_Location  location;
   ASN1T_UrbanAttributes  attributes;
} ASN1T_MsgUrbanCreation;

class EXTERN ASN1C_MsgUrbanCreation : public ASN1CType {
public:
   ASN1T_MsgUrbanCreation& msgData;
   ASN1C_MsgUrbanCreation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgUrbanCreation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgUrbanCreation (ASN1CTXT* ctxt_p, ASN1T_MsgUrbanCreation* pvalue);
EXTERN int asn1PD_MsgUrbanCreation (ASN1CTXT* ctxt_p, ASN1T_MsgUrbanCreation* pvalue);

EXTERN void asn1Print_MsgUrbanCreation (ASN1ConstCharPtr name, ASN1T_MsgUrbanCreation* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgUrbanUpdate                                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgUrbanUpdate {
   struct {
      unsigned locationPresent : 1;
   } m;
   ASN1T_OID  oid;
   ASN1T_Location  location;
   ASN1T_UrbanAttributes  attributes;

   ASN1T_MsgUrbanUpdate () {
      m.locationPresent = 0;
   }
} ASN1T_MsgUrbanUpdate;

class EXTERN ASN1C_MsgUrbanUpdate : public ASN1CType {
public:
   ASN1T_MsgUrbanUpdate& msgData;
   ASN1C_MsgUrbanUpdate (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgUrbanUpdate& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgUrbanUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgUrbanUpdate* pvalue);
EXTERN int asn1PD_MsgUrbanUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgUrbanUpdate* pvalue);

EXTERN void asn1Print_MsgUrbanUpdate (ASN1ConstCharPtr name, ASN1T_MsgUrbanUpdate* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgUrbanDestruction                                       */
/*                                                            */
/**************************************************************/

typedef ASN1T_OID ASN1T_MsgUrbanDestruction;

class EXTERN ASN1C_MsgUrbanDestruction : public ASN1CType {
public:
   ASN1T_MsgUrbanDestruction& msgData;
   ASN1C_MsgUrbanDestruction (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgUrbanDestruction& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgUrbanDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgUrbanDestruction value);
EXTERN int asn1PD_MsgUrbanDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgUrbanDestruction* pvalue);

EXTERN void asn1Print_MsgUrbanDestruction (ASN1ConstCharPtr name, ASN1T_MsgUrbanDestruction* pvalue);

/**************************************************************/
/*                                                            */
/*  MagicActionUpdateUrban                                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MagicActionUpdateUrban {
   ASN1T_OID  oid;
} ASN1T_MagicActionUpdateUrban;

class EXTERN ASN1C_MagicActionUpdateUrban : public ASN1CType {
public:
   ASN1T_MagicActionUpdateUrban& msgData;
   ASN1C_MagicActionUpdateUrban (
      ASN1MessageBuffer& msgBuf, ASN1T_MagicActionUpdateUrban& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MagicActionUpdateUrban (ASN1CTXT* ctxt_p, ASN1T_MagicActionUpdateUrban* pvalue);
EXTERN int asn1PD_MagicActionUpdateUrban (ASN1CTXT* ctxt_p, ASN1T_MagicActionUpdateUrban* pvalue);

EXTERN void asn1Print_MagicActionUpdateUrban (ASN1ConstCharPtr name, ASN1T_MagicActionUpdateUrban* pvalue);

/**************************************************************/
/*                                                            */
/*  MagicActionCreateUrban                                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MagicActionCreateUrban {
   ASN1T_Location  location;
   ASN1VisibleString  name;
} ASN1T_MagicActionCreateUrban;

class EXTERN ASN1C_MagicActionCreateUrban : public ASN1CType {
public:
   ASN1T_MagicActionCreateUrban& msgData;
   ASN1C_MagicActionCreateUrban (
      ASN1MessageBuffer& msgBuf, ASN1T_MagicActionCreateUrban& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MagicActionCreateUrban (ASN1CTXT* ctxt_p, ASN1T_MagicActionCreateUrban* pvalue);
EXTERN int asn1PD_MagicActionCreateUrban (ASN1CTXT* ctxt_p, ASN1T_MagicActionCreateUrban* pvalue);

EXTERN void asn1Print_MagicActionCreateUrban (ASN1ConstCharPtr name, ASN1T_MagicActionCreateUrban* pvalue);

/**************************************************************/
/*                                                            */
/*  MagicActionDestroyUrban                                   */
/*                                                            */
/**************************************************************/

typedef ASN1T_OID ASN1T_MagicActionDestroyUrban;

class EXTERN ASN1C_MagicActionDestroyUrban : public ASN1CType {
public:
   ASN1T_MagicActionDestroyUrban& msgData;
   ASN1C_MagicActionDestroyUrban (
      ASN1MessageBuffer& msgBuf, ASN1T_MagicActionDestroyUrban& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MagicActionDestroyUrban (ASN1CTXT* ctxt_p, ASN1T_MagicActionDestroyUrban value);
EXTERN int asn1PD_MagicActionDestroyUrban (ASN1CTXT* ctxt_p, ASN1T_MagicActionDestroyUrban* pvalue);

EXTERN void asn1Print_MagicActionDestroyUrban (ASN1ConstCharPtr name, ASN1T_MagicActionDestroyUrban* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgUrbanMagicAction_action                                */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_MsgUrbanMagicAction_action_create_urban 1
#define T_MsgUrbanMagicAction_action_update_urban 2
#define T_MsgUrbanMagicAction_action_destroy_urban 3

typedef struct EXTERN ASN1T_MsgUrbanMagicAction_action {
   int t;
   union {
      /* t = 1 */
      ASN1T_MagicActionCreateUrban *create_urban;
      /* t = 2 */
      ASN1T_MagicActionUpdateUrban *update_urban;
      /* t = 3 */
      ASN1T_MagicActionDestroyUrban  destroy_urban;
   } u;
} ASN1T_MsgUrbanMagicAction_action;

class EXTERN ASN1C_MsgUrbanMagicAction_action : public ASN1CType {
public:
   ASN1T_MsgUrbanMagicAction_action& msgData;
   ASN1C_MsgUrbanMagicAction_action (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgUrbanMagicAction_action& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgUrbanMagicAction_action (ASN1CTXT* ctxt_p, ASN1T_MsgUrbanMagicAction_action* pvalue);
EXTERN int asn1PD_MsgUrbanMagicAction_action (ASN1CTXT* ctxt_p, ASN1T_MsgUrbanMagicAction_action* pvalue);

EXTERN void asn1Print_MsgUrbanMagicAction_action (ASN1ConstCharPtr name, ASN1T_MsgUrbanMagicAction_action* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgUrbanMagicAction                                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgUrbanMagicAction {
   ASN1T_MsgUrbanMagicAction_action  action;
} ASN1T_MsgUrbanMagicAction;

class EXTERN ASN1C_MsgUrbanMagicAction : public ASN1CType {
public:
   ASN1T_MsgUrbanMagicAction& msgData;
   ASN1C_MsgUrbanMagicAction (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgUrbanMagicAction& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgUrbanMagicAction (ASN1CTXT* ctxt_p, ASN1T_MsgUrbanMagicAction* pvalue);
EXTERN int asn1PD_MsgUrbanMagicAction (ASN1CTXT* ctxt_p, ASN1T_MsgUrbanMagicAction* pvalue);

EXTERN void asn1Print_MsgUrbanMagicAction (ASN1ConstCharPtr name, ASN1T_MsgUrbanMagicAction* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgUrbanMagicActionAck                                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgUrbanMagicActionAck {
   ASN1T_EnumUrbanErrorCode  error_code;
} ASN1T_MsgUrbanMagicActionAck;

class EXTERN ASN1C_MsgUrbanMagicActionAck : public ASN1CType {
public:
   ASN1T_MsgUrbanMagicActionAck& msgData;
   ASN1C_MsgUrbanMagicActionAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgUrbanMagicActionAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgUrbanMagicActionAck (ASN1CTXT* ctxt_p, ASN1T_MsgUrbanMagicActionAck* pvalue);
EXTERN int asn1PD_MsgUrbanMagicActionAck (ASN1CTXT* ctxt_p, ASN1T_MsgUrbanMagicActionAck* pvalue);

EXTERN void asn1Print_MsgUrbanMagicActionAck (ASN1ConstCharPtr name, ASN1T_MsgUrbanMagicActionAck* pvalue);

#endif

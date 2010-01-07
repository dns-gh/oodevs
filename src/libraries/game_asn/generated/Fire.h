/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 06-Jan-2010.
 */
#ifndef FIRE_H
#define FIRE_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "Base.h"

/**************************************************************/
/*                                                            */
/*  FireIncludes                                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_FireIncludes {
} ASN1T_FireIncludes;

class EXTERN ASN1C_FireIncludes : public ASN1CType {
public:
   ASN1T_FireIncludes& msgData;
   ASN1C_FireIncludes (
      ASN1MessageBuffer& msgBuf, ASN1T_FireIncludes& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_FireIncludes (ASN1CTXT* ctxt_p, ASN1T_FireIncludes* pvalue);
EXTERN int asn1PD_FireIncludes (ASN1CTXT* ctxt_p, ASN1T_FireIncludes* pvalue);

EXTERN void asn1Print_FireIncludes (ASN1ConstCharPtr name, ASN1T_FireIncludes* pvalue);

/**************************************************************/
/*                                                            */
/*  FireExports                                               */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_FireExports : public ASN1CType {
public:
   ASN1C_FireExports (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_FireExports (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_FireExports (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_FireExports (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  UnitHumanFireDamage                                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_UnitHumanFireDamage {
   ASN1T_EnumHumanRank  rank;
   ASN1INT   alive_nbr;
   ASN1INT   dead_nbr;
   ASN1INT   wounded_u1_nbr;
   ASN1INT   wounded_u2_nbr;
   ASN1INT   wounded_u3_nbr;
   ASN1INT   wounded_ue_nbr;
} ASN1T_UnitHumanFireDamage;

class EXTERN ASN1C_UnitHumanFireDamage : public ASN1CType {
public:
   ASN1T_UnitHumanFireDamage& msgData;
   ASN1C_UnitHumanFireDamage (
      ASN1MessageBuffer& msgBuf, ASN1T_UnitHumanFireDamage& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_UnitHumanFireDamage (ASN1CTXT* ctxt_p, ASN1T_UnitHumanFireDamage* pvalue);
EXTERN int asn1PD_UnitHumanFireDamage (ASN1CTXT* ctxt_p, ASN1T_UnitHumanFireDamage* pvalue);

EXTERN void asn1Print_UnitHumanFireDamage (ASN1ConstCharPtr name, ASN1T_UnitHumanFireDamage* pvalue);

/**************************************************************/
/*                                                            */
/*  UnitEquipmentFireDamage                                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_UnitEquipmentFireDamage {
   ASN1T_EquipmentType  equipement_type;
   ASN1INT   available_nbr;
   ASN1INT   unavailable_nbr;
   ASN1INT   repairable_nbr;
} ASN1T_UnitEquipmentFireDamage;

class EXTERN ASN1C_UnitEquipmentFireDamage : public ASN1CType {
public:
   ASN1T_UnitEquipmentFireDamage& msgData;
   ASN1C_UnitEquipmentFireDamage (
      ASN1MessageBuffer& msgBuf, ASN1T_UnitEquipmentFireDamage& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_UnitEquipmentFireDamage (ASN1CTXT* ctxt_p, ASN1T_UnitEquipmentFireDamage* pvalue);
EXTERN int asn1PD_UnitEquipmentFireDamage (ASN1CTXT* ctxt_p, ASN1T_UnitEquipmentFireDamage* pvalue);

EXTERN void asn1Print_UnitEquipmentFireDamage (ASN1ConstCharPtr name, ASN1T_UnitEquipmentFireDamage* pvalue);

/**************************************************************/
/*                                                            */
/*  _SeqOfUnitHumanFireDamage                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T__SeqOfUnitHumanFireDamage {
   ASN1UINT n;
   ASN1T_UnitHumanFireDamage *elem;
} ASN1T__SeqOfUnitHumanFireDamage;

class EXTERN ASN1C__SeqOfUnitHumanFireDamage : public ASN1CType {
public:
   ASN1T__SeqOfUnitHumanFireDamage& msgData;
   ASN1C__SeqOfUnitHumanFireDamage (
      ASN1MessageBuffer& msgBuf, ASN1T__SeqOfUnitHumanFireDamage& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE__SeqOfUnitHumanFireDamage (ASN1CTXT* ctxt_p, ASN1T__SeqOfUnitHumanFireDamage* pvalue);
EXTERN int asn1PD__SeqOfUnitHumanFireDamage (ASN1CTXT* ctxt_p, ASN1T__SeqOfUnitHumanFireDamage* pvalue);

EXTERN void asn1Print__SeqOfUnitHumanFireDamage (ASN1ConstCharPtr name, ASN1T__SeqOfUnitHumanFireDamage* pvalue);

/**************************************************************/
/*                                                            */
/*  _SeqOfUnitEquipmentFireDamage                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T__SeqOfUnitEquipmentFireDamage {
   ASN1UINT n;
   ASN1T_UnitEquipmentFireDamage *elem;
} ASN1T__SeqOfUnitEquipmentFireDamage;

class EXTERN ASN1C__SeqOfUnitEquipmentFireDamage : public ASN1CType {
public:
   ASN1T__SeqOfUnitEquipmentFireDamage& msgData;
   ASN1C__SeqOfUnitEquipmentFireDamage (
      ASN1MessageBuffer& msgBuf, ASN1T__SeqOfUnitEquipmentFireDamage& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE__SeqOfUnitEquipmentFireDamage (ASN1CTXT* ctxt_p, ASN1T__SeqOfUnitEquipmentFireDamage* pvalue);
EXTERN int asn1PD__SeqOfUnitEquipmentFireDamage (ASN1CTXT* ctxt_p, ASN1T__SeqOfUnitEquipmentFireDamage* pvalue);

EXTERN void asn1Print__SeqOfUnitEquipmentFireDamage (ASN1ConstCharPtr name, ASN1T__SeqOfUnitEquipmentFireDamage* pvalue);

/**************************************************************/
/*                                                            */
/*  UnitFireDamages                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_UnitFireDamages {
   ASN1T_Unit  target;
   ASN1T__SeqOfUnitHumanFireDamage  humans;
   ASN1T__SeqOfUnitEquipmentFireDamage  equipments;
} ASN1T_UnitFireDamages;

class EXTERN ASN1C_UnitFireDamages : public ASN1CType {
public:
   ASN1T_UnitFireDamages& msgData;
   ASN1C_UnitFireDamages (
      ASN1MessageBuffer& msgBuf, ASN1T_UnitFireDamages& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_UnitFireDamages (ASN1CTXT* ctxt_p, ASN1T_UnitFireDamages* pvalue);
EXTERN int asn1PD_UnitFireDamages (ASN1CTXT* ctxt_p, ASN1T_UnitFireDamages* pvalue);

EXTERN void asn1Print_UnitFireDamages (ASN1ConstCharPtr name, ASN1T_UnitFireDamages* pvalue);

/**************************************************************/
/*                                                            */
/*  PopulationFireDamages                                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_PopulationFireDamages {
   ASN1T_Population  target;
   ASN1INT   dead_nbr;
} ASN1T_PopulationFireDamages;

class EXTERN ASN1C_PopulationFireDamages : public ASN1CType {
public:
   ASN1T_PopulationFireDamages& msgData;
   ASN1C_PopulationFireDamages (
      ASN1MessageBuffer& msgBuf, ASN1T_PopulationFireDamages& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_PopulationFireDamages (ASN1CTXT* ctxt_p, ASN1T_PopulationFireDamages* pvalue);
EXTERN int asn1PD_PopulationFireDamages (ASN1CTXT* ctxt_p, ASN1T_PopulationFireDamages* pvalue);

EXTERN void asn1Print_PopulationFireDamages (ASN1ConstCharPtr name, ASN1T_PopulationFireDamages* pvalue);

/**************************************************************/
/*                                                            */
/*  UnitsFireDamages                                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_UnitsFireDamages {
   ASN1UINT n;
   ASN1T_UnitFireDamages *elem;
} ASN1T_UnitsFireDamages;

class EXTERN ASN1C_UnitsFireDamages : public ASN1CType {
public:
   ASN1T_UnitsFireDamages& msgData;
   ASN1C_UnitsFireDamages (
      ASN1MessageBuffer& msgBuf, ASN1T_UnitsFireDamages& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_UnitsFireDamages (ASN1CTXT* ctxt_p, ASN1T_UnitsFireDamages* pvalue);
EXTERN int asn1PD_UnitsFireDamages (ASN1CTXT* ctxt_p, ASN1T_UnitsFireDamages* pvalue);

EXTERN void asn1Print_UnitsFireDamages (ASN1ConstCharPtr name, ASN1T_UnitsFireDamages* pvalue);

/**************************************************************/
/*                                                            */
/*  PopulationsFireDamages                                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_PopulationsFireDamages {
   ASN1UINT n;
   ASN1T_PopulationFireDamages *elem;
} ASN1T_PopulationsFireDamages;

class EXTERN ASN1C_PopulationsFireDamages : public ASN1CType {
public:
   ASN1T_PopulationsFireDamages& msgData;
   ASN1C_PopulationsFireDamages (
      ASN1MessageBuffer& msgBuf, ASN1T_PopulationsFireDamages& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_PopulationsFireDamages (ASN1CTXT* ctxt_p, ASN1T_PopulationsFireDamages* pvalue);
EXTERN int asn1PD_PopulationsFireDamages (ASN1CTXT* ctxt_p, ASN1T_PopulationsFireDamages* pvalue);

EXTERN void asn1Print_PopulationsFireDamages (ASN1ConstCharPtr name, ASN1T_PopulationsFireDamages* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgStartUnitFire_target                                   */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_MsgStartUnitFire_target_unit  1
#define T_MsgStartUnitFire_target_population 2
#define T_MsgStartUnitFire_target_position 3

typedef struct EXTERN ASN1T_MsgStartUnitFire_target {
   int t;
   union {
      /* t = 1 */
      ASN1T_Unit  unit;
      /* t = 2 */
      ASN1T_Population  population;
      /* t = 3 */
      ASN1T_CoordLatLong *position;
   } u;
} ASN1T_MsgStartUnitFire_target;

class EXTERN ASN1C_MsgStartUnitFire_target : public ASN1CType {
public:
   ASN1T_MsgStartUnitFire_target& msgData;
   ASN1C_MsgStartUnitFire_target (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgStartUnitFire_target& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgStartUnitFire_target (ASN1CTXT* ctxt_p, ASN1T_MsgStartUnitFire_target* pvalue);
EXTERN int asn1PD_MsgStartUnitFire_target (ASN1CTXT* ctxt_p, ASN1T_MsgStartUnitFire_target* pvalue);

EXTERN void asn1Print_MsgStartUnitFire_target (ASN1ConstCharPtr name, ASN1T_MsgStartUnitFire_target* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgStartUnitFire_type                                     */
/*                                                            */
/**************************************************************/

struct EXTERN MsgStartUnitFire_type {
   enum Root {
      direct = 0,
      indirect = 1
   } ;
} ;

typedef MsgStartUnitFire_type::Root ASN1T_MsgStartUnitFire_type;

class EXTERN ASN1C_MsgStartUnitFire_type : public ASN1CType {
public:
   ASN1T_MsgStartUnitFire_type& msgData;
   ASN1C_MsgStartUnitFire_type (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgStartUnitFire_type& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgStartUnitFire_type (ASN1CTXT* ctxt_p, ASN1T_MsgStartUnitFire_type value);
EXTERN int asn1PD_MsgStartUnitFire_type (ASN1CTXT* ctxt_p, ASN1T_MsgStartUnitFire_type* pvalue);

EXTERN void asn1Print_MsgStartUnitFire_type (ASN1ConstCharPtr name, ASN1T_MsgStartUnitFire_type* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgStartUnitFire                                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgStartUnitFire {
   struct {
      unsigned ammunitionPresent : 1;
   } m;
   ASN1T_UnitFire  fire_oid;
   ASN1T_Unit  firer_oid;
   ASN1T_MsgStartUnitFire_target  target;
   ASN1T_MsgStartUnitFire_type  type;
   ASN1T_DotationType  ammunition;

   ASN1T_MsgStartUnitFire () {
      m.ammunitionPresent = 0;
   }
} ASN1T_MsgStartUnitFire;

class EXTERN ASN1C_MsgStartUnitFire : public ASN1CType {
public:
   ASN1T_MsgStartUnitFire& msgData;
   ASN1C_MsgStartUnitFire (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgStartUnitFire& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgStartUnitFire (ASN1CTXT* ctxt_p, ASN1T_MsgStartUnitFire* pvalue);
EXTERN int asn1PD_MsgStartUnitFire (ASN1CTXT* ctxt_p, ASN1T_MsgStartUnitFire* pvalue);

EXTERN void asn1Print_MsgStartUnitFire (ASN1ConstCharPtr name, ASN1T_MsgStartUnitFire* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgStopUnitFire                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgStopUnitFire {
   ASN1T_UnitFire  fire_oid;
   ASN1T_UnitsFireDamages  units_damages;
   ASN1T_PopulationsFireDamages  populations_damages;
} ASN1T_MsgStopUnitFire;

class EXTERN ASN1C_MsgStopUnitFire : public ASN1CType {
public:
   ASN1T_MsgStopUnitFire& msgData;
   ASN1C_MsgStopUnitFire (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgStopUnitFire& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgStopUnitFire (ASN1CTXT* ctxt_p, ASN1T_MsgStopUnitFire* pvalue);
EXTERN int asn1PD_MsgStopUnitFire (ASN1CTXT* ctxt_p, ASN1T_MsgStopUnitFire* pvalue);

EXTERN void asn1Print_MsgStopUnitFire (ASN1ConstCharPtr name, ASN1T_MsgStopUnitFire* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgStartPopulationFire                                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgStartPopulationFire {
   ASN1T_PopulationFire  fire_oid;
   ASN1T_Population  firer_oid;
} ASN1T_MsgStartPopulationFire;

class EXTERN ASN1C_MsgStartPopulationFire : public ASN1CType {
public:
   ASN1T_MsgStartPopulationFire& msgData;
   ASN1C_MsgStartPopulationFire (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgStartPopulationFire& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgStartPopulationFire (ASN1CTXT* ctxt_p, ASN1T_MsgStartPopulationFire* pvalue);
EXTERN int asn1PD_MsgStartPopulationFire (ASN1CTXT* ctxt_p, ASN1T_MsgStartPopulationFire* pvalue);

EXTERN void asn1Print_MsgStartPopulationFire (ASN1ConstCharPtr name, ASN1T_MsgStartPopulationFire* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgStopPopulationFire                                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgStopPopulationFire {
   ASN1T_PopulationFire  fire_oid;
   ASN1T_UnitsFireDamages  units_damages;
} ASN1T_MsgStopPopulationFire;

class EXTERN ASN1C_MsgStopPopulationFire : public ASN1CType {
public:
   ASN1T_MsgStopPopulationFire& msgData;
   ASN1C_MsgStopPopulationFire (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgStopPopulationFire& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgStopPopulationFire (ASN1CTXT* ctxt_p, ASN1T_MsgStopPopulationFire* pvalue);
EXTERN int asn1PD_MsgStopPopulationFire (ASN1CTXT* ctxt_p, ASN1T_MsgStopPopulationFire* pvalue);

EXTERN void asn1Print_MsgStopPopulationFire (ASN1ConstCharPtr name, ASN1T_MsgStopPopulationFire* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgExplosion                                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgExplosion {
   ASN1T_OID  object_oid;
   ASN1T_UnitsFireDamages  units_damages;
   ASN1T_PopulationsFireDamages  populations_damages;
} ASN1T_MsgExplosion;

class EXTERN ASN1C_MsgExplosion : public ASN1CType {
public:
   ASN1T_MsgExplosion& msgData;
   ASN1C_MsgExplosion (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgExplosion& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgExplosion (ASN1CTXT* ctxt_p, ASN1T_MsgExplosion* pvalue);
EXTERN int asn1PD_MsgExplosion (ASN1CTXT* ctxt_p, ASN1T_MsgExplosion* pvalue);

EXTERN void asn1Print_MsgExplosion (ASN1ConstCharPtr name, ASN1T_MsgExplosion* pvalue);

/**************************************************************/
/*                                                            */
/*  EnumFireEffectType                                        */
/*                                                            */
/**************************************************************/

struct EXTERN EnumFireEffectType {
   enum Root {
      fumigene = 0,
      eclairant = 1
   } ;
} ;

typedef EnumFireEffectType::Root ASN1T_EnumFireEffectType;

class EXTERN ASN1C_EnumFireEffectType : public ASN1CType {
public:
   ASN1T_EnumFireEffectType& msgData;
   ASN1C_EnumFireEffectType (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumFireEffectType& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumFireEffectType (ASN1CTXT* ctxt_p, ASN1T_EnumFireEffectType value);
EXTERN int asn1PD_EnumFireEffectType (ASN1CTXT* ctxt_p, ASN1T_EnumFireEffectType* pvalue);

EXTERN void asn1Print_EnumFireEffectType (ASN1ConstCharPtr name, ASN1T_EnumFireEffectType* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgStartFireEffect                                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgStartFireEffect {
   ASN1T_OID  effect_oid;
   ASN1T_Location  location;
   ASN1T_EnumFireEffectType  type;
} ASN1T_MsgStartFireEffect;

class EXTERN ASN1C_MsgStartFireEffect : public ASN1CType {
public:
   ASN1T_MsgStartFireEffect& msgData;
   ASN1C_MsgStartFireEffect (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgStartFireEffect& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgStartFireEffect (ASN1CTXT* ctxt_p, ASN1T_MsgStartFireEffect* pvalue);
EXTERN int asn1PD_MsgStartFireEffect (ASN1CTXT* ctxt_p, ASN1T_MsgStartFireEffect* pvalue);

EXTERN void asn1Print_MsgStartFireEffect (ASN1ConstCharPtr name, ASN1T_MsgStartFireEffect* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgStopFireEffect                                         */
/*                                                            */
/**************************************************************/

typedef ASN1T_OID ASN1T_MsgStopFireEffect;

class EXTERN ASN1C_MsgStopFireEffect : public ASN1CType {
public:
   ASN1T_MsgStopFireEffect& msgData;
   ASN1C_MsgStopFireEffect (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgStopFireEffect& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgStopFireEffect (ASN1CTXT* ctxt_p, ASN1T_MsgStopFireEffect value);
EXTERN int asn1PD_MsgStopFireEffect (ASN1CTXT* ctxt_p, ASN1T_MsgStopFireEffect* pvalue);

EXTERN void asn1Print_MsgStopFireEffect (ASN1ConstCharPtr name, ASN1T_MsgStopFireEffect* pvalue);

#endif

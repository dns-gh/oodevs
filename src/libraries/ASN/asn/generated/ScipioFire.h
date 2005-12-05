/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 05-Dec-2005.
 */
#ifndef SCIPIOFIRE_H
#define SCIPIOFIRE_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  FireDamagePionHuman                                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_FireDamagePionHuman {
   ASN1T_EnumHumanRank  rang;
   ASN1INT   nb_non_blesses;
   ASN1INT   nb_morts;
   ASN1INT   nb_blesses_urgence_1;
   ASN1INT   nb_blesses_urgence_2;
   ASN1INT   nb_blesses_urgence_3;
   ASN1INT   nb_blesses_urgence_extreme;
} ASN1T_FireDamagePionHuman;

class EXTERN ASN1C_FireDamagePionHuman : public ASN1CType {
public:
   ASN1T_FireDamagePionHuman& msgData;
   ASN1C_FireDamagePionHuman (
      ASN1MessageBuffer& msgBuf, ASN1T_FireDamagePionHuman& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_FireDamagePionHuman (ASN1CTXT* ctxt_p, ASN1T_FireDamagePionHuman* pvalue);
EXTERN int asn1PD_FireDamagePionHuman (ASN1CTXT* ctxt_p, ASN1T_FireDamagePionHuman* pvalue);

EXTERN void asn1Print_FireDamagePionHuman (ASN1ConstCharPtr name, ASN1T_FireDamagePionHuman* pvalue);

/**************************************************************/
/*                                                            */
/*  FireDamagePionEquipment                                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_FireDamagePionEquipment {
   ASN1T_TypeEquipement  type_equipement;
   ASN1INT   nb_disponibles;
   ASN1INT   nb_indisponibles;
   ASN1INT   nb_reparables;
} ASN1T_FireDamagePionEquipment;

class EXTERN ASN1C_FireDamagePionEquipment : public ASN1CType {
public:
   ASN1T_FireDamagePionEquipment& msgData;
   ASN1C_FireDamagePionEquipment (
      ASN1MessageBuffer& msgBuf, ASN1T_FireDamagePionEquipment& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_FireDamagePionEquipment (ASN1CTXT* ctxt_p, ASN1T_FireDamagePionEquipment* pvalue);
EXTERN int asn1PD_FireDamagePionEquipment (ASN1CTXT* ctxt_p, ASN1T_FireDamagePionEquipment* pvalue);

EXTERN void asn1Print_FireDamagePionEquipment (ASN1ConstCharPtr name, ASN1T_FireDamagePionEquipment* pvalue);

/**************************************************************/
/*                                                            */
/*  _SeqOfFireDamagePionHuman                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T__SeqOfFireDamagePionHuman {
   ASN1UINT n;
   ASN1T_FireDamagePionHuman *elem;
} ASN1T__SeqOfFireDamagePionHuman;

class EXTERN ASN1C__SeqOfFireDamagePionHuman : public ASN1CType {
public:
   ASN1T__SeqOfFireDamagePionHuman& msgData;
   ASN1C__SeqOfFireDamagePionHuman (
      ASN1MessageBuffer& msgBuf, ASN1T__SeqOfFireDamagePionHuman& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE__SeqOfFireDamagePionHuman (ASN1CTXT* ctxt_p, ASN1T__SeqOfFireDamagePionHuman* pvalue);
EXTERN int asn1PD__SeqOfFireDamagePionHuman (ASN1CTXT* ctxt_p, ASN1T__SeqOfFireDamagePionHuman* pvalue);

EXTERN void asn1Print__SeqOfFireDamagePionHuman (ASN1ConstCharPtr name, ASN1T__SeqOfFireDamagePionHuman* pvalue);

/**************************************************************/
/*                                                            */
/*  _SeqOfFireDamagePionEquipment                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T__SeqOfFireDamagePionEquipment {
   ASN1UINT n;
   ASN1T_FireDamagePionEquipment *elem;
} ASN1T__SeqOfFireDamagePionEquipment;

class EXTERN ASN1C__SeqOfFireDamagePionEquipment : public ASN1CType {
public:
   ASN1T__SeqOfFireDamagePionEquipment& msgData;
   ASN1C__SeqOfFireDamagePionEquipment (
      ASN1MessageBuffer& msgBuf, ASN1T__SeqOfFireDamagePionEquipment& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE__SeqOfFireDamagePionEquipment (ASN1CTXT* ctxt_p, ASN1T__SeqOfFireDamagePionEquipment* pvalue);
EXTERN int asn1PD__SeqOfFireDamagePionEquipment (ASN1CTXT* ctxt_p, ASN1T__SeqOfFireDamagePionEquipment* pvalue);

EXTERN void asn1Print__SeqOfFireDamagePionEquipment (ASN1ConstCharPtr name, ASN1T__SeqOfFireDamagePionEquipment* pvalue);

/**************************************************************/
/*                                                            */
/*  FireDamagesPion                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_FireDamagesPion {
   ASN1T_Agent  cible;
   ASN1T__SeqOfFireDamagePionHuman  humains;
   ASN1T__SeqOfFireDamagePionEquipment  equipements;
} ASN1T_FireDamagesPion;

class EXTERN ASN1C_FireDamagesPion : public ASN1CType {
public:
   ASN1T_FireDamagesPion& msgData;
   ASN1C_FireDamagesPion (
      ASN1MessageBuffer& msgBuf, ASN1T_FireDamagesPion& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_FireDamagesPion (ASN1CTXT* ctxt_p, ASN1T_FireDamagesPion* pvalue);
EXTERN int asn1PD_FireDamagesPion (ASN1CTXT* ctxt_p, ASN1T_FireDamagesPion* pvalue);

EXTERN void asn1Print_FireDamagesPion (ASN1ConstCharPtr name, ASN1T_FireDamagesPion* pvalue);

/**************************************************************/
/*                                                            */
/*  FireDamagesPopulation                                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_FireDamagesPopulation {
   ASN1T_Population  cible;
   ASN1INT   nb_morts;
} ASN1T_FireDamagesPopulation;

class EXTERN ASN1C_FireDamagesPopulation : public ASN1CType {
public:
   ASN1T_FireDamagesPopulation& msgData;
   ASN1C_FireDamagesPopulation (
      ASN1MessageBuffer& msgBuf, ASN1T_FireDamagesPopulation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_FireDamagesPopulation (ASN1CTXT* ctxt_p, ASN1T_FireDamagesPopulation* pvalue);
EXTERN int asn1PD_FireDamagesPopulation (ASN1CTXT* ctxt_p, ASN1T_FireDamagesPopulation* pvalue);

EXTERN void asn1Print_FireDamagesPopulation (ASN1ConstCharPtr name, ASN1T_FireDamagesPopulation* pvalue);

/**************************************************************/
/*                                                            */
/*  FireDamagesPions                                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_FireDamagesPions {
   ASN1UINT n;
   ASN1T_FireDamagesPion *elem;
} ASN1T_FireDamagesPions;

class EXTERN ASN1C_FireDamagesPions : public ASN1CType {
public:
   ASN1T_FireDamagesPions& msgData;
   ASN1C_FireDamagesPions (
      ASN1MessageBuffer& msgBuf, ASN1T_FireDamagesPions& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_FireDamagesPions (ASN1CTXT* ctxt_p, ASN1T_FireDamagesPions* pvalue);
EXTERN int asn1PD_FireDamagesPions (ASN1CTXT* ctxt_p, ASN1T_FireDamagesPions* pvalue);

EXTERN void asn1Print_FireDamagesPions (ASN1ConstCharPtr name, ASN1T_FireDamagesPions* pvalue);

/**************************************************************/
/*                                                            */
/*  FireDamagesPopulations                                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_FireDamagesPopulations {
   ASN1UINT n;
   ASN1T_FireDamagesPopulation *elem;
} ASN1T_FireDamagesPopulations;

class EXTERN ASN1C_FireDamagesPopulations : public ASN1CType {
public:
   ASN1T_FireDamagesPopulations& msgData;
   ASN1C_FireDamagesPopulations (
      ASN1MessageBuffer& msgBuf, ASN1T_FireDamagesPopulations& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_FireDamagesPopulations (ASN1CTXT* ctxt_p, ASN1T_FireDamagesPopulations* pvalue);
EXTERN int asn1PD_FireDamagesPopulations (ASN1CTXT* ctxt_p, ASN1T_FireDamagesPopulations* pvalue);

EXTERN void asn1Print_FireDamagesPopulations (ASN1ConstCharPtr name, ASN1T_FireDamagesPopulations* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgStartPionFire_cible                                    */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_MsgStartPionFire_cible_pion   1
#define T_MsgStartPionFire_cible_population 2
#define T_MsgStartPionFire_cible_position 3

typedef struct EXTERN ASN1T_MsgStartPionFire_cible {
   int t;
   union {
      /* t = 1 */
      ASN1T_Agent  pion;
      /* t = 2 */
      ASN1T_Population  population;
      /* t = 3 */
      ASN1T_CoordUTM *position;
   } u;
} ASN1T_MsgStartPionFire_cible;

class EXTERN ASN1C_MsgStartPionFire_cible : public ASN1CType {
public:
   ASN1T_MsgStartPionFire_cible& msgData;
   ASN1C_MsgStartPionFire_cible (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgStartPionFire_cible& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgStartPionFire_cible (ASN1CTXT* ctxt_p, ASN1T_MsgStartPionFire_cible* pvalue);
EXTERN int asn1PD_MsgStartPionFire_cible (ASN1CTXT* ctxt_p, ASN1T_MsgStartPionFire_cible* pvalue);

EXTERN void asn1Print_MsgStartPionFire_cible (ASN1ConstCharPtr name, ASN1T_MsgStartPionFire_cible* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgStartPionFire_type                                     */
/*                                                            */
/**************************************************************/

struct EXTERN MsgStartPionFire_type {
   enum Root {
      direct = 0,
      indirect = 1
   } ;
} ;

typedef MsgStartPionFire_type::Root ASN1T_MsgStartPionFire_type;

class EXTERN ASN1C_MsgStartPionFire_type : public ASN1CType {
public:
   ASN1T_MsgStartPionFire_type& msgData;
   ASN1C_MsgStartPionFire_type (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgStartPionFire_type& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgStartPionFire_type (ASN1CTXT* ctxt_p, ASN1T_MsgStartPionFire_type value);
EXTERN int asn1PD_MsgStartPionFire_type (ASN1CTXT* ctxt_p, ASN1T_MsgStartPionFire_type* pvalue);

EXTERN void asn1Print_MsgStartPionFire_type (ASN1ConstCharPtr name, ASN1T_MsgStartPionFire_type* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgStartPionFire                                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgStartPionFire {
   struct {
      unsigned munitionPresent : 1;
   } m;
   ASN1T_TirPion  oid_tir;
   ASN1T_Agent  tireur;
   ASN1T_MsgStartPionFire_cible  cible;
   ASN1T_MsgStartPionFire_type  type;
   ASN1T_TypeDotation  munition;

   ASN1T_MsgStartPionFire () {
      m.munitionPresent = 0;
   }
} ASN1T_MsgStartPionFire;

class EXTERN ASN1C_MsgStartPionFire : public ASN1CType {
public:
   ASN1T_MsgStartPionFire& msgData;
   ASN1C_MsgStartPionFire (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgStartPionFire& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgStartPionFire (ASN1CTXT* ctxt_p, ASN1T_MsgStartPionFire* pvalue);
EXTERN int asn1PD_MsgStartPionFire (ASN1CTXT* ctxt_p, ASN1T_MsgStartPionFire* pvalue);

EXTERN void asn1Print_MsgStartPionFire (ASN1ConstCharPtr name, ASN1T_MsgStartPionFire* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgStopPionFire                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgStopPionFire {
   ASN1T_TirPion  oid_tir;
   ASN1T_FireDamagesPions  degats_pions;
   ASN1T_FireDamagesPopulations  degats_populations;
} ASN1T_MsgStopPionFire;

class EXTERN ASN1C_MsgStopPionFire : public ASN1CType {
public:
   ASN1T_MsgStopPionFire& msgData;
   ASN1C_MsgStopPionFire (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgStopPionFire& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgStopPionFire (ASN1CTXT* ctxt_p, ASN1T_MsgStopPionFire* pvalue);
EXTERN int asn1PD_MsgStopPionFire (ASN1CTXT* ctxt_p, ASN1T_MsgStopPionFire* pvalue);

EXTERN void asn1Print_MsgStopPionFire (ASN1ConstCharPtr name, ASN1T_MsgStopPionFire* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgStartPopulationFire                                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgStartPopulationFire {
   ASN1T_TirPopulation  oid_tir;
   ASN1T_Population  oid_src;
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
   ASN1T_TirPopulation  oid_tir;
   ASN1T_FireDamagesPions  degats_pions;
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
   ASN1T_OID  oid_objet;
   ASN1T_FireDamagesPions  degats_pions;
   ASN1T_FireDamagesPopulations  degats_populations;
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
   ASN1T_OID  oid_effet;
   ASN1T_Localisation  localisation;
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

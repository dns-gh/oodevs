/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 04-Jul-2005.
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
/*  FireResultHuman                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_FireResultHuman {
   ASN1T_EnumHumanRank  rang;
   ASN1INT   nb_non_blesses;
   ASN1INT   nb_morts;
   ASN1INT   nb_blesses_urgence_1;
   ASN1INT   nb_blesses_urgence_2;
   ASN1INT   nb_blesses_urgence_3;
   ASN1INT   nb_blesses_urgence_extreme;
} ASN1T_FireResultHuman;

class EXTERN ASN1C_FireResultHuman : public ASN1CType {
public:
   ASN1T_FireResultHuman& msgData;
   ASN1C_FireResultHuman (
      ASN1MessageBuffer& msgBuf, ASN1T_FireResultHuman& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_FireResultHuman (ASN1CTXT* ctxt_p, ASN1T_FireResultHuman* pvalue);
EXTERN int asn1PD_FireResultHuman (ASN1CTXT* ctxt_p, ASN1T_FireResultHuman* pvalue);

EXTERN void asn1Print_FireResultHuman (ASN1ConstCharPtr name, ASN1T_FireResultHuman* pvalue);

/**************************************************************/
/*                                                            */
/*  FireResultEquipement                                      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_FireResultEquipement {
   ASN1T_TypeEquipement  type_equipement;
   ASN1INT   nb_disponibles;
   ASN1INT   nb_indisponibles;
   ASN1INT   nb_reparables;
} ASN1T_FireResultEquipement;

class EXTERN ASN1C_FireResultEquipement : public ASN1CType {
public:
   ASN1T_FireResultEquipement& msgData;
   ASN1C_FireResultEquipement (
      ASN1MessageBuffer& msgBuf, ASN1T_FireResultEquipement& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_FireResultEquipement (ASN1CTXT* ctxt_p, ASN1T_FireResultEquipement* pvalue);
EXTERN int asn1PD_FireResultEquipement (ASN1CTXT* ctxt_p, ASN1T_FireResultEquipement* pvalue);

EXTERN void asn1Print_FireResultEquipement (ASN1ConstCharPtr name, ASN1T_FireResultEquipement* pvalue);

/**************************************************************/
/*                                                            */
/*  _SeqOfFireResultHuman                                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T__SeqOfFireResultHuman {
   ASN1UINT n;
   ASN1T_FireResultHuman *elem;
} ASN1T__SeqOfFireResultHuman;

class EXTERN ASN1C__SeqOfFireResultHuman : public ASN1CType {
public:
   ASN1T__SeqOfFireResultHuman& msgData;
   ASN1C__SeqOfFireResultHuman (
      ASN1MessageBuffer& msgBuf, ASN1T__SeqOfFireResultHuman& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE__SeqOfFireResultHuman (ASN1CTXT* ctxt_p, ASN1T__SeqOfFireResultHuman* pvalue);
EXTERN int asn1PD__SeqOfFireResultHuman (ASN1CTXT* ctxt_p, ASN1T__SeqOfFireResultHuman* pvalue);

EXTERN void asn1Print__SeqOfFireResultHuman (ASN1ConstCharPtr name, ASN1T__SeqOfFireResultHuman* pvalue);

/**************************************************************/
/*                                                            */
/*  _SeqOfFireResultEquipement                                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T__SeqOfFireResultEquipement {
   ASN1UINT n;
   ASN1T_FireResultEquipement *elem;
} ASN1T__SeqOfFireResultEquipement;

class EXTERN ASN1C__SeqOfFireResultEquipement : public ASN1CType {
public:
   ASN1T__SeqOfFireResultEquipement& msgData;
   ASN1C__SeqOfFireResultEquipement (
      ASN1MessageBuffer& msgBuf, ASN1T__SeqOfFireResultEquipement& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE__SeqOfFireResultEquipement (ASN1CTXT* ctxt_p, ASN1T__SeqOfFireResultEquipement* pvalue);
EXTERN int asn1PD__SeqOfFireResultEquipement (ASN1CTXT* ctxt_p, ASN1T__SeqOfFireResultEquipement* pvalue);

EXTERN void asn1Print__SeqOfFireResultEquipement (ASN1ConstCharPtr name, ASN1T__SeqOfFireResultEquipement* pvalue);

/**************************************************************/
/*                                                            */
/*  FireResult                                                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_FireResult {
   ASN1T_Agent  oid_cible;
   ASN1T__SeqOfFireResultHuman  resultats_humains;
   ASN1T__SeqOfFireResultEquipement  resultats_equipements;
} ASN1T_FireResult;

class EXTERN ASN1C_FireResult : public ASN1CType {
public:
   ASN1T_FireResult& msgData;
   ASN1C_FireResult (
      ASN1MessageBuffer& msgBuf, ASN1T_FireResult& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_FireResult (ASN1CTXT* ctxt_p, ASN1T_FireResult* pvalue);
EXTERN int asn1PD_FireResult (ASN1CTXT* ctxt_p, ASN1T_FireResult* pvalue);

EXTERN void asn1Print_FireResult (ASN1ConstCharPtr name, ASN1T_FireResult* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgStartDirectFire                                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgStartDirectFire {
   ASN1T_OID  oid_tir;
   ASN1T_Agent  oid_src;
   ASN1T_Agent  oid_dst;
} ASN1T_MsgStartDirectFire;

class EXTERN ASN1C_MsgStartDirectFire : public ASN1CType {
public:
   ASN1T_MsgStartDirectFire& msgData;
   ASN1C_MsgStartDirectFire (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgStartDirectFire& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgStartDirectFire (ASN1CTXT* ctxt_p, ASN1T_MsgStartDirectFire* pvalue);
EXTERN int asn1PD_MsgStartDirectFire (ASN1CTXT* ctxt_p, ASN1T_MsgStartDirectFire* pvalue);

EXTERN void asn1Print_MsgStartDirectFire (ASN1ConstCharPtr name, ASN1T_MsgStartDirectFire* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgStopDirectFire                                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgStopDirectFire {
   ASN1T_OID  oid_tir;
   ASN1T_FireResult  resultat;
} ASN1T_MsgStopDirectFire;

class EXTERN ASN1C_MsgStopDirectFire : public ASN1CType {
public:
   ASN1T_MsgStopDirectFire& msgData;
   ASN1C_MsgStopDirectFire (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgStopDirectFire& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgStopDirectFire (ASN1CTXT* ctxt_p, ASN1T_MsgStopDirectFire* pvalue);
EXTERN int asn1PD_MsgStopDirectFire (ASN1CTXT* ctxt_p, ASN1T_MsgStopDirectFire* pvalue);

EXTERN void asn1Print_MsgStopDirectFire (ASN1ConstCharPtr name, ASN1T_MsgStopDirectFire* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgStartIndirectFire                                      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgStartIndirectFire {
   ASN1T_OID  oid_tir;
   ASN1T_Agent  oid_src;
   ASN1T_CoordUTM  position;
} ASN1T_MsgStartIndirectFire;

class EXTERN ASN1C_MsgStartIndirectFire : public ASN1CType {
public:
   ASN1T_MsgStartIndirectFire& msgData;
   ASN1C_MsgStartIndirectFire (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgStartIndirectFire& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgStartIndirectFire (ASN1CTXT* ctxt_p, ASN1T_MsgStartIndirectFire* pvalue);
EXTERN int asn1PD_MsgStartIndirectFire (ASN1CTXT* ctxt_p, ASN1T_MsgStartIndirectFire* pvalue);

EXTERN void asn1Print_MsgStartIndirectFire (ASN1ConstCharPtr name, ASN1T_MsgStartIndirectFire* pvalue);

/**************************************************************/
/*                                                            */
/*  _SeqOfFireResult                                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T__SeqOfFireResult {
   ASN1UINT n;
   ASN1T_FireResult *elem;
} ASN1T__SeqOfFireResult;

class EXTERN ASN1C__SeqOfFireResult : public ASN1CType {
public:
   ASN1T__SeqOfFireResult& msgData;
   ASN1C__SeqOfFireResult (
      ASN1MessageBuffer& msgBuf, ASN1T__SeqOfFireResult& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE__SeqOfFireResult (ASN1CTXT* ctxt_p, ASN1T__SeqOfFireResult* pvalue);
EXTERN int asn1PD__SeqOfFireResult (ASN1CTXT* ctxt_p, ASN1T__SeqOfFireResult* pvalue);

EXTERN void asn1Print__SeqOfFireResult (ASN1ConstCharPtr name, ASN1T__SeqOfFireResult* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgStopIndirectFire                                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgStopIndirectFire {
   ASN1T_OID  oid_tir;
   ASN1T__SeqOfFireResult  resultats;
} ASN1T_MsgStopIndirectFire;

class EXTERN ASN1C_MsgStopIndirectFire : public ASN1CType {
public:
   ASN1T_MsgStopIndirectFire& msgData;
   ASN1C_MsgStopIndirectFire (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgStopIndirectFire& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgStopIndirectFire (ASN1CTXT* ctxt_p, ASN1T_MsgStopIndirectFire* pvalue);
EXTERN int asn1PD_MsgStopIndirectFire (ASN1CTXT* ctxt_p, ASN1T_MsgStopIndirectFire* pvalue);

EXTERN void asn1Print_MsgStopIndirectFire (ASN1ConstCharPtr name, ASN1T_MsgStopIndirectFire* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgExplosion                                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgExplosion {
   ASN1T_OID  oid_objet;
   ASN1T__SeqOfFireResult  resultats;
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

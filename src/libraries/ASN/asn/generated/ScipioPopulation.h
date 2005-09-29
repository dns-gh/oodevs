/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 29-Sep-2005.
 */
#ifndef SCIPIOPOPULATION_H
#define SCIPIOPOPULATION_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  MsgPopulationCreation                                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationCreation {
   ASN1T_OID  oid_population;
   ASN1T_TypePopulation  type_population;
   ASN1VisibleString  nom;
   ASN1T_OID  oid_camp;
} ASN1T_MsgPopulationCreation;

class EXTERN ASN1C_MsgPopulationCreation : public ASN1CType {
public:
   ASN1T_MsgPopulationCreation& msgData;
   ASN1C_MsgPopulationCreation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationCreation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPopulationCreation (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationCreation* pvalue);
EXTERN int asn1PD_MsgPopulationCreation (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationCreation* pvalue);

EXTERN void asn1Print_MsgPopulationCreation (ASN1ConstCharPtr name, ASN1T_MsgPopulationCreation* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPopulationUpdate                                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationUpdate {
   ASN1T_OID  oid_population;
} ASN1T_MsgPopulationUpdate;

class EXTERN ASN1C_MsgPopulationUpdate : public ASN1CType {
public:
   ASN1T_MsgPopulationUpdate& msgData;
   ASN1C_MsgPopulationUpdate (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationUpdate& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPopulationUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationUpdate* pvalue);
EXTERN int asn1PD_MsgPopulationUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationUpdate* pvalue);

EXTERN void asn1Print_MsgPopulationUpdate (ASN1ConstCharPtr name, ASN1T_MsgPopulationUpdate* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPopulationConcentrationCreation                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationConcentrationCreation {
   ASN1T_OID  oid_concentration;
   ASN1T_OID  oid_population;
} ASN1T_MsgPopulationConcentrationCreation;

class EXTERN ASN1C_MsgPopulationConcentrationCreation : public ASN1CType {
public:
   ASN1T_MsgPopulationConcentrationCreation& msgData;
   ASN1C_MsgPopulationConcentrationCreation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationConcentrationCreation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPopulationConcentrationCreation (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationConcentrationCreation* pvalue);
EXTERN int asn1PD_MsgPopulationConcentrationCreation (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationConcentrationCreation* pvalue);

EXTERN void asn1Print_MsgPopulationConcentrationCreation (ASN1ConstCharPtr name, ASN1T_MsgPopulationConcentrationCreation* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPopulationConcentrationDestruction                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationConcentrationDestruction {
   ASN1T_OID  oid_concentration;
   ASN1T_OID  oid_population;
} ASN1T_MsgPopulationConcentrationDestruction;

class EXTERN ASN1C_MsgPopulationConcentrationDestruction : public ASN1CType {
public:
   ASN1T_MsgPopulationConcentrationDestruction& msgData;
   ASN1C_MsgPopulationConcentrationDestruction (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationConcentrationDestruction& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPopulationConcentrationDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationConcentrationDestruction* pvalue);
EXTERN int asn1PD_MsgPopulationConcentrationDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationConcentrationDestruction* pvalue);

EXTERN void asn1Print_MsgPopulationConcentrationDestruction (ASN1ConstCharPtr name, ASN1T_MsgPopulationConcentrationDestruction* pvalue);

/**************************************************************/
/*                                                            */
/*  EnumPopulationAttitude                                    */
/*                                                            */
/**************************************************************/

struct EXTERN EnumPopulationAttitude {
   enum Root {
      calme = 0,
      agitee = 1,
      excitee = 2,
      agressive = 3
   } ;
} ;

typedef EnumPopulationAttitude::Root ASN1T_EnumPopulationAttitude;

class EXTERN ASN1C_EnumPopulationAttitude : public ASN1CType {
public:
   ASN1T_EnumPopulationAttitude& msgData;
   ASN1C_EnumPopulationAttitude (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumPopulationAttitude& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumPopulationAttitude (ASN1CTXT* ctxt_p, ASN1T_EnumPopulationAttitude value);
EXTERN int asn1PD_EnumPopulationAttitude (ASN1CTXT* ctxt_p, ASN1T_EnumPopulationAttitude* pvalue);

EXTERN void asn1Print_EnumPopulationAttitude (ASN1ConstCharPtr name, ASN1T_EnumPopulationAttitude* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPopulationConcentrationUpdate                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationConcentrationUpdate {
   struct {
      unsigned formePresent : 1;
      unsigned nb_humains_vivantsPresent : 1;
      unsigned nb_humains_mortsPresent : 1;
      unsigned attitudePresent : 1;
   } m;
   ASN1T_OID  oid_concentration;
   ASN1T_OID  oid_population;
   ASN1T_Localisation  forme;
   ASN1INT   nb_humains_vivants;
   ASN1INT   nb_humains_morts;
   ASN1T_EnumPopulationAttitude  attitude;

   ASN1T_MsgPopulationConcentrationUpdate () {
      m.formePresent = 0;
      m.nb_humains_vivantsPresent = 0;
      m.nb_humains_mortsPresent = 0;
      m.attitudePresent = 0;
   }
} ASN1T_MsgPopulationConcentrationUpdate;

class EXTERN ASN1C_MsgPopulationConcentrationUpdate : public ASN1CType {
public:
   ASN1T_MsgPopulationConcentrationUpdate& msgData;
   ASN1C_MsgPopulationConcentrationUpdate (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationConcentrationUpdate& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPopulationConcentrationUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationConcentrationUpdate* pvalue);
EXTERN int asn1PD_MsgPopulationConcentrationUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationConcentrationUpdate* pvalue);

EXTERN void asn1Print_MsgPopulationConcentrationUpdate (ASN1ConstCharPtr name, ASN1T_MsgPopulationConcentrationUpdate* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPopulationFluxCreation                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationFluxCreation {
   ASN1T_OID  oid_flux;
   ASN1T_OID  oid_population;
} ASN1T_MsgPopulationFluxCreation;

class EXTERN ASN1C_MsgPopulationFluxCreation : public ASN1CType {
public:
   ASN1T_MsgPopulationFluxCreation& msgData;
   ASN1C_MsgPopulationFluxCreation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationFluxCreation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPopulationFluxCreation (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationFluxCreation* pvalue);
EXTERN int asn1PD_MsgPopulationFluxCreation (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationFluxCreation* pvalue);

EXTERN void asn1Print_MsgPopulationFluxCreation (ASN1ConstCharPtr name, ASN1T_MsgPopulationFluxCreation* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPopulationFluxDestruction                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationFluxDestruction {
   ASN1T_OID  oid_flux;
   ASN1T_OID  oid_population;
} ASN1T_MsgPopulationFluxDestruction;

class EXTERN ASN1C_MsgPopulationFluxDestruction : public ASN1CType {
public:
   ASN1T_MsgPopulationFluxDestruction& msgData;
   ASN1C_MsgPopulationFluxDestruction (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationFluxDestruction& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPopulationFluxDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationFluxDestruction* pvalue);
EXTERN int asn1PD_MsgPopulationFluxDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationFluxDestruction* pvalue);

EXTERN void asn1Print_MsgPopulationFluxDestruction (ASN1ConstCharPtr name, ASN1T_MsgPopulationFluxDestruction* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPopulationFluxUpdate                                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationFluxUpdate {
   struct {
      unsigned itinerairePresent : 1;
      unsigned queuePresent : 1;
      unsigned tetePresent : 1;
      unsigned directionPresent : 1;
      unsigned vitessePresent : 1;
      unsigned nb_humains_vivantsPresent : 1;
      unsigned nb_humains_mortsPresent : 1;
      unsigned attitudePresent : 1;
   } m;
   ASN1T_OID  oid_flux;
   ASN1T_OID  oid_population;
   ASN1T_Itineraire  itineraire;
   ASN1T_CoordUTM  queue;
   ASN1T_CoordUTM  tete;
   ASN1T_Direction  direction;
   ASN1T_Vitesse  vitesse;
   ASN1INT   nb_humains_vivants;
   ASN1INT   nb_humains_morts;
   ASN1T_EnumPopulationAttitude  attitude;

   ASN1T_MsgPopulationFluxUpdate () {
      m.itinerairePresent = 0;
      m.queuePresent = 0;
      m.tetePresent = 0;
      m.directionPresent = 0;
      m.vitessePresent = 0;
      m.nb_humains_vivantsPresent = 0;
      m.nb_humains_mortsPresent = 0;
      m.attitudePresent = 0;
   }
} ASN1T_MsgPopulationFluxUpdate;

class EXTERN ASN1C_MsgPopulationFluxUpdate : public ASN1CType {
public:
   ASN1T_MsgPopulationFluxUpdate& msgData;
   ASN1C_MsgPopulationFluxUpdate (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationFluxUpdate& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPopulationFluxUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationFluxUpdate* pvalue);
EXTERN int asn1PD_MsgPopulationFluxUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationFluxUpdate* pvalue);

EXTERN void asn1Print_MsgPopulationFluxUpdate (ASN1ConstCharPtr name, ASN1T_MsgPopulationFluxUpdate* pvalue);

#endif

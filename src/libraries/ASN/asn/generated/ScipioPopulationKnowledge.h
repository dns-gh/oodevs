/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 13-Oct-2005.
 */
#ifndef SCIPIOPOPULATIONKNOWLEDGE_H
#define SCIPIOPOPULATIONKNOWLEDGE_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"
#include "ScipioPopulation.h"

/**************************************************************/
/*                                                            */
/*  MsgPopulationKnowledgeCreation                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationKnowledgeCreation {
   ASN1T_OID  oid_connaissance;
   ASN1T_OID  oid_groupe_possesseur;
   ASN1T_OID  oid_population_reelle;
   ASN1T_OID  camp;
} ASN1T_MsgPopulationKnowledgeCreation;

class EXTERN ASN1C_MsgPopulationKnowledgeCreation : public ASN1CType {
public:
   ASN1T_MsgPopulationKnowledgeCreation& msgData;
   ASN1C_MsgPopulationKnowledgeCreation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationKnowledgeCreation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPopulationKnowledgeCreation (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationKnowledgeCreation* pvalue);
EXTERN int asn1PD_MsgPopulationKnowledgeCreation (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationKnowledgeCreation* pvalue);

EXTERN void asn1Print_MsgPopulationKnowledgeCreation (ASN1ConstCharPtr name, ASN1T_MsgPopulationKnowledgeCreation* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPopulationKnowledgeUpdate                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationKnowledgeUpdate {
   ASN1T_OID  oid_connaissance;
   ASN1T_OID  oid_groupe_possesseur;
} ASN1T_MsgPopulationKnowledgeUpdate;

class EXTERN ASN1C_MsgPopulationKnowledgeUpdate : public ASN1CType {
public:
   ASN1T_MsgPopulationKnowledgeUpdate& msgData;
   ASN1C_MsgPopulationKnowledgeUpdate (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationKnowledgeUpdate& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPopulationKnowledgeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationKnowledgeUpdate* pvalue);
EXTERN int asn1PD_MsgPopulationKnowledgeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationKnowledgeUpdate* pvalue);

EXTERN void asn1Print_MsgPopulationKnowledgeUpdate (ASN1ConstCharPtr name, ASN1T_MsgPopulationKnowledgeUpdate* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPopulationKnowledgeDestruction                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationKnowledgeDestruction {
   ASN1T_OID  oid_connaissance;
   ASN1T_OID  oid_groupe_possesseur;
} ASN1T_MsgPopulationKnowledgeDestruction;

class EXTERN ASN1C_MsgPopulationKnowledgeDestruction : public ASN1CType {
public:
   ASN1T_MsgPopulationKnowledgeDestruction& msgData;
   ASN1C_MsgPopulationKnowledgeDestruction (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationKnowledgeDestruction& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPopulationKnowledgeDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationKnowledgeDestruction* pvalue);
EXTERN int asn1PD_MsgPopulationKnowledgeDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationKnowledgeDestruction* pvalue);

EXTERN void asn1Print_MsgPopulationKnowledgeDestruction (ASN1ConstCharPtr name, ASN1T_MsgPopulationKnowledgeDestruction* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPopulationConcentrationKnowledgeCreation               */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationConcentrationKnowledgeCreation {
   ASN1T_OID  oid_connaissance_concentration;
   ASN1T_OID  oid_connaissance_population;
   ASN1T_OID  oid_groupe_possesseur;
   ASN1T_OID  oid_concentration_reelle;
   ASN1T_CoordUTM  position;
} ASN1T_MsgPopulationConcentrationKnowledgeCreation;

class EXTERN ASN1C_MsgPopulationConcentrationKnowledgeCreation : public ASN1CType {
public:
   ASN1T_MsgPopulationConcentrationKnowledgeCreation& msgData;
   ASN1C_MsgPopulationConcentrationKnowledgeCreation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationConcentrationKnowledgeCreation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPopulationConcentrationKnowledgeCreation (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationConcentrationKnowledgeCreation* pvalue);
EXTERN int asn1PD_MsgPopulationConcentrationKnowledgeCreation (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationConcentrationKnowledgeCreation* pvalue);

EXTERN void asn1Print_MsgPopulationConcentrationKnowledgeCreation (ASN1ConstCharPtr name, ASN1T_MsgPopulationConcentrationKnowledgeCreation* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPopulationConcentrationKnowledgeDestruction            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationConcentrationKnowledgeDestruction {
   ASN1T_OID  oid_connaissance_concentration;
   ASN1T_OID  oid_connaissance_population;
   ASN1T_OID  oid_groupe_possesseur;
} ASN1T_MsgPopulationConcentrationKnowledgeDestruction;

class EXTERN ASN1C_MsgPopulationConcentrationKnowledgeDestruction : public ASN1CType {
public:
   ASN1T_MsgPopulationConcentrationKnowledgeDestruction& msgData;
   ASN1C_MsgPopulationConcentrationKnowledgeDestruction (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationConcentrationKnowledgeDestruction& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPopulationConcentrationKnowledgeDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationConcentrationKnowledgeDestruction* pvalue);
EXTERN int asn1PD_MsgPopulationConcentrationKnowledgeDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationConcentrationKnowledgeDestruction* pvalue);

EXTERN void asn1Print_MsgPopulationConcentrationKnowledgeDestruction (ASN1ConstCharPtr name, ASN1T_MsgPopulationConcentrationKnowledgeDestruction* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPopulationConcentrationKnowledgeUpdate                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationConcentrationKnowledgeUpdate {
   struct {
      unsigned oid_concentration_reellePresent : 1;
      unsigned nb_humains_vivantsPresent : 1;
      unsigned nb_humains_mortsPresent : 1;
      unsigned attitudePresent : 1;
      unsigned pertinencePresent : 1;
      unsigned est_percuPresent : 1;
   } m;
   ASN1T_OID  oid_connaissance_concentration;
   ASN1T_OID  oid_connaissance_population;
   ASN1T_OID  oid_groupe_possesseur;
   ASN1T_OID  oid_concentration_reelle;
   ASN1INT   nb_humains_vivants;
   ASN1INT   nb_humains_morts;
   ASN1T_EnumPopulationAttitude  attitude;
   ASN1T_Pourcentage  pertinence;
   ASN1BOOL  est_percu;

   ASN1T_MsgPopulationConcentrationKnowledgeUpdate () {
      m.oid_concentration_reellePresent = 0;
      m.nb_humains_vivantsPresent = 0;
      m.nb_humains_mortsPresent = 0;
      m.attitudePresent = 0;
      m.pertinencePresent = 0;
      m.est_percuPresent = 0;
   }
} ASN1T_MsgPopulationConcentrationKnowledgeUpdate;

class EXTERN ASN1C_MsgPopulationConcentrationKnowledgeUpdate : public ASN1CType {
public:
   ASN1T_MsgPopulationConcentrationKnowledgeUpdate& msgData;
   ASN1C_MsgPopulationConcentrationKnowledgeUpdate (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationConcentrationKnowledgeUpdate& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPopulationConcentrationKnowledgeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationConcentrationKnowledgeUpdate* pvalue);
EXTERN int asn1PD_MsgPopulationConcentrationKnowledgeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationConcentrationKnowledgeUpdate* pvalue);

EXTERN void asn1Print_MsgPopulationConcentrationKnowledgeUpdate (ASN1ConstCharPtr name, ASN1T_MsgPopulationConcentrationKnowledgeUpdate* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPopulationFluxKnowledgeCreation                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationFluxKnowledgeCreation {
   ASN1T_OID  oid_connaissance_flux;
   ASN1T_OID  oid_connaissance_population;
   ASN1T_OID  oid_groupe_possesseur;
} ASN1T_MsgPopulationFluxKnowledgeCreation;

class EXTERN ASN1C_MsgPopulationFluxKnowledgeCreation : public ASN1CType {
public:
   ASN1T_MsgPopulationFluxKnowledgeCreation& msgData;
   ASN1C_MsgPopulationFluxKnowledgeCreation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationFluxKnowledgeCreation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPopulationFluxKnowledgeCreation (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationFluxKnowledgeCreation* pvalue);
EXTERN int asn1PD_MsgPopulationFluxKnowledgeCreation (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationFluxKnowledgeCreation* pvalue);

EXTERN void asn1Print_MsgPopulationFluxKnowledgeCreation (ASN1ConstCharPtr name, ASN1T_MsgPopulationFluxKnowledgeCreation* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPopulationFluxKnowledgeDestruction                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationFluxKnowledgeDestruction {
   ASN1T_OID  oid_connaissance_flux;
   ASN1T_OID  oid_connaissance_population;
   ASN1T_OID  oid_groupe_possesseur;
} ASN1T_MsgPopulationFluxKnowledgeDestruction;

class EXTERN ASN1C_MsgPopulationFluxKnowledgeDestruction : public ASN1CType {
public:
   ASN1T_MsgPopulationFluxKnowledgeDestruction& msgData;
   ASN1C_MsgPopulationFluxKnowledgeDestruction (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationFluxKnowledgeDestruction& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPopulationFluxKnowledgeDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationFluxKnowledgeDestruction* pvalue);
EXTERN int asn1PD_MsgPopulationFluxKnowledgeDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationFluxKnowledgeDestruction* pvalue);

EXTERN void asn1Print_MsgPopulationFluxKnowledgeDestruction (ASN1ConstCharPtr name, ASN1T_MsgPopulationFluxKnowledgeDestruction* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPopulationFluxKnowledgeUpdate                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationFluxKnowledgeUpdate {
   struct {
      unsigned fluxPresent : 1;
      unsigned directionPresent : 1;
      unsigned vitessePresent : 1;
      unsigned nb_humains_vivantsPresent : 1;
      unsigned nb_humains_mortsPresent : 1;
      unsigned attitudePresent : 1;
      unsigned pertinencePresent : 1;
      unsigned est_percuPresent : 1;
   } m;
   ASN1T_OID  oid_connaissance_flux;
   ASN1T_OID  oid_connaissance_population;
   ASN1T_OID  oid_groupe_possesseur;
   ASN1T_Itineraire  flux;
   ASN1T_Direction  direction;
   ASN1T_Vitesse  vitesse;
   ASN1INT   nb_humains_vivants;
   ASN1INT   nb_humains_morts;
   ASN1T_EnumPopulationAttitude  attitude;
   ASN1T_Pourcentage  pertinence;
   ASN1BOOL  est_percu;

   ASN1T_MsgPopulationFluxKnowledgeUpdate () {
      m.fluxPresent = 0;
      m.directionPresent = 0;
      m.vitessePresent = 0;
      m.nb_humains_vivantsPresent = 0;
      m.nb_humains_mortsPresent = 0;
      m.attitudePresent = 0;
      m.pertinencePresent = 0;
      m.est_percuPresent = 0;
   }
} ASN1T_MsgPopulationFluxKnowledgeUpdate;

class EXTERN ASN1C_MsgPopulationFluxKnowledgeUpdate : public ASN1CType {
public:
   ASN1T_MsgPopulationFluxKnowledgeUpdate& msgData;
   ASN1C_MsgPopulationFluxKnowledgeUpdate (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationFluxKnowledgeUpdate& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPopulationFluxKnowledgeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationFluxKnowledgeUpdate* pvalue);
EXTERN int asn1PD_MsgPopulationFluxKnowledgeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationFluxKnowledgeUpdate* pvalue);

EXTERN void asn1Print_MsgPopulationFluxKnowledgeUpdate (ASN1ConstCharPtr name, ASN1T_MsgPopulationFluxKnowledgeUpdate* pvalue);

#endif

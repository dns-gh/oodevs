/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 24-Dec-2009.
 */
#ifndef POPULATIONKNOWLEDGE_H
#define POPULATIONKNOWLEDGE_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "Base.h"
#include "Population.h"

/**************************************************************/
/*                                                            */
/*  PopulationKnowledgeIncludes                               */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_PopulationKnowledgeIncludes {
} ASN1T_PopulationKnowledgeIncludes;

class EXTERN ASN1C_PopulationKnowledgeIncludes : public ASN1CType {
public:
   ASN1T_PopulationKnowledgeIncludes& msgData;
   ASN1C_PopulationKnowledgeIncludes (
      ASN1MessageBuffer& msgBuf, ASN1T_PopulationKnowledgeIncludes& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_PopulationKnowledgeIncludes (ASN1CTXT* ctxt_p, ASN1T_PopulationKnowledgeIncludes* pvalue);
EXTERN int asn1PD_PopulationKnowledgeIncludes (ASN1CTXT* ctxt_p, ASN1T_PopulationKnowledgeIncludes* pvalue);

EXTERN void asn1Print_PopulationKnowledgeIncludes (ASN1ConstCharPtr name, ASN1T_PopulationKnowledgeIncludes* pvalue);

/**************************************************************/
/*                                                            */
/*  PopulationKnowledgeExports                                */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_PopulationKnowledgeExports : public ASN1CType {
public:
   ASN1C_PopulationKnowledgeExports (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_PopulationKnowledgeExports (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_PopulationKnowledgeExports (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_PopulationKnowledgeExports (ASN1ConstCharPtr name);

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
   struct {
      unsigned etat_dominationPresent : 1;
   } m;
   ASN1T_OID  oid_connaissance;
   ASN1T_OID  oid_groupe_possesseur;
   ASN1T_Percentage  etat_domination;

   ASN1T_MsgPopulationKnowledgeUpdate () {
      m.etat_dominationPresent = 0;
   }
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
   ASN1T_CoordLatLong  position;
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
   ASN1T_Percentage  pertinence;
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
/*  MsgPopulationFlowKnowledgeCreation                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationFlowKnowledgeCreation {
   ASN1T_OID  oid_connaissance_flux;
   ASN1T_OID  oid_connaissance_population;
   ASN1T_OID  oid_groupe_possesseur;
   ASN1T_OID  oid_flux_reel;
} ASN1T_MsgPopulationFlowKnowledgeCreation;

class EXTERN ASN1C_MsgPopulationFlowKnowledgeCreation : public ASN1CType {
public:
   ASN1T_MsgPopulationFlowKnowledgeCreation& msgData;
   ASN1C_MsgPopulationFlowKnowledgeCreation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationFlowKnowledgeCreation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPopulationFlowKnowledgeCreation (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationFlowKnowledgeCreation* pvalue);
EXTERN int asn1PD_MsgPopulationFlowKnowledgeCreation (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationFlowKnowledgeCreation* pvalue);

EXTERN void asn1Print_MsgPopulationFlowKnowledgeCreation (ASN1ConstCharPtr name, ASN1T_MsgPopulationFlowKnowledgeCreation* pvalue);

/**************************************************************/
/*                                                            */
/*  FlowPart                                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_FlowPart {
   ASN1T_Path  forme;
   ASN1T_Percentage  pertinence;
} ASN1T_FlowPart;

class EXTERN ASN1C_FlowPart : public ASN1CType {
public:
   ASN1T_FlowPart& msgData;
   ASN1C_FlowPart (
      ASN1MessageBuffer& msgBuf, ASN1T_FlowPart& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_FlowPart (ASN1CTXT* ctxt_p, ASN1T_FlowPart* pvalue);
EXTERN int asn1PD_FlowPart (ASN1CTXT* ctxt_p, ASN1T_FlowPart* pvalue);

EXTERN void asn1Print_FlowPart (ASN1ConstCharPtr name, ASN1T_FlowPart* pvalue);

/**************************************************************/
/*                                                            */
/*  _SeqOfFlowPart                                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T__SeqOfFlowPart {
   ASN1UINT n;
   ASN1T_FlowPart *elem;
} ASN1T__SeqOfFlowPart;

class EXTERN ASN1C__SeqOfFlowPart : public ASN1CType {
public:
   ASN1T__SeqOfFlowPart& msgData;
   ASN1C__SeqOfFlowPart (
      ASN1MessageBuffer& msgBuf, ASN1T__SeqOfFlowPart& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE__SeqOfFlowPart (ASN1CTXT* ctxt_p, ASN1T__SeqOfFlowPart* pvalue);
EXTERN int asn1PD__SeqOfFlowPart (ASN1CTXT* ctxt_p, ASN1T__SeqOfFlowPart* pvalue);

EXTERN void asn1Print__SeqOfFlowPart (ASN1ConstCharPtr name, ASN1T__SeqOfFlowPart* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPopulationFlowKnowledgeUpdate                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationFlowKnowledgeUpdate {
   struct {
      unsigned oid_flux_reelPresent : 1;
      unsigned portions_fluxPresent : 1;
      unsigned directionPresent : 1;
      unsigned vitessePresent : 1;
      unsigned nb_humains_vivantsPresent : 1;
      unsigned nb_humains_mortsPresent : 1;
      unsigned attitudePresent : 1;
      unsigned est_percuPresent : 1;
   } m;
   ASN1T_OID  oid_connaissance_flux;
   ASN1T_OID  oid_connaissance_population;
   ASN1T_OID  oid_groupe_possesseur;
   ASN1T_OID  oid_flux_reel;
   ASN1T__SeqOfFlowPart  portions_flux;
   ASN1T_Heading  direction;
   ASN1T_Speed  vitesse;
   ASN1INT   nb_humains_vivants;
   ASN1INT   nb_humains_morts;
   ASN1T_EnumPopulationAttitude  attitude;
   ASN1BOOL  est_percu;

   ASN1T_MsgPopulationFlowKnowledgeUpdate () {
      m.oid_flux_reelPresent = 0;
      m.portions_fluxPresent = 0;
      m.directionPresent = 0;
      m.vitessePresent = 0;
      m.nb_humains_vivantsPresent = 0;
      m.nb_humains_mortsPresent = 0;
      m.attitudePresent = 0;
      m.est_percuPresent = 0;
   }
} ASN1T_MsgPopulationFlowKnowledgeUpdate;

class EXTERN ASN1C_MsgPopulationFlowKnowledgeUpdate : public ASN1CType {
public:
   ASN1T_MsgPopulationFlowKnowledgeUpdate& msgData;
   ASN1C_MsgPopulationFlowKnowledgeUpdate (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationFlowKnowledgeUpdate& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPopulationFlowKnowledgeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationFlowKnowledgeUpdate* pvalue);
EXTERN int asn1PD_MsgPopulationFlowKnowledgeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationFlowKnowledgeUpdate* pvalue);

EXTERN void asn1Print_MsgPopulationFlowKnowledgeUpdate (ASN1ConstCharPtr name, ASN1T_MsgPopulationFlowKnowledgeUpdate* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPopulationFlowKnowledgeDestruction                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationFlowKnowledgeDestruction {
   ASN1T_OID  oid_connaissance_flux;
   ASN1T_OID  oid_connaissance_population;
   ASN1T_OID  oid_groupe_possesseur;
} ASN1T_MsgPopulationFlowKnowledgeDestruction;

class EXTERN ASN1C_MsgPopulationFlowKnowledgeDestruction : public ASN1CType {
public:
   ASN1T_MsgPopulationFlowKnowledgeDestruction& msgData;
   ASN1C_MsgPopulationFlowKnowledgeDestruction (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationFlowKnowledgeDestruction& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPopulationFlowKnowledgeDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationFlowKnowledgeDestruction* pvalue);
EXTERN int asn1PD_MsgPopulationFlowKnowledgeDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationFlowKnowledgeDestruction* pvalue);

EXTERN void asn1Print_MsgPopulationFlowKnowledgeDestruction (ASN1ConstCharPtr name, ASN1T_MsgPopulationFlowKnowledgeDestruction* pvalue);

#endif

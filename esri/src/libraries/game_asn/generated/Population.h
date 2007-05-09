/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 01-Mar-2007.
 */
#ifndef POPULATION_H
#define POPULATION_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "Base.h"

/**************************************************************/
/*                                                            */
/*  PopulationIncludes                                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_PopulationIncludes {
} ASN1T_PopulationIncludes;

class EXTERN ASN1C_PopulationIncludes : public ASN1CType {
public:
   ASN1T_PopulationIncludes& msgData;
   ASN1C_PopulationIncludes (
      ASN1MessageBuffer& msgBuf, ASN1T_PopulationIncludes& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_PopulationIncludes (ASN1CTXT* ctxt_p, ASN1T_PopulationIncludes* pvalue);
EXTERN int asn1PD_PopulationIncludes (ASN1CTXT* ctxt_p, ASN1T_PopulationIncludes* pvalue);

EXTERN void asn1Print_PopulationIncludes (ASN1ConstCharPtr name, ASN1T_PopulationIncludes* pvalue);

/**************************************************************/
/*                                                            */
/*  PopulationExports                                         */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_PopulationExports : public ASN1CType {
public:
   ASN1C_PopulationExports (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_PopulationExports (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_PopulationExports (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_PopulationExports (ASN1ConstCharPtr name);

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
   struct {
      unsigned etat_dominationPresent : 1;
   } m;
   ASN1T_OID  oid_population;
   ASN1T_Pourcentage  etat_domination;

   ASN1T_MsgPopulationUpdate () {
      m.etat_dominationPresent = 0;
   }
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
   ASN1T_CoordUTM  position;
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
/*  MsgPopulationConcentrationUpdate                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationConcentrationUpdate {
   struct {
      unsigned nb_humains_vivantsPresent : 1;
      unsigned nb_humains_mortsPresent : 1;
      unsigned attitudePresent : 1;
   } m;
   ASN1T_OID  oid_concentration;
   ASN1T_OID  oid_population;
   ASN1INT   nb_humains_vivants;
   ASN1INT   nb_humains_morts;
   ASN1T_EnumPopulationAttitude  attitude;

   ASN1T_MsgPopulationConcentrationUpdate () {
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
/*  MsgPopulationFluxUpdate                                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationFluxUpdate {
   struct {
      unsigned itinerairePresent : 1;
      unsigned fluxPresent : 1;
      unsigned directionPresent : 1;
      unsigned vitessePresent : 1;
      unsigned nb_humains_vivantsPresent : 1;
      unsigned nb_humains_mortsPresent : 1;
      unsigned attitudePresent : 1;
   } m;
   ASN1T_OID  oid_flux;
   ASN1T_OID  oid_population;
   ASN1T_Itineraire  itineraire;
   ASN1T_Itineraire  flux;
   ASN1T_Direction  direction;
   ASN1T_Vitesse  vitesse;
   ASN1INT   nb_humains_vivants;
   ASN1INT   nb_humains_morts;
   ASN1T_EnumPopulationAttitude  attitude;

   ASN1T_MsgPopulationFluxUpdate () {
      m.itinerairePresent = 0;
      m.fluxPresent = 0;
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
/*  MagicActionPopulationChangeAttitude_beneficiaire          */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_MagicActionPopulationChangeAttitude_beneficiaire_flux 1
#define T_MagicActionPopulationChangeAttitude_beneficiaire_concentration 2
#define T_MagicActionPopulationChangeAttitude_beneficiaire_global 3

typedef struct EXTERN ASN1T_MagicActionPopulationChangeAttitude_beneficiaire {
   int t;
   union {
      /* t = 1 */
      ASN1T_OID  flux;
      /* t = 2 */
      ASN1T_OID  concentration;
      /* t = 3 */
   } u;
} ASN1T_MagicActionPopulationChangeAttitude_beneficiaire;

class EXTERN ASN1C_MagicActionPopulationChangeAttitude_beneficiaire : public ASN1CType {
public:
   ASN1T_MagicActionPopulationChangeAttitude_beneficiaire& msgData;
   ASN1C_MagicActionPopulationChangeAttitude_beneficiaire (
      ASN1MessageBuffer& msgBuf, ASN1T_MagicActionPopulationChangeAttitude_beneficiaire& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MagicActionPopulationChangeAttitude_beneficiaire (ASN1CTXT* ctxt_p, ASN1T_MagicActionPopulationChangeAttitude_beneficiaire* pvalue);
EXTERN int asn1PD_MagicActionPopulationChangeAttitude_beneficiaire (ASN1CTXT* ctxt_p, ASN1T_MagicActionPopulationChangeAttitude_beneficiaire* pvalue);

EXTERN void asn1Print_MagicActionPopulationChangeAttitude_beneficiaire (ASN1ConstCharPtr name, ASN1T_MagicActionPopulationChangeAttitude_beneficiaire* pvalue);

/**************************************************************/
/*                                                            */
/*  MagicActionPopulationChangeAttitude                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MagicActionPopulationChangeAttitude {
   ASN1T_EnumPopulationAttitude  attitude;
   ASN1T_MagicActionPopulationChangeAttitude_beneficiaire  beneficiaire;
} ASN1T_MagicActionPopulationChangeAttitude;

class EXTERN ASN1C_MagicActionPopulationChangeAttitude : public ASN1CType {
public:
   ASN1T_MagicActionPopulationChangeAttitude& msgData;
   ASN1C_MagicActionPopulationChangeAttitude (
      ASN1MessageBuffer& msgBuf, ASN1T_MagicActionPopulationChangeAttitude& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MagicActionPopulationChangeAttitude (ASN1CTXT* ctxt_p, ASN1T_MagicActionPopulationChangeAttitude* pvalue);
EXTERN int asn1PD_MagicActionPopulationChangeAttitude (ASN1CTXT* ctxt_p, ASN1T_MagicActionPopulationChangeAttitude* pvalue);

EXTERN void asn1Print_MagicActionPopulationChangeAttitude (ASN1ConstCharPtr name, ASN1T_MagicActionPopulationChangeAttitude* pvalue);

/**************************************************************/
/*                                                            */
/*  MagicActionPopulationMoveTo                               */
/*                                                            */
/**************************************************************/

typedef ASN1T_CoordUTM ASN1T_MagicActionPopulationMoveTo;

class EXTERN ASN1C_MagicActionPopulationMoveTo : public ASN1CType {
public:
   ASN1T_MagicActionPopulationMoveTo& msgData;
   ASN1C_MagicActionPopulationMoveTo (
      ASN1MessageBuffer& msgBuf, ASN1T_MagicActionPopulationMoveTo& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MagicActionPopulationMoveTo (ASN1CTXT* ctxt_p, ASN1T_MagicActionPopulationMoveTo value);
EXTERN int asn1PD_MagicActionPopulationMoveTo (ASN1CTXT* ctxt_p, ASN1T_MagicActionPopulationMoveTo* pvalue);

EXTERN void asn1Print_MagicActionPopulationMoveTo (ASN1ConstCharPtr name, ASN1T_MagicActionPopulationMoveTo* pvalue);

/**************************************************************/
/*                                                            */
/*  MagicActionPopulationDestructionTotale                    */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MagicActionPopulationDestructionTotale : public ASN1CType {
public:
   ASN1C_MagicActionPopulationDestructionTotale (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MagicActionPopulationDestructionTotale (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MagicActionPopulationDestructionTotale (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MagicActionPopulationDestructionTotale (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MagicActionPopulationTuer                                 */
/*                                                            */
/**************************************************************/

typedef ASN1INT  ASN1T_MagicActionPopulationTuer;

class EXTERN ASN1C_MagicActionPopulationTuer : public ASN1CType {
public:
   ASN1T_MagicActionPopulationTuer& msgData;
   ASN1C_MagicActionPopulationTuer (
      ASN1MessageBuffer& msgBuf, ASN1T_MagicActionPopulationTuer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MagicActionPopulationTuer (ASN1CTXT* ctxt_p, ASN1T_MagicActionPopulationTuer value);
EXTERN int asn1PD_MagicActionPopulationTuer (ASN1CTXT* ctxt_p, ASN1T_MagicActionPopulationTuer* pvalue);

EXTERN void asn1Print_MagicActionPopulationTuer (ASN1ConstCharPtr name, ASN1T_MagicActionPopulationTuer* pvalue);

/**************************************************************/
/*                                                            */
/*  MagicActionPopulationRessusciter                          */
/*                                                            */
/**************************************************************/

typedef ASN1INT  ASN1T_MagicActionPopulationRessusciter;

class EXTERN ASN1C_MagicActionPopulationRessusciter : public ASN1CType {
public:
   ASN1T_MagicActionPopulationRessusciter& msgData;
   ASN1C_MagicActionPopulationRessusciter (
      ASN1MessageBuffer& msgBuf, ASN1T_MagicActionPopulationRessusciter& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MagicActionPopulationRessusciter (ASN1CTXT* ctxt_p, ASN1T_MagicActionPopulationRessusciter value);
EXTERN int asn1PD_MagicActionPopulationRessusciter (ASN1CTXT* ctxt_p, ASN1T_MagicActionPopulationRessusciter* pvalue);

EXTERN void asn1Print_MagicActionPopulationRessusciter (ASN1ConstCharPtr name, ASN1T_MagicActionPopulationRessusciter* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPopulationMagicAction_action                           */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_MsgPopulationMagicAction_action_change_attitude 1
#define T_MsgPopulationMagicAction_action_move_to 2
#define T_MsgPopulationMagicAction_action_destruction_totale 3
#define T_MsgPopulationMagicAction_action_tuer 4
#define T_MsgPopulationMagicAction_action_ressusciter 5

typedef struct EXTERN ASN1T_MsgPopulationMagicAction_action {
   int t;
   union {
      /* t = 1 */
      ASN1T_MagicActionPopulationChangeAttitude *change_attitude;
      /* t = 2 */
      ASN1T_MagicActionPopulationMoveTo *move_to;
      /* t = 3 */
      /* t = 4 */
      ASN1T_MagicActionPopulationTuer  tuer;
      /* t = 5 */
      ASN1T_MagicActionPopulationRessusciter  ressusciter;
   } u;
} ASN1T_MsgPopulationMagicAction_action;

class EXTERN ASN1C_MsgPopulationMagicAction_action : public ASN1CType {
public:
   ASN1T_MsgPopulationMagicAction_action& msgData;
   ASN1C_MsgPopulationMagicAction_action (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationMagicAction_action& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPopulationMagicAction_action (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationMagicAction_action* pvalue);
EXTERN int asn1PD_MsgPopulationMagicAction_action (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationMagicAction_action* pvalue);

EXTERN void asn1Print_MsgPopulationMagicAction_action (ASN1ConstCharPtr name, ASN1T_MsgPopulationMagicAction_action* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPopulationMagicAction                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationMagicAction {
   ASN1T_OID  oid;
   ASN1T_MsgPopulationMagicAction_action  action;
} ASN1T_MsgPopulationMagicAction;

class EXTERN ASN1C_MsgPopulationMagicAction : public ASN1CType {
public:
   ASN1T_MsgPopulationMagicAction& msgData;
   ASN1C_MsgPopulationMagicAction (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationMagicAction& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPopulationMagicAction (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationMagicAction* pvalue);
EXTERN int asn1PD_MsgPopulationMagicAction (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationMagicAction* pvalue);

EXTERN void asn1Print_MsgPopulationMagicAction (ASN1ConstCharPtr name, ASN1T_MsgPopulationMagicAction* pvalue);

/**************************************************************/
/*                                                            */
/*  EnumPopulationAttrErrorCode                               */
/*                                                            */
/**************************************************************/

struct EXTERN EnumPopulationAttrErrorCode {
   enum Root {
      no_error = 0,
      error_invalid_unit = 1,
      error_invalid_attribute = 2
   } ;
} ;

typedef EnumPopulationAttrErrorCode::Root ASN1T_EnumPopulationAttrErrorCode;

class EXTERN ASN1C_EnumPopulationAttrErrorCode : public ASN1CType {
public:
   ASN1T_EnumPopulationAttrErrorCode& msgData;
   ASN1C_EnumPopulationAttrErrorCode (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumPopulationAttrErrorCode& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumPopulationAttrErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumPopulationAttrErrorCode value);
EXTERN int asn1PD_EnumPopulationAttrErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumPopulationAttrErrorCode* pvalue);

EXTERN void asn1Print_EnumPopulationAttrErrorCode (ASN1ConstCharPtr name, ASN1T_EnumPopulationAttrErrorCode* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPopulationMagicActionAck                               */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPopulationMagicActionAck {
   ASN1T_OID  oid;
   ASN1T_EnumPopulationAttrErrorCode  error_code;
} ASN1T_MsgPopulationMagicActionAck;

class EXTERN ASN1C_MsgPopulationMagicActionAck : public ASN1CType {
public:
   ASN1T_MsgPopulationMagicActionAck& msgData;
   ASN1C_MsgPopulationMagicActionAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationMagicActionAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPopulationMagicActionAck (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationMagicActionAck* pvalue);
EXTERN int asn1PD_MsgPopulationMagicActionAck (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationMagicActionAck* pvalue);

EXTERN void asn1Print_MsgPopulationMagicActionAck (ASN1ConstCharPtr name, ASN1T_MsgPopulationMagicActionAck* pvalue);

#endif

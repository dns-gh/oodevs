/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 05-Aug-2005.
 */
#ifndef SCIPIOOBJECT_H
#define SCIPIOOBJECT_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  EnumMissionGenUrgence                                     */
/*                                                            */
/**************************************************************/

struct EXTERN EnumMissionGenUrgence {
   enum Root {
      serie_minimum = 0,
      serie_complementaire = 1
   } ;
} ;

typedef EnumMissionGenUrgence::Root ASN1T_EnumMissionGenUrgence;

class EXTERN ASN1C_EnumMissionGenUrgence : public ASN1CType {
public:
   ASN1T_EnumMissionGenUrgence& msgData;
   ASN1C_EnumMissionGenUrgence (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumMissionGenUrgence& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumMissionGenUrgence (ASN1CTXT* ctxt_p, ASN1T_EnumMissionGenUrgence value);
EXTERN int asn1PD_EnumMissionGenUrgence (ASN1CTXT* ctxt_p, ASN1T_EnumMissionGenUrgence* pvalue);

EXTERN void asn1Print_EnumMissionGenUrgence (ASN1ConstCharPtr name, ASN1T_EnumMissionGenUrgence* pvalue);

/**************************************************************/
/*                                                            */
/*  EnumMissionGenSousTypeObstacle                            */
/*                                                            */
/**************************************************************/

struct EXTERN EnumMissionGenSousTypeObstacle {
   enum Root {
      preliminaire = 0,
      de_manoeuvre = 1
   } ;
} ;

typedef EnumMissionGenSousTypeObstacle::Root ASN1T_EnumMissionGenSousTypeObstacle;

class EXTERN ASN1C_EnumMissionGenSousTypeObstacle : public ASN1CType {
public:
   ASN1T_EnumMissionGenSousTypeObstacle& msgData;
   ASN1C_EnumMissionGenSousTypeObstacle (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumMissionGenSousTypeObstacle& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumMissionGenSousTypeObstacle (ASN1CTXT* ctxt_p, ASN1T_EnumMissionGenSousTypeObstacle value);
EXTERN int asn1PD_EnumMissionGenSousTypeObstacle (ASN1CTXT* ctxt_p, ASN1T_EnumMissionGenSousTypeObstacle* pvalue);

EXTERN void asn1Print_EnumMissionGenSousTypeObstacle (ASN1ConstCharPtr name, ASN1T_EnumMissionGenSousTypeObstacle* pvalue);

/**************************************************************/
/*                                                            */
/*  EnumObjectType                                            */
/*                                                            */
/**************************************************************/

struct EXTERN EnumObjectType {
   enum Root {
      bouchon_mines = 0,
      zone_minee_lineaire = 1,
      zone_minee_par_dispersion = 2,
      fosse_anti_char = 3,
      abattis = 4,
      barricade = 5,
      eboulement = 6,
      destruction_route = 7,
      destruction_pont = 8,
      pont_flottant = 9,
      poste_tir = 10,
      zone_protegee = 11,
      zone_implantation_canon = 12,
      zone_implantation_cobra = 13,
      zone_implantation_lrm = 14,
      site_franchissement = 15,
      nuage_nbc = 16,
      plot_ravitaillement = 17,
      site_decontamination = 18,
      zone_brouillage_brod = 19,
      rota = 20,
      zone_nbc = 21,
      zone_brouillage_bromure = 22,
      aire_poser = 23,
      piste = 24,
      plateforme = 25,
      zone_mobilite_amelioree = 26,
      zone_poser_helicoptere = 27,
      aire_logistique = 28,
      itineraire_logistique = 29,
      camp_prisonniers = 30,
      camp_refugies = 31,
      poste_controle = 32,
      terrain_largage = 33,
      zone_interdite_mouvement = 34,
      zone_interdite_tir = 35,
      zone_implantation_mortier = 36,
      pont_flottant_discontinu = 37
   } ;
} ;

typedef EnumObjectType::Root ASN1T_EnumObjectType;

class EXTERN ASN1C_EnumObjectType : public ASN1CType {
public:
   ASN1T_EnumObjectType& msgData;
   ASN1C_EnumObjectType (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumObjectType& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumObjectType (ASN1CTXT* ctxt_p, ASN1T_EnumObjectType value);
EXTERN int asn1PD_EnumObjectType (ASN1CTXT* ctxt_p, ASN1T_EnumObjectType* pvalue);

EXTERN void asn1Print_EnumObjectType (ASN1ConstCharPtr name, ASN1T_EnumObjectType* pvalue);

/**************************************************************/
/*                                                            */
/*  AttrObjectSiteFranchissement                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_AttrObjectSiteFranchissement {
   ASN1INT   largeur;
   ASN1INT   profondeur;
   ASN1INT   vitesse_courant;
   ASN1BOOL  berges_a_amenager;
} ASN1T_AttrObjectSiteFranchissement;

class EXTERN ASN1C_AttrObjectSiteFranchissement : public ASN1CType {
public:
   ASN1T_AttrObjectSiteFranchissement& msgData;
   ASN1C_AttrObjectSiteFranchissement (
      ASN1MessageBuffer& msgBuf, ASN1T_AttrObjectSiteFranchissement& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_AttrObjectSiteFranchissement (ASN1CTXT* ctxt_p, ASN1T_AttrObjectSiteFranchissement* pvalue);
EXTERN int asn1PD_AttrObjectSiteFranchissement (ASN1CTXT* ctxt_p, ASN1T_AttrObjectSiteFranchissement* pvalue);

EXTERN void asn1Print_AttrObjectSiteFranchissement (ASN1ConstCharPtr name, ASN1T_AttrObjectSiteFranchissement* pvalue);

/**************************************************************/
/*                                                            */
/*  AttrObjectNuageNBC                                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_AttrObjectNuageNBC {
   ASN1T_OID  agent_nbc;
} ASN1T_AttrObjectNuageNBC;

class EXTERN ASN1C_AttrObjectNuageNBC : public ASN1CType {
public:
   ASN1T_AttrObjectNuageNBC& msgData;
   ASN1C_AttrObjectNuageNBC (
      ASN1MessageBuffer& msgBuf, ASN1T_AttrObjectNuageNBC& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_AttrObjectNuageNBC (ASN1CTXT* ctxt_p, ASN1T_AttrObjectNuageNBC* pvalue);
EXTERN int asn1PD_AttrObjectNuageNBC (ASN1CTXT* ctxt_p, ASN1T_AttrObjectNuageNBC* pvalue);

EXTERN void asn1Print_AttrObjectNuageNBC (ASN1ConstCharPtr name, ASN1T_AttrObjectNuageNBC* pvalue);

/**************************************************************/
/*                                                            */
/*  _SeqOfOID                                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T__SeqOfOID {
   ASN1UINT n;
   ASN1T_OID *elem;
} ASN1T__SeqOfOID;

class EXTERN ASN1C__SeqOfOID : public ASN1CType {
public:
   ASN1T__SeqOfOID& msgData;
   ASN1C__SeqOfOID (
      ASN1MessageBuffer& msgBuf, ASN1T__SeqOfOID& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE__SeqOfOID (ASN1CTXT* ctxt_p, ASN1T__SeqOfOID* pvalue);
EXTERN int asn1PD__SeqOfOID (ASN1CTXT* ctxt_p, ASN1T__SeqOfOID* pvalue);

EXTERN void asn1Print__SeqOfOID (ASN1ConstCharPtr name, ASN1T__SeqOfOID* pvalue);

/**************************************************************/
/*                                                            */
/*  AttrObjectROTA                                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_AttrObjectROTA {
   ASN1INT   niveau_danger;
   ASN1T__SeqOfOID  agents_nbc;
} ASN1T_AttrObjectROTA;

class EXTERN ASN1C_AttrObjectROTA : public ASN1CType {
public:
   ASN1T_AttrObjectROTA& msgData;
   ASN1C_AttrObjectROTA (
      ASN1MessageBuffer& msgBuf, ASN1T_AttrObjectROTA& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_AttrObjectROTA (ASN1CTXT* ctxt_p, ASN1T_AttrObjectROTA* pvalue);
EXTERN int asn1PD_AttrObjectROTA (ASN1CTXT* ctxt_p, ASN1T_AttrObjectROTA* pvalue);

EXTERN void asn1Print_AttrObjectROTA (ASN1ConstCharPtr name, ASN1T_AttrObjectROTA* pvalue);

/**************************************************************/
/*                                                            */
/*  AttrObjectZoneNBC                                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_AttrObjectZoneNBC {
   ASN1T_OID  agent_nbc;
} ASN1T_AttrObjectZoneNBC;

class EXTERN ASN1C_AttrObjectZoneNBC : public ASN1CType {
public:
   ASN1T_AttrObjectZoneNBC& msgData;
   ASN1C_AttrObjectZoneNBC (
      ASN1MessageBuffer& msgBuf, ASN1T_AttrObjectZoneNBC& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_AttrObjectZoneNBC (ASN1CTXT* ctxt_p, ASN1T_AttrObjectZoneNBC* pvalue);
EXTERN int asn1PD_AttrObjectZoneNBC (ASN1CTXT* ctxt_p, ASN1T_AttrObjectZoneNBC* pvalue);

EXTERN void asn1Print_AttrObjectZoneNBC (ASN1ConstCharPtr name, ASN1T_AttrObjectZoneNBC* pvalue);

/**************************************************************/
/*                                                            */
/*  AttrObjectItineraireLogistique                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_AttrObjectItineraireLogistique {
   ASN1BOOL  itineraire_equipe;
   ASN1INT   poids_max_supporte;
   ASN1INT   largeur;
   ASN1INT   longueur;
   ASN1INT   debit;
} ASN1T_AttrObjectItineraireLogistique;

class EXTERN ASN1C_AttrObjectItineraireLogistique : public ASN1CType {
public:
   ASN1T_AttrObjectItineraireLogistique& msgData;
   ASN1C_AttrObjectItineraireLogistique (
      ASN1MessageBuffer& msgBuf, ASN1T_AttrObjectItineraireLogistique& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_AttrObjectItineraireLogistique (ASN1CTXT* ctxt_p, ASN1T_AttrObjectItineraireLogistique* pvalue);
EXTERN int asn1PD_AttrObjectItineraireLogistique (ASN1CTXT* ctxt_p, ASN1T_AttrObjectItineraireLogistique* pvalue);

EXTERN void asn1Print_AttrObjectItineraireLogistique (ASN1ConstCharPtr name, ASN1T_AttrObjectItineraireLogistique* pvalue);

/**************************************************************/
/*                                                            */
/*  AttrObjectCampPrisonniers                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_AttrObjectCampPrisonniers {
   ASN1T_Automate  tc2;
} ASN1T_AttrObjectCampPrisonniers;

class EXTERN ASN1C_AttrObjectCampPrisonniers : public ASN1CType {
public:
   ASN1T_AttrObjectCampPrisonniers& msgData;
   ASN1C_AttrObjectCampPrisonniers (
      ASN1MessageBuffer& msgBuf, ASN1T_AttrObjectCampPrisonniers& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_AttrObjectCampPrisonniers (ASN1CTXT* ctxt_p, ASN1T_AttrObjectCampPrisonniers* pvalue);
EXTERN int asn1PD_AttrObjectCampPrisonniers (ASN1CTXT* ctxt_p, ASN1T_AttrObjectCampPrisonniers* pvalue);

EXTERN void asn1Print_AttrObjectCampPrisonniers (ASN1ConstCharPtr name, ASN1T_AttrObjectCampPrisonniers* pvalue);

/**************************************************************/
/*                                                            */
/*  AttrObjectCampRefugies                                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_AttrObjectCampRefugies {
   ASN1T_Automate  tc2;
} ASN1T_AttrObjectCampRefugies;

class EXTERN ASN1C_AttrObjectCampRefugies : public ASN1CType {
public:
   ASN1T_AttrObjectCampRefugies& msgData;
   ASN1C_AttrObjectCampRefugies (
      ASN1MessageBuffer& msgBuf, ASN1T_AttrObjectCampRefugies& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_AttrObjectCampRefugies (ASN1CTXT* ctxt_p, ASN1T_AttrObjectCampRefugies* pvalue);
EXTERN int asn1PD_AttrObjectCampRefugies (ASN1CTXT* ctxt_p, ASN1T_AttrObjectCampRefugies* pvalue);

EXTERN void asn1Print_AttrObjectCampRefugies (ASN1ConstCharPtr name, ASN1T_AttrObjectCampRefugies* pvalue);

/**************************************************************/
/*                                                            */
/*  AttrObjectSpecific                                        */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_AttrObjectSpecific_site_franchissement 1
#define T_AttrObjectSpecific_nuage_nbc  2
#define T_AttrObjectSpecific_rota       3
#define T_AttrObjectSpecific_zone_nbc   4
#define T_AttrObjectSpecific_itineraire_logistique 5
#define T_AttrObjectSpecific_camp_prisonniers 6
#define T_AttrObjectSpecific_camp_refugies 7

typedef struct EXTERN ASN1T_AttrObjectSpecific {
   int t;
   union {
      /* t = 1 */
      ASN1T_AttrObjectSiteFranchissement *site_franchissement;
      /* t = 2 */
      ASN1T_AttrObjectNuageNBC *nuage_nbc;
      /* t = 3 */
      ASN1T_AttrObjectROTA *rota;
      /* t = 4 */
      ASN1T_AttrObjectZoneNBC *zone_nbc;
      /* t = 5 */
      ASN1T_AttrObjectItineraireLogistique *itineraire_logistique;
      /* t = 6 */
      ASN1T_AttrObjectCampPrisonniers *camp_prisonniers;
      /* t = 7 */
      ASN1T_AttrObjectCampRefugies *camp_refugies;
   } u;
} ASN1T_AttrObjectSpecific;

class EXTERN ASN1C_AttrObjectSpecific : public ASN1CType {
public:
   ASN1T_AttrObjectSpecific& msgData;
   ASN1C_AttrObjectSpecific (
      ASN1MessageBuffer& msgBuf, ASN1T_AttrObjectSpecific& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_AttrObjectSpecific (ASN1CTXT* ctxt_p, ASN1T_AttrObjectSpecific* pvalue);
EXTERN int asn1PD_AttrObjectSpecific (ASN1CTXT* ctxt_p, ASN1T_AttrObjectSpecific* pvalue);

EXTERN void asn1Print_AttrObjectSpecific (ASN1ConstCharPtr name, ASN1T_AttrObjectSpecific* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgObjectCreation                                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgObjectCreation {
   struct {
      unsigned oid_objet_planifiePresent : 1;
      unsigned attributs_specifiquesPresent : 1;
      unsigned type_dotation_constructionPresent : 1;
      unsigned type_dotation_valorisationPresent : 1;
   } m;
   ASN1T_OID  oid;
   ASN1T_EnumObjectType  type;
   ASN1T_Localisation  localisation;
   ASN1T_OID  camp;
   ASN1T_OID  oid_objet_planifie;
   ASN1T_AttrObjectSpecific  attributs_specifiques;
   ASN1T_TypeDotation  type_dotation_construction;
   ASN1T_TypeDotation  type_dotation_valorisation;

   ASN1T_MsgObjectCreation () {
      m.oid_objet_planifiePresent = 0;
      m.attributs_specifiquesPresent = 0;
      m.type_dotation_constructionPresent = 0;
      m.type_dotation_valorisationPresent = 0;
   }
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
/*  MsgObjectUpdate                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgObjectUpdate {
   struct {
      unsigned pourcentage_constructionPresent : 1;
      unsigned pourcentage_valorisationPresent : 1;
      unsigned pourcentage_creation_contournementPresent : 1;
      unsigned nb_dotation_constructionPresent : 1;
      unsigned nb_dotation_valorisationPresent : 1;
      unsigned localisationPresent : 1;
      unsigned attributs_specifiquesPresent : 1;
   } m;
   ASN1T_OID  oid;
   ASN1INT   pourcentage_construction;
   ASN1INT   pourcentage_valorisation;
   ASN1INT   pourcentage_creation_contournement;
   ASN1BOOL  en_preparation;
   ASN1INT   nb_dotation_construction;
   ASN1INT   nb_dotation_valorisation;
   ASN1T_Localisation  localisation;
   ASN1T_AttrObjectSpecific  attributs_specifiques;

   ASN1T_MsgObjectUpdate () {
      m.pourcentage_constructionPresent = 0;
      m.pourcentage_valorisationPresent = 0;
      m.pourcentage_creation_contournementPresent = 0;
      en_preparation = FALSE;
      m.nb_dotation_constructionPresent = 0;
      m.nb_dotation_valorisationPresent = 0;
      m.localisationPresent = 0;
      m.attributs_specifiquesPresent = 0;
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
/*  MsgObjectMagicActionAck                                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgObjectMagicActionAck {
   ASN1T_OID  oid;
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

/**************************************************************/
/*                                                            */
/*  MagicActionCreateObject                                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MagicActionCreateObject {
   struct {
      unsigned attributs_specifiquesPresent : 1;
   } m;
   ASN1T_OID  camp;
   ASN1T_EnumObjectType  type;
   ASN1T_Localisation  localisation;
   ASN1T_AttrObjectSpecific  attributs_specifiques;

   ASN1T_MagicActionCreateObject () {
      m.attributs_specifiquesPresent = 0;
   }
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
/*  MagicActionUpdateObject                                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MagicActionUpdateObject {
   struct {
      unsigned pourcentage_constructionPresent : 1;
      unsigned pourcentage_valorisationPresent : 1;
      unsigned pourcentage_creation_contournementPresent : 1;
      unsigned en_preparationPresent : 1;
   } m;
   ASN1INT   pourcentage_construction;
   ASN1INT   pourcentage_valorisation;
   ASN1INT   pourcentage_creation_contournement;
   ASN1BOOL  en_preparation;

   ASN1T_MagicActionUpdateObject () {
      m.pourcentage_constructionPresent = 0;
      m.pourcentage_valorisationPresent = 0;
      m.pourcentage_creation_contournementPresent = 0;
      m.en_preparationPresent = 0;
   }
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
/*  MagicActionDestroyObjet                                   */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MagicActionDestroyObjet : public ASN1CType {
public:
   ASN1C_MagicActionDestroyObjet (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MagicActionDestroyObjet (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MagicActionDestroyObjet (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MagicActionDestroyObjet (ASN1ConstCharPtr name);

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
   ASN1T_OID  oid_objet;
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

#endif

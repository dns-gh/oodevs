/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 13-Oct-2006.
 */
#ifndef SCIPIOOBJECTKNOWLEDGE_H
#define SCIPIOOBJECTKNOWLEDGE_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioObject.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  MsgObjectKnowledgeCreation                                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgObjectKnowledgeCreation {
   struct {
      unsigned type_dotation_constructionPresent : 1;
      unsigned type_dotation_valorisationPresent : 1;
   } m;
   ASN1T_KnowledgeObject  oid_connaissance;
   ASN1T_Camp  oid_camp_possesseur;
   ASN1T_OID  oid_objet_reel;
   ASN1T_EnumObjectType  type;
   ASN1T_TypeDotation  type_dotation_construction;
   ASN1T_TypeDotation  type_dotation_valorisation;

   ASN1T_MsgObjectKnowledgeCreation () {
      m.type_dotation_constructionPresent = 0;
      m.type_dotation_valorisationPresent = 0;
   }
} ASN1T_MsgObjectKnowledgeCreation;

class EXTERN ASN1C_MsgObjectKnowledgeCreation : public ASN1CType {
public:
   ASN1T_MsgObjectKnowledgeCreation& msgData;
   ASN1C_MsgObjectKnowledgeCreation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgObjectKnowledgeCreation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgObjectKnowledgeCreation (ASN1CTXT* ctxt_p, ASN1T_MsgObjectKnowledgeCreation* pvalue);
EXTERN int asn1PD_MsgObjectKnowledgeCreation (ASN1CTXT* ctxt_p, ASN1T_MsgObjectKnowledgeCreation* pvalue);

EXTERN void asn1Print_MsgObjectKnowledgeCreation (ASN1ConstCharPtr name, ASN1T_MsgObjectKnowledgeCreation* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgObjectKnowledgeUpdate                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgObjectKnowledgeUpdate {
   struct {
      unsigned oid_objet_reelPresent : 1;
      unsigned pertinencePresent : 1;
      unsigned localisationPresent : 1;
      unsigned pourcentage_constructionPresent : 1;
      unsigned pourcentage_valorisationPresent : 1;
      unsigned pourcentage_contournementPresent : 1;
      unsigned en_preparationPresent : 1;
      unsigned est_percuPresent : 1;
      unsigned attributs_specifiquesPresent : 1;
      unsigned perception_par_compagniePresent : 1;
      unsigned nb_dotation_constructionPresent : 1;
      unsigned nb_dotation_valorisationPresent : 1;
   } m;
   ASN1T_KnowledgeObject  oid_connaissance;
   ASN1T_Camp  oid_camp_possesseur;
   ASN1T_OID  oid_objet_reel;
   ASN1INT   pertinence;
   ASN1T_Localisation  localisation;
   ASN1INT   pourcentage_construction;
   ASN1INT   pourcentage_valorisation;
   ASN1INT   pourcentage_contournement;
   ASN1BOOL  en_preparation;
   ASN1BOOL  est_percu;
   ASN1T_AttrObjectSpecific  attributs_specifiques;
   ASN1T__SeqOfOID  perception_par_compagnie;
   ASN1INT   nb_dotation_construction;
   ASN1INT   nb_dotation_valorisation;

   ASN1T_MsgObjectKnowledgeUpdate () {
      m.oid_objet_reelPresent = 0;
      m.pertinencePresent = 0;
      m.localisationPresent = 0;
      m.pourcentage_constructionPresent = 0;
      m.pourcentage_valorisationPresent = 0;
      m.pourcentage_contournementPresent = 0;
      m.en_preparationPresent = 0;
      m.est_percuPresent = 0;
      m.attributs_specifiquesPresent = 0;
      m.perception_par_compagniePresent = 0;
      m.nb_dotation_constructionPresent = 0;
      m.nb_dotation_valorisationPresent = 0;
   }
} ASN1T_MsgObjectKnowledgeUpdate;

class EXTERN ASN1C_MsgObjectKnowledgeUpdate : public ASN1CType {
public:
   ASN1T_MsgObjectKnowledgeUpdate& msgData;
   ASN1C_MsgObjectKnowledgeUpdate (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgObjectKnowledgeUpdate& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgObjectKnowledgeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgObjectKnowledgeUpdate* pvalue);
EXTERN int asn1PD_MsgObjectKnowledgeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgObjectKnowledgeUpdate* pvalue);

EXTERN void asn1Print_MsgObjectKnowledgeUpdate (ASN1ConstCharPtr name, ASN1T_MsgObjectKnowledgeUpdate* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgObjectKnowledgeDestruction                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgObjectKnowledgeDestruction {
   ASN1T_KnowledgeObject  oid_connaissance;
   ASN1T_Camp  oid_camp_possesseur;
} ASN1T_MsgObjectKnowledgeDestruction;

class EXTERN ASN1C_MsgObjectKnowledgeDestruction : public ASN1CType {
public:
   ASN1T_MsgObjectKnowledgeDestruction& msgData;
   ASN1C_MsgObjectKnowledgeDestruction (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgObjectKnowledgeDestruction& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgObjectKnowledgeDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgObjectKnowledgeDestruction* pvalue);
EXTERN int asn1PD_MsgObjectKnowledgeDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgObjectKnowledgeDestruction* pvalue);

EXTERN void asn1Print_MsgObjectKnowledgeDestruction (ASN1ConstCharPtr name, ASN1T_MsgObjectKnowledgeDestruction* pvalue);

#endif

/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 13-Oct-2005.
 */
#ifndef SCIPIOPIONMISSION_LOG_H
#define SCIPIOPIONMISSION_LOG_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  Mission_Pion_LOG_SeDeployer                               */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_LOG_SeDeployer {
   ASN1T_Point  position_deploiement;
} ASN1T_Mission_Pion_LOG_SeDeployer;

class EXTERN ASN1C_Mission_Pion_LOG_SeDeployer : public ASN1CType {
public:
   ASN1T_Mission_Pion_LOG_SeDeployer& msgData;
   ASN1C_Mission_Pion_LOG_SeDeployer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_LOG_SeDeployer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_LOG_SeDeployer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_SeDeployer* pvalue);
EXTERN int asn1PD_Mission_Pion_LOG_SeDeployer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_SeDeployer* pvalue);

EXTERN void asn1Print_Mission_Pion_LOG_SeDeployer (ASN1ConstCharPtr name, ASN1T_Mission_Pion_LOG_SeDeployer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_LOG_DistribuerMoyens                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_LOG_DistribuerMoyens {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Pion_LOG_DistribuerMoyens;

class EXTERN ASN1C_Mission_Pion_LOG_DistribuerMoyens : public ASN1CType {
public:
   ASN1T_Mission_Pion_LOG_DistribuerMoyens& msgData;
   ASN1C_Mission_Pion_LOG_DistribuerMoyens (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_LOG_DistribuerMoyens& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_LOG_DistribuerMoyens (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_DistribuerMoyens* pvalue);
EXTERN int asn1PD_Mission_Pion_LOG_DistribuerMoyens (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_DistribuerMoyens* pvalue);

EXTERN void asn1Print_Mission_Pion_LOG_DistribuerMoyens (ASN1ConstCharPtr name, ASN1T_Mission_Pion_LOG_DistribuerMoyens* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_LOG_TrierBlesses                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_LOG_TrierBlesses {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Pion_LOG_TrierBlesses;

class EXTERN ASN1C_Mission_Pion_LOG_TrierBlesses : public ASN1CType {
public:
   ASN1T_Mission_Pion_LOG_TrierBlesses& msgData;
   ASN1C_Mission_Pion_LOG_TrierBlesses (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_LOG_TrierBlesses& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_LOG_TrierBlesses (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_TrierBlesses* pvalue);
EXTERN int asn1PD_Mission_Pion_LOG_TrierBlesses (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_TrierBlesses* pvalue);

EXTERN void asn1Print_Mission_Pion_LOG_TrierBlesses (ASN1ConstCharPtr name, ASN1T_Mission_Pion_LOG_TrierBlesses* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_LOG_TraiterBlesses                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_LOG_TraiterBlesses {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Pion_LOG_TraiterBlesses;

class EXTERN ASN1C_Mission_Pion_LOG_TraiterBlesses : public ASN1CType {
public:
   ASN1T_Mission_Pion_LOG_TraiterBlesses& msgData;
   ASN1C_Mission_Pion_LOG_TraiterBlesses (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_LOG_TraiterBlesses& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_LOG_TraiterBlesses (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_TraiterBlesses* pvalue);
EXTERN int asn1PD_Mission_Pion_LOG_TraiterBlesses (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_TraiterBlesses* pvalue);

EXTERN void asn1Print_Mission_Pion_LOG_TraiterBlesses (ASN1ConstCharPtr name, ASN1T_Mission_Pion_LOG_TraiterBlesses* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_LOG_TrierEtTraiterBlesses                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_LOG_TrierEtTraiterBlesses {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Pion_LOG_TrierEtTraiterBlesses;

class EXTERN ASN1C_Mission_Pion_LOG_TrierEtTraiterBlesses : public ASN1CType {
public:
   ASN1T_Mission_Pion_LOG_TrierEtTraiterBlesses& msgData;
   ASN1C_Mission_Pion_LOG_TrierEtTraiterBlesses (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_LOG_TrierEtTraiterBlesses& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_LOG_TrierEtTraiterBlesses (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_TrierEtTraiterBlesses* pvalue);
EXTERN int asn1PD_Mission_Pion_LOG_TrierEtTraiterBlesses (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_TrierEtTraiterBlesses* pvalue);

EXTERN void asn1Print_Mission_Pion_LOG_TrierEtTraiterBlesses (ASN1ConstCharPtr name, ASN1T_Mission_Pion_LOG_TrierEtTraiterBlesses* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_LOG_SoignerBlesses                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_LOG_SoignerBlesses {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Pion_LOG_SoignerBlesses;

class EXTERN ASN1C_Mission_Pion_LOG_SoignerBlesses : public ASN1CType {
public:
   ASN1T_Mission_Pion_LOG_SoignerBlesses& msgData;
   ASN1C_Mission_Pion_LOG_SoignerBlesses (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_LOG_SoignerBlesses& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_LOG_SoignerBlesses (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_SoignerBlesses* pvalue);
EXTERN int asn1PD_Mission_Pion_LOG_SoignerBlesses (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_SoignerBlesses* pvalue);

EXTERN void asn1Print_Mission_Pion_LOG_SoignerBlesses (ASN1ConstCharPtr name, ASN1T_Mission_Pion_LOG_SoignerBlesses* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_LOG_PrendreEnCompteReactionsMentales         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_LOG_PrendreEnCompteReactionsMentales {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Pion_LOG_PrendreEnCompteReactionsMentales;

class EXTERN ASN1C_Mission_Pion_LOG_PrendreEnCompteReactionsMentales : public ASN1CType {
public:
   ASN1T_Mission_Pion_LOG_PrendreEnCompteReactionsMentales& msgData;
   ASN1C_Mission_Pion_LOG_PrendreEnCompteReactionsMentales (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_LOG_PrendreEnCompteReactionsMentales& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_LOG_PrendreEnCompteReactionsMentales (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_PrendreEnCompteReactionsMentales* pvalue);
EXTERN int asn1PD_Mission_Pion_LOG_PrendreEnCompteReactionsMentales (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_PrendreEnCompteReactionsMentales* pvalue);

EXTERN void asn1Print_Mission_Pion_LOG_PrendreEnCompteReactionsMentales (ASN1ConstCharPtr name, ASN1T_Mission_Pion_LOG_PrendreEnCompteReactionsMentales* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_LOG_DecontaminerBlesses                      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_LOG_DecontaminerBlesses {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Pion_LOG_DecontaminerBlesses;

class EXTERN ASN1C_Mission_Pion_LOG_DecontaminerBlesses : public ASN1CType {
public:
   ASN1T_Mission_Pion_LOG_DecontaminerBlesses& msgData;
   ASN1C_Mission_Pion_LOG_DecontaminerBlesses (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_LOG_DecontaminerBlesses& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_LOG_DecontaminerBlesses (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_DecontaminerBlesses* pvalue);
EXTERN int asn1PD_Mission_Pion_LOG_DecontaminerBlesses (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_DecontaminerBlesses* pvalue);

EXTERN void asn1Print_Mission_Pion_LOG_DecontaminerBlesses (ASN1ConstCharPtr name, ASN1T_Mission_Pion_LOG_DecontaminerBlesses* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_LOG_Reparer                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_LOG_Reparer {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Pion_LOG_Reparer;

class EXTERN ASN1C_Mission_Pion_LOG_Reparer : public ASN1CType {
public:
   ASN1T_Mission_Pion_LOG_Reparer& msgData;
   ASN1C_Mission_Pion_LOG_Reparer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_LOG_Reparer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_LOG_Reparer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_Reparer* pvalue);
EXTERN int asn1PD_Mission_Pion_LOG_Reparer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_Reparer* pvalue);

EXTERN void asn1Print_Mission_Pion_LOG_Reparer (ASN1ConstCharPtr name, ASN1T_Mission_Pion_LOG_Reparer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_LOG_Evacuer                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_LOG_Evacuer {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Pion_LOG_Evacuer;

class EXTERN ASN1C_Mission_Pion_LOG_Evacuer : public ASN1CType {
public:
   ASN1T_Mission_Pion_LOG_Evacuer& msgData;
   ASN1C_Mission_Pion_LOG_Evacuer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_LOG_Evacuer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_LOG_Evacuer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_Evacuer* pvalue);
EXTERN int asn1PD_Mission_Pion_LOG_Evacuer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_Evacuer* pvalue);

EXTERN void asn1Print_Mission_Pion_LOG_Evacuer (ASN1ConstCharPtr name, ASN1T_Mission_Pion_LOG_Evacuer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_LOG_Convoyer                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_LOG_Convoyer {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Pion_LOG_Convoyer;

class EXTERN ASN1C_Mission_Pion_LOG_Convoyer : public ASN1CType {
public:
   ASN1T_Mission_Pion_LOG_Convoyer& msgData;
   ASN1C_Mission_Pion_LOG_Convoyer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_LOG_Convoyer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_LOG_Convoyer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_Convoyer* pvalue);
EXTERN int asn1PD_Mission_Pion_LOG_Convoyer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_Convoyer* pvalue);

EXTERN void asn1Print_Mission_Pion_LOG_Convoyer (ASN1ConstCharPtr name, ASN1T_Mission_Pion_LOG_Convoyer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_LOG_DistribuerRessources                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_LOG_DistribuerRessources {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Pion_LOG_DistribuerRessources;

class EXTERN ASN1C_Mission_Pion_LOG_DistribuerRessources : public ASN1CType {
public:
   ASN1T_Mission_Pion_LOG_DistribuerRessources& msgData;
   ASN1C_Mission_Pion_LOG_DistribuerRessources (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_LOG_DistribuerRessources& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_LOG_DistribuerRessources (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_DistribuerRessources* pvalue);
EXTERN int asn1PD_Mission_Pion_LOG_DistribuerRessources (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_DistribuerRessources* pvalue);

EXTERN void asn1Print_Mission_Pion_LOG_DistribuerRessources (ASN1ConstCharPtr name, ASN1T_Mission_Pion_LOG_DistribuerRessources* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_LOG_ConstituerConvoi                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_LOG_ConstituerConvoi {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Pion_LOG_ConstituerConvoi;

class EXTERN ASN1C_Mission_Pion_LOG_ConstituerConvoi : public ASN1CType {
public:
   ASN1T_Mission_Pion_LOG_ConstituerConvoi& msgData;
   ASN1C_Mission_Pion_LOG_ConstituerConvoi (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_LOG_ConstituerConvoi& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_LOG_ConstituerConvoi (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_ConstituerConvoi* pvalue);
EXTERN int asn1PD_Mission_Pion_LOG_ConstituerConvoi (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_ConstituerConvoi* pvalue);

EXTERN void asn1Print_Mission_Pion_LOG_ConstituerConvoi (ASN1ConstCharPtr name, ASN1T_Mission_Pion_LOG_ConstituerConvoi* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_LOG_Livrer                                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_LOG_Livrer {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Pion_LOG_Livrer;

class EXTERN ASN1C_Mission_Pion_LOG_Livrer : public ASN1CType {
public:
   ASN1T_Mission_Pion_LOG_Livrer& msgData;
   ASN1C_Mission_Pion_LOG_Livrer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_LOG_Livrer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_LOG_Livrer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_Livrer* pvalue);
EXTERN int asn1PD_Mission_Pion_LOG_Livrer (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_Livrer* pvalue);

EXTERN void asn1Print_Mission_Pion_LOG_Livrer (ASN1ConstCharPtr name, ASN1T_Mission_Pion_LOG_Livrer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_LOG_PreparerBascule                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_LOG_PreparerBascule {
   ASN1T_Automate  automate_pour_bascule;
} ASN1T_Mission_Pion_LOG_PreparerBascule;

class EXTERN ASN1C_Mission_Pion_LOG_PreparerBascule : public ASN1CType {
public:
   ASN1T_Mission_Pion_LOG_PreparerBascule& msgData;
   ASN1C_Mission_Pion_LOG_PreparerBascule (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_LOG_PreparerBascule& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_LOG_PreparerBascule (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_PreparerBascule* pvalue);
EXTERN int asn1PD_Mission_Pion_LOG_PreparerBascule (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_PreparerBascule* pvalue);

EXTERN void asn1Print_Mission_Pion_LOG_PreparerBascule (ASN1ConstCharPtr name, ASN1T_Mission_Pion_LOG_PreparerBascule* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_LOG_AppuyerMouvement                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_LOG_AppuyerMouvement {
   ASN1T_Agent  unite_a_appuyer;
} ASN1T_Mission_Pion_LOG_AppuyerMouvement;

class EXTERN ASN1C_Mission_Pion_LOG_AppuyerMouvement : public ASN1CType {
public:
   ASN1T_Mission_Pion_LOG_AppuyerMouvement& msgData;
   ASN1C_Mission_Pion_LOG_AppuyerMouvement (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_LOG_AppuyerMouvement& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_LOG_AppuyerMouvement (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_AppuyerMouvement* pvalue);
EXTERN int asn1PD_Mission_Pion_LOG_AppuyerMouvement (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_AppuyerMouvement* pvalue);

EXTERN void asn1Print_Mission_Pion_LOG_AppuyerMouvement (ASN1ConstCharPtr name, ASN1T_Mission_Pion_LOG_AppuyerMouvement* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_LOG_ReconnaitreItineraire                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_LOG_ReconnaitreItineraire {
   ASN1T_Itineraire  itineraire_log;
} ASN1T_Mission_Pion_LOG_ReconnaitreItineraire;

class EXTERN ASN1C_Mission_Pion_LOG_ReconnaitreItineraire : public ASN1CType {
public:
   ASN1T_Mission_Pion_LOG_ReconnaitreItineraire& msgData;
   ASN1C_Mission_Pion_LOG_ReconnaitreItineraire (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_LOG_ReconnaitreItineraire& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_LOG_ReconnaitreItineraire (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_ReconnaitreItineraire* pvalue);
EXTERN int asn1PD_Mission_Pion_LOG_ReconnaitreItineraire (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_ReconnaitreItineraire* pvalue);

EXTERN void asn1Print_Mission_Pion_LOG_ReconnaitreItineraire (ASN1ConstCharPtr name, ASN1T_Mission_Pion_LOG_ReconnaitreItineraire* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_LOG_TransporterUniteChars                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_LOG_TransporterUniteChars {
   ASN1T_ListAgent  unites_a_transporter;
   ASN1T_Point  point_rendez_vous;
   ASN1T_Point  point_destination;
} ASN1T_Mission_Pion_LOG_TransporterUniteChars;

class EXTERN ASN1C_Mission_Pion_LOG_TransporterUniteChars : public ASN1CType {
public:
   ASN1T_Mission_Pion_LOG_TransporterUniteChars& msgData;
   ASN1C_Mission_Pion_LOG_TransporterUniteChars (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_LOG_TransporterUniteChars& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_LOG_TransporterUniteChars (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_TransporterUniteChars* pvalue);
EXTERN int asn1PD_Mission_Pion_LOG_TransporterUniteChars (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_TransporterUniteChars* pvalue);

EXTERN void asn1Print_Mission_Pion_LOG_TransporterUniteChars (ASN1ConstCharPtr name, ASN1T_Mission_Pion_LOG_TransporterUniteChars* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_LOG_ReconnaitreZoneContaminee                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_LOG_ReconnaitreZoneContaminee {
   ASN1T_Polygon  zone;
} ASN1T_Mission_Pion_LOG_ReconnaitreZoneContaminee;

class EXTERN ASN1C_Mission_Pion_LOG_ReconnaitreZoneContaminee : public ASN1CType {
public:
   ASN1T_Mission_Pion_LOG_ReconnaitreZoneContaminee& msgData;
   ASN1C_Mission_Pion_LOG_ReconnaitreZoneContaminee (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_LOG_ReconnaitreZoneContaminee& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_LOG_ReconnaitreZoneContaminee (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_ReconnaitreZoneContaminee* pvalue);
EXTERN int asn1PD_Mission_Pion_LOG_ReconnaitreZoneContaminee (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_ReconnaitreZoneContaminee* pvalue);

EXTERN void asn1Print_Mission_Pion_LOG_ReconnaitreZoneContaminee (ASN1ConstCharPtr name, ASN1T_Mission_Pion_LOG_ReconnaitreZoneContaminee* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_LOG_AppuyerMouvementDansZone                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_LOG_AppuyerMouvementDansZone {
   ASN1T_Polygon  zone;
} ASN1T_Mission_Pion_LOG_AppuyerMouvementDansZone;

class EXTERN ASN1C_Mission_Pion_LOG_AppuyerMouvementDansZone : public ASN1CType {
public:
   ASN1T_Mission_Pion_LOG_AppuyerMouvementDansZone& msgData;
   ASN1C_Mission_Pion_LOG_AppuyerMouvementDansZone (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_LOG_AppuyerMouvementDansZone& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_LOG_AppuyerMouvementDansZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_AppuyerMouvementDansZone* pvalue);
EXTERN int asn1PD_Mission_Pion_LOG_AppuyerMouvementDansZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_AppuyerMouvementDansZone* pvalue);

EXTERN void asn1Print_Mission_Pion_LOG_AppuyerMouvementDansZone (ASN1ConstCharPtr name, ASN1T_Mission_Pion_LOG_AppuyerMouvementDansZone* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_LOG_MettreEnOeuvreZoneStationnement          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_LOG_MettreEnOeuvreZoneStationnement {
   ASN1T_Polygon  zone;
} ASN1T_Mission_Pion_LOG_MettreEnOeuvreZoneStationnement;

class EXTERN ASN1C_Mission_Pion_LOG_MettreEnOeuvreZoneStationnement : public ASN1CType {
public:
   ASN1T_Mission_Pion_LOG_MettreEnOeuvreZoneStationnement& msgData;
   ASN1C_Mission_Pion_LOG_MettreEnOeuvreZoneStationnement (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_LOG_MettreEnOeuvreZoneStationnement& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_LOG_MettreEnOeuvreZoneStationnement (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_MettreEnOeuvreZoneStationnement* pvalue);
EXTERN int asn1PD_Mission_Pion_LOG_MettreEnOeuvreZoneStationnement (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_MettreEnOeuvreZoneStationnement* pvalue);

EXTERN void asn1Print_Mission_Pion_LOG_MettreEnOeuvreZoneStationnement (ASN1ConstCharPtr name, ASN1T_Mission_Pion_LOG_MettreEnOeuvreZoneStationnement* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_LOG_AppuyerFranchissementSurUnPoint          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_LOG_AppuyerFranchissementSurUnPoint {
   ASN1T_KnowledgeObject  pont_flottant;
} ASN1T_Mission_Pion_LOG_AppuyerFranchissementSurUnPoint;

class EXTERN ASN1C_Mission_Pion_LOG_AppuyerFranchissementSurUnPoint : public ASN1CType {
public:
   ASN1T_Mission_Pion_LOG_AppuyerFranchissementSurUnPoint& msgData;
   ASN1C_Mission_Pion_LOG_AppuyerFranchissementSurUnPoint (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_LOG_AppuyerFranchissementSurUnPoint& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_LOG_AppuyerFranchissementSurUnPoint (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_AppuyerFranchissementSurUnPoint* pvalue);
EXTERN int asn1PD_Mission_Pion_LOG_AppuyerFranchissementSurUnPoint (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_AppuyerFranchissementSurUnPoint* pvalue);

EXTERN void asn1Print_Mission_Pion_LOG_AppuyerFranchissementSurUnPoint (ASN1ConstCharPtr name, ASN1T_Mission_Pion_LOG_AppuyerFranchissementSurUnPoint* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_LOG_BloquerZone                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_LOG_BloquerZone {
   ASN1T_Polygon  zone;
} ASN1T_Mission_Pion_LOG_BloquerZone;

class EXTERN ASN1C_Mission_Pion_LOG_BloquerZone : public ASN1CType {
public:
   ASN1T_Mission_Pion_LOG_BloquerZone& msgData;
   ASN1C_Mission_Pion_LOG_BloquerZone (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_LOG_BloquerZone& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_LOG_BloquerZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_BloquerZone* pvalue);
EXTERN int asn1PD_Mission_Pion_LOG_BloquerZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_BloquerZone* pvalue);

EXTERN void asn1Print_Mission_Pion_LOG_BloquerZone (ASN1ConstCharPtr name, ASN1T_Mission_Pion_LOG_BloquerZone* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_LOG_ReconnaitreZoneDeDeploiement             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_LOG_ReconnaitreZoneDeDeploiement {
   ASN1T_Polygon  zone;
} ASN1T_Mission_Pion_LOG_ReconnaitreZoneDeDeploiement;

class EXTERN ASN1C_Mission_Pion_LOG_ReconnaitreZoneDeDeploiement : public ASN1CType {
public:
   ASN1T_Mission_Pion_LOG_ReconnaitreZoneDeDeploiement& msgData;
   ASN1C_Mission_Pion_LOG_ReconnaitreZoneDeDeploiement (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_LOG_ReconnaitreZoneDeDeploiement& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_LOG_ReconnaitreZoneDeDeploiement (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_ReconnaitreZoneDeDeploiement* pvalue);
EXTERN int asn1PD_Mission_Pion_LOG_ReconnaitreZoneDeDeploiement (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_ReconnaitreZoneDeDeploiement* pvalue);

EXTERN void asn1Print_Mission_Pion_LOG_ReconnaitreZoneDeDeploiement (ASN1ConstCharPtr name, ASN1T_Mission_Pion_LOG_ReconnaitreZoneDeDeploiement* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_LOG_ArmerPIA                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_LOG_ArmerPIA {
   ASN1T_Point  point;
} ASN1T_Mission_Pion_LOG_ArmerPIA;

class EXTERN ASN1C_Mission_Pion_LOG_ArmerPIA : public ASN1CType {
public:
   ASN1T_Mission_Pion_LOG_ArmerPIA& msgData;
   ASN1C_Mission_Pion_LOG_ArmerPIA (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_LOG_ArmerPIA& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_LOG_ArmerPIA (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_ArmerPIA* pvalue);
EXTERN int asn1PD_Mission_Pion_LOG_ArmerPIA (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_ArmerPIA* pvalue);

EXTERN void asn1Print_Mission_Pion_LOG_ArmerPIA (ASN1ConstCharPtr name, ASN1T_Mission_Pion_LOG_ArmerPIA* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Pion_LOG_Surveiller                               */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_LOG_Surveiller {
   ASN1T_Polygon  zone;
} ASN1T_Mission_Pion_LOG_Surveiller;

class EXTERN ASN1C_Mission_Pion_LOG_Surveiller : public ASN1CType {
public:
   ASN1T_Mission_Pion_LOG_Surveiller& msgData;
   ASN1C_Mission_Pion_LOG_Surveiller (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_LOG_Surveiller& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_LOG_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_Surveiller* pvalue);
EXTERN int asn1PD_Mission_Pion_LOG_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_LOG_Surveiller* pvalue);

EXTERN void asn1Print_Mission_Pion_LOG_Surveiller (ASN1ConstCharPtr name, ASN1T_Mission_Pion_LOG_Surveiller* pvalue);

#endif

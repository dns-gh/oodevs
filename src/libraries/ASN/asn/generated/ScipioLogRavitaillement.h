/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 16-Aug-2005.
 */
#ifndef SCIPIOLOGRAVITAILLEMENT_H
#define SCIPIOLOGRAVITAILLEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  DemandeDotation                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_DemandeDotation {
   ASN1T_TypeDotation  ressource_id;
   ASN1UINT  quantite_demandee;
   ASN1UINT  quantite_accordee;
   ASN1UINT  quantite_en_transit;
} ASN1T_DemandeDotation;

class EXTERN ASN1C_DemandeDotation : public ASN1CType {
public:
   ASN1T_DemandeDotation& msgData;
   ASN1C_DemandeDotation (
      ASN1MessageBuffer& msgBuf, ASN1T_DemandeDotation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_DemandeDotation (ASN1CTXT* ctxt_p, ASN1T_DemandeDotation* pvalue);
EXTERN int asn1PD_DemandeDotation (ASN1CTXT* ctxt_p, ASN1T_DemandeDotation* pvalue);

EXTERN void asn1Print_DemandeDotation (ASN1ConstCharPtr name, ASN1T_DemandeDotation* pvalue);

/**************************************************************/
/*                                                            */
/*  _SeqOfDemandeDotation                                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T__SeqOfDemandeDotation {
   ASN1UINT n;
   ASN1T_DemandeDotation *elem;
} ASN1T__SeqOfDemandeDotation;

class EXTERN ASN1C__SeqOfDemandeDotation : public ASN1CType {
public:
   ASN1T__SeqOfDemandeDotation& msgData;
   ASN1C__SeqOfDemandeDotation (
      ASN1MessageBuffer& msgBuf, ASN1T__SeqOfDemandeDotation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE__SeqOfDemandeDotation (ASN1CTXT* ctxt_p, ASN1T__SeqOfDemandeDotation* pvalue);
EXTERN int asn1PD__SeqOfDemandeDotation (ASN1CTXT* ctxt_p, ASN1T__SeqOfDemandeDotation* pvalue);

EXTERN void asn1Print__SeqOfDemandeDotation (ASN1ConstCharPtr name, ASN1T__SeqOfDemandeDotation* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLogRavitaillementTraitementCreation                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLogRavitaillementTraitementCreation {
   ASN1T_OID  oid_consigne;
   ASN1T_Automate  oid_automate;
   ASN1T__SeqOfDemandeDotation  dotations;
} ASN1T_MsgLogRavitaillementTraitementCreation;

class EXTERN ASN1C_MsgLogRavitaillementTraitementCreation : public ASN1CType {
public:
   ASN1T_MsgLogRavitaillementTraitementCreation& msgData;
   ASN1C_MsgLogRavitaillementTraitementCreation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLogRavitaillementTraitementCreation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLogRavitaillementTraitementCreation (ASN1CTXT* ctxt_p, ASN1T_MsgLogRavitaillementTraitementCreation* pvalue);
EXTERN int asn1PD_MsgLogRavitaillementTraitementCreation (ASN1CTXT* ctxt_p, ASN1T_MsgLogRavitaillementTraitementCreation* pvalue);

EXTERN void asn1Print_MsgLogRavitaillementTraitementCreation (ASN1ConstCharPtr name, ASN1T_MsgLogRavitaillementTraitementCreation* pvalue);

/**************************************************************/
/*                                                            */
/*  EnumLogRavitaillementTraitementEtat                       */
/*                                                            */
/**************************************************************/

struct EXTERN EnumLogRavitaillementTraitementEtat {
   enum Root {
      convoi_consitution = 0,
      convoi_chargement = 1,
      convoi_deplacement_aller = 2,
      convoi_dechargement = 3,
      convoi_deplacement_retour = 4,
      termine = 5
   } ;
} ;

typedef EnumLogRavitaillementTraitementEtat::Root ASN1T_EnumLogRavitaillementTraitementEtat;

class EXTERN ASN1C_EnumLogRavitaillementTraitementEtat : public ASN1CType {
public:
   ASN1T_EnumLogRavitaillementTraitementEtat& msgData;
   ASN1C_EnumLogRavitaillementTraitementEtat (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumLogRavitaillementTraitementEtat& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumLogRavitaillementTraitementEtat (ASN1CTXT* ctxt_p, ASN1T_EnumLogRavitaillementTraitementEtat value);
EXTERN int asn1PD_EnumLogRavitaillementTraitementEtat (ASN1CTXT* ctxt_p, ASN1T_EnumLogRavitaillementTraitementEtat* pvalue);

EXTERN void asn1Print_EnumLogRavitaillementTraitementEtat (ASN1ConstCharPtr name, ASN1T_EnumLogRavitaillementTraitementEtat* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLogRavitaillementTraitementUpdate                      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLogRavitaillementTraitementUpdate {
   struct {
      unsigned oid_pion_log_traitantPresent : 1;
      unsigned oid_pion_convoiPresent : 1;
      unsigned etatPresent : 1;
      unsigned dotationsPresent : 1;
   } m;
   ASN1T_OID  oid_consigne;
   ASN1T_Automate  oid_automate;
   ASN1T_Agent  oid_pion_log_traitant;
   ASN1T_Agent  oid_pion_convoi;
   ASN1T_EnumLogRavitaillementTraitementEtat  etat;
   ASN1T__SeqOfDemandeDotation  dotations;

   ASN1T_MsgLogRavitaillementTraitementUpdate () {
      m.oid_pion_log_traitantPresent = 0;
      m.oid_pion_convoiPresent = 0;
      m.etatPresent = 0;
      m.dotationsPresent = 0;
   }
} ASN1T_MsgLogRavitaillementTraitementUpdate;

class EXTERN ASN1C_MsgLogRavitaillementTraitementUpdate : public ASN1CType {
public:
   ASN1T_MsgLogRavitaillementTraitementUpdate& msgData;
   ASN1C_MsgLogRavitaillementTraitementUpdate (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLogRavitaillementTraitementUpdate& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLogRavitaillementTraitementUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgLogRavitaillementTraitementUpdate* pvalue);
EXTERN int asn1PD_MsgLogRavitaillementTraitementUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgLogRavitaillementTraitementUpdate* pvalue);

EXTERN void asn1Print_MsgLogRavitaillementTraitementUpdate (ASN1ConstCharPtr name, ASN1T_MsgLogRavitaillementTraitementUpdate* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLogRavitaillementTraitementDestruction                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLogRavitaillementTraitementDestruction {
   ASN1T_OID  oid_consigne;
   ASN1T_Automate  oid_automate;
} ASN1T_MsgLogRavitaillementTraitementDestruction;

class EXTERN ASN1C_MsgLogRavitaillementTraitementDestruction : public ASN1CType {
public:
   ASN1T_MsgLogRavitaillementTraitementDestruction& msgData;
   ASN1C_MsgLogRavitaillementTraitementDestruction (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLogRavitaillementTraitementDestruction& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLogRavitaillementTraitementDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgLogRavitaillementTraitementDestruction* pvalue);
EXTERN int asn1PD_MsgLogRavitaillementTraitementDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgLogRavitaillementTraitementDestruction* pvalue);

EXTERN void asn1Print_MsgLogRavitaillementTraitementDestruction (ASN1ConstCharPtr name, ASN1T_MsgLogRavitaillementTraitementDestruction* pvalue);

/**************************************************************/
/*                                                            */
/*  RavitaillementDisponibiliteMoyens                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_RavitaillementDisponibiliteMoyens {
   ASN1T_TypeEquipement  type_equipement;
   ASN1T_Pourcentage  pourcentage_disponibilite;
} ASN1T_RavitaillementDisponibiliteMoyens;

class EXTERN ASN1C_RavitaillementDisponibiliteMoyens : public ASN1CType {
public:
   ASN1T_RavitaillementDisponibiliteMoyens& msgData;
   ASN1C_RavitaillementDisponibiliteMoyens (
      ASN1MessageBuffer& msgBuf, ASN1T_RavitaillementDisponibiliteMoyens& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_RavitaillementDisponibiliteMoyens (ASN1CTXT* ctxt_p, ASN1T_RavitaillementDisponibiliteMoyens* pvalue);
EXTERN int asn1PD_RavitaillementDisponibiliteMoyens (ASN1CTXT* ctxt_p, ASN1T_RavitaillementDisponibiliteMoyens* pvalue);

EXTERN void asn1Print_RavitaillementDisponibiliteMoyens (ASN1ConstCharPtr name, ASN1T_RavitaillementDisponibiliteMoyens* pvalue);

/**************************************************************/
/*                                                            */
/*  DotationStock                                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_DotationStock {
   ASN1T_TypeDotation  ressource_id;
   ASN1UINT  quantite_disponible;
} ASN1T_DotationStock;

class EXTERN ASN1C_DotationStock : public ASN1CType {
public:
   ASN1T_DotationStock& msgData;
   ASN1C_DotationStock (
      ASN1MessageBuffer& msgBuf, ASN1T_DotationStock& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_DotationStock (ASN1CTXT* ctxt_p, ASN1T_DotationStock* pvalue);
EXTERN int asn1PD_DotationStock (ASN1CTXT* ctxt_p, ASN1T_DotationStock* pvalue);

EXTERN void asn1Print_DotationStock (ASN1ConstCharPtr name, ASN1T_DotationStock* pvalue);

/**************************************************************/
/*                                                            */
/*  _SeqOfDotationStock                                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T__SeqOfDotationStock {
   ASN1UINT n;
   ASN1T_DotationStock *elem;
} ASN1T__SeqOfDotationStock;

class EXTERN ASN1C__SeqOfDotationStock : public ASN1CType {
public:
   ASN1T__SeqOfDotationStock& msgData;
   ASN1C__SeqOfDotationStock (
      ASN1MessageBuffer& msgBuf, ASN1T__SeqOfDotationStock& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE__SeqOfDotationStock (ASN1CTXT* ctxt_p, ASN1T__SeqOfDotationStock* pvalue);
EXTERN int asn1PD__SeqOfDotationStock (ASN1CTXT* ctxt_p, ASN1T__SeqOfDotationStock* pvalue);

EXTERN void asn1Print__SeqOfDotationStock (ASN1ConstCharPtr name, ASN1T__SeqOfDotationStock* pvalue);

/**************************************************************/
/*                                                            */
/*  _SeqOfRavitaillementDisponibiliteMoyens                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T__SeqOfRavitaillementDisponibiliteMoyens {
   ASN1UINT n;
   ASN1T_RavitaillementDisponibiliteMoyens *elem;
} ASN1T__SeqOfRavitaillementDisponibiliteMoyens;

class EXTERN ASN1C__SeqOfRavitaillementDisponibiliteMoyens : public ASN1CType {
public:
   ASN1T__SeqOfRavitaillementDisponibiliteMoyens& msgData;
   ASN1C__SeqOfRavitaillementDisponibiliteMoyens (
      ASN1MessageBuffer& msgBuf, ASN1T__SeqOfRavitaillementDisponibiliteMoyens& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE__SeqOfRavitaillementDisponibiliteMoyens (ASN1CTXT* ctxt_p, ASN1T__SeqOfRavitaillementDisponibiliteMoyens* pvalue);
EXTERN int asn1PD__SeqOfRavitaillementDisponibiliteMoyens (ASN1CTXT* ctxt_p, ASN1T__SeqOfRavitaillementDisponibiliteMoyens* pvalue);

EXTERN void asn1Print__SeqOfRavitaillementDisponibiliteMoyens (ASN1ConstCharPtr name, ASN1T__SeqOfRavitaillementDisponibiliteMoyens* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLogRavitaillementEtat                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLogRavitaillementEtat {
   struct {
      unsigned chaine_activeePresent : 1;
      unsigned stocksPresent : 1;
      unsigned disponibilites_chefs_convoisPresent : 1;
      unsigned disponibilites_transporteurs_convoisPresent : 1;
   } m;
   ASN1T_Agent  oid_pion;
   ASN1BOOL  chaine_activee;
   ASN1T__SeqOfDotationStock  stocks;
   ASN1T__SeqOfRavitaillementDisponibiliteMoyens  disponibilites_chefs_convois;
   ASN1T__SeqOfRavitaillementDisponibiliteMoyens  disponibilites_transporteurs_convois;

   ASN1T_MsgLogRavitaillementEtat () {
      m.chaine_activeePresent = 0;
      m.stocksPresent = 0;
      m.disponibilites_chefs_convoisPresent = 0;
      m.disponibilites_transporteurs_convoisPresent = 0;
   }
} ASN1T_MsgLogRavitaillementEtat;

class EXTERN ASN1C_MsgLogRavitaillementEtat : public ASN1CType {
public:
   ASN1T_MsgLogRavitaillementEtat& msgData;
   ASN1C_MsgLogRavitaillementEtat (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLogRavitaillementEtat& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLogRavitaillementEtat (ASN1CTXT* ctxt_p, ASN1T_MsgLogRavitaillementEtat* pvalue);
EXTERN int asn1PD_MsgLogRavitaillementEtat (ASN1CTXT* ctxt_p, ASN1T_MsgLogRavitaillementEtat* pvalue);

EXTERN void asn1Print_MsgLogRavitaillementEtat (ASN1ConstCharPtr name, ASN1T_MsgLogRavitaillementEtat* pvalue);

/**************************************************************/
/*                                                            */
/*  DotationQuota                                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_DotationQuota {
   ASN1T_TypeDotation  ressource_id;
   ASN1UINT  quota_disponible;
} ASN1T_DotationQuota;

class EXTERN ASN1C_DotationQuota : public ASN1CType {
public:
   ASN1T_DotationQuota& msgData;
   ASN1C_DotationQuota (
      ASN1MessageBuffer& msgBuf, ASN1T_DotationQuota& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_DotationQuota (ASN1CTXT* ctxt_p, ASN1T_DotationQuota* pvalue);
EXTERN int asn1PD_DotationQuota (ASN1CTXT* ctxt_p, ASN1T_DotationQuota* pvalue);

EXTERN void asn1Print_DotationQuota (ASN1ConstCharPtr name, ASN1T_DotationQuota* pvalue);

/**************************************************************/
/*                                                            */
/*  _SeqOfDotationQuota                                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T__SeqOfDotationQuota {
   ASN1UINT n;
   ASN1T_DotationQuota *elem;
} ASN1T__SeqOfDotationQuota;

class EXTERN ASN1C__SeqOfDotationQuota : public ASN1CType {
public:
   ASN1T__SeqOfDotationQuota& msgData;
   ASN1C__SeqOfDotationQuota (
      ASN1MessageBuffer& msgBuf, ASN1T__SeqOfDotationQuota& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE__SeqOfDotationQuota (ASN1CTXT* ctxt_p, ASN1T__SeqOfDotationQuota* pvalue);
EXTERN int asn1PD__SeqOfDotationQuota (ASN1CTXT* ctxt_p, ASN1T__SeqOfDotationQuota* pvalue);

EXTERN void asn1Print__SeqOfDotationQuota (ASN1ConstCharPtr name, ASN1T__SeqOfDotationQuota* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLogRavitaillementChangeQuotas                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLogRavitaillementChangeQuotas {
   ASN1T_Automate  oid_donneur;
   ASN1T_Automate  oid_receveur;
   ASN1T__SeqOfDotationQuota  quotas;
} ASN1T_MsgLogRavitaillementChangeQuotas;

class EXTERN ASN1C_MsgLogRavitaillementChangeQuotas : public ASN1CType {
public:
   ASN1T_MsgLogRavitaillementChangeQuotas& msgData;
   ASN1C_MsgLogRavitaillementChangeQuotas (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLogRavitaillementChangeQuotas& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLogRavitaillementChangeQuotas (ASN1CTXT* ctxt_p, ASN1T_MsgLogRavitaillementChangeQuotas* pvalue);
EXTERN int asn1PD_MsgLogRavitaillementChangeQuotas (ASN1CTXT* ctxt_p, ASN1T_MsgLogRavitaillementChangeQuotas* pvalue);

EXTERN void asn1Print_MsgLogRavitaillementChangeQuotas (ASN1ConstCharPtr name, ASN1T_MsgLogRavitaillementChangeQuotas* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLogRavitaillementChangeQuotasAck                       */
/*                                                            */
/**************************************************************/

struct EXTERN MsgLogRavitaillementChangeQuotasAck {
   enum Root {
      no_error = 0,
      error_invalid_donneur = 1,
      error_invalid_receveur = 2
   } ;
} ;

typedef MsgLogRavitaillementChangeQuotasAck::Root ASN1T_MsgLogRavitaillementChangeQuotasAck;

class EXTERN ASN1C_MsgLogRavitaillementChangeQuotasAck : public ASN1CType {
public:
   ASN1T_MsgLogRavitaillementChangeQuotasAck& msgData;
   ASN1C_MsgLogRavitaillementChangeQuotasAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLogRavitaillementChangeQuotasAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLogRavitaillementChangeQuotasAck (ASN1CTXT* ctxt_p, ASN1T_MsgLogRavitaillementChangeQuotasAck value);
EXTERN int asn1PD_MsgLogRavitaillementChangeQuotasAck (ASN1CTXT* ctxt_p, ASN1T_MsgLogRavitaillementChangeQuotasAck* pvalue);

EXTERN void asn1Print_MsgLogRavitaillementChangeQuotasAck (ASN1ConstCharPtr name, ASN1T_MsgLogRavitaillementChangeQuotasAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLogRavitaillementPousserFlux                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLogRavitaillementPousserFlux {
   ASN1T_Automate  oid_donneur;
   ASN1T_Automate  oid_receveur;
   ASN1T__SeqOfDotationStock  stocks;
} ASN1T_MsgLogRavitaillementPousserFlux;

class EXTERN ASN1C_MsgLogRavitaillementPousserFlux : public ASN1CType {
public:
   ASN1T_MsgLogRavitaillementPousserFlux& msgData;
   ASN1C_MsgLogRavitaillementPousserFlux (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLogRavitaillementPousserFlux& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLogRavitaillementPousserFlux (ASN1CTXT* ctxt_p, ASN1T_MsgLogRavitaillementPousserFlux* pvalue);
EXTERN int asn1PD_MsgLogRavitaillementPousserFlux (ASN1CTXT* ctxt_p, ASN1T_MsgLogRavitaillementPousserFlux* pvalue);

EXTERN void asn1Print_MsgLogRavitaillementPousserFlux (ASN1ConstCharPtr name, ASN1T_MsgLogRavitaillementPousserFlux* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLogRavitaillementPousserFluxAck                        */
/*                                                            */
/**************************************************************/

struct EXTERN MsgLogRavitaillementPousserFluxAck {
   enum Root {
      no_error = 0,
      error_invalid_donneur = 1,
      error_invalid_receveur = 2,
      error_ravitaillement_en_cours = 3
   } ;
} ;

typedef MsgLogRavitaillementPousserFluxAck::Root ASN1T_MsgLogRavitaillementPousserFluxAck;

class EXTERN ASN1C_MsgLogRavitaillementPousserFluxAck : public ASN1CType {
public:
   ASN1T_MsgLogRavitaillementPousserFluxAck& msgData;
   ASN1C_MsgLogRavitaillementPousserFluxAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLogRavitaillementPousserFluxAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLogRavitaillementPousserFluxAck (ASN1CTXT* ctxt_p, ASN1T_MsgLogRavitaillementPousserFluxAck value);
EXTERN int asn1PD_MsgLogRavitaillementPousserFluxAck (ASN1CTXT* ctxt_p, ASN1T_MsgLogRavitaillementPousserFluxAck* pvalue);

EXTERN void asn1Print_MsgLogRavitaillementPousserFluxAck (ASN1ConstCharPtr name, ASN1T_MsgLogRavitaillementPousserFluxAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLogRavitaillementQuotas                                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLogRavitaillementQuotas {
   ASN1T_Automate  oid_automate;
   ASN1T__SeqOfDotationQuota  quotas;
} ASN1T_MsgLogRavitaillementQuotas;

class EXTERN ASN1C_MsgLogRavitaillementQuotas : public ASN1CType {
public:
   ASN1T_MsgLogRavitaillementQuotas& msgData;
   ASN1C_MsgLogRavitaillementQuotas (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLogRavitaillementQuotas& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLogRavitaillementQuotas (ASN1CTXT* ctxt_p, ASN1T_MsgLogRavitaillementQuotas* pvalue);
EXTERN int asn1PD_MsgLogRavitaillementQuotas (ASN1CTXT* ctxt_p, ASN1T_MsgLogRavitaillementQuotas* pvalue);

EXTERN void asn1Print_MsgLogRavitaillementQuotas (ASN1ConstCharPtr name, ASN1T_MsgLogRavitaillementQuotas* pvalue);

#endif

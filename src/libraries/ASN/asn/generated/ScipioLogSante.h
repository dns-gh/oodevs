/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 16-Nov-2005.
 */
#ifndef SCIPIOLOGSANTE_H
#define SCIPIOLOGSANTE_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  EnumLogSanteTraitementEtat                                */
/*                                                            */
/**************************************************************/

struct EXTERN EnumLogSanteTraitementEtat {
   enum Root {
      attente_disponibilite_ambulance_releve = 0,
      ambulance_releve_deplacement_aller = 1,
      ambulance_releve_chargement = 2,
      attente_chargement_complet_ambulance_releve = 3,
      ambulance_releve_deplacement_retour = 4,
      ambulance_releve_dechargement = 5,
      attente_disponibilite_medecin_pour_diagnostique = 6,
      diagnostique = 7,
      recherche_secteur_tri = 8,
      attente_disponibilite_medecin_pour_tri = 9,
      tri = 10,
      recherche_secteur_soin = 11,
      attente_disponibilite_medecin_pour_soin = 12,
      soin = 13,
      attente_disponibilite_ambulance_ramassage = 14,
      ambulance_ramassage_chargement = 15,
      attente_chargement_complet_ambulance_ramassage = 16,
      ambulance_ramassage_deplacement_aller = 17,
      ambulance_ramassage_dechargement = 18,
      termine = 19
   } ;
} ;

typedef EnumLogSanteTraitementEtat::Root ASN1T_EnumLogSanteTraitementEtat;

class EXTERN ASN1C_EnumLogSanteTraitementEtat : public ASN1CType {
public:
   ASN1T_EnumLogSanteTraitementEtat& msgData;
   ASN1C_EnumLogSanteTraitementEtat (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumLogSanteTraitementEtat& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumLogSanteTraitementEtat (ASN1CTXT* ctxt_p, ASN1T_EnumLogSanteTraitementEtat value);
EXTERN int asn1PD_EnumLogSanteTraitementEtat (ASN1CTXT* ctxt_p, ASN1T_EnumLogSanteTraitementEtat* pvalue);

EXTERN void asn1Print_EnumLogSanteTraitementEtat (ASN1ConstCharPtr name, ASN1T_EnumLogSanteTraitementEtat* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLogSanteTraitementHumainCreation                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLogSanteTraitementHumainCreation {
   ASN1T_OID  oid_consigne;
   ASN1T_Agent  oid_pion;
   ASN1T_EnumHumanRank  rang;
   ASN1T_EnumHumanWound  blessure;
   ASN1BOOL  blesse_mental;
   ASN1BOOL  contamine_nbc;

   ASN1T_MsgLogSanteTraitementHumainCreation () {
      blesse_mental = FALSE;
      contamine_nbc = FALSE;
   }
} ASN1T_MsgLogSanteTraitementHumainCreation;

class EXTERN ASN1C_MsgLogSanteTraitementHumainCreation : public ASN1CType {
public:
   ASN1T_MsgLogSanteTraitementHumainCreation& msgData;
   ASN1C_MsgLogSanteTraitementHumainCreation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLogSanteTraitementHumainCreation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLogSanteTraitementHumainCreation (ASN1CTXT* ctxt_p, ASN1T_MsgLogSanteTraitementHumainCreation* pvalue);
EXTERN int asn1PD_MsgLogSanteTraitementHumainCreation (ASN1CTXT* ctxt_p, ASN1T_MsgLogSanteTraitementHumainCreation* pvalue);

EXTERN void asn1Print_MsgLogSanteTraitementHumainCreation (ASN1ConstCharPtr name, ASN1T_MsgLogSanteTraitementHumainCreation* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLogSanteTraitementHumainUpdate                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLogSanteTraitementHumainUpdate {
   struct {
      unsigned oid_pion_log_traitantPresent : 1;
      unsigned blessurePresent : 1;
      unsigned blesse_mentalPresent : 1;
      unsigned contamine_nbcPresent : 1;
      unsigned etatPresent : 1;
   } m;
   ASN1T_OID  oid_consigne;
   ASN1T_Agent  oid_pion;
   ASN1T_Agent  oid_pion_log_traitant;
   ASN1T_EnumHumanWound  blessure;
   ASN1BOOL  blesse_mental;
   ASN1BOOL  contamine_nbc;
   ASN1T_EnumLogSanteTraitementEtat  etat;

   ASN1T_MsgLogSanteTraitementHumainUpdate () {
      m.oid_pion_log_traitantPresent = 0;
      m.blessurePresent = 0;
      m.blesse_mentalPresent = 0;
      m.contamine_nbcPresent = 0;
      m.etatPresent = 0;
   }
} ASN1T_MsgLogSanteTraitementHumainUpdate;

class EXTERN ASN1C_MsgLogSanteTraitementHumainUpdate : public ASN1CType {
public:
   ASN1T_MsgLogSanteTraitementHumainUpdate& msgData;
   ASN1C_MsgLogSanteTraitementHumainUpdate (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLogSanteTraitementHumainUpdate& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLogSanteTraitementHumainUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgLogSanteTraitementHumainUpdate* pvalue);
EXTERN int asn1PD_MsgLogSanteTraitementHumainUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgLogSanteTraitementHumainUpdate* pvalue);

EXTERN void asn1Print_MsgLogSanteTraitementHumainUpdate (ASN1ConstCharPtr name, ASN1T_MsgLogSanteTraitementHumainUpdate* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLogSanteTraitementHumainDestruction                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLogSanteTraitementHumainDestruction {
   ASN1T_OID  oid_consigne;
   ASN1T_Agent  oid_pion;
} ASN1T_MsgLogSanteTraitementHumainDestruction;

class EXTERN ASN1C_MsgLogSanteTraitementHumainDestruction : public ASN1CType {
public:
   ASN1T_MsgLogSanteTraitementHumainDestruction& msgData;
   ASN1C_MsgLogSanteTraitementHumainDestruction (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLogSanteTraitementHumainDestruction& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLogSanteTraitementHumainDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgLogSanteTraitementHumainDestruction* pvalue);
EXTERN int asn1PD_MsgLogSanteTraitementHumainDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgLogSanteTraitementHumainDestruction* pvalue);

EXTERN void asn1Print_MsgLogSanteTraitementHumainDestruction (ASN1ConstCharPtr name, ASN1T_MsgLogSanteTraitementHumainDestruction* pvalue);

/**************************************************************/
/*                                                            */
/*  SanteDisponibiliteMoyens                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_SanteDisponibiliteMoyens {
   ASN1T_TypeEquipement  type_equipement;
   ASN1T_Pourcentage  pourcentage_disponibilite;
} ASN1T_SanteDisponibiliteMoyens;

class EXTERN ASN1C_SanteDisponibiliteMoyens : public ASN1CType {
public:
   ASN1T_SanteDisponibiliteMoyens& msgData;
   ASN1C_SanteDisponibiliteMoyens (
      ASN1MessageBuffer& msgBuf, ASN1T_SanteDisponibiliteMoyens& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_SanteDisponibiliteMoyens (ASN1CTXT* ctxt_p, ASN1T_SanteDisponibiliteMoyens* pvalue);
EXTERN int asn1PD_SanteDisponibiliteMoyens (ASN1CTXT* ctxt_p, ASN1T_SanteDisponibiliteMoyens* pvalue);

EXTERN void asn1Print_SanteDisponibiliteMoyens (ASN1ConstCharPtr name, ASN1T_SanteDisponibiliteMoyens* pvalue);

/**************************************************************/
/*                                                            */
/*  SantePriorites                                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_SantePriorites {
   ASN1UINT n;
   ASN1T_EnumHumanWound *elem;
} ASN1T_SantePriorites;

class EXTERN ASN1C_SantePriorites : public ASN1CType {
public:
   ASN1T_SantePriorites& msgData;
   ASN1C_SantePriorites (
      ASN1MessageBuffer& msgBuf, ASN1T_SantePriorites& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_SantePriorites (ASN1CTXT* ctxt_p, ASN1T_SantePriorites* pvalue);
EXTERN int asn1PD_SantePriorites (ASN1CTXT* ctxt_p, ASN1T_SantePriorites* pvalue);

EXTERN void asn1Print_SantePriorites (ASN1ConstCharPtr name, ASN1T_SantePriorites* pvalue);

/**************************************************************/
/*                                                            */
/*  _SeqOfSanteDisponibiliteMoyens                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T__SeqOfSanteDisponibiliteMoyens {
   ASN1UINT n;
   ASN1T_SanteDisponibiliteMoyens *elem;
} ASN1T__SeqOfSanteDisponibiliteMoyens;

class EXTERN ASN1C__SeqOfSanteDisponibiliteMoyens : public ASN1CType {
public:
   ASN1T__SeqOfSanteDisponibiliteMoyens& msgData;
   ASN1C__SeqOfSanteDisponibiliteMoyens (
      ASN1MessageBuffer& msgBuf, ASN1T__SeqOfSanteDisponibiliteMoyens& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE__SeqOfSanteDisponibiliteMoyens (ASN1CTXT* ctxt_p, ASN1T__SeqOfSanteDisponibiliteMoyens* pvalue);
EXTERN int asn1PD__SeqOfSanteDisponibiliteMoyens (ASN1CTXT* ctxt_p, ASN1T__SeqOfSanteDisponibiliteMoyens* pvalue);

EXTERN void asn1Print__SeqOfSanteDisponibiliteMoyens (ASN1ConstCharPtr name, ASN1T__SeqOfSanteDisponibiliteMoyens* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLogSanteEtat                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLogSanteEtat {
   struct {
      unsigned chaine_activeePresent : 1;
      unsigned temps_de_bordeePresent : 1;
      unsigned prioritesPresent : 1;
      unsigned priorites_tactiquesPresent : 1;
      unsigned disponibilites_ambulances_relevePresent : 1;
      unsigned disponibilites_ambulances_ramassagePresent : 1;
      unsigned disponibilites_medecinsPresent : 1;
   } m;
   ASN1T_Agent  oid_pion;
   ASN1BOOL  chaine_activee;
   ASN1T_EnumTempsBordee  temps_de_bordee;
   ASN1T_SantePriorites  priorites;
   ASN1T_ListAutomate  priorites_tactiques;
   ASN1T__SeqOfSanteDisponibiliteMoyens  disponibilites_ambulances_releve;
   ASN1T__SeqOfSanteDisponibiliteMoyens  disponibilites_ambulances_ramassage;
   ASN1T__SeqOfSanteDisponibiliteMoyens  disponibilites_medecins;

   ASN1T_MsgLogSanteEtat () {
      m.chaine_activeePresent = 0;
      m.temps_de_bordeePresent = 0;
      m.prioritesPresent = 0;
      m.priorites_tactiquesPresent = 0;
      m.disponibilites_ambulances_relevePresent = 0;
      m.disponibilites_ambulances_ramassagePresent = 0;
      m.disponibilites_medecinsPresent = 0;
   }
} ASN1T_MsgLogSanteEtat;

class EXTERN ASN1C_MsgLogSanteEtat : public ASN1CType {
public:
   ASN1T_MsgLogSanteEtat& msgData;
   ASN1C_MsgLogSanteEtat (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLogSanteEtat& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLogSanteEtat (ASN1CTXT* ctxt_p, ASN1T_MsgLogSanteEtat* pvalue);
EXTERN int asn1PD_MsgLogSanteEtat (ASN1CTXT* ctxt_p, ASN1T_MsgLogSanteEtat* pvalue);

EXTERN void asn1Print_MsgLogSanteEtat (ASN1ConstCharPtr name, ASN1T_MsgLogSanteEtat* pvalue);

#endif

/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 22-Nov-2005.
 */
#ifndef SCIPIOLOGMAINTENANCE_H
#define SCIPIOLOGMAINTENANCE_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  MsgLogMaintenanceTraitementEquipementCreation             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLogMaintenanceTraitementEquipementCreation {
   ASN1T_OID  oid_consigne;
   ASN1T_Agent  oid_pion;
   ASN1T_TypeEquipement  type_equipement;
   ASN1T_TypePanne  type_panne;
} ASN1T_MsgLogMaintenanceTraitementEquipementCreation;

class EXTERN ASN1C_MsgLogMaintenanceTraitementEquipementCreation : public ASN1CType {
public:
   ASN1T_MsgLogMaintenanceTraitementEquipementCreation& msgData;
   ASN1C_MsgLogMaintenanceTraitementEquipementCreation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLogMaintenanceTraitementEquipementCreation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLogMaintenanceTraitementEquipementCreation (ASN1CTXT* ctxt_p, ASN1T_MsgLogMaintenanceTraitementEquipementCreation* pvalue);
EXTERN int asn1PD_MsgLogMaintenanceTraitementEquipementCreation (ASN1CTXT* ctxt_p, ASN1T_MsgLogMaintenanceTraitementEquipementCreation* pvalue);

EXTERN void asn1Print_MsgLogMaintenanceTraitementEquipementCreation (ASN1ConstCharPtr name, ASN1T_MsgLogMaintenanceTraitementEquipementCreation* pvalue);

/**************************************************************/
/*                                                            */
/*  EnumLogMaintenanceTraitementEtat                          */
/*                                                            */
/**************************************************************/

struct EXTERN EnumLogMaintenanceTraitementEtat {
   enum Root {
      deplacement_vers_chaine = 0,
      attente_disponibilite_remorqueur = 1,
      remorqueur_deplacement_aller = 2,
      remorqueur_chargement = 3,
      remorqueur_deplacement_retour = 4,
      remorqueur_dechargement = 5,
      diagnostique = 6,
      attente_prise_en_charge_par_niveau_superieur = 7,
      attente_disponibilite_pieces = 8,
      attente_disponibilite_reparateur = 9,
      reparation = 10,
      attente_retour_pion = 11,
      retour_pion = 12,
      termine = 13
   } ;
} ;

typedef EnumLogMaintenanceTraitementEtat::Root ASN1T_EnumLogMaintenanceTraitementEtat;

class EXTERN ASN1C_EnumLogMaintenanceTraitementEtat : public ASN1CType {
public:
   ASN1T_EnumLogMaintenanceTraitementEtat& msgData;
   ASN1C_EnumLogMaintenanceTraitementEtat (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumLogMaintenanceTraitementEtat& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumLogMaintenanceTraitementEtat (ASN1CTXT* ctxt_p, ASN1T_EnumLogMaintenanceTraitementEtat value);
EXTERN int asn1PD_EnumLogMaintenanceTraitementEtat (ASN1CTXT* ctxt_p, ASN1T_EnumLogMaintenanceTraitementEtat* pvalue);

EXTERN void asn1Print_EnumLogMaintenanceTraitementEtat (ASN1ConstCharPtr name, ASN1T_EnumLogMaintenanceTraitementEtat* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLogMaintenanceTraitementEquipementUpdate               */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLogMaintenanceTraitementEquipementUpdate {
   ASN1T_OID  oid_consigne;
   ASN1T_Agent  oid_pion;
   ASN1T_Agent  oid_pion_log_traitant;
   ASN1T_EnumLogMaintenanceTraitementEtat  etat;
} ASN1T_MsgLogMaintenanceTraitementEquipementUpdate;

class EXTERN ASN1C_MsgLogMaintenanceTraitementEquipementUpdate : public ASN1CType {
public:
   ASN1T_MsgLogMaintenanceTraitementEquipementUpdate& msgData;
   ASN1C_MsgLogMaintenanceTraitementEquipementUpdate (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLogMaintenanceTraitementEquipementUpdate& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLogMaintenanceTraitementEquipementUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgLogMaintenanceTraitementEquipementUpdate* pvalue);
EXTERN int asn1PD_MsgLogMaintenanceTraitementEquipementUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgLogMaintenanceTraitementEquipementUpdate* pvalue);

EXTERN void asn1Print_MsgLogMaintenanceTraitementEquipementUpdate (ASN1ConstCharPtr name, ASN1T_MsgLogMaintenanceTraitementEquipementUpdate* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLogMaintenanceTraitementEquipementDestruction          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLogMaintenanceTraitementEquipementDestruction {
   ASN1T_OID  oid_consigne;
   ASN1T_Agent  oid_pion;
} ASN1T_MsgLogMaintenanceTraitementEquipementDestruction;

class EXTERN ASN1C_MsgLogMaintenanceTraitementEquipementDestruction : public ASN1CType {
public:
   ASN1T_MsgLogMaintenanceTraitementEquipementDestruction& msgData;
   ASN1C_MsgLogMaintenanceTraitementEquipementDestruction (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLogMaintenanceTraitementEquipementDestruction& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLogMaintenanceTraitementEquipementDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgLogMaintenanceTraitementEquipementDestruction* pvalue);
EXTERN int asn1PD_MsgLogMaintenanceTraitementEquipementDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgLogMaintenanceTraitementEquipementDestruction* pvalue);

EXTERN void asn1Print_MsgLogMaintenanceTraitementEquipementDestruction (ASN1ConstCharPtr name, ASN1T_MsgLogMaintenanceTraitementEquipementDestruction* pvalue);

/**************************************************************/
/*                                                            */
/*  MaintenanceDisponibiliteMoyens                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MaintenanceDisponibiliteMoyens {
   ASN1T_TypeEquipement  type_equipement;
   ASN1T_Pourcentage  pourcentage_disponibilite;
} ASN1T_MaintenanceDisponibiliteMoyens;

class EXTERN ASN1C_MaintenanceDisponibiliteMoyens : public ASN1CType {
public:
   ASN1T_MaintenanceDisponibiliteMoyens& msgData;
   ASN1C_MaintenanceDisponibiliteMoyens (
      ASN1MessageBuffer& msgBuf, ASN1T_MaintenanceDisponibiliteMoyens& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MaintenanceDisponibiliteMoyens (ASN1CTXT* ctxt_p, ASN1T_MaintenanceDisponibiliteMoyens* pvalue);
EXTERN int asn1PD_MaintenanceDisponibiliteMoyens (ASN1CTXT* ctxt_p, ASN1T_MaintenanceDisponibiliteMoyens* pvalue);

EXTERN void asn1Print_MaintenanceDisponibiliteMoyens (ASN1ConstCharPtr name, ASN1T_MaintenanceDisponibiliteMoyens* pvalue);

/**************************************************************/
/*                                                            */
/*  MaintenancePriorites                                      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MaintenancePriorites {
   ASN1UINT n;
   ASN1T_TypeEquipement *elem;
} ASN1T_MaintenancePriorites;

class EXTERN ASN1C_MaintenancePriorites : public ASN1CType {
public:
   ASN1T_MaintenancePriorites& msgData;
   ASN1C_MaintenancePriorites (
      ASN1MessageBuffer& msgBuf, ASN1T_MaintenancePriorites& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MaintenancePriorites (ASN1CTXT* ctxt_p, ASN1T_MaintenancePriorites* pvalue);
EXTERN int asn1PD_MaintenancePriorites (ASN1CTXT* ctxt_p, ASN1T_MaintenancePriorites* pvalue);

EXTERN void asn1Print_MaintenancePriorites (ASN1ConstCharPtr name, ASN1T_MaintenancePriorites* pvalue);

/**************************************************************/
/*                                                            */
/*  _SeqOfMaintenanceDisponibiliteMoyens                      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T__SeqOfMaintenanceDisponibiliteMoyens {
   ASN1UINT n;
   ASN1T_MaintenanceDisponibiliteMoyens *elem;
} ASN1T__SeqOfMaintenanceDisponibiliteMoyens;

class EXTERN ASN1C__SeqOfMaintenanceDisponibiliteMoyens : public ASN1CType {
public:
   ASN1T__SeqOfMaintenanceDisponibiliteMoyens& msgData;
   ASN1C__SeqOfMaintenanceDisponibiliteMoyens (
      ASN1MessageBuffer& msgBuf, ASN1T__SeqOfMaintenanceDisponibiliteMoyens& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE__SeqOfMaintenanceDisponibiliteMoyens (ASN1CTXT* ctxt_p, ASN1T__SeqOfMaintenanceDisponibiliteMoyens* pvalue);
EXTERN int asn1PD__SeqOfMaintenanceDisponibiliteMoyens (ASN1CTXT* ctxt_p, ASN1T__SeqOfMaintenanceDisponibiliteMoyens* pvalue);

EXTERN void asn1Print__SeqOfMaintenanceDisponibiliteMoyens (ASN1ConstCharPtr name, ASN1T__SeqOfMaintenanceDisponibiliteMoyens* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLogMaintenanceEtat                                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLogMaintenanceEtat {
   struct {
      unsigned chaine_activeePresent : 1;
      unsigned temps_de_bordeePresent : 1;
      unsigned prioritesPresent : 1;
      unsigned priorites_tactiquesPresent : 1;
      unsigned disponibilites_remorqueursPresent : 1;
      unsigned disponibilites_reparateursPresent : 1;
   } m;
   ASN1T_Agent  oid_pion;
   ASN1BOOL  chaine_activee;
   ASN1T_EnumTempsBordee  temps_de_bordee;
   ASN1T_MaintenancePriorites  priorites;
   ASN1T_ListAutomate  priorites_tactiques;
   ASN1T__SeqOfMaintenanceDisponibiliteMoyens  disponibilites_remorqueurs;
   ASN1T__SeqOfMaintenanceDisponibiliteMoyens  disponibilites_reparateurs;

   ASN1T_MsgLogMaintenanceEtat () {
      m.chaine_activeePresent = 0;
      m.temps_de_bordeePresent = 0;
      m.prioritesPresent = 0;
      m.priorites_tactiquesPresent = 0;
      m.disponibilites_remorqueursPresent = 0;
      m.disponibilites_reparateursPresent = 0;
   }
} ASN1T_MsgLogMaintenanceEtat;

class EXTERN ASN1C_MsgLogMaintenanceEtat : public ASN1CType {
public:
   ASN1T_MsgLogMaintenanceEtat& msgData;
   ASN1C_MsgLogMaintenanceEtat (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLogMaintenanceEtat& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLogMaintenanceEtat (ASN1CTXT* ctxt_p, ASN1T_MsgLogMaintenanceEtat* pvalue);
EXTERN int asn1PD_MsgLogMaintenanceEtat (ASN1CTXT* ctxt_p, ASN1T_MsgLogMaintenanceEtat* pvalue);

EXTERN void asn1Print_MsgLogMaintenanceEtat (ASN1ConstCharPtr name, ASN1T_MsgLogMaintenanceEtat* pvalue);

#endif

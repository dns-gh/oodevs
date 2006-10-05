/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 04-Oct-2006.
 */
#ifndef SCIPIOAUTOMATEMISSION_LOG_H
#define SCIPIOAUTOMATEMISSION_LOG_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  Mission_Automate_LOG_SeDeployer                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_LOG_SeDeployer {
   struct {
      unsigned position_deploiementPresent : 1;
   } m;
   ASN1T_Point  position_deploiement;

   ASN1T_Mission_Automate_LOG_SeDeployer () {
      m.position_deploiementPresent = 0;
   }
} ASN1T_Mission_Automate_LOG_SeDeployer;

class EXTERN ASN1C_Mission_Automate_LOG_SeDeployer : public ASN1CType {
public:
   ASN1T_Mission_Automate_LOG_SeDeployer& msgData;
   ASN1C_Mission_Automate_LOG_SeDeployer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_LOG_SeDeployer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_LOG_SeDeployer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_SeDeployer* pvalue);
EXTERN int asn1PD_Mission_Automate_LOG_SeDeployer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_SeDeployer* pvalue);

EXTERN void asn1Print_Mission_Automate_LOG_SeDeployer (ASN1ConstCharPtr name, ASN1T_Mission_Automate_LOG_SeDeployer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_LOG_AppuyerMouvement                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_LOG_AppuyerMouvement {
   ASN1T_ListAgent  unites_a_appuyer;
} ASN1T_Mission_Automate_LOG_AppuyerMouvement;

class EXTERN ASN1C_Mission_Automate_LOG_AppuyerMouvement : public ASN1CType {
public:
   ASN1T_Mission_Automate_LOG_AppuyerMouvement& msgData;
   ASN1C_Mission_Automate_LOG_AppuyerMouvement (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_LOG_AppuyerMouvement& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_LOG_AppuyerMouvement (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_AppuyerMouvement* pvalue);
EXTERN int asn1PD_Mission_Automate_LOG_AppuyerMouvement (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_AppuyerMouvement* pvalue);

EXTERN void asn1Print_Mission_Automate_LOG_AppuyerMouvement (ASN1ConstCharPtr name, ASN1T_Mission_Automate_LOG_AppuyerMouvement* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_LOG_ReconnaitreItineraire                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_LOG_ReconnaitreItineraire {
   ASN1T_Itineraire  itineraire_log;
} ASN1T_Mission_Automate_LOG_ReconnaitreItineraire;

class EXTERN ASN1C_Mission_Automate_LOG_ReconnaitreItineraire : public ASN1CType {
public:
   ASN1T_Mission_Automate_LOG_ReconnaitreItineraire& msgData;
   ASN1C_Mission_Automate_LOG_ReconnaitreItineraire (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_LOG_ReconnaitreItineraire& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_LOG_ReconnaitreItineraire (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_ReconnaitreItineraire* pvalue);
EXTERN int asn1PD_Mission_Automate_LOG_ReconnaitreItineraire (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_ReconnaitreItineraire* pvalue);

EXTERN void asn1Print_Mission_Automate_LOG_ReconnaitreItineraire (ASN1ConstCharPtr name, ASN1T_Mission_Automate_LOG_ReconnaitreItineraire* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_LOG_Surveiller                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_LOG_Surveiller {
   ASN1T_Polygon  zone;
} ASN1T_Mission_Automate_LOG_Surveiller;

class EXTERN ASN1C_Mission_Automate_LOG_Surveiller : public ASN1CType {
public:
   ASN1T_Mission_Automate_LOG_Surveiller& msgData;
   ASN1C_Mission_Automate_LOG_Surveiller (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_LOG_Surveiller& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_LOG_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_Surveiller* pvalue);
EXTERN int asn1PD_Mission_Automate_LOG_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_Surveiller* pvalue);

EXTERN void asn1Print_Mission_Automate_LOG_Surveiller (ASN1ConstCharPtr name, ASN1T_Mission_Automate_LOG_Surveiller* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_LOG_TransporterUnites                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_LOG_TransporterUnites {
   ASN1T_ListAgent  unites_a_transporter;
   ASN1T_Point  point_rendez_vous;
   ASN1T_Point  point_destination;
} ASN1T_Mission_Automate_LOG_TransporterUnites;

class EXTERN ASN1C_Mission_Automate_LOG_TransporterUnites : public ASN1CType {
public:
   ASN1T_Mission_Automate_LOG_TransporterUnites& msgData;
   ASN1C_Mission_Automate_LOG_TransporterUnites (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_LOG_TransporterUnites& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_LOG_TransporterUnites (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_TransporterUnites* pvalue);
EXTERN int asn1PD_Mission_Automate_LOG_TransporterUnites (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_TransporterUnites* pvalue);

EXTERN void asn1Print_Mission_Automate_LOG_TransporterUnites (ASN1ConstCharPtr name, ASN1T_Mission_Automate_LOG_TransporterUnites* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_LOG_AppuyerMouvementDansZone             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_LOG_AppuyerMouvementDansZone {
   ASN1T_Polygon  zone;
} ASN1T_Mission_Automate_LOG_AppuyerMouvementDansZone;

class EXTERN ASN1C_Mission_Automate_LOG_AppuyerMouvementDansZone : public ASN1CType {
public:
   ASN1T_Mission_Automate_LOG_AppuyerMouvementDansZone& msgData;
   ASN1C_Mission_Automate_LOG_AppuyerMouvementDansZone (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_LOG_AppuyerMouvementDansZone& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_LOG_AppuyerMouvementDansZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_AppuyerMouvementDansZone* pvalue);
EXTERN int asn1PD_Mission_Automate_LOG_AppuyerMouvementDansZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_AppuyerMouvementDansZone* pvalue);

EXTERN void asn1Print_Mission_Automate_LOG_AppuyerMouvementDansZone (ASN1ConstCharPtr name, ASN1T_Mission_Automate_LOG_AppuyerMouvementDansZone* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_LOG_MettreEnOeuvreZoneStationnement      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement {
   ASN1T_Polygon  zone;
} ASN1T_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement;

class EXTERN ASN1C_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement : public ASN1CType {
public:
   ASN1T_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement& msgData;
   ASN1C_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement* pvalue);
EXTERN int asn1PD_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement* pvalue);

EXTERN void asn1Print_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement (ASN1ConstCharPtr name, ASN1T_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_LOG_AppuyerFranchissementDansZone        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_LOG_AppuyerFranchissementDansZone {
   ASN1T_Polygon  zone;
} ASN1T_Mission_Automate_LOG_AppuyerFranchissementDansZone;

class EXTERN ASN1C_Mission_Automate_LOG_AppuyerFranchissementDansZone : public ASN1CType {
public:
   ASN1T_Mission_Automate_LOG_AppuyerFranchissementDansZone& msgData;
   ASN1C_Mission_Automate_LOG_AppuyerFranchissementDansZone (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_LOG_AppuyerFranchissementDansZone& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_LOG_AppuyerFranchissementDansZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_AppuyerFranchissementDansZone* pvalue);
EXTERN int asn1PD_Mission_Automate_LOG_AppuyerFranchissementDansZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_AppuyerFranchissementDansZone* pvalue);

EXTERN void asn1Print_Mission_Automate_LOG_AppuyerFranchissementDansZone (ASN1ConstCharPtr name, ASN1T_Mission_Automate_LOG_AppuyerFranchissementDansZone* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_LOG_BloquerZone                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_LOG_BloquerZone {
   ASN1T_Polygon  zone;
} ASN1T_Mission_Automate_LOG_BloquerZone;

class EXTERN ASN1C_Mission_Automate_LOG_BloquerZone : public ASN1CType {
public:
   ASN1T_Mission_Automate_LOG_BloquerZone& msgData;
   ASN1C_Mission_Automate_LOG_BloquerZone (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_LOG_BloquerZone& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_LOG_BloquerZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_BloquerZone* pvalue);
EXTERN int asn1PD_Mission_Automate_LOG_BloquerZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_BloquerZone* pvalue);

EXTERN void asn1Print_Mission_Automate_LOG_BloquerZone (ASN1ConstCharPtr name, ASN1T_Mission_Automate_LOG_BloquerZone* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_LOG_ReconnaitreZoneContaminee            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_LOG_ReconnaitreZoneContaminee {
   ASN1T_Polygon  zone;
} ASN1T_Mission_Automate_LOG_ReconnaitreZoneContaminee;

class EXTERN ASN1C_Mission_Automate_LOG_ReconnaitreZoneContaminee : public ASN1CType {
public:
   ASN1T_Mission_Automate_LOG_ReconnaitreZoneContaminee& msgData;
   ASN1C_Mission_Automate_LOG_ReconnaitreZoneContaminee (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_LOG_ReconnaitreZoneContaminee& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_LOG_ReconnaitreZoneContaminee (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_ReconnaitreZoneContaminee* pvalue);
EXTERN int asn1PD_Mission_Automate_LOG_ReconnaitreZoneContaminee (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_ReconnaitreZoneContaminee* pvalue);

EXTERN void asn1Print_Mission_Automate_LOG_ReconnaitreZoneContaminee (ASN1ConstCharPtr name, ASN1T_Mission_Automate_LOG_ReconnaitreZoneContaminee* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_LOG_ReconnaitreZoneDeDeploiement         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement {
   ASN1T_Polygon  zone;
} ASN1T_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement;

class EXTERN ASN1C_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement : public ASN1CType {
public:
   ASN1T_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement& msgData;
   ASN1C_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement* pvalue);
EXTERN int asn1PD_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement* pvalue);

EXTERN void asn1Print_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement (ASN1ConstCharPtr name, ASN1T_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_LOG_ArmerPIAs                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_LOG_ArmerPIAs {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Automate_LOG_ArmerPIAs;

class EXTERN ASN1C_Mission_Automate_LOG_ArmerPIAs : public ASN1CType {
public:
   ASN1T_Mission_Automate_LOG_ArmerPIAs& msgData;
   ASN1C_Mission_Automate_LOG_ArmerPIAs (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_LOG_ArmerPIAs& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_LOG_ArmerPIAs (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_ArmerPIAs* pvalue);
EXTERN int asn1PD_Mission_Automate_LOG_ArmerPIAs (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_ArmerPIAs* pvalue);

EXTERN void asn1Print_Mission_Automate_LOG_ArmerPIAs (ASN1ConstCharPtr name, ASN1T_Mission_Automate_LOG_ArmerPIAs* pvalue);

#endif

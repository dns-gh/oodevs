/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 20-Oct-2005.
 */
#ifndef SCIPIOAUTOMATEMISSION_GEN_H
#define SCIPIOAUTOMATEMISSION_GEN_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"
#include "ScipioObject.h"

/**************************************************************/
/*                                                            */
/*  EnumMissionGenPriorite                                    */
/*                                                            */
/**************************************************************/

struct EXTERN EnumMissionGenPriorite {
   enum Root {
      prioritaire = 0,
      normal = 1,
      peu_prioritaire = 2
   } ;
} ;

typedef EnumMissionGenPriorite::Root ASN1T_EnumMissionGenPriorite;

class EXTERN ASN1C_EnumMissionGenPriorite : public ASN1CType {
public:
   ASN1T_EnumMissionGenPriorite& msgData;
   ASN1C_EnumMissionGenPriorite (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumMissionGenPriorite& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumMissionGenPriorite (ASN1CTXT* ctxt_p, ASN1T_EnumMissionGenPriorite value);
EXTERN int asn1PD_EnumMissionGenPriorite (ASN1CTXT* ctxt_p, ASN1T_EnumMissionGenPriorite* pvalue);

EXTERN void asn1Print_EnumMissionGenPriorite (ASN1ConstCharPtr name, ASN1T_EnumMissionGenPriorite* pvalue);

/**************************************************************/
/*                                                            */
/*  MissionGenObject                                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MissionGenObject {
   ASN1T_EnumObjectType  type_obstacle;
   ASN1T_Localisation  pos_obstacle;
   ASN1T_OID  oid_obstacle_planifie;
   ASN1T_EnumMissionGenUrgence  urgence;
   ASN1T_EnumMissionGenSousTypeObstacle  preliminaire;
   ASN1T_EnumMissionGenPriorite  priorite;

   ASN1T_MissionGenObject () {
      urgence = EnumMissionGenUrgence::serie_complementaire;
      preliminaire = EnumMissionGenSousTypeObstacle::preliminaire;
   }
} ASN1T_MissionGenObject;

class EXTERN ASN1C_MissionGenObject : public ASN1CType {
public:
   ASN1T_MissionGenObject& msgData;
   ASN1C_MissionGenObject (
      ASN1MessageBuffer& msgBuf, ASN1T_MissionGenObject& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MissionGenObject (ASN1CTXT* ctxt_p, ASN1T_MissionGenObject* pvalue);
EXTERN int asn1PD_MissionGenObject (ASN1CTXT* ctxt_p, ASN1T_MissionGenObject* pvalue);

EXTERN void asn1Print_MissionGenObject (ASN1ConstCharPtr name, ASN1T_MissionGenObject* pvalue);

/**************************************************************/
/*                                                            */
/*  ListMissionGenObject                                      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ListMissionGenObject {
   ASN1UINT n;
   ASN1T_MissionGenObject *elem;
} ASN1T_ListMissionGenObject;

class EXTERN ASN1C_ListMissionGenObject : public ASN1CType {
public:
   ASN1T_ListMissionGenObject& msgData;
   ASN1C_ListMissionGenObject (
      ASN1MessageBuffer& msgBuf, ASN1T_ListMissionGenObject& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ListMissionGenObject (ASN1CTXT* ctxt_p, ASN1T_ListMissionGenObject* pvalue);
EXTERN int asn1PD_ListMissionGenObject (ASN1CTXT* ctxt_p, ASN1T_ListMissionGenObject* pvalue);

EXTERN void asn1Print_ListMissionGenObject (ASN1ConstCharPtr name, ASN1T_ListMissionGenObject* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_GEN_SeDeployer                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_GEN_SeDeployer {
   ASN1T_Polygon  zone_deploiement;
} ASN1T_Mission_Automate_GEN_SeDeployer;

class EXTERN ASN1C_Mission_Automate_GEN_SeDeployer : public ASN1CType {
public:
   ASN1T_Mission_Automate_GEN_SeDeployer& msgData;
   ASN1C_Mission_Automate_GEN_SeDeployer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_GEN_SeDeployer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_GEN_SeDeployer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_SeDeployer* pvalue);
EXTERN int asn1PD_Mission_Automate_GEN_SeDeployer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_SeDeployer* pvalue);

EXTERN void asn1Print_Mission_Automate_GEN_SeDeployer (ASN1ConstCharPtr name, ASN1T_Mission_Automate_GEN_SeDeployer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_GEN_FaireFranchir                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_GEN_FaireFranchir {
   ASN1T_ListKnowledgeObject  sites_de_franchissement;
   ASN1T_Localisation  zone_regroupement;
} ASN1T_Mission_Automate_GEN_FaireFranchir;

class EXTERN ASN1C_Mission_Automate_GEN_FaireFranchir : public ASN1CType {
public:
   ASN1T_Mission_Automate_GEN_FaireFranchir& msgData;
   ASN1C_Mission_Automate_GEN_FaireFranchir (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_GEN_FaireFranchir& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_GEN_FaireFranchir (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_FaireFranchir* pvalue);
EXTERN int asn1PD_Mission_Automate_GEN_FaireFranchir (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_FaireFranchir* pvalue);

EXTERN void asn1Print_Mission_Automate_GEN_FaireFranchir (ASN1ConstCharPtr name, ASN1T_Mission_Automate_GEN_FaireFranchir* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_GEN_RealiserSystemeObstacles             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_GEN_RealiserSystemeObstacles {
   ASN1T_ListMissionGenObject  obstacles;
} ASN1T_Mission_Automate_GEN_RealiserSystemeObstacles;

class EXTERN ASN1C_Mission_Automate_GEN_RealiserSystemeObstacles : public ASN1CType {
public:
   ASN1T_Mission_Automate_GEN_RealiserSystemeObstacles& msgData;
   ASN1C_Mission_Automate_GEN_RealiserSystemeObstacles (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_GEN_RealiserSystemeObstacles& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_GEN_RealiserSystemeObstacles (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_RealiserSystemeObstacles* pvalue);
EXTERN int asn1PD_Mission_Automate_GEN_RealiserSystemeObstacles (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_RealiserSystemeObstacles* pvalue);

EXTERN void asn1Print_Mission_Automate_GEN_RealiserSystemeObstacles (ASN1ConstCharPtr name, ASN1T_Mission_Automate_GEN_RealiserSystemeObstacles* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_GEN_FranchirDeViveForce                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_GEN_FranchirDeViveForce {
   ASN1T_ListKnowledgeObject  obstacles_a_franchir;
} ASN1T_Mission_Automate_GEN_FranchirDeViveForce;

class EXTERN ASN1C_Mission_Automate_GEN_FranchirDeViveForce : public ASN1CType {
public:
   ASN1T_Mission_Automate_GEN_FranchirDeViveForce& msgData;
   ASN1C_Mission_Automate_GEN_FranchirDeViveForce (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_GEN_FranchirDeViveForce& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_GEN_FranchirDeViveForce (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_FranchirDeViveForce* pvalue);
EXTERN int asn1PD_Mission_Automate_GEN_FranchirDeViveForce (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_FranchirDeViveForce* pvalue);

EXTERN void asn1Print_Mission_Automate_GEN_FranchirDeViveForce (ASN1ConstCharPtr name, ASN1T_Mission_Automate_GEN_FranchirDeViveForce* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_GEN_RetablirItineraires                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_GEN_RetablirItineraires {
   ASN1T_ListItineraire  itineraires;
} ASN1T_Mission_Automate_GEN_RetablirItineraires;

class EXTERN ASN1C_Mission_Automate_GEN_RetablirItineraires : public ASN1CType {
public:
   ASN1T_Mission_Automate_GEN_RetablirItineraires& msgData;
   ASN1C_Mission_Automate_GEN_RetablirItineraires (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_GEN_RetablirItineraires& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_GEN_RetablirItineraires (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_RetablirItineraires* pvalue);
EXTERN int asn1PD_Mission_Automate_GEN_RetablirItineraires (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_RetablirItineraires* pvalue);

EXTERN void asn1Print_Mission_Automate_GEN_RetablirItineraires (ASN1ConstCharPtr name, ASN1T_Mission_Automate_GEN_RetablirItineraires* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_GEN_AmenagerAireStationnement            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_GEN_AmenagerAireStationnement {
   ASN1T_ListMissionGenObject  travaux;
} ASN1T_Mission_Automate_GEN_AmenagerAireStationnement;

class EXTERN ASN1C_Mission_Automate_GEN_AmenagerAireStationnement : public ASN1CType {
public:
   ASN1T_Mission_Automate_GEN_AmenagerAireStationnement& msgData;
   ASN1C_Mission_Automate_GEN_AmenagerAireStationnement (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_GEN_AmenagerAireStationnement& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_GEN_AmenagerAireStationnement (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_AmenagerAireStationnement* pvalue);
EXTERN int asn1PD_Mission_Automate_GEN_AmenagerAireStationnement (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_AmenagerAireStationnement* pvalue);

EXTERN void asn1Print_Mission_Automate_GEN_AmenagerAireStationnement (ASN1ConstCharPtr name, ASN1T_Mission_Automate_GEN_AmenagerAireStationnement* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_GEN_AmenagerTerrainPoserSommaire         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_GEN_AmenagerTerrainPoserSommaire {
   ASN1T_MissionGenObject  terrain;
} ASN1T_Mission_Automate_GEN_AmenagerTerrainPoserSommaire;

class EXTERN ASN1C_Mission_Automate_GEN_AmenagerTerrainPoserSommaire : public ASN1CType {
public:
   ASN1T_Mission_Automate_GEN_AmenagerTerrainPoserSommaire& msgData;
   ASN1C_Mission_Automate_GEN_AmenagerTerrainPoserSommaire (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_GEN_AmenagerTerrainPoserSommaire& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_GEN_AmenagerTerrainPoserSommaire (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_AmenagerTerrainPoserSommaire* pvalue);
EXTERN int asn1PD_Mission_Automate_GEN_AmenagerTerrainPoserSommaire (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_AmenagerTerrainPoserSommaire* pvalue);

EXTERN void asn1Print_Mission_Automate_GEN_AmenagerTerrainPoserSommaire (ASN1ConstCharPtr name, ASN1T_Mission_Automate_GEN_AmenagerTerrainPoserSommaire* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_GEN_RealiserZonePoser                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_GEN_RealiserZonePoser {
   ASN1T_MissionGenObject  zone;
} ASN1T_Mission_Automate_GEN_RealiserZonePoser;

class EXTERN ASN1C_Mission_Automate_GEN_RealiserZonePoser : public ASN1CType {
public:
   ASN1T_Mission_Automate_GEN_RealiserZonePoser& msgData;
   ASN1C_Mission_Automate_GEN_RealiserZonePoser (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_GEN_RealiserZonePoser& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_GEN_RealiserZonePoser (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_RealiserZonePoser* pvalue);
EXTERN int asn1PD_Mission_Automate_GEN_RealiserZonePoser (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_RealiserZonePoser* pvalue);

EXTERN void asn1Print_Mission_Automate_GEN_RealiserZonePoser (ASN1ConstCharPtr name, ASN1T_Mission_Automate_GEN_RealiserZonePoser* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_GEN_ReconnaitreZoneDeploiement           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_GEN_ReconnaitreZoneDeploiement {
   ASN1T_Polygon  zone;
} ASN1T_Mission_Automate_GEN_ReconnaitreZoneDeploiement;

class EXTERN ASN1C_Mission_Automate_GEN_ReconnaitreZoneDeploiement : public ASN1CType {
public:
   ASN1T_Mission_Automate_GEN_ReconnaitreZoneDeploiement& msgData;
   ASN1C_Mission_Automate_GEN_ReconnaitreZoneDeploiement (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_GEN_ReconnaitreZoneDeploiement& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_GEN_ReconnaitreZoneDeploiement (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_ReconnaitreZoneDeploiement* pvalue);
EXTERN int asn1PD_Mission_Automate_GEN_ReconnaitreZoneDeploiement (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_ReconnaitreZoneDeploiement* pvalue);

EXTERN void asn1Print_Mission_Automate_GEN_ReconnaitreZoneDeploiement (ASN1ConstCharPtr name, ASN1T_Mission_Automate_GEN_ReconnaitreZoneDeploiement* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_GEN_RealiserCampPrisonniers              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_GEN_RealiserCampPrisonniers {
   ASN1T_Polygon  camp;
   ASN1T_Automate  tc2;
} ASN1T_Mission_Automate_GEN_RealiserCampPrisonniers;

class EXTERN ASN1C_Mission_Automate_GEN_RealiserCampPrisonniers : public ASN1CType {
public:
   ASN1T_Mission_Automate_GEN_RealiserCampPrisonniers& msgData;
   ASN1C_Mission_Automate_GEN_RealiserCampPrisonniers (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_GEN_RealiserCampPrisonniers& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_GEN_RealiserCampPrisonniers (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_RealiserCampPrisonniers* pvalue);
EXTERN int asn1PD_Mission_Automate_GEN_RealiserCampPrisonniers (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_RealiserCampPrisonniers* pvalue);

EXTERN void asn1Print_Mission_Automate_GEN_RealiserCampPrisonniers (ASN1ConstCharPtr name, ASN1T_Mission_Automate_GEN_RealiserCampPrisonniers* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_GEN_RealiserCampRefugies                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_GEN_RealiserCampRefugies {
   ASN1T_Polygon  camp;
   ASN1T_Automate  tc2;
} ASN1T_Mission_Automate_GEN_RealiserCampRefugies;

class EXTERN ASN1C_Mission_Automate_GEN_RealiserCampRefugies : public ASN1CType {
public:
   ASN1T_Mission_Automate_GEN_RealiserCampRefugies& msgData;
   ASN1C_Mission_Automate_GEN_RealiserCampRefugies (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_GEN_RealiserCampRefugies& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_GEN_RealiserCampRefugies (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_RealiserCampRefugies* pvalue);
EXTERN int asn1PD_Mission_Automate_GEN_RealiserCampRefugies (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_RealiserCampRefugies* pvalue);

EXTERN void asn1Print_Mission_Automate_GEN_RealiserCampRefugies (ASN1ConstCharPtr name, ASN1T_Mission_Automate_GEN_RealiserCampRefugies* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_GEN_RealiserPlatesformesSommairesDeDepl  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial {
   ASN1T_ListMissionGenObject  plates_formes;
} ASN1T_Mission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial;

class EXTERN ASN1C_Mission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial : public ASN1CType {
public:
   ASN1T_Mission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial& msgData;
   ASN1C_Mission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial* pvalue);
EXTERN int asn1PD_Mission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial* pvalue);

EXTERN void asn1Print_Mission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial (ASN1ConstCharPtr name, ASN1T_Mission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_GEN_RealiserTravauxSommairesAuProfitPop  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation {
   ASN1T_ListMissionGenObject  travaux;
} ASN1T_Mission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation;

class EXTERN ASN1C_Mission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation : public ASN1CType {
public:
   ASN1T_Mission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation& msgData;
   ASN1C_Mission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation* pvalue);
EXTERN int asn1PD_Mission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation* pvalue);

EXTERN void asn1Print_Mission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation (ASN1ConstCharPtr name, ASN1T_Mission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_GEN_AppuyerDebarquementTroupes           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_GEN_AppuyerDebarquementTroupes {
   ASN1T_ListMissionGenObject  zones_debarquement;
} ASN1T_Mission_Automate_GEN_AppuyerDebarquementTroupes;

class EXTERN ASN1C_Mission_Automate_GEN_AppuyerDebarquementTroupes : public ASN1CType {
public:
   ASN1T_Mission_Automate_GEN_AppuyerDebarquementTroupes& msgData;
   ASN1C_Mission_Automate_GEN_AppuyerDebarquementTroupes (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_GEN_AppuyerDebarquementTroupes& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_GEN_AppuyerDebarquementTroupes (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_AppuyerDebarquementTroupes* pvalue);
EXTERN int asn1PD_Mission_Automate_GEN_AppuyerDebarquementTroupes (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_AppuyerDebarquementTroupes* pvalue);

EXTERN void asn1Print_Mission_Automate_GEN_AppuyerDebarquementTroupes (ASN1ConstCharPtr name, ASN1T_Mission_Automate_GEN_AppuyerDebarquementTroupes* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_GEN_OuvrirAxesProgression                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_GEN_OuvrirAxesProgression {
   ASN1T_ListItineraire  axes;
} ASN1T_Mission_Automate_GEN_OuvrirAxesProgression;

class EXTERN ASN1C_Mission_Automate_GEN_OuvrirAxesProgression : public ASN1CType {
public:
   ASN1T_Mission_Automate_GEN_OuvrirAxesProgression& msgData;
   ASN1C_Mission_Automate_GEN_OuvrirAxesProgression (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_GEN_OuvrirAxesProgression& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_GEN_OuvrirAxesProgression (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_OuvrirAxesProgression* pvalue);
EXTERN int asn1PD_Mission_Automate_GEN_OuvrirAxesProgression (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_GEN_OuvrirAxesProgression* pvalue);

EXTERN void asn1Print_Mission_Automate_GEN_OuvrirAxesProgression (ASN1ConstCharPtr name, ASN1T_Mission_Automate_GEN_OuvrirAxesProgression* pvalue);

#endif

/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 22-Nov-2005.
 */
#ifndef SCIPIOAUTOMATEMISSION_NBC_H
#define SCIPIOAUTOMATEMISSION_NBC_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  Mission_Automate_NBC_ReconnaitreUnAxe                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_NBC_ReconnaitreUnAxe {
   ASN1OCTET __dummy__;
} ASN1T_Mission_Automate_NBC_ReconnaitreUnAxe;

class EXTERN ASN1C_Mission_Automate_NBC_ReconnaitreUnAxe : public ASN1CType {
public:
   ASN1T_Mission_Automate_NBC_ReconnaitreUnAxe& msgData;
   ASN1C_Mission_Automate_NBC_ReconnaitreUnAxe (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_NBC_ReconnaitreUnAxe& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_NBC_ReconnaitreUnAxe (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_NBC_ReconnaitreUnAxe* pvalue);
EXTERN int asn1PD_Mission_Automate_NBC_ReconnaitreUnAxe (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_NBC_ReconnaitreUnAxe* pvalue);

EXTERN void asn1Print_Mission_Automate_NBC_ReconnaitreUnAxe (ASN1ConstCharPtr name, ASN1T_Mission_Automate_NBC_ReconnaitreUnAxe* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_NBC_ReconnaitreUneZone                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_NBC_ReconnaitreUneZone {
   ASN1T_Polygon  zone;
} ASN1T_Mission_Automate_NBC_ReconnaitreUneZone;

class EXTERN ASN1C_Mission_Automate_NBC_ReconnaitreUneZone : public ASN1CType {
public:
   ASN1T_Mission_Automate_NBC_ReconnaitreUneZone& msgData;
   ASN1C_Mission_Automate_NBC_ReconnaitreUneZone (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_NBC_ReconnaitreUneZone& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_NBC_ReconnaitreUneZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_NBC_ReconnaitreUneZone* pvalue);
EXTERN int asn1PD_Mission_Automate_NBC_ReconnaitreUneZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_NBC_ReconnaitreUneZone* pvalue);

EXTERN void asn1Print_Mission_Automate_NBC_ReconnaitreUneZone (ASN1ConstCharPtr name, ASN1T_Mission_Automate_NBC_ReconnaitreUneZone* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_NBC_ArmerUnSiteDeDecontamination         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_NBC_ArmerUnSiteDeDecontamination {
   ASN1T_Polygon  site;
} ASN1T_Mission_Automate_NBC_ArmerUnSiteDeDecontamination;

class EXTERN ASN1C_Mission_Automate_NBC_ArmerUnSiteDeDecontamination : public ASN1CType {
public:
   ASN1T_Mission_Automate_NBC_ArmerUnSiteDeDecontamination& msgData;
   ASN1C_Mission_Automate_NBC_ArmerUnSiteDeDecontamination (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_NBC_ArmerUnSiteDeDecontamination& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_NBC_ArmerUnSiteDeDecontamination (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_NBC_ArmerUnSiteDeDecontamination* pvalue);
EXTERN int asn1PD_Mission_Automate_NBC_ArmerUnSiteDeDecontamination (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_NBC_ArmerUnSiteDeDecontamination* pvalue);

EXTERN void asn1Print_Mission_Automate_NBC_ArmerUnSiteDeDecontamination (ASN1ConstCharPtr name, ASN1T_Mission_Automate_NBC_ArmerUnSiteDeDecontamination* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_NBC_DecontaminerUneZone                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_NBC_DecontaminerUneZone {
   ASN1T_Polygon  zone;
} ASN1T_Mission_Automate_NBC_DecontaminerUneZone;

class EXTERN ASN1C_Mission_Automate_NBC_DecontaminerUneZone : public ASN1CType {
public:
   ASN1T_Mission_Automate_NBC_DecontaminerUneZone& msgData;
   ASN1C_Mission_Automate_NBC_DecontaminerUneZone (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_NBC_DecontaminerUneZone& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_NBC_DecontaminerUneZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_NBC_DecontaminerUneZone* pvalue);
EXTERN int asn1PD_Mission_Automate_NBC_DecontaminerUneZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_NBC_DecontaminerUneZone* pvalue);

EXTERN void asn1Print_Mission_Automate_NBC_DecontaminerUneZone (ASN1ConstCharPtr name, ASN1T_Mission_Automate_NBC_DecontaminerUneZone* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_NBC_ReconnaitreDesSitesROTA              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_NBC_ReconnaitreDesSitesROTA {
   ASN1T_ListKnowledgeObject  sites_ROTA;
} ASN1T_Mission_Automate_NBC_ReconnaitreDesSitesROTA;

class EXTERN ASN1C_Mission_Automate_NBC_ReconnaitreDesSitesROTA : public ASN1CType {
public:
   ASN1T_Mission_Automate_NBC_ReconnaitreDesSitesROTA& msgData;
   ASN1C_Mission_Automate_NBC_ReconnaitreDesSitesROTA (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_NBC_ReconnaitreDesSitesROTA& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_NBC_ReconnaitreDesSitesROTA (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_NBC_ReconnaitreDesSitesROTA* pvalue);
EXTERN int asn1PD_Mission_Automate_NBC_ReconnaitreDesSitesROTA (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_NBC_ReconnaitreDesSitesROTA* pvalue);

EXTERN void asn1Print_Mission_Automate_NBC_ReconnaitreDesSitesROTA (ASN1ConstCharPtr name, ASN1T_Mission_Automate_NBC_ReconnaitreDesSitesROTA* pvalue);

#endif

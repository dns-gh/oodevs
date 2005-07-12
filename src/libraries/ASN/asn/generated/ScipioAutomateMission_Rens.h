/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 04-Jul-2005.
 */
#ifndef SCIPIOAUTOMATEMISSION_RENS_H
#define SCIPIOAUTOMATEMISSION_RENS_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  Mission_Automate_RENS_ROHUM_RenseignerSur                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_RENS_ROHUM_RenseignerSur {
   ASN1T_Polygon  zone_a_observer;
} ASN1T_Mission_Automate_RENS_ROHUM_RenseignerSur;

class EXTERN ASN1C_Mission_Automate_RENS_ROHUM_RenseignerSur : public ASN1CType {
public:
   ASN1T_Mission_Automate_RENS_ROHUM_RenseignerSur& msgData;
   ASN1C_Mission_Automate_RENS_ROHUM_RenseignerSur (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_RENS_ROHUM_RenseignerSur& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_RENS_ROHUM_RenseignerSur (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_ROHUM_RenseignerSur* pvalue);
EXTERN int asn1PD_Mission_Automate_RENS_ROHUM_RenseignerSur (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_ROHUM_RenseignerSur* pvalue);

EXTERN void asn1Print_Mission_Automate_RENS_ROHUM_RenseignerSur (ASN1ConstCharPtr name, ASN1T_Mission_Automate_RENS_ROHUM_RenseignerSur* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_RENS_ROHUM_OrienterGuider                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_RENS_ROHUM_OrienterGuider {
   ASN1T_Automate  ami;
} ASN1T_Mission_Automate_RENS_ROHUM_OrienterGuider;

class EXTERN ASN1C_Mission_Automate_RENS_ROHUM_OrienterGuider : public ASN1CType {
public:
   ASN1T_Mission_Automate_RENS_ROHUM_OrienterGuider& msgData;
   ASN1C_Mission_Automate_RENS_ROHUM_OrienterGuider (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_RENS_ROHUM_OrienterGuider& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_RENS_ROHUM_OrienterGuider (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_ROHUM_OrienterGuider* pvalue);
EXTERN int asn1PD_Mission_Automate_RENS_ROHUM_OrienterGuider (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_ROHUM_OrienterGuider* pvalue);

EXTERN void asn1Print_Mission_Automate_RENS_ROHUM_OrienterGuider (ASN1ConstCharPtr name, ASN1T_Mission_Automate_RENS_ROHUM_OrienterGuider* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_RENS_ROHUM_SExfiltrer                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_RENS_ROHUM_SExfiltrer {
   ASN1T_Polygon  zone_de_regroupement;
} ASN1T_Mission_Automate_RENS_ROHUM_SExfiltrer;

class EXTERN ASN1C_Mission_Automate_RENS_ROHUM_SExfiltrer : public ASN1CType {
public:
   ASN1T_Mission_Automate_RENS_ROHUM_SExfiltrer& msgData;
   ASN1C_Mission_Automate_RENS_ROHUM_SExfiltrer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_RENS_ROHUM_SExfiltrer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_RENS_ROHUM_SExfiltrer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_ROHUM_SExfiltrer* pvalue);
EXTERN int asn1PD_Mission_Automate_RENS_ROHUM_SExfiltrer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_ROHUM_SExfiltrer* pvalue);

EXTERN void asn1Print_Mission_Automate_RENS_ROHUM_SExfiltrer (ASN1ConstCharPtr name, ASN1T_Mission_Automate_RENS_ROHUM_SExfiltrer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_RENS_ROIM_CL289_RenseignerSur            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_RENS_ROIM_CL289_RenseignerSur {
   ASN1T_Itineraire  ficelle_de_vole;
   ASN1T_ListPolygon  zones_de_recherche;
} ASN1T_Mission_Automate_RENS_ROIM_CL289_RenseignerSur;

class EXTERN ASN1C_Mission_Automate_RENS_ROIM_CL289_RenseignerSur : public ASN1CType {
public:
   ASN1T_Mission_Automate_RENS_ROIM_CL289_RenseignerSur& msgData;
   ASN1C_Mission_Automate_RENS_ROIM_CL289_RenseignerSur (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_RENS_ROIM_CL289_RenseignerSur& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_RENS_ROIM_CL289_RenseignerSur (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_ROIM_CL289_RenseignerSur* pvalue);
EXTERN int asn1PD_Mission_Automate_RENS_ROIM_CL289_RenseignerSur (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_ROIM_CL289_RenseignerSur* pvalue);

EXTERN void asn1Print_Mission_Automate_RENS_ROIM_CL289_RenseignerSur (ASN1ConstCharPtr name, ASN1T_Mission_Automate_RENS_ROIM_CL289_RenseignerSur* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_RENS_ROIM_SDTI_RenseignerSur             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur {
   ASN1T_Itineraire  ficelle_de_vole;
} ASN1T_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur;

class EXTERN ASN1C_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur : public ASN1CType {
public:
   ASN1T_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur& msgData;
   ASN1C_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur* pvalue);
EXTERN int asn1PD_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur* pvalue);

EXTERN void asn1Print_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur (ASN1ConstCharPtr name, ASN1T_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_RENS_ROEM_Appuyer                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_RENS_ROEM_Appuyer {
   ASN1T_Point  position_deploiement;
   ASN1T_Point  point_regroupement;
} ASN1T_Mission_Automate_RENS_ROEM_Appuyer;

class EXTERN ASN1C_Mission_Automate_RENS_ROEM_Appuyer : public ASN1CType {
public:
   ASN1T_Mission_Automate_RENS_ROEM_Appuyer& msgData;
   ASN1C_Mission_Automate_RENS_ROEM_Appuyer (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_RENS_ROEM_Appuyer& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_RENS_ROEM_Appuyer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_ROEM_Appuyer* pvalue);
EXTERN int asn1PD_Mission_Automate_RENS_ROEM_Appuyer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_ROEM_Appuyer* pvalue);

EXTERN void asn1Print_Mission_Automate_RENS_ROEM_Appuyer (ASN1ConstCharPtr name, ASN1T_Mission_Automate_RENS_ROEM_Appuyer* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_RENS_ROEM_RenseignerSur                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_RENS_ROEM_RenseignerSur {
   ASN1T_Polygon  zone;
} ASN1T_Mission_Automate_RENS_ROEM_RenseignerSur;

class EXTERN ASN1C_Mission_Automate_RENS_ROEM_RenseignerSur : public ASN1CType {
public:
   ASN1T_Mission_Automate_RENS_ROEM_RenseignerSur& msgData;
   ASN1C_Mission_Automate_RENS_ROEM_RenseignerSur (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_RENS_ROEM_RenseignerSur& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_RENS_ROEM_RenseignerSur (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_ROEM_RenseignerSur* pvalue);
EXTERN int asn1PD_Mission_Automate_RENS_ROEM_RenseignerSur (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_ROEM_RenseignerSur* pvalue);

EXTERN void asn1Print_Mission_Automate_RENS_ROEM_RenseignerSur (ASN1ConstCharPtr name, ASN1T_Mission_Automate_RENS_ROEM_RenseignerSur* pvalue);

/**************************************************************/
/*                                                            */
/*  Mission_Automate_RENS_RADINT_RenseignerSur                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_RENS_RADINT_RenseignerSur {
   ASN1T_Polygon  zone;
} ASN1T_Mission_Automate_RENS_RADINT_RenseignerSur;

class EXTERN ASN1C_Mission_Automate_RENS_RADINT_RenseignerSur : public ASN1CType {
public:
   ASN1T_Mission_Automate_RENS_RADINT_RenseignerSur& msgData;
   ASN1C_Mission_Automate_RENS_RADINT_RenseignerSur (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_RENS_RADINT_RenseignerSur& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_RENS_RADINT_RenseignerSur (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_RADINT_RenseignerSur* pvalue);
EXTERN int asn1PD_Mission_Automate_RENS_RADINT_RenseignerSur (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_RADINT_RenseignerSur* pvalue);

EXTERN void asn1Print_Mission_Automate_RENS_RADINT_RenseignerSur (ASN1ConstCharPtr name, ASN1T_Mission_Automate_RENS_RADINT_RenseignerSur* pvalue);

#endif

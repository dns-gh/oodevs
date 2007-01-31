/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 31-Jan-2007.
 */
#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"

/**************************************************************/
/*                                                            */
/*  DataIncludes                                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_DataIncludes {
   ASN1OCTET __dummy__;
} ASN1T_DataIncludes;

class EXTERN ASN1C_DataIncludes : public ASN1CType {
public:
   ASN1T_DataIncludes& msgData;
   ASN1C_DataIncludes (
      ASN1MessageBuffer& msgBuf, ASN1T_DataIncludes& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_DataIncludes (ASN1CTXT* ctxt_p, ASN1T_DataIncludes* pvalue);
EXTERN int asn1PD_DataIncludes (ASN1CTXT* ctxt_p, ASN1T_DataIncludes* pvalue);

EXTERN void asn1Print_DataIncludes (ASN1ConstCharPtr name, ASN1T_DataIncludes* pvalue);

/**************************************************************/
/*                                                            */
/*  DataExports                                               */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_DataExports : public ASN1CType {
public:
   ASN1C_DataExports (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_DataExports (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_DataExports (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_DataExports (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  Dataset                                                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Dataset {
   ASN1VisibleString  name;
} ASN1T_Dataset;

class EXTERN ASN1C_Dataset : public ASN1CType {
public:
   ASN1T_Dataset& msgData;
   ASN1C_Dataset (
      ASN1MessageBuffer& msgBuf, ASN1T_Dataset& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Dataset (ASN1CTXT* ctxt_p, ASN1T_Dataset* pvalue);
EXTERN int asn1PD_Dataset (ASN1CTXT* ctxt_p, ASN1T_Dataset* pvalue);

EXTERN void asn1Print_Dataset (ASN1ConstCharPtr name, ASN1T_Dataset* pvalue);

/**************************************************************/
/*                                                            */
/*  Terrain                                                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Terrain {
   ASN1VisibleString  name;
} ASN1T_Terrain;

class EXTERN ASN1C_Terrain : public ASN1CType {
public:
   ASN1T_Terrain& msgData;
   ASN1C_Terrain (
      ASN1MessageBuffer& msgBuf, ASN1T_Terrain& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Terrain (ASN1CTXT* ctxt_p, ASN1T_Terrain* pvalue);
EXTERN int asn1PD_Terrain (ASN1CTXT* ctxt_p, ASN1T_Terrain* pvalue);

EXTERN void asn1Print_Terrain (ASN1ConstCharPtr name, ASN1T_Terrain* pvalue);

/**************************************************************/
/*                                                            */
/*  PhysicalModel                                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_PhysicalModel {
   ASN1VisibleString  name;
   ASN1VisibleString  dataset;
} ASN1T_PhysicalModel;

class EXTERN ASN1C_PhysicalModel : public ASN1CType {
public:
   ASN1T_PhysicalModel& msgData;
   ASN1C_PhysicalModel (
      ASN1MessageBuffer& msgBuf, ASN1T_PhysicalModel& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_PhysicalModel (ASN1CTXT* ctxt_p, ASN1T_PhysicalModel* pvalue);
EXTERN int asn1PD_PhysicalModel (ASN1CTXT* ctxt_p, ASN1T_PhysicalModel* pvalue);

EXTERN void asn1Print_PhysicalModel (ASN1ConstCharPtr name, ASN1T_PhysicalModel* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgDatasetCreation                                        */
/*                                                            */
/**************************************************************/

typedef ASN1T_Dataset ASN1T_MsgDatasetCreation;

class EXTERN ASN1C_MsgDatasetCreation : public ASN1CType {
public:
   ASN1T_MsgDatasetCreation& msgData;
   ASN1C_MsgDatasetCreation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgDatasetCreation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgDatasetCreation (ASN1CTXT* ctxt_p, ASN1T_MsgDatasetCreation* pvalue);
EXTERN int asn1PD_MsgDatasetCreation (ASN1CTXT* ctxt_p, ASN1T_MsgDatasetCreation* pvalue);

EXTERN void asn1Print_MsgDatasetCreation (ASN1ConstCharPtr name, ASN1T_MsgDatasetCreation* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgTerrainCreation                                        */
/*                                                            */
/**************************************************************/

typedef ASN1T_Terrain ASN1T_MsgTerrainCreation;

class EXTERN ASN1C_MsgTerrainCreation : public ASN1CType {
public:
   ASN1T_MsgTerrainCreation& msgData;
   ASN1C_MsgTerrainCreation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgTerrainCreation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgTerrainCreation (ASN1CTXT* ctxt_p, ASN1T_MsgTerrainCreation* pvalue);
EXTERN int asn1PD_MsgTerrainCreation (ASN1CTXT* ctxt_p, ASN1T_MsgTerrainCreation* pvalue);

EXTERN void asn1Print_MsgTerrainCreation (ASN1ConstCharPtr name, ASN1T_MsgTerrainCreation* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPhysicalModelCreation                                  */
/*                                                            */
/**************************************************************/

typedef ASN1T_PhysicalModel ASN1T_MsgPhysicalModelCreation;

class EXTERN ASN1C_MsgPhysicalModelCreation : public ASN1CType {
public:
   ASN1T_MsgPhysicalModelCreation& msgData;
   ASN1C_MsgPhysicalModelCreation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPhysicalModelCreation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPhysicalModelCreation (ASN1CTXT* ctxt_p, ASN1T_MsgPhysicalModelCreation* pvalue);
EXTERN int asn1PD_MsgPhysicalModelCreation (ASN1CTXT* ctxt_p, ASN1T_MsgPhysicalModelCreation* pvalue);

EXTERN void asn1Print_MsgPhysicalModelCreation (ASN1ConstCharPtr name, ASN1T_MsgPhysicalModelCreation* pvalue);

#endif

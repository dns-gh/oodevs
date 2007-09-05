/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 05-Sep-2007.
 */
#ifndef FOLK_H
#define FOLK_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "Base.h"

/**************************************************************/
/*                                                            */
/*  FolkIncludes                                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_FolkIncludes {
} ASN1T_FolkIncludes;

class EXTERN ASN1C_FolkIncludes : public ASN1CType {
public:
   ASN1T_FolkIncludes& msgData;
   ASN1C_FolkIncludes (
      ASN1MessageBuffer& msgBuf, ASN1T_FolkIncludes& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_FolkIncludes (ASN1CTXT* ctxt_p, ASN1T_FolkIncludes* pvalue);
EXTERN int asn1PD_FolkIncludes (ASN1CTXT* ctxt_p, ASN1T_FolkIncludes* pvalue);

EXTERN void asn1Print_FolkIncludes (ASN1ConstCharPtr name, ASN1T_FolkIncludes* pvalue);

/**************************************************************/
/*                                                            */
/*  FolkExports                                               */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_FolkExports : public ASN1CType {
public:
   ASN1C_FolkExports (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_FolkExports (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_FolkExports (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_FolkExports (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgFolkCreation_profiles                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgFolkCreation_profiles {
   ASN1UINT n;
   ASN1TDynOctStr *elem;
} ASN1T_MsgFolkCreation_profiles;

class EXTERN ASN1C_MsgFolkCreation_profiles : public ASN1CType {
public:
   ASN1T_MsgFolkCreation_profiles& msgData;
   ASN1C_MsgFolkCreation_profiles (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgFolkCreation_profiles& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgFolkCreation_profiles (ASN1CTXT* ctxt_p, ASN1T_MsgFolkCreation_profiles* pvalue);
EXTERN int asn1PD_MsgFolkCreation_profiles (ASN1CTXT* ctxt_p, ASN1T_MsgFolkCreation_profiles* pvalue);

EXTERN void asn1Print_MsgFolkCreation_profiles (ASN1ConstCharPtr name, ASN1T_MsgFolkCreation_profiles* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgFolkCreation_activities                                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgFolkCreation_activities {
   ASN1UINT n;
   ASN1TDynOctStr *elem;
} ASN1T_MsgFolkCreation_activities;

class EXTERN ASN1C_MsgFolkCreation_activities : public ASN1CType {
public:
   ASN1T_MsgFolkCreation_activities& msgData;
   ASN1C_MsgFolkCreation_activities (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgFolkCreation_activities& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgFolkCreation_activities (ASN1CTXT* ctxt_p, ASN1T_MsgFolkCreation_activities* pvalue);
EXTERN int asn1PD_MsgFolkCreation_activities (ASN1CTXT* ctxt_p, ASN1T_MsgFolkCreation_activities* pvalue);

EXTERN void asn1Print_MsgFolkCreation_activities (ASN1ConstCharPtr name, ASN1T_MsgFolkCreation_activities* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgFolkCreation                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgFolkCreation {
   ASN1T_MsgFolkCreation_profiles  profiles;
   ASN1T_MsgFolkCreation_activities  activities;
   ASN1INT   container_size;
   ASN1INT   edge_number;
} ASN1T_MsgFolkCreation;

class EXTERN ASN1C_MsgFolkCreation : public ASN1CType {
public:
   ASN1T_MsgFolkCreation& msgData;
   ASN1C_MsgFolkCreation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgFolkCreation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgFolkCreation (ASN1CTXT* ctxt_p, ASN1T_MsgFolkCreation* pvalue);
EXTERN int asn1PD_MsgFolkCreation (ASN1CTXT* ctxt_p, ASN1T_MsgFolkCreation* pvalue);

EXTERN void asn1Print_MsgFolkCreation (ASN1ConstCharPtr name, ASN1T_MsgFolkCreation* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgFolkGraphEdgeUpdate_population_occupation              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgFolkGraphEdgeUpdate_population_occupation {
   ASN1UINT n;
   ASN1INT  *elem;
} ASN1T_MsgFolkGraphEdgeUpdate_population_occupation;

class EXTERN ASN1C_MsgFolkGraphEdgeUpdate_population_occupation : public ASN1CType {
public:
   ASN1T_MsgFolkGraphEdgeUpdate_population_occupation& msgData;
   ASN1C_MsgFolkGraphEdgeUpdate_population_occupation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgFolkGraphEdgeUpdate_population_occupation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgFolkGraphEdgeUpdate_population_occupation (ASN1CTXT* ctxt_p, ASN1T_MsgFolkGraphEdgeUpdate_population_occupation* pvalue);
EXTERN int asn1PD_MsgFolkGraphEdgeUpdate_population_occupation (ASN1CTXT* ctxt_p, ASN1T_MsgFolkGraphEdgeUpdate_population_occupation* pvalue);

EXTERN void asn1Print_MsgFolkGraphEdgeUpdate_population_occupation (ASN1ConstCharPtr name, ASN1T_MsgFolkGraphEdgeUpdate_population_occupation* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgFolkGraphEdgeUpdate                                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgFolkGraphEdgeUpdate {
   ASN1T_OID  oid;
   ASN1T_OID  shp_oid;
   ASN1T_MsgFolkGraphEdgeUpdate_population_occupation  population_occupation;
} ASN1T_MsgFolkGraphEdgeUpdate;

class EXTERN ASN1C_MsgFolkGraphEdgeUpdate : public ASN1CType {
public:
   ASN1T_MsgFolkGraphEdgeUpdate& msgData;
   ASN1C_MsgFolkGraphEdgeUpdate (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgFolkGraphEdgeUpdate& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgFolkGraphEdgeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgFolkGraphEdgeUpdate* pvalue);
EXTERN int asn1PD_MsgFolkGraphEdgeUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgFolkGraphEdgeUpdate* pvalue);

EXTERN void asn1Print_MsgFolkGraphEdgeUpdate (ASN1ConstCharPtr name, ASN1T_MsgFolkGraphEdgeUpdate* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgFolkGraphUpdate                                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgFolkGraphUpdate {
   ASN1UINT n;
   ASN1T_MsgFolkGraphEdgeUpdate *elem;
} ASN1T_MsgFolkGraphUpdate;

class EXTERN ASN1C_MsgFolkGraphUpdate : public ASN1CType {
public:
   ASN1T_MsgFolkGraphUpdate& msgData;
   ASN1C_MsgFolkGraphUpdate (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgFolkGraphUpdate& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgFolkGraphUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgFolkGraphUpdate* pvalue);
EXTERN int asn1PD_MsgFolkGraphUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgFolkGraphUpdate* pvalue);

EXTERN void asn1Print_MsgFolkGraphUpdate (ASN1ConstCharPtr name, ASN1T_MsgFolkGraphUpdate* pvalue);

#endif

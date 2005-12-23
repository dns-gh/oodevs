/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 23-Dec-2005.
 */
#ifndef SCIPIOAUTOMATEMISSION_TRANS_H
#define SCIPIOAUTOMATEMISSION_TRANS_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  Mission_Automate_TRANS_Commander                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Automate_TRANS_Commander {
   ASN1T_Polygon  zone_deploiement;
} ASN1T_Mission_Automate_TRANS_Commander;

class EXTERN ASN1C_Mission_Automate_TRANS_Commander : public ASN1CType {
public:
   ASN1T_Mission_Automate_TRANS_Commander& msgData;
   ASN1C_Mission_Automate_TRANS_Commander (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_TRANS_Commander& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate_TRANS_Commander (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_TRANS_Commander* pvalue);
EXTERN int asn1PD_Mission_Automate_TRANS_Commander (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_TRANS_Commander* pvalue);

EXTERN void asn1Print_Mission_Automate_TRANS_Commander (ASN1ConstCharPtr name, ASN1T_Mission_Automate_TRANS_Commander* pvalue);

#endif

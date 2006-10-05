/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 04-Oct-2006.
 */
#ifndef SCIPIOPIONMISSION_TRANS_H
#define SCIPIOPIONMISSION_TRANS_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  Mission_Pion_TRANS_EtablirGererLiaison                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Pion_TRANS_EtablirGererLiaison {
   ASN1T_Point  position_deploiement;
} ASN1T_Mission_Pion_TRANS_EtablirGererLiaison;

class EXTERN ASN1C_Mission_Pion_TRANS_EtablirGererLiaison : public ASN1CType {
public:
   ASN1T_Mission_Pion_TRANS_EtablirGererLiaison& msgData;
   ASN1C_Mission_Pion_TRANS_EtablirGererLiaison (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_TRANS_EtablirGererLiaison& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion_TRANS_EtablirGererLiaison (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_TRANS_EtablirGererLiaison* pvalue);
EXTERN int asn1PD_Mission_Pion_TRANS_EtablirGererLiaison (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_TRANS_EtablirGererLiaison* pvalue);

EXTERN void asn1Print_Mission_Pion_TRANS_EtablirGererLiaison (ASN1ConstCharPtr name, ASN1T_Mission_Pion_TRANS_EtablirGererLiaison* pvalue);

#endif

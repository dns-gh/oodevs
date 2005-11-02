/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 28-Oct-2005.
 */
#ifndef SCIPIOPOPULATIONMISSION_H
#define SCIPIOPOPULATIONMISSION_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  Mission_Population_FaireMouvement                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_Mission_Population_FaireMouvement {
   ASN1T_Point  destination;
} ASN1T_Mission_Population_FaireMouvement;

class EXTERN ASN1C_Mission_Population_FaireMouvement : public ASN1CType {
public:
   ASN1T_Mission_Population_FaireMouvement& msgData;
   ASN1C_Mission_Population_FaireMouvement (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Population_FaireMouvement& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Population_FaireMouvement (ASN1CTXT* ctxt_p, ASN1T_Mission_Population_FaireMouvement* pvalue);
EXTERN int asn1PD_Mission_Population_FaireMouvement (ASN1CTXT* ctxt_p, ASN1T_Mission_Population_FaireMouvement* pvalue);

EXTERN void asn1Print_Mission_Population_FaireMouvement (ASN1ConstCharPtr name, ASN1T_Mission_Population_FaireMouvement* pvalue);

#endif

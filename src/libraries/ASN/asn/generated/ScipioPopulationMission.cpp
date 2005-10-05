/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 05-Oct-2005.
 */
#include "ScipioPopulationMission.h"

/**************************************************************/
/*                                                            */
/*  Mission_Population_FaireMouvement                         */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Population_FaireMouvement::ASN1C_Mission_Population_FaireMouvement (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Population_FaireMouvement& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Population_FaireMouvement (ASN1CTXT* ctxt_p, ASN1T_Mission_Population_FaireMouvement* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Population_FaireMouvement: start\n");

   /* encode destination */

   PU_PUSHNAME (ctxt_p, "destination");

   stat = asn1PE_Point (ctxt_p, &pvalue->destination);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Population_FaireMouvement: end\n");
   return (stat);
}

int ASN1C_Mission_Population_FaireMouvement::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Population_FaireMouvement (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Population_FaireMouvement (ASN1CTXT* ctxt_p, ASN1T_Mission_Population_FaireMouvement* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Population_FaireMouvement: start\n");

   /* decode destination */

   PU_PUSHNAME (ctxt_p, "destination");

   stat = asn1PD_Point (ctxt_p, &pvalue->destination);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Population_FaireMouvement: end\n");

   return (stat);
}

int ASN1C_Mission_Population_FaireMouvement::Decode ()
{
   return asn1PD_Mission_Population_FaireMouvement (mpContext->GetPtr(), &msgData);
}


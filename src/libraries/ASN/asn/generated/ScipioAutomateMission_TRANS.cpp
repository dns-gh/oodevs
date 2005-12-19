/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 19-Dec-2005.
 */
#include "ScipioAutomateMission_TRANS.h"

/**************************************************************/
/*                                                            */
/*  Mission_Automate_TRANS_Commander                          */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_TRANS_Commander::ASN1C_Mission_Automate_TRANS_Commander (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_TRANS_Commander& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_TRANS_Commander (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_TRANS_Commander* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_TRANS_Commander: start\n");

   /* encode zone_deploiement */

   PU_PUSHNAME (ctxt_p, "zone_deploiement");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_TRANS_Commander: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_TRANS_Commander::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_TRANS_Commander (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_TRANS_Commander (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_TRANS_Commander* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_TRANS_Commander: start\n");

   /* decode zone_deploiement */

   PU_PUSHNAME (ctxt_p, "zone_deploiement");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_TRANS_Commander: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_TRANS_Commander::Decode ()
{
   return asn1PD_Mission_Automate_TRANS_Commander (mpContext->GetPtr(), &msgData);
}


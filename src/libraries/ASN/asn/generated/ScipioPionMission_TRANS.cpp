/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 05-Aug-2005.
 */
#include "ScipioPionMission_TRANS.h"

/**************************************************************/
/*                                                            */
/*  Mission_Pion_TRANS_EtablirGererLiaison                    */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Pion_TRANS_EtablirGererLiaison::ASN1C_Mission_Pion_TRANS_EtablirGererLiaison (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_TRANS_EtablirGererLiaison& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Pion_TRANS_EtablirGererLiaison (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_TRANS_EtablirGererLiaison* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Pion_TRANS_EtablirGererLiaison: start\n");

   /* encode position_deploiement */

   PU_PUSHNAME (ctxt_p, "position_deploiement");

   stat = asn1PE_Point (ctxt_p, &pvalue->position_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Pion_TRANS_EtablirGererLiaison: end\n");
   return (stat);
}

int ASN1C_Mission_Pion_TRANS_EtablirGererLiaison::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Pion_TRANS_EtablirGererLiaison (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Pion_TRANS_EtablirGererLiaison (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_TRANS_EtablirGererLiaison* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Pion_TRANS_EtablirGererLiaison: start\n");

   /* decode position_deploiement */

   PU_PUSHNAME (ctxt_p, "position_deploiement");

   stat = asn1PD_Point (ctxt_p, &pvalue->position_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Pion_TRANS_EtablirGererLiaison: end\n");

   return (stat);
}

int ASN1C_Mission_Pion_TRANS_EtablirGererLiaison::Decode ()
{
   return asn1PD_Mission_Pion_TRANS_EtablirGererLiaison (mpContext->GetPtr(), &msgData);
}


/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 23-Dec-2005.
 */
#include "ScipioPionMission_NBC.h"

/**************************************************************/
/*                                                            */
/*  Mission_Pion_NBC_AnimerUnPlotDeDecontamination            */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Pion_NBC_AnimerUnPlotDeDecontamination::ASN1C_Mission_Pion_NBC_AnimerUnPlotDeDecontamination (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_NBC_AnimerUnPlotDeDecontamination& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Pion_NBC_AnimerUnPlotDeDecontamination (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_NBC_AnimerUnPlotDeDecontamination* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Pion_NBC_AnimerUnPlotDeDecontamination: start\n");

   /* encode position_site_decontamination */

   PU_PUSHNAME (ctxt_p, "position_site_decontamination");

   stat = asn1PE_Point (ctxt_p, &pvalue->position_site_decontamination);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Pion_NBC_AnimerUnPlotDeDecontamination: end\n");
   return (stat);
}

int ASN1C_Mission_Pion_NBC_AnimerUnPlotDeDecontamination::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Pion_NBC_AnimerUnPlotDeDecontamination (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Pion_NBC_AnimerUnPlotDeDecontamination (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_NBC_AnimerUnPlotDeDecontamination* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Pion_NBC_AnimerUnPlotDeDecontamination: start\n");

   /* decode position_site_decontamination */

   PU_PUSHNAME (ctxt_p, "position_site_decontamination");

   stat = asn1PD_Point (ctxt_p, &pvalue->position_site_decontamination);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Pion_NBC_AnimerUnPlotDeDecontamination: end\n");

   return (stat);
}

int ASN1C_Mission_Pion_NBC_AnimerUnPlotDeDecontamination::Decode ()
{
   return asn1PD_Mission_Pion_NBC_AnimerUnPlotDeDecontamination (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Pion_NBC_ReconnaitreItineraire                    */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Pion_NBC_ReconnaitreItineraire::ASN1C_Mission_Pion_NBC_ReconnaitreItineraire (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_NBC_ReconnaitreItineraire& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Pion_NBC_ReconnaitreItineraire (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_NBC_ReconnaitreItineraire* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Pion_NBC_ReconnaitreItineraire: start\n");

   /* encode itineraire */

   PU_PUSHNAME (ctxt_p, "itineraire");

   stat = asn1PE_Itineraire (ctxt_p, &pvalue->itineraire);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Pion_NBC_ReconnaitreItineraire: end\n");
   return (stat);
}

int ASN1C_Mission_Pion_NBC_ReconnaitreItineraire::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Pion_NBC_ReconnaitreItineraire (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Pion_NBC_ReconnaitreItineraire (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_NBC_ReconnaitreItineraire* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Pion_NBC_ReconnaitreItineraire: start\n");

   /* decode itineraire */

   PU_PUSHNAME (ctxt_p, "itineraire");

   stat = asn1PD_Itineraire (ctxt_p, &pvalue->itineraire);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Pion_NBC_ReconnaitreItineraire: end\n");

   return (stat);
}

int ASN1C_Mission_Pion_NBC_ReconnaitreItineraire::Decode ()
{
   return asn1PD_Mission_Pion_NBC_ReconnaitreItineraire (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Pion_NBC_ReconnaitreZone                          */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Pion_NBC_ReconnaitreZone::ASN1C_Mission_Pion_NBC_ReconnaitreZone (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_NBC_ReconnaitreZone& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Pion_NBC_ReconnaitreZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_NBC_ReconnaitreZone* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Pion_NBC_ReconnaitreZone: start\n");

   /* encode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Pion_NBC_ReconnaitreZone: end\n");
   return (stat);
}

int ASN1C_Mission_Pion_NBC_ReconnaitreZone::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Pion_NBC_ReconnaitreZone (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Pion_NBC_ReconnaitreZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_NBC_ReconnaitreZone* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Pion_NBC_ReconnaitreZone: start\n");

   /* decode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Pion_NBC_ReconnaitreZone: end\n");

   return (stat);
}

int ASN1C_Mission_Pion_NBC_ReconnaitreZone::Decode ()
{
   return asn1PD_Mission_Pion_NBC_ReconnaitreZone (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Pion_NBC_DecontaminerZone                         */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Pion_NBC_DecontaminerZone::ASN1C_Mission_Pion_NBC_DecontaminerZone (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_NBC_DecontaminerZone& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Pion_NBC_DecontaminerZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_NBC_DecontaminerZone* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Pion_NBC_DecontaminerZone: start\n");

   /* encode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Pion_NBC_DecontaminerZone: end\n");
   return (stat);
}

int ASN1C_Mission_Pion_NBC_DecontaminerZone::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Pion_NBC_DecontaminerZone (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Pion_NBC_DecontaminerZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_NBC_DecontaminerZone* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Pion_NBC_DecontaminerZone: start\n");

   /* decode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Pion_NBC_DecontaminerZone: end\n");

   return (stat);
}

int ASN1C_Mission_Pion_NBC_DecontaminerZone::Decode ()
{
   return asn1PD_Mission_Pion_NBC_DecontaminerZone (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Pion_NBC_ReconnaitreUnSiteROTA                    */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Pion_NBC_ReconnaitreUnSiteROTA::ASN1C_Mission_Pion_NBC_ReconnaitreUnSiteROTA (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_NBC_ReconnaitreUnSiteROTA& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Pion_NBC_ReconnaitreUnSiteROTA (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_NBC_ReconnaitreUnSiteROTA* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Pion_NBC_ReconnaitreUnSiteROTA: start\n");

   /* encode site_ROTA */

   PU_PUSHNAME (ctxt_p, "site_ROTA");

   stat = asn1PE_KnowledgeObject (ctxt_p, pvalue->site_ROTA);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Pion_NBC_ReconnaitreUnSiteROTA: end\n");
   return (stat);
}

int ASN1C_Mission_Pion_NBC_ReconnaitreUnSiteROTA::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Pion_NBC_ReconnaitreUnSiteROTA (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Pion_NBC_ReconnaitreUnSiteROTA (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_NBC_ReconnaitreUnSiteROTA* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Pion_NBC_ReconnaitreUnSiteROTA: start\n");

   /* decode site_ROTA */

   PU_PUSHNAME (ctxt_p, "site_ROTA");

   stat = asn1PD_KnowledgeObject (ctxt_p, &pvalue->site_ROTA);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Pion_NBC_ReconnaitreUnSiteROTA: end\n");

   return (stat);
}

int ASN1C_Mission_Pion_NBC_ReconnaitreUnSiteROTA::Decode ()
{
   return asn1PD_Mission_Pion_NBC_ReconnaitreUnSiteROTA (mpContext->GetPtr(), &msgData);
}


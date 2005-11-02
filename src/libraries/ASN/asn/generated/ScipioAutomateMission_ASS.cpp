/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 28-Oct-2005.
 */
#include "ScipioAutomateMission_ASS.h"

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ASS_SeDeployer                           */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_ASS_SeDeployer::ASN1C_Mission_Automate_ASS_SeDeployer (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ASS_SeDeployer& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_ASS_SeDeployer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASS_SeDeployer* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_ASS_SeDeployer: start\n");

   /* encode zone_deploiement */

   PU_PUSHNAME (ctxt_p, "zone_deploiement");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_ASS_SeDeployer: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_ASS_SeDeployer::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_ASS_SeDeployer (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_ASS_SeDeployer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASS_SeDeployer* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_ASS_SeDeployer: start\n");

   /* decode zone_deploiement */

   PU_PUSHNAME (ctxt_p, "zone_deploiement");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_ASS_SeDeployer: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_ASS_SeDeployer::Decode ()
{
   return asn1PD_Mission_Automate_ASS_SeDeployer (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ASS_ReconnaitreZonesDeploiement          */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_ASS_ReconnaitreZonesDeploiement::ASN1C_Mission_Automate_ASS_ReconnaitreZonesDeploiement (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ASS_ReconnaitreZonesDeploiement& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_ASS_ReconnaitreZonesDeploiement (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASS_ReconnaitreZonesDeploiement* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_ASS_ReconnaitreZonesDeploiement: start\n");

   /* encode positions_a_reconnaitre */

   PU_PUSHNAME (ctxt_p, "positions_a_reconnaitre");

   stat = asn1PE_ListPoint (ctxt_p, &pvalue->positions_a_reconnaitre);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_ASS_ReconnaitreZonesDeploiement: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_ASS_ReconnaitreZonesDeploiement::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_ASS_ReconnaitreZonesDeploiement (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_ASS_ReconnaitreZonesDeploiement (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASS_ReconnaitreZonesDeploiement* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_ASS_ReconnaitreZonesDeploiement: start\n");

   /* decode positions_a_reconnaitre */

   PU_PUSHNAME (ctxt_p, "positions_a_reconnaitre");

   stat = asn1PD_ListPoint (ctxt_p, &pvalue->positions_a_reconnaitre);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_ASS_ReconnaitreZonesDeploiement: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_ASS_ReconnaitreZonesDeploiement::Decode ()
{
   return asn1PD_Mission_Automate_ASS_ReconnaitreZonesDeploiement (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ASS_AcquerirObjectifs                    */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_ASS_AcquerirObjectifs::ASN1C_Mission_Automate_ASS_AcquerirObjectifs (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ASS_AcquerirObjectifs& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_ASS_AcquerirObjectifs (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASS_AcquerirObjectifs* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_ASS_AcquerirObjectifs: start\n");

   /* encode zones_a_observer */

   PU_PUSHNAME (ctxt_p, "zones_a_observer");

   stat = asn1PE_ListPolygon (ctxt_p, &pvalue->zones_a_observer);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode positions_deploiement */

   PU_PUSHNAME (ctxt_p, "positions_deploiement");

   stat = asn1PE_ListPoint (ctxt_p, &pvalue->positions_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode categories */

   PU_PUSHNAME (ctxt_p, "categories");

   stat = asn1PE_NatureAtlas (ctxt_p, pvalue->categories);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_ASS_AcquerirObjectifs: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_ASS_AcquerirObjectifs::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_ASS_AcquerirObjectifs (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_ASS_AcquerirObjectifs (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASS_AcquerirObjectifs* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_ASS_AcquerirObjectifs: start\n");

   /* decode zones_a_observer */

   PU_PUSHNAME (ctxt_p, "zones_a_observer");

   stat = asn1PD_ListPolygon (ctxt_p, &pvalue->zones_a_observer);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode positions_deploiement */

   PU_PUSHNAME (ctxt_p, "positions_deploiement");

   stat = asn1PD_ListPoint (ctxt_p, &pvalue->positions_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode categories */

   PU_PUSHNAME (ctxt_p, "categories");

   stat = asn1PD_NatureAtlas (ctxt_p, &pvalue->categories);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_ASS_AcquerirObjectifs: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_ASS_AcquerirObjectifs::Decode ()
{
   return asn1PD_Mission_Automate_ASS_AcquerirObjectifs (mpContext->GetPtr(), &msgData);
}


/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 13-Oct-2005.
 */
#include "ScipioPionMission_ASA.h"

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASA_DefendreSite                             */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Pion_ASA_DefendreSite::ASN1C_Mission_Pion_ASA_DefendreSite (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASA_DefendreSite& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Pion_ASA_DefendreSite (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_DefendreSite* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Pion_ASA_DefendreSite: start\n");

   /* encode site */

   PU_PUSHNAME (ctxt_p, "site");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->site);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Pion_ASA_DefendreSite: end\n");
   return (stat);
}

int ASN1C_Mission_Pion_ASA_DefendreSite::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Pion_ASA_DefendreSite (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Pion_ASA_DefendreSite (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_DefendreSite* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Pion_ASA_DefendreSite: start\n");

   /* decode site */

   PU_PUSHNAME (ctxt_p, "site");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->site);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Pion_ASA_DefendreSite: end\n");

   return (stat);
}

int ASN1C_Mission_Pion_ASA_DefendreSite::Decode ()
{
   return asn1PD_Mission_Pion_ASA_DefendreSite (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASA_DefendreSiteAPartirPosition              */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Pion_ASA_DefendreSiteAPartirPosition::ASN1C_Mission_Pion_ASA_DefendreSiteAPartirPosition (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASA_DefendreSiteAPartirPosition& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Pion_ASA_DefendreSiteAPartirPosition (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_DefendreSiteAPartirPosition* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Pion_ASA_DefendreSiteAPartirPosition: start\n");

   /* encode site */

   PU_PUSHNAME (ctxt_p, "site");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->site);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode point_de_deploiement */

   PU_PUSHNAME (ctxt_p, "point_de_deploiement");

   stat = asn1PE_Point (ctxt_p, &pvalue->point_de_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Pion_ASA_DefendreSiteAPartirPosition: end\n");
   return (stat);
}

int ASN1C_Mission_Pion_ASA_DefendreSiteAPartirPosition::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Pion_ASA_DefendreSiteAPartirPosition (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Pion_ASA_DefendreSiteAPartirPosition (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_DefendreSiteAPartirPosition* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Pion_ASA_DefendreSiteAPartirPosition: start\n");

   /* decode site */

   PU_PUSHNAME (ctxt_p, "site");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->site);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode point_de_deploiement */

   PU_PUSHNAME (ctxt_p, "point_de_deploiement");

   stat = asn1PD_Point (ctxt_p, &pvalue->point_de_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Pion_ASA_DefendreSiteAPartirPosition: end\n");

   return (stat);
}

int ASN1C_Mission_Pion_ASA_DefendreSiteAPartirPosition::Decode ()
{
   return asn1PD_Mission_Pion_ASA_DefendreSiteAPartirPosition (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASA_DefendreZone                             */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Pion_ASA_DefendreZone::ASN1C_Mission_Pion_ASA_DefendreZone (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASA_DefendreZone& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Pion_ASA_DefendreZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_DefendreZone* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Pion_ASA_DefendreZone: start\n");

   /* encode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Pion_ASA_DefendreZone: end\n");
   return (stat);
}

int ASN1C_Mission_Pion_ASA_DefendreZone::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Pion_ASA_DefendreZone (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Pion_ASA_DefendreZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_DefendreZone* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Pion_ASA_DefendreZone: start\n");

   /* decode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Pion_ASA_DefendreZone: end\n");

   return (stat);
}

int ASN1C_Mission_Pion_ASA_DefendreZone::Decode ()
{
   return asn1PD_Mission_Pion_ASA_DefendreZone (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASA_DefendreZoneAPartirPosition              */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Pion_ASA_DefendreZoneAPartirPosition::ASN1C_Mission_Pion_ASA_DefendreZoneAPartirPosition (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASA_DefendreZoneAPartirPosition& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Pion_ASA_DefendreZoneAPartirPosition (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_DefendreZoneAPartirPosition* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Pion_ASA_DefendreZoneAPartirPosition: start\n");

   /* encode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode point_de_deploiement */

   PU_PUSHNAME (ctxt_p, "point_de_deploiement");

   stat = asn1PE_Point (ctxt_p, &pvalue->point_de_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Pion_ASA_DefendreZoneAPartirPosition: end\n");
   return (stat);
}

int ASN1C_Mission_Pion_ASA_DefendreZoneAPartirPosition::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Pion_ASA_DefendreZoneAPartirPosition (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Pion_ASA_DefendreZoneAPartirPosition (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_DefendreZoneAPartirPosition* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Pion_ASA_DefendreZoneAPartirPosition: start\n");

   /* decode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode point_de_deploiement */

   PU_PUSHNAME (ctxt_p, "point_de_deploiement");

   stat = asn1PD_Point (ctxt_p, &pvalue->point_de_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Pion_ASA_DefendreZoneAPartirPosition: end\n");

   return (stat);
}

int ASN1C_Mission_Pion_ASA_DefendreZoneAPartirPosition::Decode ()
{
   return asn1PD_Mission_Pion_ASA_DefendreZoneAPartirPosition (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASA_Surveiller                               */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Pion_ASA_Surveiller::ASN1C_Mission_Pion_ASA_Surveiller (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASA_Surveiller& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Pion_ASA_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_Surveiller* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Pion_ASA_Surveiller: start\n");

   /* encode point_de_deploiement */

   PU_PUSHNAME (ctxt_p, "point_de_deploiement");

   stat = asn1PE_Point (ctxt_p, &pvalue->point_de_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Pion_ASA_Surveiller: end\n");
   return (stat);
}

int ASN1C_Mission_Pion_ASA_Surveiller::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Pion_ASA_Surveiller (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Pion_ASA_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_Surveiller* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Pion_ASA_Surveiller: start\n");

   /* decode point_de_deploiement */

   PU_PUSHNAME (ctxt_p, "point_de_deploiement");

   stat = asn1PD_Point (ctxt_p, &pvalue->point_de_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Pion_ASA_Surveiller: end\n");

   return (stat);
}

int ASN1C_Mission_Pion_ASA_Surveiller::Decode ()
{
   return asn1PD_Mission_Pion_ASA_Surveiller (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASA_DefendreUnites                           */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Pion_ASA_DefendreUnites::ASN1C_Mission_Pion_ASA_DefendreUnites (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASA_DefendreUnites& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Pion_ASA_DefendreUnites (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_DefendreUnites* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Pion_ASA_DefendreUnites: start\n");

   /* encode unites */

   PU_PUSHNAME (ctxt_p, "unites");

   stat = asn1PE_ListAgent (ctxt_p, &pvalue->unites);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Pion_ASA_DefendreUnites: end\n");
   return (stat);
}

int ASN1C_Mission_Pion_ASA_DefendreUnites::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Pion_ASA_DefendreUnites (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Pion_ASA_DefendreUnites (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_DefendreUnites* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Pion_ASA_DefendreUnites: start\n");

   /* decode unites */

   PU_PUSHNAME (ctxt_p, "unites");

   stat = asn1PD_ListAgent (ctxt_p, &pvalue->unites);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Pion_ASA_DefendreUnites: end\n");

   return (stat);
}

int ASN1C_Mission_Pion_ASA_DefendreUnites::Decode ()
{
   return asn1PD_Mission_Pion_ASA_DefendreUnites (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  EnumModeDeploiementSectionMistral                         */
/*                                                            */
/**************************************************************/

ASN1C_EnumModeDeploiementSectionMistral::ASN1C_EnumModeDeploiementSectionMistral (
   ASN1MessageBuffer& msgBuf, ASN1T_EnumModeDeploiementSectionMistral& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_EnumModeDeploiementSectionMistral (ASN1CTXT* ctxt_p, ASN1T_EnumModeDeploiementSectionMistral value)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   rtdiag ("asn1PE_EnumModeDeploiementSectionMistral: start\n");

   switch (value) {
      case EnumModeDeploiementSectionMistral::omnidirectionnel: ui = 0; break;
      case EnumModeDeploiementSectionMistral::directionnel: ui = 1; break;
      default: return LOG_ASN1ERR (ctxt_p, ASN_E_INVENUM);
   }

   stat = pe_ConsUnsigned (ctxt_p, ui, 0, 1);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   rtdiag ("asn1PE_EnumModeDeploiementSectionMistral: end\n");
   return (stat);
}

int ASN1C_EnumModeDeploiementSectionMistral::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_EnumModeDeploiementSectionMistral (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_EnumModeDeploiementSectionMistral (ASN1CTXT* ctxt_p, ASN1T_EnumModeDeploiementSectionMistral* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   rtdiag ("asn1PD_EnumModeDeploiementSectionMistral: start\n");

   stat = pd_ConsUnsigned (ctxt_p, &ui, 0, 1);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   switch (ui) {
      case 0: *pvalue = EnumModeDeploiementSectionMistral::omnidirectionnel; break;
      case 1: *pvalue = EnumModeDeploiementSectionMistral::directionnel; break;
      default: return LOG_ASN1ERR (ctxt_p, ASN_E_INVENUM);
   }

   rtdiag ("asn1PD_EnumModeDeploiementSectionMistral: end\n");

   return (stat);
}

int ASN1C_EnumModeDeploiementSectionMistral::Decode ()
{
   return asn1PD_EnumModeDeploiementSectionMistral (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASA_MISTRAL_DefendreSite                     */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Pion_ASA_MISTRAL_DefendreSite::ASN1C_Mission_Pion_ASA_MISTRAL_DefendreSite (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSite& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Pion_ASA_MISTRAL_DefendreSite (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSite* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Pion_ASA_MISTRAL_DefendreSite: start\n");

   /* encode site */

   PU_PUSHNAME (ctxt_p, "site");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->site);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode mode_deploiement */

   PU_PUSHNAME (ctxt_p, "mode_deploiement");

   stat = asn1PE_EnumModeDeploiementSectionMistral (ctxt_p, pvalue->mode_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Pion_ASA_MISTRAL_DefendreSite: end\n");
   return (stat);
}

int ASN1C_Mission_Pion_ASA_MISTRAL_DefendreSite::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Pion_ASA_MISTRAL_DefendreSite (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Pion_ASA_MISTRAL_DefendreSite (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSite* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Pion_ASA_MISTRAL_DefendreSite: start\n");

   /* decode site */

   PU_PUSHNAME (ctxt_p, "site");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->site);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode mode_deploiement */

   PU_PUSHNAME (ctxt_p, "mode_deploiement");

   stat = asn1PD_EnumModeDeploiementSectionMistral (ctxt_p, &pvalue->mode_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Pion_ASA_MISTRAL_DefendreSite: end\n");

   return (stat);
}

int ASN1C_Mission_Pion_ASA_MISTRAL_DefendreSite::Decode ()
{
   return asn1PD_Mission_Pion_ASA_MISTRAL_DefendreSite (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition      */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition::ASN1C_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition: start\n");

   /* encode site */

   PU_PUSHNAME (ctxt_p, "site");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->site);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode point_de_deploiement */

   PU_PUSHNAME (ctxt_p, "point_de_deploiement");

   stat = asn1PE_Point (ctxt_p, &pvalue->point_de_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode mode_deploiement */

   PU_PUSHNAME (ctxt_p, "mode_deploiement");

   stat = asn1PE_EnumModeDeploiementSectionMistral (ctxt_p, pvalue->mode_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition: end\n");
   return (stat);
}

int ASN1C_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition: start\n");

   /* decode site */

   PU_PUSHNAME (ctxt_p, "site");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->site);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode point_de_deploiement */

   PU_PUSHNAME (ctxt_p, "point_de_deploiement");

   stat = asn1PD_Point (ctxt_p, &pvalue->point_de_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode mode_deploiement */

   PU_PUSHNAME (ctxt_p, "mode_deploiement");

   stat = asn1PD_EnumModeDeploiementSectionMistral (ctxt_p, &pvalue->mode_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition: end\n");

   return (stat);
}

int ASN1C_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition::Decode ()
{
   return asn1PD_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASA_MISTRAL_DefendreZone                     */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Pion_ASA_MISTRAL_DefendreZone::ASN1C_Mission_Pion_ASA_MISTRAL_DefendreZone (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZone& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Pion_ASA_MISTRAL_DefendreZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZone* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Pion_ASA_MISTRAL_DefendreZone: start\n");

   /* encode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode mode_deploiement */

   PU_PUSHNAME (ctxt_p, "mode_deploiement");

   stat = asn1PE_EnumModeDeploiementSectionMistral (ctxt_p, pvalue->mode_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Pion_ASA_MISTRAL_DefendreZone: end\n");
   return (stat);
}

int ASN1C_Mission_Pion_ASA_MISTRAL_DefendreZone::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Pion_ASA_MISTRAL_DefendreZone (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Pion_ASA_MISTRAL_DefendreZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZone* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Pion_ASA_MISTRAL_DefendreZone: start\n");

   /* decode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode mode_deploiement */

   PU_PUSHNAME (ctxt_p, "mode_deploiement");

   stat = asn1PD_EnumModeDeploiementSectionMistral (ctxt_p, &pvalue->mode_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Pion_ASA_MISTRAL_DefendreZone: end\n");

   return (stat);
}

int ASN1C_Mission_Pion_ASA_MISTRAL_DefendreZone::Decode ()
{
   return asn1PD_Mission_Pion_ASA_MISTRAL_DefendreZone (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition      */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition::ASN1C_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition: start\n");

   /* encode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode point_de_deploiement */

   PU_PUSHNAME (ctxt_p, "point_de_deploiement");

   stat = asn1PE_Point (ctxt_p, &pvalue->point_de_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode mode_deploiement */

   PU_PUSHNAME (ctxt_p, "mode_deploiement");

   stat = asn1PE_EnumModeDeploiementSectionMistral (ctxt_p, pvalue->mode_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition: end\n");
   return (stat);
}

int ASN1C_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition: start\n");

   /* decode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode point_de_deploiement */

   PU_PUSHNAME (ctxt_p, "point_de_deploiement");

   stat = asn1PD_Point (ctxt_p, &pvalue->point_de_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode mode_deploiement */

   PU_PUSHNAME (ctxt_p, "mode_deploiement");

   stat = asn1PD_EnumModeDeploiementSectionMistral (ctxt_p, &pvalue->mode_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition: end\n");

   return (stat);
}

int ASN1C_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition::Decode ()
{
   return asn1PD_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASA_MISTRAL_Surveiller                       */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Pion_ASA_MISTRAL_Surveiller::ASN1C_Mission_Pion_ASA_MISTRAL_Surveiller (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASA_MISTRAL_Surveiller& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Pion_ASA_MISTRAL_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_MISTRAL_Surveiller* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Pion_ASA_MISTRAL_Surveiller: start\n");

   /* encode point_de_deploiement */

   PU_PUSHNAME (ctxt_p, "point_de_deploiement");

   stat = asn1PE_Point (ctxt_p, &pvalue->point_de_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode mode_deploiement */

   PU_PUSHNAME (ctxt_p, "mode_deploiement");

   stat = asn1PE_EnumModeDeploiementSectionMistral (ctxt_p, pvalue->mode_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Pion_ASA_MISTRAL_Surveiller: end\n");
   return (stat);
}

int ASN1C_Mission_Pion_ASA_MISTRAL_Surveiller::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Pion_ASA_MISTRAL_Surveiller (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Pion_ASA_MISTRAL_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASA_MISTRAL_Surveiller* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Pion_ASA_MISTRAL_Surveiller: start\n");

   /* decode point_de_deploiement */

   PU_PUSHNAME (ctxt_p, "point_de_deploiement");

   stat = asn1PD_Point (ctxt_p, &pvalue->point_de_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode mode_deploiement */

   PU_PUSHNAME (ctxt_p, "mode_deploiement");

   stat = asn1PD_EnumModeDeploiementSectionMistral (ctxt_p, &pvalue->mode_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Pion_ASA_MISTRAL_Surveiller: end\n");

   return (stat);
}

int ASN1C_Mission_Pion_ASA_MISTRAL_Surveiller::Decode ()
{
   return asn1PD_Mission_Pion_ASA_MISTRAL_Surveiller (mpContext->GetPtr(), &msgData);
}


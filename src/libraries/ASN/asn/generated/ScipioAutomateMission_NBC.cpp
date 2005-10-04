/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 04-Oct-2005.
 */
#include "ScipioAutomateMission_NBC.h"

/**************************************************************/
/*                                                            */
/*  Mission_Automate_NBC_ReconnaitreUnAxe                     */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_NBC_ReconnaitreUnAxe::ASN1C_Mission_Automate_NBC_ReconnaitreUnAxe (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_NBC_ReconnaitreUnAxe& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_NBC_ReconnaitreUnAxe (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_NBC_ReconnaitreUnAxe* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_NBC_ReconnaitreUnAxe: start\n");


   rtdiag ("asn1PE_Mission_Automate_NBC_ReconnaitreUnAxe: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_NBC_ReconnaitreUnAxe::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_NBC_ReconnaitreUnAxe (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_NBC_ReconnaitreUnAxe (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_NBC_ReconnaitreUnAxe* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_NBC_ReconnaitreUnAxe: start\n");

   /* decode  */

   PU_PUSHNAME (ctxt_p, "");


   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_NBC_ReconnaitreUnAxe: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_NBC_ReconnaitreUnAxe::Decode ()
{
   return asn1PD_Mission_Automate_NBC_ReconnaitreUnAxe (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_NBC_ReconnaitreUneZone                   */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_NBC_ReconnaitreUneZone::ASN1C_Mission_Automate_NBC_ReconnaitreUneZone (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_NBC_ReconnaitreUneZone& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_NBC_ReconnaitreUneZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_NBC_ReconnaitreUneZone* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_NBC_ReconnaitreUneZone: start\n");

   /* encode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_NBC_ReconnaitreUneZone: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_NBC_ReconnaitreUneZone::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_NBC_ReconnaitreUneZone (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_NBC_ReconnaitreUneZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_NBC_ReconnaitreUneZone* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_NBC_ReconnaitreUneZone: start\n");

   /* decode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_NBC_ReconnaitreUneZone: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_NBC_ReconnaitreUneZone::Decode ()
{
   return asn1PD_Mission_Automate_NBC_ReconnaitreUneZone (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_NBC_ArmerUnSiteDeDecontamination         */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_NBC_ArmerUnSiteDeDecontamination::ASN1C_Mission_Automate_NBC_ArmerUnSiteDeDecontamination (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_NBC_ArmerUnSiteDeDecontamination& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_NBC_ArmerUnSiteDeDecontamination (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_NBC_ArmerUnSiteDeDecontamination* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_NBC_ArmerUnSiteDeDecontamination: start\n");

   /* encode site */

   PU_PUSHNAME (ctxt_p, "site");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->site);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_NBC_ArmerUnSiteDeDecontamination: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_NBC_ArmerUnSiteDeDecontamination::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_NBC_ArmerUnSiteDeDecontamination (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_NBC_ArmerUnSiteDeDecontamination (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_NBC_ArmerUnSiteDeDecontamination* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_NBC_ArmerUnSiteDeDecontamination: start\n");

   /* decode site */

   PU_PUSHNAME (ctxt_p, "site");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->site);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_NBC_ArmerUnSiteDeDecontamination: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_NBC_ArmerUnSiteDeDecontamination::Decode ()
{
   return asn1PD_Mission_Automate_NBC_ArmerUnSiteDeDecontamination (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_NBC_DecontaminerUneZone                  */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_NBC_DecontaminerUneZone::ASN1C_Mission_Automate_NBC_DecontaminerUneZone (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_NBC_DecontaminerUneZone& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_NBC_DecontaminerUneZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_NBC_DecontaminerUneZone* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_NBC_DecontaminerUneZone: start\n");

   /* encode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_NBC_DecontaminerUneZone: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_NBC_DecontaminerUneZone::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_NBC_DecontaminerUneZone (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_NBC_DecontaminerUneZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_NBC_DecontaminerUneZone* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_NBC_DecontaminerUneZone: start\n");

   /* decode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_NBC_DecontaminerUneZone: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_NBC_DecontaminerUneZone::Decode ()
{
   return asn1PD_Mission_Automate_NBC_DecontaminerUneZone (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_NBC_ReconnaitreDesSitesROTA              */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_NBC_ReconnaitreDesSitesROTA::ASN1C_Mission_Automate_NBC_ReconnaitreDesSitesROTA (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_NBC_ReconnaitreDesSitesROTA& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_NBC_ReconnaitreDesSitesROTA (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_NBC_ReconnaitreDesSitesROTA* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_NBC_ReconnaitreDesSitesROTA: start\n");

   /* encode sites_ROTA */

   PU_PUSHNAME (ctxt_p, "sites_ROTA");

   stat = asn1PE_ListKnowledgeObject (ctxt_p, &pvalue->sites_ROTA);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_NBC_ReconnaitreDesSitesROTA: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_NBC_ReconnaitreDesSitesROTA::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_NBC_ReconnaitreDesSitesROTA (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_NBC_ReconnaitreDesSitesROTA (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_NBC_ReconnaitreDesSitesROTA* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_NBC_ReconnaitreDesSitesROTA: start\n");

   /* decode sites_ROTA */

   PU_PUSHNAME (ctxt_p, "sites_ROTA");

   stat = asn1PD_ListKnowledgeObject (ctxt_p, &pvalue->sites_ROTA);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_NBC_ReconnaitreDesSitesROTA: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_NBC_ReconnaitreDesSitesROTA::Decode ()
{
   return asn1PD_Mission_Automate_NBC_ReconnaitreDesSitesROTA (mpContext->GetPtr(), &msgData);
}


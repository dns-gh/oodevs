/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 19-Aug-2005.
 */
#include "ScipioAutomateMission_RENS.h"

/**************************************************************/
/*                                                            */
/*  Mission_Automate_RENS_ROHUM_RenseignerSur                 */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_RENS_ROHUM_RenseignerSur::ASN1C_Mission_Automate_RENS_ROHUM_RenseignerSur (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_RENS_ROHUM_RenseignerSur& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_RENS_ROHUM_RenseignerSur (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_ROHUM_RenseignerSur* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_RENS_ROHUM_RenseignerSur: start\n");

   /* encode zone_a_observer */

   PU_PUSHNAME (ctxt_p, "zone_a_observer");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone_a_observer);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_RENS_ROHUM_RenseignerSur: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_RENS_ROHUM_RenseignerSur::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_RENS_ROHUM_RenseignerSur (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_RENS_ROHUM_RenseignerSur (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_ROHUM_RenseignerSur* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_RENS_ROHUM_RenseignerSur: start\n");

   /* decode zone_a_observer */

   PU_PUSHNAME (ctxt_p, "zone_a_observer");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone_a_observer);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_RENS_ROHUM_RenseignerSur: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_RENS_ROHUM_RenseignerSur::Decode ()
{
   return asn1PD_Mission_Automate_RENS_ROHUM_RenseignerSur (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_RENS_ROHUM_OrienterGuider                */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_RENS_ROHUM_OrienterGuider::ASN1C_Mission_Automate_RENS_ROHUM_OrienterGuider (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_RENS_ROHUM_OrienterGuider& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_RENS_ROHUM_OrienterGuider (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_ROHUM_OrienterGuider* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_RENS_ROHUM_OrienterGuider: start\n");

   /* encode ami */

   PU_PUSHNAME (ctxt_p, "ami");

   stat = asn1PE_Automate (ctxt_p, pvalue->ami);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_RENS_ROHUM_OrienterGuider: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_RENS_ROHUM_OrienterGuider::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_RENS_ROHUM_OrienterGuider (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_RENS_ROHUM_OrienterGuider (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_ROHUM_OrienterGuider* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_RENS_ROHUM_OrienterGuider: start\n");

   /* decode ami */

   PU_PUSHNAME (ctxt_p, "ami");

   stat = asn1PD_Automate (ctxt_p, &pvalue->ami);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_RENS_ROHUM_OrienterGuider: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_RENS_ROHUM_OrienterGuider::Decode ()
{
   return asn1PD_Mission_Automate_RENS_ROHUM_OrienterGuider (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_RENS_ROHUM_SExfiltrer                    */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_RENS_ROHUM_SExfiltrer::ASN1C_Mission_Automate_RENS_ROHUM_SExfiltrer (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_RENS_ROHUM_SExfiltrer& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_RENS_ROHUM_SExfiltrer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_ROHUM_SExfiltrer* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_RENS_ROHUM_SExfiltrer: start\n");

   /* encode zone_de_regroupement */

   PU_PUSHNAME (ctxt_p, "zone_de_regroupement");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone_de_regroupement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_RENS_ROHUM_SExfiltrer: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_RENS_ROHUM_SExfiltrer::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_RENS_ROHUM_SExfiltrer (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_RENS_ROHUM_SExfiltrer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_ROHUM_SExfiltrer* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_RENS_ROHUM_SExfiltrer: start\n");

   /* decode zone_de_regroupement */

   PU_PUSHNAME (ctxt_p, "zone_de_regroupement");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone_de_regroupement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_RENS_ROHUM_SExfiltrer: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_RENS_ROHUM_SExfiltrer::Decode ()
{
   return asn1PD_Mission_Automate_RENS_ROHUM_SExfiltrer (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_RENS_ROIM_CL289_RenseignerSur            */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_RENS_ROIM_CL289_RenseignerSur::ASN1C_Mission_Automate_RENS_ROIM_CL289_RenseignerSur (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_RENS_ROIM_CL289_RenseignerSur& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_RENS_ROIM_CL289_RenseignerSur (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_ROIM_CL289_RenseignerSur* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_RENS_ROIM_CL289_RenseignerSur: start\n");

   /* encode ficelle_de_vole */

   PU_PUSHNAME (ctxt_p, "ficelle_de_vole");

   stat = asn1PE_Itineraire (ctxt_p, &pvalue->ficelle_de_vole);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode zones_de_recherche */

   PU_PUSHNAME (ctxt_p, "zones_de_recherche");

   stat = asn1PE_ListPolygon (ctxt_p, &pvalue->zones_de_recherche);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_RENS_ROIM_CL289_RenseignerSur: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_RENS_ROIM_CL289_RenseignerSur::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_RENS_ROIM_CL289_RenseignerSur (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_RENS_ROIM_CL289_RenseignerSur (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_ROIM_CL289_RenseignerSur* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_RENS_ROIM_CL289_RenseignerSur: start\n");

   /* decode ficelle_de_vole */

   PU_PUSHNAME (ctxt_p, "ficelle_de_vole");

   stat = asn1PD_Itineraire (ctxt_p, &pvalue->ficelle_de_vole);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode zones_de_recherche */

   PU_PUSHNAME (ctxt_p, "zones_de_recherche");

   stat = asn1PD_ListPolygon (ctxt_p, &pvalue->zones_de_recherche);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_RENS_ROIM_CL289_RenseignerSur: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_RENS_ROIM_CL289_RenseignerSur::Decode ()
{
   return asn1PD_Mission_Automate_RENS_ROIM_CL289_RenseignerSur (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_RENS_ROIM_SDTI_RenseignerSur             */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur::ASN1C_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur: start\n");

   /* encode ficelle_de_vole */

   PU_PUSHNAME (ctxt_p, "ficelle_de_vole");

   stat = asn1PE_Itineraire (ctxt_p, &pvalue->ficelle_de_vole);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur: start\n");

   /* decode ficelle_de_vole */

   PU_PUSHNAME (ctxt_p, "ficelle_de_vole");

   stat = asn1PD_Itineraire (ctxt_p, &pvalue->ficelle_de_vole);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur::Decode ()
{
   return asn1PD_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_RENS_ROEM_Appuyer                        */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_RENS_ROEM_Appuyer::ASN1C_Mission_Automate_RENS_ROEM_Appuyer (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_RENS_ROEM_Appuyer& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_RENS_ROEM_Appuyer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_ROEM_Appuyer* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_RENS_ROEM_Appuyer: start\n");

   /* encode position_deploiement */

   PU_PUSHNAME (ctxt_p, "position_deploiement");

   stat = asn1PE_Point (ctxt_p, &pvalue->position_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode point_regroupement */

   PU_PUSHNAME (ctxt_p, "point_regroupement");

   stat = asn1PE_Point (ctxt_p, &pvalue->point_regroupement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_RENS_ROEM_Appuyer: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_RENS_ROEM_Appuyer::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_RENS_ROEM_Appuyer (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_RENS_ROEM_Appuyer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_ROEM_Appuyer* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_RENS_ROEM_Appuyer: start\n");

   /* decode position_deploiement */

   PU_PUSHNAME (ctxt_p, "position_deploiement");

   stat = asn1PD_Point (ctxt_p, &pvalue->position_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode point_regroupement */

   PU_PUSHNAME (ctxt_p, "point_regroupement");

   stat = asn1PD_Point (ctxt_p, &pvalue->point_regroupement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_RENS_ROEM_Appuyer: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_RENS_ROEM_Appuyer::Decode ()
{
   return asn1PD_Mission_Automate_RENS_ROEM_Appuyer (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_RENS_ROEM_RenseignerSur                  */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_RENS_ROEM_RenseignerSur::ASN1C_Mission_Automate_RENS_ROEM_RenseignerSur (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_RENS_ROEM_RenseignerSur& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_RENS_ROEM_RenseignerSur (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_ROEM_RenseignerSur* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_RENS_ROEM_RenseignerSur: start\n");

   /* encode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_RENS_ROEM_RenseignerSur: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_RENS_ROEM_RenseignerSur::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_RENS_ROEM_RenseignerSur (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_RENS_ROEM_RenseignerSur (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_ROEM_RenseignerSur* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_RENS_ROEM_RenseignerSur: start\n");

   /* decode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_RENS_ROEM_RenseignerSur: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_RENS_ROEM_RenseignerSur::Decode ()
{
   return asn1PD_Mission_Automate_RENS_ROEM_RenseignerSur (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_RENS_RADINT_RenseignerSur                */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_RENS_RADINT_RenseignerSur::ASN1C_Mission_Automate_RENS_RADINT_RenseignerSur (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_RENS_RADINT_RenseignerSur& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_RENS_RADINT_RenseignerSur (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_RADINT_RenseignerSur* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_RENS_RADINT_RenseignerSur: start\n");

   /* encode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_RENS_RADINT_RenseignerSur: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_RENS_RADINT_RenseignerSur::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_RENS_RADINT_RenseignerSur (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_RENS_RADINT_RenseignerSur (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_RENS_RADINT_RenseignerSur* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_RENS_RADINT_RenseignerSur: start\n");

   /* decode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_RENS_RADINT_RenseignerSur: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_RENS_RADINT_RenseignerSur::Decode ()
{
   return asn1PD_Mission_Automate_RENS_RADINT_RenseignerSur (mpContext->GetPtr(), &msgData);
}


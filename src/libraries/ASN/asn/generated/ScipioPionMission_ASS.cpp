/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 28-Oct-2005.
 */
#include "ScipioPionMission_ASS.h"

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASS_AssurerMiseEnOeuvre                      */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Pion_ASS_AssurerMiseEnOeuvre::ASN1C_Mission_Pion_ASS_AssurerMiseEnOeuvre (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvre& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Pion_ASS_AssurerMiseEnOeuvre (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvre* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Pion_ASS_AssurerMiseEnOeuvre: start\n");

   /* encode zone_implantation */

   PU_PUSHNAME (ctxt_p, "zone_implantation");

   stat = asn1PE_KnowledgeObject (ctxt_p, pvalue->zone_implantation);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode presence_contre_batterie */

   PU_PUSHNAME (ctxt_p, "presence_contre_batterie");

   PU_NEWFIELD (ctxt_p, "boolean");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->presence_contre_batterie);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Pion_ASS_AssurerMiseEnOeuvre: end\n");
   return (stat);
}

int ASN1C_Mission_Pion_ASS_AssurerMiseEnOeuvre::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Pion_ASS_AssurerMiseEnOeuvre (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Pion_ASS_AssurerMiseEnOeuvre (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvre* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Pion_ASS_AssurerMiseEnOeuvre: start\n");

   /* decode zone_implantation */

   PU_PUSHNAME (ctxt_p, "zone_implantation");

   stat = asn1PD_KnowledgeObject (ctxt_p, &pvalue->zone_implantation);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode presence_contre_batterie */

   PU_PUSHNAME (ctxt_p, "presence_contre_batterie");

   PU_NEWFIELD (ctxt_p, "boolean");

   stat = pd_bit (ctxt_p, &pvalue->presence_contre_batterie);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Pion_ASS_AssurerMiseEnOeuvre: end\n");

   return (stat);
}

int ASN1C_Mission_Pion_ASS_AssurerMiseEnOeuvre::Decode ()
{
   return asn1PD_Mission_Pion_ASS_AssurerMiseEnOeuvre (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition           */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition::ASN1C_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition: start\n");

   /* encode position_implantation */

   PU_PUSHNAME (ctxt_p, "position_implantation");

   stat = asn1PE_Point (ctxt_p, &pvalue->position_implantation);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode presence_contre_batterie */

   PU_PUSHNAME (ctxt_p, "presence_contre_batterie");

   PU_NEWFIELD (ctxt_p, "boolean");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->presence_contre_batterie);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition: end\n");
   return (stat);
}

int ASN1C_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition: start\n");

   /* decode position_implantation */

   PU_PUSHNAME (ctxt_p, "position_implantation");

   stat = asn1PD_Point (ctxt_p, &pvalue->position_implantation);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode presence_contre_batterie */

   PU_PUSHNAME (ctxt_p, "presence_contre_batterie");

   PU_NEWFIELD (ctxt_p, "boolean");

   stat = pd_bit (ctxt_p, &pvalue->presence_contre_batterie);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition: end\n");

   return (stat);
}

int ASN1C_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition::Decode ()
{
   return asn1PD_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASS_ReconnaitreZoneDeploiement               */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Pion_ASS_ReconnaitreZoneDeploiement::ASN1C_Mission_Pion_ASS_ReconnaitreZoneDeploiement (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASS_ReconnaitreZoneDeploiement& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Pion_ASS_ReconnaitreZoneDeploiement (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASS_ReconnaitreZoneDeploiement* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Pion_ASS_ReconnaitreZoneDeploiement: start\n");

   /* encode position_deploiement */

   PU_PUSHNAME (ctxt_p, "position_deploiement");

   stat = asn1PE_Point (ctxt_p, &pvalue->position_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Pion_ASS_ReconnaitreZoneDeploiement: end\n");
   return (stat);
}

int ASN1C_Mission_Pion_ASS_ReconnaitreZoneDeploiement::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Pion_ASS_ReconnaitreZoneDeploiement (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Pion_ASS_ReconnaitreZoneDeploiement (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASS_ReconnaitreZoneDeploiement* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Pion_ASS_ReconnaitreZoneDeploiement: start\n");

   /* decode position_deploiement */

   PU_PUSHNAME (ctxt_p, "position_deploiement");

   stat = asn1PD_Point (ctxt_p, &pvalue->position_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Pion_ASS_ReconnaitreZoneDeploiement: end\n");

   return (stat);
}

int ASN1C_Mission_Pion_ASS_ReconnaitreZoneDeploiement::Decode ()
{
   return asn1PD_Mission_Pion_ASS_ReconnaitreZoneDeploiement (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASS_AcquerirLanceursEni                      */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Pion_ASS_AcquerirLanceursEni::ASN1C_Mission_Pion_ASS_AcquerirLanceursEni (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASS_AcquerirLanceursEni& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Pion_ASS_AcquerirLanceursEni (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASS_AcquerirLanceursEni* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Pion_ASS_AcquerirLanceursEni: start\n");

   /* encode zones_a_observer */

   PU_PUSHNAME (ctxt_p, "zones_a_observer");

   stat = asn1PE_ListPolygon (ctxt_p, &pvalue->zones_a_observer);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode zone_implantation */

   PU_PUSHNAME (ctxt_p, "zone_implantation");

   stat = asn1PE_KnowledgeObject (ctxt_p, pvalue->zone_implantation);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode menance_roem */

   PU_PUSHNAME (ctxt_p, "menance_roem");

   PU_NEWFIELD (ctxt_p, "boolean");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->menance_roem);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Pion_ASS_AcquerirLanceursEni: end\n");
   return (stat);
}

int ASN1C_Mission_Pion_ASS_AcquerirLanceursEni::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Pion_ASS_AcquerirLanceursEni (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Pion_ASS_AcquerirLanceursEni (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASS_AcquerirLanceursEni* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Pion_ASS_AcquerirLanceursEni: start\n");

   /* decode zones_a_observer */

   PU_PUSHNAME (ctxt_p, "zones_a_observer");

   stat = asn1PD_ListPolygon (ctxt_p, &pvalue->zones_a_observer);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode zone_implantation */

   PU_PUSHNAME (ctxt_p, "zone_implantation");

   stat = asn1PD_KnowledgeObject (ctxt_p, &pvalue->zone_implantation);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode menance_roem */

   PU_PUSHNAME (ctxt_p, "menance_roem");

   PU_NEWFIELD (ctxt_p, "boolean");

   stat = pd_bit (ctxt_p, &pvalue->menance_roem);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Pion_ASS_AcquerirLanceursEni: end\n");

   return (stat);
}

int ASN1C_Mission_Pion_ASS_AcquerirLanceursEni::Decode ()
{
   return asn1PD_Mission_Pion_ASS_AcquerirLanceursEni (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASS_EO_AcquerirObjectifs                     */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Pion_ASS_EO_AcquerirObjectifs::ASN1C_Mission_Pion_ASS_EO_AcquerirObjectifs (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASS_EO_AcquerirObjectifs& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Pion_ASS_EO_AcquerirObjectifs (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASS_EO_AcquerirObjectifs* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Pion_ASS_EO_AcquerirObjectifs: start\n");

   /* encode zone_a_observer */

   PU_PUSHNAME (ctxt_p, "zone_a_observer");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone_a_observer);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode unite_a_appuyer */

   PU_PUSHNAME (ctxt_p, "unite_a_appuyer");

   stat = asn1PE_Automate (ctxt_p, pvalue->unite_a_appuyer);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode categories */

   PU_PUSHNAME (ctxt_p, "categories");

   stat = asn1PE_NatureAtlas (ctxt_p, pvalue->categories);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Pion_ASS_EO_AcquerirObjectifs: end\n");
   return (stat);
}

int ASN1C_Mission_Pion_ASS_EO_AcquerirObjectifs::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Pion_ASS_EO_AcquerirObjectifs (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Pion_ASS_EO_AcquerirObjectifs (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASS_EO_AcquerirObjectifs* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Pion_ASS_EO_AcquerirObjectifs: start\n");

   /* decode zone_a_observer */

   PU_PUSHNAME (ctxt_p, "zone_a_observer");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone_a_observer);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode unite_a_appuyer */

   PU_PUSHNAME (ctxt_p, "unite_a_appuyer");

   stat = asn1PD_Automate (ctxt_p, &pvalue->unite_a_appuyer);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode categories */

   PU_PUSHNAME (ctxt_p, "categories");

   stat = asn1PD_NatureAtlas (ctxt_p, &pvalue->categories);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Pion_ASS_EO_AcquerirObjectifs: end\n");

   return (stat);
}

int ASN1C_Mission_Pion_ASS_EO_AcquerirObjectifs::Decode ()
{
   return asn1PD_Mission_Pion_ASS_EO_AcquerirObjectifs (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASS_EOP_AcquerirObjectifs                    */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Pion_ASS_EOP_AcquerirObjectifs::ASN1C_Mission_Pion_ASS_EOP_AcquerirObjectifs (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASS_EOP_AcquerirObjectifs& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Pion_ASS_EOP_AcquerirObjectifs (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASS_EOP_AcquerirObjectifs* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Pion_ASS_EOP_AcquerirObjectifs: start\n");

   /* encode zone_a_observer */

   PU_PUSHNAME (ctxt_p, "zone_a_observer");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone_a_observer);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode position_installation */

   PU_PUSHNAME (ctxt_p, "position_installation");

   stat = asn1PE_Point (ctxt_p, &pvalue->position_installation);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode position_debarquement */

   PU_PUSHNAME (ctxt_p, "position_debarquement");

   stat = asn1PE_Point (ctxt_p, &pvalue->position_debarquement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Pion_ASS_EOP_AcquerirObjectifs: end\n");
   return (stat);
}

int ASN1C_Mission_Pion_ASS_EOP_AcquerirObjectifs::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Pion_ASS_EOP_AcquerirObjectifs (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Pion_ASS_EOP_AcquerirObjectifs (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASS_EOP_AcquerirObjectifs* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Pion_ASS_EOP_AcquerirObjectifs: start\n");

   /* decode zone_a_observer */

   PU_PUSHNAME (ctxt_p, "zone_a_observer");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone_a_observer);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode position_installation */

   PU_PUSHNAME (ctxt_p, "position_installation");

   stat = asn1PD_Point (ctxt_p, &pvalue->position_installation);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode position_debarquement */

   PU_PUSHNAME (ctxt_p, "position_debarquement");

   stat = asn1PD_Point (ctxt_p, &pvalue->position_debarquement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Pion_ASS_EOP_AcquerirObjectifs: end\n");

   return (stat);
}

int ASN1C_Mission_Pion_ASS_EOP_AcquerirObjectifs::Decode ()
{
   return asn1PD_Mission_Pion_ASS_EOP_AcquerirObjectifs (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Pion_ASS_RATAC_AcquerirObjectifs                  */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Pion_ASS_RATAC_AcquerirObjectifs::ASN1C_Mission_Pion_ASS_RATAC_AcquerirObjectifs (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion_ASS_RATAC_AcquerirObjectifs& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Pion_ASS_RATAC_AcquerirObjectifs (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASS_RATAC_AcquerirObjectifs* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Pion_ASS_RATAC_AcquerirObjectifs: start\n");

   /* encode zone_a_observer */

   PU_PUSHNAME (ctxt_p, "zone_a_observer");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone_a_observer);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode categories */

   PU_PUSHNAME (ctxt_p, "categories");

   stat = asn1PE_NatureAtlas (ctxt_p, pvalue->categories);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode position_deploiement */

   PU_PUSHNAME (ctxt_p, "position_deploiement");

   stat = asn1PE_Point (ctxt_p, &pvalue->position_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Pion_ASS_RATAC_AcquerirObjectifs: end\n");
   return (stat);
}

int ASN1C_Mission_Pion_ASS_RATAC_AcquerirObjectifs::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Pion_ASS_RATAC_AcquerirObjectifs (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Pion_ASS_RATAC_AcquerirObjectifs (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion_ASS_RATAC_AcquerirObjectifs* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Pion_ASS_RATAC_AcquerirObjectifs: start\n");

   /* decode zone_a_observer */

   PU_PUSHNAME (ctxt_p, "zone_a_observer");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone_a_observer);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode categories */

   PU_PUSHNAME (ctxt_p, "categories");

   stat = asn1PD_NatureAtlas (ctxt_p, &pvalue->categories);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode position_deploiement */

   PU_PUSHNAME (ctxt_p, "position_deploiement");

   stat = asn1PD_Point (ctxt_p, &pvalue->position_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Pion_ASS_RATAC_AcquerirObjectifs: end\n");

   return (stat);
}

int ASN1C_Mission_Pion_ASS_RATAC_AcquerirObjectifs::Decode ()
{
   return asn1PD_Mission_Pion_ASS_RATAC_AcquerirObjectifs (mpContext->GetPtr(), &msgData);
}


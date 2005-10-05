/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 05-Oct-2005.
 */
#include "ScipioAutomateMission_ASA.h"

/**************************************************************/
/*                                                            */
/*  EnumModeDeploiement                                       */
/*                                                            */
/**************************************************************/

ASN1C_EnumModeDeploiement::ASN1C_EnumModeDeploiement (
   ASN1MessageBuffer& msgBuf, ASN1T_EnumModeDeploiement& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_EnumModeDeploiement (ASN1CTXT* ctxt_p, ASN1T_EnumModeDeploiement value)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   rtdiag ("asn1PE_EnumModeDeploiement: start\n");

   switch (value) {
      case EnumModeDeploiement::en_carre: ui = 0; break;
      case EnumModeDeploiement::en_triangle: ui = 1; break;
      case EnumModeDeploiement::nasse_trois_sections: ui = 2; break;
      case EnumModeDeploiement::nasse_quatre_sections: ui = 3; break;
      case EnumModeDeploiement::double_rideau: ui = 4; break;
      case EnumModeDeploiement::simple_rideau: ui = 5; break;
      default: return LOG_ASN1ERR (ctxt_p, ASN_E_INVENUM);
   }

   stat = pe_ConsUnsigned (ctxt_p, ui, 0, 5);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   rtdiag ("asn1PE_EnumModeDeploiement: end\n");
   return (stat);
}

int ASN1C_EnumModeDeploiement::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_EnumModeDeploiement (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_EnumModeDeploiement (ASN1CTXT* ctxt_p, ASN1T_EnumModeDeploiement* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   rtdiag ("asn1PD_EnumModeDeploiement: start\n");

   stat = pd_ConsUnsigned (ctxt_p, &ui, 0, 5);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   switch (ui) {
      case 0: *pvalue = EnumModeDeploiement::en_carre; break;
      case 1: *pvalue = EnumModeDeploiement::en_triangle; break;
      case 2: *pvalue = EnumModeDeploiement::nasse_trois_sections; break;
      case 3: *pvalue = EnumModeDeploiement::nasse_quatre_sections; break;
      case 4: *pvalue = EnumModeDeploiement::double_rideau; break;
      case 5: *pvalue = EnumModeDeploiement::simple_rideau; break;
      default: return LOG_ASN1ERR (ctxt_p, ASN_E_INVENUM);
   }

   rtdiag ("asn1PD_EnumModeDeploiement: end\n");

   return (stat);
}

int ASN1C_EnumModeDeploiement::Decode ()
{
   return asn1PD_EnumModeDeploiement (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ASA_DefendreZone                         */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_ASA_DefendreZone::ASN1C_Mission_Automate_ASA_DefendreZone (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ASA_DefendreZone& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_ASA_DefendreZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_DefendreZone* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_ASA_DefendreZone: start\n");

   /* encode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode positions_sections */

   PU_PUSHNAME (ctxt_p, "positions_sections");

   stat = asn1PE_ListPoint (ctxt_p, &pvalue->positions_sections);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_ASA_DefendreZone: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_ASA_DefendreZone::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_ASA_DefendreZone (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_ASA_DefendreZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_DefendreZone* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_ASA_DefendreZone: start\n");

   /* decode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode positions_sections */

   PU_PUSHNAME (ctxt_p, "positions_sections");

   stat = asn1PD_ListPoint (ctxt_p, &pvalue->positions_sections);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_ASA_DefendreZone: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_ASA_DefendreZone::Decode ()
{
   return asn1PD_Mission_Automate_ASA_DefendreZone (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ASA_DefendreSite                         */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_ASA_DefendreSite::ASN1C_Mission_Automate_ASA_DefendreSite (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ASA_DefendreSite& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_ASA_DefendreSite (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_DefendreSite* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_ASA_DefendreSite: start\n");

   /* encode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode positions_sections */

   PU_PUSHNAME (ctxt_p, "positions_sections");

   stat = asn1PE_ListPoint (ctxt_p, &pvalue->positions_sections);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_ASA_DefendreSite: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_ASA_DefendreSite::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_ASA_DefendreSite (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_ASA_DefendreSite (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_DefendreSite* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_ASA_DefendreSite: start\n");

   /* decode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode positions_sections */

   PU_PUSHNAME (ctxt_p, "positions_sections");

   stat = asn1PD_ListPoint (ctxt_p, &pvalue->positions_sections);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_ASA_DefendreSite: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_ASA_DefendreSite::Decode ()
{
   return asn1PD_Mission_Automate_ASA_DefendreSite (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ASA_Surveiller                           */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_ASA_Surveiller::ASN1C_Mission_Automate_ASA_Surveiller (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ASA_Surveiller& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_ASA_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_Surveiller* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_ASA_Surveiller: start\n");

   /* encode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode positions_sections */

   PU_PUSHNAME (ctxt_p, "positions_sections");

   stat = asn1PE_ListPoint (ctxt_p, &pvalue->positions_sections);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_ASA_Surveiller: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_ASA_Surveiller::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_ASA_Surveiller (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_ASA_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_Surveiller* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_ASA_Surveiller: start\n");

   /* decode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode positions_sections */

   PU_PUSHNAME (ctxt_p, "positions_sections");

   stat = asn1PD_ListPoint (ctxt_p, &pvalue->positions_sections);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_ASA_Surveiller: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_ASA_Surveiller::Decode ()
{
   return asn1PD_Mission_Automate_ASA_Surveiller (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ASA_DefendreItineraire                   */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_ASA_DefendreItineraire::ASN1C_Mission_Automate_ASA_DefendreItineraire (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ASA_DefendreItineraire& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_ASA_DefendreItineraire (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_DefendreItineraire* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_ASA_DefendreItineraire: start\n");

   /* encode itineraire */

   PU_PUSHNAME (ctxt_p, "itineraire");

   stat = asn1PE_Itineraire (ctxt_p, &pvalue->itineraire);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_ASA_DefendreItineraire: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_ASA_DefendreItineraire::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_ASA_DefendreItineraire (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_ASA_DefendreItineraire (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_DefendreItineraire* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_ASA_DefendreItineraire: start\n");

   /* decode itineraire */

   PU_PUSHNAME (ctxt_p, "itineraire");

   stat = asn1PD_Itineraire (ctxt_p, &pvalue->itineraire);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_ASA_DefendreItineraire: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_ASA_DefendreItineraire::Decode ()
{
   return asn1PD_Mission_Automate_ASA_DefendreItineraire (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ASA_MISTRAL_DefendreZone                 */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_ASA_MISTRAL_DefendreZone::ASN1C_Mission_Automate_ASA_MISTRAL_DefendreZone (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ASA_MISTRAL_DefendreZone& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_ASA_MISTRAL_DefendreZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_MISTRAL_DefendreZone* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_ASA_MISTRAL_DefendreZone: start\n");

   /* encode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode mode_deploiement */

   PU_PUSHNAME (ctxt_p, "mode_deploiement");

   stat = asn1PE_EnumModeDeploiement (ctxt_p, pvalue->mode_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode angle */

   PU_PUSHNAME (ctxt_p, "angle");

   stat = asn1PE_Angle (ctxt_p, pvalue->angle);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_ASA_MISTRAL_DefendreZone: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_ASA_MISTRAL_DefendreZone::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_ASA_MISTRAL_DefendreZone (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_ASA_MISTRAL_DefendreZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_MISTRAL_DefendreZone* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_ASA_MISTRAL_DefendreZone: start\n");

   /* decode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode mode_deploiement */

   PU_PUSHNAME (ctxt_p, "mode_deploiement");

   stat = asn1PD_EnumModeDeploiement (ctxt_p, &pvalue->mode_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode angle */

   PU_PUSHNAME (ctxt_p, "angle");

   stat = asn1PD_Angle (ctxt_p, &pvalue->angle);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_ASA_MISTRAL_DefendreZone: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_ASA_MISTRAL_DefendreZone::Decode ()
{
   return asn1PD_Mission_Automate_ASA_MISTRAL_DefendreZone (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ASA_MISTRAL_DefendreSite                 */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_ASA_MISTRAL_DefendreSite::ASN1C_Mission_Automate_ASA_MISTRAL_DefendreSite (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ASA_MISTRAL_DefendreSite& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_ASA_MISTRAL_DefendreSite (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_MISTRAL_DefendreSite* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_ASA_MISTRAL_DefendreSite: start\n");

   /* encode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode mode_deploiement */

   PU_PUSHNAME (ctxt_p, "mode_deploiement");

   stat = asn1PE_EnumModeDeploiement (ctxt_p, pvalue->mode_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode angle */

   PU_PUSHNAME (ctxt_p, "angle");

   stat = asn1PE_Angle (ctxt_p, pvalue->angle);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_ASA_MISTRAL_DefendreSite: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_ASA_MISTRAL_DefendreSite::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_ASA_MISTRAL_DefendreSite (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_ASA_MISTRAL_DefendreSite (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_MISTRAL_DefendreSite* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_ASA_MISTRAL_DefendreSite: start\n");

   /* decode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode mode_deploiement */

   PU_PUSHNAME (ctxt_p, "mode_deploiement");

   stat = asn1PD_EnumModeDeploiement (ctxt_p, &pvalue->mode_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode angle */

   PU_PUSHNAME (ctxt_p, "angle");

   stat = asn1PD_Angle (ctxt_p, &pvalue->angle);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_ASA_MISTRAL_DefendreSite: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_ASA_MISTRAL_DefendreSite::Decode ()
{
   return asn1PD_Mission_Automate_ASA_MISTRAL_DefendreSite (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ASA_MISTRAL_Surveiller                   */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_ASA_MISTRAL_Surveiller::ASN1C_Mission_Automate_ASA_MISTRAL_Surveiller (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ASA_MISTRAL_Surveiller& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_ASA_MISTRAL_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_MISTRAL_Surveiller* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_ASA_MISTRAL_Surveiller: start\n");

   /* encode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode mode_deploiement */

   PU_PUSHNAME (ctxt_p, "mode_deploiement");

   stat = asn1PE_EnumModeDeploiement (ctxt_p, pvalue->mode_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode angle */

   PU_PUSHNAME (ctxt_p, "angle");

   stat = asn1PE_Angle (ctxt_p, pvalue->angle);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_ASA_MISTRAL_Surveiller: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_ASA_MISTRAL_Surveiller::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_ASA_MISTRAL_Surveiller (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_ASA_MISTRAL_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_MISTRAL_Surveiller* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_ASA_MISTRAL_Surveiller: start\n");

   /* decode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode mode_deploiement */

   PU_PUSHNAME (ctxt_p, "mode_deploiement");

   stat = asn1PD_EnumModeDeploiement (ctxt_p, &pvalue->mode_deploiement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode angle */

   PU_PUSHNAME (ctxt_p, "angle");

   stat = asn1PD_Angle (ctxt_p, &pvalue->angle);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_ASA_MISTRAL_Surveiller: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_ASA_MISTRAL_Surveiller::Decode ()
{
   return asn1PD_Mission_Automate_ASA_MISTRAL_Surveiller (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ASA_HAWK_DefendreZone                    */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_ASA_HAWK_DefendreZone::ASN1C_Mission_Automate_ASA_HAWK_DefendreZone (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ASA_HAWK_DefendreZone& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_ASA_HAWK_DefendreZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_HAWK_DefendreZone* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_ASA_HAWK_DefendreZone: start\n");

   /* encode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode positions_sections */

   PU_PUSHNAME (ctxt_p, "positions_sections");

   stat = asn1PE_ListPoint (ctxt_p, &pvalue->positions_sections);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_ASA_HAWK_DefendreZone: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_ASA_HAWK_DefendreZone::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_ASA_HAWK_DefendreZone (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_ASA_HAWK_DefendreZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_HAWK_DefendreZone* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_ASA_HAWK_DefendreZone: start\n");

   /* decode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode positions_sections */

   PU_PUSHNAME (ctxt_p, "positions_sections");

   stat = asn1PD_ListPoint (ctxt_p, &pvalue->positions_sections);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_ASA_HAWK_DefendreZone: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_ASA_HAWK_DefendreZone::Decode ()
{
   return asn1PD_Mission_Automate_ASA_HAWK_DefendreZone (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ASA_HAWK_DefendreSite                    */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_ASA_HAWK_DefendreSite::ASN1C_Mission_Automate_ASA_HAWK_DefendreSite (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ASA_HAWK_DefendreSite& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_ASA_HAWK_DefendreSite (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_HAWK_DefendreSite* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_ASA_HAWK_DefendreSite: start\n");

   /* encode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode positions_sections */

   PU_PUSHNAME (ctxt_p, "positions_sections");

   stat = asn1PE_ListPoint (ctxt_p, &pvalue->positions_sections);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_ASA_HAWK_DefendreSite: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_ASA_HAWK_DefendreSite::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_ASA_HAWK_DefendreSite (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_ASA_HAWK_DefendreSite (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_HAWK_DefendreSite* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_ASA_HAWK_DefendreSite: start\n");

   /* decode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode positions_sections */

   PU_PUSHNAME (ctxt_p, "positions_sections");

   stat = asn1PD_ListPoint (ctxt_p, &pvalue->positions_sections);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_ASA_HAWK_DefendreSite: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_ASA_HAWK_DefendreSite::Decode ()
{
   return asn1PD_Mission_Automate_ASA_HAWK_DefendreSite (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ASA_HAWK_Surveiller                      */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_ASA_HAWK_Surveiller::ASN1C_Mission_Automate_ASA_HAWK_Surveiller (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ASA_HAWK_Surveiller& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_ASA_HAWK_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_HAWK_Surveiller* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_ASA_HAWK_Surveiller: start\n");

   /* encode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode positions_sections */

   PU_PUSHNAME (ctxt_p, "positions_sections");

   stat = asn1PE_ListPoint (ctxt_p, &pvalue->positions_sections);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_ASA_HAWK_Surveiller: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_ASA_HAWK_Surveiller::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_ASA_HAWK_Surveiller (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_ASA_HAWK_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_HAWK_Surveiller* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_ASA_HAWK_Surveiller: start\n");

   /* decode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode positions_sections */

   PU_PUSHNAME (ctxt_p, "positions_sections");

   stat = asn1PD_ListPoint (ctxt_p, &pvalue->positions_sections);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_ASA_HAWK_Surveiller: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_ASA_HAWK_Surveiller::Decode ()
{
   return asn1PD_Mission_Automate_ASA_HAWK_Surveiller (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ASA_DefendreUnites                       */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_ASA_DefendreUnites::ASN1C_Mission_Automate_ASA_DefendreUnites (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ASA_DefendreUnites& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_ASA_DefendreUnites (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_DefendreUnites* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_ASA_DefendreUnites: start\n");

   /* encode unites */

   PU_PUSHNAME (ctxt_p, "unites");

   stat = asn1PE_ListAgent (ctxt_p, &pvalue->unites);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_ASA_DefendreUnites: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_ASA_DefendreUnites::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_ASA_DefendreUnites (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_ASA_DefendreUnites (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ASA_DefendreUnites* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_ASA_DefendreUnites: start\n");

   /* decode unites */

   PU_PUSHNAME (ctxt_p, "unites");

   stat = asn1PD_ListAgent (ctxt_p, &pvalue->unites);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_ASA_DefendreUnites: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_ASA_DefendreUnites::Decode ()
{
   return asn1PD_Mission_Automate_ASA_DefendreUnites (mpContext->GetPtr(), &msgData);
}


/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 03-Aug-2006.
 */
#include "ScipioAutomateMission_LOG.h"

/**************************************************************/
/*                                                            */
/*  Mission_Automate_LOG_SeDeployer                           */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_LOG_SeDeployer::ASN1C_Mission_Automate_LOG_SeDeployer (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_LOG_SeDeployer& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_LOG_SeDeployer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_SeDeployer* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_LOG_SeDeployer: start\n");

   PU_NEWFIELD (ctxt_p, "position_deploiementPresent");

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.position_deploiementPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_SETBITCOUNT (ctxt_p);

   /* encode position_deploiement */

   if (pvalue->m.position_deploiementPresent) {
      PU_PUSHNAME (ctxt_p, "position_deploiement");

      stat = asn1PE_Point (ctxt_p, &pvalue->position_deploiement);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }


   rtdiag ("asn1PE_Mission_Automate_LOG_SeDeployer: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_LOG_SeDeployer::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_LOG_SeDeployer (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_LOG_SeDeployer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_SeDeployer* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL optbit;

   rtdiag ("asn1PD_Mission_Automate_LOG_SeDeployer: start\n");

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   PU_NEWFIELD (ctxt_p, "position_deploiementPresent");

   stat = pd_bit (ctxt_p, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->m.position_deploiementPresent = optbit;

   PU_SETBITCOUNT (ctxt_p);

   /* decode position_deploiement */

   if (pvalue->m.position_deploiementPresent) {
      PU_PUSHNAME (ctxt_p, "position_deploiement");

      stat = asn1PD_Point (ctxt_p, &pvalue->position_deploiement);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }


   rtdiag ("asn1PD_Mission_Automate_LOG_SeDeployer: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_LOG_SeDeployer::Decode ()
{
   return asn1PD_Mission_Automate_LOG_SeDeployer (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_LOG_AppuyerMouvement                     */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_LOG_AppuyerMouvement::ASN1C_Mission_Automate_LOG_AppuyerMouvement (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_LOG_AppuyerMouvement& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_LOG_AppuyerMouvement (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_AppuyerMouvement* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_LOG_AppuyerMouvement: start\n");

   /* encode unites_a_appuyer */

   PU_PUSHNAME (ctxt_p, "unites_a_appuyer");

   stat = asn1PE_ListAgent (ctxt_p, &pvalue->unites_a_appuyer);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_LOG_AppuyerMouvement: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_LOG_AppuyerMouvement::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_LOG_AppuyerMouvement (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_LOG_AppuyerMouvement (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_AppuyerMouvement* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_LOG_AppuyerMouvement: start\n");

   /* decode unites_a_appuyer */

   PU_PUSHNAME (ctxt_p, "unites_a_appuyer");

   stat = asn1PD_ListAgent (ctxt_p, &pvalue->unites_a_appuyer);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_LOG_AppuyerMouvement: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_LOG_AppuyerMouvement::Decode ()
{
   return asn1PD_Mission_Automate_LOG_AppuyerMouvement (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_LOG_ReconnaitreItineraire                */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_LOG_ReconnaitreItineraire::ASN1C_Mission_Automate_LOG_ReconnaitreItineraire (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_LOG_ReconnaitreItineraire& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_LOG_ReconnaitreItineraire (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_ReconnaitreItineraire* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_LOG_ReconnaitreItineraire: start\n");

   /* encode itineraire_log */

   PU_PUSHNAME (ctxt_p, "itineraire_log");

   stat = asn1PE_Itineraire (ctxt_p, &pvalue->itineraire_log);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_LOG_ReconnaitreItineraire: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_LOG_ReconnaitreItineraire::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_LOG_ReconnaitreItineraire (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_LOG_ReconnaitreItineraire (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_ReconnaitreItineraire* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_LOG_ReconnaitreItineraire: start\n");

   /* decode itineraire_log */

   PU_PUSHNAME (ctxt_p, "itineraire_log");

   stat = asn1PD_Itineraire (ctxt_p, &pvalue->itineraire_log);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_LOG_ReconnaitreItineraire: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_LOG_ReconnaitreItineraire::Decode ()
{
   return asn1PD_Mission_Automate_LOG_ReconnaitreItineraire (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_LOG_Surveiller                           */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_LOG_Surveiller::ASN1C_Mission_Automate_LOG_Surveiller (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_LOG_Surveiller& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_LOG_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_Surveiller* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_LOG_Surveiller: start\n");

   /* encode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_LOG_Surveiller: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_LOG_Surveiller::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_LOG_Surveiller (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_LOG_Surveiller (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_Surveiller* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_LOG_Surveiller: start\n");

   /* decode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_LOG_Surveiller: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_LOG_Surveiller::Decode ()
{
   return asn1PD_Mission_Automate_LOG_Surveiller (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_LOG_TransporterUnites                    */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_LOG_TransporterUnites::ASN1C_Mission_Automate_LOG_TransporterUnites (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_LOG_TransporterUnites& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_LOG_TransporterUnites (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_TransporterUnites* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_LOG_TransporterUnites: start\n");

   /* encode unites_a_transporter */

   PU_PUSHNAME (ctxt_p, "unites_a_transporter");

   stat = asn1PE_ListAgent (ctxt_p, &pvalue->unites_a_transporter);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode point_rendez_vous */

   PU_PUSHNAME (ctxt_p, "point_rendez_vous");

   stat = asn1PE_Point (ctxt_p, &pvalue->point_rendez_vous);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode point_destination */

   PU_PUSHNAME (ctxt_p, "point_destination");

   stat = asn1PE_Point (ctxt_p, &pvalue->point_destination);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_LOG_TransporterUnites: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_LOG_TransporterUnites::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_LOG_TransporterUnites (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_LOG_TransporterUnites (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_TransporterUnites* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_LOG_TransporterUnites: start\n");

   /* decode unites_a_transporter */

   PU_PUSHNAME (ctxt_p, "unites_a_transporter");

   stat = asn1PD_ListAgent (ctxt_p, &pvalue->unites_a_transporter);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode point_rendez_vous */

   PU_PUSHNAME (ctxt_p, "point_rendez_vous");

   stat = asn1PD_Point (ctxt_p, &pvalue->point_rendez_vous);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode point_destination */

   PU_PUSHNAME (ctxt_p, "point_destination");

   stat = asn1PD_Point (ctxt_p, &pvalue->point_destination);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_LOG_TransporterUnites: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_LOG_TransporterUnites::Decode ()
{
   return asn1PD_Mission_Automate_LOG_TransporterUnites (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_LOG_AppuyerMouvementDansZone             */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_LOG_AppuyerMouvementDansZone::ASN1C_Mission_Automate_LOG_AppuyerMouvementDansZone (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_LOG_AppuyerMouvementDansZone& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_LOG_AppuyerMouvementDansZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_AppuyerMouvementDansZone* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_LOG_AppuyerMouvementDansZone: start\n");

   /* encode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_LOG_AppuyerMouvementDansZone: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_LOG_AppuyerMouvementDansZone::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_LOG_AppuyerMouvementDansZone (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_LOG_AppuyerMouvementDansZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_AppuyerMouvementDansZone* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_LOG_AppuyerMouvementDansZone: start\n");

   /* decode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_LOG_AppuyerMouvementDansZone: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_LOG_AppuyerMouvementDansZone::Decode ()
{
   return asn1PD_Mission_Automate_LOG_AppuyerMouvementDansZone (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_LOG_MettreEnOeuvreZoneStationnement      */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement::ASN1C_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement: start\n");

   /* encode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement: start\n");

   /* decode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement::Decode ()
{
   return asn1PD_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_LOG_AppuyerFranchissementDansZone        */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_LOG_AppuyerFranchissementDansZone::ASN1C_Mission_Automate_LOG_AppuyerFranchissementDansZone (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_LOG_AppuyerFranchissementDansZone& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_LOG_AppuyerFranchissementDansZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_AppuyerFranchissementDansZone* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_LOG_AppuyerFranchissementDansZone: start\n");

   /* encode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_LOG_AppuyerFranchissementDansZone: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_LOG_AppuyerFranchissementDansZone::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_LOG_AppuyerFranchissementDansZone (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_LOG_AppuyerFranchissementDansZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_AppuyerFranchissementDansZone* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_LOG_AppuyerFranchissementDansZone: start\n");

   /* decode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_LOG_AppuyerFranchissementDansZone: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_LOG_AppuyerFranchissementDansZone::Decode ()
{
   return asn1PD_Mission_Automate_LOG_AppuyerFranchissementDansZone (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_LOG_BloquerZone                          */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_LOG_BloquerZone::ASN1C_Mission_Automate_LOG_BloquerZone (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_LOG_BloquerZone& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_LOG_BloquerZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_BloquerZone* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_LOG_BloquerZone: start\n");

   /* encode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_LOG_BloquerZone: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_LOG_BloquerZone::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_LOG_BloquerZone (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_LOG_BloquerZone (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_BloquerZone* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_LOG_BloquerZone: start\n");

   /* decode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_LOG_BloquerZone: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_LOG_BloquerZone::Decode ()
{
   return asn1PD_Mission_Automate_LOG_BloquerZone (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_LOG_ReconnaitreZoneContaminee            */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_LOG_ReconnaitreZoneContaminee::ASN1C_Mission_Automate_LOG_ReconnaitreZoneContaminee (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_LOG_ReconnaitreZoneContaminee& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_LOG_ReconnaitreZoneContaminee (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_ReconnaitreZoneContaminee* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_LOG_ReconnaitreZoneContaminee: start\n");

   /* encode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_LOG_ReconnaitreZoneContaminee: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_LOG_ReconnaitreZoneContaminee::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_LOG_ReconnaitreZoneContaminee (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_LOG_ReconnaitreZoneContaminee (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_ReconnaitreZoneContaminee* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_LOG_ReconnaitreZoneContaminee: start\n");

   /* decode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_LOG_ReconnaitreZoneContaminee: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_LOG_ReconnaitreZoneContaminee::Decode ()
{
   return asn1PD_Mission_Automate_LOG_ReconnaitreZoneContaminee (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_LOG_ReconnaitreZoneDeDeploiement         */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement::ASN1C_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement: start\n");

   /* encode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement: start\n");

   /* decode zone */

   PU_PUSHNAME (ctxt_p, "zone");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement::Decode ()
{
   return asn1PD_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_LOG_ArmerPIAs                            */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_LOG_ArmerPIAs::ASN1C_Mission_Automate_LOG_ArmerPIAs (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_LOG_ArmerPIAs& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_LOG_ArmerPIAs (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_ArmerPIAs* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_LOG_ArmerPIAs: start\n");


   rtdiag ("asn1PE_Mission_Automate_LOG_ArmerPIAs: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_LOG_ArmerPIAs::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_LOG_ArmerPIAs (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_LOG_ArmerPIAs (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_LOG_ArmerPIAs* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_LOG_ArmerPIAs: start\n");

   /* decode  */

   PU_PUSHNAME (ctxt_p, "");


   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_LOG_ArmerPIAs: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_LOG_ArmerPIAs::Decode ()
{
   return asn1PD_Mission_Automate_LOG_ArmerPIAs (mpContext->GetPtr(), &msgData);
}


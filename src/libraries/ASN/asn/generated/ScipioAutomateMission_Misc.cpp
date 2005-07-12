/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 04-Jul-2005.
 */
#include "ScipioAutomateMission_Misc.h"

/**************************************************************/
/*                                                            */
/*  Mission_Automate_Test_MoveTo                              */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_Test_MoveTo::ASN1C_Mission_Automate_Test_MoveTo (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_Test_MoveTo& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_Test_MoveTo (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_Test_MoveTo* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_Test_MoveTo: start\n");

   /* encode itineraire */

   PU_PUSHNAME (ctxt_p, "itineraire");

   stat = asn1PE_Itineraire (ctxt_p, &pvalue->itineraire);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_Test_MoveTo: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_Test_MoveTo::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_Test_MoveTo (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_Test_MoveTo (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_Test_MoveTo* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_Test_MoveTo: start\n");

   /* decode itineraire */

   PU_PUSHNAME (ctxt_p, "itineraire");

   stat = asn1PD_Itineraire (ctxt_p, &pvalue->itineraire);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_Test_MoveTo: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_Test_MoveTo::Decode ()
{
   return asn1PD_Mission_Automate_Test_MoveTo (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_Decrocher                                */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_Decrocher::ASN1C_Mission_Automate_Decrocher (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_Decrocher& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_Decrocher (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_Decrocher* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_Decrocher: start\n");

   /* encode point_regroupement */

   PU_PUSHNAME (ctxt_p, "point_regroupement");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->point_regroupement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_Decrocher: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_Decrocher::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_Decrocher (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_Decrocher (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_Decrocher* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_Decrocher: start\n");

   /* decode point_regroupement */

   PU_PUSHNAME (ctxt_p, "point_regroupement");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->point_regroupement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_Decrocher: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_Decrocher::Decode ()
{
   return asn1PD_Mission_Automate_Decrocher (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_ReleverSurPlace                          */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_ReleverSurPlace::ASN1C_Mission_Automate_ReleverSurPlace (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_ReleverSurPlace& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_ReleverSurPlace (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ReleverSurPlace* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_ReleverSurPlace: start\n");

   /* encode compagnie_a_relever */

   PU_PUSHNAME (ctxt_p, "compagnie_a_relever");

   stat = asn1PE_Automate (ctxt_p, pvalue->compagnie_a_relever);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode zone_installation */

   PU_PUSHNAME (ctxt_p, "zone_installation");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone_installation);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_ReleverSurPlace: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_ReleverSurPlace::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_ReleverSurPlace (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_ReleverSurPlace (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_ReleverSurPlace* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_ReleverSurPlace: start\n");

   /* decode compagnie_a_relever */

   PU_PUSHNAME (ctxt_p, "compagnie_a_relever");

   stat = asn1PD_Automate (ctxt_p, &pvalue->compagnie_a_relever);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode zone_installation */

   PU_PUSHNAME (ctxt_p, "zone_installation");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone_installation);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_ReleverSurPlace: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_ReleverSurPlace::Decode ()
{
   return asn1PD_Mission_Automate_ReleverSurPlace (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_FaireMouvement                           */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_FaireMouvement::ASN1C_Mission_Automate_FaireMouvement (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_FaireMouvement& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_FaireMouvement (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_FaireMouvement* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_FaireMouvement: start\n");

   /* encode itineraire */

   PU_PUSHNAME (ctxt_p, "itineraire");

   stat = asn1PE_Itineraire (ctxt_p, &pvalue->itineraire);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_FaireMouvement: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_FaireMouvement::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_FaireMouvement (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_FaireMouvement (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_FaireMouvement* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_FaireMouvement: start\n");

   /* decode itineraire */

   PU_PUSHNAME (ctxt_p, "itineraire");

   stat = asn1PD_Itineraire (ctxt_p, &pvalue->itineraire);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_FaireMouvement: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_FaireMouvement::Decode ()
{
   return asn1PD_Mission_Automate_FaireMouvement (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_SeFaireDecontaminer                      */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_SeFaireDecontaminer::ASN1C_Mission_Automate_SeFaireDecontaminer (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_SeFaireDecontaminer& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_SeFaireDecontaminer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_SeFaireDecontaminer* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_SeFaireDecontaminer: start\n");

   /* encode zone_arrivee */

   PU_PUSHNAME (ctxt_p, "zone_arrivee");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone_arrivee);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode site_decontamination */

   PU_PUSHNAME (ctxt_p, "site_decontamination");

   stat = asn1PE_KnowledgeObject (ctxt_p, pvalue->site_decontamination);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_SeFaireDecontaminer: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_SeFaireDecontaminer::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_SeFaireDecontaminer (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_SeFaireDecontaminer (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_SeFaireDecontaminer* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_SeFaireDecontaminer: start\n");

   /* decode zone_arrivee */

   PU_PUSHNAME (ctxt_p, "zone_arrivee");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone_arrivee);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode site_decontamination */

   PU_PUSHNAME (ctxt_p, "site_decontamination");

   stat = asn1PD_KnowledgeObject (ctxt_p, &pvalue->site_decontamination);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_SeFaireDecontaminer: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_SeFaireDecontaminer::Decode ()
{
   return asn1PD_Mission_Automate_SeFaireDecontaminer (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_Franchir                                 */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_Franchir::ASN1C_Mission_Automate_Franchir (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_Franchir& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_Franchir (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_Franchir* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_Franchir: start\n");

   /* encode pont_flottant */

   PU_PUSHNAME (ctxt_p, "pont_flottant");

   stat = asn1PE_KnowledgeObject (ctxt_p, pvalue->pont_flottant);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode zone_arrivee */

   PU_PUSHNAME (ctxt_p, "zone_arrivee");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone_arrivee);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_Franchir: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_Franchir::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_Franchir (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_Franchir (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_Franchir* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_Franchir: start\n");

   /* decode pont_flottant */

   PU_PUSHNAME (ctxt_p, "pont_flottant");

   stat = asn1PD_KnowledgeObject (ctxt_p, &pvalue->pont_flottant);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode zone_arrivee */

   PU_PUSHNAME (ctxt_p, "zone_arrivee");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone_arrivee);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_Franchir: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_Franchir::Decode ()
{
   return asn1PD_Mission_Automate_Franchir (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_Stationner                               */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_Stationner::ASN1C_Mission_Automate_Stationner (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_Stationner& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_Stationner (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_Stationner* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_Stationner: start\n");

   /* encode zone_attente */

   PU_PUSHNAME (ctxt_p, "zone_attente");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone_attente);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_Stationner: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_Stationner::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_Stationner (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_Stationner (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_Stationner* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_Stationner: start\n");

   /* decode zone_attente */

   PU_PUSHNAME (ctxt_p, "zone_attente");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone_attente);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_Stationner: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_Stationner::Decode ()
{
   return asn1PD_Mission_Automate_Stationner (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_SeFaireTransporter                       */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_SeFaireTransporter::ASN1C_Mission_Automate_SeFaireTransporter (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_SeFaireTransporter& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_SeFaireTransporter (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_SeFaireTransporter* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_SeFaireTransporter: start\n");

   /* encode zone_embarquement */

   PU_PUSHNAME (ctxt_p, "zone_embarquement");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone_embarquement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_SeFaireTransporter: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_SeFaireTransporter::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_SeFaireTransporter (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_SeFaireTransporter (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_SeFaireTransporter* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_SeFaireTransporter: start\n");

   /* decode zone_embarquement */

   PU_PUSHNAME (ctxt_p, "zone_embarquement");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone_embarquement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_SeFaireTransporter: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_SeFaireTransporter::Decode ()
{
   return asn1PD_Mission_Automate_SeFaireTransporter (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_SeRecompleter                            */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_SeRecompleter::ASN1C_Mission_Automate_SeRecompleter (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_SeRecompleter& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_SeRecompleter (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_SeRecompleter* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_SeRecompleter: start\n");

   /* encode zone_ravitaillement */

   PU_PUSHNAME (ctxt_p, "zone_ravitaillement");

   stat = asn1PE_Polygon (ctxt_p, &pvalue->zone_ravitaillement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_SeRecompleter: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_SeRecompleter::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_SeRecompleter (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_SeRecompleter (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_SeRecompleter* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_SeRecompleter: start\n");

   /* decode zone_ravitaillement */

   PU_PUSHNAME (ctxt_p, "zone_ravitaillement");

   stat = asn1PD_Polygon (ctxt_p, &pvalue->zone_ravitaillement);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_SeRecompleter: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_SeRecompleter::Decode ()
{
   return asn1PD_Mission_Automate_SeRecompleter (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_SeRendre                                 */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_SeRendre::ASN1C_Mission_Automate_SeRendre (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_SeRendre& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_SeRendre (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_SeRendre* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_SeRendre: start\n");


   rtdiag ("asn1PE_Mission_Automate_SeRendre: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_SeRendre::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_SeRendre (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_SeRendre (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_SeRendre* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_SeRendre: start\n");

   /* decode  */

   PU_PUSHNAME (ctxt_p, "");


   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_SeRendre: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_SeRendre::Decode ()
{
   return asn1PD_Mission_Automate_SeRendre (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_FaireMouvementVersCampRefugies           */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_FaireMouvementVersCampRefugies::ASN1C_Mission_Automate_FaireMouvementVersCampRefugies (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_FaireMouvementVersCampRefugies& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_FaireMouvementVersCampRefugies (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_FaireMouvementVersCampRefugies* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_FaireMouvementVersCampRefugies: start\n");


   rtdiag ("asn1PE_Mission_Automate_FaireMouvementVersCampRefugies: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_FaireMouvementVersCampRefugies::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_FaireMouvementVersCampRefugies (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_FaireMouvementVersCampRefugies (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_FaireMouvementVersCampRefugies* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_FaireMouvementVersCampRefugies: start\n");

   /* decode  */

   PU_PUSHNAME (ctxt_p, "");


   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_FaireMouvementVersCampRefugies: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_FaireMouvementVersCampRefugies::Decode ()
{
   return asn1PD_Mission_Automate_FaireMouvementVersCampRefugies (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Automate_Generique                                */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Automate_Generique::ASN1C_Mission_Automate_Generique (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate_Generique& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Automate_Generique (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_Generique* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Automate_Generique: start\n");

   /* encode zones */

   PU_PUSHNAME (ctxt_p, "zones");

   stat = asn1PE_ListPolygon (ctxt_p, &pvalue->zones);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode points */

   PU_PUSHNAME (ctxt_p, "points");

   stat = asn1PE_ListPoint (ctxt_p, &pvalue->points);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode itineraires */

   PU_PUSHNAME (ctxt_p, "itineraires");

   stat = asn1PE_ListItineraire (ctxt_p, &pvalue->itineraires);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode unites */

   PU_PUSHNAME (ctxt_p, "unites");

   stat = asn1PE_ListAgent (ctxt_p, &pvalue->unites);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode automates */

   PU_PUSHNAME (ctxt_p, "automates");

   stat = asn1PE_ListAutomate (ctxt_p, &pvalue->automates);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode connaissances_agents */

   PU_PUSHNAME (ctxt_p, "connaissances_agents");

   stat = asn1PE_ListKnowledgeAgent (ctxt_p, &pvalue->connaissances_agents);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode connaissances_objets */

   PU_PUSHNAME (ctxt_p, "connaissances_objets");

   stat = asn1PE_ListKnowledgeObject (ctxt_p, &pvalue->connaissances_objets);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Mission_Automate_Generique: end\n");
   return (stat);
}

int ASN1C_Mission_Automate_Generique::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Automate_Generique (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Automate_Generique (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate_Generique* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Mission_Automate_Generique: start\n");

   /* decode zones */

   PU_PUSHNAME (ctxt_p, "zones");

   stat = asn1PD_ListPolygon (ctxt_p, &pvalue->zones);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode points */

   PU_PUSHNAME (ctxt_p, "points");

   stat = asn1PD_ListPoint (ctxt_p, &pvalue->points);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode itineraires */

   PU_PUSHNAME (ctxt_p, "itineraires");

   stat = asn1PD_ListItineraire (ctxt_p, &pvalue->itineraires);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode unites */

   PU_PUSHNAME (ctxt_p, "unites");

   stat = asn1PD_ListAgent (ctxt_p, &pvalue->unites);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode automates */

   PU_PUSHNAME (ctxt_p, "automates");

   stat = asn1PD_ListAutomate (ctxt_p, &pvalue->automates);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode connaissances_agents */

   PU_PUSHNAME (ctxt_p, "connaissances_agents");

   stat = asn1PD_ListKnowledgeAgent (ctxt_p, &pvalue->connaissances_agents);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode connaissances_objets */

   PU_PUSHNAME (ctxt_p, "connaissances_objets");

   stat = asn1PD_ListKnowledgeObject (ctxt_p, &pvalue->connaissances_objets);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Mission_Automate_Generique: end\n");

   return (stat);
}

int ASN1C_Mission_Automate_Generique::Decode ()
{
   return asn1PD_Mission_Automate_Generique (mpContext->GetPtr(), &msgData);
}


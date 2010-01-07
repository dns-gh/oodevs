/* This file was generated by ASN1C V5.31 on 06-Jan-2010 */

#include "LogRavitaillement.h"

void asn1Print_LogRavitaillementIncludes (ASN1ConstCharPtr name, ASN1T_LogRavitaillementIncludes* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.baseExports", name);

}

void ASN1C_LogRavitaillementIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_LogRavitaillementIncludes (name, &msgData);
}

void asn1Print_LogRavitaillementExports (ASN1ConstCharPtr name)
{
}

void ASN1C_LogRavitaillementExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_LogRavitaillementExports (name);
}

void asn1Print_DotationQuery (ASN1ConstCharPtr name, ASN1T_DotationQuery* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.ressource_id", name);
   asn1Print_DotationType (namebuf, &pvalue->ressource_id);

   sprintf (namebuf, "%s.quantite_demandee", name);
   rtPrintInteger (namebuf, pvalue->quantite_demandee);

   sprintf (namebuf, "%s.quantite_accordee", name);
   rtPrintInteger (namebuf, pvalue->quantite_accordee);

   sprintf (namebuf, "%s.quantite_en_transit", name);
   rtPrintInteger (namebuf, pvalue->quantite_en_transit);

}

void ASN1C_DotationQuery::Print (ASN1ConstCharPtr name)

{
   asn1Print_DotationQuery (name, &msgData);
}

void asn1Print__SeqOfDotationQuery (ASN1ConstCharPtr name, ASN1T__SeqOfDotationQuery* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_DotationQuery (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C__SeqOfDotationQuery::Print (ASN1ConstCharPtr name)

{
   asn1Print__SeqOfDotationQuery (name, &msgData);
}

void asn1Print_MsgLogSupplyHandlingCreation (ASN1ConstCharPtr name, ASN1T_MsgLogSupplyHandlingCreation* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_consigne", name);
   asn1Print_OID (namebuf, &pvalue->oid_consigne);

   sprintf (namebuf, "%s.oid_automate", name);
   asn1Print_Automat (namebuf, &pvalue->oid_automate);

   sprintf (namebuf, "%s.tick_creation", name);
   rtPrintInteger (namebuf, pvalue->tick_creation);

   sprintf (namebuf, "%s.dotations", name);
   asn1Print__SeqOfDotationQuery (namebuf, &pvalue->dotations);

}

void ASN1C_MsgLogSupplyHandlingCreation::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgLogSupplyHandlingCreation (name, &msgData);
}

void asn1Print_EnumLogSupplyHandlingStatus (ASN1ConstCharPtr name, ASN1T_EnumLogSupplyHandlingStatus* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("convoi_en_attente_camions\n"); break;
      case 1: printf ("convoi_constitution\n"); break;
      case 2: printf ("convoi_deplacement_vers_point_chargement\n"); break;
      case 3: printf ("convoi_chargement\n"); break;
      case 4: printf ("convoi_deplacement_vers_point_dechargement\n"); break;
      case 5: printf ("convoi_dechargement\n"); break;
      case 6: printf ("convoi_deplacement_retour\n"); break;
      case 7: printf ("termine\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumLogSupplyHandlingStatus::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumLogSupplyHandlingStatus (name, &msgData);
}

void asn1Print_MsgLogSupplyHandlingUpdate (ASN1ConstCharPtr name, ASN1T_MsgLogSupplyHandlingUpdate* pvalue)
{
   char namebuf[512];

   printf ("%s.m.oid_automate_log_traitantPresent = %d\n", name, (int)pvalue->m.oid_automate_log_traitantPresent);
   printf ("%s.m.oid_automate_log_fournissant_moyens_convoiPresent = %d\n", name, (int)pvalue->m.oid_automate_log_fournissant_moyens_convoiPresent);
   printf ("%s.m.oid_pion_convoyantPresent = %d\n", name, (int)pvalue->m.oid_pion_convoyantPresent);
   printf ("%s.m.etatPresent = %d\n", name, (int)pvalue->m.etatPresent);
   printf ("%s.m.dotationsPresent = %d\n", name, (int)pvalue->m.dotationsPresent);
   sprintf (namebuf, "%s.oid_consigne", name);
   asn1Print_OID (namebuf, &pvalue->oid_consigne);

   sprintf (namebuf, "%s.oid_automate", name);
   asn1Print_Automat (namebuf, &pvalue->oid_automate);

   if (pvalue->m.oid_automate_log_traitantPresent) {
      sprintf (namebuf, "%s.oid_automate_log_traitant", name);
      asn1Print_Automat (namebuf, &pvalue->oid_automate_log_traitant);
   }

   if (pvalue->m.oid_automate_log_fournissant_moyens_convoiPresent) {
      sprintf (namebuf, "%s.oid_automate_log_fournissant_moyens_convoi", name);
      asn1Print_Automat (namebuf, &pvalue->oid_automate_log_fournissant_moyens_convoi);
   }

   if (pvalue->m.oid_pion_convoyantPresent) {
      sprintf (namebuf, "%s.oid_pion_convoyant", name);
      asn1Print_Unit (namebuf, &pvalue->oid_pion_convoyant);
   }

   if (pvalue->m.etatPresent) {
      sprintf (namebuf, "%s.etat", name);
      asn1Print_EnumLogSupplyHandlingStatus (namebuf, &pvalue->etat);
   }

   if (pvalue->m.dotationsPresent) {
      sprintf (namebuf, "%s.dotations", name);
      asn1Print__SeqOfDotationQuery (namebuf, &pvalue->dotations);
   }

}

void ASN1C_MsgLogSupplyHandlingUpdate::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgLogSupplyHandlingUpdate (name, &msgData);
}

void asn1Print_MsgLogSupplyHandlingDestruction (ASN1ConstCharPtr name, ASN1T_MsgLogSupplyHandlingDestruction* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_consigne", name);
   asn1Print_OID (namebuf, &pvalue->oid_consigne);

   sprintf (namebuf, "%s.oid_automate", name);
   asn1Print_Automat (namebuf, &pvalue->oid_automate);

}

void ASN1C_MsgLogSupplyHandlingDestruction::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgLogSupplyHandlingDestruction (name, &msgData);
}

void asn1Print_LogSupplyEquimentAvailability (ASN1ConstCharPtr name, ASN1T_LogSupplyEquimentAvailability* pvalue)
{
   char namebuf[512];

   printf ("%s.m.nbr_au_reposPresent = %d\n", name, (int)pvalue->m.nbr_au_reposPresent);
   sprintf (namebuf, "%s.type_equipement", name);
   asn1Print_EquipmentType (namebuf, &pvalue->type_equipement);

   sprintf (namebuf, "%s.nbr_total", name);
   rtPrintInteger (namebuf, pvalue->nbr_total);

   sprintf (namebuf, "%s.nbr_disponibles", name);
   rtPrintInteger (namebuf, pvalue->nbr_disponibles);

   sprintf (namebuf, "%s.nbr_au_travail", name);
   rtPrintInteger (namebuf, pvalue->nbr_au_travail);

   sprintf (namebuf, "%s.nbr_pretes", name);
   rtPrintInteger (namebuf, pvalue->nbr_pretes);

   if (pvalue->m.nbr_au_reposPresent) {
      sprintf (namebuf, "%s.nbr_au_repos", name);
      rtPrintInteger (namebuf, pvalue->nbr_au_repos);
   }

}

void ASN1C_LogSupplyEquimentAvailability::Print (ASN1ConstCharPtr name)

{
   asn1Print_LogSupplyEquimentAvailability (name, &msgData);
}

void asn1Print_DotationStock (ASN1ConstCharPtr name, ASN1T_DotationStock* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.ressource_id", name);
   asn1Print_DotationType (namebuf, &pvalue->ressource_id);

   sprintf (namebuf, "%s.quantite_disponible", name);
   rtPrintInteger (namebuf, pvalue->quantite_disponible);

}

void ASN1C_DotationStock::Print (ASN1ConstCharPtr name)

{
   asn1Print_DotationStock (name, &msgData);
}

void asn1Print__SeqOfDotationStock (ASN1ConstCharPtr name, ASN1T__SeqOfDotationStock* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_DotationStock (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C__SeqOfDotationStock::Print (ASN1ConstCharPtr name)

{
   asn1Print__SeqOfDotationStock (name, &msgData);
}

void asn1Print__SeqOfLogSupplyEquimentAvailability (ASN1ConstCharPtr name, ASN1T__SeqOfLogSupplyEquimentAvailability* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_LogSupplyEquimentAvailability (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C__SeqOfLogSupplyEquimentAvailability::Print (ASN1ConstCharPtr name)

{
   asn1Print__SeqOfLogSupplyEquimentAvailability (name, &msgData);
}

void asn1Print_MsgLogSupplyState (ASN1ConstCharPtr name, ASN1T_MsgLogSupplyState* pvalue)
{
   char namebuf[512];

   printf ("%s.m.chaine_activeePresent = %d\n", name, (int)pvalue->m.chaine_activeePresent);
   printf ("%s.m.stocksPresent = %d\n", name, (int)pvalue->m.stocksPresent);
   printf ("%s.m.disponibilites_transporteurs_convoisPresent = %d\n", name, (int)pvalue->m.disponibilites_transporteurs_convoisPresent);
   sprintf (namebuf, "%s.oid_pion", name);
   asn1Print_Unit (namebuf, &pvalue->oid_pion);

   if (pvalue->m.chaine_activeePresent) {
      sprintf (namebuf, "%s.chaine_activee", name);
      rtPrintBoolean (namebuf, pvalue->chaine_activee);
   }

   if (pvalue->m.stocksPresent) {
      sprintf (namebuf, "%s.stocks", name);
      asn1Print__SeqOfDotationStock (namebuf, &pvalue->stocks);
   }

   if (pvalue->m.disponibilites_transporteurs_convoisPresent) {
      sprintf (namebuf, "%s.disponibilites_transporteurs_convois", name);
      asn1Print__SeqOfLogSupplyEquimentAvailability (namebuf, &pvalue->disponibilites_transporteurs_convois);
   }

}

void ASN1C_MsgLogSupplyState::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgLogSupplyState (name, &msgData);
}

void asn1Print_DotationQuota (ASN1ConstCharPtr name, ASN1T_DotationQuota* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.ressource_id", name);
   asn1Print_DotationType (namebuf, &pvalue->ressource_id);

   sprintf (namebuf, "%s.quota_disponible", name);
   rtPrintInteger (namebuf, pvalue->quota_disponible);

}

void ASN1C_DotationQuota::Print (ASN1ConstCharPtr name)

{
   asn1Print_DotationQuota (name, &msgData);
}

void asn1Print__SeqOfDotationQuota (ASN1ConstCharPtr name, ASN1T__SeqOfDotationQuota* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_DotationQuota (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C__SeqOfDotationQuota::Print (ASN1ConstCharPtr name)

{
   asn1Print__SeqOfDotationQuota (name, &msgData);
}

void asn1Print_MsgLogSupplyChangeQuotas (ASN1ConstCharPtr name, ASN1T_MsgLogSupplyChangeQuotas* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_donneur", name);
   asn1Print_Automat (namebuf, &pvalue->oid_donneur);

   sprintf (namebuf, "%s.oid_receveur", name);
   asn1Print_Automat (namebuf, &pvalue->oid_receveur);

   sprintf (namebuf, "%s.quotas", name);
   asn1Print__SeqOfDotationQuota (namebuf, &pvalue->quotas);

}

void ASN1C_MsgLogSupplyChangeQuotas::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgLogSupplyChangeQuotas (name, &msgData);
}

void asn1Print_MsgLogSupplyChangeQuotasAck (ASN1ConstCharPtr name, ASN1T_MsgLogSupplyChangeQuotasAck* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("no_error\n"); break;
      case 1: printf ("error_invalid_donneur\n"); break;
      case 2: printf ("error_invalid_receveur\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_MsgLogSupplyChangeQuotasAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgLogSupplyChangeQuotasAck (name, &msgData);
}

void asn1Print_MsgLogSupplyPushFlow (ASN1ConstCharPtr name, ASN1T_MsgLogSupplyPushFlow* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_donneur", name);
   asn1Print_Automat (namebuf, &pvalue->oid_donneur);

   sprintf (namebuf, "%s.oid_receveur", name);
   asn1Print_Automat (namebuf, &pvalue->oid_receveur);

   sprintf (namebuf, "%s.stocks", name);
   asn1Print__SeqOfDotationStock (namebuf, &pvalue->stocks);

}

void ASN1C_MsgLogSupplyPushFlow::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgLogSupplyPushFlow (name, &msgData);
}

void asn1Print_MsgLogSupplyPushFlowAck (ASN1ConstCharPtr name, ASN1T_MsgLogSupplyPushFlowAck* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("no_error\n"); break;
      case 1: printf ("error_invalid_donneur\n"); break;
      case 2: printf ("error_invalid_receveur\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_MsgLogSupplyPushFlowAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgLogSupplyPushFlowAck (name, &msgData);
}

void asn1Print_MsgLogSupplyQuotas (ASN1ConstCharPtr name, ASN1T_MsgLogSupplyQuotas* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid_automate", name);
   asn1Print_Automat (namebuf, &pvalue->oid_automate);

   sprintf (namebuf, "%s.quotas", name);
   asn1Print__SeqOfDotationQuota (namebuf, &pvalue->quotas);

}

void ASN1C_MsgLogSupplyQuotas::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgLogSupplyQuotas (name, &msgData);
}


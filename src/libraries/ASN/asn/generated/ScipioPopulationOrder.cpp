/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 23-Nov-2005.
 */
#include "ScipioPopulationOrder.h"

/**************************************************************/
/*                                                            */
/*  MsgPopulationOrderAck                                     */
/*                                                            */
/**************************************************************/

ASN1C_MsgPopulationOrderAck::ASN1C_MsgPopulationOrderAck (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationOrderAck& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgPopulationOrderAck (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationOrderAck* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgPopulationOrderAck: start\n");

   /* encode oid_unite_executante */

   PU_PUSHNAME (ctxt_p, "oid_unite_executante");

   stat = asn1PE_Population (ctxt_p, pvalue->oid_unite_executante);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode order_id */

   PU_PUSHNAME (ctxt_p, "order_id");

   stat = asn1PE_OID (ctxt_p, pvalue->order_id);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode error_code */

   PU_PUSHNAME (ctxt_p, "error_code");

   stat = asn1PE_EnumOrderErrorCode (ctxt_p, pvalue->error_code);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_MsgPopulationOrderAck: end\n");
   return (stat);
}

int ASN1C_MsgPopulationOrderAck::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgPopulationOrderAck (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgPopulationOrderAck (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationOrderAck* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgPopulationOrderAck: start\n");

   /* decode oid_unite_executante */

   PU_PUSHNAME (ctxt_p, "oid_unite_executante");

   stat = asn1PD_Population (ctxt_p, &pvalue->oid_unite_executante);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode order_id */

   PU_PUSHNAME (ctxt_p, "order_id");

   stat = asn1PD_OID (ctxt_p, &pvalue->order_id);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode error_code */

   PU_PUSHNAME (ctxt_p, "error_code");

   stat = asn1PD_EnumOrderErrorCode (ctxt_p, &pvalue->error_code);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_MsgPopulationOrderAck: end\n");

   return (stat);
}

int ASN1C_MsgPopulationOrderAck::Decode ()
{
   return asn1PD_MsgPopulationOrderAck (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Mission_Population                                        */
/*                                                            */
/**************************************************************/

ASN1C_Mission_Population::ASN1C_Mission_Population (
   ASN1MessageBuffer& msgBuf, ASN1T_Mission_Population& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Mission_Population (ASN1CTXT* ctxt_p, ASN1T_Mission_Population* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Mission_Population: start\n");

   /* Encode choice index value */

   PU_PUSHNAME (ctxt_p, "t");

   stat = pe_ConsUnsigned (ctxt_p, pvalue->t - 1, 0, 0);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* Encode root element data value */

   switch (pvalue->t)
   {
      /* mission_population_faire_mouvement */
      case 1:
         PU_PUSHNAME (ctxt_p, "u.mission_population_faire_mouvement");

         stat = asn1PE_Mission_Population_FaireMouvement (ctxt_p, pvalue->u.mission_population_faire_mouvement);
         if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
         PU_POPNAME (ctxt_p);

         break;

      default:
         return LOG_ASN1ERR (ctxt_p, ASN_E_INVOPT);
   }

   rtdiag ("asn1PE_Mission_Population: end\n");
   return (stat);
}

int ASN1C_Mission_Population::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Mission_Population (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Mission_Population (ASN1CTXT* ctxt_p, ASN1T_Mission_Population* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   rtdiag ("asn1PD_Mission_Population: start\n");

   PU_PUSHNAME (ctxt_p, "t");

   stat = pd_ConsUnsigned (ctxt_p, &ui, 0, 0);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   else pvalue->t = ui + 1;

   PU_POPNAME (ctxt_p);

   switch (ui) {
      /* mission_population_faire_mouvement */
      case 0:
         PU_PUSHNAME (ctxt_p, "u.mission_population_faire_mouvement");

         pvalue->u.mission_population_faire_mouvement = ALLOC_ASN1ELEM (ctxt_p, ASN1T_Mission_Population_FaireMouvement);
         if (pvalue->u.mission_population_faire_mouvement == NULL)
            return LOG_ASN1ERR (ctxt_p, ASN_E_NOMEM);

         stat = asn1PD_Mission_Population_FaireMouvement (ctxt_p, pvalue->u.mission_population_faire_mouvement);
         if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

         PU_POPNAME (ctxt_p);

         break;

      default:
         return LOG_ASN1ERR (ctxt_p, ASN_E_INVOPT);
   }

   rtdiag ("asn1PD_Mission_Population: end\n");

   return (stat);
}

int ASN1C_Mission_Population::Decode ()
{
   return asn1PD_Mission_Population (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgPopulationOrder                                        */
/*                                                            */
/**************************************************************/

ASN1C_MsgPopulationOrder::ASN1C_MsgPopulationOrder (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgPopulationOrder& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgPopulationOrder (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationOrder* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_MsgPopulationOrder: start\n");

   /* encode oid_unite_executante */

   PU_PUSHNAME (ctxt_p, "oid_unite_executante");

   stat = asn1PE_Population (ctxt_p, pvalue->oid_unite_executante);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode order_id */

   PU_PUSHNAME (ctxt_p, "order_id");

   stat = asn1PE_OID (ctxt_p, pvalue->order_id);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode mission */

   PU_PUSHNAME (ctxt_p, "mission");

   stat = asn1PE_Mission_Population (ctxt_p, &pvalue->mission);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_MsgPopulationOrder: end\n");
   return (stat);
}

int ASN1C_MsgPopulationOrder::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgPopulationOrder (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgPopulationOrder (ASN1CTXT* ctxt_p, ASN1T_MsgPopulationOrder* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_MsgPopulationOrder: start\n");

   /* decode oid_unite_executante */

   PU_PUSHNAME (ctxt_p, "oid_unite_executante");

   stat = asn1PD_Population (ctxt_p, &pvalue->oid_unite_executante);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode order_id */

   PU_PUSHNAME (ctxt_p, "order_id");

   stat = asn1PD_OID (ctxt_p, &pvalue->order_id);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode mission */

   PU_PUSHNAME (ctxt_p, "mission");

   stat = asn1PD_Mission_Population (ctxt_p, &pvalue->mission);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_MsgPopulationOrder: end\n");

   return (stat);
}

int ASN1C_MsgPopulationOrder::Decode ()
{
   return asn1PD_MsgPopulationOrder (mpContext->GetPtr(), &msgData);
}


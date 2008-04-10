/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 07-Apr-2008.
 */
#include "Fire.h"

/**************************************************************/
/*                                                            */
/*  FireIncludes                                              */
/*                                                            */
/**************************************************************/

ASN1C_FireIncludes::ASN1C_FireIncludes (
   ASN1MessageBuffer& msgBuf, ASN1T_FireIncludes& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_FireIncludes (ASN1CTXT* ctxt_p, ASN1T_FireIncludes* pvalue)
{
   int stat = ASN_OK;

   /* encode baseExports */

   stat = asn1PE_BaseExports (ctxt_p);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_FireIncludes::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_FireIncludes (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_FireIncludes (ASN1CTXT* ctxt_p, ASN1T_FireIncludes* pvalue)
{
   int stat = ASN_OK;

   /* decode baseExports */

   stat = asn1PD_BaseExports (ctxt_p);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_FireIncludes::Decode ()
{
   return asn1PD_FireIncludes (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  FireExports                                               */
/*                                                            */
/**************************************************************/

ASN1C_FireExports::ASN1C_FireExports (
   ASN1MessageBuffer& msgBuf) :
   ASN1CType(msgBuf)
{}

EXTERN int asn1PE_FireExports (ASN1CTXT* ctxt_p)
{
   int stat = ASN_OK;

   /* NULL */
   return (stat);
}

int ASN1C_FireExports::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_FireExports (mpContext->GetPtr());
   return stat;
}

EXTERN int asn1PD_FireExports (ASN1CTXT* ctxt_p)
{
   int stat = ASN_OK;

   /* NULL */

   return (stat);
}

int ASN1C_FireExports::Decode ()
{
   return asn1PD_FireExports (mpContext->GetPtr());
}

/**************************************************************/
/*                                                            */
/*  UnitHumanFireDamage                                       */
/*                                                            */
/**************************************************************/

ASN1C_UnitHumanFireDamage::ASN1C_UnitHumanFireDamage (
   ASN1MessageBuffer& msgBuf, ASN1T_UnitHumanFireDamage& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_UnitHumanFireDamage (ASN1CTXT* ctxt_p, ASN1T_UnitHumanFireDamage* pvalue)
{
   int stat = ASN_OK;

   /* encode rank */

   stat = asn1PE_EnumHumanRank (ctxt_p, pvalue->rank);
   if (stat != ASN_OK) return stat;

   /* encode alive_nbr */

   stat = pe_UnconsInteger (ctxt_p, pvalue->alive_nbr);
   if (stat != ASN_OK) return stat;

   /* encode dead_nbr */

   stat = pe_UnconsInteger (ctxt_p, pvalue->dead_nbr);
   if (stat != ASN_OK) return stat;

   /* encode wounded_u1_nbr */

   stat = pe_UnconsInteger (ctxt_p, pvalue->wounded_u1_nbr);
   if (stat != ASN_OK) return stat;

   /* encode wounded_u2_nbr */

   stat = pe_UnconsInteger (ctxt_p, pvalue->wounded_u2_nbr);
   if (stat != ASN_OK) return stat;

   /* encode wounded_u3_nbr */

   stat = pe_UnconsInteger (ctxt_p, pvalue->wounded_u3_nbr);
   if (stat != ASN_OK) return stat;

   /* encode wounded_ue_nbr */

   stat = pe_UnconsInteger (ctxt_p, pvalue->wounded_ue_nbr);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_UnitHumanFireDamage::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_UnitHumanFireDamage (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_UnitHumanFireDamage (ASN1CTXT* ctxt_p, ASN1T_UnitHumanFireDamage* pvalue)
{
   int stat = ASN_OK;

   /* decode rank */

   stat = asn1PD_EnumHumanRank (ctxt_p, &pvalue->rank);
   if (stat != ASN_OK) return stat;

   /* decode alive_nbr */

   stat = pd_UnconsInteger (ctxt_p, &pvalue->alive_nbr);
   if (stat != ASN_OK) return stat;

   /* decode dead_nbr */

   stat = pd_UnconsInteger (ctxt_p, &pvalue->dead_nbr);
   if (stat != ASN_OK) return stat;

   /* decode wounded_u1_nbr */

   stat = pd_UnconsInteger (ctxt_p, &pvalue->wounded_u1_nbr);
   if (stat != ASN_OK) return stat;

   /* decode wounded_u2_nbr */

   stat = pd_UnconsInteger (ctxt_p, &pvalue->wounded_u2_nbr);
   if (stat != ASN_OK) return stat;

   /* decode wounded_u3_nbr */

   stat = pd_UnconsInteger (ctxt_p, &pvalue->wounded_u3_nbr);
   if (stat != ASN_OK) return stat;

   /* decode wounded_ue_nbr */

   stat = pd_UnconsInteger (ctxt_p, &pvalue->wounded_ue_nbr);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_UnitHumanFireDamage::Decode ()
{
   return asn1PD_UnitHumanFireDamage (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  UnitEquipmentFireDamage                                   */
/*                                                            */
/**************************************************************/

ASN1C_UnitEquipmentFireDamage::ASN1C_UnitEquipmentFireDamage (
   ASN1MessageBuffer& msgBuf, ASN1T_UnitEquipmentFireDamage& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_UnitEquipmentFireDamage (ASN1CTXT* ctxt_p, ASN1T_UnitEquipmentFireDamage* pvalue)
{
   int stat = ASN_OK;

   /* encode equipement_type */

   stat = asn1PE_EquipmentType (ctxt_p, pvalue->equipement_type);
   if (stat != ASN_OK) return stat;

   /* encode available_nbr */

   stat = pe_UnconsInteger (ctxt_p, pvalue->available_nbr);
   if (stat != ASN_OK) return stat;

   /* encode unavailable_nbr */

   stat = pe_UnconsInteger (ctxt_p, pvalue->unavailable_nbr);
   if (stat != ASN_OK) return stat;

   /* encode repairable_nbr */

   stat = pe_UnconsInteger (ctxt_p, pvalue->repairable_nbr);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_UnitEquipmentFireDamage::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_UnitEquipmentFireDamage (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_UnitEquipmentFireDamage (ASN1CTXT* ctxt_p, ASN1T_UnitEquipmentFireDamage* pvalue)
{
   int stat = ASN_OK;

   /* decode equipement_type */

   stat = asn1PD_EquipmentType (ctxt_p, &pvalue->equipement_type);
   if (stat != ASN_OK) return stat;

   /* decode available_nbr */

   stat = pd_UnconsInteger (ctxt_p, &pvalue->available_nbr);
   if (stat != ASN_OK) return stat;

   /* decode unavailable_nbr */

   stat = pd_UnconsInteger (ctxt_p, &pvalue->unavailable_nbr);
   if (stat != ASN_OK) return stat;

   /* decode repairable_nbr */

   stat = pd_UnconsInteger (ctxt_p, &pvalue->repairable_nbr);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_UnitEquipmentFireDamage::Decode ()
{
   return asn1PD_UnitEquipmentFireDamage (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfUnitHumanFireDamage                                 */
/*                                                            */
/**************************************************************/

ASN1C__SeqOfUnitHumanFireDamage::ASN1C__SeqOfUnitHumanFireDamage (
   ASN1MessageBuffer& msgBuf, ASN1T__SeqOfUnitHumanFireDamage& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE__SeqOfUnitHumanFireDamage (ASN1CTXT* ctxt_p, ASN1T__SeqOfUnitHumanFireDamage* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* encode length determinant */

   stat = pe_Length (ctxt_p, pvalue->n);
   if (stat < 0) return stat;

   /* encode elements */

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      stat = asn1PE_UnitHumanFireDamage (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return stat;
   }
   return (stat);
}

int ASN1C__SeqOfUnitHumanFireDamage::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE__SeqOfUnitHumanFireDamage (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD__SeqOfUnitHumanFireDamage (ASN1CTXT* ctxt_p, ASN1T__SeqOfUnitHumanFireDamage* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = pd_Length (ctxt_p, &pvalue->n);
   if (stat != ASN_OK) return stat;

   /* decode elements */

   ALLOC_ASN1ARRAY (ctxt_p, pvalue, ASN1T_UnitHumanFireDamage);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      stat = asn1PD_UnitHumanFireDamage (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return stat;
   }


   return (stat);
}

int ASN1C__SeqOfUnitHumanFireDamage::Decode ()
{
   return asn1PD__SeqOfUnitHumanFireDamage (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfUnitEquipmentFireDamage                             */
/*                                                            */
/**************************************************************/

ASN1C__SeqOfUnitEquipmentFireDamage::ASN1C__SeqOfUnitEquipmentFireDamage (
   ASN1MessageBuffer& msgBuf, ASN1T__SeqOfUnitEquipmentFireDamage& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE__SeqOfUnitEquipmentFireDamage (ASN1CTXT* ctxt_p, ASN1T__SeqOfUnitEquipmentFireDamage* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* encode length determinant */

   stat = pe_Length (ctxt_p, pvalue->n);
   if (stat < 0) return stat;

   /* encode elements */

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      stat = asn1PE_UnitEquipmentFireDamage (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return stat;
   }
   return (stat);
}

int ASN1C__SeqOfUnitEquipmentFireDamage::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE__SeqOfUnitEquipmentFireDamage (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD__SeqOfUnitEquipmentFireDamage (ASN1CTXT* ctxt_p, ASN1T__SeqOfUnitEquipmentFireDamage* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = pd_Length (ctxt_p, &pvalue->n);
   if (stat != ASN_OK) return stat;

   /* decode elements */

   ALLOC_ASN1ARRAY (ctxt_p, pvalue, ASN1T_UnitEquipmentFireDamage);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      stat = asn1PD_UnitEquipmentFireDamage (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return stat;
   }


   return (stat);
}

int ASN1C__SeqOfUnitEquipmentFireDamage::Decode ()
{
   return asn1PD__SeqOfUnitEquipmentFireDamage (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  UnitFireDamages                                           */
/*                                                            */
/**************************************************************/

ASN1C_UnitFireDamages::ASN1C_UnitFireDamages (
   ASN1MessageBuffer& msgBuf, ASN1T_UnitFireDamages& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_UnitFireDamages (ASN1CTXT* ctxt_p, ASN1T_UnitFireDamages* pvalue)
{
   int stat = ASN_OK;

   /* encode target */

   stat = asn1PE_Unit (ctxt_p, pvalue->target);
   if (stat != ASN_OK) return stat;

   /* encode humans */

   stat = asn1PE__SeqOfUnitHumanFireDamage (ctxt_p, &pvalue->humans);
   if (stat != ASN_OK) return stat;

   /* encode equipments */

   stat = asn1PE__SeqOfUnitEquipmentFireDamage (ctxt_p, &pvalue->equipments);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_UnitFireDamages::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_UnitFireDamages (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_UnitFireDamages (ASN1CTXT* ctxt_p, ASN1T_UnitFireDamages* pvalue)
{
   int stat = ASN_OK;

   /* decode target */

   stat = asn1PD_Unit (ctxt_p, &pvalue->target);
   if (stat != ASN_OK) return stat;

   /* decode humans */

   stat = asn1PD__SeqOfUnitHumanFireDamage (ctxt_p, &pvalue->humans);
   if (stat != ASN_OK) return stat;

   /* decode equipments */

   stat = asn1PD__SeqOfUnitEquipmentFireDamage (ctxt_p, &pvalue->equipments);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_UnitFireDamages::Decode ()
{
   return asn1PD_UnitFireDamages (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  PopulationFireDamages                                     */
/*                                                            */
/**************************************************************/

ASN1C_PopulationFireDamages::ASN1C_PopulationFireDamages (
   ASN1MessageBuffer& msgBuf, ASN1T_PopulationFireDamages& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_PopulationFireDamages (ASN1CTXT* ctxt_p, ASN1T_PopulationFireDamages* pvalue)
{
   int stat = ASN_OK;

   /* encode target */

   stat = asn1PE_Population (ctxt_p, pvalue->target);
   if (stat != ASN_OK) return stat;

   /* encode dead_nbr */

   stat = pe_UnconsInteger (ctxt_p, pvalue->dead_nbr);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_PopulationFireDamages::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_PopulationFireDamages (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_PopulationFireDamages (ASN1CTXT* ctxt_p, ASN1T_PopulationFireDamages* pvalue)
{
   int stat = ASN_OK;

   /* decode target */

   stat = asn1PD_Population (ctxt_p, &pvalue->target);
   if (stat != ASN_OK) return stat;

   /* decode dead_nbr */

   stat = pd_UnconsInteger (ctxt_p, &pvalue->dead_nbr);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_PopulationFireDamages::Decode ()
{
   return asn1PD_PopulationFireDamages (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  UnitsFireDamages                                          */
/*                                                            */
/**************************************************************/

ASN1C_UnitsFireDamages::ASN1C_UnitsFireDamages (
   ASN1MessageBuffer& msgBuf, ASN1T_UnitsFireDamages& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_UnitsFireDamages (ASN1CTXT* ctxt_p, ASN1T_UnitsFireDamages* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* encode length determinant */

   stat = pe_Length (ctxt_p, pvalue->n);
   if (stat < 0) return stat;

   /* encode elements */

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      stat = asn1PE_UnitFireDamages (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return stat;
   }
   return (stat);
}

int ASN1C_UnitsFireDamages::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_UnitsFireDamages (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_UnitsFireDamages (ASN1CTXT* ctxt_p, ASN1T_UnitsFireDamages* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = pd_Length (ctxt_p, &pvalue->n);
   if (stat != ASN_OK) return stat;

   /* decode elements */

   ALLOC_ASN1ARRAY (ctxt_p, pvalue, ASN1T_UnitFireDamages);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      stat = asn1PD_UnitFireDamages (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return stat;
   }


   return (stat);
}

int ASN1C_UnitsFireDamages::Decode ()
{
   return asn1PD_UnitsFireDamages (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  PopulationsFireDamages                                    */
/*                                                            */
/**************************************************************/

ASN1C_PopulationsFireDamages::ASN1C_PopulationsFireDamages (
   ASN1MessageBuffer& msgBuf, ASN1T_PopulationsFireDamages& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_PopulationsFireDamages (ASN1CTXT* ctxt_p, ASN1T_PopulationsFireDamages* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* encode length determinant */

   stat = pe_Length (ctxt_p, pvalue->n);
   if (stat < 0) return stat;

   /* encode elements */

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      stat = asn1PE_PopulationFireDamages (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return stat;
   }
   return (stat);
}

int ASN1C_PopulationsFireDamages::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_PopulationsFireDamages (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_PopulationsFireDamages (ASN1CTXT* ctxt_p, ASN1T_PopulationsFireDamages* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = pd_Length (ctxt_p, &pvalue->n);
   if (stat != ASN_OK) return stat;

   /* decode elements */

   ALLOC_ASN1ARRAY (ctxt_p, pvalue, ASN1T_PopulationFireDamages);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      stat = asn1PD_PopulationFireDamages (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return stat;
   }


   return (stat);
}

int ASN1C_PopulationsFireDamages::Decode ()
{
   return asn1PD_PopulationsFireDamages (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgStartUnitFire_target                                   */
/*                                                            */
/**************************************************************/

ASN1C_MsgStartUnitFire_target::ASN1C_MsgStartUnitFire_target (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgStartUnitFire_target& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgStartUnitFire_target (ASN1CTXT* ctxt_p, ASN1T_MsgStartUnitFire_target* pvalue)
{
   int stat = ASN_OK;

   /* Encode choice index value */

   stat = pe_ConsUnsigned (ctxt_p, pvalue->t - 1, 0, 2);
   if (stat != ASN_OK) return stat;

   /* Encode root element data value */

   switch (pvalue->t)
   {
      /* unit */
      case 1:
         stat = asn1PE_Unit (ctxt_p, pvalue->u.unit);
         if (stat != ASN_OK) return stat;
         break;

      /* population */
      case 2:
         stat = asn1PE_Population (ctxt_p, pvalue->u.population);
         if (stat != ASN_OK) return stat;
         break;

      /* position */
      case 3:
         stat = asn1PE_CoordLatLong (ctxt_p, pvalue->u.position);
         if (stat != ASN_OK) return stat;
         break;

      default:
         return ASN_E_INVOPT;
   }
   return (stat);
}

int ASN1C_MsgStartUnitFire_target::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgStartUnitFire_target (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgStartUnitFire_target (ASN1CTXT* ctxt_p, ASN1T_MsgStartUnitFire_target* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   stat = pd_ConsUnsigned (ctxt_p, &ui, 0, 2);
   if (stat != ASN_OK) return stat;
   else pvalue->t = ui + 1;

   switch (ui) {
      /* unit */
      case 0:
         stat = asn1PD_Unit (ctxt_p, &pvalue->u.unit);
         if (stat != ASN_OK) return stat;

         break;

      /* population */
      case 1:
         stat = asn1PD_Population (ctxt_p, &pvalue->u.population);
         if (stat != ASN_OK) return stat;

         break;

      /* position */
      case 2:
         pvalue->u.position = ALLOC_ASN1ELEM (ctxt_p, ASN1T_CoordLatLong);

         stat = asn1PD_CoordLatLong (ctxt_p, pvalue->u.position);
         if (stat != ASN_OK) return stat;

         break;

      default:
         return ASN_E_INVOPT;
   }

   return (stat);
}

int ASN1C_MsgStartUnitFire_target::Decode ()
{
   return asn1PD_MsgStartUnitFire_target (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgStartUnitFire_type                                     */
/*                                                            */
/**************************************************************/

ASN1C_MsgStartUnitFire_type::ASN1C_MsgStartUnitFire_type (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgStartUnitFire_type& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgStartUnitFire_type (ASN1CTXT* ctxt_p, ASN1T_MsgStartUnitFire_type value)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   switch (value) {
      case MsgStartUnitFire_type::direct: ui = 0; break;
      case MsgStartUnitFire_type::indirect: ui = 1; break;
      default: return ASN_E_INVENUM;
   }

   stat = pe_ConsUnsigned (ctxt_p, ui, 0, 1);
   if (stat != ASN_OK) return stat;
   return (stat);
}

int ASN1C_MsgStartUnitFire_type::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgStartUnitFire_type (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_MsgStartUnitFire_type (ASN1CTXT* ctxt_p, ASN1T_MsgStartUnitFire_type* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   stat = pd_ConsUnsigned (ctxt_p, &ui, 0, 1);
   if (stat != ASN_OK) return stat;

   switch (ui) {
      case 0: *pvalue = MsgStartUnitFire_type::direct; break;
      case 1: *pvalue = MsgStartUnitFire_type::indirect; break;
      default: return ASN_E_INVENUM;
   }

   return (stat);
}

int ASN1C_MsgStartUnitFire_type::Decode ()
{
   return asn1PD_MsgStartUnitFire_type (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgStartUnitFire                                          */
/*                                                            */
/**************************************************************/

ASN1C_MsgStartUnitFire::ASN1C_MsgStartUnitFire (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgStartUnitFire& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgStartUnitFire (ASN1CTXT* ctxt_p, ASN1T_MsgStartUnitFire* pvalue)
{
   int stat = ASN_OK;

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.ammunitionPresent);

   /* encode fire_oid */

   stat = asn1PE_UnitFire (ctxt_p, pvalue->fire_oid);
   if (stat != ASN_OK) return stat;

   /* encode firer_oid */

   stat = asn1PE_Unit (ctxt_p, pvalue->firer_oid);
   if (stat != ASN_OK) return stat;

   /* encode target */

   stat = asn1PE_MsgStartUnitFire_target (ctxt_p, &pvalue->target);
   if (stat != ASN_OK) return stat;

   /* encode type */

   stat = asn1PE_MsgStartUnitFire_type (ctxt_p, pvalue->type);
   if (stat != ASN_OK) return stat;

   /* encode ammunition */

   if (pvalue->m.ammunitionPresent) {
      stat = asn1PE_DotationType (ctxt_p, pvalue->ammunition);
      if (stat != ASN_OK) return stat;
   }

   return (stat);
}

int ASN1C_MsgStartUnitFire::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgStartUnitFire (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgStartUnitFire (ASN1CTXT* ctxt_p, ASN1T_MsgStartUnitFire* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL optbit;

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   pd_bit (ctxt_p, &optbit);
   pvalue->m.ammunitionPresent = optbit;

   /* decode fire_oid */

   stat = asn1PD_UnitFire (ctxt_p, &pvalue->fire_oid);
   if (stat != ASN_OK) return stat;

   /* decode firer_oid */

   stat = asn1PD_Unit (ctxt_p, &pvalue->firer_oid);
   if (stat != ASN_OK) return stat;

   /* decode target */

   stat = asn1PD_MsgStartUnitFire_target (ctxt_p, &pvalue->target);
   if (stat != ASN_OK) return stat;

   /* decode type */

   stat = asn1PD_MsgStartUnitFire_type (ctxt_p, &pvalue->type);
   if (stat != ASN_OK) return stat;

   /* decode ammunition */

   if (pvalue->m.ammunitionPresent) {
      stat = asn1PD_DotationType (ctxt_p, &pvalue->ammunition);
      if (stat != ASN_OK) return stat;
   }


   return (stat);
}

int ASN1C_MsgStartUnitFire::Decode ()
{
   return asn1PD_MsgStartUnitFire (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgStopUnitFire                                           */
/*                                                            */
/**************************************************************/

ASN1C_MsgStopUnitFire::ASN1C_MsgStopUnitFire (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgStopUnitFire& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgStopUnitFire (ASN1CTXT* ctxt_p, ASN1T_MsgStopUnitFire* pvalue)
{
   int stat = ASN_OK;

   /* encode fire_oid */

   stat = asn1PE_UnitFire (ctxt_p, pvalue->fire_oid);
   if (stat != ASN_OK) return stat;

   /* encode units_damages */

   stat = asn1PE_UnitsFireDamages (ctxt_p, &pvalue->units_damages);
   if (stat != ASN_OK) return stat;

   /* encode populations_damages */

   stat = asn1PE_PopulationsFireDamages (ctxt_p, &pvalue->populations_damages);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgStopUnitFire::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgStopUnitFire (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgStopUnitFire (ASN1CTXT* ctxt_p, ASN1T_MsgStopUnitFire* pvalue)
{
   int stat = ASN_OK;

   /* decode fire_oid */

   stat = asn1PD_UnitFire (ctxt_p, &pvalue->fire_oid);
   if (stat != ASN_OK) return stat;

   /* decode units_damages */

   stat = asn1PD_UnitsFireDamages (ctxt_p, &pvalue->units_damages);
   if (stat != ASN_OK) return stat;

   /* decode populations_damages */

   stat = asn1PD_PopulationsFireDamages (ctxt_p, &pvalue->populations_damages);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgStopUnitFire::Decode ()
{
   return asn1PD_MsgStopUnitFire (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgStartPopulationFire                                    */
/*                                                            */
/**************************************************************/

ASN1C_MsgStartPopulationFire::ASN1C_MsgStartPopulationFire (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgStartPopulationFire& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgStartPopulationFire (ASN1CTXT* ctxt_p, ASN1T_MsgStartPopulationFire* pvalue)
{
   int stat = ASN_OK;

   /* encode fire_oid */

   stat = asn1PE_PopulationFire (ctxt_p, pvalue->fire_oid);
   if (stat != ASN_OK) return stat;

   /* encode firer_oid */

   stat = asn1PE_Population (ctxt_p, pvalue->firer_oid);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgStartPopulationFire::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgStartPopulationFire (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgStartPopulationFire (ASN1CTXT* ctxt_p, ASN1T_MsgStartPopulationFire* pvalue)
{
   int stat = ASN_OK;

   /* decode fire_oid */

   stat = asn1PD_PopulationFire (ctxt_p, &pvalue->fire_oid);
   if (stat != ASN_OK) return stat;

   /* decode firer_oid */

   stat = asn1PD_Population (ctxt_p, &pvalue->firer_oid);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgStartPopulationFire::Decode ()
{
   return asn1PD_MsgStartPopulationFire (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgStopPopulationFire                                     */
/*                                                            */
/**************************************************************/

ASN1C_MsgStopPopulationFire::ASN1C_MsgStopPopulationFire (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgStopPopulationFire& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgStopPopulationFire (ASN1CTXT* ctxt_p, ASN1T_MsgStopPopulationFire* pvalue)
{
   int stat = ASN_OK;

   /* encode fire_oid */

   stat = asn1PE_PopulationFire (ctxt_p, pvalue->fire_oid);
   if (stat != ASN_OK) return stat;

   /* encode units_damages */

   stat = asn1PE_UnitsFireDamages (ctxt_p, &pvalue->units_damages);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgStopPopulationFire::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgStopPopulationFire (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgStopPopulationFire (ASN1CTXT* ctxt_p, ASN1T_MsgStopPopulationFire* pvalue)
{
   int stat = ASN_OK;

   /* decode fire_oid */

   stat = asn1PD_PopulationFire (ctxt_p, &pvalue->fire_oid);
   if (stat != ASN_OK) return stat;

   /* decode units_damages */

   stat = asn1PD_UnitsFireDamages (ctxt_p, &pvalue->units_damages);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgStopPopulationFire::Decode ()
{
   return asn1PD_MsgStopPopulationFire (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgExplosion                                              */
/*                                                            */
/**************************************************************/

ASN1C_MsgExplosion::ASN1C_MsgExplosion (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgExplosion& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgExplosion (ASN1CTXT* ctxt_p, ASN1T_MsgExplosion* pvalue)
{
   int stat = ASN_OK;

   /* encode object_oid */

   stat = asn1PE_OID (ctxt_p, pvalue->object_oid);
   if (stat != ASN_OK) return stat;

   /* encode units_damages */

   stat = asn1PE_UnitsFireDamages (ctxt_p, &pvalue->units_damages);
   if (stat != ASN_OK) return stat;

   /* encode populations_damages */

   stat = asn1PE_PopulationsFireDamages (ctxt_p, &pvalue->populations_damages);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgExplosion::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgExplosion (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgExplosion (ASN1CTXT* ctxt_p, ASN1T_MsgExplosion* pvalue)
{
   int stat = ASN_OK;

   /* decode object_oid */

   stat = asn1PD_OID (ctxt_p, &pvalue->object_oid);
   if (stat != ASN_OK) return stat;

   /* decode units_damages */

   stat = asn1PD_UnitsFireDamages (ctxt_p, &pvalue->units_damages);
   if (stat != ASN_OK) return stat;

   /* decode populations_damages */

   stat = asn1PD_PopulationsFireDamages (ctxt_p, &pvalue->populations_damages);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgExplosion::Decode ()
{
   return asn1PD_MsgExplosion (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  EnumFireEffectType                                        */
/*                                                            */
/**************************************************************/

ASN1C_EnumFireEffectType::ASN1C_EnumFireEffectType (
   ASN1MessageBuffer& msgBuf, ASN1T_EnumFireEffectType& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_EnumFireEffectType (ASN1CTXT* ctxt_p, ASN1T_EnumFireEffectType value)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   switch (value) {
      case EnumFireEffectType::fumigene: ui = 0; break;
      case EnumFireEffectType::eclairant: ui = 1; break;
      default: return ASN_E_INVENUM;
   }

   stat = pe_ConsUnsigned (ctxt_p, ui, 0, 1);
   if (stat != ASN_OK) return stat;
   return (stat);
}

int ASN1C_EnumFireEffectType::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_EnumFireEffectType (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_EnumFireEffectType (ASN1CTXT* ctxt_p, ASN1T_EnumFireEffectType* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   stat = pd_ConsUnsigned (ctxt_p, &ui, 0, 1);
   if (stat != ASN_OK) return stat;

   switch (ui) {
      case 0: *pvalue = EnumFireEffectType::fumigene; break;
      case 1: *pvalue = EnumFireEffectType::eclairant; break;
      default: return ASN_E_INVENUM;
   }

   return (stat);
}

int ASN1C_EnumFireEffectType::Decode ()
{
   return asn1PD_EnumFireEffectType (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgStartFireEffect                                        */
/*                                                            */
/**************************************************************/

ASN1C_MsgStartFireEffect::ASN1C_MsgStartFireEffect (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgStartFireEffect& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgStartFireEffect (ASN1CTXT* ctxt_p, ASN1T_MsgStartFireEffect* pvalue)
{
   int stat = ASN_OK;

   /* encode effect_oid */

   stat = asn1PE_OID (ctxt_p, pvalue->effect_oid);
   if (stat != ASN_OK) return stat;

   /* encode location */

   stat = asn1PE_Location (ctxt_p, &pvalue->location);
   if (stat != ASN_OK) return stat;

   /* encode type */

   stat = asn1PE_EnumFireEffectType (ctxt_p, pvalue->type);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgStartFireEffect::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgStartFireEffect (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgStartFireEffect (ASN1CTXT* ctxt_p, ASN1T_MsgStartFireEffect* pvalue)
{
   int stat = ASN_OK;

   /* decode effect_oid */

   stat = asn1PD_OID (ctxt_p, &pvalue->effect_oid);
   if (stat != ASN_OK) return stat;

   /* decode location */

   stat = asn1PD_Location (ctxt_p, &pvalue->location);
   if (stat != ASN_OK) return stat;

   /* decode type */

   stat = asn1PD_EnumFireEffectType (ctxt_p, &pvalue->type);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgStartFireEffect::Decode ()
{
   return asn1PD_MsgStartFireEffect (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgStopFireEffect                                         */
/*                                                            */
/**************************************************************/

ASN1C_MsgStopFireEffect::ASN1C_MsgStopFireEffect (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgStopFireEffect& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgStopFireEffect (ASN1CTXT* ctxt_p, ASN1T_MsgStopFireEffect value)
{
   int stat = ASN_OK;

   stat = asn1PE_OID (ctxt_p, value);
   if (stat != ASN_OK) return stat;
   return (stat);
}

int ASN1C_MsgStopFireEffect::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgStopFireEffect (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_MsgStopFireEffect (ASN1CTXT* ctxt_p, ASN1T_MsgStopFireEffect* pvalue)
{
   int stat = ASN_OK;

   stat = asn1PD_OID (ctxt_p, pvalue);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgStopFireEffect::Decode ()
{
   return asn1PD_MsgStopFireEffect (mpContext->GetPtr(), &msgData);
}


/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 08-Dec-2009.
 */
#include "Urban.h"

/**************************************************************/
/*                                                            */
/*  UrbanIncludes                                             */
/*                                                            */
/**************************************************************/

ASN1C_UrbanIncludes::ASN1C_UrbanIncludes (
   ASN1MessageBuffer& msgBuf, ASN1T_UrbanIncludes& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_UrbanIncludes (ASN1CTXT* ctxt_p, ASN1T_UrbanIncludes* pvalue)
{
   int stat = ASN_OK;

   /* encode baseExports */

   stat = asn1PE_BaseExports (ctxt_p);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_UrbanIncludes::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_UrbanIncludes (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_UrbanIncludes (ASN1CTXT* ctxt_p, ASN1T_UrbanIncludes* pvalue)
{
   int stat = ASN_OK;

   /* decode baseExports */

   stat = asn1PD_BaseExports (ctxt_p);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_UrbanIncludes::Decode ()
{
   return asn1PD_UrbanIncludes (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  UrbanExports                                              */
/*                                                            */
/**************************************************************/

ASN1C_UrbanExports::ASN1C_UrbanExports (
   ASN1MessageBuffer& msgBuf) :
   ASN1CType(msgBuf)
{}

EXTERN int asn1PE_UrbanExports (ASN1CTXT* ctxt_p)
{
   int stat = ASN_OK;

   /* NULL */
   return (stat);
}

int ASN1C_UrbanExports::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_UrbanExports (mpContext->GetPtr());
   return stat;
}

EXTERN int asn1PD_UrbanExports (ASN1CTXT* ctxt_p)
{
   int stat = ASN_OK;

   /* NULL */

   return (stat);
}

int ASN1C_UrbanExports::Decode ()
{
   return asn1PD_UrbanExports (mpContext->GetPtr());
}

/**************************************************************/
/*                                                            */
/*  EnumUrbanErrorCode                                        */
/*                                                            */
/**************************************************************/

ASN1C_EnumUrbanErrorCode::ASN1C_EnumUrbanErrorCode (
   ASN1MessageBuffer& msgBuf, ASN1T_EnumUrbanErrorCode& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_EnumUrbanErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumUrbanErrorCode value)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   switch (value) {
      case EnumUrbanErrorCode::no_error: ui = 0; break;
      case EnumUrbanErrorCode::error_invalid_urban: ui = 1; break;
      case EnumUrbanErrorCode::error_invalid_id: ui = 2; break;
      case EnumUrbanErrorCode::error_invalid_camp: ui = 3; break;
      case EnumUrbanErrorCode::error_invalid_localisation: ui = 4; break;
      case EnumUrbanErrorCode::error_missing_specific_attributes: ui = 5; break;
      case EnumUrbanErrorCode::error_invalid_specific_attributes: ui = 6; break;
      default: return ASN_E_INVENUM;
   }

   stat = pe_ConsUnsigned (ctxt_p, ui, 0, 6);
   if (stat != ASN_OK) return stat;
   return (stat);
}

int ASN1C_EnumUrbanErrorCode::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_EnumUrbanErrorCode (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_EnumUrbanErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumUrbanErrorCode* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   stat = pd_ConsUnsigned (ctxt_p, &ui, 0, 6);
   if (stat != ASN_OK) return stat;

   switch (ui) {
      case 0: *pvalue = EnumUrbanErrorCode::no_error; break;
      case 1: *pvalue = EnumUrbanErrorCode::error_invalid_urban; break;
      case 2: *pvalue = EnumUrbanErrorCode::error_invalid_id; break;
      case 3: *pvalue = EnumUrbanErrorCode::error_invalid_camp; break;
      case 4: *pvalue = EnumUrbanErrorCode::error_invalid_localisation; break;
      case 5: *pvalue = EnumUrbanErrorCode::error_missing_specific_attributes; break;
      case 6: *pvalue = EnumUrbanErrorCode::error_invalid_specific_attributes; break;
      default: return ASN_E_INVENUM;
   }

   return (stat);
}

int ASN1C_EnumUrbanErrorCode::Decode ()
{
   return asn1PD_EnumUrbanErrorCode (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  ObjectAttributeVegetation                                 */
/*                                                            */
/**************************************************************/

ASN1C_ObjectAttributeVegetation::ASN1C_ObjectAttributeVegetation (
   ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributeVegetation& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_ObjectAttributeVegetation (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeVegetation* pvalue)
{
   int stat = ASN_OK;

   /* encode type */

   stat = pe_VisibleString (ctxt_p, pvalue->type, 0);
   if (stat != ASN_OK) return stat;

   /* encode height */

   stat = pe_UnconsInteger (ctxt_p, pvalue->height);
   if (stat != ASN_OK) return stat;

   /* encode density */

   stat = pe_Real (ctxt_p, pvalue->density);if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_ObjectAttributeVegetation::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_ObjectAttributeVegetation (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_ObjectAttributeVegetation (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeVegetation* pvalue)
{
   int stat = ASN_OK;

   /* decode type */

   stat = pd_VisibleString (ctxt_p, &pvalue->type, 0);
   if (stat != ASN_OK) return stat;

   /* decode height */

   stat = pd_UnconsInteger (ctxt_p, &pvalue->height);
   if (stat != ASN_OK) return stat;

   /* decode density */

   stat = pd_Real (ctxt_p, &pvalue->density);if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_ObjectAttributeVegetation::Decode ()
{
   return asn1PD_ObjectAttributeVegetation (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  ObjectAttributeSoil                                       */
/*                                                            */
/**************************************************************/

ASN1C_ObjectAttributeSoil::ASN1C_ObjectAttributeSoil (
   ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributeSoil& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_ObjectAttributeSoil (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeSoil* pvalue)
{
   int stat = ASN_OK;

   /* encode occupation */

   stat = pe_Real (ctxt_p, pvalue->occupation);if (stat != ASN_OK) return stat;

   /* encode trafficability */

   stat = pe_Real (ctxt_p, pvalue->trafficability);if (stat != ASN_OK) return stat;

   /* encode multiple */

   stat = pe_bit (ctxt_p, (ASN1BOOL)pvalue->multiple);
   if (stat != ASN_OK) return stat;

   /* encode compoundClearing */

   stat = pe_VisibleString (ctxt_p, pvalue->compoundClearing, 0);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_ObjectAttributeSoil::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_ObjectAttributeSoil (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_ObjectAttributeSoil (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeSoil* pvalue)
{
   int stat = ASN_OK;

   /* decode occupation */

   stat = pd_Real (ctxt_p, &pvalue->occupation);if (stat != ASN_OK) return stat;

   /* decode trafficability */

   stat = pd_Real (ctxt_p, &pvalue->trafficability);if (stat != ASN_OK) return stat;

   /* decode multiple */

   stat = pd_bit (ctxt_p, &pvalue->multiple);
   if (stat != ASN_OK) return stat;

   /* decode compoundClearing */

   stat = pd_VisibleString (ctxt_p, &pvalue->compoundClearing, 0);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_ObjectAttributeSoil::Decode ()
{
   return asn1PD_ObjectAttributeSoil (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  ObjectAttributeArchitecture                               */
/*                                                            */
/**************************************************************/

ASN1C_ObjectAttributeArchitecture::ASN1C_ObjectAttributeArchitecture (
   ASN1MessageBuffer& msgBuf, ASN1T_ObjectAttributeArchitecture& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_ObjectAttributeArchitecture (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeArchitecture* pvalue)
{
   int stat = ASN_OK;

   /* encode height */

   stat = pe_Real (ctxt_p, pvalue->height);if (stat != ASN_OK) return stat;

   /* encode floorNumber */

   stat = pe_UnconsInteger (ctxt_p, pvalue->floorNumber);
   if (stat != ASN_OK) return stat;

   /* encode basementLevelNumber */

   stat = pe_UnconsInteger (ctxt_p, pvalue->basementLevelNumber);
   if (stat != ASN_OK) return stat;

   /* encode roofShape */

   stat = pe_VisibleString (ctxt_p, pvalue->roofShape, 0);
   if (stat != ASN_OK) return stat;

   /* encode material */

   stat = pe_VisibleString (ctxt_p, pvalue->material, 0);
   if (stat != ASN_OK) return stat;

   /* encode innerCluttering */

   stat = pe_Real (ctxt_p, pvalue->innerCluttering);if (stat != ASN_OK) return stat;

   /* encode facadeOpacity */

   stat = pe_Real (ctxt_p, pvalue->facadeOpacity);if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_ObjectAttributeArchitecture::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_ObjectAttributeArchitecture (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_ObjectAttributeArchitecture (ASN1CTXT* ctxt_p, ASN1T_ObjectAttributeArchitecture* pvalue)
{
   int stat = ASN_OK;

   /* decode height */

   stat = pd_Real (ctxt_p, &pvalue->height);if (stat != ASN_OK) return stat;

   /* decode floorNumber */

   stat = pd_UnconsInteger (ctxt_p, &pvalue->floorNumber);
   if (stat != ASN_OK) return stat;

   /* decode basementLevelNumber */

   stat = pd_UnconsInteger (ctxt_p, &pvalue->basementLevelNumber);
   if (stat != ASN_OK) return stat;

   /* decode roofShape */

   stat = pd_VisibleString (ctxt_p, &pvalue->roofShape, 0);
   if (stat != ASN_OK) return stat;

   /* decode material */

   stat = pd_VisibleString (ctxt_p, &pvalue->material, 0);
   if (stat != ASN_OK) return stat;

   /* decode innerCluttering */

   stat = pd_Real (ctxt_p, &pvalue->innerCluttering);if (stat != ASN_OK) return stat;

   /* decode facadeOpacity */

   stat = pd_Real (ctxt_p, &pvalue->facadeOpacity);if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_ObjectAttributeArchitecture::Decode ()
{
   return asn1PD_ObjectAttributeArchitecture (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  UrbanAttributes                                           */
/*                                                            */
/**************************************************************/

ASN1C_UrbanAttributes::ASN1C_UrbanAttributes (
   ASN1MessageBuffer& msgBuf, ASN1T_UrbanAttributes& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_UrbanAttributes (ASN1CTXT* ctxt_p, ASN1T_UrbanAttributes* pvalue)
{
   int stat = ASN_OK;

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.vegetationPresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.soilPresent);

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.architecturePresent);

   /* encode vegetation */

   if (pvalue->m.vegetationPresent) {
      stat = asn1PE_ObjectAttributeVegetation (ctxt_p, &pvalue->vegetation);
      if (stat != ASN_OK) return stat;
   }

   /* encode soil */

   if (pvalue->m.soilPresent) {
      stat = asn1PE_ObjectAttributeSoil (ctxt_p, &pvalue->soil);
      if (stat != ASN_OK) return stat;
   }

   /* encode architecture */

   if (pvalue->m.architecturePresent) {
      stat = asn1PE_ObjectAttributeArchitecture (ctxt_p, &pvalue->architecture);
      if (stat != ASN_OK) return stat;
   }

   return (stat);
}

int ASN1C_UrbanAttributes::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_UrbanAttributes (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_UrbanAttributes (ASN1CTXT* ctxt_p, ASN1T_UrbanAttributes* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL optbit;

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   pd_bit (ctxt_p, &optbit);
   pvalue->m.vegetationPresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.soilPresent = optbit;

   pd_bit (ctxt_p, &optbit);
   pvalue->m.architecturePresent = optbit;

   /* decode vegetation */

   if (pvalue->m.vegetationPresent) {
      stat = asn1PD_ObjectAttributeVegetation (ctxt_p, &pvalue->vegetation);
      if (stat != ASN_OK) return stat;
   }

   /* decode soil */

   if (pvalue->m.soilPresent) {
      stat = asn1PD_ObjectAttributeSoil (ctxt_p, &pvalue->soil);
      if (stat != ASN_OK) return stat;
   }

   /* decode architecture */

   if (pvalue->m.architecturePresent) {
      stat = asn1PD_ObjectAttributeArchitecture (ctxt_p, &pvalue->architecture);
      if (stat != ASN_OK) return stat;
   }


   return (stat);
}

int ASN1C_UrbanAttributes::Decode ()
{
   return asn1PD_UrbanAttributes (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgUrbanCreation                                          */
/*                                                            */
/**************************************************************/

ASN1C_MsgUrbanCreation::ASN1C_MsgUrbanCreation (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgUrbanCreation& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgUrbanCreation (ASN1CTXT* ctxt_p, ASN1T_MsgUrbanCreation* pvalue)
{
   int stat = ASN_OK;

   /* encode oid */

   stat = asn1PE_OID (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* encode name */

   stat = pe_VisibleString (ctxt_p, pvalue->name, 0);
   if (stat != ASN_OK) return stat;

   /* encode location */

   stat = asn1PE_Location (ctxt_p, &pvalue->location);
   if (stat != ASN_OK) return stat;

   /* encode attributes */

   stat = asn1PE_UrbanAttributes (ctxt_p, &pvalue->attributes);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgUrbanCreation::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgUrbanCreation (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgUrbanCreation (ASN1CTXT* ctxt_p, ASN1T_MsgUrbanCreation* pvalue)
{
   int stat = ASN_OK;

   /* decode oid */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* decode name */

   stat = pd_VisibleString (ctxt_p, &pvalue->name, 0);
   if (stat != ASN_OK) return stat;

   /* decode location */

   stat = asn1PD_Location (ctxt_p, &pvalue->location);
   if (stat != ASN_OK) return stat;

   /* decode attributes */

   stat = asn1PD_UrbanAttributes (ctxt_p, &pvalue->attributes);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgUrbanCreation::Decode ()
{
   return asn1PD_MsgUrbanCreation (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgUrbanUpdate                                            */
/*                                                            */
/**************************************************************/

ASN1C_MsgUrbanUpdate::ASN1C_MsgUrbanUpdate (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgUrbanUpdate& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgUrbanUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgUrbanUpdate* pvalue)
{
   int stat = ASN_OK;

   pe_bit (ctxt_p, (ASN1BOOL)pvalue->m.locationPresent);

   /* encode oid */

   stat = asn1PE_OID (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* encode location */

   if (pvalue->m.locationPresent) {
      stat = asn1PE_Location (ctxt_p, &pvalue->location);
      if (stat != ASN_OK) return stat;
   }

   /* encode attributes */

   stat = asn1PE_UrbanAttributes (ctxt_p, &pvalue->attributes);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgUrbanUpdate::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgUrbanUpdate (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgUrbanUpdate (ASN1CTXT* ctxt_p, ASN1T_MsgUrbanUpdate* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL optbit;

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   pd_bit (ctxt_p, &optbit);
   pvalue->m.locationPresent = optbit;

   /* decode oid */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return stat;

   /* decode location */

   if (pvalue->m.locationPresent) {
      stat = asn1PD_Location (ctxt_p, &pvalue->location);
      if (stat != ASN_OK) return stat;
   }

   /* decode attributes */

   stat = asn1PD_UrbanAttributes (ctxt_p, &pvalue->attributes);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgUrbanUpdate::Decode ()
{
   return asn1PD_MsgUrbanUpdate (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgUrbanDestruction                                       */
/*                                                            */
/**************************************************************/

ASN1C_MsgUrbanDestruction::ASN1C_MsgUrbanDestruction (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgUrbanDestruction& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgUrbanDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgUrbanDestruction value)
{
   int stat = ASN_OK;

   stat = asn1PE_OID (ctxt_p, value);
   if (stat != ASN_OK) return stat;
   return (stat);
}

int ASN1C_MsgUrbanDestruction::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgUrbanDestruction (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_MsgUrbanDestruction (ASN1CTXT* ctxt_p, ASN1T_MsgUrbanDestruction* pvalue)
{
   int stat = ASN_OK;

   stat = asn1PD_OID (ctxt_p, pvalue);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgUrbanDestruction::Decode ()
{
   return asn1PD_MsgUrbanDestruction (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MagicActionUpdateUrban                                    */
/*                                                            */
/**************************************************************/

ASN1C_MagicActionUpdateUrban::ASN1C_MagicActionUpdateUrban (
   ASN1MessageBuffer& msgBuf, ASN1T_MagicActionUpdateUrban& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MagicActionUpdateUrban (ASN1CTXT* ctxt_p, ASN1T_MagicActionUpdateUrban* pvalue)
{
   int stat = ASN_OK;

   /* encode oid */

   stat = asn1PE_OID (ctxt_p, pvalue->oid);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MagicActionUpdateUrban::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MagicActionUpdateUrban (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MagicActionUpdateUrban (ASN1CTXT* ctxt_p, ASN1T_MagicActionUpdateUrban* pvalue)
{
   int stat = ASN_OK;

   /* decode oid */

   stat = asn1PD_OID (ctxt_p, &pvalue->oid);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MagicActionUpdateUrban::Decode ()
{
   return asn1PD_MagicActionUpdateUrban (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MagicActionCreateUrban                                    */
/*                                                            */
/**************************************************************/

ASN1C_MagicActionCreateUrban::ASN1C_MagicActionCreateUrban (
   ASN1MessageBuffer& msgBuf, ASN1T_MagicActionCreateUrban& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MagicActionCreateUrban (ASN1CTXT* ctxt_p, ASN1T_MagicActionCreateUrban* pvalue)
{
   int stat = ASN_OK;

   /* encode location */

   stat = asn1PE_Location (ctxt_p, &pvalue->location);
   if (stat != ASN_OK) return stat;

   /* encode name */

   stat = pe_VisibleString (ctxt_p, pvalue->name, 0);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MagicActionCreateUrban::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MagicActionCreateUrban (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MagicActionCreateUrban (ASN1CTXT* ctxt_p, ASN1T_MagicActionCreateUrban* pvalue)
{
   int stat = ASN_OK;

   /* decode location */

   stat = asn1PD_Location (ctxt_p, &pvalue->location);
   if (stat != ASN_OK) return stat;

   /* decode name */

   stat = pd_VisibleString (ctxt_p, &pvalue->name, 0);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MagicActionCreateUrban::Decode ()
{
   return asn1PD_MagicActionCreateUrban (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MagicActionDestroyUrban                                   */
/*                                                            */
/**************************************************************/

ASN1C_MagicActionDestroyUrban::ASN1C_MagicActionDestroyUrban (
   ASN1MessageBuffer& msgBuf, ASN1T_MagicActionDestroyUrban& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MagicActionDestroyUrban (ASN1CTXT* ctxt_p, ASN1T_MagicActionDestroyUrban value)
{
   int stat = ASN_OK;

   stat = asn1PE_OID (ctxt_p, value);
   if (stat != ASN_OK) return stat;
   return (stat);
}

int ASN1C_MagicActionDestroyUrban::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MagicActionDestroyUrban (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_MagicActionDestroyUrban (ASN1CTXT* ctxt_p, ASN1T_MagicActionDestroyUrban* pvalue)
{
   int stat = ASN_OK;

   stat = asn1PD_OID (ctxt_p, pvalue);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MagicActionDestroyUrban::Decode ()
{
   return asn1PD_MagicActionDestroyUrban (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgUrbanMagicAction_action                                */
/*                                                            */
/**************************************************************/

ASN1C_MsgUrbanMagicAction_action::ASN1C_MsgUrbanMagicAction_action (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgUrbanMagicAction_action& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgUrbanMagicAction_action (ASN1CTXT* ctxt_p, ASN1T_MsgUrbanMagicAction_action* pvalue)
{
   int stat = ASN_OK;

   /* Encode choice index value */

   stat = pe_ConsUnsigned (ctxt_p, pvalue->t - 1, 0, 2);
   if (stat != ASN_OK) return stat;

   /* Encode root element data value */

   switch (pvalue->t)
   {
      /* create_urban */
      case 1:
         stat = asn1PE_MagicActionCreateUrban (ctxt_p, pvalue->u.create_urban);
         if (stat != ASN_OK) return stat;
         break;

      /* update_urban */
      case 2:
         stat = asn1PE_MagicActionUpdateUrban (ctxt_p, pvalue->u.update_urban);
         if (stat != ASN_OK) return stat;
         break;

      /* destroy_urban */
      case 3:
         stat = asn1PE_MagicActionDestroyUrban (ctxt_p, pvalue->u.destroy_urban);
         if (stat != ASN_OK) return stat;
         break;

      default:
         return ASN_E_INVOPT;
   }
   return (stat);
}

int ASN1C_MsgUrbanMagicAction_action::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgUrbanMagicAction_action (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgUrbanMagicAction_action (ASN1CTXT* ctxt_p, ASN1T_MsgUrbanMagicAction_action* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   stat = pd_ConsUnsigned (ctxt_p, &ui, 0, 2);
   if (stat != ASN_OK) return stat;
   else pvalue->t = ui + 1;

   switch (ui) {
      /* create_urban */
      case 0:
         pvalue->u.create_urban = ALLOC_ASN1ELEM (ctxt_p, ASN1T_MagicActionCreateUrban);

         stat = asn1PD_MagicActionCreateUrban (ctxt_p, pvalue->u.create_urban);
         if (stat != ASN_OK) return stat;

         break;

      /* update_urban */
      case 1:
         pvalue->u.update_urban = ALLOC_ASN1ELEM (ctxt_p, ASN1T_MagicActionUpdateUrban);

         stat = asn1PD_MagicActionUpdateUrban (ctxt_p, pvalue->u.update_urban);
         if (stat != ASN_OK) return stat;

         break;

      /* destroy_urban */
      case 2:
         stat = asn1PD_MagicActionDestroyUrban (ctxt_p, &pvalue->u.destroy_urban);
         if (stat != ASN_OK) return stat;

         break;

      default:
         return ASN_E_INVOPT;
   }

   return (stat);
}

int ASN1C_MsgUrbanMagicAction_action::Decode ()
{
   return asn1PD_MsgUrbanMagicAction_action (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgUrbanMagicAction                                       */
/*                                                            */
/**************************************************************/

ASN1C_MsgUrbanMagicAction::ASN1C_MsgUrbanMagicAction (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgUrbanMagicAction& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgUrbanMagicAction (ASN1CTXT* ctxt_p, ASN1T_MsgUrbanMagicAction* pvalue)
{
   int stat = ASN_OK;

   /* encode action */

   stat = asn1PE_MsgUrbanMagicAction_action (ctxt_p, &pvalue->action);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgUrbanMagicAction::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgUrbanMagicAction (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgUrbanMagicAction (ASN1CTXT* ctxt_p, ASN1T_MsgUrbanMagicAction* pvalue)
{
   int stat = ASN_OK;

   /* decode action */

   stat = asn1PD_MsgUrbanMagicAction_action (ctxt_p, &pvalue->action);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgUrbanMagicAction::Decode ()
{
   return asn1PD_MsgUrbanMagicAction (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgUrbanMagicActionAck                                    */
/*                                                            */
/**************************************************************/

ASN1C_MsgUrbanMagicActionAck::ASN1C_MsgUrbanMagicActionAck (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgUrbanMagicActionAck& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgUrbanMagicActionAck (ASN1CTXT* ctxt_p, ASN1T_MsgUrbanMagicActionAck* pvalue)
{
   int stat = ASN_OK;

   /* encode error_code */

   stat = asn1PE_EnumUrbanErrorCode (ctxt_p, pvalue->error_code);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MsgUrbanMagicActionAck::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgUrbanMagicActionAck (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgUrbanMagicActionAck (ASN1CTXT* ctxt_p, ASN1T_MsgUrbanMagicActionAck* pvalue)
{
   int stat = ASN_OK;

   /* decode error_code */

   stat = asn1PD_EnumUrbanErrorCode (ctxt_p, &pvalue->error_code);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MsgUrbanMagicActionAck::Decode ()
{
   return asn1PD_MsgUrbanMagicActionAck (mpContext->GetPtr(), &msgData);
}


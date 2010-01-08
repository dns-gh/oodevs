/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 24-Dec-2009.
 */
#include "MessengerMessages.h"

/**************************************************************/
/*                                                            */
/*  MessengerMessagesIncludes                                 */
/*                                                            */
/**************************************************************/

ASN1C_MessengerMessagesIncludes::ASN1C_MessengerMessagesIncludes (
   ASN1MessageBuffer& msgBuf, ASN1T_MessengerMessagesIncludes& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MessengerMessagesIncludes (ASN1CTXT* ctxt_p, ASN1T_MessengerMessagesIncludes* pvalue)
{
   int stat = ASN_OK;

   /* encode baseExports */

   stat = asn1PE_BaseExports (ctxt_p);
   if (stat != ASN_OK) return stat;

   /* encode limaLimiteExports */

   stat = asn1PE_LimaLimiteExports (ctxt_p);
   if (stat != ASN_OK) return stat;

   /* encode intelligenceExports */

   stat = asn1PE_IntelligenceExports (ctxt_p);
   if (stat != ASN_OK) return stat;

   /* encode drawingExports */

   stat = asn1PE_DrawingExports (ctxt_p);
   if (stat != ASN_OK) return stat;

   /* encode chatExports */

   stat = asn1PE_ChatExports (ctxt_p);
   if (stat != ASN_OK) return stat;

   return (stat);
}

int ASN1C_MessengerMessagesIncludes::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MessengerMessagesIncludes (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MessengerMessagesIncludes (ASN1CTXT* ctxt_p, ASN1T_MessengerMessagesIncludes* pvalue)
{
   int stat = ASN_OK;

   /* decode baseExports */

   stat = asn1PD_BaseExports (ctxt_p);
   if (stat != ASN_OK) return stat;

   /* decode limaLimiteExports */

   stat = asn1PD_LimaLimiteExports (ctxt_p);
   if (stat != ASN_OK) return stat;

   /* decode intelligenceExports */

   stat = asn1PD_IntelligenceExports (ctxt_p);
   if (stat != ASN_OK) return stat;

   /* decode drawingExports */

   stat = asn1PD_DrawingExports (ctxt_p);
   if (stat != ASN_OK) return stat;

   /* decode chatExports */

   stat = asn1PD_ChatExports (ctxt_p);
   if (stat != ASN_OK) return stat;


   return (stat);
}

int ASN1C_MessengerMessagesIncludes::Decode ()
{
   return asn1PD_MessengerMessagesIncludes (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MessengerMessagesExports                                  */
/*                                                            */
/**************************************************************/

ASN1C_MessengerMessagesExports::ASN1C_MessengerMessagesExports (
   ASN1MessageBuffer& msgBuf) :
   ASN1CType(msgBuf)
{}

EXTERN int asn1PE_MessengerMessagesExports (ASN1CTXT* ctxt_p)
{
   int stat = ASN_OK;

   /* NULL */
   return (stat);
}

int ASN1C_MessengerMessagesExports::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MessengerMessagesExports (mpContext->GetPtr());
   return stat;
}

EXTERN int asn1PD_MessengerMessagesExports (ASN1CTXT* ctxt_p)
{
   int stat = ASN_OK;

   /* NULL */

   return (stat);
}

int ASN1C_MessengerMessagesExports::Decode ()
{
   return asn1PD_MessengerMessagesExports (mpContext->GetPtr());
}

/**************************************************************/
/*                                                            */
/*  MsgsMessengerToClient                                     */
/*                                                            */
/**************************************************************/

ASN1C_MsgsMessengerToClient::ASN1C_MsgsMessengerToClient (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgsMessengerToClient& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgsMessengerToClient (ASN1CTXT* ctxt_p, ASN1T_MsgsMessengerToClient* pvalue)
{
   int stat = ASN_OK;

   /* Encode choice index value */

   stat = pe_ConsUnsigned (ctxt_p, pvalue->t - 1, 0, 24);
   if (stat != ASN_OK) return stat;

   /* Encode root element data value */

   switch (pvalue->t)
   {
      /* msg_limit_creation */
      case 1:
         stat = asn1PE_MsgLimitCreation (ctxt_p, pvalue->u.msg_limit_creation);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_limit_update */
      case 2:
         stat = asn1PE_MsgLimitUpdate (ctxt_p, pvalue->u.msg_limit_update);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_limit_destruction */
      case 3:
         stat = asn1PE_MsgLimitDestruction (ctxt_p, pvalue->u.msg_limit_destruction);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_limit_creation_request_ack */
      case 4:
         stat = asn1PE_MsgLimitCreationRequestAck (ctxt_p, pvalue->u.msg_limit_creation_request_ack);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_limit_destruction_request_ack */
      case 5:
         stat = asn1PE_MsgLimitDestructionRequestAck (ctxt_p, pvalue->u.msg_limit_destruction_request_ack);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_limit_update_request_ack */
      case 6:
         stat = asn1PE_MsgLimitUpdateRequestAck (ctxt_p, pvalue->u.msg_limit_update_request_ack);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_lima_creation */
      case 7:
         stat = asn1PE_MsgLimaCreation (ctxt_p, pvalue->u.msg_lima_creation);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_lima_update */
      case 8:
         stat = asn1PE_MsgLimaUpdate (ctxt_p, pvalue->u.msg_lima_update);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_lima_destruction */
      case 9:
         stat = asn1PE_MsgLimaDestruction (ctxt_p, pvalue->u.msg_lima_destruction);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_lima_creation_request_ack */
      case 10:
         stat = asn1PE_MsgLimaCreationRequestAck (ctxt_p, pvalue->u.msg_lima_creation_request_ack);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_lima_destruction_request_ack */
      case 11:
         stat = asn1PE_MsgLimaDestructionRequestAck (ctxt_p, pvalue->u.msg_lima_destruction_request_ack);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_lima_update_request_ack */
      case 12:
         stat = asn1PE_MsgLimaUpdateRequestAck (ctxt_p, pvalue->u.msg_lima_update_request_ack);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_intelligence_creation */
      case 13:
         stat = asn1PE_MsgIntelligenceCreation (ctxt_p, pvalue->u.msg_intelligence_creation);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_intelligence_update */
      case 14:
         stat = asn1PE_MsgIntelligenceUpdate (ctxt_p, pvalue->u.msg_intelligence_update);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_intelligence_destruction */
      case 15:
         stat = asn1PE_MsgIntelligenceDestruction (ctxt_p, pvalue->u.msg_intelligence_destruction);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_intelligence_creation_request_ack */
      case 16:
         stat = asn1PE_MsgIntelligenceCreationRequestAck (ctxt_p, pvalue->u.msg_intelligence_creation_request_ack);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_intelligence_update_request_ack */
      case 17:
         stat = asn1PE_MsgIntelligenceUpdateRequestAck (ctxt_p, pvalue->u.msg_intelligence_update_request_ack);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_intelligence_destruction_request_ack */
      case 18:
         stat = asn1PE_MsgIntelligenceDestructionRequestAck (ctxt_p, pvalue->u.msg_intelligence_destruction_request_ack);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_shape_creation */
      case 19:
         stat = asn1PE_MsgShapeCreation (ctxt_p, pvalue->u.msg_shape_creation);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_shape_update */
      case 20:
         stat = asn1PE_MsgShapeUpdate (ctxt_p, pvalue->u.msg_shape_update);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_shape_destruction */
      case 21:
         stat = asn1PE_MsgShapeDestruction (ctxt_p, pvalue->u.msg_shape_destruction);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_shape_creation_request_ack */
      case 22:
         stat = asn1PE_MsgShapeCreationRequestAck (ctxt_p, pvalue->u.msg_shape_creation_request_ack);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_shape_destruction_request_ack */
      case 23:
         stat = asn1PE_MsgShapeDestructionRequestAck (ctxt_p, pvalue->u.msg_shape_destruction_request_ack);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_shape_update_request_ack */
      case 24:
         stat = asn1PE_MsgShapeUpdateRequestAck (ctxt_p, pvalue->u.msg_shape_update_request_ack);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_text_message */
      case 25:
         stat = asn1PE_MsgTextMessage (ctxt_p, pvalue->u.msg_text_message);
         if (stat != ASN_OK) return stat;
         break;

      default:
         return ASN_E_INVOPT;
   }
   return (stat);
}

int ASN1C_MsgsMessengerToClient::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgsMessengerToClient (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgsMessengerToClient (ASN1CTXT* ctxt_p, ASN1T_MsgsMessengerToClient* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   stat = pd_ConsUnsigned (ctxt_p, &ui, 0, 24);
   if (stat != ASN_OK) return stat;
   else pvalue->t = ui + 1;

   switch (ui) {
      /* msg_limit_creation */
      case 0:
         pvalue->u.msg_limit_creation = ALLOC_ASN1ELEM (ctxt_p, ASN1T_MsgLimitCreation);

         stat = asn1PD_MsgLimitCreation (ctxt_p, pvalue->u.msg_limit_creation);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_limit_update */
      case 1:
         pvalue->u.msg_limit_update = ALLOC_ASN1ELEM (ctxt_p, ASN1T_MsgLimitUpdate);

         stat = asn1PD_MsgLimitUpdate (ctxt_p, pvalue->u.msg_limit_update);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_limit_destruction */
      case 2:
         stat = asn1PD_MsgLimitDestruction (ctxt_p, &pvalue->u.msg_limit_destruction);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_limit_creation_request_ack */
      case 3:
         stat = asn1PD_MsgLimitCreationRequestAck (ctxt_p, &pvalue->u.msg_limit_creation_request_ack);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_limit_destruction_request_ack */
      case 4:
         stat = asn1PD_MsgLimitDestructionRequestAck (ctxt_p, &pvalue->u.msg_limit_destruction_request_ack);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_limit_update_request_ack */
      case 5:
         stat = asn1PD_MsgLimitUpdateRequestAck (ctxt_p, &pvalue->u.msg_limit_update_request_ack);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_lima_creation */
      case 6:
         pvalue->u.msg_lima_creation = ALLOC_ASN1ELEM (ctxt_p, ASN1T_MsgLimaCreation);

         stat = asn1PD_MsgLimaCreation (ctxt_p, pvalue->u.msg_lima_creation);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_lima_update */
      case 7:
         pvalue->u.msg_lima_update = ALLOC_ASN1ELEM (ctxt_p, ASN1T_MsgLimaUpdate);

         stat = asn1PD_MsgLimaUpdate (ctxt_p, pvalue->u.msg_lima_update);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_lima_destruction */
      case 8:
         stat = asn1PD_MsgLimaDestruction (ctxt_p, &pvalue->u.msg_lima_destruction);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_lima_creation_request_ack */
      case 9:
         stat = asn1PD_MsgLimaCreationRequestAck (ctxt_p, &pvalue->u.msg_lima_creation_request_ack);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_lima_destruction_request_ack */
      case 10:
         stat = asn1PD_MsgLimaDestructionRequestAck (ctxt_p, &pvalue->u.msg_lima_destruction_request_ack);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_lima_update_request_ack */
      case 11:
         stat = asn1PD_MsgLimaUpdateRequestAck (ctxt_p, &pvalue->u.msg_lima_update_request_ack);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_intelligence_creation */
      case 12:
         pvalue->u.msg_intelligence_creation = ALLOC_ASN1ELEM (ctxt_p, ASN1T_MsgIntelligenceCreation);

         stat = asn1PD_MsgIntelligenceCreation (ctxt_p, pvalue->u.msg_intelligence_creation);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_intelligence_update */
      case 13:
         pvalue->u.msg_intelligence_update = ALLOC_ASN1ELEM (ctxt_p, ASN1T_MsgIntelligenceUpdate);

         stat = asn1PD_MsgIntelligenceUpdate (ctxt_p, pvalue->u.msg_intelligence_update);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_intelligence_destruction */
      case 14:
         pvalue->u.msg_intelligence_destruction = ALLOC_ASN1ELEM (ctxt_p, ASN1T_MsgIntelligenceDestruction);

         stat = asn1PD_MsgIntelligenceDestruction (ctxt_p, pvalue->u.msg_intelligence_destruction);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_intelligence_creation_request_ack */
      case 15:
         stat = asn1PD_MsgIntelligenceCreationRequestAck (ctxt_p, &pvalue->u.msg_intelligence_creation_request_ack);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_intelligence_update_request_ack */
      case 16:
         stat = asn1PD_MsgIntelligenceUpdateRequestAck (ctxt_p, &pvalue->u.msg_intelligence_update_request_ack);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_intelligence_destruction_request_ack */
      case 17:
         stat = asn1PD_MsgIntelligenceDestructionRequestAck (ctxt_p, &pvalue->u.msg_intelligence_destruction_request_ack);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_shape_creation */
      case 18:
         pvalue->u.msg_shape_creation = ALLOC_ASN1ELEM (ctxt_p, ASN1T_MsgShapeCreation);

         stat = asn1PD_MsgShapeCreation (ctxt_p, pvalue->u.msg_shape_creation);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_shape_update */
      case 19:
         pvalue->u.msg_shape_update = ALLOC_ASN1ELEM (ctxt_p, ASN1T_MsgShapeUpdate);

         stat = asn1PD_MsgShapeUpdate (ctxt_p, pvalue->u.msg_shape_update);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_shape_destruction */
      case 20:
         pvalue->u.msg_shape_destruction = ALLOC_ASN1ELEM (ctxt_p, ASN1T_MsgShapeDestruction);

         stat = asn1PD_MsgShapeDestruction (ctxt_p, pvalue->u.msg_shape_destruction);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_shape_creation_request_ack */
      case 21:
         stat = asn1PD_MsgShapeCreationRequestAck (ctxt_p, &pvalue->u.msg_shape_creation_request_ack);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_shape_destruction_request_ack */
      case 22:
         stat = asn1PD_MsgShapeDestructionRequestAck (ctxt_p, &pvalue->u.msg_shape_destruction_request_ack);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_shape_update_request_ack */
      case 23:
         stat = asn1PD_MsgShapeUpdateRequestAck (ctxt_p, &pvalue->u.msg_shape_update_request_ack);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_text_message */
      case 24:
         pvalue->u.msg_text_message = ALLOC_ASN1ELEM (ctxt_p, ASN1T_MsgTextMessage);

         stat = asn1PD_MsgTextMessage (ctxt_p, pvalue->u.msg_text_message);
         if (stat != ASN_OK) return stat;

         break;

      default:
         return ASN_E_INVOPT;
   }

   return (stat);
}

int ASN1C_MsgsMessengerToClient::Decode ()
{
   return asn1PD_MsgsMessengerToClient (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  MsgsClientToMessenger                                     */
/*                                                            */
/**************************************************************/

ASN1C_MsgsClientToMessenger::ASN1C_MsgsClientToMessenger (
   ASN1MessageBuffer& msgBuf, ASN1T_MsgsClientToMessenger& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_MsgsClientToMessenger (ASN1CTXT* ctxt_p, ASN1T_MsgsClientToMessenger* pvalue)
{
   int stat = ASN_OK;

   /* Encode choice index value */

   stat = pe_ConsUnsigned (ctxt_p, pvalue->t - 1, 0, 12);
   if (stat != ASN_OK) return stat;

   /* Encode root element data value */

   switch (pvalue->t)
   {
      /* msg_limit_creation_request */
      case 1:
         stat = asn1PE_MsgLimitCreationRequest (ctxt_p, pvalue->u.msg_limit_creation_request);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_limit_destruction_request */
      case 2:
         stat = asn1PE_MsgLimitDestructionRequest (ctxt_p, pvalue->u.msg_limit_destruction_request);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_limit_update_request */
      case 3:
         stat = asn1PE_MsgLimitUpdateRequest (ctxt_p, pvalue->u.msg_limit_update_request);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_lima_creation_request */
      case 4:
         stat = asn1PE_MsgLimaCreationRequest (ctxt_p, pvalue->u.msg_lima_creation_request);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_lima_destruction_request */
      case 5:
         stat = asn1PE_MsgLimaDestructionRequest (ctxt_p, pvalue->u.msg_lima_destruction_request);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_lima_update_request */
      case 6:
         stat = asn1PE_MsgLimaUpdateRequest (ctxt_p, pvalue->u.msg_lima_update_request);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_intelligence_creation_request */
      case 7:
         stat = asn1PE_MsgIntelligenceCreationRequest (ctxt_p, pvalue->u.msg_intelligence_creation_request);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_intelligence_update_request */
      case 8:
         stat = asn1PE_MsgIntelligenceUpdateRequest (ctxt_p, pvalue->u.msg_intelligence_update_request);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_intelligence_destruction_request */
      case 9:
         stat = asn1PE_MsgIntelligenceDestructionRequest (ctxt_p, pvalue->u.msg_intelligence_destruction_request);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_shape_creation_request */
      case 10:
         stat = asn1PE_MsgShapeCreationRequest (ctxt_p, pvalue->u.msg_shape_creation_request);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_shape_destruction_request */
      case 11:
         stat = asn1PE_MsgShapeDestructionRequest (ctxt_p, pvalue->u.msg_shape_destruction_request);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_shape_update_request */
      case 12:
         stat = asn1PE_MsgShapeUpdateRequest (ctxt_p, pvalue->u.msg_shape_update_request);
         if (stat != ASN_OK) return stat;
         break;

      /* msg_text_message */
      case 13:
         stat = asn1PE_MsgTextMessage (ctxt_p, pvalue->u.msg_text_message);
         if (stat != ASN_OK) return stat;
         break;

      default:
         return ASN_E_INVOPT;
   }
   return (stat);
}

int ASN1C_MsgsClientToMessenger::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_MsgsClientToMessenger (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_MsgsClientToMessenger (ASN1CTXT* ctxt_p, ASN1T_MsgsClientToMessenger* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   stat = pd_ConsUnsigned (ctxt_p, &ui, 0, 12);
   if (stat != ASN_OK) return stat;
   else pvalue->t = ui + 1;

   switch (ui) {
      /* msg_limit_creation_request */
      case 0:
         pvalue->u.msg_limit_creation_request = ALLOC_ASN1ELEM (ctxt_p, ASN1T_MsgLimitCreationRequest);

         stat = asn1PD_MsgLimitCreationRequest (ctxt_p, pvalue->u.msg_limit_creation_request);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_limit_destruction_request */
      case 1:
         stat = asn1PD_MsgLimitDestructionRequest (ctxt_p, &pvalue->u.msg_limit_destruction_request);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_limit_update_request */
      case 2:
         pvalue->u.msg_limit_update_request = ALLOC_ASN1ELEM (ctxt_p, ASN1T_MsgLimitUpdateRequest);

         stat = asn1PD_MsgLimitUpdateRequest (ctxt_p, pvalue->u.msg_limit_update_request);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_lima_creation_request */
      case 3:
         pvalue->u.msg_lima_creation_request = ALLOC_ASN1ELEM (ctxt_p, ASN1T_MsgLimaCreationRequest);

         stat = asn1PD_MsgLimaCreationRequest (ctxt_p, pvalue->u.msg_lima_creation_request);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_lima_destruction_request */
      case 4:
         stat = asn1PD_MsgLimaDestructionRequest (ctxt_p, &pvalue->u.msg_lima_destruction_request);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_lima_update_request */
      case 5:
         pvalue->u.msg_lima_update_request = ALLOC_ASN1ELEM (ctxt_p, ASN1T_MsgLimaUpdateRequest);

         stat = asn1PD_MsgLimaUpdateRequest (ctxt_p, pvalue->u.msg_lima_update_request);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_intelligence_creation_request */
      case 6:
         pvalue->u.msg_intelligence_creation_request = ALLOC_ASN1ELEM (ctxt_p, ASN1T_MsgIntelligenceCreationRequest);

         stat = asn1PD_MsgIntelligenceCreationRequest (ctxt_p, pvalue->u.msg_intelligence_creation_request);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_intelligence_update_request */
      case 7:
         pvalue->u.msg_intelligence_update_request = ALLOC_ASN1ELEM (ctxt_p, ASN1T_MsgIntelligenceUpdateRequest);

         stat = asn1PD_MsgIntelligenceUpdateRequest (ctxt_p, pvalue->u.msg_intelligence_update_request);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_intelligence_destruction_request */
      case 8:
         pvalue->u.msg_intelligence_destruction_request = ALLOC_ASN1ELEM (ctxt_p, ASN1T_MsgIntelligenceDestructionRequest);

         stat = asn1PD_MsgIntelligenceDestructionRequest (ctxt_p, pvalue->u.msg_intelligence_destruction_request);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_shape_creation_request */
      case 9:
         pvalue->u.msg_shape_creation_request = ALLOC_ASN1ELEM (ctxt_p, ASN1T_MsgShapeCreationRequest);

         stat = asn1PD_MsgShapeCreationRequest (ctxt_p, pvalue->u.msg_shape_creation_request);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_shape_destruction_request */
      case 10:
         pvalue->u.msg_shape_destruction_request = ALLOC_ASN1ELEM (ctxt_p, ASN1T_MsgShapeDestructionRequest);

         stat = asn1PD_MsgShapeDestructionRequest (ctxt_p, pvalue->u.msg_shape_destruction_request);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_shape_update_request */
      case 11:
         pvalue->u.msg_shape_update_request = ALLOC_ASN1ELEM (ctxt_p, ASN1T_MsgShapeUpdateRequest);

         stat = asn1PD_MsgShapeUpdateRequest (ctxt_p, pvalue->u.msg_shape_update_request);
         if (stat != ASN_OK) return stat;

         break;

      /* msg_text_message */
      case 12:
         pvalue->u.msg_text_message = ALLOC_ASN1ELEM (ctxt_p, ASN1T_MsgTextMessage);

         stat = asn1PD_MsgTextMessage (ctxt_p, pvalue->u.msg_text_message);
         if (stat != ASN_OK) return stat;

         break;

      default:
         return ASN_E_INVOPT;
   }

   return (stat);
}

int ASN1C_MsgsClientToMessenger::Decode ()
{
   return asn1PD_MsgsClientToMessenger (mpContext->GetPtr(), &msgData);
}


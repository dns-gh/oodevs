/* This file was generated by ASN1C V5.31 on 09-Jul-2009 */

#include "AarMessages.h"

void asn1Print_AarMessagesIncludes (ASN1ConstCharPtr name, ASN1T_AarMessagesIncludes* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.aarExports", name);

}

void ASN1C_AarMessagesIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_AarMessagesIncludes (name, &msgData);
}

void asn1Print_AarMessagesExports (ASN1ConstCharPtr name)
{
}

void ASN1C_AarMessagesExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_AarMessagesExports (name);
}

void asn1Print_MsgsAarToClient_msg (ASN1ConstCharPtr name, ASN1T_MsgsAarToClient_msg* pvalue)
{
   char namebuf[512];

   printf ("%s.t = %d\n", name, pvalue->t);
   switch (pvalue->t) {
      case 1:
         sprintf (namebuf, "%s.u.msg_aar_information", name);
         asn1Print_MsgAarInformation (namebuf, pvalue->u.msg_aar_information);
         break;

      case 2:
         sprintf (namebuf, "%s.u.msg_plot_result", name);
         asn1Print_MsgPlotResult (namebuf, pvalue->u.msg_plot_result);
         break;

      case 3:
         sprintf (namebuf, "%s.u.msg_indicator", name);
         asn1Print_MsgIndicator (namebuf, pvalue->u.msg_indicator);
         break;

      default:;
   }
}

void ASN1C_MsgsAarToClient_msg::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgsAarToClient_msg (name, &msgData);
}

void asn1Print_MsgsAarToClient (ASN1ConstCharPtr name, ASN1T_MsgsAarToClient* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.context", name);
   rtPrintInteger (namebuf, pvalue->context);

   sprintf (namebuf, "%s.msg", name);
   asn1Print_MsgsAarToClient_msg (namebuf, &pvalue->msg);

}

void ASN1C_MsgsAarToClient::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgsAarToClient (name, &msgData);
}

void asn1Print_MsgsClientToAar_msg (ASN1ConstCharPtr name, ASN1T_MsgsClientToAar_msg* pvalue)
{
   char namebuf[512];

   printf ("%s.t = %d\n", name, pvalue->t);
   switch (pvalue->t) {
      case 1:
         sprintf (namebuf, "%s.u.msg_plot_request", name);
         asn1Print_MsgPlotRequest (namebuf, pvalue->u.msg_plot_request);
         break;

      default:;
   }
}

void ASN1C_MsgsClientToAar_msg::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgsClientToAar_msg (name, &msgData);
}

void asn1Print_MsgsClientToAar (ASN1ConstCharPtr name, ASN1T_MsgsClientToAar* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.context", name);
   rtPrintInteger (namebuf, pvalue->context);

   sprintf (namebuf, "%s.msg", name);
   asn1Print_MsgsClientToAar_msg (namebuf, &pvalue->msg);

}

void ASN1C_MsgsClientToAar::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgsClientToAar (name, &msgData);
}


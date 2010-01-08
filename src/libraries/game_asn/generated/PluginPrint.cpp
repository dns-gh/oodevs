/* This file was generated by ASN1C V5.31 on 24-Dec-2009 */

#include "Plugin.h"

void asn1Print_PluginIncludes (ASN1ConstCharPtr name, ASN1T_PluginIncludes* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.baseExports", name);

   sprintf (namebuf, "%s.armyExports", name);

   sprintf (namebuf, "%s.intelligenceExports", name);

   sprintf (namebuf, "%s.limaLimiteExports", name);

}

void ASN1C_PluginIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_PluginIncludes (name, &msgData);
}

void asn1Print_PluginExports (ASN1ConstCharPtr name)
{
}

void ASN1C_PluginExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_PluginExports (name);
}

void asn1Print_MsgPluginTextMessage (ASN1ConstCharPtr name, ASN1T_MsgPluginTextMessage* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.destinataire", name);
   rtPrintCharStr (namebuf, pvalue->destinataire);

   sprintf (namebuf, "%s.message", name);
   rtPrintCharStr (namebuf, pvalue->message);

}

void ASN1C_MsgPluginTextMessage::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgPluginTextMessage (name, &msgData);
}


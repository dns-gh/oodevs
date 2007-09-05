/* This file was generated by ASN1C V5.31 on 05-Sep-2007 */

#include "Folk.h"

void asn1Print_FolkIncludes (ASN1ConstCharPtr name, ASN1T_FolkIncludes* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.baseExports", name);

}

void ASN1C_FolkIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_FolkIncludes (name, &msgData);
}

void asn1Print_FolkExports (ASN1ConstCharPtr name)
{
}

void ASN1C_FolkExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_FolkExports (name);
}

void asn1Print_MsgFolkCreation_profiles (ASN1ConstCharPtr name, ASN1T_MsgFolkCreation_profiles* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      rtPrintOctStr (namebuf, pvalue->elem[xx1].numocts, pvalue->elem[xx1].data, ".");
   }
   }

}

void ASN1C_MsgFolkCreation_profiles::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgFolkCreation_profiles (name, &msgData);
}

void asn1Print_MsgFolkCreation_activities (ASN1ConstCharPtr name, ASN1T_MsgFolkCreation_activities* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      rtPrintOctStr (namebuf, pvalue->elem[xx1].numocts, pvalue->elem[xx1].data, ".");
   }
   }

}

void ASN1C_MsgFolkCreation_activities::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgFolkCreation_activities (name, &msgData);
}

void asn1Print_MsgFolkCreation (ASN1ConstCharPtr name, ASN1T_MsgFolkCreation* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.profiles", name);
   asn1Print_MsgFolkCreation_profiles (namebuf, &pvalue->profiles);

   sprintf (namebuf, "%s.activities", name);
   asn1Print_MsgFolkCreation_activities (namebuf, &pvalue->activities);

   sprintf (namebuf, "%s.container_size", name);
   rtPrintInteger (namebuf, pvalue->container_size);

   sprintf (namebuf, "%s.edge_number", name);
   rtPrintInteger (namebuf, pvalue->edge_number);

}

void ASN1C_MsgFolkCreation::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgFolkCreation (name, &msgData);
}

void asn1Print_MsgFolkGraphEdgeUpdate_population_occupation (ASN1ConstCharPtr name, ASN1T_MsgFolkGraphEdgeUpdate_population_occupation* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      rtPrintInteger (namebuf, pvalue->elem[xx1]);
   }
   }

}

void ASN1C_MsgFolkGraphEdgeUpdate_population_occupation::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgFolkGraphEdgeUpdate_population_occupation (name, &msgData);
}

void asn1Print_MsgFolkGraphEdgeUpdate (ASN1ConstCharPtr name, ASN1T_MsgFolkGraphEdgeUpdate* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.oid", name);
   asn1Print_OID (namebuf, &pvalue->oid);

   sprintf (namebuf, "%s.shp_oid", name);
   asn1Print_OID (namebuf, &pvalue->shp_oid);

   sprintf (namebuf, "%s.population_occupation", name);
   asn1Print_MsgFolkGraphEdgeUpdate_population_occupation (namebuf, &pvalue->population_occupation);

}

void ASN1C_MsgFolkGraphEdgeUpdate::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgFolkGraphEdgeUpdate (name, &msgData);
}

void asn1Print_MsgFolkGraphUpdate (ASN1ConstCharPtr name, ASN1T_MsgFolkGraphUpdate* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_MsgFolkGraphEdgeUpdate (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C_MsgFolkGraphUpdate::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgFolkGraphUpdate (name, &msgData);
}


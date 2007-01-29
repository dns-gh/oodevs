/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 29-Jan-2007.
 */
#include "Games.h"

/**************************************************************/
/*                                                            */
/*  GamesIncludes                                             */
/*                                                            */
/**************************************************************/

ASN1C_GamesIncludes::ASN1C_GamesIncludes (
   ASN1MessageBuffer& msgBuf, ASN1T_GamesIncludes& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_GamesIncludes (ASN1CTXT* ctxt_p, ASN1T_GamesIncludes* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_GamesIncludes: start\n");


   rtdiag ("asn1PE_GamesIncludes: end\n");
   return (stat);
}

int ASN1C_GamesIncludes::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_GamesIncludes (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_GamesIncludes (ASN1CTXT* ctxt_p, ASN1T_GamesIncludes* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_GamesIncludes: start\n");

   /* decode  */

   PU_PUSHNAME (ctxt_p, "");


   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_GamesIncludes: end\n");

   return (stat);
}

int ASN1C_GamesIncludes::Decode ()
{
   return asn1PD_GamesIncludes (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  GamesExports                                              */
/*                                                            */
/**************************************************************/

ASN1C_GamesExports::ASN1C_GamesExports (
   ASN1MessageBuffer& msgBuf) :
   ASN1CType(msgBuf)
{}

EXTERN int asn1PE_GamesExports (ASN1CTXT* ctxt_p)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_GamesExports: start\n");

   /* NULL */

   rtdiag ("asn1PE_GamesExports: end\n");
   return (stat);
}

int ASN1C_GamesExports::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_GamesExports (mpContext->GetPtr());
   return stat;
}

EXTERN int asn1PD_GamesExports (ASN1CTXT* ctxt_p)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_GamesExports: start\n");

   /* NULL */

   rtdiag ("asn1PD_GamesExports: end\n");

   return (stat);
}

int ASN1C_GamesExports::Decode ()
{
   return asn1PD_GamesExports (mpContext->GetPtr());
}

/**************************************************************/
/*                                                            */
/*  Game_status                                               */
/*                                                            */
/**************************************************************/

ASN1C_Game_status::ASN1C_Game_status (
   ASN1MessageBuffer& msgBuf, ASN1T_Game_status& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Game_status (ASN1CTXT* ctxt_p, ASN1T_Game_status value)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   rtdiag ("asn1PE_Game_status: start\n");

   switch (value) {
      case Game_status::running: ui = 0; break;
      case Game_status::paused: ui = 1; break;
      default: return LOG_ASN1ERR (ctxt_p, ASN_E_INVENUM);
   }

   stat = pe_ConsUnsigned (ctxt_p, ui, 0, 1);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   rtdiag ("asn1PE_Game_status: end\n");
   return (stat);
}

int ASN1C_Game_status::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Game_status (mpContext->GetPtr(), msgData);
   return stat;
}

EXTERN int asn1PD_Game_status (ASN1CTXT* ctxt_p, ASN1T_Game_status* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;

   rtdiag ("asn1PD_Game_status: start\n");

   stat = pd_ConsUnsigned (ctxt_p, &ui, 0, 1);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   switch (ui) {
      case 0: *pvalue = Game_status::running; break;
      case 1: *pvalue = Game_status::paused; break;
      default: return LOG_ASN1ERR (ctxt_p, ASN_E_INVENUM);
   }

   rtdiag ("asn1PD_Game_status: end\n");

   return (stat);
}

int ASN1C_Game_status::Decode ()
{
   return asn1PD_Game_status (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  Game                                                      */
/*                                                            */
/**************************************************************/

ASN1C_Game::ASN1C_Game (
   ASN1MessageBuffer& msgBuf, ASN1T_Game& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_Game (ASN1CTXT* ctxt_p, ASN1T_Game* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PE_Game: start\n");

   /* encode name */

   PU_PUSHNAME (ctxt_p, "name");

   stat = pe_VisibleString (ctxt_p, pvalue->name, 0);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode host */

   PU_PUSHNAME (ctxt_p, "host");

   stat = pe_VisibleString (ctxt_p, pvalue->host, 0);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode exercise_name */

   PU_PUSHNAME (ctxt_p, "exercise_name");

   stat = pe_VisibleString (ctxt_p, pvalue->exercise_name, 0);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode start_date */

   PU_PUSHNAME (ctxt_p, "start_date");

   stat = pe_VisibleString (ctxt_p, pvalue->start_date, 0);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);

   /* encode status */

   PU_PUSHNAME (ctxt_p, "status");

   stat = asn1PE_Game_status (ctxt_p, pvalue->status);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PE_Game: end\n");
   return (stat);
}

int ASN1C_Game::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_Game (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_Game (ASN1CTXT* ctxt_p, ASN1T_Game* pvalue)
{
   int stat = ASN_OK;

   rtdiag ("asn1PD_Game: start\n");

   /* decode name */

   PU_PUSHNAME (ctxt_p, "name");

   stat = pd_VisibleString (ctxt_p, &pvalue->name, 0);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode host */

   PU_PUSHNAME (ctxt_p, "host");

   stat = pd_VisibleString (ctxt_p, &pvalue->host, 0);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode exercise_name */

   PU_PUSHNAME (ctxt_p, "exercise_name");

   stat = pd_VisibleString (ctxt_p, &pvalue->exercise_name, 0);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode start_date */

   PU_PUSHNAME (ctxt_p, "start_date");

   stat = pd_VisibleString (ctxt_p, &pvalue->start_date, 0);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode status */

   PU_PUSHNAME (ctxt_p, "status");

   stat = asn1PD_Game_status (ctxt_p, &pvalue->status);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);


   rtdiag ("asn1PD_Game: end\n");

   return (stat);
}

int ASN1C_Game::Decode ()
{
   return asn1PD_Game (mpContext->GetPtr(), &msgData);
}

/**************************************************************/
/*                                                            */
/*  GameList                                                  */
/*                                                            */
/**************************************************************/

ASN1C_GameList::ASN1C_GameList (
   ASN1MessageBuffer& msgBuf, ASN1T_GameList& data) :
   ASN1CType(msgBuf), msgData(data)
{}

EXTERN int asn1PE_GameList (ASN1CTXT* ctxt_p, ASN1T_GameList* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   rtdiag ("asn1PE_GameList: start\n");

   /* encode length determinant */

   PU_PUSHNAME (ctxt_p, "n");

   stat = pe_Length (ctxt_p, pvalue->n);
   if (stat < 0) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* encode elements */

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      PU_PUSHELEMNAME (ctxt_p, xx1);

      stat = asn1PE_Game (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);
      PU_POPNAME (ctxt_p);
   }

   rtdiag ("asn1PE_GameList: end\n");
   return (stat);
}

int ASN1C_GameList::Encode ()
{
   mMsgBuf.Init ();
   int stat = asn1PE_GameList (mpContext->GetPtr(), &msgData);
   return stat;
}

EXTERN int asn1PD_GameList (ASN1CTXT* ctxt_p, ASN1T_GameList* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   rtdiag ("asn1PD_GameList: start\n");

   /* decode length determinant */

   PU_PUSHNAME (ctxt_p, "n");

   stat = pd_Length (ctxt_p, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

   PU_POPNAME (ctxt_p);

   /* decode elements */

   ALLOC_ASN1ARRAY (ctxt_p, pvalue, ASN1T_Game);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      PU_PUSHELEMNAME (ctxt_p, xx1);

      stat = asn1PD_Game (ctxt_p, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (ctxt_p, stat);

      PU_POPNAME (ctxt_p);
   }


   rtdiag ("asn1PD_GameList: end\n");

   return (stat);
}

int ASN1C_GameList::Decode ()
{
   return asn1PD_GameList (mpContext->GetPtr(), &msgData);
}


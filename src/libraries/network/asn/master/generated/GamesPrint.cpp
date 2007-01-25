/* This file was generated by ASN1C V5.31 on 25-Jan-2007 */

#include "Games.h"

void asn1Print_GamesIncludes (ASN1ConstCharPtr name, ASN1T_GamesIncludes* pvalue)
{
}

void ASN1C_GamesIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_GamesIncludes (name, &msgData);
}

void asn1Print_GamesExports (ASN1ConstCharPtr name)
{
}

void ASN1C_GamesExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_GamesExports (name);
}

void asn1Print_Game_status (ASN1ConstCharPtr name, ASN1T_Game_status* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("running\n"); break;
      case 1: printf ("paused\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_Game_status::Print (ASN1ConstCharPtr name)

{
   asn1Print_Game_status (name, &msgData);
}

void asn1Print_Game (ASN1ConstCharPtr name, ASN1T_Game* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.name", name);
   rtPrintCharStr (namebuf, pvalue->name);

   sprintf (namebuf, "%s.host", name);
   rtPrintCharStr (namebuf, pvalue->host);

   sprintf (namebuf, "%s.exercise_name", name);
   rtPrintCharStr (namebuf, pvalue->exercise_name);

   sprintf (namebuf, "%s.start_date", name);
   rtPrintCharStr (namebuf, pvalue->start_date);

   sprintf (namebuf, "%s.status", name);
   asn1Print_Game_status (namebuf, &pvalue->status);

}

void ASN1C_Game::Print (ASN1ConstCharPtr name)

{
   asn1Print_Game (name, &msgData);
}

void asn1Print_GameList (ASN1ConstCharPtr name, ASN1T_GameList* pvalue)
{
   char namebuf[512];

   printf ("%s.n = %d\n", name, pvalue->n);
   {
   ASN1UINT xx1;
   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      sprintf (namebuf, "%s.elem[%d]", name, xx1);
      asn1Print_Game (namebuf, &pvalue->elem[xx1]);
   }
   }

}

void ASN1C_GameList::Print (ASN1ConstCharPtr name)

{
   asn1Print_GameList (name, &msgData);
}

void asn1Print_MsgGameListRequest (ASN1ConstCharPtr name, ASN1T_MsgGameListRequest* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.exercise_name", name);
   rtPrintCharStr (namebuf, pvalue->exercise_name);

}

void ASN1C_MsgGameListRequest::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgGameListRequest (name, &msgData);
}

void asn1Print_MsgGameListResponse_return_code (ASN1ConstCharPtr name, ASN1T_MsgGameListResponse_return_code* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("ok\n"); break;
      case 1: printf ("forbidden\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_MsgGameListResponse_return_code::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgGameListResponse_return_code (name, &msgData);
}

void asn1Print_MsgGameListResponse (ASN1ConstCharPtr name, ASN1T_MsgGameListResponse* pvalue)
{
   char namebuf[512];

   printf ("%s.m.gamesPresent = %d\n", name, (int)pvalue->m.gamesPresent);
   sprintf (namebuf, "%s.return_code", name);
   asn1Print_MsgGameListResponse_return_code (namebuf, &pvalue->return_code);

   if (pvalue->m.gamesPresent) {
      sprintf (namebuf, "%s.games", name);
      asn1Print_GameList (namebuf, &pvalue->games);
   }

}

void ASN1C_MsgGameListResponse::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgGameListResponse (name, &msgData);
}

void asn1Print_MsgGameStartRequest (ASN1ConstCharPtr name, ASN1T_MsgGameStartRequest* pvalue)
{
   char namebuf[512];

   printf ("%s.m.namePresent = %d\n", name, (int)pvalue->m.namePresent);
   if (pvalue->m.namePresent) {
      sprintf (namebuf, "%s.name", name);
      rtPrintCharStr (namebuf, pvalue->name);
   }

   sprintf (namebuf, "%s.exercise", name);
   rtPrintCharStr (namebuf, pvalue->exercise);

}

void ASN1C_MsgGameStartRequest::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgGameStartRequest (name, &msgData);
}

void asn1Print_MsgGameStartResponse_return_code (ASN1ConstCharPtr name, ASN1T_MsgGameStartResponse_return_code* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("ok\n"); break;
      case 1: printf ("forbidden\n"); break;
      case 2: printf ("exercise_not_found\n"); break;
      case 3: printf ("game_name_exists\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_MsgGameStartResponse_return_code::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgGameStartResponse_return_code (name, &msgData);
}

void asn1Print_MsgGameStartResponse (ASN1ConstCharPtr name, ASN1T_MsgGameStartResponse* pvalue)
{
   char namebuf[512];

   printf ("%s.m.gamePresent = %d\n", name, (int)pvalue->m.gamePresent);
   sprintf (namebuf, "%s.return_code", name);
   asn1Print_MsgGameStartResponse_return_code (namebuf, &pvalue->return_code);

   if (pvalue->m.gamePresent) {
      sprintf (namebuf, "%s.game", name);
      asn1Print_Game (namebuf, &pvalue->game);
   }

}

void ASN1C_MsgGameStartResponse::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgGameStartResponse (name, &msgData);
}

void asn1Print_MsgGameStopRequest (ASN1ConstCharPtr name, ASN1T_MsgGameStopRequest* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.name", name);
   rtPrintCharStr (namebuf, pvalue->name);

}

void ASN1C_MsgGameStopRequest::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgGameStopRequest (name, &msgData);
}

void asn1Print_MsgGameStopResponse_return_code (ASN1ConstCharPtr name, ASN1T_MsgGameStopResponse_return_code* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("ok\n"); break;
      case 1: printf ("forbidden\n"); break;
      case 2: printf ("game_not_found\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_MsgGameStopResponse_return_code::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgGameStopResponse_return_code (name, &msgData);
}

void asn1Print_MsgGameStopResponse (ASN1ConstCharPtr name, ASN1T_MsgGameStopResponse* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.return_code", name);
   asn1Print_MsgGameStopResponse_return_code (namebuf, &pvalue->return_code);

}

void ASN1C_MsgGameStopResponse::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgGameStopResponse (name, &msgData);
}


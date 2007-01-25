/* This file was generated by ASN1C V5.31 on 25-Jan-2007 */

#include "Messages.h"

void asn1Print_MessagesIncludes (ASN1ConstCharPtr name, ASN1T_MessagesIncludes* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.profilesExports", name);

   sprintf (namebuf, "%s.exercisesExports", name);

   sprintf (namebuf, "%s.gamesExports", name);

}

void ASN1C_MessagesIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_MessagesIncludes (name, &msgData);
}

void asn1Print_MessagesExports (ASN1ConstCharPtr name)
{
}

void ASN1C_MessagesExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_MessagesExports (name);
}

void asn1Print_MsgsInMaster_msg (ASN1ConstCharPtr name, ASN1T_MsgsInMaster_msg* pvalue)
{
   char namebuf[512];

   printf ("%s.t = %d\n", name, pvalue->t);
   switch (pvalue->t) {
      case 1:
         sprintf (namebuf, "%s.u.msg_authentication_request", name);
         asn1Print_MsgAuthenticationRequest (namebuf, pvalue->u.msg_authentication_request);
         break;

      case 2:
         sprintf (namebuf, "%s.u.msg_exercise_list_request", name);
         asn1Print_MsgExerciseListRequest (namebuf, pvalue->u.msg_exercise_list_request);
         break;

      case 3:
         sprintf (namebuf, "%s.u.msg_game_start_request", name);
         asn1Print_MsgGameStartRequest (namebuf, pvalue->u.msg_game_start_request);
         break;

      case 4:
         sprintf (namebuf, "%s.u.msg_game_stop_request", name);
         asn1Print_MsgGameStopRequest (namebuf, pvalue->u.msg_game_stop_request);
         break;

      default:;
   }
}

void ASN1C_MsgsInMaster_msg::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgsInMaster_msg (name, &msgData);
}

void asn1Print_MsgsInMaster (ASN1ConstCharPtr name, ASN1T_MsgsInMaster* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.context", name);
   rtPrintInteger (namebuf, pvalue->context);

   sprintf (namebuf, "%s.msg", name);
   asn1Print_MsgsInMaster_msg (namebuf, &pvalue->msg);

}

void ASN1C_MsgsInMaster::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgsInMaster (name, &msgData);
}

void asn1Print_MsgsOutMaster_msg (ASN1ConstCharPtr name, ASN1T_MsgsOutMaster_msg* pvalue)
{
   char namebuf[512];

   printf ("%s.t = %d\n", name, pvalue->t);
   switch (pvalue->t) {
      case 1:
         sprintf (namebuf, "%s.u.msg_authentication_response", name);
         asn1Print_MsgAuthenticationResponse (namebuf, pvalue->u.msg_authentication_response);
         break;

      case 2:
         sprintf (namebuf, "%s.u.msg_exercise_list_response", name);
         asn1Print_MsgExerciseListResponse (namebuf, pvalue->u.msg_exercise_list_response);
         break;

      case 3:
         sprintf (namebuf, "%s.u.msg_game_start_response", name);
         asn1Print_MsgGameStartResponse (namebuf, pvalue->u.msg_game_start_response);
         break;

      case 4:
         sprintf (namebuf, "%s.u.msg_game_stop_response", name);
         asn1Print_MsgGameStopResponse (namebuf, pvalue->u.msg_game_stop_response);
         break;

      default:;
   }
}

void ASN1C_MsgsOutMaster_msg::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgsOutMaster_msg (name, &msgData);
}

void asn1Print_MsgsOutMaster (ASN1ConstCharPtr name, ASN1T_MsgsOutMaster* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.context", name);
   rtPrintInteger (namebuf, pvalue->context);

   sprintf (namebuf, "%s.msg", name);
   asn1Print_MsgsOutMaster_msg (namebuf, &pvalue->msg);

}

void ASN1C_MsgsOutMaster::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgsOutMaster (name, &msgData);
}

void asn1Print_MsgsInFrontend_msg (ASN1ConstCharPtr name, ASN1T_MsgsInFrontend_msg* pvalue)
{
   char namebuf[512];

   printf ("%s.t = %d\n", name, pvalue->t);
   switch (pvalue->t) {
      case 1:
         sprintf (namebuf, "%s.u.msg_authentication_response", name);
         asn1Print_MsgAuthenticationResponse (namebuf, pvalue->u.msg_authentication_response);
         break;

      case 2:
         sprintf (namebuf, "%s.u.msg_exercise_list_response", name);
         asn1Print_MsgExerciseListResponse (namebuf, pvalue->u.msg_exercise_list_response);
         break;

      case 3:
         sprintf (namebuf, "%s.u.msg_game_start_response", name);
         asn1Print_MsgGameStartResponse (namebuf, pvalue->u.msg_game_start_response);
         break;

      case 4:
         sprintf (namebuf, "%s.u.msg_game_stop_response", name);
         asn1Print_MsgGameStopResponse (namebuf, pvalue->u.msg_game_stop_response);
         break;

      default:;
   }
}

void ASN1C_MsgsInFrontend_msg::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgsInFrontend_msg (name, &msgData);
}

void asn1Print_MsgsInFrontend (ASN1ConstCharPtr name, ASN1T_MsgsInFrontend* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.context", name);
   rtPrintInteger (namebuf, pvalue->context);

   sprintf (namebuf, "%s.msg", name);
   asn1Print_MsgsInFrontend_msg (namebuf, &pvalue->msg);

}

void ASN1C_MsgsInFrontend::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgsInFrontend (name, &msgData);
}

void asn1Print_MsgsOutFrontend_msg (ASN1ConstCharPtr name, ASN1T_MsgsOutFrontend_msg* pvalue)
{
   char namebuf[512];

   printf ("%s.t = %d\n", name, pvalue->t);
   switch (pvalue->t) {
      case 1:
         sprintf (namebuf, "%s.u.msg_authentication_request", name);
         asn1Print_MsgAuthenticationRequest (namebuf, pvalue->u.msg_authentication_request);
         break;

      case 2:
         sprintf (namebuf, "%s.u.msg_exercise_list_request", name);
         asn1Print_MsgExerciseListRequest (namebuf, pvalue->u.msg_exercise_list_request);
         break;

      case 3:
         sprintf (namebuf, "%s.u.msg_game_start_request", name);
         asn1Print_MsgGameStartRequest (namebuf, pvalue->u.msg_game_start_request);
         break;

      case 4:
         sprintf (namebuf, "%s.u.msg_game_stop_request", name);
         asn1Print_MsgGameStopRequest (namebuf, pvalue->u.msg_game_stop_request);
         break;

      default:;
   }
}

void ASN1C_MsgsOutFrontend_msg::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgsOutFrontend_msg (name, &msgData);
}

void asn1Print_MsgsOutFrontend (ASN1ConstCharPtr name, ASN1T_MsgsOutFrontend* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.context", name);
   rtPrintInteger (namebuf, pvalue->context);

   sprintf (namebuf, "%s.msg", name);
   asn1Print_MsgsOutFrontend_msg (namebuf, &pvalue->msg);

}

void ASN1C_MsgsOutFrontend::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgsOutFrontend (name, &msgData);
}


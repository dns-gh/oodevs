/* This file was generated by ASN1C V5.31 on 30-Jan-2007 */

#include "ScipioControl.h"

void asn1Print_ScipioControlIncludes (ASN1ConstCharPtr name, ASN1T_ScipioControlIncludes* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.scipioBaseExports", name);

}

void ASN1C_ScipioControlIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_ScipioControlIncludes (name, &msgData);
}

void asn1Print_ScipioControlExports (ASN1ConstCharPtr name)
{
}

void ASN1C_ScipioControlExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_ScipioControlExports (name);
}

void asn1Print_EnumCtrlErrorCode (ASN1ConstCharPtr name, ASN1T_EnumCtrlErrorCode* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("no_error\n"); break;
      case 1: printf ("error_already_started\n"); break;
      case 2: printf ("error_not_started\n"); break;
      case 3: printf ("error_not_paused\n"); break;
      case 4: printf ("error_already_paused\n"); break;
      case 5: printf ("error_invalid_time_factor\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumCtrlErrorCode::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumCtrlErrorCode (name, &msgData);
}

void asn1Print_MsgCtrlStop (ASN1ConstCharPtr name)
{
}

void ASN1C_MsgCtrlStop::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgCtrlStop (name);
}

void asn1Print_MsgCtrlStopAck (ASN1ConstCharPtr name, ASN1T_MsgCtrlStopAck* pvalue)
{
   asn1Print_EnumCtrlErrorCode (name, pvalue);
}

void ASN1C_MsgCtrlStopAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgCtrlStopAck (name, &msgData);
}

void asn1Print_MsgCtrlPause (ASN1ConstCharPtr name)
{
}

void ASN1C_MsgCtrlPause::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgCtrlPause (name);
}

void asn1Print_MsgCtrlPauseAck (ASN1ConstCharPtr name, ASN1T_MsgCtrlPauseAck* pvalue)
{
   asn1Print_EnumCtrlErrorCode (name, pvalue);
}

void ASN1C_MsgCtrlPauseAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgCtrlPauseAck (name, &msgData);
}

void asn1Print_MsgCtrlResume (ASN1ConstCharPtr name)
{
}

void ASN1C_MsgCtrlResume::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgCtrlResume (name);
}

void asn1Print_MsgCtrlResumeAck (ASN1ConstCharPtr name, ASN1T_MsgCtrlResumeAck* pvalue)
{
   asn1Print_EnumCtrlErrorCode (name, pvalue);
}

void ASN1C_MsgCtrlResumeAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgCtrlResumeAck (name, &msgData);
}

void asn1Print_MsgCtrlChangeTimeFactor (ASN1ConstCharPtr name, ASN1T_MsgCtrlChangeTimeFactor* pvalue)
{
   rtPrintInteger (name, *pvalue);
}

void ASN1C_MsgCtrlChangeTimeFactor::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgCtrlChangeTimeFactor (name, &msgData);
}

void asn1Print_MsgCtrlChangeTimeFactorAck (ASN1ConstCharPtr name, ASN1T_MsgCtrlChangeTimeFactorAck* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.time_factor", name);
   rtPrintInteger (namebuf, pvalue->time_factor);

   sprintf (namebuf, "%s.error_code", name);
   asn1Print_EnumCtrlErrorCode (namebuf, &pvalue->error_code);

}

void ASN1C_MsgCtrlChangeTimeFactorAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgCtrlChangeTimeFactorAck (name, &msgData);
}

void asn1Print_MsgCtrlBeginTick (ASN1ConstCharPtr name, ASN1T_MsgCtrlBeginTick* pvalue)
{
   rtPrintInteger (name, *pvalue);
}

void ASN1C_MsgCtrlBeginTick::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgCtrlBeginTick (name, &msgData);
}

void asn1Print_MsgCtrlEndTick (ASN1ConstCharPtr name, ASN1T_MsgCtrlEndTick* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.current_tick", name);
   rtPrintInteger (namebuf, pvalue->current_tick);

   sprintf (namebuf, "%s.tick_duration", name);
   rtPrintInteger (namebuf, pvalue->tick_duration);

   sprintf (namebuf, "%s.nb_pathfinds_longs", name);
   rtPrintInteger (namebuf, pvalue->nb_pathfinds_longs);

   sprintf (namebuf, "%s.nb_pathfinds_courts", name);
   rtPrintInteger (namebuf, pvalue->nb_pathfinds_courts);

   sprintf (namebuf, "%s.memory", name);
   rtPrintInteger (namebuf, pvalue->memory);

   sprintf (namebuf, "%s.virtual_memory", name);
   rtPrintInteger (namebuf, pvalue->virtual_memory);

}

void ASN1C_MsgCtrlEndTick::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgCtrlEndTick (name, &msgData);
}

void asn1Print_MsgCtrlClientAnnouncement (ASN1ConstCharPtr name)
{
}

void ASN1C_MsgCtrlClientAnnouncement::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgCtrlClientAnnouncement (name);
}

void asn1Print_EnumEtatSim (ASN1ConstCharPtr name, ASN1T_EnumEtatSim* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("running\n"); break;
      case 1: printf ("paused\n"); break;
      case 2: printf ("stopped\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumEtatSim::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumEtatSim (name, &msgData);
}

void asn1Print_MsgCtrlInfo (ASN1ConstCharPtr name, ASN1T_MsgCtrlInfo* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.current_tick", name);
   rtPrintInteger (namebuf, pvalue->current_tick);

   sprintf (namebuf, "%s.tick_duration", name);
   rtPrintInteger (namebuf, pvalue->tick_duration);

   sprintf (namebuf, "%s.time_factor", name);
   rtPrintInteger (namebuf, pvalue->time_factor);

   sprintf (namebuf, "%s.checkpoint_frequence", name);
   rtPrintInteger (namebuf, pvalue->checkpoint_frequence);

   sprintf (namebuf, "%s.etat", name);
   asn1Print_EnumEtatSim (namebuf, &pvalue->etat);

   sprintf (namebuf, "%s.send_vision_cones", name);
   rtPrintBoolean (namebuf, pvalue->send_vision_cones);

   sprintf (namebuf, "%s.profiling_enabled", name);
   rtPrintBoolean (namebuf, pvalue->profiling_enabled);

}

void ASN1C_MsgCtrlInfo::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgCtrlInfo (name, &msgData);
}

void asn1Print_MsgCtrlCheckPointSaveBegin (ASN1ConstCharPtr name)
{
}

void ASN1C_MsgCtrlCheckPointSaveBegin::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgCtrlCheckPointSaveBegin (name);
}

void asn1Print_MsgCtrlCheckPointSaveEnd (ASN1ConstCharPtr name)
{
}

void ASN1C_MsgCtrlCheckPointSaveEnd::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgCtrlCheckPointSaveEnd (name);
}

void asn1Print_MsgCtrlCheckPointLoadBegin (ASN1ConstCharPtr name)
{
}

void ASN1C_MsgCtrlCheckPointLoadBegin::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgCtrlCheckPointLoadBegin (name);
}

void asn1Print_MsgCtrlCheckPointLoadEnd (ASN1ConstCharPtr name)
{
}

void ASN1C_MsgCtrlCheckPointLoadEnd::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgCtrlCheckPointLoadEnd (name);
}

void asn1Print_MsgCtrlCheckPointSetFrequency (ASN1ConstCharPtr name, ASN1T_MsgCtrlCheckPointSetFrequency* pvalue)
{
   rtPrintInteger (name, *pvalue);
}

void ASN1C_MsgCtrlCheckPointSetFrequency::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgCtrlCheckPointSetFrequency (name, &msgData);
}

void asn1Print_MsgCtrlCheckPointSetFrequencyAck (ASN1ConstCharPtr name)
{
}

void ASN1C_MsgCtrlCheckPointSetFrequencyAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgCtrlCheckPointSetFrequencyAck (name);
}

void asn1Print_MsgCtrlCheckPointSaveNow (ASN1ConstCharPtr name, ASN1T_MsgCtrlCheckPointSaveNow* pvalue)
{
   char namebuf[512];

   printf ("%s.m.nomPresent = %d\n", name, (int)pvalue->m.nomPresent);
   if (pvalue->m.nomPresent) {
      sprintf (namebuf, "%s.nom", name);
      rtPrintCharStr (namebuf, pvalue->nom);
   }

}

void ASN1C_MsgCtrlCheckPointSaveNow::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgCtrlCheckPointSaveNow (name, &msgData);
}

void asn1Print_MsgCtrlCheckPointSaveNowAck (ASN1ConstCharPtr name)
{
}

void ASN1C_MsgCtrlCheckPointSaveNowAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgCtrlCheckPointSaveNowAck (name);
}

void asn1Print_MsgCtrlSendCurrentStateBegin (ASN1ConstCharPtr name)
{
}

void ASN1C_MsgCtrlSendCurrentStateBegin::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgCtrlSendCurrentStateBegin (name);
}

void asn1Print_MsgCtrlSendCurrentStateEnd (ASN1ConstCharPtr name)
{
}

void ASN1C_MsgCtrlSendCurrentStateEnd::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgCtrlSendCurrentStateEnd (name);
}

void asn1Print_EnumPrecipitationType (ASN1ConstCharPtr name, ASN1T_EnumPrecipitationType* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("pas_de_precipitation\n"); break;
      case 1: printf ("tempete_sable\n"); break;
      case 2: printf ("brouillard\n"); break;
      case 3: printf ("crachin\n"); break;
      case 4: printf ("pluie\n"); break;
      case 5: printf ("neige\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumPrecipitationType::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumPrecipitationType (name, &msgData);
}

void asn1Print_MeteoAttributs (ASN1ConstCharPtr name, ASN1T_MeteoAttributs* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.temperature", name);
   rtPrintInteger (namebuf, pvalue->temperature);

   sprintf (namebuf, "%s.vitesse_vent", name);
   asn1Print_Vitesse (namebuf, &pvalue->vitesse_vent);

   sprintf (namebuf, "%s.direction_vent", name);
   asn1Print_Direction (namebuf, &pvalue->direction_vent);

   sprintf (namebuf, "%s.plancher_couverture_nuageuse", name);
   asn1Print_Altitude (namebuf, &pvalue->plancher_couverture_nuageuse);

   sprintf (namebuf, "%s.plafond_couverture_nuageuse", name);
   asn1Print_Altitude (namebuf, &pvalue->plafond_couverture_nuageuse);

   sprintf (namebuf, "%s.densite_moyenne_couverture_nuageuse", name);
   asn1Print_Pourcentage (namebuf, &pvalue->densite_moyenne_couverture_nuageuse);

   sprintf (namebuf, "%s.precipitation", name);
   asn1Print_EnumPrecipitationType (namebuf, &pvalue->precipitation);

}

void ASN1C_MeteoAttributs::Print (ASN1ConstCharPtr name)

{
   asn1Print_MeteoAttributs (name, &msgData);
}

void asn1Print_MsgCtrlMeteoGlobale (ASN1ConstCharPtr name, ASN1T_MsgCtrlMeteoGlobale* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.attributs", name);
   asn1Print_MeteoAttributs (namebuf, &pvalue->attributs);

}

void ASN1C_MsgCtrlMeteoGlobale::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgCtrlMeteoGlobale (name, &msgData);
}

void asn1Print_MsgCtrlMeteoLocale_meteo (ASN1ConstCharPtr name, ASN1T_MsgCtrlMeteoLocale_meteo* pvalue)
{
   char namebuf[512];

   printf ("%s.t = %d\n", name, pvalue->t);
   switch (pvalue->t) {
      case 1:
         sprintf (namebuf, "%s.u.attributs", name);
         asn1Print_MeteoAttributs (namebuf, pvalue->u.attributs);
         break;

      case 2:
         sprintf (namebuf, "%s.u.globale", name);
         break;

      default:;
   }
}

void ASN1C_MsgCtrlMeteoLocale_meteo::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgCtrlMeteoLocale_meteo (name, &msgData);
}

void asn1Print_MsgCtrlMeteoLocale (ASN1ConstCharPtr name, ASN1T_MsgCtrlMeteoLocale* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.rect_point_haut_gauche", name);
   asn1Print_CoordUTM (namebuf, &pvalue->rect_point_haut_gauche);

   sprintf (namebuf, "%s.rect_point_bas_droite", name);
   asn1Print_CoordUTM (namebuf, &pvalue->rect_point_bas_droite);

   sprintf (namebuf, "%s.meteo", name);
   asn1Print_MsgCtrlMeteoLocale_meteo (namebuf, &pvalue->meteo);

}

void ASN1C_MsgCtrlMeteoLocale::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgCtrlMeteoLocale (name, &msgData);
}

void asn1Print_MsgCtrlMeteoGlobaleAck (ASN1ConstCharPtr name)
{
}

void ASN1C_MsgCtrlMeteoGlobaleAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgCtrlMeteoGlobaleAck (name);
}

void asn1Print_MsgCtrlMeteoLocaleAck (ASN1ConstCharPtr name)
{
}

void ASN1C_MsgCtrlMeteoLocaleAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgCtrlMeteoLocaleAck (name);
}


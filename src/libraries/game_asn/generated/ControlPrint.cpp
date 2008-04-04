/* This file was generated by ASN1C V5.31 on 11-Oct-2007 */

#include "Control.h"

void asn1Print_ControlIncludes (ASN1ConstCharPtr name, ASN1T_ControlIncludes* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.baseExports", name);

}

void ASN1C_ControlIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_ControlIncludes (name, &msgData);
}

void asn1Print_ControlExports (ASN1ConstCharPtr name)
{
}

void ASN1C_ControlExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_ControlExports (name);
}

void asn1Print_EnumControlErrorCode (ASN1ConstCharPtr name, ASN1T_EnumControlErrorCode* pvalue)
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

void ASN1C_EnumControlErrorCode::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumControlErrorCode (name, &msgData);
}

void asn1Print_MsgControlStop (ASN1ConstCharPtr name)
{
}

void ASN1C_MsgControlStop::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlStop (name);
}

void asn1Print_MsgControlStopAck (ASN1ConstCharPtr name, ASN1T_MsgControlStopAck* pvalue)
{
   asn1Print_EnumControlErrorCode (name, pvalue);
}

void ASN1C_MsgControlStopAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlStopAck (name, &msgData);
}

void asn1Print_MsgControlPause (ASN1ConstCharPtr name)
{
}

void ASN1C_MsgControlPause::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlPause (name);
}

void asn1Print_MsgControlPauseAck (ASN1ConstCharPtr name, ASN1T_MsgControlPauseAck* pvalue)
{
   asn1Print_EnumControlErrorCode (name, pvalue);
}

void ASN1C_MsgControlPauseAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlPauseAck (name, &msgData);
}

void asn1Print_MsgControlResume (ASN1ConstCharPtr name)
{
}

void ASN1C_MsgControlResume::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlResume (name);
}

void asn1Print_MsgControlResumeAck (ASN1ConstCharPtr name, ASN1T_MsgControlResumeAck* pvalue)
{
   asn1Print_EnumControlErrorCode (name, pvalue);
}

void ASN1C_MsgControlResumeAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlResumeAck (name, &msgData);
}

void asn1Print_MsgControlSkipToTick (ASN1ConstCharPtr name, ASN1T_MsgControlSkipToTick* pvalue)
{
   rtPrintInteger (name, *pvalue);
}

void ASN1C_MsgControlSkipToTick::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlSkipToTick (name, &msgData);
}

void asn1Print_MsgControlSkipToTickAck (ASN1ConstCharPtr name, ASN1T_MsgControlSkipToTickAck* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.tick", name);
   rtPrintInteger (namebuf, pvalue->tick);

   sprintf (namebuf, "%s.error_code", name);
   asn1Print_EnumControlErrorCode (namebuf, &pvalue->error_code);

}

void ASN1C_MsgControlSkipToTickAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlSkipToTickAck (name, &msgData);
}

void asn1Print_MsgControlToggleVisionCones (ASN1ConstCharPtr name, ASN1T_MsgControlToggleVisionCones* pvalue)
{
   rtPrintBoolean (name, *pvalue);
}

void ASN1C_MsgControlToggleVisionCones::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlToggleVisionCones (name, &msgData);
}

void asn1Print_MsgControlChangeTimeFactor (ASN1ConstCharPtr name, ASN1T_MsgControlChangeTimeFactor* pvalue)
{
   rtPrintInteger (name, *pvalue);
}

void ASN1C_MsgControlChangeTimeFactor::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlChangeTimeFactor (name, &msgData);
}

void asn1Print_MsgControlChangeTimeFactorAck (ASN1ConstCharPtr name, ASN1T_MsgControlChangeTimeFactorAck* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.time_factor", name);
   rtPrintInteger (namebuf, pvalue->time_factor);

   sprintf (namebuf, "%s.error_code", name);
   asn1Print_EnumControlErrorCode (namebuf, &pvalue->error_code);

}

void ASN1C_MsgControlChangeTimeFactorAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlChangeTimeFactorAck (name, &msgData);
}

void asn1Print_MsgControlBeginTick (ASN1ConstCharPtr name, ASN1T_MsgControlBeginTick* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.current_tick", name);
   rtPrintInteger (namebuf, pvalue->current_tick);

   sprintf (namebuf, "%s.date_time", name);
   asn1Print_DateTime (namebuf, &pvalue->date_time);

}

void ASN1C_MsgControlBeginTick::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlBeginTick (name, &msgData);
}

void asn1Print_MsgControlEndTick (ASN1ConstCharPtr name, ASN1T_MsgControlEndTick* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.current_tick", name);
   rtPrintInteger (namebuf, pvalue->current_tick);

   sprintf (namebuf, "%s.tick_duration", name);
   rtPrintInteger (namebuf, pvalue->tick_duration);

   sprintf (namebuf, "%s.long_pathfinds", name);
   rtPrintInteger (namebuf, pvalue->long_pathfinds);

   sprintf (namebuf, "%s.short_pathfinds", name);
   rtPrintInteger (namebuf, pvalue->short_pathfinds);

   sprintf (namebuf, "%s.memory", name);
   rtPrintInteger (namebuf, pvalue->memory);

   sprintf (namebuf, "%s.virtual_memory", name);
   rtPrintInteger (namebuf, pvalue->virtual_memory);

}

void ASN1C_MsgControlEndTick::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlEndTick (name, &msgData);
}

void asn1Print_MsgControlClientAnnouncement (ASN1ConstCharPtr name)
{
}

void ASN1C_MsgControlClientAnnouncement::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlClientAnnouncement (name);
}

void asn1Print_EnumSimulationState (ASN1ConstCharPtr name, ASN1T_EnumSimulationState* pvalue)
{
   printf ("%s = ", name);
   switch (*pvalue) {
      case 0: printf ("running\n"); break;
      case 1: printf ("paused\n"); break;
      case 2: printf ("stopped\n"); break;
      default: printf ("???\n");
   }
}

void ASN1C_EnumSimulationState::Print (ASN1ConstCharPtr name)

{
   asn1Print_EnumSimulationState (name, &msgData);
}

void asn1Print_MsgControlInformation (ASN1ConstCharPtr name, ASN1T_MsgControlInformation* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.current_tick", name);
   rtPrintInteger (namebuf, pvalue->current_tick);

   sprintf (namebuf, "%s.date_time", name);
   asn1Print_DateTime (namebuf, &pvalue->date_time);

   sprintf (namebuf, "%s.tick_duration", name);
   rtPrintInteger (namebuf, pvalue->tick_duration);

   sprintf (namebuf, "%s.time_factor", name);
   rtPrintInteger (namebuf, pvalue->time_factor);

   sprintf (namebuf, "%s.checkpoint_frequency", name);
   rtPrintInteger (namebuf, pvalue->checkpoint_frequency);

   sprintf (namebuf, "%s.status", name);
   asn1Print_EnumSimulationState (namebuf, &pvalue->status);

   sprintf (namebuf, "%s.send_vision_cones", name);
   rtPrintBoolean (namebuf, pvalue->send_vision_cones);

   sprintf (namebuf, "%s.profiling_enabled", name);
   rtPrintBoolean (namebuf, pvalue->profiling_enabled);

}

void ASN1C_MsgControlInformation::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlInformation (name, &msgData);
}

void asn1Print_MsgControlReplayInformation (ASN1ConstCharPtr name, ASN1T_MsgControlReplayInformation* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.current_tick", name);
   rtPrintInteger (namebuf, pvalue->current_tick);

   sprintf (namebuf, "%s.date_time", name);
   asn1Print_DateTime (namebuf, &pvalue->date_time);

   sprintf (namebuf, "%s.tick_duration", name);
   rtPrintInteger (namebuf, pvalue->tick_duration);

   sprintf (namebuf, "%s.time_factor", name);
   rtPrintInteger (namebuf, pvalue->time_factor);

   sprintf (namebuf, "%s.status", name);
   asn1Print_EnumSimulationState (namebuf, &pvalue->status);

   sprintf (namebuf, "%s.tick_count", name);
   rtPrintInteger (namebuf, pvalue->tick_count);

}

void ASN1C_MsgControlReplayInformation::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlReplayInformation (name, &msgData);
}

void asn1Print_MsgControlProfilingInformation (ASN1ConstCharPtr name, ASN1T_MsgControlProfilingInformation* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.perception", name);
   rtPrintReal (namebuf, pvalue->perception);

   sprintf (namebuf, "%s.decision", name);
   rtPrintReal (namebuf, pvalue->decision);

   sprintf (namebuf, "%s.action", name);
   rtPrintReal (namebuf, pvalue->action);

   sprintf (namebuf, "%s.main_loop", name);
   rtPrintReal (namebuf, pvalue->main_loop);

}

void ASN1C_MsgControlProfilingInformation::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlProfilingInformation (name, &msgData);
}

void asn1Print_MsgControlCheckPointSaveBegin (ASN1ConstCharPtr name)
{
}

void ASN1C_MsgControlCheckPointSaveBegin::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlCheckPointSaveBegin (name);
}

void asn1Print_MsgControlCheckPointSaveEnd (ASN1ConstCharPtr name)
{
}

void ASN1C_MsgControlCheckPointSaveEnd::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlCheckPointSaveEnd (name);
}

void asn1Print_MsgControlCheckPointLoadBegin (ASN1ConstCharPtr name)
{
}

void ASN1C_MsgControlCheckPointLoadBegin::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlCheckPointLoadBegin (name);
}

void asn1Print_MsgControlCheckPointLoadEnd (ASN1ConstCharPtr name)
{
}

void ASN1C_MsgControlCheckPointLoadEnd::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlCheckPointLoadEnd (name);
}

void asn1Print_MsgControlCheckPointSetFrequency (ASN1ConstCharPtr name, ASN1T_MsgControlCheckPointSetFrequency* pvalue)
{
   rtPrintInteger (name, *pvalue);
}

void ASN1C_MsgControlCheckPointSetFrequency::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlCheckPointSetFrequency (name, &msgData);
}

void asn1Print_MsgControlCheckPointSetFrequencyAck (ASN1ConstCharPtr name)
{
}

void ASN1C_MsgControlCheckPointSetFrequencyAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlCheckPointSetFrequencyAck (name);
}

void asn1Print_MsgControlCheckPointSaveNow (ASN1ConstCharPtr name, ASN1T_MsgControlCheckPointSaveNow* pvalue)
{
   char namebuf[512];

   printf ("%s.m.namePresent = %d\n", name, (int)pvalue->m.namePresent);
   if (pvalue->m.namePresent) {
      sprintf (namebuf, "%s.name", name);
      rtPrintCharStr (namebuf, pvalue->name);
   }

}

void ASN1C_MsgControlCheckPointSaveNow::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlCheckPointSaveNow (name, &msgData);
}

void asn1Print_MsgControlCheckPointSaveNowAck (ASN1ConstCharPtr name)
{
}

void ASN1C_MsgControlCheckPointSaveNowAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlCheckPointSaveNowAck (name);
}

void asn1Print_MsgControlSendCurrentStateBegin (ASN1ConstCharPtr name)
{
}

void ASN1C_MsgControlSendCurrentStateBegin::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlSendCurrentStateBegin (name);
}

void asn1Print_MsgControlSendCurrentStateEnd (ASN1ConstCharPtr name)
{
}

void ASN1C_MsgControlSendCurrentStateEnd::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlSendCurrentStateEnd (name);
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

void asn1Print_MeteoAttributes (ASN1ConstCharPtr name, ASN1T_MeteoAttributes* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.temperature", name);
   rtPrintInteger (namebuf, pvalue->temperature);

   sprintf (namebuf, "%s.wind_speed", name);
   asn1Print_Speed (namebuf, &pvalue->wind_speed);

   sprintf (namebuf, "%s.wind_direction", name);
   asn1Print_Heading (namebuf, &pvalue->wind_direction);

   sprintf (namebuf, "%s.cloud_floor", name);
   asn1Print_Elevation (namebuf, &pvalue->cloud_floor);

   sprintf (namebuf, "%s.cloud_ceiling", name);
   asn1Print_Elevation (namebuf, &pvalue->cloud_ceiling);

   sprintf (namebuf, "%s.cloud_density", name);
   asn1Print_Percentage (namebuf, &pvalue->cloud_density);

   sprintf (namebuf, "%s.precipitation", name);
   asn1Print_EnumPrecipitationType (namebuf, &pvalue->precipitation);

}

void ASN1C_MeteoAttributes::Print (ASN1ConstCharPtr name)

{
   asn1Print_MeteoAttributes (name, &msgData);
}

void asn1Print_MsgControlGlobalMeteo (ASN1ConstCharPtr name, ASN1T_MsgControlGlobalMeteo* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.attributes", name);
   asn1Print_MeteoAttributes (namebuf, &pvalue->attributes);

}

void ASN1C_MsgControlGlobalMeteo::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlGlobalMeteo (name, &msgData);
}

void asn1Print_MsgControlLocalMeteo_meteo (ASN1ConstCharPtr name, ASN1T_MsgControlLocalMeteo_meteo* pvalue)
{
   char namebuf[512];

   printf ("%s.t = %d\n", name, pvalue->t);
   switch (pvalue->t) {
      case 1:
         sprintf (namebuf, "%s.u.attributes", name);
         asn1Print_MeteoAttributes (namebuf, pvalue->u.attributes);
         break;

      case 2:
         sprintf (namebuf, "%s.u.global", name);
         break;

      default:;
   }
}

void ASN1C_MsgControlLocalMeteo_meteo::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlLocalMeteo_meteo (name, &msgData);
}

void asn1Print_MsgControlLocalMeteo (ASN1ConstCharPtr name, ASN1T_MsgControlLocalMeteo* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.top_left_coordinate", name);
   asn1Print_CoordLatLong (namebuf, &pvalue->top_left_coordinate);

   sprintf (namebuf, "%s.bottom_right_coordinate", name);
   asn1Print_CoordLatLong (namebuf, &pvalue->bottom_right_coordinate);

   sprintf (namebuf, "%s.meteo", name);
   asn1Print_MsgControlLocalMeteo_meteo (namebuf, &pvalue->meteo);

}

void ASN1C_MsgControlLocalMeteo::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlLocalMeteo (name, &msgData);
}

void asn1Print_MsgControlGlobalMeteoAck (ASN1ConstCharPtr name)
{
}

void ASN1C_MsgControlGlobalMeteoAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlGlobalMeteoAck (name);
}

void asn1Print_MsgControlLocalMeteoAck (ASN1ConstCharPtr name)
{
}

void ASN1C_MsgControlLocalMeteoAck::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgControlLocalMeteoAck (name);
}



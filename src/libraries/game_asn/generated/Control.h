/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 11-Oct-2007.
 */
#ifndef CONTROL_H
#define CONTROL_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "Base.h"

/**************************************************************/
/*                                                            */
/*  ControlIncludes                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ControlIncludes {
} ASN1T_ControlIncludes;

class EXTERN ASN1C_ControlIncludes : public ASN1CType {
public:
   ASN1T_ControlIncludes& msgData;
   ASN1C_ControlIncludes (
      ASN1MessageBuffer& msgBuf, ASN1T_ControlIncludes& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ControlIncludes (ASN1CTXT* ctxt_p, ASN1T_ControlIncludes* pvalue);
EXTERN int asn1PD_ControlIncludes (ASN1CTXT* ctxt_p, ASN1T_ControlIncludes* pvalue);

EXTERN void asn1Print_ControlIncludes (ASN1ConstCharPtr name, ASN1T_ControlIncludes* pvalue);

/**************************************************************/
/*                                                            */
/*  ControlExports                                            */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_ControlExports : public ASN1CType {
public:
   ASN1C_ControlExports (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ControlExports (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_ControlExports (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_ControlExports (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  EnumControlErrorCode                                      */
/*                                                            */
/**************************************************************/

struct EXTERN EnumControlErrorCode {
   enum Root {
      no_error = 0,
      error_already_started = 1,
      error_not_started = 2,
      error_not_paused = 3,
      error_already_paused = 4,
      error_invalid_time_factor = 5
   } ;
} ;

typedef EnumControlErrorCode::Root ASN1T_EnumControlErrorCode;

class EXTERN ASN1C_EnumControlErrorCode : public ASN1CType {
public:
   ASN1T_EnumControlErrorCode& msgData;
   ASN1C_EnumControlErrorCode (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumControlErrorCode& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumControlErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumControlErrorCode value);
EXTERN int asn1PD_EnumControlErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumControlErrorCode* pvalue);

EXTERN void asn1Print_EnumControlErrorCode (ASN1ConstCharPtr name, ASN1T_EnumControlErrorCode* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgControlStop                                            */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgControlStop : public ASN1CType {
public:
   ASN1C_MsgControlStop (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlStop (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgControlStop (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgControlStop (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgControlStopAck                                         */
/*                                                            */
/**************************************************************/

typedef ASN1T_EnumControlErrorCode ASN1T_MsgControlStopAck;

class EXTERN ASN1C_MsgControlStopAck : public ASN1CType {
public:
   ASN1T_MsgControlStopAck& msgData;
   ASN1C_MsgControlStopAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgControlStopAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlStopAck (ASN1CTXT* ctxt_p, ASN1T_MsgControlStopAck value);
EXTERN int asn1PD_MsgControlStopAck (ASN1CTXT* ctxt_p, ASN1T_MsgControlStopAck* pvalue);

EXTERN void asn1Print_MsgControlStopAck (ASN1ConstCharPtr name, ASN1T_MsgControlStopAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgControlPause                                           */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgControlPause : public ASN1CType {
public:
   ASN1C_MsgControlPause (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlPause (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgControlPause (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgControlPause (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgControlPauseAck                                        */
/*                                                            */
/**************************************************************/

typedef ASN1T_EnumControlErrorCode ASN1T_MsgControlPauseAck;

class EXTERN ASN1C_MsgControlPauseAck : public ASN1CType {
public:
   ASN1T_MsgControlPauseAck& msgData;
   ASN1C_MsgControlPauseAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgControlPauseAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlPauseAck (ASN1CTXT* ctxt_p, ASN1T_MsgControlPauseAck value);
EXTERN int asn1PD_MsgControlPauseAck (ASN1CTXT* ctxt_p, ASN1T_MsgControlPauseAck* pvalue);

EXTERN void asn1Print_MsgControlPauseAck (ASN1ConstCharPtr name, ASN1T_MsgControlPauseAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgControlResume                                          */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgControlResume : public ASN1CType {
public:
   ASN1C_MsgControlResume (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlResume (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgControlResume (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgControlResume (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgControlResumeAck                                       */
/*                                                            */
/**************************************************************/

typedef ASN1T_EnumControlErrorCode ASN1T_MsgControlResumeAck;

class EXTERN ASN1C_MsgControlResumeAck : public ASN1CType {
public:
   ASN1T_MsgControlResumeAck& msgData;
   ASN1C_MsgControlResumeAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgControlResumeAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlResumeAck (ASN1CTXT* ctxt_p, ASN1T_MsgControlResumeAck value);
EXTERN int asn1PD_MsgControlResumeAck (ASN1CTXT* ctxt_p, ASN1T_MsgControlResumeAck* pvalue);

EXTERN void asn1Print_MsgControlResumeAck (ASN1ConstCharPtr name, ASN1T_MsgControlResumeAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgControlSkipToTick                                      */
/*                                                            */
/**************************************************************/

typedef ASN1INT  ASN1T_MsgControlSkipToTick;

class EXTERN ASN1C_MsgControlSkipToTick : public ASN1CType {
public:
   ASN1T_MsgControlSkipToTick& msgData;
   ASN1C_MsgControlSkipToTick (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgControlSkipToTick& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlSkipToTick (ASN1CTXT* ctxt_p, ASN1T_MsgControlSkipToTick value);
EXTERN int asn1PD_MsgControlSkipToTick (ASN1CTXT* ctxt_p, ASN1T_MsgControlSkipToTick* pvalue);

EXTERN void asn1Print_MsgControlSkipToTick (ASN1ConstCharPtr name, ASN1T_MsgControlSkipToTick* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgControlSkipToTickAck                                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgControlSkipToTickAck {
   ASN1INT   tick;
   ASN1T_EnumControlErrorCode  error_code;
} ASN1T_MsgControlSkipToTickAck;

class EXTERN ASN1C_MsgControlSkipToTickAck : public ASN1CType {
public:
   ASN1T_MsgControlSkipToTickAck& msgData;
   ASN1C_MsgControlSkipToTickAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgControlSkipToTickAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlSkipToTickAck (ASN1CTXT* ctxt_p, ASN1T_MsgControlSkipToTickAck* pvalue);
EXTERN int asn1PD_MsgControlSkipToTickAck (ASN1CTXT* ctxt_p, ASN1T_MsgControlSkipToTickAck* pvalue);

EXTERN void asn1Print_MsgControlSkipToTickAck (ASN1ConstCharPtr name, ASN1T_MsgControlSkipToTickAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgControlToggleVisionCones                               */
/*                                                            */
/**************************************************************/

typedef ASN1BOOL ASN1T_MsgControlToggleVisionCones;

class EXTERN ASN1C_MsgControlToggleVisionCones : public ASN1CType {
public:
   ASN1T_MsgControlToggleVisionCones& msgData;
   ASN1C_MsgControlToggleVisionCones (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgControlToggleVisionCones& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlToggleVisionCones (ASN1CTXT* ctxt_p, ASN1T_MsgControlToggleVisionCones value);
EXTERN int asn1PD_MsgControlToggleVisionCones (ASN1CTXT* ctxt_p, ASN1T_MsgControlToggleVisionCones* pvalue);

EXTERN void asn1Print_MsgControlToggleVisionCones (ASN1ConstCharPtr name, ASN1T_MsgControlToggleVisionCones* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgControlChangeTimeFactor                                */
/*                                                            */
/**************************************************************/

typedef ASN1INT  ASN1T_MsgControlChangeTimeFactor;

class EXTERN ASN1C_MsgControlChangeTimeFactor : public ASN1CType {
public:
   ASN1T_MsgControlChangeTimeFactor& msgData;
   ASN1C_MsgControlChangeTimeFactor (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgControlChangeTimeFactor& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlChangeTimeFactor (ASN1CTXT* ctxt_p, ASN1T_MsgControlChangeTimeFactor value);
EXTERN int asn1PD_MsgControlChangeTimeFactor (ASN1CTXT* ctxt_p, ASN1T_MsgControlChangeTimeFactor* pvalue);

EXTERN void asn1Print_MsgControlChangeTimeFactor (ASN1ConstCharPtr name, ASN1T_MsgControlChangeTimeFactor* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgControlChangeTimeFactorAck                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgControlChangeTimeFactorAck {
   ASN1INT   time_factor;
   ASN1T_EnumControlErrorCode  error_code;
} ASN1T_MsgControlChangeTimeFactorAck;

class EXTERN ASN1C_MsgControlChangeTimeFactorAck : public ASN1CType {
public:
   ASN1T_MsgControlChangeTimeFactorAck& msgData;
   ASN1C_MsgControlChangeTimeFactorAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgControlChangeTimeFactorAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlChangeTimeFactorAck (ASN1CTXT* ctxt_p, ASN1T_MsgControlChangeTimeFactorAck* pvalue);
EXTERN int asn1PD_MsgControlChangeTimeFactorAck (ASN1CTXT* ctxt_p, ASN1T_MsgControlChangeTimeFactorAck* pvalue);

EXTERN void asn1Print_MsgControlChangeTimeFactorAck (ASN1ConstCharPtr name, ASN1T_MsgControlChangeTimeFactorAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgControlBeginTick                                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgControlBeginTick {
   ASN1INT   current_tick;
   ASN1T_DateTime  date_time;
} ASN1T_MsgControlBeginTick;

class EXTERN ASN1C_MsgControlBeginTick : public ASN1CType {
public:
   ASN1T_MsgControlBeginTick& msgData;
   ASN1C_MsgControlBeginTick (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgControlBeginTick& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlBeginTick (ASN1CTXT* ctxt_p, ASN1T_MsgControlBeginTick* pvalue);
EXTERN int asn1PD_MsgControlBeginTick (ASN1CTXT* ctxt_p, ASN1T_MsgControlBeginTick* pvalue);

EXTERN void asn1Print_MsgControlBeginTick (ASN1ConstCharPtr name, ASN1T_MsgControlBeginTick* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgControlEndTick                                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgControlEndTick {
   ASN1INT   current_tick;
   ASN1INT   tick_duration;
   ASN1INT   long_pathfinds;
   ASN1INT   short_pathfinds;
   ASN1INT   memory;
   ASN1INT   virtual_memory;
} ASN1T_MsgControlEndTick;

class EXTERN ASN1C_MsgControlEndTick : public ASN1CType {
public:
   ASN1T_MsgControlEndTick& msgData;
   ASN1C_MsgControlEndTick (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgControlEndTick& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlEndTick (ASN1CTXT* ctxt_p, ASN1T_MsgControlEndTick* pvalue);
EXTERN int asn1PD_MsgControlEndTick (ASN1CTXT* ctxt_p, ASN1T_MsgControlEndTick* pvalue);

EXTERN void asn1Print_MsgControlEndTick (ASN1ConstCharPtr name, ASN1T_MsgControlEndTick* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgControlClientAnnouncement                              */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgControlClientAnnouncement : public ASN1CType {
public:
   ASN1C_MsgControlClientAnnouncement (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlClientAnnouncement (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgControlClientAnnouncement (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgControlClientAnnouncement (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  EnumSimulationState                                       */
/*                                                            */
/**************************************************************/

struct EXTERN EnumSimulationState {
   enum Root {
      running = 0,
      paused = 1,
      stopped = 2
   } ;
} ;

typedef EnumSimulationState::Root ASN1T_EnumSimulationState;

class EXTERN ASN1C_EnumSimulationState : public ASN1CType {
public:
   ASN1T_EnumSimulationState& msgData;
   ASN1C_EnumSimulationState (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumSimulationState& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumSimulationState (ASN1CTXT* ctxt_p, ASN1T_EnumSimulationState value);
EXTERN int asn1PD_EnumSimulationState (ASN1CTXT* ctxt_p, ASN1T_EnumSimulationState* pvalue);

EXTERN void asn1Print_EnumSimulationState (ASN1ConstCharPtr name, ASN1T_EnumSimulationState* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgControlInformation                                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgControlInformation {
   ASN1INT   current_tick;
   ASN1T_DateTime  date_time;
   ASN1INT   tick_duration;
   ASN1INT   time_factor;
   ASN1INT   checkpoint_frequency;
   ASN1T_EnumSimulationState  status;
   ASN1BOOL  send_vision_cones;
   ASN1BOOL  profiling_enabled;
} ASN1T_MsgControlInformation;

class EXTERN ASN1C_MsgControlInformation : public ASN1CType {
public:
   ASN1T_MsgControlInformation& msgData;
   ASN1C_MsgControlInformation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgControlInformation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlInformation (ASN1CTXT* ctxt_p, ASN1T_MsgControlInformation* pvalue);
EXTERN int asn1PD_MsgControlInformation (ASN1CTXT* ctxt_p, ASN1T_MsgControlInformation* pvalue);

EXTERN void asn1Print_MsgControlInformation (ASN1ConstCharPtr name, ASN1T_MsgControlInformation* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgControlReplayInformation                               */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgControlReplayInformation {
   ASN1INT   current_tick;
   ASN1T_DateTime  date_time;
   ASN1INT   tick_duration;
   ASN1INT   time_factor;
   ASN1T_EnumSimulationState  status;
   ASN1INT   tick_count;
} ASN1T_MsgControlReplayInformation;

class EXTERN ASN1C_MsgControlReplayInformation : public ASN1CType {
public:
   ASN1T_MsgControlReplayInformation& msgData;
   ASN1C_MsgControlReplayInformation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgControlReplayInformation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlReplayInformation (ASN1CTXT* ctxt_p, ASN1T_MsgControlReplayInformation* pvalue);
EXTERN int asn1PD_MsgControlReplayInformation (ASN1CTXT* ctxt_p, ASN1T_MsgControlReplayInformation* pvalue);

EXTERN void asn1Print_MsgControlReplayInformation (ASN1ConstCharPtr name, ASN1T_MsgControlReplayInformation* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgControlProfilingInformation                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgControlProfilingInformation {
   ASN1REAL  perception;
   ASN1REAL  decision;
   ASN1REAL  action;
   ASN1REAL  main_loop;
} ASN1T_MsgControlProfilingInformation;

class EXTERN ASN1C_MsgControlProfilingInformation : public ASN1CType {
public:
   ASN1T_MsgControlProfilingInformation& msgData;
   ASN1C_MsgControlProfilingInformation (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgControlProfilingInformation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlProfilingInformation (ASN1CTXT* ctxt_p, ASN1T_MsgControlProfilingInformation* pvalue);
EXTERN int asn1PD_MsgControlProfilingInformation (ASN1CTXT* ctxt_p, ASN1T_MsgControlProfilingInformation* pvalue);

EXTERN void asn1Print_MsgControlProfilingInformation (ASN1ConstCharPtr name, ASN1T_MsgControlProfilingInformation* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgControlCheckPointSaveBegin                             */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgControlCheckPointSaveBegin : public ASN1CType {
public:
   ASN1C_MsgControlCheckPointSaveBegin (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlCheckPointSaveBegin (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgControlCheckPointSaveBegin (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgControlCheckPointSaveBegin (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgControlCheckPointSaveEnd                               */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgControlCheckPointSaveEnd : public ASN1CType {
public:
   ASN1C_MsgControlCheckPointSaveEnd (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlCheckPointSaveEnd (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgControlCheckPointSaveEnd (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgControlCheckPointSaveEnd (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgControlCheckPointLoadBegin                             */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgControlCheckPointLoadBegin : public ASN1CType {
public:
   ASN1C_MsgControlCheckPointLoadBegin (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlCheckPointLoadBegin (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgControlCheckPointLoadBegin (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgControlCheckPointLoadBegin (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgControlCheckPointLoadEnd                               */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgControlCheckPointLoadEnd : public ASN1CType {
public:
   ASN1C_MsgControlCheckPointLoadEnd (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlCheckPointLoadEnd (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgControlCheckPointLoadEnd (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgControlCheckPointLoadEnd (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgControlCheckPointSetFrequency                          */
/*                                                            */
/**************************************************************/

typedef ASN1INT  ASN1T_MsgControlCheckPointSetFrequency;

class EXTERN ASN1C_MsgControlCheckPointSetFrequency : public ASN1CType {
public:
   ASN1T_MsgControlCheckPointSetFrequency& msgData;
   ASN1C_MsgControlCheckPointSetFrequency (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgControlCheckPointSetFrequency& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlCheckPointSetFrequency (ASN1CTXT* ctxt_p, ASN1T_MsgControlCheckPointSetFrequency value);
EXTERN int asn1PD_MsgControlCheckPointSetFrequency (ASN1CTXT* ctxt_p, ASN1T_MsgControlCheckPointSetFrequency* pvalue);

EXTERN void asn1Print_MsgControlCheckPointSetFrequency (ASN1ConstCharPtr name, ASN1T_MsgControlCheckPointSetFrequency* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgControlCheckPointSetFrequencyAck                       */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgControlCheckPointSetFrequencyAck : public ASN1CType {
public:
   ASN1C_MsgControlCheckPointSetFrequencyAck (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlCheckPointSetFrequencyAck (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgControlCheckPointSetFrequencyAck (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgControlCheckPointSetFrequencyAck (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgControlCheckPointSaveNow                               */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgControlCheckPointSaveNow {
   struct {
      unsigned namePresent : 1;
   } m;
   ASN1VisibleString  name;

   ASN1T_MsgControlCheckPointSaveNow () {
      m.namePresent = 0;
   }
} ASN1T_MsgControlCheckPointSaveNow;

class EXTERN ASN1C_MsgControlCheckPointSaveNow : public ASN1CType {
public:
   ASN1T_MsgControlCheckPointSaveNow& msgData;
   ASN1C_MsgControlCheckPointSaveNow (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgControlCheckPointSaveNow& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlCheckPointSaveNow (ASN1CTXT* ctxt_p, ASN1T_MsgControlCheckPointSaveNow* pvalue);
EXTERN int asn1PD_MsgControlCheckPointSaveNow (ASN1CTXT* ctxt_p, ASN1T_MsgControlCheckPointSaveNow* pvalue);

EXTERN void asn1Print_MsgControlCheckPointSaveNow (ASN1ConstCharPtr name, ASN1T_MsgControlCheckPointSaveNow* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgControlCheckPointSaveNowAck                            */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgControlCheckPointSaveNowAck : public ASN1CType {
public:
   ASN1C_MsgControlCheckPointSaveNowAck (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlCheckPointSaveNowAck (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgControlCheckPointSaveNowAck (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgControlCheckPointSaveNowAck (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgControlSendCurrentStateBegin                           */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgControlSendCurrentStateBegin : public ASN1CType {
public:
   ASN1C_MsgControlSendCurrentStateBegin (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlSendCurrentStateBegin (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgControlSendCurrentStateBegin (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgControlSendCurrentStateBegin (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgControlSendCurrentStateEnd                             */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgControlSendCurrentStateEnd : public ASN1CType {
public:
   ASN1C_MsgControlSendCurrentStateEnd (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlSendCurrentStateEnd (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgControlSendCurrentStateEnd (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgControlSendCurrentStateEnd (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  EnumPrecipitationType                                     */
/*                                                            */
/**************************************************************/

struct EXTERN EnumPrecipitationType {
   enum Root {
      pas_de_precipitation = 0,
      tempete_sable = 1,
      brouillard = 2,
      crachin = 3,
      pluie = 4,
      neige = 5
   } ;
} ;

typedef EnumPrecipitationType::Root ASN1T_EnumPrecipitationType;

class EXTERN ASN1C_EnumPrecipitationType : public ASN1CType {
public:
   ASN1T_EnumPrecipitationType& msgData;
   ASN1C_EnumPrecipitationType (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumPrecipitationType& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumPrecipitationType (ASN1CTXT* ctxt_p, ASN1T_EnumPrecipitationType value);
EXTERN int asn1PD_EnumPrecipitationType (ASN1CTXT* ctxt_p, ASN1T_EnumPrecipitationType* pvalue);

EXTERN void asn1Print_EnumPrecipitationType (ASN1ConstCharPtr name, ASN1T_EnumPrecipitationType* pvalue);

/**************************************************************/
/*                                                            */
/*  MeteoAttributes                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MeteoAttributes {
   ASN1INT   temperature;
   ASN1T_Speed  wind_speed;
   ASN1T_Heading  wind_direction;
   ASN1T_Elevation  cloud_floor;
   ASN1T_Elevation  cloud_ceiling;
   ASN1T_Percentage  cloud_density;
   ASN1T_EnumPrecipitationType  precipitation;
} ASN1T_MeteoAttributes;

class EXTERN ASN1C_MeteoAttributes : public ASN1CType {
public:
   ASN1T_MeteoAttributes& msgData;
   ASN1C_MeteoAttributes (
      ASN1MessageBuffer& msgBuf, ASN1T_MeteoAttributes& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MeteoAttributes (ASN1CTXT* ctxt_p, ASN1T_MeteoAttributes* pvalue);
EXTERN int asn1PD_MeteoAttributes (ASN1CTXT* ctxt_p, ASN1T_MeteoAttributes* pvalue);

EXTERN void asn1Print_MeteoAttributes (ASN1ConstCharPtr name, ASN1T_MeteoAttributes* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgControlGlobalMeteo                                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgControlGlobalMeteo {
   ASN1T_MeteoAttributes  attributes;
} ASN1T_MsgControlGlobalMeteo;

class EXTERN ASN1C_MsgControlGlobalMeteo : public ASN1CType {
public:
   ASN1T_MsgControlGlobalMeteo& msgData;
   ASN1C_MsgControlGlobalMeteo (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgControlGlobalMeteo& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlGlobalMeteo (ASN1CTXT* ctxt_p, ASN1T_MsgControlGlobalMeteo* pvalue);
EXTERN int asn1PD_MsgControlGlobalMeteo (ASN1CTXT* ctxt_p, ASN1T_MsgControlGlobalMeteo* pvalue);

EXTERN void asn1Print_MsgControlGlobalMeteo (ASN1ConstCharPtr name, ASN1T_MsgControlGlobalMeteo* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgControlLocalMeteo_meteo                                */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_MsgControlLocalMeteo_meteo_attributes 1
#define T_MsgControlLocalMeteo_meteo_global 2

typedef struct EXTERN ASN1T_MsgControlLocalMeteo_meteo {
   int t;
   union {
      /* t = 1 */
      ASN1T_MeteoAttributes *attributes;
      /* t = 2 */
   } u;
} ASN1T_MsgControlLocalMeteo_meteo;

class EXTERN ASN1C_MsgControlLocalMeteo_meteo : public ASN1CType {
public:
   ASN1T_MsgControlLocalMeteo_meteo& msgData;
   ASN1C_MsgControlLocalMeteo_meteo (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgControlLocalMeteo_meteo& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlLocalMeteo_meteo (ASN1CTXT* ctxt_p, ASN1T_MsgControlLocalMeteo_meteo* pvalue);
EXTERN int asn1PD_MsgControlLocalMeteo_meteo (ASN1CTXT* ctxt_p, ASN1T_MsgControlLocalMeteo_meteo* pvalue);

EXTERN void asn1Print_MsgControlLocalMeteo_meteo (ASN1ConstCharPtr name, ASN1T_MsgControlLocalMeteo_meteo* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgControlLocalMeteo                                      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgControlLocalMeteo {
   ASN1T_CoordLatLong  top_left_coordinate;
   ASN1T_CoordLatLong  bottom_right_coordinate;
   ASN1T_MsgControlLocalMeteo_meteo  meteo;
} ASN1T_MsgControlLocalMeteo;

class EXTERN ASN1C_MsgControlLocalMeteo : public ASN1CType {
public:
   ASN1T_MsgControlLocalMeteo& msgData;
   ASN1C_MsgControlLocalMeteo (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgControlLocalMeteo& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlLocalMeteo (ASN1CTXT* ctxt_p, ASN1T_MsgControlLocalMeteo* pvalue);
EXTERN int asn1PD_MsgControlLocalMeteo (ASN1CTXT* ctxt_p, ASN1T_MsgControlLocalMeteo* pvalue);

EXTERN void asn1Print_MsgControlLocalMeteo (ASN1ConstCharPtr name, ASN1T_MsgControlLocalMeteo* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgControlGlobalMeteoAck                                  */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgControlGlobalMeteoAck : public ASN1CType {
public:
   ASN1C_MsgControlGlobalMeteoAck (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlGlobalMeteoAck (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgControlGlobalMeteoAck (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgControlGlobalMeteoAck (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgControlLocalMeteoAck                                   */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgControlLocalMeteoAck : public ASN1CType {
public:
   ASN1C_MsgControlLocalMeteoAck (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgControlLocalMeteoAck (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgControlLocalMeteoAck (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgControlLocalMeteoAck (ASN1ConstCharPtr name);

#endif

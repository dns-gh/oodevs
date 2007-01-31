/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 25-Jan-2007.
 */
#ifndef SCIPIOCONTROL_H
#define SCIPIOCONTROL_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  ScipioControlIncludes                                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ScipioControlIncludes {
} ASN1T_ScipioControlIncludes;

class EXTERN ASN1C_ScipioControlIncludes : public ASN1CType {
public:
   ASN1T_ScipioControlIncludes& msgData;
   ASN1C_ScipioControlIncludes (
      ASN1MessageBuffer& msgBuf, ASN1T_ScipioControlIncludes& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ScipioControlIncludes (ASN1CTXT* ctxt_p, ASN1T_ScipioControlIncludes* pvalue);
EXTERN int asn1PD_ScipioControlIncludes (ASN1CTXT* ctxt_p, ASN1T_ScipioControlIncludes* pvalue);

EXTERN void asn1Print_ScipioControlIncludes (ASN1ConstCharPtr name, ASN1T_ScipioControlIncludes* pvalue);

/**************************************************************/
/*                                                            */
/*  ScipioControlExports                                      */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_ScipioControlExports : public ASN1CType {
public:
   ASN1C_ScipioControlExports (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ScipioControlExports (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_ScipioControlExports (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_ScipioControlExports (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  EnumCtrlErrorCode                                         */
/*                                                            */
/**************************************************************/

struct EXTERN EnumCtrlErrorCode {
   enum Root {
      no_error = 0,
      error_already_started = 1,
      error_not_started = 2,
      error_not_paused = 3,
      error_already_paused = 4,
      error_invalid_time_factor = 5
   } ;
} ;

typedef EnumCtrlErrorCode::Root ASN1T_EnumCtrlErrorCode;

class EXTERN ASN1C_EnumCtrlErrorCode : public ASN1CType {
public:
   ASN1T_EnumCtrlErrorCode& msgData;
   ASN1C_EnumCtrlErrorCode (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumCtrlErrorCode& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumCtrlErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumCtrlErrorCode value);
EXTERN int asn1PD_EnumCtrlErrorCode (ASN1CTXT* ctxt_p, ASN1T_EnumCtrlErrorCode* pvalue);

EXTERN void asn1Print_EnumCtrlErrorCode (ASN1ConstCharPtr name, ASN1T_EnumCtrlErrorCode* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgCtrlStop                                               */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgCtrlStop : public ASN1CType {
public:
   ASN1C_MsgCtrlStop (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgCtrlStop (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgCtrlStop (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgCtrlStop (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgCtrlStopAck                                            */
/*                                                            */
/**************************************************************/

typedef ASN1T_EnumCtrlErrorCode ASN1T_MsgCtrlStopAck;

class EXTERN ASN1C_MsgCtrlStopAck : public ASN1CType {
public:
   ASN1T_MsgCtrlStopAck& msgData;
   ASN1C_MsgCtrlStopAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgCtrlStopAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgCtrlStopAck (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlStopAck value);
EXTERN int asn1PD_MsgCtrlStopAck (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlStopAck* pvalue);

EXTERN void asn1Print_MsgCtrlStopAck (ASN1ConstCharPtr name, ASN1T_MsgCtrlStopAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgCtrlPause                                              */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgCtrlPause : public ASN1CType {
public:
   ASN1C_MsgCtrlPause (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgCtrlPause (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgCtrlPause (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgCtrlPause (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgCtrlPauseAck                                           */
/*                                                            */
/**************************************************************/

typedef ASN1T_EnumCtrlErrorCode ASN1T_MsgCtrlPauseAck;

class EXTERN ASN1C_MsgCtrlPauseAck : public ASN1CType {
public:
   ASN1T_MsgCtrlPauseAck& msgData;
   ASN1C_MsgCtrlPauseAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgCtrlPauseAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgCtrlPauseAck (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlPauseAck value);
EXTERN int asn1PD_MsgCtrlPauseAck (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlPauseAck* pvalue);

EXTERN void asn1Print_MsgCtrlPauseAck (ASN1ConstCharPtr name, ASN1T_MsgCtrlPauseAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgCtrlResume                                             */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgCtrlResume : public ASN1CType {
public:
   ASN1C_MsgCtrlResume (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgCtrlResume (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgCtrlResume (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgCtrlResume (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgCtrlResumeAck                                          */
/*                                                            */
/**************************************************************/

typedef ASN1T_EnumCtrlErrorCode ASN1T_MsgCtrlResumeAck;

class EXTERN ASN1C_MsgCtrlResumeAck : public ASN1CType {
public:
   ASN1T_MsgCtrlResumeAck& msgData;
   ASN1C_MsgCtrlResumeAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgCtrlResumeAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgCtrlResumeAck (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlResumeAck value);
EXTERN int asn1PD_MsgCtrlResumeAck (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlResumeAck* pvalue);

EXTERN void asn1Print_MsgCtrlResumeAck (ASN1ConstCharPtr name, ASN1T_MsgCtrlResumeAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgCtrlChangeTimeFactor                                   */
/*                                                            */
/**************************************************************/

typedef ASN1INT  ASN1T_MsgCtrlChangeTimeFactor;

class EXTERN ASN1C_MsgCtrlChangeTimeFactor : public ASN1CType {
public:
   ASN1T_MsgCtrlChangeTimeFactor& msgData;
   ASN1C_MsgCtrlChangeTimeFactor (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgCtrlChangeTimeFactor& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgCtrlChangeTimeFactor (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlChangeTimeFactor value);
EXTERN int asn1PD_MsgCtrlChangeTimeFactor (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlChangeTimeFactor* pvalue);

EXTERN void asn1Print_MsgCtrlChangeTimeFactor (ASN1ConstCharPtr name, ASN1T_MsgCtrlChangeTimeFactor* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgCtrlChangeTimeFactorAck                                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgCtrlChangeTimeFactorAck {
   ASN1INT   time_factor;
   ASN1T_EnumCtrlErrorCode  error_code;
} ASN1T_MsgCtrlChangeTimeFactorAck;

class EXTERN ASN1C_MsgCtrlChangeTimeFactorAck : public ASN1CType {
public:
   ASN1T_MsgCtrlChangeTimeFactorAck& msgData;
   ASN1C_MsgCtrlChangeTimeFactorAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgCtrlChangeTimeFactorAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgCtrlChangeTimeFactorAck (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlChangeTimeFactorAck* pvalue);
EXTERN int asn1PD_MsgCtrlChangeTimeFactorAck (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlChangeTimeFactorAck* pvalue);

EXTERN void asn1Print_MsgCtrlChangeTimeFactorAck (ASN1ConstCharPtr name, ASN1T_MsgCtrlChangeTimeFactorAck* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgCtrlBeginTick                                          */
/*                                                            */
/**************************************************************/

typedef ASN1INT  ASN1T_MsgCtrlBeginTick;

class EXTERN ASN1C_MsgCtrlBeginTick : public ASN1CType {
public:
   ASN1T_MsgCtrlBeginTick& msgData;
   ASN1C_MsgCtrlBeginTick (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgCtrlBeginTick& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgCtrlBeginTick (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlBeginTick value);
EXTERN int asn1PD_MsgCtrlBeginTick (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlBeginTick* pvalue);

EXTERN void asn1Print_MsgCtrlBeginTick (ASN1ConstCharPtr name, ASN1T_MsgCtrlBeginTick* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgCtrlEndTick                                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgCtrlEndTick {
   ASN1INT   current_tick;
   ASN1INT   tick_duration;
   ASN1INT   nb_pathfinds_longs;
   ASN1INT   nb_pathfinds_courts;
   ASN1INT   memory;
   ASN1INT   virtual_memory;
} ASN1T_MsgCtrlEndTick;

class EXTERN ASN1C_MsgCtrlEndTick : public ASN1CType {
public:
   ASN1T_MsgCtrlEndTick& msgData;
   ASN1C_MsgCtrlEndTick (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgCtrlEndTick& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgCtrlEndTick (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlEndTick* pvalue);
EXTERN int asn1PD_MsgCtrlEndTick (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlEndTick* pvalue);

EXTERN void asn1Print_MsgCtrlEndTick (ASN1ConstCharPtr name, ASN1T_MsgCtrlEndTick* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgCtrlClientAnnouncement                                 */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgCtrlClientAnnouncement : public ASN1CType {
public:
   ASN1C_MsgCtrlClientAnnouncement (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgCtrlClientAnnouncement (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgCtrlClientAnnouncement (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgCtrlClientAnnouncement (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  EnumEtatSim                                               */
/*                                                            */
/**************************************************************/

struct EXTERN EnumEtatSim {
   enum Root {
      running = 0,
      paused = 1,
      stopped = 2
   } ;
} ;

typedef EnumEtatSim::Root ASN1T_EnumEtatSim;

class EXTERN ASN1C_EnumEtatSim : public ASN1CType {
public:
   ASN1T_EnumEtatSim& msgData;
   ASN1C_EnumEtatSim (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumEtatSim& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumEtatSim (ASN1CTXT* ctxt_p, ASN1T_EnumEtatSim value);
EXTERN int asn1PD_EnumEtatSim (ASN1CTXT* ctxt_p, ASN1T_EnumEtatSim* pvalue);

EXTERN void asn1Print_EnumEtatSim (ASN1ConstCharPtr name, ASN1T_EnumEtatSim* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgCtrlInfo                                               */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgCtrlInfo {
   ASN1INT   current_tick;
   ASN1INT   tick_duration;
   ASN1INT   time_factor;
   ASN1INT   checkpoint_frequence;
   ASN1T_EnumEtatSim  etat;
} ASN1T_MsgCtrlInfo;

class EXTERN ASN1C_MsgCtrlInfo : public ASN1CType {
public:
   ASN1T_MsgCtrlInfo& msgData;
   ASN1C_MsgCtrlInfo (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgCtrlInfo& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgCtrlInfo (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlInfo* pvalue);
EXTERN int asn1PD_MsgCtrlInfo (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlInfo* pvalue);

EXTERN void asn1Print_MsgCtrlInfo (ASN1ConstCharPtr name, ASN1T_MsgCtrlInfo* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgCtrlCheckPointSaveBegin                                */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgCtrlCheckPointSaveBegin : public ASN1CType {
public:
   ASN1C_MsgCtrlCheckPointSaveBegin (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgCtrlCheckPointSaveBegin (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgCtrlCheckPointSaveBegin (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgCtrlCheckPointSaveBegin (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgCtrlCheckPointSaveEnd                                  */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgCtrlCheckPointSaveEnd : public ASN1CType {
public:
   ASN1C_MsgCtrlCheckPointSaveEnd (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgCtrlCheckPointSaveEnd (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgCtrlCheckPointSaveEnd (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgCtrlCheckPointSaveEnd (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgCtrlCheckPointLoadBegin                                */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgCtrlCheckPointLoadBegin : public ASN1CType {
public:
   ASN1C_MsgCtrlCheckPointLoadBegin (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgCtrlCheckPointLoadBegin (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgCtrlCheckPointLoadBegin (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgCtrlCheckPointLoadBegin (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgCtrlCheckPointLoadEnd                                  */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgCtrlCheckPointLoadEnd : public ASN1CType {
public:
   ASN1C_MsgCtrlCheckPointLoadEnd (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgCtrlCheckPointLoadEnd (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgCtrlCheckPointLoadEnd (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgCtrlCheckPointLoadEnd (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgCtrlCheckPointSetFrequency                             */
/*                                                            */
/**************************************************************/

typedef ASN1INT  ASN1T_MsgCtrlCheckPointSetFrequency;

class EXTERN ASN1C_MsgCtrlCheckPointSetFrequency : public ASN1CType {
public:
   ASN1T_MsgCtrlCheckPointSetFrequency& msgData;
   ASN1C_MsgCtrlCheckPointSetFrequency (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgCtrlCheckPointSetFrequency& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgCtrlCheckPointSetFrequency (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlCheckPointSetFrequency value);
EXTERN int asn1PD_MsgCtrlCheckPointSetFrequency (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlCheckPointSetFrequency* pvalue);

EXTERN void asn1Print_MsgCtrlCheckPointSetFrequency (ASN1ConstCharPtr name, ASN1T_MsgCtrlCheckPointSetFrequency* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgCtrlCheckPointSetFrequencyAck                          */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgCtrlCheckPointSetFrequencyAck : public ASN1CType {
public:
   ASN1C_MsgCtrlCheckPointSetFrequencyAck (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgCtrlCheckPointSetFrequencyAck (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgCtrlCheckPointSetFrequencyAck (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgCtrlCheckPointSetFrequencyAck (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgCtrlCheckPointSaveNow                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgCtrlCheckPointSaveNow {
   struct {
      unsigned nomPresent : 1;
   } m;
   ASN1VisibleString  nom;

   ASN1T_MsgCtrlCheckPointSaveNow () {
      m.nomPresent = 0;
   }
} ASN1T_MsgCtrlCheckPointSaveNow;

class EXTERN ASN1C_MsgCtrlCheckPointSaveNow : public ASN1CType {
public:
   ASN1T_MsgCtrlCheckPointSaveNow& msgData;
   ASN1C_MsgCtrlCheckPointSaveNow (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgCtrlCheckPointSaveNow& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgCtrlCheckPointSaveNow (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlCheckPointSaveNow* pvalue);
EXTERN int asn1PD_MsgCtrlCheckPointSaveNow (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlCheckPointSaveNow* pvalue);

EXTERN void asn1Print_MsgCtrlCheckPointSaveNow (ASN1ConstCharPtr name, ASN1T_MsgCtrlCheckPointSaveNow* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgCtrlCheckPointSaveNowAck                               */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgCtrlCheckPointSaveNowAck : public ASN1CType {
public:
   ASN1C_MsgCtrlCheckPointSaveNowAck (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgCtrlCheckPointSaveNowAck (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgCtrlCheckPointSaveNowAck (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgCtrlCheckPointSaveNowAck (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgCtrlSendCurrentStateBegin                              */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgCtrlSendCurrentStateBegin : public ASN1CType {
public:
   ASN1C_MsgCtrlSendCurrentStateBegin (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgCtrlSendCurrentStateBegin (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgCtrlSendCurrentStateBegin (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgCtrlSendCurrentStateBegin (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgCtrlSendCurrentStateEnd                                */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgCtrlSendCurrentStateEnd : public ASN1CType {
public:
   ASN1C_MsgCtrlSendCurrentStateEnd (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgCtrlSendCurrentStateEnd (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgCtrlSendCurrentStateEnd (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgCtrlSendCurrentStateEnd (ASN1ConstCharPtr name);

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
/*  MeteoAttributs                                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MeteoAttributs {
   ASN1INT   temperature;
   ASN1T_Vitesse  vitesse_vent;
   ASN1T_Direction  direction_vent;
   ASN1T_Altitude  plancher_couverture_nuageuse;
   ASN1T_Altitude  plafond_couverture_nuageuse;
   ASN1T_Pourcentage  densite_moyenne_couverture_nuageuse;
   ASN1T_EnumPrecipitationType  precipitation;
} ASN1T_MeteoAttributs;

class EXTERN ASN1C_MeteoAttributs : public ASN1CType {
public:
   ASN1T_MeteoAttributs& msgData;
   ASN1C_MeteoAttributs (
      ASN1MessageBuffer& msgBuf, ASN1T_MeteoAttributs& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MeteoAttributs (ASN1CTXT* ctxt_p, ASN1T_MeteoAttributs* pvalue);
EXTERN int asn1PD_MeteoAttributs (ASN1CTXT* ctxt_p, ASN1T_MeteoAttributs* pvalue);

EXTERN void asn1Print_MeteoAttributs (ASN1ConstCharPtr name, ASN1T_MeteoAttributs* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgCtrlMeteoGlobale                                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgCtrlMeteoGlobale {
   ASN1T_MeteoAttributs  attributs;
} ASN1T_MsgCtrlMeteoGlobale;

class EXTERN ASN1C_MsgCtrlMeteoGlobale : public ASN1CType {
public:
   ASN1T_MsgCtrlMeteoGlobale& msgData;
   ASN1C_MsgCtrlMeteoGlobale (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgCtrlMeteoGlobale& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgCtrlMeteoGlobale (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlMeteoGlobale* pvalue);
EXTERN int asn1PD_MsgCtrlMeteoGlobale (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlMeteoGlobale* pvalue);

EXTERN void asn1Print_MsgCtrlMeteoGlobale (ASN1ConstCharPtr name, ASN1T_MsgCtrlMeteoGlobale* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgCtrlMeteoLocale_meteo                                  */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_MsgCtrlMeteoLocale_meteo_attributs 1
#define T_MsgCtrlMeteoLocale_meteo_globale 2

typedef struct EXTERN ASN1T_MsgCtrlMeteoLocale_meteo {
   int t;
   union {
      /* t = 1 */
      ASN1T_MeteoAttributs *attributs;
      /* t = 2 */
   } u;
} ASN1T_MsgCtrlMeteoLocale_meteo;

class EXTERN ASN1C_MsgCtrlMeteoLocale_meteo : public ASN1CType {
public:
   ASN1T_MsgCtrlMeteoLocale_meteo& msgData;
   ASN1C_MsgCtrlMeteoLocale_meteo (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgCtrlMeteoLocale_meteo& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgCtrlMeteoLocale_meteo (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlMeteoLocale_meteo* pvalue);
EXTERN int asn1PD_MsgCtrlMeteoLocale_meteo (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlMeteoLocale_meteo* pvalue);

EXTERN void asn1Print_MsgCtrlMeteoLocale_meteo (ASN1ConstCharPtr name, ASN1T_MsgCtrlMeteoLocale_meteo* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgCtrlMeteoLocale                                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgCtrlMeteoLocale {
   ASN1T_CoordUTM  rect_point_haut_gauche;
   ASN1T_CoordUTM  rect_point_bas_droite;
   ASN1T_MsgCtrlMeteoLocale_meteo  meteo;
} ASN1T_MsgCtrlMeteoLocale;

class EXTERN ASN1C_MsgCtrlMeteoLocale : public ASN1CType {
public:
   ASN1T_MsgCtrlMeteoLocale& msgData;
   ASN1C_MsgCtrlMeteoLocale (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgCtrlMeteoLocale& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgCtrlMeteoLocale (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlMeteoLocale* pvalue);
EXTERN int asn1PD_MsgCtrlMeteoLocale (ASN1CTXT* ctxt_p, ASN1T_MsgCtrlMeteoLocale* pvalue);

EXTERN void asn1Print_MsgCtrlMeteoLocale (ASN1ConstCharPtr name, ASN1T_MsgCtrlMeteoLocale* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgCtrlMeteoGlobaleAck                                    */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgCtrlMeteoGlobaleAck : public ASN1CType {
public:
   ASN1C_MsgCtrlMeteoGlobaleAck (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgCtrlMeteoGlobaleAck (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgCtrlMeteoGlobaleAck (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgCtrlMeteoGlobaleAck (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgCtrlMeteoLocaleAck                                     */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgCtrlMeteoLocaleAck : public ASN1CType {
public:
   ASN1C_MsgCtrlMeteoLocaleAck (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgCtrlMeteoLocaleAck (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgCtrlMeteoLocaleAck (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgCtrlMeteoLocaleAck (ASN1ConstCharPtr name);

#endif

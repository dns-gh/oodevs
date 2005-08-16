/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 16-Aug-2005.
 */
#ifndef SCIPIOLAUNCHER_H
#define SCIPIOLAUNCHER_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"

/**************************************************************/
/*                                                            */
/*  StringLibelle                                             */
/*                                                            */
/**************************************************************/

typedef ASN1VisibleString ASN1T_StringLibelle;

class EXTERN ASN1C_StringLibelle : public ASN1CType {
public:
   ASN1T_StringLibelle& msgData;
   ASN1C_StringLibelle (
      ASN1MessageBuffer& msgBuf, ASN1T_StringLibelle& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_StringLibelle (ASN1CTXT* ctxt_p, ASN1T_StringLibelle value);
EXTERN int asn1PD_StringLibelle (ASN1CTXT* ctxt_p, ASN1T_StringLibelle* pvalue);

EXTERN void asn1Print_StringLibelle (ASN1ConstCharPtr name, ASN1T_StringLibelle* pvalue);

/**************************************************************/
/*                                                            */
/*  ID_EX                                                     */
/*                                                            */
/**************************************************************/

typedef ASN1T_OID ASN1T_ID_EX;

class EXTERN ASN1C_ID_EX : public ASN1CType {
public:
   ASN1T_ID_EX& msgData;
   ASN1C_ID_EX (
      ASN1MessageBuffer& msgBuf, ASN1T_ID_EX& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ID_EX (ASN1CTXT* ctxt_p, ASN1T_ID_EX value);
EXTERN int asn1PD_ID_EX (ASN1CTXT* ctxt_p, ASN1T_ID_EX* pvalue);

EXTERN void asn1Print_ID_EX (ASN1ConstCharPtr name, ASN1T_ID_EX* pvalue);

/**************************************************************/
/*                                                            */
/*  NumPort                                                   */
/*                                                            */
/**************************************************************/

typedef ASN1INT  ASN1T_NumPort;

class EXTERN ASN1C_NumPort : public ASN1CType {
public:
   ASN1T_NumPort& msgData;
   ASN1C_NumPort (
      ASN1MessageBuffer& msgBuf, ASN1T_NumPort& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_NumPort (ASN1CTXT* ctxt_p, ASN1T_NumPort value);
EXTERN int asn1PD_NumPort (ASN1CTXT* ctxt_p, ASN1T_NumPort* pvalue);

EXTERN void asn1Print_NumPort (ASN1ConstCharPtr name, ASN1T_NumPort* pvalue);

/**************************************************************/
/*                                                            */
/*  DinMagicNumber                                            */
/*                                                            */
/**************************************************************/

typedef ASN1UINT ASN1T_DinMagicNumber;

class EXTERN ASN1C_DinMagicNumber : public ASN1CType {
public:
   ASN1T_DinMagicNumber& msgData;
   ASN1C_DinMagicNumber (
      ASN1MessageBuffer& msgBuf, ASN1T_DinMagicNumber& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_DinMagicNumber (ASN1CTXT* ctxt_p, ASN1T_DinMagicNumber value);
EXTERN int asn1PD_DinMagicNumber (ASN1CTXT* ctxt_p, ASN1T_DinMagicNumber* pvalue);

EXTERN void asn1Print_DinMagicNumber (ASN1ConstCharPtr name, ASN1T_DinMagicNumber* pvalue);

/**************************************************************/
/*                                                            */
/*  IdAndLibelle                                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_IdAndLibelle {
   ASN1T_OID  nID;
   ASN1T_StringLibelle  libelle;
   ASN1VisibleString  uuid_odb;
} ASN1T_IdAndLibelle;

class EXTERN ASN1C_IdAndLibelle : public ASN1CType {
public:
   ASN1T_IdAndLibelle& msgData;
   ASN1C_IdAndLibelle (
      ASN1MessageBuffer& msgBuf, ASN1T_IdAndLibelle& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_IdAndLibelle (ASN1CTXT* ctxt_p, ASN1T_IdAndLibelle* pvalue);
EXTERN int asn1PD_IdAndLibelle (ASN1CTXT* ctxt_p, ASN1T_IdAndLibelle* pvalue);

EXTERN void asn1Print_IdAndLibelle (ASN1ConstCharPtr name, ASN1T_IdAndLibelle* pvalue);

/**************************************************************/
/*                                                            */
/*  ListIdAndLibelle                                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ListIdAndLibelle {
   ASN1UINT n;
   ASN1T_IdAndLibelle *elem;
} ASN1T_ListIdAndLibelle;

class EXTERN ASN1C_ListIdAndLibelle : public ASN1CType {
public:
   ASN1T_ListIdAndLibelle& msgData;
   ASN1C_ListIdAndLibelle (
      ASN1MessageBuffer& msgBuf, ASN1T_ListIdAndLibelle& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ListIdAndLibelle (ASN1CTXT* ctxt_p, ASN1T_ListIdAndLibelle* pvalue);
EXTERN int asn1PD_ListIdAndLibelle (ASN1CTXT* ctxt_p, ASN1T_ListIdAndLibelle* pvalue);

EXTERN void asn1Print_ListIdAndLibelle (ASN1ConstCharPtr name, ASN1T_ListIdAndLibelle* pvalue);

/**************************************************************/
/*                                                            */
/*  CheckPointInfo                                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_CheckPointInfo {
   struct {
      unsigned nom_checkpointPresent : 1;
   } m;
   ASN1T_OID  nID;
   ASN1T_StringLibelle  libelle_scenario;
   ASN1T_StringLibelle  libelle_checkpoint;
   ASN1VisibleString  nom_checkpoint;

   ASN1T_CheckPointInfo () {
      m.nom_checkpointPresent = 0;
   }
} ASN1T_CheckPointInfo;

class EXTERN ASN1C_CheckPointInfo : public ASN1CType {
public:
   ASN1T_CheckPointInfo& msgData;
   ASN1C_CheckPointInfo (
      ASN1MessageBuffer& msgBuf, ASN1T_CheckPointInfo& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_CheckPointInfo (ASN1CTXT* ctxt_p, ASN1T_CheckPointInfo* pvalue);
EXTERN int asn1PD_CheckPointInfo (ASN1CTXT* ctxt_p, ASN1T_CheckPointInfo* pvalue);

EXTERN void asn1Print_CheckPointInfo (ASN1ConstCharPtr name, ASN1T_CheckPointInfo* pvalue);

/**************************************************************/
/*                                                            */
/*  ListCheckPointInfo                                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ListCheckPointInfo {
   ASN1UINT n;
   ASN1T_CheckPointInfo *elem;
} ASN1T_ListCheckPointInfo;

class EXTERN ASN1C_ListCheckPointInfo : public ASN1CType {
public:
   ASN1T_ListCheckPointInfo& msgData;
   ASN1C_ListCheckPointInfo (
      ASN1MessageBuffer& msgBuf, ASN1T_ListCheckPointInfo& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ListCheckPointInfo (ASN1CTXT* ctxt_p, ASN1T_ListCheckPointInfo* pvalue);
EXTERN int asn1PD_ListCheckPointInfo (ASN1CTXT* ctxt_p, ASN1T_ListCheckPointInfo* pvalue);

EXTERN void asn1Print_ListCheckPointInfo (ASN1ConstCharPtr name, ASN1T_ListCheckPointInfo* pvalue);

/**************************************************************/
/*                                                            */
/*  EnumCtrlErrorCodeLauncher                                 */
/*                                                            */
/**************************************************************/

struct EXTERN EnumCtrlErrorCodeLauncher {
   enum Root {
      no_error = 0,
      error_already_started = 1,
      error_not_started = 2,
      error_not_paused = 3,
      error_already_paused = 4,
      error_invalid_time_factor = 5,
      error_invalid_scenario_identifier = 6,
      error_error_scenario_init = 7,
      error_invalid_data_path = 8,
      error_invalid_data = 9,
      error_invalid_checkpoint_identifier = 10,
      error_error_checkpoint_init = 11,
      error_sim_killed_by_remote = 12
   } ;
} ;

typedef EnumCtrlErrorCodeLauncher::Root ASN1T_EnumCtrlErrorCodeLauncher;

class EXTERN ASN1C_EnumCtrlErrorCodeLauncher : public ASN1CType {
public:
   ASN1T_EnumCtrlErrorCodeLauncher& msgData;
   ASN1C_EnumCtrlErrorCodeLauncher (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumCtrlErrorCodeLauncher& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumCtrlErrorCodeLauncher (ASN1CTXT* ctxt_p, ASN1T_EnumCtrlErrorCodeLauncher value);
EXTERN int asn1PD_EnumCtrlErrorCodeLauncher (ASN1CTXT* ctxt_p, ASN1T_EnumCtrlErrorCodeLauncher* pvalue);

EXTERN void asn1Print_EnumCtrlErrorCodeLauncher (ASN1ConstCharPtr name, ASN1T_EnumCtrlErrorCodeLauncher* pvalue);

/**************************************************************/
/*                                                            */
/*  LaunchInformation                                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_LaunchInformation {
   ASN1T_EnumCtrlErrorCodeLauncher  error_code;
   ASN1T_NumPort  num_port;
   ASN1T_DinMagicNumber  din_magic_number;
} ASN1T_LaunchInformation;

class EXTERN ASN1C_LaunchInformation : public ASN1CType {
public:
   ASN1T_LaunchInformation& msgData;
   ASN1C_LaunchInformation (
      ASN1MessageBuffer& msgBuf, ASN1T_LaunchInformation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_LaunchInformation (ASN1CTXT* ctxt_p, ASN1T_LaunchInformation* pvalue);
EXTERN int asn1PD_LaunchInformation (ASN1CTXT* ctxt_p, ASN1T_LaunchInformation* pvalue);

EXTERN void asn1Print_LaunchInformation (ASN1ConstCharPtr name, ASN1T_LaunchInformation* pvalue);

/**************************************************************/
/*                                                            */
/*  IdExAndLaunchInformation                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_IdExAndLaunchInformation {
   struct {
      unsigned launch_informationPresent : 1;
   } m;
   ASN1T_ID_EX  id_exercice;
   ASN1T_LaunchInformation  launch_information;

   ASN1T_IdExAndLaunchInformation () {
      m.launch_informationPresent = 0;
   }
} ASN1T_IdExAndLaunchInformation;

class EXTERN ASN1C_IdExAndLaunchInformation : public ASN1CType {
public:
   ASN1T_IdExAndLaunchInformation& msgData;
   ASN1C_IdExAndLaunchInformation (
      ASN1MessageBuffer& msgBuf, ASN1T_IdExAndLaunchInformation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_IdExAndLaunchInformation (ASN1CTXT* ctxt_p, ASN1T_IdExAndLaunchInformation* pvalue);
EXTERN int asn1PD_IdExAndLaunchInformation (ASN1CTXT* ctxt_p, ASN1T_IdExAndLaunchInformation* pvalue);

EXTERN void asn1Print_IdExAndLaunchInformation (ASN1ConstCharPtr name, ASN1T_IdExAndLaunchInformation* pvalue);

/**************************************************************/
/*                                                            */
/*  ListIdExAndLaunchInformation                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_ListIdExAndLaunchInformation {
   ASN1UINT n;
   ASN1T_IdExAndLaunchInformation *elem;
} ASN1T_ListIdExAndLaunchInformation;

class EXTERN ASN1C_ListIdExAndLaunchInformation : public ASN1CType {
public:
   ASN1T_ListIdExAndLaunchInformation& msgData;
   ASN1C_ListIdExAndLaunchInformation (
      ASN1MessageBuffer& msgBuf, ASN1T_ListIdExAndLaunchInformation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_ListIdExAndLaunchInformation (ASN1CTXT* ctxt_p, ASN1T_ListIdExAndLaunchInformation* pvalue);
EXTERN int asn1PD_ListIdExAndLaunchInformation (ASN1CTXT* ctxt_p, ASN1T_ListIdExAndLaunchInformation* pvalue);

EXTERN void asn1Print_ListIdExAndLaunchInformation (ASN1ConstCharPtr name, ASN1T_ListIdExAndLaunchInformation* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgMosLau_RequestListMission                              */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgMosLau_RequestListMission : public ASN1CType {
public:
   ASN1C_MsgMosLau_RequestListMission (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgMosLau_RequestListMission (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgMosLau_RequestListMission (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgMosLau_RequestListMission (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgLauMos_ListMission                                     */
/*                                                            */
/**************************************************************/

typedef ASN1T_ListIdAndLibelle ASN1T_MsgLauMos_ListMission;

class EXTERN ASN1C_MsgLauMos_ListMission : public ASN1CType {
public:
   ASN1T_MsgLauMos_ListMission& msgData;
   ASN1C_MsgLauMos_ListMission (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLauMos_ListMission& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLauMos_ListMission (ASN1CTXT* ctxt_p, ASN1T_MsgLauMos_ListMission* pvalue);
EXTERN int asn1PD_MsgLauMos_ListMission (ASN1CTXT* ctxt_p, ASN1T_MsgLauMos_ListMission* pvalue);

EXTERN void asn1Print_MsgLauMos_ListMission (ASN1ConstCharPtr name, ASN1T_MsgLauMos_ListMission* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgMosLau_LaunchSim                                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgMosLau_LaunchSim {
   ASN1T_ID_EX  id_exercice;
   ASN1T_OID  id_scenario;
   ASN1VisibleString  sim_commandline;
} ASN1T_MsgMosLau_LaunchSim;

class EXTERN ASN1C_MsgMosLau_LaunchSim : public ASN1CType {
public:
   ASN1T_MsgMosLau_LaunchSim& msgData;
   ASN1C_MsgMosLau_LaunchSim (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgMosLau_LaunchSim& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgMosLau_LaunchSim (ASN1CTXT* ctxt_p, ASN1T_MsgMosLau_LaunchSim* pvalue);
EXTERN int asn1PD_MsgMosLau_LaunchSim (ASN1CTXT* ctxt_p, ASN1T_MsgMosLau_LaunchSim* pvalue);

EXTERN void asn1Print_MsgMosLau_LaunchSim (ASN1ConstCharPtr name, ASN1T_MsgMosLau_LaunchSim* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgSimLau_SimLaunched                                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgSimLau_SimLaunched {
   ASN1T_ID_EX  id_exercice;
   ASN1T_LaunchInformation  launch_information;
} ASN1T_MsgSimLau_SimLaunched;

class EXTERN ASN1C_MsgSimLau_SimLaunched : public ASN1CType {
public:
   ASN1T_MsgSimLau_SimLaunched& msgData;
   ASN1C_MsgSimLau_SimLaunched (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgSimLau_SimLaunched& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgSimLau_SimLaunched (ASN1CTXT* ctxt_p, ASN1T_MsgSimLau_SimLaunched* pvalue);
EXTERN int asn1PD_MsgSimLau_SimLaunched (ASN1CTXT* ctxt_p, ASN1T_MsgSimLau_SimLaunched* pvalue);

EXTERN void asn1Print_MsgSimLau_SimLaunched (ASN1ConstCharPtr name, ASN1T_MsgSimLau_SimLaunched* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLauMos_SimLaunched                                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLauMos_SimLaunched {
   ASN1T_ID_EX  id_exercice;
   ASN1T_LaunchInformation  launch_information;
} ASN1T_MsgLauMos_SimLaunched;

class EXTERN ASN1C_MsgLauMos_SimLaunched : public ASN1CType {
public:
   ASN1T_MsgLauMos_SimLaunched& msgData;
   ASN1C_MsgLauMos_SimLaunched (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLauMos_SimLaunched& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLauMos_SimLaunched (ASN1CTXT* ctxt_p, ASN1T_MsgLauMos_SimLaunched* pvalue);
EXTERN int asn1PD_MsgLauMos_SimLaunched (ASN1CTXT* ctxt_p, ASN1T_MsgLauMos_SimLaunched* pvalue);

EXTERN void asn1Print_MsgLauMos_SimLaunched (ASN1ConstCharPtr name, ASN1T_MsgLauMos_SimLaunched* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLauSim_Ping                                            */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgLauSim_Ping : public ASN1CType {
public:
   ASN1C_MsgLauSim_Ping (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLauSim_Ping (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgLauSim_Ping (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgLauSim_Ping (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgSimLau_Pong                                            */
/*                                                            */
/**************************************************************/

typedef ASN1T_ID_EX ASN1T_MsgSimLau_Pong;

class EXTERN ASN1C_MsgSimLau_Pong : public ASN1CType {
public:
   ASN1T_MsgSimLau_Pong& msgData;
   ASN1C_MsgSimLau_Pong (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgSimLau_Pong& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgSimLau_Pong (ASN1CTXT* ctxt_p, ASN1T_MsgSimLau_Pong value);
EXTERN int asn1PD_MsgSimLau_Pong (ASN1CTXT* ctxt_p, ASN1T_MsgSimLau_Pong* pvalue);

EXTERN void asn1Print_MsgSimLau_Pong (ASN1ConstCharPtr name, ASN1T_MsgSimLau_Pong* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgMosLau_RequestListSim                                  */
/*                                                            */
/**************************************************************/

class EXTERN ASN1C_MsgMosLau_RequestListSim : public ASN1CType {
public:
   ASN1C_MsgMosLau_RequestListSim (
      ASN1MessageBuffer& msgBuf);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgMosLau_RequestListSim (ASN1CTXT* ctxt_p);
EXTERN int asn1PD_MsgMosLau_RequestListSim (ASN1CTXT* ctxt_p);

EXTERN void asn1Print_MsgMosLau_RequestListSim (ASN1ConstCharPtr name);

/**************************************************************/
/*                                                            */
/*  MsgLauMos_ListSim                                         */
/*                                                            */
/**************************************************************/

typedef ASN1T_ListIdExAndLaunchInformation ASN1T_MsgLauMos_ListSim;

class EXTERN ASN1C_MsgLauMos_ListSim : public ASN1CType {
public:
   ASN1T_MsgLauMos_ListSim& msgData;
   ASN1C_MsgLauMos_ListSim (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLauMos_ListSim& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLauMos_ListSim (ASN1CTXT* ctxt_p, ASN1T_MsgLauMos_ListSim* pvalue);
EXTERN int asn1PD_MsgLauMos_ListSim (ASN1CTXT* ctxt_p, ASN1T_MsgLauMos_ListSim* pvalue);

EXTERN void asn1Print_MsgLauMos_ListSim (ASN1ConstCharPtr name, ASN1T_MsgLauMos_ListSim* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLauMos_SimFailure                                      */
/*                                                            */
/**************************************************************/

typedef ASN1T_ID_EX ASN1T_MsgLauMos_SimFailure;

class EXTERN ASN1C_MsgLauMos_SimFailure : public ASN1CType {
public:
   ASN1T_MsgLauMos_SimFailure& msgData;
   ASN1C_MsgLauMos_SimFailure (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLauMos_SimFailure& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLauMos_SimFailure (ASN1CTXT* ctxt_p, ASN1T_MsgLauMos_SimFailure value);
EXTERN int asn1PD_MsgLauMos_SimFailure (ASN1CTXT* ctxt_p, ASN1T_MsgLauMos_SimFailure* pvalue);

EXTERN void asn1Print_MsgLauMos_SimFailure (ASN1ConstCharPtr name, ASN1T_MsgLauMos_SimFailure* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgMosLau_RequestListCheckpoint                           */
/*                                                            */
/**************************************************************/

typedef ASN1T_ID_EX ASN1T_MsgMosLau_RequestListCheckpoint;

class EXTERN ASN1C_MsgMosLau_RequestListCheckpoint : public ASN1CType {
public:
   ASN1T_MsgMosLau_RequestListCheckpoint& msgData;
   ASN1C_MsgMosLau_RequestListCheckpoint (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgMosLau_RequestListCheckpoint& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgMosLau_RequestListCheckpoint (ASN1CTXT* ctxt_p, ASN1T_MsgMosLau_RequestListCheckpoint value);
EXTERN int asn1PD_MsgMosLau_RequestListCheckpoint (ASN1CTXT* ctxt_p, ASN1T_MsgMosLau_RequestListCheckpoint* pvalue);

EXTERN void asn1Print_MsgMosLau_RequestListCheckpoint (ASN1ConstCharPtr name, ASN1T_MsgMosLau_RequestListCheckpoint* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLauMos_ListCheckpoint                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLauMos_ListCheckpoint {
   ASN1T_ID_EX  id_exercice;
   ASN1T_ListCheckPointInfo  list_checkpoint;
} ASN1T_MsgLauMos_ListCheckpoint;

class EXTERN ASN1C_MsgLauMos_ListCheckpoint : public ASN1CType {
public:
   ASN1T_MsgLauMos_ListCheckpoint& msgData;
   ASN1C_MsgLauMos_ListCheckpoint (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLauMos_ListCheckpoint& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLauMos_ListCheckpoint (ASN1CTXT* ctxt_p, ASN1T_MsgLauMos_ListCheckpoint* pvalue);
EXTERN int asn1PD_MsgLauMos_ListCheckpoint (ASN1CTXT* ctxt_p, ASN1T_MsgLauMos_ListCheckpoint* pvalue);

EXTERN void asn1Print_MsgLauMos_ListCheckpoint (ASN1ConstCharPtr name, ASN1T_MsgLauMos_ListCheckpoint* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgMosLau_LaunchSimFromCheckpoint                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgMosLau_LaunchSimFromCheckpoint {
   ASN1T_ID_EX  id_exercice;
   ASN1T_OID  id_checkpoint;
   ASN1VisibleString  sim_commandline;
} ASN1T_MsgMosLau_LaunchSimFromCheckpoint;

class EXTERN ASN1C_MsgMosLau_LaunchSimFromCheckpoint : public ASN1CType {
public:
   ASN1T_MsgMosLau_LaunchSimFromCheckpoint& msgData;
   ASN1C_MsgMosLau_LaunchSimFromCheckpoint (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgMosLau_LaunchSimFromCheckpoint& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgMosLau_LaunchSimFromCheckpoint (ASN1CTXT* ctxt_p, ASN1T_MsgMosLau_LaunchSimFromCheckpoint* pvalue);
EXTERN int asn1PD_MsgMosLau_LaunchSimFromCheckpoint (ASN1CTXT* ctxt_p, ASN1T_MsgMosLau_LaunchSimFromCheckpoint* pvalue);

EXTERN void asn1Print_MsgMosLau_LaunchSimFromCheckpoint (ASN1ConstCharPtr name, ASN1T_MsgMosLau_LaunchSimFromCheckpoint* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgMosLau_StopSim                                         */
/*                                                            */
/**************************************************************/

typedef ASN1T_ID_EX ASN1T_MsgMosLau_StopSim;

class EXTERN ASN1C_MsgMosLau_StopSim : public ASN1CType {
public:
   ASN1T_MsgMosLau_StopSim& msgData;
   ASN1C_MsgMosLau_StopSim (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgMosLau_StopSim& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgMosLau_StopSim (ASN1CTXT* ctxt_p, ASN1T_MsgMosLau_StopSim value);
EXTERN int asn1PD_MsgMosLau_StopSim (ASN1CTXT* ctxt_p, ASN1T_MsgMosLau_StopSim* pvalue);

EXTERN void asn1Print_MsgMosLau_StopSim (ASN1ConstCharPtr name, ASN1T_MsgMosLau_StopSim* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLauSim_StopSim                                         */
/*                                                            */
/**************************************************************/

typedef ASN1T_ID_EX ASN1T_MsgLauSim_StopSim;

class EXTERN ASN1C_MsgLauSim_StopSim : public ASN1CType {
public:
   ASN1T_MsgLauSim_StopSim& msgData;
   ASN1C_MsgLauSim_StopSim (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLauSim_StopSim& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLauSim_StopSim (ASN1CTXT* ctxt_p, ASN1T_MsgLauSim_StopSim value);
EXTERN int asn1PD_MsgLauSim_StopSim (ASN1CTXT* ctxt_p, ASN1T_MsgLauSim_StopSim* pvalue);

EXTERN void asn1Print_MsgLauSim_StopSim (ASN1ConstCharPtr name, ASN1T_MsgLauSim_StopSim* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgSimLau_ExecSuccess                                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgSimLau_ExecSuccess {
   ASN1T_ID_EX  id_exercice;
   ASN1BOOL  finished_state;

   ASN1T_MsgSimLau_ExecSuccess () {
      finished_state = TRUE;
   }
} ASN1T_MsgSimLau_ExecSuccess;

class EXTERN ASN1C_MsgSimLau_ExecSuccess : public ASN1CType {
public:
   ASN1T_MsgSimLau_ExecSuccess& msgData;
   ASN1C_MsgSimLau_ExecSuccess (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgSimLau_ExecSuccess& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgSimLau_ExecSuccess (ASN1CTXT* ctxt_p, ASN1T_MsgSimLau_ExecSuccess* pvalue);
EXTERN int asn1PD_MsgSimLau_ExecSuccess (ASN1CTXT* ctxt_p, ASN1T_MsgSimLau_ExecSuccess* pvalue);

EXTERN void asn1Print_MsgSimLau_ExecSuccess (ASN1ConstCharPtr name, ASN1T_MsgSimLau_ExecSuccess* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgLauMos_ExecSuccess                                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgLauMos_ExecSuccess {
   ASN1T_ID_EX  id_exercice;
   ASN1BOOL  finished_state;

   ASN1T_MsgLauMos_ExecSuccess () {
      finished_state = TRUE;
   }
} ASN1T_MsgLauMos_ExecSuccess;

class EXTERN ASN1C_MsgLauMos_ExecSuccess : public ASN1CType {
public:
   ASN1T_MsgLauMos_ExecSuccess& msgData;
   ASN1C_MsgLauMos_ExecSuccess (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgLauMos_ExecSuccess& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgLauMos_ExecSuccess (ASN1CTXT* ctxt_p, ASN1T_MsgLauMos_ExecSuccess* pvalue);
EXTERN int asn1PD_MsgLauMos_ExecSuccess (ASN1CTXT* ctxt_p, ASN1T_MsgLauMos_ExecSuccess* pvalue);

EXTERN void asn1Print_MsgLauMos_ExecSuccess (ASN1ConstCharPtr name, ASN1T_MsgLauMos_ExecSuccess* pvalue);

#endif

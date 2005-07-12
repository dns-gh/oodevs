//*****************************************************************************
//
// $Created : AML 03-03-26 $
// $Archive : $
// $Author  : $
// $Modtime : $
// $Revision: 16 $
// $Workfile: LAU_SIM.cpp $
//
//*****************************************************************************

#include "LAU_pch.h"
#include "LAU_SIM.h"


#ifndef USE_INLINE
#   include "LAU_SIM.inl"
#endif

#include "LAU_MsgMgr.h"
#include "LAU_NetMgr.h"
#include "LAU_Workspace.h"
#include "LAU_App.h"



//-----------------------------------------------------------------------------
// Name   : LAU_SIM constructor
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
LAU_SIM::LAU_SIM( /*MT_TimerManager& timerManager, uint nNbrTimesToRun*/ )
//:   simTimer_( timerManager, nNbrTimesToRun )
{
    Reset();
}


//-----------------------------------------------------------------------------
// Name   : LAU_SIM destructor
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
LAU_SIM::~LAU_SIM()
{
    Terminate();
}

    

//-----------------------------------------------------------------------------
// Name   : LAU_SIM::Reset
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
void LAU_SIM::Reset( void )
{
    nPort_                  = 0;
    nMagic_                 = 0;
                            
    nExerciceID_            = 0;
    nID_Internal_           = 0;
//    nState_                 = eStateUnitialized;
    bAskToStop_             = false;
    bStopSended_            = false;

    strExecPath_.erase();

    nProcessHandle_         = 0;
    nProcessID_             = -1;

    bPingHasBeenSended_     = false;
    bHasBeenInitialised_    = false;
    nLaunchErrorCode_       = -1;
    tNextTimeToSendPing_    .Reset();
    tTimeLimitWithoutPong_  .Reset();

    MT_TimeSpan tCurrentTime;
    tCurrentTime.SetToSystemTime();
    tTimeLimitWithoutConnect_ = tCurrentTime + LAU_App::Instance()->GetWorkspace().GetTimeMaxWaitInit();


    pMosClient_             = 0;

}


//-----------------------------------------------------------------------------
// Name: LAU_SIM::CheckExecution
// Return : true si l'application est encore "vivante"
// Created: AML 03-04-14
//-----------------------------------------------------------------------------
bool32 LAU_SIM::CheckExecution( LAU_MsgMgr& msgMgr )
{
    // verifie si le process existe encore

//    GetModuleFileName( );
//    GetGuiResources( nProcessHandle_, GR_USEROBJECTS );

////    if( GetPriorityClass( (HANDLE)nProcessHandle_ ) == 0 )
////    {
////        // $$$$ AML 03-04-14: ne passe jamais la!
////        return false;
////    }
////
////    if( GetProcessVersion( nProcessHandle_ ) == 0 )
////    {
////        // $$$$ AML 03-04-14: sort toujours ici!
//        return false;
////    }

    uint32 nExitStatus;
    BOOL nRet = GetExitCodeProcess( (HANDLE)nProcessHandle_, &nExitStatus );
    if( nRet == 0 || nExitStatus != STILL_ACTIVE )
    {
        MT_LOG_INFO_MSG( MT_FormatString( "Launcher: Sim with ID %d has released is process handle (destroying the image)", nExerciceID_ ).c_str() );
        return false;
    }

    MT_TimeSpan tCurrentTime;
    tCurrentTime.SetToSystemTime();

    if( IsConnectedToLauncher() )
    {
        if( !IsPingHasBeenSended() )
        {
            if( tCurrentTime > tNextTimeToSendPing_ )
            {
                // envoie un ping
                if( msgMgr.SendMsg_LauSim_Ping( *this ) )
                {
                    SetPingHasBeenSended();
                    tNextTimeToSendPing_    = tCurrentTime + msgMgr.GetNetMgr().GetWorkspace().GetTimeBetweenTwoPings();
                    tTimeLimitWithoutPong_  = tCurrentTime + msgMgr.GetNetMgr().GetWorkspace().GetTimeMaxWaitPong();
                }
            }
        }
        else
        {
            // compte le nombre de temps depuis qu'on a envoyer un ping
            if( tCurrentTime > tTimeLimitWithoutPong_ && LAU_App::Instance()->IsKillStuckedSim() ) // $$$$ Alma 2004-01-30: adding the lecture of the option KillStucked
            {
                MT_LOG_INFO_MSG( MT_FormatString( "Launcher: Sim with ID %d doesn't respond to ping (destroying the process and the image!)", nExerciceID_ ).c_str() );
                return false;
            }
        }
    }
    else
    {
        bPingHasBeenSended_ = false;

        if( tCurrentTime > tTimeLimitWithoutConnect_ ) // $$$$ Alma 2004-01-30: si vous voulez aussi qu'une sim qui ne se connecte jamais ne soit pas quand meme killé, rajouté l'option && LAU_App::Instance()->IsKillStuckedSim() ici aussi
        {
            MT_LOG_INFO_MSG( MT_FormatString( "Launcher: Sim with ID %d take too much time to init (destroying the process and the image!)", nExerciceID_ ).c_str() );
            return false;            
        }


    }

    return true;
}


//-----------------------------------------------------------------------------
// Name: LAU_SIM::GetDescription
// Object: rempli strDescription avec une description de l'objet
// Created: AML 03-04-23
//-----------------------------------------------------------------------------
#define BOOL_TO_SZ(b) (b?"true ":"false")
void LAU_SIM::GetDescription( std::string& strDescription ) const
{
    strDescription = MT_FormatString
        ( 
            "Sim ID %d - state: %s, AskToStop: %s StopSend: %s, process:%4x, psid:%3d, ping: %s, init: %s, launcherrcode: %-34s, mos: %p, addr: %s",
            nExerciceID_, 
            "nostate",//GetLibelleFromState( nState_ ),
            BOOL_TO_SZ( bAskToStop_ ),
            BOOL_TO_SZ( bStopSended_ ),
            nProcessHandle_,
            nProcessID_,
            BOOL_TO_SZ( bPingHasBeenSended_ ),
            BOOL_TO_SZ( bHasBeenInitialised_ ),
            GetLibelleFromLaunchErrorCode( nLaunchErrorCode_ ),
            pMosClient_,
            pSession_== NULL?"(null)":GetSession().GetRemoteAddress().GetAddressAsString().c_str()
        );

}



//-----------------------------------------------------------------------------
// Name: LAU_SIM::GetLibelleFromState
// Created: AML 03-04-23
//-----------------------------------------------------------------------------
const char* LAU_SIM::GetLibelleFromState( const E_State nState )
{
    static char aszLibelleFromState[][12] =
    {
        "Unitialized",
        "Launched",
        "Connected",
        "NoReponse",
        "UnConnected",
        "Destroyable",
        "ValueError"
    };

    if( (int)nState < eStateUnitialized || (int)nState >= eStateValueMax )
    {
        return aszLibelleFromState[eStateValueMax];
    }

    return aszLibelleFromState[(int)nState];
}


//-----------------------------------------------------------------------------
// Name: LAU_SIM::GetLibelleFromLaunchErrorCode
// Created: AML 03-06-12
//-----------------------------------------------------------------------------
const char* LAU_SIM::GetLibelleFromLaunchErrorCode( const int32 nLaunchErrorCode )
{
    static char aszLibelleFromState[][36] =
    {
      "no_error",
      "error_already_started",
      "error_not_started",
      "error_not_paused",
      "error_already_paused",
      "error_invalid_time_factor",
      "error_invalid_scenario_identifier",
      "error_error_scenario_init",
      "error_invalid_data_path",
      "error_invalid_data",
      "error_invalid_checkpoint_identifier",
      "error_error_checkpoint_init",
      "error_sim_killed_by_remote",
      "???",
    };

    const int eLaunchErrorCodeValueMax = EnumCtrlErrorCodeLauncher::error_sim_killed_by_remote + 1;

    if( (int)nLaunchErrorCode < EnumCtrlErrorCodeLauncher::no_error || (int)nLaunchErrorCode >= eLaunchErrorCodeValueMax )
    {
        return aszLibelleFromState[eLaunchErrorCodeValueMax];
    }

    return aszLibelleFromState[(int)nLaunchErrorCode];
}
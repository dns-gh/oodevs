//*****************************************************************************
//
// $Created: AML 03-04-25 $
// $Archive: /MVW_v10/Build/SDK/LAU/src/ProcessInfo.cpp $
// $Author: Nld $
// $Modtime: 20/04/04 15:15 $
// $Revision: 6 $
// $Workfile: ProcessInfo.cpp $
//
//*****************************************************************************

#include "LAU_pch.h"
#include "ProcessInfo.h"

#ifndef USE_INLINE
#	include "ProcessInfo.inl"
#endif

#include "TerminateApp.c"


uint32 ProcessInfo::static_nNextID = 1;

//-----------------------------------------------------------------------------
// Name: ProcessInfo constructor
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
ProcessInfo::ProcessInfo()
{
    Reset();
}


//-----------------------------------------------------------------------------
// Name: ProcessInfo constructor
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
ProcessInfo::ProcessInfo( const ProcessInfo& rhs )
:    nID_                   (   rhs.nID_                )
,    nState_                (   rhs.nState_             )
,    nExitCode_             (   rhs.nExitCode_          )
,    hProcess_              (   rhs.hProcess_           )
,    hPrimaryThread_        (   rhs.hPrimaryThread_     )
,    nProcessID_            (   rhs.nProcessID_         )
,    nPrimaryThreadID_      (   rhs.nPrimaryThreadID_   )
{

}



//-----------------------------------------------------------------------------
// Name: ProcessInfo destructor
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
ProcessInfo::~ProcessInfo()
{

}



//-----------------------------------------------------------------------------
// Name: ProcessInfo::Reset
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
void ProcessInfo::Reset()
{
    nID_                = static_nNextID; ++static_nNextID;
    nState_             = eStateUnitialized;
    nExitCode_          = -1;
    hProcess_           = NULL;
    hPrimaryThread_     = NULL;
    nProcessID_         = 0;
    nPrimaryThreadID_   = 0;
}


//-----------------------------------------------------------------------------
// Name: ProcessInfo::operator=
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
ProcessInfo& ProcessInfo::operator=( const ProcessInfo& rhs )
{
    nID_                = rhs.nID_;
    nState_             = rhs.nState_;
    nExitCode_          = rhs.nExitCode_;
    hProcess_           = rhs.hProcess_;
    hPrimaryThread_     = rhs.hPrimaryThread_;
    nProcessID_         = rhs.nProcessID_;
    nPrimaryThreadID_   = rhs.nPrimaryThreadID_;

    return *this;
}


//-----------------------------------------------------------------------------
// Name: ProcessInfo::UpdateState
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
void ProcessInfo::UpdateState()
{
    if( nState_ != eStateZombie && nState_ != eStateDestroyable )
    {
        uint32 nExitStatus;
        if( GetExitCodeProcess( hProcess_, &nExitStatus ) == 0 )
        {
            // hProcess_ n'est plus un handle valide
            nState_ = eStateDestroyable;
        }            
        else if( nExitStatus != STILL_ACTIVE )
        {
            // l'appli a fini
            nExitCode_ = nExitStatus;
            nState_ = eStateZombie;
        }

        // detecte les programme plantés:
    
////        // marche correctement: (NB: le processID est passé)
////        uint32 nVersion = GetProcessVersion( nProcessID_ );
////        if( nVersion == 0 )
////        {
////            int nLastError = GetLastError();
////            MT_LOG_ERROR_MSG( MT_FormatString( "test temporaire: error %d: %s", nLastError, SystemGetErrorMessage( nLastError )  ).c_str() );
////        }
////        MT_LOG_INFO_MSG( MT_FormatString( "processversion: %x",  nVersion ).c_str() );

////        // marche pas:
////        uint32 nRet = WaitForSingleObject( hProcess_, 1000 );
////        if( nRet != WAIT_TIMEOUT )
////        {
////            nState_ = eStateNoResponse;
////        }

    }
}


//-----------------------------------------------------------------------------
// Name: ProcessInfo::FinishProcess
// Return: true si le programme a était supprimé
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
bool32 ProcessInfo::FinishProcess( const bool32 bForceKillIfUnrespond )
{
    uint32 nRet = Terminate32App( nProcessID_, 5000, bForceKillIfUnrespond );

    if( nRet == TA_SUCCESS_CLEAN )
    {
        nState_ = eStateExiting;
    }
    else if( nRet == TA_SUCCESS_KILL )
    {
        nState_ = eStateKilling;
    }

    return nRet != TA_FAILED;
}



//-----------------------------------------------------------------------------
// Name: ProcessInfo::GetDescription
// Return: retourne un etat d'un process lancé
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
const std::string& ProcessInfo::GetDescription() const
{
    char buf[512];

    sprintf( buf, "PID:%4d | hP:%4d | hT:%4d | iP:%4d | iT:%4d | %-11s | ExitCode:0x%8X", nID_, hProcess_, hPrimaryThread_, nProcessID_, nPrimaryThreadID_, GetLibelleFromState( nState_ ), nExitCode_ );
    
    strStateDescription_ = buf;

    return strStateDescription_;
}



//-----------------------------------------------------------------------------
// Name: ProcessInfo::GetLibelleFromState
// Warning: Static Method
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
const char* ProcessInfo::GetLibelleFromState( const E_State nState )
{
    static const char szLibelleFromState[][12] =
    {
	    "Unitialized",
        "Running",
        "Exiting",
        "Killeding",
        "No response",
        "Zombie",
        "Destroyable",
        "Error"
    };

    if( nState < eStateUnitialized || nState >= eStateValueMax )
    {
        return szLibelleFromState[eStateValueMax-eStateUnitialized];
    }

    return szLibelleFromState[nState-eStateUnitialized];
}


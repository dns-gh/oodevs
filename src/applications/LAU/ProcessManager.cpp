//*****************************************************************************
//
// $Created: AML 03-04-25 $
// $Archive: /MVW_v10/Build/SDK/LAU/src/ProcessManager.cpp $
// $Author: Aml $
// $Modtime: 17/06/03 11:50 $
// $Revision: 4 $
// $Workfile: ProcessManager.cpp $
//
//*****************************************************************************

#include "LAU_pch.h"
#include "ProcessManager.h"

#ifndef USE_INLINE
#	include "ProcessManager.inl"
#endif


//-----------------------------------------------------------------------------
// Name: ProcessManager constructor
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
ProcessManager::ProcessManager()
{
    Reset();
}


//-----------------------------------------------------------------------------
// Name: ProcessManager destructor
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
ProcessManager::~ProcessManager()
{

}


//-----------------------------------------------------------------------------
// Name: ProcessManager::Reset
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
void ProcessManager::Reset()
{
    bKeepTraceAllProcess_   = false;
    strStateDescription_    .erase();

    processList_            .clear();

    processList_.reserve( 100 );
}




//-----------------------------------------------------------------------------
// Name: ProcessManager::LaunchProcess
// Examples: LaunchProcess( "c:\exe\ftp", "http://38.12 -reconnect: 15", "d:\download\" );
// Return: return l'ID du process ou -1 si le process n'a pu être lancé
// Warning: szParamsSeparatedBySpaces: les parametres sans le nom de l'exe
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
int ProcessManager::LaunchProcess( const char* szPathFilenameExe, const char* szParamsSeparatedBySpaces, const char* szExecutionPath )
{
    char bufFinalCommandLine[2048]; // this string must not be const for CreateProcess
    strcpy( bufFinalCommandLine, "\"" );
    strcat( bufFinalCommandLine, szPathFilenameExe );
    strcat( bufFinalCommandLine, "\"" );

    if( szParamsSeparatedBySpaces != NULL )
    {
        strcat( bufFinalCommandLine, " " );
        strcat( bufFinalCommandLine, szParamsSeparatedBySpaces );
    }

    bool32 bChildInheritsHandles = FALSE;
    uint32 dwCreationFlags = CREATE_DEFAULT_ERROR_MODE | CREATE_FORCEDOS | CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP | NORMAL_PRIORITY_CLASS;

    STARTUPINFO startupInfo;
    memset( &startupInfo, 0, sizeof( startupInfo ) );
    startupInfo.cb = sizeof( startupInfo );

    PROCESS_INFORMATION processInformation;

    if( 0 == CreateProcess( NULL, bufFinalCommandLine, NULL, NULL, bChildInheritsHandles, dwCreationFlags, NULL, szExecutionPath, &startupInfo, &processInformation ) )
    {
        int nLastError = GetLastError();
        MT_LOG_ERROR_MSG( MT_FormatString( "LaunchProcess: CreateProcess: error %d: %s", nLastError, SystemGetErrorMessage( nLastError )  ).c_str() );
        return -1;
    }

    processList_.push_back( ProcessInfo() );
    ProcessInfo* p = &( processList_.back() );

    p->SetHandleProcess( processInformation.hProcess );
    p->SetHandlePrimaryThread( processInformation.hThread );
    p->SetProcessID( processInformation.dwProcessId );
    p->SetPrimaryThreadID( processInformation.dwThreadId );


    return p->GetID();
}


//-----------------------------------------------------------------------------
// Name: ProcessManager::GetAllProcessDescription
// Object: retourne un état de la liste de tout les process lancés
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
const std::string& ProcessManager::GetAllProcessDescription() const
{
    strStateDescription_.erase();

    const char szSeparatorLine[] = "------------------------------------------------------------------------------------\n";

    strStateDescription_ = szSeparatorLine;

    CIT_ProcessInfo_Vector itProcess, itProcessEnd;
    for( itProcess = processList_.begin(), itProcessEnd = processList_.end(); itProcess != itProcessEnd; ++itProcess )
    {
         strStateDescription_ += (*itProcess).GetDescription() + "\n";
    }

    strStateDescription_ += szSeparatorLine;

    return strStateDescription_;
}


//-----------------------------------------------------------------------------
// Name: ProcessManager::GetProcessDescription
// Object: retourne un etat d'un process lancé
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
const std::string& ProcessManager::GetProcessDescription( const int nID ) const
{
    ProcessInfo* p = FindProcess( nID );
    if( p == NULL )
    {
        strStateDescription_ = "Process not found";
        return strStateDescription_;
    }

    return p->GetDescription();
}



//-----------------------------------------------------------------------------
// Name: ProcessManager::FinishProcess
// Return: true si le programme a était supprimé
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
bool32 ProcessManager::FinishProcess( const int nID, const bool32 bForceKillIfUnrespond )
{
    ProcessInfo* p = FindProcess( nID );
    if( p == NULL )
    {
        return false;
    }

    return p->FinishProcess( bForceKillIfUnrespond );
}



//-----------------------------------------------------------------------------
// Name: ProcessManager::GetExitCode
// Object: demande le code de retour d'un process, après cette info, on peut detruire les infos sur le process (sauf si bDestroyProcessInfo est false)
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
int ProcessManager::GetExitCode( const int nID, const bool32 bDestroyProcessInfo )
{
    ProcessInfo *p = FindProcess( nID );
    if( p == NULL )
    {
        return -1;
    }

    if( p->GetState() != ProcessInfo::eStateZombie &&  p->GetState() != ProcessInfo::eStateDestroyable )
    {
        // le process n'est pas encore fini!
        return -1;
    }

    int nExitCode = p->GetExitCode();
    if( bDestroyProcessInfo )
    {
        DestroyProcessInfo( p );
    }

    // $$$$ AML 03-06-16: return 3FFFFEC6 when killed why ? a quoi ca correspond?
    return nExitCode;
}


//-----------------------------------------------------------------------------
// Name: ProcessManager::UpdateState
// Object: verifie l'etat de tout les processus
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
void ProcessManager::UpdateState( void )
{
    IT_ProcessInfo_Vector itProcess, itProcessEnd;
    for( itProcess = processList_.begin(), itProcessEnd = processList_.end(); itProcess != itProcessEnd; ++itProcess )
    {
         (*itProcess).UpdateState();
    }        
}



//-----------------------------------------------------------------------------
// Name: ProcessManager::GetHandleProcess
// Object: on devrais pas laisser cela possible, mais bon...
// Return: -1 si aucunprocess of this ID...
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
HANDLE ProcessManager::GetHandleProcess( const int nID )
{
    ProcessInfo* p = FindProcess( nID );
    if( p == NULL )
    {
        return (HANDLE)-1;
    }

    return p->GetHandleProcess();

}




//-----------------------------------------------------------------------------
// Name: ProcessManager::FindProcess
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
ProcessInfo* ProcessManager::FindProcess( const int nID ) const
{
    CIT_ProcessInfo_Vector itProcess, itProcessEnd;
    for( itProcess = processList_.begin(), itProcessEnd = processList_.end(); itProcess != itProcessEnd; ++itProcess )
    {
         if( (*itProcess).GetID() == nID )
         {
            return const_cast<ProcessInfo*>(&(*itProcess));
         }
    }

    return NULL;
}


//-----------------------------------------------------------------------------
// Name: ProcessManager::DestroyProcessInfo
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
void ProcessManager::DestroyProcessInfo( const ProcessInfo* p )
{
    assert( p->GetState() == ProcessInfo::eStateDestroyable );

    if( !bKeepTraceAllProcess_ )
    {
        int nRangInVector = ( p - &(*(processList_.begin())) );
        processList_.erase( processList_.begin() + nRangInVector );
    }
}

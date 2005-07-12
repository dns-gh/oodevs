//*****************************************************************************
//
// $Created: AML 03-04-25 $
// $Archive: /MVW_v10/Build/SDK/LAU/src/ProcessManager.h $
// $Author: Aml $
// $Modtime: 11/06/03 12:22 $
// $Revision: 3 $
// $Workfile: ProcessManager.h $
//
//*****************************************************************************

#ifndef __ProcessManager_h_
#define __ProcessManager_h_

#include "LAU.h"

#include "ProcessInfo.h"

//*****************************************************************************
// Object : launch, manage and terminate external process
// Created: AML 03-04-25
//*****************************************************************************
class ProcessManager
{
    MT_COPYNOTALLOWED( ProcessManager );

public:
    explicit            ProcessManager();
    virtual             ~ProcessManager();

    void                Reset();

    int                 LaunchProcess( const char* szPathFilenameExe, const char* szParamsSeparatedBySpaces = NULL, const char* szExecutionPath = NULL ); // return l'ID du process (szParamsSeparatedBySpaces: les parametres sans le nom de l'exe)
    const std::string&  GetAllProcessDescription() const; // retourne un état de la liste de tout les process lancés
    const std::string&  GetProcessDescription( const int nID ) const; // retourne un etat d'un process lancé

    bool32              FinishProcess( const int nID, const bool32 bForceKillIfUnrespond = false ); // return true si le programme a était supprimé

    int                 GetExitCode( const int nID, const bool32 bDestroyProcessInfo = true ); // demande le code de retour d'un process, après cette info, on peut detruire les infos sur le process (sauf si bDestroyProcessInfo est false)

    void                UpdateState( void ); // verifie l'etat de tout les processus (a faire juste pour avoir un GetAllProcessDescription realiste)

    HANDLE              GetHandleProcess( const int nID );  // on devrais pas laisser cela possible, mais bon... (return -1 si aucun process of this ID...)

    bool32              HasProcess( void ) const;

private:
   ProcessInfo*         FindProcess( const int nID ) const;
   void                 DestroyProcessInfo( const ProcessInfo* p );

private:

    bool32                  bKeepTraceAllProcess_;      // keep a trace of all process launched (even the terminate one)

    mutable std::string     strStateDescription_;       // un buffer pour retourner des descriptions de l'objet

    T_ProcessInfo_Vector    processList_;
};

#ifdef USE_INLINE
#	include "ProcessManager.inl"
#endif


#endif // __ProcessManager_h_
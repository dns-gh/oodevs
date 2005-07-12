//*****************************************************************************
//
// $Created: AML 03-04-25 $
// $Archive: /MVW_v10/Build/SDK/LAU/src/ProcessInfo.h $
// $Author: Aml $
// $Modtime: 25/04/03 18:24 $
// $Revision: 3 $
// $Workfile: ProcessInfo.h $
//
//*****************************************************************************

#ifndef __ProcessInfo_h_
#define __ProcessInfo_h_

#include "LAU.h"

//*****************************************************************************
// Object : info of a process (use by ProcessManager class, read it first)
// Created: AML 03-04-25
//*****************************************************************************
class ProcessInfo
{

public:

    typedef enum
    {
	    eStateUnitialized = 1,  // la structure vient juste d'etre initialisé
        eStateLaunched,         // la process a était lancé
        eStateExiting,          // le process a recu une demande d'arret
        eStateKilling,          // le process a recu un gros kill brutal
        eStateNoResponse,       // le process ne repond plus (trouver un moyen de savoir si un process tourne ou et en ne repond plus...)
        eStateZombie,           // le process a été détruit, personne n'a encore demandé son code retour
        eStateDestroyable,      // le process est destructible (on a demandé son code retour)

	    eStateValueMax,
	    eStateForceInt = 0x7fffffff // force 32-bit size enum

    } E_State;


public:
    explicit                ProcessInfo();
                            ProcessInfo( const ProcessInfo& rhs );

    virtual                 ~ProcessInfo();

    void                    Reset();

    ProcessInfo&            operator=( const ProcessInfo& rhs );

    void                    UpdateState();

    bool32                  FinishProcess( const bool32 bForceKillIfUnrespond = false ); // return true si le programme a était supprimé

    const std::string&      GetDescription() const; // retourne un etat d'un process lancé

    //-----------------------------------------------------------------------------
    /** @name Accessors */
    //-----------------------------------------------------------------------------
    //@{
    int         GetID() const;
    E_State     GetState() const;
    int         GetExitCode();

    HANDLE      GetHandleProcess() const;
    HANDLE      GetHandlePrimaryThread() const;

    uint32      GetProcessID() const;
    uint32      GetPrimaryThreadID() const;

    void        SetHandleProcess( HANDLE h );
    void        SetHandlePrimaryThread( HANDLE h );

    void        SetProcessID( uint32 nID );
    void        SetPrimaryThreadID( uint32 nID );
    //@}


    static const char*      GetLibelleFromState( const E_State nState );


private:
    int                     nID_;                           // l'ID du process au point de vue High level (pour le retrouver dans le manager)

    E_State                 nState_;

    int                     nExitCode_;                     // le code de fin du process


    // infos relatives au systemes pour gerer le process
    HANDLE                  hProcess_;
    HANDLE                  hPrimaryThread_;
    uint32                  nProcessID_;
    uint32                  nPrimaryThreadID_;



    mutable std::string     strStateDescription_;           // un buffer pour retourner des descriptions de l'objet

    static uint32           static_nNextID;                 // next id to use
};

typedef std::vector< ProcessInfo > T_ProcessInfo_Vector;
typedef T_ProcessInfo_Vector::iterator IT_ProcessInfo_Vector;
typedef T_ProcessInfo_Vector::const_iterator CIT_ProcessInfo_Vector;
typedef T_ProcessInfo_Vector::reverse_iterator RIT_ProcessInfo_Vector;
typedef T_ProcessInfo_Vector::const_reverse_iterator CRIT_ProcessInfo_Vector;

#ifdef USE_INLINE
#	include "ProcessInfo.inl"
#endif


#endif // __ProcessInfo_h_
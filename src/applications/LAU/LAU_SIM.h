//*****************************************************************************
//
// $Created : AML 03-03-26 $
// $Archive : $
// $Author  : $
// $Modtime : $
// $Revision: 15 $
// $Workfile: LAU_SIM.h $
//
//*****************************************************************************

#ifndef __LAU_SIM_h_
#define __LAU_SIM_h_

#include "LAU_Client_ABC.h"
#include "MT/MT_Time/MT_TimeSpan.h"

class LAU_MOS;
class LAU_MsgMgr;

//*****************************************************************************
// Object : L'image d'une SIM distante
// Created: AML 03-03-26
//*****************************************************************************
class LAU_SIM : public LAU_Client_ABC
{
    MT_COPYNOTALLOWED( LAU_SIM )

    typedef enum
    {
	    eStateUnitialized = 0,
        eStateLaunched,
        eStateConnected,
        eStateNoResponse,
        eStateUnConnected,
        eStateDestroyable,

	    eStateValueMax,
	    eStateForceInt = 0x7fffffff // force 32-bit size enum


    } E_State;

public:
    explicit LAU_SIM( /*MT_TimerManager& timerManager, uint nNbrTimesToRun*/ );
    virtual ~LAU_SIM();
    
    void Reset( void );
    
    //-----------------------------------------------------------------------------
    /** @name Main methods */
    //-----------------------------------------------------------------------------
    //@{
    bool32      CheckExecution( LAU_MsgMgr& msgMgr );    // return true si l'application est encore "vivante"
    void        GetDescription( std::string& strDescription ) const;

    //@}
    
    //-----------------------------------------------------------------------------
    /** @name Accessors */
    //-----------------------------------------------------------------------------
    //@{

    uint16      GetPort() const;
    void        SetPort( const uint16 nVal );

    uint        GetMagic() const;
    void        SetMagic( const uint nVal );

    int32       GetProcessHandle() const;
    void        SetProcessHandle( const int32 nProcessHandle );

    int         GetProcessID() const;
    void        SetProcessID( const int nID );

    bool32      IsPingHasBeenSended() const;
    void        SetPingHasBeenSended( const bool32 bNewState = true );
    void        SetPongHasBeenReceived();

    bool32      IsConnectedToLauncher() const;

    bool32      IsAskToStop() const;
    void        SetAskToStop();

    bool32      IsStopSended() const;
    void        SetStopSended();    

    uint32      GetExerciceID() const;
    void        SetExerciceID( const int32 nExerciceID );

    bool32      HasBeenInitialised() const;
    void        SetHasBeenInitialised( const bool32 bNewState = true );

    int32       GetLaunchErrorCode() const;
    void        SetLaunchErrorCode( const int32 nVal );

    

    void        SetMosClient( LAU_MOS* pMosClient );
    LAU_MOS*    GetMosClient() const;

    //@}

    static      const char* GetLibelleFromState( const E_State nState );
    static      const char* GetLibelleFromLaunchErrorCode( const int32 nLaunchErrorCode );

private:

    uint16                  nPort_;     // c pas deja dans la session du client_abc ?
    uint                    nMagic_;
    
                            
    uint32                  nExerciceID_;
    uint32                  nID_Internal_;
//    E_State                 nState_;
    bool32                  bAskToStop_;            // un stop pour cette sim a était recu, mais comme elle n'es pas encore connecté, on ne lui a pas envoyé (a envoyé lorsque elle se connectera (si elle se connecte)
    bool32                  bStopSended_;           // on a envoyer un StopSim a la sim
    std::string             strExecPath_;

    uint32                  nProcessHandle_;
    int                     nProcessID_;            // l'ID du process dans le process manager

    bool32                  bPingHasBeenSended_;    // on est en attente de la reception d'un pong
    MT_TimeSpan             tNextTimeToSendPing_;
    MT_TimeSpan             tTimeLimitWithoutPong_;

    MT_TimeSpan             tTimeLimitWithoutConnect_; // temps maxi que la sim a pour se connecter (temps maxi pour l'init)

    bool32                  bHasBeenInitialised_;   // la sim a t'elle passé le stade de l'initialisation?
    int32                   nLaunchErrorCode_;      // quel est le code d'erreur retourné par le SimLaunched (-1 si pas encore recu)

    LAU_MOS*                pMosClient_;            // le mos qui a demandé a lancé cette SIM
};


// Add Normalized Access Facilities

typedef std::vector< LAU_SIM > T_LAU_SIM_Vector;
typedef T_LAU_SIM_Vector::iterator IT_LAU_SIM_Vector;
typedef T_LAU_SIM_Vector::const_iterator CIT_LAU_SIM_Vector;
typedef T_LAU_SIM_Vector::reverse_iterator RIT_LAU_SIM_Vector;
typedef T_LAU_SIM_Vector::const_reverse_iterator CRIT_LAU_SIM_Vector;

typedef std::vector< LAU_SIM* > T_LAU_SIM_PtrVector;
typedef T_LAU_SIM_PtrVector::iterator IT_LAU_SIM_PtrVector;
typedef T_LAU_SIM_PtrVector::const_iterator CIT_LAU_SIM_PtrVector;
typedef T_LAU_SIM_PtrVector::reverse_iterator RIT_LAU_SIM_PtrVector;
typedef T_LAU_SIM_PtrVector::const_reverse_iterator CRIT_LAU_SIM_PtrVector;

#ifdef USE_INLINE
#   include "LAU_SIM.inl"
#endif 

#endif // __LAU_SIM_h_

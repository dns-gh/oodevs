//*****************************************************************************
// Created: DFT 02-02-28
//*****************************************************************************

#ifndef __MIL_AgentServer_h_
#define __MIL_AgentServer_h_

#include "MIL.h"

#include "Tools/MIL_Config.h"

#include "MT/MT_Time/MT_Timer_ABC.h"
#include "MT/MT_Time/MT_TimerManager.h"


class MIL_LimaManager;
class MIL_LimitManager;
class MIL_Agent_ABC;
class MIL_CheckPointManager;
class MIL_EntityManager;

class DEC_Workspace;
class DEC_PathFind_Manager;

class NET_AgentServer;

class PHY_MeteoDataManager;

class MIL_ProfilerMgr;

class HLA_Federate;

//*****************************************************************************
// Created: DFT 02-02-28
// Last modified: JVT 03-12-15
//*****************************************************************************
class MIL_AgentServer : public MT_Timer_ABC
{
	MT_COPYNOTALLOWED( MIL_AgentServer )

public:
    //! @name Type
    //@{
    enum E_SimState
    {
        eSimRunning = 0,
        eSimPaused  = 1,
        eSimStopped = 2,
        eSimLoading = 3
    };
    //@}

public:
     MIL_AgentServer( const MIL_Config& config );
    ~MIL_AgentServer();

    //! @name Checkpoints
    //@{
    void save( MIL_CheckPointOutArchive& ) const;
    void load( MIL_CheckPointInArchive& );    
    //@}

    //! @name Simulation management
    //@{
    bool StopSim  ();
    bool ResetSim ();
    bool PauseSim ();
    bool ResumeSim();
    //@}

    //! @name Simulation main loop
    //@{
    E_SimState Update     ();
	void       MainSimLoop();
    //@}

    //! @name Accessors
    //@{ 
    MIL_EntityManager&          GetEntityManager    () const;
	DEC_Workspace&	            GetWorkspaceDIA     () const;
	NET_AgentServer&            GetAgentServer      () const;
    MIL_LimaManager&            GetLimaManager      () const;
    MIL_LimitManager&           GetLimitManager     () const;
    DEC_PathFind_Manager&       GetPathFindManager  () const;
    PHY_MeteoDataManager&       GetMeteoDataManager () const;
    MIL_ProfilerMgr&            GetProfilerManager  () const;
    MIL_CheckPointManager&      GetCheckPointManager() const;
    MT_TimerManager&            GetTimerManager     ();
    HLA_Federate*               GetHLAFederate      () const;               

    uint                        GetExerciceID       () const;
    E_SimState                  GetSimState         () const;
    MIL_Config&                 GetConfig           ();
    //@}

    //! @name Workspace management
    //@{
	static MIL_AgentServer& GetWorkspace    ();
    static void             CreateWorkspace ( const MIL_Config& config );
    static void             DestroyWorkspace();
    //@}

    //! @name Time management
    //@{
    uint GetSimStartTime           () const; // En secondes depuis 01/01/1970
    uint GetCurrentTimeStepRealTime() const; // En secondes depuis 01/01/1970
    uint GetSimTime                () const; // En secondes depuis le démarrage de la SIM

    uint GetCurrentTimeStep () const;
    uint GetTimeStepDuration() const; 

    uint GetTimeFactor() const;
    bool SetTimeFactor( uint nTimeFactor );
    //@}

private:  
    //! @name
    //@{
    void Initialize      ( MIL_InputArchive& initArchive );

    void ReadStaticData  ( MIL_InputArchive& );
    void ReadODB         ( MIL_InputArchive& );
    void ReadIDClasses   ( MIL_InputArchive& );
    void ReadTerData     ( MIL_InputArchive& );
    void ReadHLA         ( MIL_InputArchive& );
    void ReadMissionsData( MIL_InputArchive& );
    void ReadPathFindData( MIL_InputArchive& );
    //@}

    //! @name Timer
    //@{
    void WaitForNextStep();
    void OnTimer();
    //@}

    //! @name Tools
    //@{
    void SendMsgBeginTick() const;
    void SendMsgEndTick  () const;
    //@}
      
private:
    E_SimState nSimState_;

    MIL_Config config_;

    uint nTimeStepDuration_; /* const */
    uint nTimeFactor_;
    uint nCurrentTimeStep_;
    uint nSimTime_;
    uint nCurrentTimeStepRealTime_; // NB : The real time is stored once at the beginning of the tick
    uint nSimStartTime_;            // The time (in seconds from the 01/01/1970) when the sim was started

    MIL_EntityManager*           pEntityManager_;
    DEC_Workspace*	             pWorkspaceDIA_;
    PHY_MeteoDataManager*        pMeteoDataManager_;

    MT_TimerManager              timerManager_;
    MIL_LimaManager*             pLimaManager_;
    MIL_LimitManager*            pLimitManager_;
    DEC_PathFind_Manager*        pPathFindManager_;
    MIL_ProfilerMgr*             pProfilerMgr_;
    MIL_CheckPointManager*       pCheckPointManager_;
    NET_AgentServer*             pAgentServer_;
    HLA_Federate*                pFederate_;

    clock_t lastStep_;
    
private:
	static MIL_AgentServer*      pTheAgentServer_;
};

#include "MIL_AgentServer.inl"


#endif // __MIL_AgentServer_h_


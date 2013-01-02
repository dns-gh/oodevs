// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 MASA Group
//
// *****************************************************************************

#ifndef __MIL_AgentServer_h_
#define __MIL_AgentServer_h_

#include "MIL.h"
#include "Tools/MIL_Config.h"
#include "Network/NET_Simulation_ABC.h"
#include "MT_Tools/MT_Profiler.h"
#include "MT_Tools/MT_Timer_ABC.h"
#include "MT_Tools/MT_TimerManager.h"
#include "MIL_Time_ABC.h"

namespace resource
{
    class ResourceNetworkModel;
    class ResourceTools_ABC;
}

namespace tools
{
    class ExerciseSettings;
}

namespace xml
{
    class xostream;
}

class DEC_Workspace;
class DEC_PathFind_Manager;
class MIL_CheckPointManager;
class MIL_EffectManager;
class MIL_EntityManager;
class NET_AgentServer;
class PHY_MeteoDataManager;
class ProcessMonitor;
class MIL_BurningCells;
class MIL_UrbanCache;
class MIL_ObjectFactory;

//*****************************************************************************
// Created: DFT 02-02-28
// Last modified: JVT 03-12-15
//*****************************************************************************
class MIL_AgentServer : public NET_Simulation_ABC
                      , private MT_Timer_ABC
                      , private MIL_Time_ABC
{
public:
    //! @name Type
    //@{
    enum E_SimState
    {
        eSimRunning = 0,
        eSimPaused  = 1,
        eSimStopped = 2,
        eSimLoading = 3,
        eSimWait    = 4
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_AgentServer( MIL_Config& config );
    virtual ~MIL_AgentServer();
    //@}

    //! @name Checkpoints
    //@{
    void save( MIL_CheckPointOutArchive& ) const;
    void load( MIL_CheckPointInArchive& );

    void WriteODB( xml::xostream& xos ) const;
    void WriteKnowledges( xml::xostream& xos ) const;
    void WriteWeather( xml::xostream& xos ) const;
    void WriteUrban( xml::xostream& xos ) const;
    //@}

    //! @name Simulation management
    //@{
    virtual void Start();
    virtual void Stop();
    virtual void Pause();
    virtual void Continue();
    virtual void Resume( unsigned int ticks );
    virtual void SetTimeFactor( unsigned timeFactor );
    virtual void SetRealTime( const std::string& realTime );
    //@}

    //! @name Simulation main loop
    //@{
    E_SimState Update();
    void MainSimLoop();
    //@}

    //! @name Network
    //@{
    void SendStateToNewClient() const;
    //@}

    //! @name Accessors
    //@{
    MIL_EntityManager& GetEntityManager() const;
    MIL_UrbanCache& GetUrbanCache() const;
    DEC_Workspace& GetWorkspaceDIA() const;
    NET_AgentServer& GetAgentServer() const;
    DEC_PathFind_Manager& GetPathFindManager() const;
    PHY_MeteoDataManager& GetMeteoDataManager() const;
    MIL_CheckPointManager& GetCheckPointManager() const;
    resource::ResourceNetworkModel& GetResourceNetworkModel() const;
    resource::ResourceTools_ABC& GetResourceTools() const;
    MIL_Config& GetConfig();
    MIL_BurningCells& GetBurningCells() const;
    tools::ExerciseSettings& GetSettings() const;
    MIL_ObjectFactory& GetObjectFactory() const;
    //@}

    //! @name Workspace management
    //@{
    static MIL_AgentServer& GetWorkspace();
    static void CreateWorkspace( MIL_Config& config );
    static void DestroyWorkspace();
    static bool IsInitialized();
    //@}

    //! @name Time management
    //@{
    void SetInitialRealTime( unsigned int time );
    unsigned int GetSimTime () const; // Dur�e en secondes depuis le d�but de la SIM
    unsigned int RealTimeToTick( unsigned int rt   ) const;
    unsigned int TickToRealTime( unsigned int tick ) const;
    //@}

private:
    //! @name Time management
    //@{
    virtual unsigned int GetTickDuration() const;
    virtual unsigned int GetCurrentTimeStep() const;
    virtual unsigned int GetRealTime() const; // Temps simul� en secondes depuis 01/01/1970
    //@}

    //! @name
    //@{
    void ReadStaticData();
    void ReadTerData();
    //@}

    //! @name Timer
    //@{
    void Wait();
    void WaitForNextStep();
    void OnTimer();
    //@}

    //! @name Tools
    //@{
    void SendControlInformation() const;
    void SendMsgBeginTick() const;
    void SendMsgEndTick() const;
    //@}

private:
    //! @name Member data
    //@{
    E_SimState nSimState_;
    MIL_Config& config_;
    tools::ExerciseSettings* settings_;
    unsigned int nTimeStepDuration_; /* const */
    unsigned int nTimeFactor_;
    unsigned int nCurrentTimeStep_;
    unsigned int nSimTime_;
    unsigned int nInitialRealTime_;
    unsigned int nRealTime_;
    long lastStep_;
    unsigned int nextPause_;
    double rWaitTime_;
    int waitTicks_;
    int waitLatency_;
    std::string localTime_;
    MT_Profiler profiler_;
    MT_Profiler loopTimer_;
    MIL_EffectManager* pEffectManager_;
    MIL_EntityManager* pEntityManager_;
    DEC_Workspace* pWorkspaceDIA_;
    PHY_MeteoDataManager* pMeteoDataManager_;
    MT_TimerManager timerManager_;
    DEC_PathFind_Manager* pPathFindManager_;
    MIL_CheckPointManager* pCheckPointManager_;
    NET_AgentServer* pAgentServer_;
    MIL_UrbanCache* pUrbanCache_;
    resource::ResourceNetworkModel* pResourceNetworkModel_;
    resource::ResourceTools_ABC* pResourceTools_;
    MIL_BurningCells* pBurningCells_;
    ProcessMonitor* pProcessMonitor_;
    std::auto_ptr< MIL_ObjectFactory > pObjectFactory_;
    //@}

private:
    static MIL_AgentServer* pTheAgentServer_;
};

#include "MIL_AgentServer.inl"

#endif // __MIL_AgentServer_h_

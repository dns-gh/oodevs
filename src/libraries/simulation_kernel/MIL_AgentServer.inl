//*****************************************************************************
// Created: DFT 02-02-28
//*****************************************************************************

//=============================================================================
// SIM CONTROL
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::StopSim
// Created: NLD 2002-09-05
//-----------------------------------------------------------------------------
inline
bool MIL_AgentServer::StopSim()
{
    if( nSimState_ == eSimStopped )
        return false;

    nSimState_ = eSimStopped;
    MT_Timer_ABC::Stop();
    MT_LOG_INFO_MSG( "Simulation stopped" );
    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::PauseSim
// Created: NLD 2002-09-09
//-----------------------------------------------------------------------------
inline
bool MIL_AgentServer::PauseSim()
{
    if( nSimState_ != eSimRunning )
        return false;

    nSimState_ = eSimPaused;
    MT_Timer_ABC::Stop();
    MT_LOG_INFO_MSG( "Simulation paused" );
    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::ResumeSim
// Created: NLD 2002-09-09
//-----------------------------------------------------------------------------
inline
bool MIL_AgentServer::ResumeSim()
{
    if( nSimState_ != eSimPaused )
        return false;

    nSimState_ = eSimRunning;
    MT_Timer_ABC::Start( MT_TimeSpan( (int)( 1000 * nTimeStepDuration_ / nTimeFactor_ ) ) );
    MT_LOG_INFO_MSG( "Simulation resumed" );
    return true;
}


//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::SetTimeFactor
// Created: NLD 2002-09-13
//-----------------------------------------------------------------------------
inline
bool MIL_AgentServer::SetTimeFactor( uint nTimeFactor )
{
    if( nTimeFactor == 0 )
        return false;

    nTimeFactor_ = nTimeFactor;
    if( nSimState_ != eSimRunning )
        return true;

    MT_Timer_ABC::Start( MT_TimeSpan( (int)( 1000 * nTimeStepDuration_ / nTimeFactor_ ) ) );
    MT_LOG_INFO_MSG( MT_FormatString( "Time factor set to %d", nTimeFactor_ ).c_str() )
    return true;
}

//=============================================================================
// ACCESSORS
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetSimStartTime
// Created: NLD 2004-01-19
// -----------------------------------------------------------------------------
inline
uint MIL_AgentServer::GetSimStartTime() const
{
    return nSimStartTime_;
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetTimeFactor
// Created: NLD 2003-03-06
//-----------------------------------------------------------------------------
inline
uint MIL_AgentServer::GetTimeFactor() const
{
    return nTimeFactor_;
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetExerciceID
// Created: NLD 2003-03-04
//-----------------------------------------------------------------------------
inline
uint MIL_AgentServer::GetExerciceID() const
{
    return config_.GetExerciceID();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetConfig
// Created: NLD 2003-12-04
// -----------------------------------------------------------------------------
inline
MIL_Config& MIL_AgentServer::GetConfig()
{
    return config_;
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetWorkspaceDIA
// Created: DFT 02-03-13
//-----------------------------------------------------------------------------
inline
DEC_Workspace& MIL_AgentServer::GetWorkspaceDIA() const
{
    assert( pWorkspaceDIA_ );
	return *pWorkspaceDIA_;
}

//-----------------------------------------------------------------------------
// Name: NET_AgentServer::GetAgentServer
// Created: JVT 2002-07-17
//-----------------------------------------------------------------------------
inline
NET_AgentServer& MIL_AgentServer::GetAgentServer() const
{
    assert( pAgentServer_ );
	return *pAgentServer_;
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetTacticalLineManager
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
inline
MIL_TacticalLineManager& MIL_AgentServer::GetTacticalLineManager() const
{
    assert( pTacticalLineManager_ );
    return *pTacticalLineManager_;
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetMeteoDataManager
// Created: JVT 02-10-21
//-----------------------------------------------------------------------------
inline
PHY_MeteoDataManager& MIL_AgentServer::GetMeteoDataManager() const
{
    assert( pMeteoDataManager_ );
    return *pMeteoDataManager_;
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetPathFindManager
// Created: JDY 03-02-12
//-----------------------------------------------------------------------------
inline
DEC_PathFind_Manager& MIL_AgentServer::GetPathFindManager() const
{
    assert( pPathFindManager_ );
    return *pPathFindManager_;
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetProfilerManager
// Created: AML 03-05-21
//-----------------------------------------------------------------------------
inline
MIL_ProfilerMgr& MIL_AgentServer::GetProfilerManager() const
{
    assert( pProfilerMgr_ );
    return *pProfilerMgr_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetTimerManager
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
inline
MT_TimerManager& MIL_AgentServer::GetTimerManager()
{
    return timerManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetHLAFederate
// Created: AGE 2004-11-26
// -----------------------------------------------------------------------------
inline
HLA_Federate* MIL_AgentServer::GetHLAFederate() const
{
    return pFederate_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetCheckPointManager
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
inline
MIL_CheckPointManager& MIL_AgentServer::GetCheckPointManager() const
{
    assert( pCheckPointManager_ );
    return *pCheckPointManager_;
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetSimTime
// Created: JVT 02-08-02
//-----------------------------------------------------------------------------
inline
uint MIL_AgentServer::GetSimTime() const
{
    return nSimTime_;
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetRealTime
// Created: JVT 02-08-02
//-----------------------------------------------------------------------------
inline
uint MIL_AgentServer::GetCurrentTimeStepRealTime() const
{
    return nCurrentTimeStepRealTime_;
}


//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetCurrentTimeStep
// Created: JVT 02-08-02
//-----------------------------------------------------------------------------
inline
uint MIL_AgentServer::GetCurrentTimeStep() const
{
    return nCurrentTimeStep_;
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetTimeStepDuration
// Created: NLD 2002-09-10
//-----------------------------------------------------------------------------
inline
uint MIL_AgentServer::GetTimeStepDuration() const
{
    return nTimeStepDuration_;
}


//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetSimState
// Created: NLD 2003-05-23
//-----------------------------------------------------------------------------
inline
MIL_AgentServer::E_SimState MIL_AgentServer::GetSimState() const
{
    return nSimState_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetEntityManager
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
inline
MIL_EntityManager& MIL_AgentServer::GetEntityManager() const
{
    assert( pEntityManager_ );
    return *pEntityManager_;
}

//=============================================================================
// STATICS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetWorkspace
// Created: DFT 02-03-13
//-----------------------------------------------------------------------------
inline
MIL_AgentServer& MIL_AgentServer::GetWorkspace()
{
    assert( pTheAgentServer_ );
	return *pTheAgentServer_;
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::CreateWorkspace
// Created: NLD 2002-09-09
//-----------------------------------------------------------------------------
inline
void MIL_AgentServer::CreateWorkspace( const MIL_Config& config )
{
    assert( pTheAgentServer_ == 0 );
    pTheAgentServer_ = new MIL_AgentServer( config );
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::DestroyWorkspace
// Created: NLD 2002-09-09
//-----------------------------------------------------------------------------
inline
void MIL_AgentServer::DestroyWorkspace()
{
    if( pTheAgentServer_ )
    {
        delete pTheAgentServer_;
        pTheAgentServer_ = 0;
    }
}


// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 MASA Group
//
// *****************************************************************************

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
unsigned int MIL_AgentServer::GetSimTime() const
{
    return nSimTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::SetInitialRealTime
// Created: AGE 2007-10-12
// -----------------------------------------------------------------------------
inline
void MIL_AgentServer::SetInitialRealTime( unsigned int time )
{
    nRealTime_ = nInitialRealTime_ = time;
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetRealTime
// Created: JVT 02-08-02
//-----------------------------------------------------------------------------
inline
unsigned int MIL_AgentServer::GetRealTime() const
{
    return nRealTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetCurrentTick
// Created: AGE 2007-08-10
// -----------------------------------------------------------------------------
inline
unsigned int MIL_AgentServer::GetCurrentTick() const
{
    return nCurrentTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetCurrentTimeStep
// Created: AGE 2007-08-10
// -----------------------------------------------------------------------------
inline
unsigned int MIL_AgentServer::GetCurrentTimeStep() const
{
    return nCurrentTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetTickDuration
// Created: AGE 2007-08-13
// -----------------------------------------------------------------------------
inline
unsigned int MIL_AgentServer::GetTickDuration() const
{
    return nTimeStepDuration_;
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetTimeStepDuration
// Created: NLD 2002-09-10
//-----------------------------------------------------------------------------
inline
unsigned int MIL_AgentServer::GetTimeStepDuration() const
{
    return nTimeStepDuration_;
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

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::UrbanModel
// Created: SLG 2009-10-11
// -----------------------------------------------------------------------------
inline
urban::Model& MIL_AgentServer::GetUrbanModel() const
{
    assert( pUrbanModel_ );
    return *pUrbanModel_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetResourceNetworkModel
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
inline
resource::ResourceNetworkModel& MIL_AgentServer::GetResourceNetworkModel() const
{
    assert( pResourceNetworkModel_ );
    return *pResourceNetworkModel_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetResourceTools
// Created: JSR 2010-09-03
// -----------------------------------------------------------------------------
inline
resource::ResourceTools_ABC& MIL_AgentServer::GetResourceTools() const
{
    assert( pResourceTools_ );
    return *pResourceTools_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetBurningCells
// Created: BCI 2010-12-22
// -----------------------------------------------------------------------------
inline
MIL_BurningCells& MIL_AgentServer::GetBurningCells() const
{
    assert( pBurningCells_ );
    return *pBurningCells_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetPropagationManager
// Created: BCI 2010-12-22
// -----------------------------------------------------------------------------
inline
MIL_PropagationManager& MIL_AgentServer::GetPropagationManager() const
{
    assert( pPropagationManager_ );
    return *pPropagationManager_;
}

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
void MIL_AgentServer::CreateWorkspace( MIL_Config& config )
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

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::IsInitialized
// Created: JSR 2010-11-22
// -----------------------------------------------------------------------------
inline
bool MIL_AgentServer::IsInitialized()
{
    return pTheAgentServer_ != 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentServer::GetSettings
// Created: ABR 2011-12-13
// -----------------------------------------------------------------------------
inline
tools::ExerciseSettings& MIL_AgentServer::GetSettings() const
{
    assert( settings_ != 0 );
    return *settings_;
}

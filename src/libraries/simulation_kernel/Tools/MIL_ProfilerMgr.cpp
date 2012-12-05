//*****************************************************************************
//
// $Created: AML 03-05-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Tools/MIL_ProfilerMgr.cpp $
// $Author: Nld $
// $Modtime: 21/04/05 12:04 $
// $Revision: 5 $
// $Workfile: MIL_ProfilerMgr.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ProfilerMgr.h"
#include "MIL_AgentServer.h"
#include "MIL_Config.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Orders/MIL_AutomateOrderManager.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "MT_Tools/MT_Logger.h"
#include <direct.h>

//-----------------------------------------------------------------------------
// Name: MIL_ProfilerMgr constructor
// Created: AML 03-05-21
//-----------------------------------------------------------------------------
MIL_ProfilerMgr::MIL_ProfilerMgr( const MIL_Config& config )
    : bEnabled_( config.IsProfilingEnabled() )
{
    if( bEnabled_ )
        MT_LOG_INFO_MSG( "Profiling mode enabled at startup time : dec functions profiled" )
    else
        MT_LOG_INFO_MSG( "Profiling mode not enabled at startup time : dec functions won't be profiled" );
    _mkdir( "./Profiling/");
    decisionUpdateFile_.open( "./Profiling/DecisionUpdate.txt", std::ios_base::out | std::ios_base::trunc );
}

//-----------------------------------------------------------------------------
// Name: MIL_ProfilerMgr destructor
// Created: AML 03-05-21
//-----------------------------------------------------------------------------
MIL_ProfilerMgr::~MIL_ProfilerMgr()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ProfilerMgr::NotifyDecisionUpdated
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
void MIL_ProfilerMgr::NotifyDecisionUpdated( const MIL_AgentPion& pion, double rTime )
{
    if( !bEnabled_ )
        return;
    decisionUpdateFile_ << MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() << " P " << pion.GetID() << " \"" << pion.GetOrderManager().GetMissionName() << "\" " << rTime << std::endl;
}

// -----------------------------------------------------------------------------
// Name: MIL_ProfilerMgr::NotifyDecisionUpdated
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
void MIL_ProfilerMgr::NotifyDecisionUpdated( const MIL_Automate& automate, double rTime )
{
    if( !bEnabled_ )
        return;
    decisionUpdateFile_ << MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() << " A " << automate.GetID() << " \"" << automate.GetOrderManager().GetMissionName() << "\" " << rTime << std::endl;
}

// -----------------------------------------------------------------------------
// Name: MIL_ProfilerMgr::NotifyDecisionUpdated
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
void MIL_ProfilerMgr::NotifyDecisionUpdated( const MIL_Population& population, double rTime )
{
    if( !bEnabled_ )
        return;
    decisionUpdateFile_ << MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() << " Pop " << population.GetID() << " \"" << population.GetOrderManager().GetMissionName() << "\" " << rTime << std::endl;
}

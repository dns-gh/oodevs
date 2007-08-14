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

#include "Entities/Populations/MIL_Population.h"
#include "MIL_AgentServer.h"

#include <direct.h>

//-----------------------------------------------------------------------------
// Name: MIL_ProfilerMgr constructor
// Created: AML 03-05-21
//-----------------------------------------------------------------------------
MIL_ProfilerMgr::MIL_ProfilerMgr( bool bEnabled )
    : bEnabled_( bEnabled )
{
    if( bEnabled_ )
        MT_LOG_INFO_MSG( "Profiling mode enabled at startup time : dec functions profiled" )
    else
        MT_LOG_INFO_MSG( "Profiling mode not enabled at startup time : dec functions won't be profiled" )

    _mkdir( "./Profiling/");
    decFunctionsFile_  .open( "./Profiling/DecFunctions.txt"  , std::ios_base::out | std::ios_base::trunc );
    decisionUpdateFile_.open( "./Profiling/DecisionUpdate.txt", std::ios_base::out | std::ios_base::trunc );
}

//-----------------------------------------------------------------------------
// Name: MIL_ProfilerMgr destructor
// Created: AML 03-05-21
//-----------------------------------------------------------------------------
MIL_ProfilerMgr::~MIL_ProfilerMgr()
{
    decFunctionsFile_  .close();
    decisionUpdateFile_.close();
}

//-----------------------------------------------------------------------------
// Name: MIL_ProfilerMgr::NotifyTickEnd
// Created: AML 03-05-21
//-----------------------------------------------------------------------------
void MIL_ProfilerMgr::NotifyTickEnd( uint nTick )
{
    tickDurationProfiler_.Stop();

    if( !decFunctionProfilingVector_.empty() )
    {
        for( CIT_DecFunctionProfilingVector itData = decFunctionProfilingVector_.begin(); itData != decFunctionProfilingVector_.end(); ++itData )
            decFunctionsFile_ << nTick << ' ' << itData->nAgentID_ << ' ' << itData->strFunction_ << ' ' << itData->rTime_ << std::endl;
        decFunctionsFile_.flush();
        decFunctionProfilingVector_.clear(); 
        decFunctionProfilingVector_.reserve( 1000 );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_ProfilerMgr::NotifyDecisionUpdated
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
void MIL_ProfilerMgr::NotifyDecisionUpdated( const MIL_AgentPion& pion, MT_Float rTime )
{
    if( !bEnabled_ )
        return;
    decisionUpdateFile_ << MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() << " P " << pion.GetID() << " \"" << pion.GetOrderManager().GetMissionName() << "\" " << rTime << std::endl;
    decisionUpdateFile_.flush();
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ProfilerMgr::NotifyDecisionUpdated
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
void MIL_ProfilerMgr::NotifyDecisionUpdated( const MIL_Automate& automate, MT_Float rTime )
{
    if( !bEnabled_ )
        return;
    decisionUpdateFile_ << MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() << " A " << automate.GetID() << " \"" << automate.GetOrderManager().GetMissionName() << "\" " << rTime << std::endl;
    decisionUpdateFile_.flush();
}

    
// -----------------------------------------------------------------------------
// Name: MIL_ProfilerMgr::NotifyDecisionUpdated
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
void MIL_ProfilerMgr::NotifyDecisionUpdated( const MIL_Population& population, MT_Float rTime )
{
    if( !bEnabled_ )
        return;
    decisionUpdateFile_ << MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() << " Pop " << population.GetID() << " \"" << population.GetOrderManager().GetMissionName() << "\" " << rTime << std::endl;
    decisionUpdateFile_.flush();
}

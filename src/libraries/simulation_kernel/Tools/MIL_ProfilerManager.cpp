//*****************************************************************************
//
// $Created: AML 03-05-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Tools/MIL_ProfilerManager.cpp $
// $Author: Nld $
// $Modtime: 21/04/05 12:04 $
// $Revision: 5 $
// $Workfile: MIL_ProfilerManager.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ProfilerManager.h"
#include "MIL_Time_ABC.h"
#include "MIL_Config.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Orders/MIL_AutomateOrderManager.h"
#include "Entities/Orders/MIL_PopulationOrderManager.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "MT_Tools/MT_Logger.h"
#include <direct.h>

//-----------------------------------------------------------------------------
// Name: MIL_ProfilerManager constructor
// Created: AML 03-05-21
//-----------------------------------------------------------------------------
MIL_ProfilerManager::MIL_ProfilerManager( const MIL_Config& config )
    : bEnabled_( config.IsDecisionalProfilingEnabled() )
{
    if( bEnabled_ )
        MT_LOG_INFO_MSG( "Profiling mode enabled at startup time : dec functions profiled" );
    else
        MT_LOG_INFO_MSG( "Profiling mode not enabled at startup time : dec functions won't be profiled" );
    try
    {
        tools::Path( "./Profiling" ).CreateDirectories();
        decisionUpdateFile_.open( "./Profiling/DecisionUpdate.txt", std::ios_base::out | std::ios_base::trunc );
    }
    catch( std::exception& )
    {
         // NOTHING
    }
}

//-----------------------------------------------------------------------------
// Name: MIL_ProfilerManager destructor
// Created: AML 03-05-21
//-----------------------------------------------------------------------------
MIL_ProfilerManager::~MIL_ProfilerManager()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ProfilerManager::NotifyDecisionUpdated
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
void MIL_ProfilerManager::NotifyDecisionUpdated( const MIL_AgentPion& pion, double rTime )
{
    if( !bEnabled_ )
        return;
    decisionUpdateFile_ << MIL_Time_ABC::GetTime().GetCurrentTimeStep() << " P " << pion.GetID() << " \"" << pion.GetOrderManager().GetMissionName() << "\" " << rTime << std::endl;
}

// -----------------------------------------------------------------------------
// Name: MIL_ProfilerManager::NotifyDecisionUpdated
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
void MIL_ProfilerManager::NotifyDecisionUpdated( const MIL_Automate& automate, double rTime )
{
    if( !bEnabled_ )
        return;
    decisionUpdateFile_ << MIL_Time_ABC::GetTime().GetCurrentTimeStep() << " A " << automate.GetID() << " \"" << automate.GetOrderManager().GetMissionName() << "\" " << rTime << std::endl;
}

// -----------------------------------------------------------------------------
// Name: MIL_ProfilerManager::NotifyDecisionUpdated
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
void MIL_ProfilerManager::NotifyDecisionUpdated( const MIL_Population& population, double rTime )
{
    if( !bEnabled_ )
        return;
    decisionUpdateFile_ << MIL_Time_ABC::GetTime().GetCurrentTimeStep() << " Pop " << population.GetID() << " \"" << population.GetOrderManager().GetMissionName() << "\" " << rTime << std::endl;
}

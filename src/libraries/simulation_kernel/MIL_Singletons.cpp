// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Singletons.h"
#include "MIL_AgentServer.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/Orders/MIL_TacticalLineManager.h"

namespace
{
    MIL_Time_ABC* pTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Singletons::GetTime
// Created: AGE 2007-08-13
// -----------------------------------------------------------------------------
MIL_Time_ABC& MIL_Singletons::GetTime()
{
    return pTime_ ? *pTime_ : MIL_AgentServer::GetWorkspace();
}

// -----------------------------------------------------------------------------
// Name: MIL_Singletons::RegisterTime
// Created: LDC 2010-01-04
// -----------------------------------------------------------------------------
void MIL_Singletons::RegisterTime( MIL_Time_ABC& time )
{
    if( pTime_ )
        throw std::runtime_error( "Time already registered" );
    pTime_ = &time;
}

// -----------------------------------------------------------------------------
// Name: MIL_Singletons::UnregisterTime
// Created: LDC 2010-01-04
// -----------------------------------------------------------------------------
void MIL_Singletons::UnregisterTime( MIL_Time_ABC& time )
{
    if( &time != pTime_ )
        throw std::runtime_error( "Unregistering wrong time" );
    pTime_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_Singletons::GetEntityManager
// Created: AGE 2007-08-13
// -----------------------------------------------------------------------------
MIL_EntityManager_ABC& MIL_Singletons::GetEntityManager()
{
    return MIL_EntityManager_ABC::GetSingleton();
}

// -----------------------------------------------------------------------------
// Name: MIL_Singletons::GetProfiler
// Created: AGE 2007-08-13
// -----------------------------------------------------------------------------
MIL_ProfilerMgr& MIL_Singletons::GetProfiler()
{
    return MIL_AgentServer::GetWorkspace().GetProfilerManager();
}

MIL_TacticalLineManager& MIL_Singletons::GetTacticalLineManager()
{
    return MIL_TacticalLineManager::GetSingleton();
}
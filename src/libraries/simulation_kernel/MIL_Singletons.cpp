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

// -----------------------------------------------------------------------------
// Name: MIL_Singletons::GetTime
// Created: AGE 2007-08-13
// -----------------------------------------------------------------------------
MIL_Time_ABC& MIL_Singletons::GetTime()
{
    return MIL_AgentServer::GetWorkspace();
}

// -----------------------------------------------------------------------------
// Name: MIL_Singletons::GetEntityManager
// Created: AGE 2007-08-13
// -----------------------------------------------------------------------------
MIL_EntityManager& MIL_Singletons::GetEntityManager()
{
    return MIL_EntityManager::GetSingleton();
}

// -----------------------------------------------------------------------------
// Name: MIL_Singletons::GetHla
// Created: AGE 2007-08-13
// -----------------------------------------------------------------------------
HLA_Federate* MIL_Singletons::GetHla()
{
    return MIL_AgentServer::GetWorkspace().GetHLAFederate();
}

// -----------------------------------------------------------------------------
// Name: MIL_Singletons::GetProfiler
// Created: AGE 2007-08-13
// -----------------------------------------------------------------------------
MIL_ProfilerMgr& MIL_Singletons::GetProfiler()
{
    return MIL_AgentServer::GetWorkspace().GetProfilerManager();
}

// -----------------------------------------------------------------------------
// Name: MIL_Singletons::GetEffectManager
// Created: AGE 2007-08-13
// -----------------------------------------------------------------------------
MIL_EffectManager& MIL_Singletons::GetEffectManager()
{
    return MIL_AgentServer::GetWorkspace().GetEffectManager();
    
}

//*****************************************************************************
//
// $Created: AML 03-05-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Tools/MIL_ProfilerMgr.inl $
// $Author: Jvt $
// $Modtime: 12/04/05 14:55 $
// $Revision: 4 $
// $Workfile: MIL_ProfilerMgr.inl $
//
//*****************************************************************************

#include "Entities/Agents/MIL_AgentPion.h"

//-----------------------------------------------------------------------------
// Name: MIL_ProfilerMgr::NotifyTickBegin
// Created: AML 03-05-21
//-----------------------------------------------------------------------------
inline
void MIL_ProfilerMgr::NotifyTickBegin( unsigned int /*nTick*/ )
{
    tickDurationProfiler_.Start();
}

//-----------------------------------------------------------------------------
// Name: MIL_ProfilerMgr::GetLastTickDuration
// Created: AML 03-05-21
//-----------------------------------------------------------------------------
inline
double MIL_ProfilerMgr::GetLastTickDuration()
{
    return tickDurationProfiler_.GetLastTime();
}

// -----------------------------------------------------------------------------
// Name: MIL_ProfilerMgr::IsProfilingEnabled
// Created: NLD 2005-03-15
// -----------------------------------------------------------------------------
inline
bool MIL_ProfilerMgr::IsProfilingEnabled() const
{
    return bEnabled_;
}

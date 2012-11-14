// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MT_Profiler.h"
#include "MT_Assert.h"
#include <windows.h>

namespace
{
    static long long int frequency = 0;

    static struct MT_ProfilerInitializer
    {
        MT_ProfilerInitializer()
        {
            QueryPerformanceFrequency( (LARGE_INTEGER*)&frequency );
            frequency /= 1000; // For ms instead of s
        }
    } initializer;
}

//-----------------------------------------------------------------------------
// Name: MT_Profiler constructor
// Created: NLD 2002-10-14
//-----------------------------------------------------------------------------
MT_Profiler::MT_Profiler()
    : nCounterStart_    ( 0 )
    , rLastTime_        ( 0. )
    , rTotalTime_       ( 0. )
    , nNbrCount_        ( 0 )
{
    assert( frequency != 0 ); // Call Initialize before instanciation
}

//-----------------------------------------------------------------------------
// Name: MT_Profiler::Reset
// Created: AML 03-05-21
//-----------------------------------------------------------------------------
void MT_Profiler::Reset()
{
    nCounterStart_  = 0;
    rLastTime_      = 0.;
    rTotalTime_     = 0.;
    nNbrCount_      = 0;
}

//-----------------------------------------------------------------------------
// Name: MT_Profiler::Start
// Created: NLD 2002-10-14
//-----------------------------------------------------------------------------
void MT_Profiler::Start()
{
    QueryPerformanceCounter( (LARGE_INTEGER*)&nCounterStart_ );
}

//-----------------------------------------------------------------------------
// Name: MT_Profiler::Stop
// Created: NLD 2002-10-14
//-----------------------------------------------------------------------------
double MT_Profiler::Stop()
{
    if( frequency == 0 )
        return 0.;
    long long int nCounterStop;
    QueryPerformanceCounter( (LARGE_INTEGER*)&nCounterStop );
    double rTime = ( nCounterStop - nCounterStart_ ) / (double)frequency;
    rLastTime_   = rTime;
    rTotalTime_ += rTime;
    ++nNbrCount_;
    // stop the counter
    nCounterStart_ = 0;
    return rTime;
}

// -----------------------------------------------------------------------------
// Name: MT_Profiler::GetTotalTime
// Created: MCO 2012-11-05
// -----------------------------------------------------------------------------
double MT_Profiler::GetTotalTime() const
{
    return rTotalTime_;
}

// -----------------------------------------------------------------------------
// Name: MT_Profiler::GetCount
// Created: MCO 2012-11-05
// -----------------------------------------------------------------------------
int MT_Profiler::GetCount() const
{
    return nNbrCount_;
}

//*****************************************************************************
//
// $Created: NLD 2002-10-14 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Tools/MT_Profiler.cpp $
// $Author: Jvt $
// $Modtime: 12/04/05 14:54 $
// $Revision: 2 $
// $Workfile: MT_Profiler.cpp $
//
//*****************************************************************************

#include "MIL_Pch.h"

#include "MT_Profiler.h"

#include <windows.h>

int64 MT_Profiler::nFrequency_ = 0;

//-----------------------------------------------------------------------------
// Name: MT_Profiler::Initialize
// Created: NLD 2002-10-14
//-----------------------------------------------------------------------------
void MT_Profiler::Initialize()
{
    QueryPerformanceFrequency( (LARGE_INTEGER*)&nFrequency_ );
    nFrequency_ /= 1000; // For ms instead of s
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
    assert( nFrequency_ != 0 ); // Call Initialize before instanciation
}    


//-----------------------------------------------------------------------------
// Name: MT_Profiler destructor
// Created: NLD 2002-10-14
//-----------------------------------------------------------------------------
MT_Profiler::~MT_Profiler()
{
    
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
    if( nFrequency_ == 0 )
        return 0.;

    int64 nCounterStop;

    QueryPerformanceCounter( (LARGE_INTEGER*)&nCounterStop );

	double rTime = ( nCounterStop - nCounterStart_ ) / (double)nFrequency_;

    rLastTime_   = rTime;
    rTotalTime_ += rTime;
    ++nNbrCount_;

    // stop the counter
    nCounterStart_ = 0;

    return rTime;
}



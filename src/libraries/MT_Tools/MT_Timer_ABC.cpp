// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "MT_Tools_pch.h"
#include "MT_Timer_ABC.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <windows.h>

namespace
{
    int64 GetSystemTime()
    {
        FILETIME ft;
        GetSystemTimeAsFileTime(&ft);
        return boost::date_time::winapi::file_time_to_microseconds( ft ) / 1000ul;
    }
}

//-----------------------------------------------------------------------------
// Name: MT_Timer_ABC constructor
// Created: NLD 2002-09-26
//-----------------------------------------------------------------------------
MT_Timer_ABC::MT_Timer_ABC()
    : tPeriod_          ( 0 )
    , tLastTimeLaunched_( GetSystemTime() )
    , bStarted_         ( false )
    , bPaused_          ( false )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MT_Timer_ABC destructor
// Created:  NLD 2002-03-12 
//-----------------------------------------------------------------------------
MT_Timer_ABC::~MT_Timer_ABC()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MT_Timer_ABC::Process
/**
    @param  currentTime EXPLANATION
    @return False if the time must be removed from the timer 

  PUT YOUR COMMENTS HERE
*/
// Created:  NLD 00-09-29 
//-----------------------------------------------------------------------------
void MT_Timer_ABC::Process( int64 currentTime )
{
    if( !bStarted_ || bPaused_ )
        return;
    if( currentTime - tLastTimeLaunched_ < tPeriod_ )
        return;
    tLastTimeLaunched_ = currentTime;
    OnTimer();
}

//-----------------------------------------------------------------------------
// Name: MT_Timer_ABC::Start
/** @param  tPeriod Delay between each 'OnTimer' call

  Start the timer
*/
// Created:  NLD 00-09-29 
//-----------------------------------------------------------------------------
void MT_Timer_ABC::Start( int64 tPeriod )
{
    tPeriod_  = tPeriod;
    bStarted_ = true;
}

//-----------------------------------------------------------------------------
// Name: MT_Timer_ABC::Stop
/** 
  Stop the timer (OnTimerStop will be called) 
*/
// Created:  NLD 00-09-29 
//-----------------------------------------------------------------------------
void MT_Timer_ABC::Stop()
{
    bStarted_ = false;
}

//-----------------------------------------------------------------------------
// Name: MT_Timer_ABC::Pause
// Created: NLD 2002-09-26
//-----------------------------------------------------------------------------
void MT_Timer_ABC::Pause()
{
    bPaused_ = true;
}

//-----------------------------------------------------------------------------
// Name: MT_Timer_ABC::Resume
// Created: NLD 2002-09-26
//-----------------------------------------------------------------------------
void MT_Timer_ABC::Resume()
{
    bPaused_ = false;
}

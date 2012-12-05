// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MT_TimerManager.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <algorithm>
#include <windows.h>

//-----------------------------------------------------------------------------
// Name: MT_TimerManager constructor
// Created:  NLD 00-09-29
//-----------------------------------------------------------------------------
MT_TimerManager::MT_TimerManager()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MT_TimerManager constructor
// Created:  NLD 00-09-29
//-----------------------------------------------------------------------------
MT_TimerManager::~MT_TimerManager()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MT_TimerManager::Register
// Created:  NLD 00-09-29
//-----------------------------------------------------------------------------
void MT_TimerManager::Register( MT_Timer_ABC& timer )
{
    timers_.push_back( &timer );
}

//-----------------------------------------------------------------------------
// Name: MT_TimerManager::Unregister
// Created:  MBN 00-05-04
//-----------------------------------------------------------------------------
void MT_TimerManager::Unregister( MT_Timer_ABC& timer )
{
    timers_.remove( &timer );
}

namespace
{
    long long int GetSystemTime()
    {
        FILETIME ft;
        GetSystemTimeAsFileTime( &ft );
        return boost::date_time::winapi::file_time_to_microseconds( ft ) / 1000ul;
    }
}

//-----------------------------------------------------------------------------
// Name: MT_TimerManager::Update
// Created:  NLD 00-09-29
//-----------------------------------------------------------------------------
void MT_TimerManager::Update() const
{
    const long long int current = GetSystemTime();
    for( auto it = timers_.begin(); it != timers_.end(); ++it )
        (*it)->Process( current );
}

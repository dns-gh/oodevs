// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Time_ABC.h"

namespace
{
    const MIL_Time_ABC* pTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Time_ABC::GetTime
// Created: SLI 2012-12-06
// -----------------------------------------------------------------------------
const MIL_Time_ABC& MIL_Time_ABC::GetTime()
{
    return *pTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Time_ABC::RegisterTime
// Created: LDC 2010-01-04
// -----------------------------------------------------------------------------
void MIL_Time_ABC::RegisterTime( const MIL_Time_ABC& time )
{
    if( pTime_ )
        throw MASA_EXCEPTION( "Time already registered" );
    pTime_ = &time;
}

// -----------------------------------------------------------------------------
// Name: MIL_Time_ABC::UnregisterTime
// Created: LDC 2010-01-04
// -----------------------------------------------------------------------------
void MIL_Time_ABC::UnregisterTime( const MIL_Time_ABC& time )
{
    if( &time != pTime_ )
        throw MASA_EXCEPTION( "Unregistering wrong time" );
    pTime_ = 0;
}

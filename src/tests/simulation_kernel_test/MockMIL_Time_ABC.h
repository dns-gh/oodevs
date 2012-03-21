// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MockMIL_Time_ABC_h_
#define __MockMIL_Time_ABC_h_

#include "MIL_Time_ABC.h"
#include "MIL_Singletons.h"

MOCK_BASE_CLASS( MockMIL_Time_ABC, MIL_Time_ABC )
{
             MockMIL_Time_ABC() { MIL_Singletons::RegisterTime( *this ); }
    virtual ~MockMIL_Time_ABC() { MIL_Singletons::UnregisterTime( *this ); }

    MOCK_METHOD( GetCurrentTick, 0 );
    MOCK_METHOD( GetTickDuration, 0 );
    MOCK_METHOD( GetRealTime, 0 );
    MOCK_METHOD( GetCurrentTimeStep, 0 );
};

#endif // __MockMIL_Time_ABC_h_

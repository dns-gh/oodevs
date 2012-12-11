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

MOCK_BASE_CLASS( MockMIL_Time_ABC, MIL_Time_ABC )
{
             MockMIL_Time_ABC() { MIL_Time_ABC::RegisterTime( *this ); }
    virtual ~MockMIL_Time_ABC() { MIL_Time_ABC::UnregisterTime( *this ); }

    MOCK_METHOD( GetCurrentTick, 0 );
    MOCK_METHOD( GetTickDuration, 0 );
    MOCK_METHOD( GetRealTime, 0 );
    MOCK_METHOD( GetCurrentTimeStep, 0 );
};

#endif // __MockMIL_Time_ABC_h_

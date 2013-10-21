// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef MOCK_PROCESS_H
#define MOCK_PROCESS_H

#include "runtime/Process_ABC.h"

namespace mocks
{
    MOCK_BASE_CLASS( MockProcess, runtime::Process_ABC )
    {
        MockProcess( int pid, std::string name )
        {
            MOCK_EXPECT( GetPid ).returns( pid );
            MOCK_EXPECT( GetName ).returns( name );
            MOCK_EXPECT( IsAlive ).returns( true );
        }
        MOCK_METHOD( GetPid, 0 );
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( IsAlive, 0 );
        MOCK_METHOD( Join, 1 );
        MOCK_METHOD( Kill, 0 );
    };
};

#endif // MOCK_PROCESS_H

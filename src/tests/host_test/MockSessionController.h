// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef MOCK_SESSION_CONTROLLER_H
#define MOCK_SESSION_CONTROLLER_H

#include "host/SessionController_ABC.h"

namespace mocks
{
    MOCK_BASE_CLASS( MockSessionController, host::SessionController_ABC )
    {
        MockSessionController()
        {
            MOCK_EXPECT( Reload ).once();
        }
        MOCK_METHOD( Reload, 1 );
        MOCK_METHOD( List, 3 );
        MOCK_METHOD( Count, 1 );
        MOCK_METHOD( Get, 2 );
        MOCK_METHOD( Create, 3 );
        MOCK_METHOD( Delete, 2 );
        MOCK_METHOD( DeleteUser, 2 );
        MOCK_METHOD( Start, 3 );
        MOCK_METHOD( Stop, 2 );
        MOCK_METHOD( Pause, 2 );
        MOCK_METHOD( Update, 3 );
        MOCK_METHOD( Archive, 2 );
        MOCK_METHOD( Restore, 2 );
        MOCK_METHOD( Download, 3 );
        MOCK_METHOD( Replay, 2 );
        MOCK_METHOD( NotifyNode, 1 );
        MOCK_METHOD( DownloadLog, 6 );
        MOCK_METHOD( Filter, 2 );
    };
};

#endif // MOCK_SESSION_CONTROLLER_H

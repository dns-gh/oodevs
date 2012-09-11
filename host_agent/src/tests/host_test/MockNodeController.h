// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef MOCK_NODE_CONTROLLER_H
#define MOCK_NODE_CONTROLLER_H

#include "host/NodeController_ABC.h"

namespace mocks
{
    MOCK_BASE_CLASS( MockNodeController, host::NodeController_ABC )
    {
        MockNodeController( bool mustReload )
        {
            if( mustReload )
                MOCK_EXPECT( Reload ).once();
        }
        MOCK_METHOD( Reload, 0 );
        MOCK_METHOD( List, 2 );
        MOCK_METHOD( Count, 0 );
        MOCK_METHOD( Has, 1 );
        MOCK_METHOD( Get, 1 );
        MOCK_METHOD( Create, 2 );
        MOCK_METHOD( Delete, 1 );
        MOCK_METHOD( Start, 1 );
        MOCK_METHOD( Stop, 1 );
        MOCK_METHOD( Update, 2 );
        MOCK_METHOD( GetInstall, 1 );
        MOCK_METHOD( DeleteInstall, 2 );
        MOCK_METHOD( DownloadInstall, 3 );
        MOCK_METHOD( GetCache, 1 );
        MOCK_METHOD( UploadCache, 2 );
        MOCK_METHOD( DeleteCache, 1 );
        MOCK_METHOD( InstallFromCache, 2 );
        MOCK_METHOD( GetExercises, 3 );
        MOCK_METHOD( CountExercises, 1 );
        MOCK_METHOD( Notify, 1 );
        MOCK_METHOD( GetPlugins, 2 );
        MOCK_METHOD( CountPlugins, 0 );
    };
};

#endif // MOCK_NODE_CONTROLLER_H

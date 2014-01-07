// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef MOCK_PORT_FACTORY_H
#define MOCK_PORT_FACTORY_H

#include "host/PortFactory_ABC.h"

namespace mocks
{
    MOCK_BASE_CLASS( MockPort, host::Port_ABC )
    {
        MOCK_METHOD( Get, 0 );
        MockPort( int port )
        {
            MOCK_EXPECT( Get ).returns( port );
        }
    };

    MOCK_BASE_CLASS( MockPortFactory, host::PortFactory_ABC )
    {
        MOCK_METHOD_EXT( Create, 0, host::Port(), Create0 );
        MOCK_METHOD_EXT( Create, 1, host::Port( int ), Create1 );
        MOCK_METHOD( WaitConnected, 2 );
    };
};

#endif // MOCK_PORT_FACTORY_H

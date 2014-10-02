// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef MOCK_PROXY_H
#define MOCK_PROXY_H

#include "host/Proxy_ABC.h"

namespace mocks
{
    MOCK_BASE_CLASS( MockProxy, host::Proxy_ABC )
    {
        MOCK_METHOD( GetPort, 0 );
        MOCK_METHOD( Register, 3 );
        MOCK_METHOD( Unregister, 1 );
    };
};

#endif // MOCK_PROXY_H

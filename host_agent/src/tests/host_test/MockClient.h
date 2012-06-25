// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef MOCK_CLIENT_H
#define MOCK_CLIENT_H

#include "web/Client_ABC.h"

namespace mocks
{
    MOCK_BASE_CLASS( MockClient, web::Client_ABC )
    {
        MOCK_METHOD( Get, 4 );
    };

    MOCK_BASE_CLASS( MockResponse, web::Response_ABC )
    {
        MOCK_METHOD( GetStatus, 0 );
        MOCK_METHOD( GetBody, 0 );
    };
};

#endif // MOCK_CLIENT_H

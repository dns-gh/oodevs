// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef MOCK_USER_CONTROLLER_H
#define MOCK_USER_CONTROLLER_H

#include "web/UserController_ABC.h"

namespace mocks
{
    MOCK_BASE_CLASS( MockUserController, web::UserController_ABC )
    {
        MOCK_METHOD( Login, 3 );
        MOCK_METHOD( IsAuthenticated, 2 );
        MOCK_METHOD( Logout, 1 );
        MOCK_METHOD( UpdateLogin, 4 );
    };
};

#endif // MOCK_USER_CONTROLLER_H

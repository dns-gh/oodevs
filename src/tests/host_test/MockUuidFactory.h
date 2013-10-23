// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef MOCK_UUID_FACTORY_H
#define MOCK_UUID_FACTORY_H

#include "host/UuidFactory_ABC.h"

namespace mocks
{
    MOCK_BASE_CLASS( MockUuidFactory, host::UuidFactory_ABC )
    {
        MOCK_METHOD( Create, 0 );
    };
};

#endif // MOCK_UUID_FACTORY_H

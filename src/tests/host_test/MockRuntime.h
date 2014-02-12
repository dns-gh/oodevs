// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef MOCK_RUNTIME_H
#define MOCK_RUNTIME_H

#include "runtime/Runtime_ABC.h"

namespace mocks
{
    MOCK_BASE_CLASS( MockRuntime, runtime::Runtime_ABC )
    {
        MOCK_METHOD( GetProcess, 1 );
        MOCK_METHOD( Start, 4 );
        MOCK_METHOD( GetModuleFilename, 0 );
        MOCK_METHOD( GetLastError, 0 );
    };
};

#endif // MOCK_RUNTIME_H

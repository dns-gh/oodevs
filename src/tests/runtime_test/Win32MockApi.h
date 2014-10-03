// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#ifndef WIN32_MOCK_API_H__
#define WIN32_MOCK_API_H__

#include "runtime_test_pch.h"
#include <runtime/win32/Api_ABC.h>

namespace test
{
    MOCK_BASE_CLASS( MockApi, runtime::Api_ABC )
    {
        MOCK_METHOD( GetLastError, 0 );
        MOCK_METHOD( MakeProcess, 4 );
        MOCK_METHOD( CloseHandle, 1 );
        MOCK_METHOD( OpenProcess, 3 );
        MOCK_METHOD( GetProcessName, 3 );
        MOCK_METHOD( WaitForSingleObjectEx, 3 );
        MOCK_METHOD( TerminateProcess, 2 );
        MOCK_METHOD( GetModuleFilename, 0 );
        MOCK_METHOD( GetCurrentProcessIdentifier, 0 );
    };
}

#endif

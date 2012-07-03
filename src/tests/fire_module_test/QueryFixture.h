// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef QUERY_FIXTURE_H
#define QUERY_FIXTURE_H

#include "ModuleFixture.h"

namespace
{
    MOCK_FUNCTION( filter, 1, bool( const SWORD_Model* component ), filter )

    struct QueryFixture : sword::fire::ModuleFixture
    {
        ~QueryFixture()
        {
            MOCK_VERIFY( filter );
            MOCK_RESET( filter );
        }
    };
}

#endif // QUERY_FIXTURE_H

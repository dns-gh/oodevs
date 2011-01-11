// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MockElevationGetter_ABC_h_
#define __MockElevationGetter_ABC_h_

#include "flood/ElevationGetter_ABC.h"

// =============================================================================
/** @class  MockElevationGetter_ABC
    @brief  MockElevationGetter_ABC
*/
// Created: ABR 2011-01-10
// =============================================================================
MOCK_BASE_CLASS( MockElevationGetter, flood::ElevationGetter_ABC )
{
    MOCK_METHOD( GetElevationAt, 1 );
};

#endif // __MockElevationGetter_ABC_h_

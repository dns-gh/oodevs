// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MockMovable_h_
#define __MockMovable_h_

#include "tic/Movable_ABC.h"

// =============================================================================
/** @class  MockMovable
    @brief  Mock movable
*/
// Created: LGY 2010-08-09
// =============================================================================
MOCK_BASE_CLASS( MockMovable, tic::Movable_ABC )
{
    MOCK_METHOD( Move, 1 )
    MOCK_METHOD( Stop, 0 )
};

#endif // __MockMovable_h_

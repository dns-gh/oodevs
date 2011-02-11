// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MockMIL_LivingArea_h
#define __MockMIL_LivingArea_h

#include "Entities/Inhabitants/MIL_LivingArea_ABC.h"

// =============================================================================
/** @class  MockMIL_LivingArea
    @brief  Mock living Area
*/
// Created: LGY 2010-11-15
// =============================================================================
MOCK_BASE_CLASS( MockMIL_LivingArea, MIL_LivingArea_ABC )
{
    MOCK_METHOD( StartMotivation, 1 );
    MOCK_METHOD( MovePeople, 1 );
    MOCK_METHOD( FinishMoving, 0 );
    MOCK_METHOD( ComputeMovingArea, 0 );
};

#endif // __MockMIL_LivingArea_h
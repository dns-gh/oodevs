// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockToxicAttribute_h_
#define __MockToxicAttribute_h_

#include "simulation_kernel/Entities/Objects/ToxicAttribute_ABC.h"

// =============================================================================
/** @class  MockToxicAttribute
    @brief  Mock toxic attribute
*/
// Created: LGY 2010-08-06
// =============================================================================
MOCK_BASE_CLASS( MockToxicAttribute, ToxicAttribute_ABC )
{
    MOCK_METHOD( GetContaminationEffect, 2 );
};

#endif // __MockToxicAttribute_h_

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MockBuilder_h_
#define __MockBuilder_h_

#include "simulation_kernel/Entities/Objects/MIL_ObjectBuilder_ABC.h"

// =============================================================================
/** @class  MockBuilder
    @brief  MockBuilder
*/
// Created: JCR 2008-09-01
// =============================================================================
MOCK_BASE_CLASS( MockBuilder, MIL_ObjectBuilder_ABC )
{
    MOCK_METHOD( GetType, 0 );
    MOCK_METHOD( Build, 1 );
};

#endif // __MockBuilder_h_

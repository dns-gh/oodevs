// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MockStaticModel_h_
#define __MockStaticModel_h_

#include "3a/StaticModel_ABC.h"

// =============================================================================
/** @class  MockStaticModel
    @brief  Mock Static Model
*/
// Created: ABR 2011-02-14
// =============================================================================
MOCK_BASE_CLASS( MockStaticModel, aar::StaticModel_ABC )
{
    MOCK_METHOD( ComputePower, 2 )
};

#endif // __MockStaticModel_h_

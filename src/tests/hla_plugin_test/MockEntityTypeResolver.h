// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef rpr_MockEntityTypeResolver_h
#define rpr_MockEntityTypeResolver_h

#include "rpr/EntityTypeResolver_ABC.h"

namespace rpr
{

// =============================================================================
/** @class  MockEntityTypeResolver
    @brief  Mock Entity type resolver
*/
// Created: SLI 2011-09-29
// =============================================================================
MOCK_BASE_CLASS( MockEntityTypeResolver, EntityTypeResolver_ABC )
{
    MOCK_METHOD( Find, 1 )
    MOCK_METHOD( Resolve, 1 )
};

}

#endif // rpr_MockEntityTypeResolver_h

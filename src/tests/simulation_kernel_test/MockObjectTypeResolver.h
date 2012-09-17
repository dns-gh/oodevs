// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef MockObjectTypeResolver_h
#define MockObjectTypeResolver_h

#include "Entities/Objects/ObjectTypeResolver_ABC.h"

// =============================================================================
/** @class  MockObjectTypeResolver
    @brief  Mock ObjectTypeResolver
*/
// Created: LGY 2012-09-14
// =============================================================================
MOCK_BASE_CLASS( MockObjectTypeResolver, ObjectTypeResolver_ABC )
{
    MOCK_METHOD( FindType, 1 );
};

#endif // MockObjectTypeResolver_h

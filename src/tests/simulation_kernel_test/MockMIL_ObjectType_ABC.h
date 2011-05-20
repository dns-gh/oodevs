// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __StubMIL_ObjectType_ABC_h_
#define __StubMIL_ObjectType_ABC_h_

#include "Entities/Objects/MIL_ObjectType_ABC.h"

MOCK_BASE_CLASS( MockMIL_ObjectType_ABC, MIL_ObjectType_ABC )
{
    MOCK_METHOD( GetID, 0 );
    MOCK_METHOD( GetName, 0 );
    MOCK_METHOD( GetPointSize, 0 );
};

#endif // __StubMIL_ObjectType_ABC_h_

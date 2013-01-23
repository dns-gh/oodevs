// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockPHY_FireResults_ABC_h_
#define __MockPHY_FireResults_ABC_h_

#include "Entities/Actions/PHY_FireResults_ABC.h"

MOCK_BASE_CLASS( MockPHY_FireResults_ABC, PHY_FireResults_ABC )
{
    MOCK_METHOD( Hit, 1 );
};

#endif // __MockPHY_FireResults_ABC_h_

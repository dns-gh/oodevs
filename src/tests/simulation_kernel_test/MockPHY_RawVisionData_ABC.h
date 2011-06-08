// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockPHY_RawVisionData_ABC_h_
#define __MockPHY_RawVisionData_ABC_h_

#include "meteo/PHY_RawVisionData_ABC.h"

MOCK_BASE_CLASS( MockPHY_RawVisionData_ABC, PHY_RawVisionData_ABC )
{
    MOCK_METHOD( RegisterMeteoPatch, 3 );
    MOCK_METHOD( UnregisterMeteoPatch, 2 );
};

#endif // __MockPHY_RawVisionData_ABC_h_

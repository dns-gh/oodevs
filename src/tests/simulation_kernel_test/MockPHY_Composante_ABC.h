// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockPHY_Composante_ABC_h_
#define __MockPHY_Composante_ABC_h_

#include"Entities/Agents/Units/Composantes/PHY_Composante_ABC.h"

MOCK_BASE_CLASS( MockPHY_Composante_ABC, PHY_Composante_ABC )
{
    MOCK_METHOD( GetType, 0 );
    MOCK_METHOD( GetIdentificationMaxRange, 0 );
    MOCK_METHOD( GetReconnoissanceMaxRange, 0 );
    MOCK_METHOD( CanFireWhenUnloaded, 0 );
    MOCK_METHOD( GetMajorScore, 0 );
};

#endif // __MockPHY_Composante_ABC_h_

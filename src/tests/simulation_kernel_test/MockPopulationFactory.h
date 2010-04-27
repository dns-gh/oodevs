// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MockPopulationFactory_h_
#define __MockPopulationFactory_h_

#include "simulation_kernel/PopulationFactory_ABC.h"

MOCK_BASE_CLASS( MockPopulationFactory, PopulationFactory_ABC )
{
    MOCK_METHOD( Create, 2 );
};

#endif // __MockArmy_h_

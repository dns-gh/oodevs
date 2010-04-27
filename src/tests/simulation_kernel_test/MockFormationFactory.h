// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MockFormationFactory_h_
#define __MockFormationFactory_h_

#include "simulation_kernel/FormationFactory_ABC.h"

MOCK_BASE_CLASS( MockFormationFactory, FormationFactory_ABC )
{
    MOCK_METHOD( Create, 3 );
};

#endif // __MockArmy_h_

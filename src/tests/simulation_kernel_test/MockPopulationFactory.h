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
    MOCK_METHOD_EXT( Create, 2, MIL_Population&( xml::xistream&, MIL_Army_ABC& ), Create1 )
    MOCK_METHOD_EXT( Create, 7, MIL_Population&( const std::string&, const MT_Vector2D&, int, const std::string&, MIL_Army_ABC&, UrbanObjectWrapper*, unsigned int ), Create2 )
};

#endif // __MockArmy_h_

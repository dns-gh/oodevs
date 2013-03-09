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
    MOCK_METHOD( Create, 3, MIL_Formation&( xml::xistream&, MIL_Army_ABC&, MIL_Formation* ), Create3 );
    MOCK_METHOD( Create, 5, MIL_Formation&( int, const std::string&, const std::string&, MIL_Army_ABC&, MIL_Formation* ), Create5 );
};

#endif // __MockArmy_h_

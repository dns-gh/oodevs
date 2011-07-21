// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MockInhabitantFactory_h_
#define __MockInhabitantFactory_h_

#include "simulation_kernel/InhabitantFactory_ABC.h"

MOCK_BASE_CLASS( MockInhabitantFactory, InhabitantFactory_ABC )
{
    MOCK_METHOD_EXT( Create, 2, MIL_Inhabitant&( xml::xistream&, MIL_Army_ABC& ), Create1 )
    MOCK_METHOD_EXT( Create, 5, MIL_Inhabitant&( const std::string&, const MT_Vector2D&, int, const std::string&, MIL_Army_ABC& ), Create2 )
    MOCK_METHOD( Finalize, 0 )
};

#endif // __MockArmy_h_

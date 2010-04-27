// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MockAutomateFactory_h_
#define __MockAutomateFactory_h_

#include "simulation_kernel/AutomateFactory_ABC.h"

MOCK_BASE_CLASS( MockAutomateFactory, AutomateFactory_ABC )
{
    MOCK_METHOD_EXT( Create, 2, MIL_Automate& ( xml::xistream&, MIL_Automate& ), CreateInAutomat );
    MOCK_METHOD_EXT( Create, 2, MIL_Automate& ( xml::xistream&, MIL_Formation& ), CreateInFormation );
};

#endif // __MockArmy_h_

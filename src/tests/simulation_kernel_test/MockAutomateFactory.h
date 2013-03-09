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
    MOCK_METHOD( Create, 2, MIL_Automate& ( xml::xistream&, MIL_Entity_ABC& ), Create );
    MOCK_METHOD( Create, 5, MIL_Automate& ( const MIL_AutomateType &,unsigned int,const std::string &,MIL_Entity_ABC &, unsigned int ), DynamicCreate );
};

#endif // __MockArmy_h_

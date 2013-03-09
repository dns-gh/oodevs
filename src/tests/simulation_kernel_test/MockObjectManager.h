// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MockObjectManager_h_
#define __MockObjectManager_h_

#include "simulation_kernel/Entities/Objects/MIL_ObjectManager.h"

MOCK_BASE_CLASS( MockObjectManager, MIL_ObjectManager )
{
    MOCK_METHOD( CreateObject, 2, MIL_Object_ABC&( xml::xistream&, MIL_Army_ABC& ), CreateObjectFromXml );
};

#endif // __MockArmy_h_

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
// LTO
// *****************************************************************************

#ifndef __MockKnowledgeGroupFactory_h_
#define __MockKnowledgeGroupFactory_h_

#include "simulation_kernel/Knowledge/KnowledgeGroupFactory_ABC.h"

MOCK_BASE_CLASS( MockKnowledgeGroupFactory, KnowledgeGroupFactory_ABC )
{
    MOCK_METHOD( Create, 3 );
};

#endif // __MockKnowledgeGroupFactory_h_

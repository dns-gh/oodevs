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
    MOCK_METHOD_EXT( Create, 3, boost::shared_ptr< MIL_KnowledgeGroup >( xml::xistream&, MIL_Army_ABC&, boost::shared_ptr< MIL_KnowledgeGroup > ), CreateFromXis );
    MOCK_METHOD_EXT( Create, 1, boost::shared_ptr< MIL_KnowledgeGroup >( MIL_Army_ABC& ), CreateForCrowd );
};

#endif // __MockKnowledgeGroupFactory_h_

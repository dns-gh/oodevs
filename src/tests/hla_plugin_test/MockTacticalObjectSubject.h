// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef hla_pluginMockTacticalObjectSubject_h
#define hla_pluginMockTacticalObjectSubject_h

#include "hla_plugin/TacticalObjectSubject_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockTacticalObjectSubject
    @brief  MockTacticalObjectSubject
*/
// Created: AHC 2010-08-10
// =============================================================================
MOCK_BASE_CLASS( MockTacticalObjectSubject, TacticalObjectSubject_ABC )
{
    MOCK_METHOD( Register, 1 );
    MOCK_METHOD( Unregister, 1 );
};

}

}

#endif // hla_pluginMockTacticalObjectSubject_h

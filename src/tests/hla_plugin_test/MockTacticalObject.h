// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef hla_plugin_MocTacticalObject_h
#define hla_plugin_MocTacticalObject_h

#include "hla_plugin/TacticalObject_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MocTacticalObject
    @brief  Mock agent
*/
// Created: AHC 2013-01-17
// =============================================================================
MOCK_BASE_CLASS( MockTacticalObject, TacticalObject_ABC )
{
    MOCK_METHOD( Register, 1 )
    MOCK_METHOD( Unregister, 1)
};

}
}

#endif // hla_plugin_MocTacticalObject_h

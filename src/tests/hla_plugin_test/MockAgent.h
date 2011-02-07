// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef hla_plugin_MockAgent_h
#define hla_plugin_MockAgent_h

#include "hla_plugin/Agent_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockAgent
    @brief  Mock agent
*/
// Created: SLI 2011-01-11
// =============================================================================
MOCK_BASE_CLASS( MockAgent, Agent_ABC )
{
    MOCK_METHOD( GetName, 0 )
    MOCK_METHOD( GetPosition, 0 )
    MOCK_METHOD( GetAltitude, 0 )
    MOCK_METHOD( GetSpeed, 0 )
    MOCK_METHOD( GetDirection, 0 )
    MOCK_METHOD( GetForce, 0 )
    MOCK_METHOD( GetEquipments, 0 )
};

}
}

#endif // hla_plugin_MockAgent_h

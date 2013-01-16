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
    MOCK_METHOD( Register, 1 )
    MOCK_METHOD( Unregister, 1 )
    MOCK_METHOD( HasSubordinate, 1 )
    MOCK_METHOD( AddSubordinate, 2 )
    MOCK_METHOD( RemoveSubordinate, 1 )
};

}
}

#endif // hla_plugin_MockAgent_h

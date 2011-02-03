// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef hla_plugin_MockAgentSubject_h
#define hla_plugin_MockAgentSubject_h

#include "hla_plugin/AgentSubject_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockAgentSubject
    @brief  Mock agent subject
*/
// Created: SLI 2011-01-11
// =============================================================================
MOCK_BASE_CLASS( MockAgentSubject, AgentSubject_ABC )
{
    MOCK_METHOD( Register, 1 );
    MOCK_METHOD( Unregister, 1 );
};

}

}

#endif // hla_plugin_MockAgentSubject_h

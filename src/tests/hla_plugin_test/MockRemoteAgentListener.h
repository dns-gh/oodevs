// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef hla_plugin_MockRemoteAgentListener_h
#define hla_plugin_MockRemoteAgentListener_h

#include "hla_plugin/RemoteAgentListener_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockRemoteAgentListener
    @brief  Mock remote agent listener
*/
// Created: SLI 2011-08-29
// =============================================================================
MOCK_BASE_CLASS( MockRemoteAgentListener, RemoteAgentListener_ABC )
{
    MOCK_METHOD( Created, 1 )
    MOCK_METHOD( Destroyed, 1 )
    MOCK_METHOD( Moved, 3 )
    MOCK_METHOD( SideChanged, 2 )
    MOCK_METHOD( NameChanged, 2 )
    MOCK_METHOD( TypeChanged, 2 )
    MOCK_METHOD( EquipmentUpdated, 3 )
    MOCK_METHOD( UniqueIdChanged, 2 )
    MOCK_METHOD( CallsignChanged, 2 )
};

}

}

#endif // hla_plugin_MockRemoteAgentListener_h

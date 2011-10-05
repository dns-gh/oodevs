// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef hla_plugin_MockRemoteHlaObjectFactory_h
#define hla_plugin_MockRemoteHlaObjectFactory_h

#include "hla_plugin/RemoteHlaObjectFactory_ABC.h"
#include "hla_plugin/HlaObject_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockRemoteHlaObjectFactory
    @brief  Mock remote hla object factory
*/
// Created: SLI 2011-07-26
// =============================================================================
MOCK_BASE_CLASS( MockRemoteHlaObjectFactory, RemoteHlaObjectFactory_ABC )
{
    MOCK_METHOD( CreateAggregate, 2 )
};

}
}

#endif // hla_plugin_MockRemoteHlaObjectFactory_h

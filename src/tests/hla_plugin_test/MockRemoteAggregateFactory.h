// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef hla_plugin_MockRemoteAggregateFactory_h
#define hla_plugin_MockRemoteAggregateFactory_h

#include "hla_plugin/RemoteAggregateFactory_ABC.h"
#include "hla_plugin/Aggregate_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockRemoteAggregateFactory
    @brief  Mock remote aggregate factory
*/
// Created: SLI 2011-07-26
// =============================================================================
MOCK_BASE_CLASS( MockRemoteAggregateFactory, RemoteAggregateFactory_ABC )
{
    MOCK_METHOD( Create, 2 )
};

}
}

#endif // hla_plugin_MockRemoteAggregateFactory_h

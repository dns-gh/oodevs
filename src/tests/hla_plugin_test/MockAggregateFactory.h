// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef hla_plugin_MockAggregateFactory_h
#define hla_plugin_MockAggregateFactory_h

#include "hla_plugin/AggregateFactory_ABC.h"
#include "hla_plugin/Aggregate_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockAggregateFactory
    @brief  Mock aggregate factory
*/
// Created: SLI 2011-07-26
// =============================================================================
MOCK_BASE_CLASS( MockAggregateFactory, AggregateFactory_ABC )
{
    MOCK_METHOD( Create, 5 )
};

}
}

#endif // hla_plugin_MockAggregateFactory_h

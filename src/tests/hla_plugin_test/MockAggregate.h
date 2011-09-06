// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef hla_plugin_MockAggregate_h
#define hla_plugin_MockAggregate_h

#include "hla_plugin/Aggregate_ABC.h"
#include <hla/Deserializer.h>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockAggregate
    @brief  Mock class builder
*/
// Created: SLI 2011-07-26
// =============================================================================
MOCK_BASE_CLASS( MockAggregate, Aggregate_ABC )
{
    MOCK_METHOD( Serialize, 2 )
    MOCK_METHOD( Deserialize, 2 )
};

}
}

#endif // hla_plugin_MockAggregate_h

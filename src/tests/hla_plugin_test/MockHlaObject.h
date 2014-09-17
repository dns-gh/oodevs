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

#include "hla_plugin/HlaObject_ABC.h"
#include <hla/Deserializer.h>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockHlaObject
    @brief  Mock hla object
*/
// Created: SLI 2011-07-26
// =============================================================================
MOCK_BASE_CLASS( MockHlaObject, HlaObject_ABC )
{
    MOCK_METHOD( Serialize, 2 )
    MOCK_METHOD( Deserialize, 2 )
    MOCK_METHOD( Register, 1 )
    MOCK_METHOD( Unregister, 1 )
    MOCK_METHOD( Attach, 2 )
    MOCK_METHOD( GetAgent, 0 )
    MOCK_METHOD( GetIdentifier, 0 )
    MOCK_METHOD( ResetAttributes, 0 )
};

}
}

#endif // hla_plugin_MockAggregate_h

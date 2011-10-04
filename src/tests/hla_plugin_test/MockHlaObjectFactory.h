// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef hla_plugin_MockHlaObjectFactory_h
#define hla_plugin_MockHlaObjectFactory_h

#include "hla_plugin/HlaObjectFactory_ABC.h"
#include "hla_plugin/HlaObject_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockHlaObjectFactory
    @brief  Mock hla object factory
*/
// Created: SLI 2011-07-26
// =============================================================================
MOCK_BASE_CLASS( MockHlaObjectFactory, HlaObjectFactory_ABC )
{
    MOCK_METHOD( Create, 5 )
};

}
}

#endif // hla_plugin_MockHlaObjectFactory_h

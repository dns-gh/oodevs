// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef hla_plugin_MockHlaObjectNameFactory_h
#define hla_plugin_MockHlaObjectNameFactory_h

#include "hla_plugin/HlaObjectNameFactory_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockHlaObjectNameFactory
    @brief  MockHlaObjectNameFactory
*/
// Created: AHC 2012-03-23
// =============================================================================
MOCK_BASE_CLASS( MockHlaObjectNameFactory, HlaObjectNameFactory_ABC )
{
    MOCK_METHOD( CreateName, 1 )
};

}
}

#endif // hla_plugin_MockHlaObjectFactory_h

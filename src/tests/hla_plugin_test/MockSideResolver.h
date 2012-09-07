// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef hla_plugin_MockSideResolver_h
#define hla_plugin_MockSideResolver_h

#include "hla_plugin/SideResolver_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockSideResolver
    @brief  MockSideResolver
*/
// Created: AHC 2012-09-07
// =============================================================================
MOCK_BASE_CLASS( MockSideResolver, SideResolver_ABC )
{
    MOCK_METHOD( ResolveTeam, 1 )
    MOCK_METHOD( ResolveForce, 1 )
};

}
}

#endif // hla_plugin_MockSideResolver_h

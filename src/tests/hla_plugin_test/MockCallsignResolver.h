// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MockCallsignResolver_h
#define plugins_hla_MockCallsignResolver_h

#include "hla_plugin/CallsignResolver_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockCallsignResolver
    @brief  Mock callsign resolver
*/
// Created: SLI 2011-10-10
// =============================================================================
MOCK_BASE_CLASS( MockCallsignResolver, CallsignResolver_ABC )
{
    MOCK_METHOD( Generate, 2 )
    MOCK_METHOD( Add, 3 )
    MOCK_METHOD( ResolveCallsign, 1 )
    MOCK_METHOD( ResolveUniqueId, 1 )
    MOCK_METHOD( ResolveSimulationIdentifier, 1 )
};

}
}

#endif // plugins_hla_MockCallsignResolver_h

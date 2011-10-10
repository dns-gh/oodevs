// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MockTransportedUnits_h
#define plugins_hla_MockTransportedUnits_h

#include "hla_plugin/TransportedUnits_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockTransportedUnits
    @brief  Mock Transported units
*/
// Created: SLI 2011-10-07
// =============================================================================
MOCK_BASE_CLASS( MockTransportedUnits, TransportedUnits_ABC )
{
    MOCK_METHOD( Apply, 1 )
};

}
}

#endif // plugins_hla_MockTransportedUnits_h

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MockTransporters_h
#define plugins_hla_MockTransporters_h

#include "hla_plugin/Transporters_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockTransporters
    @brief  Mock Transporters
*/
// Created: SLI 2011-10-11
// =============================================================================
MOCK_BASE_CLASS( MockTransporters, Transporters_ABC )
{
    MOCK_METHOD( Apply, 3 )
};

}
}

#endif // plugins_hla_MockTransporters_h

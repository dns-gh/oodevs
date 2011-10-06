// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MockMissionResolver_h
#define plugins_hla_MockMissionResolver_h

#include "hla_plugin/MissionResolver_ABC.h"

namespace plugins
{
namespace hla
{

// =============================================================================
/** @class  MockMissionResolver
    @brief  Mock Mission resolver
*/
// Created: SLI 2011-10-06
// =============================================================================
MOCK_BASE_CLASS( MockMissionResolver, MissionResolver_ABC )
{
    MOCK_METHOD( Resolve, 1 )
};

}
}

#endif // plugins_hla_MockMissionResolver_h

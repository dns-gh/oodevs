// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef hla_plugin_MockSimulationTimeManager_h
#define hla_plugin_MockSimulationTimeManager_h

#include "hla_plugin/SimulationTimeManager_ABC.h"

namespace plugins
{
namespace hla
{
MOCK_BASE_CLASS( MockSimulationTimeManager, SimulationTimeManager_ABC )
{
    MOCK_METHOD( getSimulationTime, 0 )
    MOCK_METHOD( getSimulationtick, 0 )
};

}
}

#endif // hla_plugin_MockSimulationTimeManager_h

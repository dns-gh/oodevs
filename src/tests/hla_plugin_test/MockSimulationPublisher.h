// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef MockSimulationPublisher_h
#define MockSimulationPublisher_h

#include "dispatcher/SimulationPublisher_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  MockSimulationPublisher
    @brief  Mock simulation publisher
*/
// Created: VPR 2011-09-06
// =============================================================================
MOCK_BASE_CLASS( MockSimulationPublisher, SimulationPublisher_ABC )
{
    MOCK_METHOD_EXT( Send, 1, void( const sword::ClientToSim& ), SendClientToSim )
    MOCK_METHOD_EXT( Send, 1, void( const sword::DispatcherToSim& ), SendDispatcherToSim )
};

}

#endif // MockSimulationPublisher_h

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CommandLineTools_h_
#define __CommandLineTools_h_

#include <cstdint>

namespace frontend
{
    enum Ports
    {
        SIMULATION_PORT,
        DISPATCHER_PORT,
        TIMELINE_PORT,
        NUM_PORTS,
    };

    uint16_t GetPort( unsigned idx, Ports port );
}

#endif // __CommandLineTools_h_

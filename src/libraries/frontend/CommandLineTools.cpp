// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "CommandLineTools.h"

namespace frontend
{
    // -----------------------------------------------------------------------------
    // Name: SimulationPort
    // Created: SBO 2008-02-20
    // -----------------------------------------------------------------------------
    unsigned short SimulationPort( unsigned int exerciseNumber )
    {
        return static_cast< unsigned short >( 10000 + ( exerciseNumber - 1 ) * 2 );
    }

    // -----------------------------------------------------------------------------
    // Name: DispatcherPort
    // Created: SBO 2008-02-20
    // -----------------------------------------------------------------------------
    unsigned short DispatcherPort( unsigned int exerciseNumber )
    {
        return SimulationPort( exerciseNumber ) + 1;
    }
}

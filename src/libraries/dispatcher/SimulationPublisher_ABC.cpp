// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "SimulationPublisher_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: SimulationPublisher_ABC constructor
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
SimulationPublisher_ABC::SimulationPublisher_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimulationPublisher_ABC destructor
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
SimulationPublisher_ABC::~SimulationPublisher_ABC()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: SimulationPublisher_ABC::Send
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void SimulationPublisher_ABC::Send( const ASN1T_MsgsClientToSim& )
{
    throw std::runtime_error( "Invalid dispatcher : " __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: SimulationPublisher_ABC::Send
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void SimulationPublisher_ABC::Send( const ASN1T_MsgsMiddleToSim& )
{
    throw std::runtime_error( "Invalid dispatcher : " __FUNCTION__ " not implemented" );
}

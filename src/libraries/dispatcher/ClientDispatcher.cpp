// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ClientDispatcher.h"
#include "SimulationNetworker.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ClientDispatcher constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
ClientDispatcher::ClientDispatcher( Publisher_ABC& simulationPublisher )
    : simulationPublisher_( simulationPublisher )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClientDispatcher destructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
ClientDispatcher::~ClientDispatcher()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClientDispatcher::OnReceive
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void ClientDispatcher::OnReceive( const ASN1T_MsgsClientToSim& asnMsg )
{
    simulationPublisher_.Send( asnMsg );
}

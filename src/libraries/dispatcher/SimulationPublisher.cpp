// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "SimulationPublisher.h"

#include "Simulation.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: SimulationPublisher constructor
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
SimulationPublisher::SimulationPublisher( Simulation& simulation )
    : Publisher_ABC()
    , simulation_  ( simulation )
{

}

// -----------------------------------------------------------------------------
// Name: SimulationPublisher destructor
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
SimulationPublisher::~SimulationPublisher()
{

}

// -----------------------------------------------------------------------------
// Name: SimulationPublisher::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void SimulationPublisher::Send( const ASN1T_MsgsInSim& msg )
{
    simulation_.Send( msg );
}

// -----------------------------------------------------------------------------
// Name: SimulationPublisher::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void SimulationPublisher::Send( const ASN1T_MsgsInClient& /*msg*/ )
{
    throw std::runtime_error( "Invalid publisher" );    
}

// -----------------------------------------------------------------------------
// Name: SimulationPublisher::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void SimulationPublisher::Send( unsigned int nMsgID, const DIN::DIN_BufferedMessage& dinMsg )
{
    simulation_.Send( nMsgID, dinMsg );
}

// -----------------------------------------------------------------------------
// Name: SimulationPublisher::GetDinMsg
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
DIN::DIN_BufferedMessage SimulationPublisher::GetDinMsg()
{
    return simulation_.GetDinMsg();
}

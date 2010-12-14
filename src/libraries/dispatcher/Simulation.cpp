// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Simulation.h"
#include "tools/MessageSender_ABC.h"
#include "MessageHandler_ABC.h"
#include "protocol/SimulationSenders.h"
#include "protocol/DispatcherSenders.h"

using namespace dispatcher;
using namespace tools;

// -----------------------------------------------------------------------------
// Name: Simulation constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
Simulation::Simulation( MessageHandler_ABC& handler, MessageSender_ABC& messageService, const std::string& endpoint )
    : handler_       ( handler )
    , messageService_( messageService )
    , endpoint_      ( endpoint )
{
    dispatcher::CtrlClientAnnouncement msg;
    msg().set_announce( true );
    msg.Send( *this );
}

//-----------------------------------------------------------------------------
// Name: Simulation destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
Simulation::~Simulation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Simulation::OnReceive
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
void Simulation::OnReceive( const sword::SimToClient& asnMsg )
{
    handler_.Receive( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Send
// Created: NLD 2007-04-24
// -----------------------------------------------------------------------------
void Simulation::Send( const MsgsDispatcherToSim::MsgDispatcherToSim& asnMsg )
{
    messageService_.Send( endpoint_, asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Simulation::Send( const sword::ClientToSim& asnMsg )
{
    messageService_.Send( endpoint_, asnMsg );
}

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
void Simulation::OnReceive( const sword::SimToClient& msg )
{
    try
    {
        handler_.Receive( msg );
    }
    catch( std::exception& e )
    {
        if( msg.message().has_control_end_tick() )
        {
            dispatcher::CtrlDispatcherTickAck msg;
            msg.Send( *this );
        }
        throw;
    }
    if( msg.message().has_control_end_tick() )
    {
        dispatcher::CtrlDispatcherTickAck msg;
        msg.Send( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: Simulation::Send
// Created: NLD 2007-04-24
// -----------------------------------------------------------------------------
void Simulation::Send( const sword::DispatcherToSim& msg )
{
    messageService_.Send( endpoint_, msg );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Simulation::Send( const sword::ClientToSim& msg )
{
    messageService_.Send( endpoint_, msg );
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "MagicOrders.h"

#include "clients_gui/Decisions_ABC.h"

#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Controller.h"

#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MagicOrders constructor
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
MagicOrders::MagicOrders( const Agent_ABC& agent )
    : agent_( agent )
    , transportersReady_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MagicOrders destructor
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
MagicOrders::~MagicOrders()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MagicOrders::DoUpdate
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void MagicOrders::DoUpdate( const sword::UnitAttributes& message )
{
    if( message.has_transporters_available()  )
        transportersReady_ = message.transporters_available() != 0;
}

// -----------------------------------------------------------------------------
// Name: MagicOrders::CanRetrieveTransporters
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
bool MagicOrders::CanRetrieveTransporters() const
{
    return ! transportersReady_;
}

// -----------------------------------------------------------------------------
// Name: MagicOrders::CanMagicMove
// Created: AGE 2006-10-13
// -----------------------------------------------------------------------------
bool MagicOrders::CanMagicMove() const
{
    return agent_.Get< gui::Decisions_ABC >().CanBeOrdered();
}

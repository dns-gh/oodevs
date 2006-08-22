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
#include "clients_kernel/Agent_ABC.h"
#include "AutomatDecisions.h"
#include "clients_kernel/Controller.h"

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
void MagicOrders::DoUpdate( const ASN1T_MsgUnitAttributes& message )
{
    if( message.m.transporteurs_disponiblesPresent )
        transportersReady_ = message.transporteurs_disponibles;
}

// -----------------------------------------------------------------------------
// Name: MagicOrders::CanSurrender
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
bool MagicOrders::CanSurrender() const
{
    return agent_.GetAutomatType();
}

// -----------------------------------------------------------------------------
// Name: MagicOrders::CanMagicMove
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
bool MagicOrders::CanMagicMove() const
{
    if( agent_.GetAutomatType() )
        return true;
    if( agent_.GetSuperior() && agent_.GetSuperior()->GetAutomatType() )
        return !agent_.GetSuperior()->Retrieve< AutomatDecisions >()->IsEmbraye();
    return false;
}

// -----------------------------------------------------------------------------
// Name: MagicOrders::CanRetrieveTransporters
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
bool MagicOrders::CanRetrieveTransporters() const
{
    return ! transportersReady_;
}

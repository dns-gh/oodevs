// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "MagicOrders.h"
#include "Agent.h"
#include "AutomatDecisions.h"
#include "Controller.h"

// -----------------------------------------------------------------------------
// Name: MagicOrders constructor
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
MagicOrders::MagicOrders( Controller& controller, const Agent& agent )
    : controller_( controller )
    , agent_( agent )
    , transportersReady_( false )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: MagicOrders destructor
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
MagicOrders::~MagicOrders()
{
    controller_.Remove( *this );
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
        return agent_.Retrieve< AutomatDecisions >()->IsEmbraye();
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

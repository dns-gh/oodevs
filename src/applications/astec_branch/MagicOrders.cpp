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

// -----------------------------------------------------------------------------
// Name: MagicOrders constructor
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
MagicOrders::MagicOrders( const Agent& agent )
    : automat_( agent.GetAutomatType() )
    , embraye_( false )
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
    if( message.m.etat_automatePresent )
        transportersReady_ = ( message.etat_automate == EnumAutomateState::embraye );
}

// -----------------------------------------------------------------------------
// Name: MagicOrders::CanSurrender
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
bool MagicOrders::CanSurrender() const
{
    return automat_;
}

// -----------------------------------------------------------------------------
// Name: MagicOrders::CanMagicMove
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
bool MagicOrders::CanMagicMove() const
{
    return (   automat_ &&   embraye_ ) 
        || ( ! automat_ && ! embraye_ );
}

// -----------------------------------------------------------------------------
// Name: MagicOrders::CanRetrieveTransporters
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
bool MagicOrders::CanRetrieveTransporters() const
{
    return ! transportersReady_;
}

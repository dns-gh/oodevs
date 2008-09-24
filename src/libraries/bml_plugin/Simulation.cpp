// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "Simulation.h"

using namespace plugins::bml;

// -----------------------------------------------------------------------------
// Name: Simulation constructor
// Created: SBO 2008-06-09
// -----------------------------------------------------------------------------
Simulation::Simulation()
    : currentTick_( 1 )
    , tickDuration_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Simulation destructor
// Created: SBO 2008-06-09
// -----------------------------------------------------------------------------
Simulation::~Simulation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Simulation::Update
// Created: SBO 2008-06-09
// -----------------------------------------------------------------------------
void Simulation::Update( const ASN1T_MsgControlEndTick& message )
{
    currentTick_  = message.current_tick;
    tickDuration_ = message.tick_duration;
}

// -----------------------------------------------------------------------------
// Name: Simulation::MustReportPosition
// Created: SBO 2008-06-09
// -----------------------------------------------------------------------------
bool Simulation::MustReportPosition( unsigned long& currentTick ) const
{
    if( currentTick == 0 || tickDuration_ > 0 && currentTick_ > currentTick + 5 * 60 / tickDuration_ ) // $$$$ SBO 2008-06-09: every 5 minutes
    {
        currentTick = currentTick_;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: Simulation::MustReportStatus
// Created: SBO 2008-07-15
// -----------------------------------------------------------------------------
bool Simulation::MustReportStatus( unsigned long& currentTick ) const
{
    currentTick = currentTick_;
    return true;
}

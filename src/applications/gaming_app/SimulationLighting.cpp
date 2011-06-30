// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "SimulationLighting.h"
#include "gaming/Simulation.h"
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: SimulationLighting constructor
// Created: AGE 2007-10-15
// -----------------------------------------------------------------------------
SimulationLighting::SimulationLighting( kernel::Controllers& controller, QObject* parent )
    : gui::LightingProxy( parent )
    , controller_( controller )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: SimulationLighting destructor
// Created: AGE 2007-10-15
// -----------------------------------------------------------------------------
SimulationLighting::~SimulationLighting()
{
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: SimulationLighting::NotifyUpdated
// Created: AGE 2007-10-15
// -----------------------------------------------------------------------------
void SimulationLighting::NotifyUpdated( const Simulation& simu )
{
    SetTime( simu.GetDateTime() );
}

// -----------------------------------------------------------------------------
// Name: SimulationLighting::NotifyUpdated
// Created: AGE 2007-10-15
// -----------------------------------------------------------------------------
void SimulationLighting::NotifyUpdated( const kernel::ModelLoaded& model )
{
    WorldParameters::Load( model.config_ );
    SetLatitude( latitude_ );
}


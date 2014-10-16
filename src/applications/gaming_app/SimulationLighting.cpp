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
#include "tools/ExerciseConfig.h"
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: SimulationLighting constructor
// Created: AGE 2007-10-15
// -----------------------------------------------------------------------------
SimulationLighting::SimulationLighting( kernel::Controllers& controller )
    : gui::LightingProxy()
    , controller_( controller )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: SimulationLighting constructor
// Created: ABR 2014-07-04
// -----------------------------------------------------------------------------
SimulationLighting::SimulationLighting( const SimulationLighting& other )
    : gui::LightingProxy( other )
    , controller_( other.controller_ )
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
    SetLatitude( model.config_.GetTerrainLatitude() );
}

// -----------------------------------------------------------------------------
// Name: std::shared_ptr< Lighting_ABC > SimulationLighting::Clone
// Created: ABR 2014-10-13
// -----------------------------------------------------------------------------
std::shared_ptr< Lighting_ABC > SimulationLighting::Clone() const
{
    return std::make_shared< SimulationLighting >( *this );
}

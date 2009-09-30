// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "simulation_kernel/AlgorithmsFactories.h"
#include "simulation_kernel/WeaponAvailabilityComputerFactory.h"
#include "simulation_kernel/ComposantesAbleToBeFiredComputerFactory.h"
#include "simulation_kernel/LocationComputerFactory.h"
#include "simulation_kernel/PostureComputerFactory.h"
#include "simulation_kernel/ConsumptionComputerFactory.h"
#include "simulation_kernel/MoveComputerFactory.h"
#include "simulation_kernel/DetectionComputerFactory.h"
#include "simulation_kernel/TransportCapacityComputerFactory.h"
#include "simulation_kernel/HealComputerFactory.h"
#include "simulation_kernel/OnComponentFunctorComputerFactory.h"
#include "simulation_kernel/OnComponentLendedFunctorComputerFactory.h"
#include "simulation_kernel/LoadingComputerFactory.h"

// -----------------------------------------------------------------------------
// Name: AlgorithmsFactories constructor
// Created: MGD 2009-08-17
// -----------------------------------------------------------------------------
AlgorithmsFactories::AlgorithmsFactories()
    : weaponAvailabilityComputerFactory_( new firing::WeaponAvailabilityComputerFactory() )
    , composantesAbleToBeFiredComputerFactory_( new firing::ComposantesAbleToBeFiredComputerFactory() )
    , locationComputerFactory_( new location::LocationComputerFactory() )
    , postureComputerFactory_( new posture::PostureComputerFactory() )
    , consumptionComputerFactory_( new dotation::ConsumptionComputerFactory() )
    , moveComputerFactory_( new moving::MoveComputerFactory() )
    , detectionComputerFactory_( new detection::DetectionComputerFactory() )
    , transportComputerFactory_( new transport::TransportComputerFactory() )
    , healComputerFactory_( new human::HealComputerFactory() )
    , onComponentFunctorComputerFactory_( new OnComponentFunctorComputerFactory() )
    , onComponentLendedFunctorComputerFactory_( new OnComponentLendedFunctorComputerFactory() )
    , loadingComputerFactory_( new transport::LoadingComputerFactory() )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AlgorithmsFactories destructor
// Created: MGD 2009-08-17
// -----------------------------------------------------------------------------
AlgorithmsFactories::~AlgorithmsFactories()
{
    // NOTHING
}


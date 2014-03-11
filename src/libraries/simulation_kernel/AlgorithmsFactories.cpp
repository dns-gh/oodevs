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
#include "simulation_kernel/HealComputerFactory.h"
#include "simulation_kernel/OnComponentFunctorComputerFactory.h"
#include "simulation_kernel/OnComponentLendedFunctorComputerFactory.h"
#include "simulation_kernel/DotationComputerFactory.h"
#include "simulation_kernel/UrbanLocationComputerFactory.h"
#include "simulation_kernel/WeaponReloadingComputerFactory.h"
#include <boost/serialization/export.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( AlgorithmsFactories )

// -----------------------------------------------------------------------------
// Name: AlgorithmsFactories constructor
// Created: MGD 2009-08-17
// -----------------------------------------------------------------------------
AlgorithmsFactories::AlgorithmsFactories()
    : weaponAvailabilityComputerFactory_      ( new firing::WeaponAvailabilityComputerFactory() )
    , composantesAbleToBeFiredComputerFactory_( new firing::ComposantesAbleToBeFiredComputerFactory() )
    , locationComputerFactory_                ( new location::LocationComputerFactory() )
    , postureComputerFactory_                 ( new posture::PostureComputerFactory() )
    , consumptionComputerFactory_             ( new dotation::ConsumptionComputerFactory() )
    , moveComputerFactory_                    ( new moving::MoveComputerFactory() )
    , healComputerFactory_                    ( new human::HealComputerFactory() )
    , onComponentFunctorComputerFactory_      ( new OnComponentFunctorComputerFactory() )
    , onComponentLendedFunctorComputerFactory_( new OnComponentLendedFunctorComputerFactory() )
    , dotationComputerFactory_                ( new dotation::DotationComputerFactory() )
    , weaponReloadingComputerFactory_         ( new firing::WeaponReloadingComputerFactory() )
    , urbanLocationComputerFactory_           ( new urbanLocation::UrbanLocationComputerFactory() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AlgorithmsFactories destructor
// Created: MGD 2009-08-17
// -----------------------------------------------------------------------------
AlgorithmsFactories::~AlgorithmsFactories()
{
    // NOTHING
}

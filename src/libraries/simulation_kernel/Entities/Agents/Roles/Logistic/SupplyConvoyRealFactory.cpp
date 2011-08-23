// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SupplyConvoyRealFactory.h"
#include "SupplyConvoyReal.h"

using namespace logistic;

SupplyConvoyRealFactory SupplyConvoyRealFactory::instance_;

// -----------------------------------------------------------------------------
// Name: SupplyConvoyRealFactory::Create
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
boost::shared_ptr< SupplyConvoy_ABC > SupplyConvoyRealFactory::Create( SupplyConvoyEventsObserver_ABC& eventsObserver, SupplySupplier_ABC& supplier, SupplyRequestParameters_ABC& parameters ) const
{
    return boost::shared_ptr< SupplyConvoy_ABC >( new SupplyConvoyReal( eventsObserver, supplier, parameters ) );
}

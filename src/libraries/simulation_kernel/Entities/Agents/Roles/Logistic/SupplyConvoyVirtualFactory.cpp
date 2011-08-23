// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SupplyConvoyVirtualFactory.h"
#include "SupplyConvoyVirtual.h"

using namespace logistic;

SupplyConvoyVirtualFactory SupplyConvoyVirtualFactory::instance_;

// -----------------------------------------------------------------------------
// Name: SupplyConvoyVirtualFactory::Create
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
boost::shared_ptr< SupplyConvoy_ABC > SupplyConvoyVirtualFactory::Create( SupplyConvoyEventsObserver_ABC& eventsObserver, SupplySupplier_ABC& supplier, SupplyRequestParameters_ABC& parameters ) const
{
    return boost::shared_ptr< SupplyConvoy_ABC >( new SupplyConvoyVirtual( eventsObserver, supplier, parameters ) );
}

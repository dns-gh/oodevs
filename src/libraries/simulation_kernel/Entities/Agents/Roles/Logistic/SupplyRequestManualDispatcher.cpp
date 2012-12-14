//*****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SupplyRequestManualDispatcher.h"
#include "SupplyRequest_ABC.h"

using namespace logistic;

// -----------------------------------------------------------------------------
// Name: SupplyDotationRequestBuilder constructor
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
SupplyRequestManualDispatcher::SupplyRequestManualDispatcher( SupplySupplier_ABC& supplier )
    : supplier_( supplier )
    , supply_  ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyDotationRequestBuilder destructor
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
SupplyRequestManualDispatcher::~SupplyRequestManualDispatcher()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestManualDispatcher::Dispatch
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void SupplyRequestManualDispatcher::Dispatch( SupplyRecipient_ABC& /*recipient*/, SupplyRequest_ABC& request )
{
    if( request.AffectSupplier( supplier_ ) )
        supply_ = true;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestManualDispatcher::AllowSupply
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
bool SupplyRequestManualDispatcher::AllowSupply()
{
    return supply_;
}

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
    : supplier_           ( supplier )
    , nbRequests_         ( 0 )
    , nbRequestsSatisfied_( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: SupplyDotationRequestBuilder destructor
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
SupplyRequestManualDispatcher::~SupplyRequestManualDispatcher()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyRequestManualDispatcher::Dispatch
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void SupplyRequestManualDispatcher::Dispatch( SupplyRequest_ABC& request )
{
    ++nbRequests_;
    if( request.AffectSupplier( supplier_ ) )
        ++ nbRequestsSatisfied_;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestManualDispatcher::AllowSupply
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
bool SupplyRequestManualDispatcher::AllowSupply()
{
    return nbRequestsSatisfied_ > 0;
}

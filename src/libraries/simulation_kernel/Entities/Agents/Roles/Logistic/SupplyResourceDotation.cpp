// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SupplyResourceDotation.h"
#include "Entities/Agents/Units/Dotations/PHY_Dotation.h"

using namespace logistic;

// -----------------------------------------------------------------------------
// Name: SupplyResourceDotation::SupplyResourceDotation
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
SupplyResourceDotation::SupplyResourceDotation( PHY_Dotation& dotation )
    : dotation_( dotation )
{
}

// -----------------------------------------------------------------------------
// Name: SupplyResourceDotation::SupplyResourceDotation
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
SupplyResourceDotation::~SupplyResourceDotation()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyResourceDotation::SupplyResourceStock
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
double SupplyResourceDotation::Supply( double quantity )
{
    return dotation_.Supply( quantity );
}

// -----------------------------------------------------------------------------
// Name: SupplyResourceDotation::SupplyResourceStock
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
bool SupplyResourceDotation::HasReachedSupplyThreshold() const
{
    return dotation_.HasReachedSupplyThreshold();
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyResourceDotation::GetCategory
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
const PHY_DotationCategory& SupplyResourceDotation::GetCategory() const
{
    return dotation_.GetCategory();
}


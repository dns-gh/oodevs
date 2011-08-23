// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SupplyResourceStock.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationStock.h"

using namespace logistic;

// -----------------------------------------------------------------------------
// Name: SupplyResourceStock::SupplyResourceStock
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
SupplyResourceStock::SupplyResourceStock( PHY_DotationStock& stock )
    : stock_( stock )
{
}

// -----------------------------------------------------------------------------
// Name: SupplyResourceStock::SupplyResourceStock
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
SupplyResourceStock::~SupplyResourceStock()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyResourceStock::SupplyResourceStock
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
double SupplyResourceStock::Supply( double quantity )
{
    return stock_.Supply( quantity );
}

// -----------------------------------------------------------------------------
// Name: SupplyResourceStock::SupplyResourceStock
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
bool SupplyResourceStock::HasReachedSupplyThreshold() const
{
    return stock_.HasReachedSupplyThreshold();
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyResourceStock::GetCategory
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
const PHY_DotationCategory& SupplyResourceStock::GetCategory() const
{
    return stock_.GetCategory();
}

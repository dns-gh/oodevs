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

BOOST_CLASS_EXPORT_IMPLEMENT( logistic::SupplyResourceStock )

// -----------------------------------------------------------------------------
// Name: SupplyResourceStock::SupplyResourceStock
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
SupplyResourceStock::SupplyResourceStock( PHY_DotationStock& stock )
    : stock_( &stock )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyResourceStock constructor
// Created: LDC 2013-01-16
// -----------------------------------------------------------------------------
SupplyResourceStock::SupplyResourceStock()
    : stock_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyResourceStock::SupplyResourceStock
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
SupplyResourceStock::~SupplyResourceStock()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyResourceStock::SupplyResourceStock
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
double SupplyResourceStock::Supply( double quantity )
{
    return stock_->Supply( quantity );
}

// -----------------------------------------------------------------------------
// Name: SupplyResourceStock::SupplyResourceStock
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
bool SupplyResourceStock::HasReachedSupplyThreshold() const
{
    return stock_->HasReachedSupplyThreshold();
}

// -----------------------------------------------------------------------------
// Name: SupplyResourceStock::GetCategory
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
const PHY_DotationCategory& SupplyResourceStock::GetCategory() const
{
    return stock_->GetCategory();
}

// -----------------------------------------------------------------------------
// Name: SupplyResourceStock::serialize
// Created: LDC 2013-01-16
// -----------------------------------------------------------------------------
template< typename Archive > void SupplyResourceStock::serialize( Archive& archive, const unsigned int )
{
    archive & boost::serialization::base_object< SupplyResource_ABC >(* this );
    archive & stock_;
}

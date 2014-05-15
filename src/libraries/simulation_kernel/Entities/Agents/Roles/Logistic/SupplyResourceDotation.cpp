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

BOOST_CLASS_EXPORT_IMPLEMENT( logistic::SupplyResourceDotation )

// -----------------------------------------------------------------------------
// Name: SupplyResourceDotation::SupplyResourceDotation
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
SupplyResourceDotation::SupplyResourceDotation( PHY_Dotation& dotation )
    : dotation_( &dotation )
{
}

// -----------------------------------------------------------------------------
// Name: SupplyResourceDotation constructor
// Created: LDC 2013-01-16
// -----------------------------------------------------------------------------
SupplyResourceDotation::SupplyResourceDotation()
    : dotation_( 0 )
{
    // NOTHING
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
    return dotation_->Supply( quantity );
}

// -----------------------------------------------------------------------------
// Name: SupplyResourceDotation::SupplyResourceStock
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
bool SupplyResourceDotation::HasReachedLowThreshold() const
{
    return dotation_->HasReachedLowThreshold();
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
    return dotation_->GetCategory();
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void SupplyResourceDotation::serialize
// Created: LDC 2013-01-16
// -----------------------------------------------------------------------------
template< typename Archive > void SupplyResourceDotation::serialize( Archive& archive, const unsigned int )
{
    archive & boost::serialization::base_object< SupplyResource_ABC >( *this );
    archive & dotation_;
}

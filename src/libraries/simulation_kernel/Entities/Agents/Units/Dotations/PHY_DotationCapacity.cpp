// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationCapacity.cpp $
// $Author: Nld $
// $Modtime: 28/01/05 16:17 $
// $Revision: 3 $
// $Workfile: PHY_DotationCapacity.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_DotationCapacity.h"

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacity constructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_DotationCapacity::PHY_DotationCapacity( const PHY_DotationCategory& category, xml::xistream& xis )
    : category_        ( category )
    , rCapacity_       ( xis.attribute< double >( "capacity" ) )
    , rSupplyThreshold_( 0. )
    , rSupplyThresholdPercentage_( xis.attribute< double >( "logistic-threshold" ) )
{
    ComputeThreshold();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacity constructor
// Created: ABR 2011-03-08
// -----------------------------------------------------------------------------
PHY_DotationCapacity::PHY_DotationCapacity( const PHY_DotationCategory& category, double capacity, double rSupplyThresholdPercentage )
    : category_        ( category )
    , rCapacity_       ( capacity )
    , rSupplyThreshold_( 0. )
    , rSupplyThresholdPercentage_( rSupplyThresholdPercentage )
{
    ComputeThreshold();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacity destructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_DotationCapacity::~PHY_DotationCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacity::ComputeThreshold
// Created: ABR 2011-07-27
// -----------------------------------------------------------------------------
void PHY_DotationCapacity::ComputeThreshold()
{
    if( rCapacity_ < 0 )
        throw MASA_EXCEPTION( "Dotation capacity < 0." );
    if( rSupplyThresholdPercentage_ < 0 || rSupplyThresholdPercentage_ > 100 )
        throw MASA_EXCEPTION( "Dotation logistic-threshold not in [0..100]." );
    rSupplyThreshold_ = rCapacity_ * rSupplyThresholdPercentage_ / 100.;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacity::GetCapacity
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
double PHY_DotationCapacity::GetCapacity() const
{
    return rCapacity_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacity::GetSupplyThreshold
// Created: NLD 2005-01-21
// -----------------------------------------------------------------------------
double PHY_DotationCapacity::GetSupplyThreshold() const
{
    return rSupplyThreshold_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacity::GetDefaultThreshold
// Created: JSR 2013-01-08
// -----------------------------------------------------------------------------
double PHY_DotationCapacity::GetDefaultThreshold() const
{
    return rSupplyThresholdPercentage_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacity::GetCategory
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
const PHY_DotationCategory& PHY_DotationCapacity::GetCategory() const
{
    return category_;
}

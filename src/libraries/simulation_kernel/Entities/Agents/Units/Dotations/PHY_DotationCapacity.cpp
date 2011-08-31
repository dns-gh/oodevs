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
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacity constructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_DotationCapacity::PHY_DotationCapacity( const PHY_DotationCategory& category, xml::xistream& xis )
    : category_        ( category )
    , rCapacity_       ( 0. )
    , rSupplyThreshold_( 0. )
{
    float rSupplyThresholdPercentage;

    xis >> xml::attribute( "capacity", rCapacity_ )
        >> xml::attribute( "logistic-threshold", rSupplyThresholdPercentage );

    ComputeThreshold( rSupplyThresholdPercentage );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacity constructor
// Created: ABR 2011-03-08
// -----------------------------------------------------------------------------
PHY_DotationCapacity::PHY_DotationCapacity( const PHY_DotationCategory& category, double capacity, double rSupplyThresholdPercentage )
    : category_        ( category )
    , rCapacity_       ( capacity )
    , rSupplyThreshold_( 0. )
{
    ComputeThreshold( rSupplyThresholdPercentage );
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
void PHY_DotationCapacity::ComputeThreshold( double rSupplyThresholdPercentage )
{
    if( rCapacity_ <= 0 )
        throw std::runtime_error( __FUNCTION__ " dotation capacity <= 0" );
    if( rSupplyThresholdPercentage < 0 || rSupplyThresholdPercentage > 100 )
        throw std::runtime_error( __FUNCTION__ " dotation logistic-threshold not in [0..100]" );
    rSupplyThreshold_ = static_cast< float >( rCapacity_ * rSupplyThresholdPercentage / 100. );
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
// Name: PHY_DotationCapacity::GetCategory
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
const PHY_DotationCategory& PHY_DotationCapacity::GetCategory() const
{
    return category_;
}

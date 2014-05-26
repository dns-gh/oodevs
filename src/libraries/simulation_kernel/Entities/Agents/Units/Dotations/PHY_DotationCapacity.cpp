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
    , rCapacity_               ( xis.attribute< double >( "capacity" ) )
    , rLowThreshold_           ( 0. )
    , rLowThresholdPercentage_ ( xis.attribute< double >( "low-threshold" ) )
    , rHighThreshold_          ( 0. )
    , rHighThresholdPercentage_( xis.attribute< double >( "high-threshold" ) )
{
    ComputeThreshold();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacity constructor
// Created: ABR 2011-03-08
// -----------------------------------------------------------------------------
PHY_DotationCapacity::PHY_DotationCapacity( const PHY_DotationCategory& category, double capacity, double rLowThresholdPercentage, double rHighThresholdPercentage )
    : category_                ( category )
    , rCapacity_               ( capacity )
    , rLowThreshold_           ( 0. )
    , rLowThresholdPercentage_ ( rLowThresholdPercentage )
    , rHighThresholdPercentage_( rHighThresholdPercentage )
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
    if( rLowThresholdPercentage_ < 0 || rLowThresholdPercentage_ > 100 )
        throw MASA_EXCEPTION( "Dotation low-threshold not in [0..100]." );
    rLowThreshold_ = rCapacity_ * rLowThresholdPercentage_ / 100.;
    if( rHighThresholdPercentage_ < 0 || rHighThresholdPercentage_ > 100 )
        throw MASA_EXCEPTION( "Dotation high-threshold not in [0..100]." );
    if( rHighThresholdPercentage_ < rLowThresholdPercentage_ )
        rHighThresholdPercentage_ = rLowThresholdPercentage_;
    rHighThreshold_ = rCapacity_ * rHighThresholdPercentage_ / 100.;
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
// Name: PHY_DotationCapacity::GetLowThreshold
// Created: NLD 2005-01-21
// -----------------------------------------------------------------------------
double PHY_DotationCapacity::GetLowThreshold() const
{
    return rLowThreshold_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacity::GetDefaultLowThreshold
// Created: JSR 2013-01-08
// -----------------------------------------------------------------------------
double PHY_DotationCapacity::GetDefaultLowThreshold() const
{
    return rLowThresholdPercentage_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacity::GetHighThreshold
// Created: SLI 2014-04-24
// -----------------------------------------------------------------------------
double PHY_DotationCapacity::GetHighThreshold() const
{
    return rHighThreshold_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacity::GetDefaultHighThreshold
// Created: SLI 2014-04-24
// -----------------------------------------------------------------------------
double PHY_DotationCapacity::GetDefaultHighThreshold() const
{
    return rHighThresholdPercentage_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacity::GetCategory
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
const PHY_DotationCategory& PHY_DotationCapacity::GetCategory() const
{
    return category_;
}

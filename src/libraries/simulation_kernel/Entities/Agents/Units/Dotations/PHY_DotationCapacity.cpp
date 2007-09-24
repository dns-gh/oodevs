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
#include "xeumeuleu/xml.h"

using namespace xml;

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

    xis >> attribute( "capacity", rCapacity_ )
        >> attribute( "logistic-threshold", rSupplyThresholdPercentage );

    if( rCapacity_ <= 0 )
        xis.error( "dotation: capacity <= 0" );
    if( rSupplyThresholdPercentage < 0 || rSupplyThresholdPercentage > 100 )
        xis.error( "dotation: logistic-threshold not in [0..100]" );

    rSupplyThreshold_ = (float)( rCapacity_ * rSupplyThresholdPercentage / 100. );
}


// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacity destructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_DotationCapacity::~PHY_DotationCapacity()
{

}

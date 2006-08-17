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
PHY_DotationCapacity::PHY_DotationCapacity( const PHY_DotationCategory& category, MIL_InputArchive& archive )
    : category_        ( category )
    , rCapacity_       ( 0. )
    , rSupplyThreshold_( 0. )
{
    archive.ReadAttribute( "contenance", rCapacity_, CheckValueGreater( 0. ) );
    float rSupplyThresholdPercentage;
    archive.ReadAttribute( "seuilLogistique", rSupplyThresholdPercentage, CheckValueBound( 0., 100. ) );
    rSupplyThreshold_ = (float)( rCapacity_ * rSupplyThresholdPercentage / 100. );
}


// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacity destructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_DotationCapacity::~PHY_DotationCapacity()
{

}

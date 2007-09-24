// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Dotations/PHY_DotationConsumption.cpp $
// $Author: Nld $
// $Modtime: 19/10/04 14:06 $
// $Revision: 1 $
// $Workfile: PHY_DotationConsumption.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_DotationConsumption.h"

#include "Tools/MIL_Tools.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: PHY_DotationConsumption constructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_DotationConsumption::PHY_DotationConsumption( const PHY_DotationCategory& category, xml::xistream& xis )
    : category_ ( category )
    , rConsumption_( 0. )
{
    xis >> attribute( "value", rConsumption_ );
    if( rConsumption_ < 0 )
        xis.error( "dotation: value < 0" );

    if ( rConsumption_ != 0. )
        rConsumption_ = 1. / MIL_Tools::ConvertHoursToSim( 1. / rConsumption_ );
}


// -----------------------------------------------------------------------------
// Name: PHY_DotationConsumption destructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_DotationConsumption::~PHY_DotationConsumption()
{

}

// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationCategory_IndirectFire_ABC.cpp $
// $Author: Nld $
// $Modtime: 17/03/05 14:53 $
// $Revision: 3 $
// $Workfile: PHY_DotationCategory_IndirectFire_ABC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_DotationCategory_IndirectFire_ABC.h"

#include "Tools/MIL_Tools.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire_ABC::PHY_DotationCategory_IndirectFire_ABC
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectFire_ABC::PHY_DotationCategory_IndirectFire_ABC( const PHY_IndirectFireDotationClass& category, const PHY_DotationCategory& dotationCategory, xml::xistream& xis )
    : category_         ( category )
    , dotationCategory_ ( dotationCategory )
    , rDispersionX_     ( 0. )
    , rDispersionY_     ( 0. )
{
    xis >> attribute( "intervention-type", nInterventionType_ )
        >> attribute( "x-dispersion", rDispersionX_ )
        >> attribute( "y-dispersion", rDispersionY_ );

    if( nInterventionType_ <= 0. )
        xis.error( "intervention-type <= 0" );
    if( rDispersionX_ <= 0. )
        xis.error( "rDispersionX_ <= 0" );
    if( rDispersionY_ <= 0. )
        xis.error( "rDispersionY_ <= 0" );

    rDispersionX_ = MIL_Tools::ConvertMeterToSim( rDispersionX_ );
    rDispersionY_ = MIL_Tools::ConvertMeterToSim( rDispersionY_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire_ABC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectFire_ABC::~PHY_DotationCategory_IndirectFire_ABC()
{
}

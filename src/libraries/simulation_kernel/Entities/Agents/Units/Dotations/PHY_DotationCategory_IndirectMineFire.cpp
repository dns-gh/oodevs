// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationCategory_IndirectMineFire.cpp $
// $Author: Nld $
// $Modtime: 17/03/05 14:53 $
// $Revision: 4 $
// $Workfile: PHY_DotationCategory_IndirectMineFire.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_DotationCategory_IndirectMineFire.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire::Create
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectFire_ABC& PHY_DotationCategory_IndirectMineFire::Create( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, xml::xistream& xis,
                                                                                      unsigned int nInterventionType, double rDispersionX, double rDispersionY )
{
    return *new PHY_DotationCategory_IndirectMineFire( type, dotationCategory, xis, nInterventionType, rDispersionX, rDispersionY );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectMineFire constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectMineFire::PHY_DotationCategory_IndirectMineFire( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, xml::xistream& xis,
                                                                              unsigned int nInterventionType, double rDispersionX, double rDispersionY )
    : PHY_DotationCategory_IndirectObjectCreationFire( type, dotationCategory, "mined area (scattered)", nInterventionType, rDispersionX, rDispersionY )
{
    xis >> xml::attribute( "mine-count", nNbrObjects_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectMineFire destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectMineFire::~PHY_DotationCategory_IndirectMineFire()
{
    // NOTHING
}

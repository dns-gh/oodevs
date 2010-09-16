// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_DotationCategory_IndirectFire_ABC.h"
#include "tools/MIL_Tools.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire_ABC::PHY_DotationCategory_IndirectFire_ABC
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectFire_ABC::PHY_DotationCategory_IndirectFire_ABC( const PHY_IndirectFireDotationClass& category, const PHY_DotationCategory& dotationCategory, xml::xistream& xis )
    : category_        ( category )
    , dotationCategory_( dotationCategory )
    , rDispersionX_    ( 0. )
    , rDispersionY_    ( 0. )
{
    xis >> xml::attribute( "intervention-type", nInterventionType_ )
        >> xml::attribute( "x-dispersion", rDispersionX_ )
        >> xml::attribute( "y-dispersion", rDispersionY_ );
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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire_ABC::GetDotationCategory
// Created: NLD 2006-08-07
// -----------------------------------------------------------------------------
const PHY_DotationCategory& PHY_DotationCategory_IndirectFire_ABC::GetDotationCategory() const
{
    return dotationCategory_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire_ABC::GetIndirectFireDotationCategory
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
const PHY_IndirectFireDotationClass& PHY_DotationCategory_IndirectFire_ABC::GetIndirectFireDotationCategory() const
{
    return category_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire_ABC::ConvertToInterventionType
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
MT_Float PHY_DotationCategory_IndirectFire_ABC::ConvertToInterventionType( unsigned int nNbr ) const
{
    return static_cast< float >( nNbr ) / static_cast< float >( nInterventionType_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire_ABC::ConvertToNbrAmmo
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
MT_Float PHY_DotationCategory_IndirectFire_ABC::ConvertToNbrAmmo( MT_Float rNbrIT ) const
{
    return nInterventionType_ * rNbrIT;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire_ABC::HasHit
// Created: NLD 2005-08-04
// -----------------------------------------------------------------------------
bool PHY_DotationCategory_IndirectFire_ABC::HasHit( const MIL_Agent_ABC& /*target*/, MT_Float ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire_ABC::ApplyEffect
// Created: MGD 2010-02-16
// -----------------------------------------------------------------------------
void PHY_DotationCategory_IndirectFire_ABC::ApplyEffect( const MIL_Agent_ABC& /*firer*/, MIL_Agent_ABC& /*target*/, MT_Float /*rInterventionTypeFired*/, PHY_FireResults_ABC& /*fireResult*/ ) const
{
    // NOTHING
}

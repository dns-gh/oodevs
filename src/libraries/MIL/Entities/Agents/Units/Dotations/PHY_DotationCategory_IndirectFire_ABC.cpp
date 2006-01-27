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

#include "MIL_pch.h"

#include "PHY_DotationCategory_IndirectFire_ABC.h"

#include "Tools/MIL_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire_ABC::PHY_DotationCategory_IndirectFire_ABC
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectFire_ABC::PHY_DotationCategory_IndirectFire_ABC( const PHY_IndirectFireDotationClass& category, const PHY_DotationCategory& dotationCategory, MIL_InputArchive& archive )
    : category_         ( category )
    , dotationCategory_ ( dotationCategory )
    , rDispersionX_     ( 0. )
    , rDispersionY_     ( 0. )
{
    archive.ReadField( "InterventionType", nInterventionType_, CheckValueGreater( 0 ) );

    archive.ReadField( "DispersionX", rDispersionX_, CheckValueGreater( 0. ) );
    archive.ReadField( "DispersionY", rDispersionY_, CheckValueGreater( 0. ) );

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


// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire_ABC::Fire
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
void PHY_DotationCategory_IndirectFire_ABC::Fire( const MIL_AgentPion& /*firer*/, const MT_Vector2D& /*vSourcePosition*/, const MT_Vector2D& /*vTargetPosition*/, MT_Float /*rInterventionTypeFired*/ ) const
{
    // NOTHING
}

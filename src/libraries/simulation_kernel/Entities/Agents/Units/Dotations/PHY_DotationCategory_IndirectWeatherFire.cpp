// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationCategory_IndirectWeatherFire.cpp $
// $Author: Nld $
// $Modtime: 17/03/05 14:53 $
// $Revision: 7 $
// $Workfile: PHY_DotationCategory_IndirectWeatherFire.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_DotationCategory_IndirectWeatherFire.h"
#include "Entities/Effects/MIL_Effect_Weather.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include <xeumeuleu/xml.hpp>
#include "Tools/MIL_Tools.h"
#include "tools/xmlcodecs.h"

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire::Create
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectFire_ABC& PHY_DotationCategory_IndirectWeatherFire::Create( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, xml::xistream& xis )
{
    return *new PHY_DotationCategory_IndirectWeatherFire( type, dotationCategory, xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectWeatherFire constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectWeatherFire::PHY_DotationCategory_IndirectWeatherFire( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, xml::xistream& xis )
    : PHY_DotationCategory_IndirectFire_ABC( type, dotationCategory, xis )
{
    std::string setupTime, lifeTime;
    xis >> xml::attribute( "setup-time", setupTime )
        >> xml::attribute( "life-time", lifeTime );

    if( ! tools::DecodeTime( setupTime, rDeploymentDuration_ ) || rDeploymentDuration_ < 0. )
        xis.error( "indirect-fire: setup-time < 0" );
    if( ! tools::DecodeTime( lifeTime, rLifeDuration_ ) || rLifeDuration_ < 0. )
        xis.error( "indirect-fire: life-time < 0" );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectWeatherFire destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectWeatherFire::~PHY_DotationCategory_IndirectWeatherFire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectWeatherFire::ApplyEffect
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
void PHY_DotationCategory_IndirectWeatherFire::ApplyEffect( const MIL_Agent_ABC* /*pFirer*/, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, double rInterventionTypeFired, PHY_FireResults_ABC& /*fireResult*/ ) const
{
    MT_Vector2D vFireDirection( 0., 1. );
    if( vTargetPosition != vSourcePosition )
        vFireDirection = ( vTargetPosition - vSourcePosition ).Normalize();
    MT_Vector2D vRotatedFireDirection = vFireDirection;
    vRotatedFireDirection.Rotate90();

    vFireDirection        *= ( rInterventionTypeFired * rDispersionX_ );
    vRotatedFireDirection *= ( rInterventionTypeFired * rDispersionY_ );

    const MT_Ellipse effectSurface( vTargetPosition, vTargetPosition + vFireDirection, vTargetPosition + vRotatedFireDirection );
    MIL_Effect_Weather* pEffect = new MIL_Effect_Weather( effectSurface, category_, MIL_Tools::ConvertSecondsToSim( rLifeDuration_ ), MIL_Tools::ConvertSecondsToSim( rDeploymentDuration_ ) );
    MIL_EffectManager::GetEffectManager().Register( *pEffect );
}

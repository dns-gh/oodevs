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
#include "Entities/MIL_EntityManager.h"

#include "Tools/MIL_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire::Create
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectFire_ABC& PHY_DotationCategory_IndirectWeatherFire::Create( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, MIL_InputArchive& archive )
{
    return *new PHY_DotationCategory_IndirectWeatherFire( type, dotationCategory, archive );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectWeatherFire constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectWeatherFire::PHY_DotationCategory_IndirectWeatherFire( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, MIL_InputArchive& archive )
    : PHY_DotationCategory_IndirectFire_ABC( type, dotationCategory, archive )
{
    archive.ReadTimeField( "DureeDeploiement", rDeploymentDuration_, CheckValueGreaterOrEqual( 0. ) );
    rDeploymentDuration_ = MIL_Tools::ConvertSecondsToSim( rDeploymentDuration_ );

    archive.ReadTimeField( "DureeVie", rLifeDuration_, CheckValueGreaterOrEqual( 0. ) );
    rLifeDuration_ = MIL_Tools::ConvertSecondsToSim( rLifeDuration_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectWeatherFire destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectWeatherFire::~PHY_DotationCategory_IndirectWeatherFire()
{

}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectWeatherFire::ApplyEffect
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
void PHY_DotationCategory_IndirectWeatherFire::ApplyEffect( const MIL_AgentPion& /*firer*/, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, MT_Float rInterventionTypeFired, PHY_FireResults_ABC& /*fireResult*/ ) const
{
    MT_Vector2D vFireDirection( 0., 1. );
    if( vTargetPosition != vSourcePosition )
        vFireDirection = ( vTargetPosition - vSourcePosition ).Normalize();
    MT_Vector2D vRotatedFireDirection = vFireDirection;
    vRotatedFireDirection.Rotate90();

    vFireDirection        *= ( rInterventionTypeFired * rDispersionX_ );
    vRotatedFireDirection *= ( rInterventionTypeFired * rDispersionY_ );

    const MT_Ellipse effectSurface( vTargetPosition, vTargetPosition + vFireDirection, vTargetPosition + vRotatedFireDirection );
    MIL_Effect_Weather* pEffect = new MIL_Effect_Weather( effectSurface, category_, rLifeDuration_, rDeploymentDuration_ );
    MIL_AgentServer::GetWorkspace().GetEntityManager().GetEffectManager().Register( *pEffect );
}


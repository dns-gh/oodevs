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
#include "PHY_IndirectFireDotationClass.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Effects/MIL_Effect_Weather.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/Orders/MIL_Report.h"
#include "simulation_terrain/TER_AgentManager.h"
#include "simulation_terrain/TER_World.h"
#include "Tools/MIL_Tools.h"
#include "tools/Codec.h"

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire::Create
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectFire_ABC& PHY_DotationCategory_IndirectWeatherFire::Create( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, xml::xistream& xis,
                                                                                         unsigned int nInterventionType, double rDispersionX, double rDispersionY, double rDetectionRange )
{
    return *new PHY_DotationCategory_IndirectWeatherFire( type, dotationCategory, xis, nInterventionType, rDispersionX, rDispersionY, rDetectionRange );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectWeatherFire constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectWeatherFire::PHY_DotationCategory_IndirectWeatherFire( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, xml::xistream& xis,
                                                                                    unsigned int nInterventionType, double rDispersionX, double rDispersionY, double rDetectionRange )
    : PHY_DotationCategory_IndirectFire_ABC( type, dotationCategory, nInterventionType, rDispersionX, rDispersionY, rDetectionRange )
    , rDispersionCoef_( xis.attribute< double >( "dispersion-factor", 0 ) )
{
    std::string setupTime, lifeTime;
    xis >> xml::attribute( "setup-time", setupTime )
        >> xml::attribute( "life-time", lifeTime );

    if( ! tools::DecodeTime( setupTime, rDeploymentDuration_ ) || rDeploymentDuration_ < 0. )
        throw MASA_EXCEPTION( xis.context() + "indirect-fire: setup-time < 0" );
    if( ! tools::DecodeTime( lifeTime, rLifeDuration_ ) || rLifeDuration_ < 0. )
        throw MASA_EXCEPTION( xis.context() + "indirect-fire: life-time < 0" );
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

    const double itm1 = std::max( 0., rInterventionTypeFired - 1 );
    vFireDirection        *= ( rDispersionX_ +  rDispersionX_ * itm1 * rDispersionCoef_ );
    vRotatedFireDirection *= ( rDispersionY_ +  rDispersionY_ * itm1 * rDispersionCoef_ );

    const MT_Ellipse effectSurface( vTargetPosition, vTargetPosition + vFireDirection, vTargetPosition + vRotatedFireDirection );
    const double deploymentDuration = MIL_Tools::ConvertSecondsToSim( rDeploymentDuration_ );
    MIL_Effect_Weather* pEffect = new MIL_Effect_Weather( effectSurface, category_, MIL_Tools::ConvertSecondsToSim( rLifeDuration_ ), deploymentDuration );
    MIL_EffectManager::GetEffectManager().Register( *pEffect );

    std::vector< unsigned int > fireEffectsIds;
    fireEffectsIds.push_back( pEffect->GetFireEffectId() );
    ApplyDetectionRangeEffect( vTargetPosition, fireEffectsIds );

    TER_Agent_ABC::T_AgentPtrVector targets;
    TER_World::GetWorld().GetAgentManager().GetListWithinEllipse( effectSurface, targets );
    for( TER_Agent_ABC::CIT_AgentPtrVector itTarget = targets.begin(); itTarget != targets.end(); ++itTarget )
    {
        MIL_Agent_ABC& target = static_cast< PHY_RoleInterface_Location& >( **itTarget ).GetAgent();
        if( target.GetRole< PHY_RoleInterface_Location >().GetHeight() > 0 )
            continue;
        if( category_ == PHY_IndirectFireDotationClass::eclairant_ )
            MIL_Report::PostEvent( target, report::eRC_PrisSousTirEclairant, dotationCategory_ );
        else if( category_ == PHY_IndirectFireDotationClass::fumigene_  )
            MIL_Report::PostEvent( target, report::eRC_PrisSousTirFumigene, dotationCategory_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectWeatherFire::GetSmokeDuration
// Created: LDC 2013-08-30
// -----------------------------------------------------------------------------
double PHY_DotationCategory_IndirectWeatherFire::GetSmokeDuration() const
{
    if( category_ == PHY_IndirectFireDotationClass::fumigene_  )
        return MIL_Tools::ConvertSecondsToSim( rDeploymentDuration_ + rLifeDuration_ );
    else
        return 0.;
}

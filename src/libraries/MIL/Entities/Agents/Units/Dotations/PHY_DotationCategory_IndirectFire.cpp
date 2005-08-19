// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Dotations/PHY_DotationCategory_IndirectFire.cpp $
// $Author: Nld $
// $Modtime: 29/03/05 14:16 $
// $Revision: 5 $
// $Workfile: PHY_DotationCategory_IndirectFire.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_DotationCategory_IndirectFire.h"
#include "PHY_DotationCategory.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "MT_Tools/MT_Ellipse.h"
#include "TER/TER_Agent_ABC.h"
#include "TER/TER_World.h"

MT_Random PHY_DotationCategory_IndirectFire::random_;

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire::Create
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectFire_ABC& PHY_DotationCategory_IndirectFire::Create( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, MIL_InputArchive& archive )
{
    return *new PHY_DotationCategory_IndirectFire( type, dotationCategory, archive );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectFire::PHY_DotationCategory_IndirectFire( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, MIL_InputArchive& archive )
    : PHY_DotationCategory_IndirectFire_ABC( type, dotationCategory, archive )
    , phs_                                 ( PHY_Posture::GetPostures().size(), 1. )
{
    archive.ReadField( "CoefficientNeutralisation", rNeutralizationCoef_, CheckValueGreaterOrEqual( 1. ) );

    if( !dotationCategory.HasAttritions() )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Dotation has no attritions defined", archive.GetContext() );

    archive.Section( "PHs" );
    archive.Section( "PostureCible" );
    const PHY_Posture::T_PostureMap& postures = PHY_Posture::GetPostures();
    for( PHY_Posture::CIT_PostureMap it = postures.begin(); it != postures.end(); ++it )
    {
        const PHY_Posture& posture = *it->second;    
        assert( phs_.size() > posture.GetID() );
        archive.ReadField( posture.GetName(), phs_[ posture.GetID() ] );
    }
    archive.EndSection(); // PostureCible
    archive.EndSection(); // PHs
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectFire::~PHY_DotationCategory_IndirectFire()
{
}


// =============================================================================
// OPERATIONS
// =============================================================================
   
// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire::Fire
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
void PHY_DotationCategory_IndirectFire::Fire( const MIL_AgentPion& /*firer*/, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, MT_Float rInterventionTypeFired, PHY_FireResults_ABC& fireResult ) const
{
    MT_Vector2D vFireDirection        = ( vTargetPosition - vSourcePosition ).Normalize();
    MT_Vector2D vRotatedFireDirection = vFireDirection;
    vRotatedFireDirection.Rotate90();

    vFireDirection        *= ( rInterventionTypeFired * rDispersionX_ );
    vRotatedFireDirection *= ( rInterventionTypeFired * rDispersionY_ );

    const MT_Ellipse attritionSurface     ( vTargetPosition, vTargetPosition + ( vFireDirection                        ),  vTargetPosition + ( vRotatedFireDirection                        ) );
    const MT_Ellipse neutralizationSurface( vTargetPosition, vTargetPosition + ( vFireDirection * rNeutralizationCoef_ ),  vTargetPosition + ( vRotatedFireDirection * rNeutralizationCoef_ ) );

    TER_Agent_ABC::T_AgentPtrVector targets;
    TER_World::GetWorld().GetListAgentWithinEllipse( neutralizationSurface, targets );
    for( TER_Agent_ABC::CIT_AgentPtrVector itTarget = targets.begin(); itTarget != targets.end(); ++itTarget )
    {
        MIL_Agent_ABC& target = static_cast< PHY_RoleInterface_Location& >( **itTarget ).GetAgent();

        if( target.GetRole< PHY_RoleInterface_Location >().GetHeight() > 0 )
            continue;

        PHY_RoleInterface_Composantes& targetRoleComposantes = target.GetRole< PHY_RoleInterface_Composantes >();
        targetRoleComposantes.Neutralize();
        if( attritionSurface.IsInside( (**itTarget).GetPosition() ) )
            targetRoleComposantes.ApplyIndirectFire( dotationCategory_, fireResult );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire::HasHit
// Created: NLD 2005-08-04
// -----------------------------------------------------------------------------
bool PHY_DotationCategory_IndirectFire::HasHit( const MIL_Agent_ABC& target ) const
{
    const PHY_RoleInterface_Posture& targetPosture = target.GetRole< PHY_RoleInterface_Posture >();
    
    const MT_Float rPH =   phs_[ targetPosture.GetCurrentPosture().GetID() ] *        targetPosture.GetPostureCompletionPercentage()
                         + phs_[ targetPosture.GetLastPosture   ().GetID() ] * ( 1. - targetPosture.GetPostureCompletionPercentage() );

    return random_.rand_oi() <= rPH ;
}

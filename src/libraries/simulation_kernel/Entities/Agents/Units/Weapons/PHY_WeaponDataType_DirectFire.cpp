// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Weapons/PHY_WeaponDataType_DirectFire.cpp $
// $Author: Jvt $
// $Modtime: 2/05/05 18:28 $
// $Revision: 12 $
// $Workfile: PHY_WeaponDataType_DirectFire.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_WeaponDataType_DirectFire.h"
#include "PHY_WeaponType.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Categories/PHY_Protection.h"
#include "Entities/Agents/Units/Categories/PHY_RoePopulation.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Composantes/PHY_Composante_ABC.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteType_ABC.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Posture/PHY_RolePion_Posture.h"
#include "Entities/Agents/Roles/HumanFactors/PHY_RolePion_HumanFactors.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Effects/MIL_Effect_DirectFirePion.h"
#include "Entities/Effects/MIL_Effect_DirectFirePopulation.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Entities/Populations/MIL_PopulationType.h"
#include "simulation_terrain/TER_PopulationConcentration_ABC.h"
#include "simulation_terrain/TER_PopulationFlow_ABC.h"
#include "simulation_terrain/TER_PopulationManager.h"
#include "simulation_terrain/TER_World.h"
#include "Tools/MIL_Tools.h"

MT_Random PHY_WeaponDataType_DirectFire::randomGenerator_;

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_WeaponDataType_DirectFire::PHY_WeaponDataType_DirectFire( MIL_EffectManager& manager, const PHY_WeaponType& weaponType, MIL_InputArchive& archive )
    : manager_   ( manager )
    , weaponType_( weaponType )
    , phs_       ( PHY_Volume::GetVolumes().size(), MT_InterpolatedFunction< MT_Float >( 0., 0. ) )
{
    archive.Section( "PHs" );
    const PHY_Volume::T_VolumeMap& volumes = PHY_Volume::GetVolumes();
    for ( PHY_Volume::CIT_VolumeMap itVolume = volumes.begin(); itVolume != volumes.end(); ++itVolume )
        InitializePH( *itVolume->second, archive );
    archive.EndSection(); // PHs
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_WeaponDataType_DirectFire::~PHY_WeaponDataType_DirectFire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::InitializePH
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_WeaponDataType_DirectFire::InitializePH( const PHY_Volume& volume, MIL_InputArchive& archive )
{
    std::stringstream strSectionName;
    strSectionName << "VolumeCible" << volume.GetName();

    assert( phs_.size() > volume.GetID() );

    MT_InterpolatedFunction< MT_Float >& phFunction = phs_[ volume.GetID() ];

    archive.BeginList( strSectionName.str() );

    while( archive.NextListElement() )
    {
        archive.Section( "PH" );

        MT_Float rDistance;
        MT_Float rPH;

        archive.ReadAttribute( "dist", rDistance, CheckValueGreaterOrEqual( 0. ) );
        archive.Read( rPH, CheckValueBound( 0., 1. ) );

        phFunction.AddNewPoint( MIL_Tools::ConvertMeterToSim( rDistance ), rPH );

        archive.EndSection(); // PH
    }

    archive.EndList(); // VolumeCibleXXX
}

// =============================================================================
// TOOLS
// =============================================================================

//-----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::GetMaxDistanceForPH
// Created: JVT 03-09-19
//-----------------------------------------------------------------------------
inline
MT_Float PHY_WeaponDataType_DirectFire::GetMaxDistanceForPH( MT_Float rPH, const PHY_Posture& firerPosture, const PHY_Posture& targetPosture, const PHY_Volume& targetVolume ) const
{
    return weaponType_.GetPHModificator( firerPosture, targetPosture ) * phs_[ targetVolume.GetID() ].GetMaxYForX( rPH );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::GetMinDistanceForPH
// Created: JVT 2004-12-17
// -----------------------------------------------------------------------------
inline
MT_Float PHY_WeaponDataType_DirectFire::GetMinDistanceForPH( MT_Float rPH, const PHY_Posture& firerPosture, const PHY_Posture& targetPosture, const PHY_Volume& targetVolume ) const
{
    return weaponType_.GetPHModificator( firerPosture, targetPosture ) * phs_[ targetVolume.GetID() ].GetMinYForX( rPH );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::GetMaxDistanceForPH
// Created: SBO 2006-01-25
// -----------------------------------------------------------------------------
MT_Float PHY_WeaponDataType_DirectFire::GetMaxDistanceForPH( MT_Float rPH, const PHY_RolePion_Posture& firerPosture, const PHY_RoleInterface_Posture& targetPosture, const PHY_Volume& targetVolume ) const
{
    assert( firerPosture.GetElongationFactor() > 0. );
    return weaponType_.GetPHModificator( firerPosture, targetPosture ) * phs_[ targetVolume.GetID() ].GetMaxYForX( rPH ) * firerPosture.GetElongationFactor();
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::GetMinDistanceForPH
// Created: SBO 2006-01-25
// -----------------------------------------------------------------------------
MT_Float PHY_WeaponDataType_DirectFire::GetMinDistanceForPH( MT_Float rPH, const PHY_RolePion_Posture& firerPosture, const PHY_RoleInterface_Posture& targetPosture, const PHY_Volume& targetVolume ) const
{
    assert( firerPosture.GetElongationFactor() > 0. );
    return weaponType_.GetPHModificator( firerPosture, targetPosture ) * phs_[ targetVolume.GetID() ].GetMinYForX( rPH ) * firerPosture.GetElongationFactor();
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::GetPH
// Created: NLD 2004-10-05
// Modified: JVT 2004-11-03
// -----------------------------------------------------------------------------
inline
MT_Float PHY_WeaponDataType_DirectFire::GetPH( const MIL_AgentPion& firer, const MIL_Agent_ABC& target, const PHY_Volume& targetVolume, MT_Float rDistance ) const
{
    const PHY_RolePion_Posture&      firerPosture  = firer .GetRole< PHY_RolePion_Posture      >();
    const PHY_RoleInterface_Posture& targetPosture = target.GetRole< PHY_RoleInterface_Posture >();

    assert( phs_.size() > targetVolume.GetID() );

    assert( firerPosture.GetElongationFactor() > 0. );
    rDistance /= firerPosture.GetElongationFactor();

    const MT_Float rPHModificator = weaponType_.GetPHModificator( firerPosture, targetPosture );
    if( rPHModificator <= 0. )
        return 0.;
    rDistance /= rPHModificator;

    const MT_Float rPH = phs_[ targetVolume.GetID() ]( rDistance );
    return firer.GetRole< PHY_RolePion_HumanFactors >().ModifyPH( rPH );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::GetPH
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
inline
MT_Float PHY_WeaponDataType_DirectFire::GetPH( const PHY_Posture& firerPosture, const PHY_Posture& targetPosture, const PHY_Volume& targetVolume, MT_Float rDistance ) const
{
    assert( phs_.size() > targetVolume.GetID() );
    
    const MT_Float rPHModificator = weaponType_.GetPHModificator( firerPosture, targetPosture );
    if( rPHModificator <= 0. )
        return 0.;
    rDistance /= rPHModificator;
    return phs_[ targetVolume.GetID() ]( rDistance );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::GetMaxRangeToFireOn
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
MT_Float PHY_WeaponDataType_DirectFire::GetMaxRangeToFireOn( const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rWantedPH ) const
{
    return GetMaxDistanceForPH( rWantedPH, PHY_Posture::posteReflexe_, PHY_Posture::posteReflexe_, targetComposanteType.GetVolume() );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::GetMinRangeToFireOn
// Created: JVT 2004-12-17
// -----------------------------------------------------------------------------
MT_Float PHY_WeaponDataType_DirectFire::GetMinRangeToFireOn( const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rWantedPH ) const
{
    return GetMinDistanceForPH( rWantedPH, PHY_Posture::posteReflexe_, PHY_Posture::posteReflexe_, targetComposanteType.GetVolume() );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::GetMaxRangeToFireOnWithPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
MT_Float PHY_WeaponDataType_DirectFire::GetMaxRangeToFireOnWithPosture( const PHY_ComposanteType_ABC& targetComposanteType, const MIL_AgentPion& firer, const MIL_Agent_ABC& target, MT_Float rWantedPH ) const
{
    const PHY_RolePion_Posture&      firerPosture  = firer .GetRole< PHY_RolePion_Posture      >();
    const PHY_RoleInterface_Posture& targetPosture = target.GetRole< PHY_RoleInterface_Posture >();

    return GetMaxDistanceForPH( rWantedPH, firerPosture, targetPosture, targetComposanteType.GetVolume() );   
}
    
// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::GetMinRangeToFireOnWithPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
MT_Float PHY_WeaponDataType_DirectFire::GetMinRangeToFireOnWithPosture( const PHY_ComposanteType_ABC& targetComposanteType, const MIL_AgentPion& firer, const MIL_Agent_ABC& target, MT_Float rWantedPH ) const
{
    const PHY_RolePion_Posture&      firerPosture  = firer .GetRole< PHY_RolePion_Posture      >();
    const PHY_RoleInterface_Posture& targetPosture = target.GetRole< PHY_RoleInterface_Posture >();

    return GetMinDistanceForPH( rWantedPH, firerPosture, targetPosture, targetComposanteType.GetVolume() );   
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::GetDangerosity
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
MT_Float PHY_WeaponDataType_DirectFire::GetDangerosity( const MIL_AgentPion& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& targetComposanteType, bool bUsePH ) const
{
    const PHY_Volume&     targetVolume      = targetComposanteType.GetVolume    (); 
    const PHY_Protection& targetProtection  = targetComposanteType.GetProtection();

    const PHY_RolePion_Location& firerLocation = firer.GetRole< PHY_RolePion_Location >();
    const MT_Vector3D vFirerPosition( firerLocation.GetPosition().rX_, firerLocation.GetPosition().rY_, firerLocation.GetAltitude() );
    
    const PHY_RoleInterface_Location& targetLocation = target.GetRole< PHY_RoleInterface_Location >();
    const MT_Vector3D vTargetPosition( targetLocation.GetPosition().rX_, targetLocation.GetPosition().rY_, targetLocation.GetAltitude() );
    
    MT_Float rDangerosity  = bUsePH ? GetPH( firer, target, targetVolume, vFirerPosition.Distance( vTargetPosition ) ) : 1.;
             rDangerosity *= weaponType_.GetDotationCategory().GetAttritionScore( targetProtection );
    return rDangerosity;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::GetDangerosity
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
MT_Float PHY_WeaponDataType_DirectFire::GetDangerosity( const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rDistBtwFirerAndTarget ) const
{
    const PHY_Volume&     targetVolume      = targetComposanteType.GetVolume    (); 
    const PHY_Protection& targetProtection  = targetComposanteType.GetProtection();

    MT_Float rDangerosity  = GetPH( PHY_Posture::posteReflexe_, PHY_Posture::posteReflexe_, targetVolume, rDistBtwFirerAndTarget );
             rDangerosity *= weaponType_.GetDotationCategory().GetAttritionScore( targetProtection );
    return rDangerosity;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::Fire
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void PHY_WeaponDataType_DirectFire::Fire( MIL_AgentPion& firer, MIL_Agent_ABC& target, PHY_Composante_ABC& compTarget, PHY_FireResults_ABC& fireResult, bool bUsePH ) const
{
    const PHY_RolePion_Location& firerLocation = firer.GetRole< PHY_RolePion_Location >();
    const PHY_RoleInterface_Location& targetLocation = target.GetRole< PHY_RoleInterface_Location >();

    if( bUsePH )
    {
        const PHY_Volume& targetVolume = compTarget.GetType().GetVolume(); //$$$$ ENCAPSULER

        const MT_Vector3D firerPosition( firerLocation.GetPosition().rX_, firerLocation.GetPosition().rY_, firerLocation.GetAltitude() );
        const MT_Vector3D targetPosition( targetLocation.GetPosition().rX_, targetLocation.GetPosition().rY_, targetLocation.GetAltitude() );

        const MT_Float rPH = GetPH( firer, target, targetVolume, firerPosition.Distance( targetPosition ) );
        if ( !( randomGenerator_.rand_oi() <= rPH ) ) 
            return;
    }

    MIL_Effect_DirectFirePion* pEffect = new MIL_Effect_DirectFirePion( weaponType_.GetDotationCategory(), target, compTarget, fireResult );
    manager_.Register( *pEffect );


    // handle direct-indirect fire on populations
    const MT_Vector2D firerPosition ( firerLocation.GetPosition().rX_, firerLocation.GetPosition().rY_ );
    const MT_Vector2D targetPosition( targetLocation.GetPosition().rX_, targetLocation.GetPosition().rY_ );

    TER_PopulationConcentration_ABC::T_PopulationConcentrationVector concentrations;
    TER_World::GetWorld().GetPopulationManager().GetConcentrationManager()
                         .GetListIntersectingLine( firerPosition, targetPosition, concentrations );
    for( TER_PopulationConcentration_ABC::CIT_PopulationConcentrationVector itConcentration = concentrations.begin();
        itConcentration != concentrations.end(); ++itConcentration )
    {
        MIL_PopulationConcentration* pElement = static_cast< MIL_PopulationConcentration* >( *itConcentration );
        MIL_Effect_DirectFirePopulation* pEffect = new MIL_Effect_DirectFirePopulation( *pElement, 1, fireResult );
        manager_.Register( *pEffect );
    }

    TER_PopulationFlow_ABC::T_PopulationFlowVector flows;
    TER_World::GetWorld().GetPopulationManager().GetFlowManager()
                         .GetListIntersectingLine( firerPosition, targetPosition, flows );
    for( TER_PopulationFlow_ABC::CIT_PopulationFlowVector itFlow = flows.begin(); itFlow != flows.end(); ++itFlow )
    {
        MIL_PopulationFlow* pElement = static_cast< MIL_PopulationFlow* >( *itFlow );
        MIL_Effect_DirectFirePopulation* pEffect = new MIL_Effect_DirectFirePopulation( *pElement, 1, fireResult );
        manager_.Register( *pEffect );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::Fire
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
void PHY_WeaponDataType_DirectFire::Fire( MIL_AgentPion& firer, MIL_PopulationElement_ABC& target, uint nNbrAmmoReserved, PHY_FireResults_ABC& fireResult ) const
{
    const PHY_RoePopulation& roe  = firer.GetRole< DEC_RolePion_Decision >().GetRoePopulation();
    const MT_Float           rPH  = target.GetPopulation().GetType().GetDamagePH( roe );

    uint nHit = 0;
    for( uint i = 1; i <= nNbrAmmoReserved; ++i )
        if( randomGenerator_.rand_oi() <= rPH )
            ++nHit;

    MIL_Effect_DirectFirePopulation* pEffect = new MIL_Effect_DirectFirePopulation( target, nHit, fireResult );
    manager_.Register( *pEffect );
}

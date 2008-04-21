// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Weapons/PHY_WeaponType.cpp $
// $Author: Jvt $
// $Modtime: 2/05/05 17:34 $
// $Revision: 10 $
// $Workfile: PHY_WeaponType.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_WeaponType.h"

#include "PHY_LauncherType.h"

#include "PHY_WeaponDataType_IndirectFire.h"
#include "PHY_WeaponDataType_DirectFire.h"
#include "PHY_Weapon.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory_IndirectFire_ABC.h"
#include "Entities/Agents/Units/Dotations/PHY_IndirectFireDotationClass.h"
#include "Entities/Agents/Units/Categories/PHY_Protection.h"
#include "Entities/Agents/Roles/Dotations/PHY_RolePion_Dotations.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"

#include "Tools/MIL_Tools.h"
#include "Tools/xmlcodecs.h"
#include "xeumeuleu/xml.h"



PHY_WeaponType::T_WeaponTypeMap PHY_WeaponType::weaponTypes_;

// =============================================================================
// MANAGER
// =============================================================================

struct PHY_WeaponType::LoadingWrapper
{
    void ReadWeapon( xml::xistream& xis, MIL_EffectManager& manager, const MIL_Time_ABC& time )
    {
        PHY_WeaponType::ReadWeapon( xis, manager, time );
    }
};

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_WeaponType::Initialize( MIL_EffectManager& manager, const MIL_Time_ABC& time, xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing weapon types" );

    LoadingWrapper loader;

    // Initialisation des composantes
    xis >> xml::start( "weapons" )
            >> xml::list( "weapon-system", loader, &LoadingWrapper::ReadWeapon, manager, time )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::ReadWeapon
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_WeaponType::ReadWeapon( xml::xistream& xis, MIL_EffectManager& manager, const MIL_Time_ABC& time )
{
    std::string strLauncher;
    std::string strAmmunition;

    xis >> xml::attribute( "launcher", strLauncher )
        >> xml::attribute( "munition", strAmmunition );

    const PHY_WeaponType*& pWeaponType = weaponTypes_[ std::make_pair( strLauncher, strAmmunition ) ];
    if ( pWeaponType )
        xis.error( "Weapon " + strLauncher + "/" + strAmmunition + " already registered" );
    pWeaponType = new PHY_WeaponType( manager, time, strLauncher, strAmmunition, xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::Terminate
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_WeaponType::Terminate()
{
    for( CIT_WeaponTypeMap it = weaponTypes_.begin() ; it != weaponTypes_.end(); ++it )
        delete it->second;
    weaponTypes_.clear();
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_WeaponType::PHY_WeaponType( MIL_EffectManager& manager, const MIL_Time_ABC& time, const std::string& strLauncher, const std::string& strAmmunition, xml::xistream& xis )
    : time_               ( time )
    , pLauncherType_      ( PHY_LauncherType::FindLauncherType( strLauncher ) )
    , pDotationCategory_  ( PHY_DotationType::FindDotationCategory( strAmmunition ) )
    , nNbrAmmoPerBurst_   ( 1 )
    , rBurstDuration_     ( 1. )
    , nNbrAmmoPerLoader_  ( 1 )
    , rReloadingDuration_ ( 1. )
    , pDirectFireData_    ( 0 )
    , pIndirectFireData_  ( 0 )
{ 
    if( !pLauncherType_ )
        xis.error( "Unknown launcher type '" + strLauncher + "'" );
    if( !pDotationCategory_ )
        xis.error( "Unknown dotation category '" + strAmmunition + "'" );
    std::string burstTime, reloadingTime;

    xis >> xml::start( "burst" )
            >> xml::attribute( "munition", nNbrAmmoPerBurst_ )
            >> xml::attribute( "duration", burstTime )
        >> xml::end()
        >> xml::start( "reloading" )
            >> xml::attribute( "munition", nNbrAmmoPerLoader_ )
            >> xml::attribute( "duration", reloadingTime )
        >> xml::end();
    if( ! tools::DecodeTime( burstTime,     rBurstDuration_ )
     || ! tools::DecodeTime( reloadingTime, rReloadingDuration_ ) )
        xis.error( "Invalid burst or reloading durations" );

    rBurstDuration_      = MIL_Tools::ConvertSecondsToSim( rBurstDuration_     );
    rReloadingDuration_  = MIL_Tools::ConvertSecondsToSim( rReloadingDuration_ );
    
    if( nNbrAmmoPerBurst_ <= 0 )
        xis.error( "burst: munition <= 0" );
    if( rBurstDuration_ <= 0 )
        xis.error( "burst: duration <= 0" );
    if( nNbrAmmoPerLoader_ <= 0 )
        xis.error( "reloading: munition <= 0" );
    if( rReloadingDuration_ <= 0 )
        xis.error( "reloading: duration <= 0" );

    InitializeDirectFireData  ( manager, xis );
    InitializeIndirectFireData( xis );

    strName_ = pLauncherType_->GetName() + "/" + pDotationCategory_->GetName();
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType destructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
PHY_WeaponType::~PHY_WeaponType()
{
    if( pDirectFireData_ )
        delete pDirectFireData_;

    if( pIndirectFireData_ )
        delete pIndirectFireData_;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::InitializeDirectFireData
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_WeaponType::InitializeDirectFireData( MIL_EffectManager& manager, xml::xistream& xis )
{
    pDirectFireData_ = 0;
    xis >> xml::list( "direct-fire", *this, &PHY_WeaponType::ReadDirect, manager );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::ReadDirect
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_WeaponType::ReadDirect( xml::xistream& xis, MIL_EffectManager& manager )
{
    assert( pLauncherType_ );
    assert( pDotationCategory_ );

    if ( !pLauncherType_->CanDirectFire() )
        xis.error( "Associated launcher can not direct fire" );
    if ( !pDotationCategory_->CanBeUsedForDirectFire() )
        xis.error( "Associated ammunition can not direct fire" );

    pDirectFireData_ = new PHY_WeaponDataType_DirectFire( manager, *this, xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::InitializeIndirectFireData
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_WeaponType::InitializeIndirectFireData( xml::xistream& xis )
{
    pIndirectFireData_ = 0;
    xis >> xml::list( "indirect-fire", *this, &PHY_WeaponType::ReadIndirect );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::ReadIndirect
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_WeaponType::ReadIndirect( xml::xistream& xis )
{
    assert( pLauncherType_ );
    assert( pDotationCategory_ );

    if ( !pLauncherType_->CanIndirectFire() )
        xis.error( "Associated launcher can not indirect fire" );
    if ( !pDotationCategory_->CanBeUsedForIndirectFire() )
        xis.error( "Associated ammunition can not indirect fire" );

    pIndirectFireData_ = new PHY_WeaponDataType_IndirectFire( *this, xis );
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::InstanciateWeapon
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_Weapon& PHY_WeaponType::InstanciateWeapon( bool bMajor ) const
{
    return *new PHY_Weapon( time_, *this, bMajor );
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetPHModificator
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
MT_Float PHY_WeaponType::GetPHModificator( const PHY_RolePion_Posture& firerPosture, const PHY_RoleInterface_Posture& targetPosture ) const
{
    assert( pLauncherType_ );
    return pLauncherType_->GetPHModificator( firerPosture, targetPosture );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetPHModificator
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
MT_Float PHY_WeaponType::GetPHModificator( const PHY_Posture& firerPosture, const PHY_Posture& targetPosture ) const
{
    assert( pLauncherType_ );
    return pLauncherType_->GetPHModificator( firerPosture, targetPosture );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::IndirectFire
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void PHY_WeaponType::IndirectFire( MIL_AgentPion& firer, MIL_Effect_IndirectFire& effect, uint nNbrAmmoReserved ) const
{
    assert( pIndirectFireData_ );
    pIndirectFireData_->Fire( firer, effect, nNbrAmmoReserved );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::DirectFire
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void PHY_WeaponType::DirectFire( MIL_AgentPion& firer, MIL_Agent_ABC& target, PHY_Composante_ABC& compTarget, PHY_FireResults_ABC& fireResult, bool bUsePH ) const
{
    assert( pDirectFireData_ );
    pDirectFireData_->Fire( firer, target, compTarget, fireResult, bUsePH );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::DirectFire
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
void PHY_WeaponType::DirectFire( MIL_AgentPion& firer, MIL_PopulationElement_ABC& target, uint nNbrAmmoReserved, PHY_FireResults_ABC& fireResult ) const
{
    assert( pDirectFireData_ );
    pDirectFireData_->Fire( firer, target, nNbrAmmoReserved, fireResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::ThrowSmoke
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
void PHY_WeaponType::ThrowSmoke( MIL_AgentPion& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, uint nNbrAmmo, PHY_FireResults_ABC& fireResult ) const
{
    assert( pIndirectFireData_ );
    pIndirectFireData_->ThrowSmoke( firer, vSourcePosition, vTargetPosition, nNbrAmmo, fireResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetDangerosity
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
MT_Float PHY_WeaponType::GetDangerosity( const MIL_AgentPion& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& targetComposanteType, bool bUsePH ) const
{
    assert( pDotationCategory_ );

    if( !pDirectFireData_ || !firer.GetRole< PHY_RolePion_Dotations >().HasDotation( *pDotationCategory_ ) )
        return 0.;
    return pDirectFireData_->GetDangerosity( firer, target, targetComposanteType, bUsePH );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetDangerosity
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
MT_Float PHY_WeaponType::GetDangerosity( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rDistBtwFirerAndTarget ) const
{
    assert( pDotationCategory_ );

    if( !pDirectFireData_ || !firer.GetRole< PHY_RoleInterface_Dotations >().HasDotation( *pDotationCategory_ ) )
        return 0.;
    return pDirectFireData_->GetDangerosity( targetComposanteType, rDistBtwFirerAndTarget );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetMaxRangeToFireOn
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
MT_Float PHY_WeaponType::GetMaxRangeToFireOn( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rWantedPH ) const
{
    assert( pDotationCategory_ );

    if( !pDirectFireData_ || !firer.GetRole< PHY_RoleInterface_Dotations >().HasDotation( *pDotationCategory_ ) )
        return 0.;   
    return pDirectFireData_->GetMaxRangeToFireOn( targetComposanteType, rWantedPH );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetMinRangeToFireOn
// Created: JVT 2004-12-17
// -----------------------------------------------------------------------------
MT_Float PHY_WeaponType::GetMinRangeToFireOn( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rWantedPH ) const
{
    assert( pDotationCategory_ );

    if( !pDirectFireData_ || !firer.GetRole< PHY_RoleInterface_Dotations >().HasDotation( *pDotationCategory_ ) )
        return std::numeric_limits< MT_Float >::max(); 
    return pDirectFireData_->GetMinRangeToFireOn( targetComposanteType, rWantedPH );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetMaxRangeToFireOnWithPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
MT_Float PHY_WeaponType::GetMaxRangeToFireOnWithPosture( const MIL_AgentPion& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rWantedPH ) const
{
    assert( pDotationCategory_ );

    if( !pDirectFireData_ || !firer.GetRole< PHY_RoleInterface_Dotations >().HasDotation( *pDotationCategory_ ) )
        return 0.;
    return pDirectFireData_->GetMaxRangeToFireOnWithPosture( targetComposanteType, firer, target, rWantedPH );
}
    
// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetMinRangeToFireOnWithPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
MT_Float PHY_WeaponType::GetMinRangeToFireOnWithPosture( const MIL_AgentPion& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rWantedPH ) const
{
    assert( pDotationCategory_ );

    if( !pDirectFireData_ || !firer.GetRole< PHY_RoleInterface_Dotations >().HasDotation( *pDotationCategory_ ) )
        return 0.;
    return pDirectFireData_->GetMinRangeToFireOnWithPosture( targetComposanteType, firer, target, rWantedPH );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetMaxRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
MT_Float PHY_WeaponType::GetMaxRangeToIndirectFire( const MIL_Agent_ABC& firer, bool bCheckDotationsAvailability ) const
{
    if ( !pIndirectFireData_ )
        return -1.;

    if( bCheckDotationsAvailability && !firer.GetRole< PHY_RoleInterface_Dotations >().HasDotation( *pDotationCategory_ ) )
        return -1.;
        
    return pIndirectFireData_->GetMaxRange();
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetMinRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
MT_Float PHY_WeaponType::GetMinRangeToIndirectFire( const MIL_Agent_ABC& firer, bool bCheckDotationsAvailability ) const
{
    if ( !pIndirectFireData_ )
      return std::numeric_limits< MT_Float >::max();

    if( bCheckDotationsAvailability && !firer.GetRole< PHY_RoleInterface_Dotations >().HasDotation( *pDotationCategory_ ) )
        return std::numeric_limits< MT_Float >::max();
        
    return pIndirectFireData_->GetMinRange();
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetMaxRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
MT_Float PHY_WeaponType::GetMaxRangeToIndirectFire() const
{
    assert( pDotationCategory_ );
    return pIndirectFireData_ ? pIndirectFireData_->GetMaxRange() : -1.;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetMaxRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
MT_Float PHY_WeaponType::GetMinRangeToIndirectFire() const
{
    assert( pDotationCategory_ );
    return pIndirectFireData_ ? pIndirectFireData_->GetMinRange() : std::numeric_limits< MT_Float >::max();
}

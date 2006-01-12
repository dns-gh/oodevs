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

#include "MIL_pch.h"

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

PHY_WeaponType::T_WeaponTypeMap PHY_WeaponType::weaponTypes_;

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_WeaponType::Initialize( MIL_InputArchive& archive )
{
    MT_LOG_INFO_MSG( "Initializing weapon types" );

    // Initialisation des composantes
    archive.BeginList( "Armements" );

    while ( archive.NextListElement() )
    {
        archive.Section( "Armement" );

        std::string strLauncher;
        std::string strAmmunition;

        archive.ReadAttribute( "lanceur", strLauncher );
        archive.ReadAttribute( "munition", strAmmunition );

        const PHY_WeaponType*& pWeaponType = weaponTypes_[ std::make_pair( strLauncher, strAmmunition ) ];
        if ( pWeaponType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Weapon %s/%s already registered", strLauncher.c_str(), strAmmunition.c_str() ), archive.GetContext() );

        pWeaponType = new PHY_WeaponType( strLauncher, strAmmunition, archive );

        archive.EndSection(); // Armement
    }
    archive.EndList(); // Armements
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
PHY_WeaponType::PHY_WeaponType( const std::string& strLauncher, const std::string& strAmmunition, MIL_InputArchive& archive )
    : pLauncherType_      ( PHY_LauncherType::FindLauncherType( strLauncher ) )
    , pDotationCategory_  ( PHY_DotationType::munition_.FindDotationCategory( strAmmunition ) )
    , nNbrAmmoPerBurst_   ( 1 )
    , rBurstDuration_     ( 1. )
    , nNbrAmmoPerLoader_  ( 1 )
    , rReloadingDuration_ ( 1. )
    , pDirectFireData_    ( 0 )
    , pIndirectFireData_  ( 0 )
{ 
    if( !pLauncherType_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Unknown launcher type '%s'", strLauncher.c_str() ), archive.GetContext() );
    if( !pDotationCategory_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Unknown dotation category '%s'", strAmmunition.c_str() ), archive.GetContext() );

    archive.ReadField    ( "NbrMunitionParRafale"       , nNbrAmmoPerBurst_  , CheckValueGreaterOrEqual( 0. ) );
    archive.ReadTimeField( "DureeRafale"                , rBurstDuration_    , CheckValueGreater       ( 0. ) );
    archive.ReadField    ( "NbrMunitionsParRechargement", nNbrAmmoPerLoader_ , CheckValueGreater       ( 0. ) );
    archive.ReadTimeField( "DureeRechargement"          , rReloadingDuration_, CheckValueGreater       ( 0. ) );

    rBurstDuration_      = MIL_Tools::ConvertSecondsToSim( rBurstDuration_     );
    rReloadingDuration_  = MIL_Tools::ConvertSecondsToSim( rReloadingDuration_ );
    
    InitializeDirectFireData  ( archive );
    InitializeIndirectFireData( archive );

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
void PHY_WeaponType::InitializeDirectFireData( MIL_InputArchive& archive )
{
    pDirectFireData_ = 0;
    
    if( !archive.Section( "Direct", MIL_InputArchive::eNothing ) )
        return;

    assert( pLauncherType_ );
    assert( pDotationCategory_ );

    if ( !pLauncherType_->CanDirectFire() )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Associated launcher can not direct fire", archive.GetContext() );
    if ( !pDotationCategory_->CanBeUsedForDirectFire() )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Associated ammunition can not direct fire", archive.GetContext() );

    pDirectFireData_ = new PHY_WeaponDataType_DirectFire( *this, archive );
    archive.EndSection(); // Direct
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::InitializeIndirectFireData
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_WeaponType::InitializeIndirectFireData( MIL_InputArchive& archive )
{
    pIndirectFireData_ = 0;

    if ( !archive.Section( "Indirect", MIL_InputArchive::eNothing ) )
        return;

    assert( pLauncherType_ );
    assert( pDotationCategory_ );

    if ( !pLauncherType_->CanIndirectFire() )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Associated launcher can not indirect fire", archive.GetContext() );
    if ( !pDotationCategory_->CanBeUsedForIndirectFire() )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Associated ammunition can not indirect fire", archive.GetContext() );

    pIndirectFireData_ = new PHY_WeaponDataType_IndirectFire( *this, archive );
    archive.EndSection(); // Indirect
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
    return *new PHY_Weapon( *this, bMajor );
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
void PHY_WeaponType::ThrowSmoke( MIL_AgentPion& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, uint nNbrAmmo ) const
{
    assert( pIndirectFireData_ );
    pIndirectFireData_->ThrowSmoke( firer, vSourcePosition, vTargetPosition, nNbrAmmo );
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
MT_Float PHY_WeaponType::GetMaxRangeToFireOnWithPosture( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, const PHY_Posture& targetPosture, MT_Float rWantedPH ) const
{
    assert( pDotationCategory_ );

    if( !pDirectFireData_ || !firer.GetRole< PHY_RoleInterface_Dotations >().HasDotation( *pDotationCategory_ ) )
        return 0.;
    const PHY_Posture& firerPosture = firer.GetRole< PHY_RoleInterface_Posture >().GetCurrentPosture();
    return pDirectFireData_->GetMaxRangeToFireOnWithPosture( targetComposanteType, firerPosture, targetPosture, rWantedPH );
}
    
// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetMinRangeToFireOnWithPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
MT_Float PHY_WeaponType::GetMinRangeToFireOnWithPosture( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, const PHY_Posture& targetPosture, MT_Float rWantedPH ) const
{
    assert( pDotationCategory_ );

    if( !pDirectFireData_ || !firer.GetRole< PHY_RoleInterface_Dotations >().HasDotation( *pDotationCategory_ ) )
        return 0.;
    const PHY_Posture& firerPosture = firer.GetRole< PHY_RoleInterface_Posture >().GetCurrentPosture();
    return pDirectFireData_->GetMinRangeToFireOnWithPosture( targetComposanteType, firerPosture, targetPosture, rWantedPH );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetMaxRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
MT_Float PHY_WeaponType::GetMaxRangeToIndirectFire( const MIL_Agent_ABC& firer, const PHY_IndirectFireDotationClass& ammoClass, bool bCheckDotationsAvailability ) const
{
    assert( pDotationCategory_ );

    if ( !pIndirectFireData_ 
      || !pDotationCategory_->GetIndirectFireData()
      ||  pDotationCategory_->GetIndirectFireData()->GetIndirectFireDotationCategory() != ammoClass )
        return -1.;

    if( bCheckDotationsAvailability && !firer.GetRole< PHY_RoleInterface_Dotations >().HasDotation( *pDotationCategory_ ) )
        return -1.;
        
    return pIndirectFireData_->GetMaxRange();
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetMinRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
MT_Float PHY_WeaponType::GetMinRangeToIndirectFire( const MIL_Agent_ABC& firer, const PHY_IndirectFireDotationClass& ammoClass, bool bCheckDotationsAvailability ) const
{
    assert( pDotationCategory_ );
    
    if ( !pIndirectFireData_ 
      || !pDotationCategory_->GetIndirectFireData()
      ||  pDotationCategory_->GetIndirectFireData()->GetIndirectFireDotationCategory() != ammoClass )
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

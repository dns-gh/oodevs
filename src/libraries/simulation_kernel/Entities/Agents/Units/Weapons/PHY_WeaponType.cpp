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
#include "DefaultDotationComputer.h"
#include "MIL_Time_ABC.h"
#include "PHY_LauncherType.h"
#include "PHY_WeaponDataType_IndirectFire.h"
#include "PHY_WeaponDataType_DirectFire.h"
#include "PHY_Weapon.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Tools/MIL_Tools.h"
#include "tools/Codec.h"
#include "simulation_kernel/AlgorithmsFactories.h"
#include "simulation_kernel/DefaultDotationComputer.h"
#include "MT_Tools/MT_Logger.h"

PHY_WeaponType::T_WeaponTypeMap PHY_WeaponType::weaponTypes_;

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_WeaponType::Initialize( const MIL_Time_ABC& time, xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing weapon types" );
    xis >> xml::start( "weapons" )
            >> xml::list( "weapon-system", boost::bind( &PHY_WeaponType::ReadWeapon, _1, boost::cref( time ) ) )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::ReadWeapon
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_WeaponType::ReadWeapon( xml::xistream& xis, const MIL_Time_ABC& time )
{
    std::string strLauncher;
    std::string strAmmunition;

    xis >> xml::attribute( "launcher", strLauncher )
        >> xml::attribute( "munition", strAmmunition );

    const PHY_WeaponType*& pWeaponType = weaponTypes_[ std::make_pair( strLauncher, strAmmunition ) ];
    if( pWeaponType )
        throw MASA_EXCEPTION( xis.context() + "Weapon " + strLauncher + "/" + strAmmunition + " already registered" );
    pWeaponType = new PHY_WeaponType( time, strLauncher, strAmmunition,
        weaponTypes_.size() - 1, xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::Terminate
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_WeaponType::Terminate()
{
    for( auto it = weaponTypes_.begin() ; it != weaponTypes_.end(); ++it )
        delete it->second;
    weaponTypes_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_WeaponType::PHY_WeaponType( const MIL_Time_ABC& time, const std::string& strLauncher, const std::string& strAmmunition, size_t identifier, xml::xistream& xis )
    : time_               ( time )
    , pLauncherType_      ( PHY_LauncherType::FindLauncherType( strLauncher ) )
    , pDotationCategory_  ( PHY_DotationType::FindDotationCategory( strAmmunition ) )
    , nNbrAmmoPerBurst_   ( 1 )
    , rBurstDuration_     ( 1. )
    , nNbrAmmoPerLoader_  ( 1 )
    , rReloadingDuration_ ( 1. )
    , pDirectFireData_    ( 0 )
    , pIndirectFireData_  ( 0 )
    , identifier_         ( identifier )
{
    if( !pLauncherType_ )
        throw MASA_EXCEPTION( xis.context() + "Unknown launcher type '" + strLauncher + "'" );
    if( !pDotationCategory_ )
        throw MASA_EXCEPTION( xis.context() + "Unknown dotation category '" + strAmmunition + "'" );
    std::string burstTime, reloadingTime;

    xis >> xml::start( "burst" )
            >> xml::attribute( "munition", nNbrAmmoPerBurst_ )
            >> xml::attribute( "duration", burstTime )
        >> xml::end
        >> xml::start( "reloading" )
            >> xml::attribute( "munition", nNbrAmmoPerLoader_ )
            >> xml::attribute( "duration", reloadingTime )
        >> xml::end;
    if( ! tools::DecodeTime( burstTime,     rBurstDuration_ )
     || ! tools::DecodeTime( reloadingTime, rReloadingDuration_ ) )
        throw MASA_EXCEPTION( xis.context() + "Invalid burst or reloading durations" );

    const double timeFactor = time.GetTickDuration();
    rBurstDuration_     /= timeFactor;
    rReloadingDuration_ /= timeFactor;

    if( nNbrAmmoPerBurst_ <= 0 )
        throw MASA_EXCEPTION( xis.context() + "burst: munition <= 0" );
    if( rBurstDuration_ <= 0 )
        throw MASA_EXCEPTION( xis.context() + "burst: duration <= 0" );
    if( nNbrAmmoPerLoader_ <= 0 )
        throw MASA_EXCEPTION( xis.context() + "reloading: munition <= 0" );
    if( rReloadingDuration_ <= 0 )
        throw MASA_EXCEPTION( xis.context() + "reloading: duration <= 0" );

    InitializeDirectFireData  ( xis );
    InitializeIndirectFireData( xis, timeFactor );

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
void PHY_WeaponType::InitializeDirectFireData(  xml::xistream& xis )
{
    pDirectFireData_ = 0;
    xis >> xml::list( "direct-fire", *this, &PHY_WeaponType::ReadDirect );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::ReadDirect
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_WeaponType::ReadDirect( xml::xistream& xis )
{
    assert( pLauncherType_ );
    assert( pDotationCategory_ );

    if( !pLauncherType_->CanDirectFire() )
        throw MASA_EXCEPTION( xis.context() + "Associated launcher cannot direct fire" );
    if( !pDotationCategory_->CanBeUsedForDirectFire() )
        throw MASA_EXCEPTION( xis.context() + "Associated ammunition cannot direct fire" );

    pDirectFireData_ = new PHY_WeaponDataType_DirectFire( *this, xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::InitializeIndirectFireData
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_WeaponType::InitializeIndirectFireData( xml::xistream& xis, double timeFactor )
{
    pIndirectFireData_ = 0;
    xis >> xml::list( "indirect-fire", *this, &PHY_WeaponType::ReadIndirect, timeFactor );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::ReadIndirect
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_WeaponType::ReadIndirect( xml::xistream& xis, double timeFactor )
{
    assert( pLauncherType_ );
    assert( pDotationCategory_ );

    if( !pLauncherType_->CanIndirectFire() )
        throw MASA_EXCEPTION( xis.context() + "Associated launcher cannot indirect fire" );
    if( !pDotationCategory_->CanBeUsedForIndirectFire() )
        throw MASA_EXCEPTION( xis.context() + "Associated ammunition cannot indirect fire" );

    pIndirectFireData_ = new PHY_WeaponDataType_IndirectFire( *this, xis, timeFactor );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::InstanciateWeapon
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_Weapon& PHY_WeaponType::InstanciateWeapon( bool bMajor ) const
{
    return *new PHY_Weapon( time_, *this, bMajor );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetPHModificator
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
double PHY_WeaponType::GetPHModificator( const PHY_RoleInterface_Posture& firerPosture, const PHY_RoleInterface_Posture& targetPosture ) const
{
    assert( pLauncherType_ );
    return pLauncherType_->GetPHModificator( firerPosture, targetPosture );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetPHModificator
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
double PHY_WeaponType::GetPHModificator() const
{
    assert( pLauncherType_ );
    return pLauncherType_->GetPHModificator();
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::IndirectFire
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void PHY_WeaponType::IndirectFire( MIL_Effect_IndirectFire& effect, unsigned int nNbrAmmoReserved ) const
{
    assert( pIndirectFireData_ );
    pIndirectFireData_->Fire( effect, nNbrAmmoReserved );
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
void PHY_WeaponType::DirectFire( MIL_AgentPion& firer, MIL_PopulationElement_ABC& target, unsigned int nNbrAmmoReserved, PHY_FireResults_ABC& fireResult ) const
{
    assert( pDirectFireData_ );
    pDirectFireData_->Fire( firer, target, nNbrAmmoReserved, fireResult, GetDotationCategory().GetAmmoDotationClass() );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::ThrowSmoke
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
void PHY_WeaponType::ThrowSmoke( MIL_Agent_ABC& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, unsigned int nNbrAmmo, PHY_FireResults_ABC& fireResult ) const
{
    assert( pIndirectFireData_ );
    pIndirectFireData_->ThrowSmoke( firer, vSourcePosition, vTargetPosition, nNbrAmmo, fireResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetSmokeDuration
// Created: LDC 2013-08-30
// -----------------------------------------------------------------------------
double PHY_WeaponType::GetSmokeDuration() const
{
    return pDotationCategory_->GetSmokeDuration();
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetDangerosity
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
double PHY_WeaponType::GetDangerosity( const MIL_AgentPion& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& targetComposanteType, bool bUsePH, bool bUseAmmo ) const
{
    if( !pDirectFireData_ )
        return 0.;
    if( bUseAmmo)
    {
        assert( pDotationCategory_ );

        dotation::DefaultDotationComputer dotationComputer;
        MIL_AgentPion& localFirer = const_cast< MIL_AgentPion& >( firer );
        localFirer.Execute< dotation::DotationComputer_ABC >( dotationComputer );

        if( !dotationComputer.HasDotation( *pDotationCategory_ ) )//@TODO MGD See if it's possible to merge all HasDotation of this class
            return 0.;
    }
    return pDirectFireData_->GetDangerosity( firer, target, targetComposanteType, bUsePH );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetDangerosity
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
double PHY_WeaponType::GetDangerosity( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, double rDistBtwFirerAndTarget, bool bUseAmmo ) const
{
    if( !pDirectFireData_ )
        return 0.;
    if( bUseAmmo)
    {
        assert( pDotationCategory_ );

        dotation::DefaultDotationComputer dotationComputer;
        MIL_Agent_ABC& localFirer = const_cast< MIL_Agent_ABC& >( firer );
        localFirer.Execute< dotation::DotationComputer_ABC >( dotationComputer );

        if( !dotationComputer.HasDotation( *pDotationCategory_ ) )
            return 0.;
    }
    return pDirectFireData_->GetDangerosity( targetComposanteType, rDistBtwFirerAndTarget );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetMaxRangeToFireOn
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
double PHY_WeaponType::GetMaxRangeToFireOn( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, double rWantedPH, const PHY_DotationCategory* dotation ) const
{
    if( !pDirectFireData_ )
        return 0.;
    assert( pDotationCategory_ );
    if( dotation )
    {
        if( dotation != pDotationCategory_ )
            return 0.;

        dotation::DefaultDotationComputer dotationComputer;
        MIL_Agent_ABC& localFirer = const_cast< MIL_Agent_ABC& >( firer );
        localFirer.Execute< dotation::DotationComputer_ABC >( dotationComputer );

        if( !dotationComputer.HasDotation( *pDotationCategory_ ) )
            return 0.;
    }
    return pDirectFireData_->GetMaxRangeToFireOn( targetComposanteType, rWantedPH );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetMinRangeToFireOn
// Created: JVT 2004-12-17
// -----------------------------------------------------------------------------
double PHY_WeaponType::GetMinRangeToFireOn( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, double rWantedPH ) const
{
    if( !pDirectFireData_ )
        return std::numeric_limits< double >::max();
    assert( pDotationCategory_ );

    dotation::DefaultDotationComputer dotationComputer;
    MIL_Agent_ABC& localFirer = const_cast< MIL_Agent_ABC& >( firer );
    localFirer.Execute< dotation::DotationComputer_ABC >( dotationComputer );

    if( !dotationComputer.HasDotation( *pDotationCategory_ ) )
        return std::numeric_limits< double >::max();
    return pDirectFireData_->GetMinRangeToFireOn( targetComposanteType, rWantedPH );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetMaxRangeToFire
// Created: DDA 2010-05-03
// -----------------------------------------------------------------------------
double PHY_WeaponType::GetMaxRangeToFire( const MIL_Agent_ABC& /*pion*/, double rWantedPH ) const
{
    if( !pDirectFireData_ )
        return 0.;
    return pDirectFireData_->GetMaxRangeToFire( rWantedPH );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetMaxRangeToFireOnWithPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
double PHY_WeaponType::GetMaxRangeToFireOnWithPosture( const MIL_Agent_ABC& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& targetComposanteType, double rWantedPH ) const
{
    if( !pDirectFireData_ )
        return 0.;
    assert( pDotationCategory_ );

    dotation::DefaultDotationComputer dotationComputer;
    MIL_Agent_ABC& localFirer = const_cast< MIL_Agent_ABC& >( firer );
    localFirer.Execute< dotation::DotationComputer_ABC >( dotationComputer );

    if( !dotationComputer.HasDotation( *pDotationCategory_ ) )
        return 0.;
    return pDirectFireData_->GetMaxRangeToFireOnWithPosture( targetComposanteType, firer, target, rWantedPH );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetMinRangeToFireOnWithPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
double PHY_WeaponType::GetMinRangeToFireOnWithPosture( const MIL_Agent_ABC& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& targetComposanteType, double rWantedPH ) const
{
    if( !pDirectFireData_ )
        return 0.;
    assert( pDotationCategory_ );

    dotation::DefaultDotationComputer dotationComputer;
    MIL_Agent_ABC& localFirer = const_cast< MIL_Agent_ABC& >( firer );
    localFirer.Execute< dotation::DotationComputer_ABC >( dotationComputer );

    if( !dotationComputer.HasDotation( *pDotationCategory_ ) )
        return 0.;
    return pDirectFireData_->GetMinRangeToFireOnWithPosture( targetComposanteType, firer, target, rWantedPH );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetMaxRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
double PHY_WeaponType::GetMaxRangeToIndirectFire( const MIL_Agent_ABC& firer, bool bCheckDotationsAvailability ) const
{
    if( !pIndirectFireData_ )
        return -1.;

    dotation::DefaultDotationComputer dotationComputer;
    MIL_Agent_ABC& localFirer = const_cast< MIL_Agent_ABC& >( firer );
    localFirer.Execute< dotation::DotationComputer_ABC >( dotationComputer );

    if( bCheckDotationsAvailability && !dotationComputer.HasDotation( *pDotationCategory_ ) )
        return -1.;

    return pIndirectFireData_->GetMaxRange();
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetMinRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
double PHY_WeaponType::GetMinRangeToIndirectFire( const MIL_Agent_ABC& firer, bool bCheckDotationsAvailability ) const
{
    if( !pIndirectFireData_ )
      return std::numeric_limits< double >::max();

    dotation::DefaultDotationComputer dotationComputer;
    MIL_Agent_ABC& localFirer = const_cast< MIL_Agent_ABC& >( firer );
    localFirer.Execute< dotation::DotationComputer_ABC >( dotationComputer );

    if( bCheckDotationsAvailability && !dotationComputer.HasDotation( *pDotationCategory_ ) )
        return std::numeric_limits< double >::max();

    return pIndirectFireData_->GetMinRange();
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetMaxRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
double PHY_WeaponType::GetMaxRangeToIndirectFire() const
{
    assert( pDotationCategory_ );
    return pIndirectFireData_ ? pIndirectFireData_->GetMaxRange() : -1.;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetMaxRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
double PHY_WeaponType::GetMinRangeToIndirectFire() const
{
    assert( pDotationCategory_ );
    return pIndirectFireData_ ? pIndirectFireData_->GetMinRange() : std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetMaxRangeToDirectFire
// Created: MCO 2012-11-23
// -----------------------------------------------------------------------------
double PHY_WeaponType::GetMaxRangeToDirectFire() const
{
    return pDirectFireData_ ? pDirectFireData_->GetMaxRange() : 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::FindWeaponType
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
const PHY_WeaponType* PHY_WeaponType::FindWeaponType( const std::string& strLauncher, const std::string& strAmmunition )
{
    auto it = weaponTypes_.find( std::make_pair( strLauncher, strAmmunition ) );
    if( it == weaponTypes_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetDotationCategory
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
const PHY_DotationCategory& PHY_WeaponType::GetDotationCategory() const
{
    assert( pDotationCategory_ );
    return *pDotationCategory_;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetNbrAmmoPerLoader
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
unsigned int PHY_WeaponType::GetNbrAmmoPerLoader() const
{
    return nNbrAmmoPerLoader_;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetNbrAmmoPerBurst
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
unsigned int PHY_WeaponType::GetNbrAmmoPerBurst() const
{
    return nNbrAmmoPerBurst_;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetBurstDuration
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
double PHY_WeaponType::GetBurstDuration() const
{
    return rBurstDuration_;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetReloadingDuration
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
double PHY_WeaponType::GetReloadingDuration() const
{
    return rReloadingDuration_;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetName
// Created: NLD 2004-10-07
// -----------------------------------------------------------------------------
const std::string& PHY_WeaponType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::GetID
// Created: MCO 2012-12-17
// -----------------------------------------------------------------------------
std::size_t PHY_WeaponType::GetID() const
{
    return identifier_;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::CanDirectFire
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
bool PHY_WeaponType::CanDirectFire() const
{
    return pDirectFireData_ != 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponType::CanIndirectFire
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
bool PHY_WeaponType::CanIndirectFire() const
{
    return pIndirectFireData_ != 0;
}

// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Composantes/PHY_ComposantePion.inl $
// $Author: Jvt $
// $Modtime: 11/05/05 14:49 $
// $Revision: 15 $
// $Workfile: PHY_ComposantePion.inl $
//
// *****************************************************************************

#include "Entities/Agents/Units/Categories/PHY_Protection.h"

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyHumansWounds
// Created: NLD 2004-10-07
// -----------------------------------------------------------------------------
inline
void PHY_ComposantePion::ApplyHumansWounds( const PHY_ComposanteState& composanteNewState, PHY_FireDamages_Agent& fireDamages )
{
    humans_.ApplyWounds( composanteNewState, fireDamages );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetOperationalState
// Created: NLD 2006-02-09
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposantePion::GetOperationalState() const
{
    assert( pState_ );
    if( *pState_ != PHY_ComposanteState::undamaged_ )
        return 0.;

    return ( 1. - rOpStateWeightHumans_ ) + rOpStateWeightHumans_ * humans_.GetOperationalState();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetNbrUsableHumans
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
inline
uint PHY_ComposantePion::GetNbrUsableHumans() const
{
    return humans_.GetNbrUsableHumans();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetBreakdown
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
inline
const PHY_Breakdown* PHY_ComposantePion::GetBreakdown() const
{
    return pBreakdown_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanHaul
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
inline
bool PHY_ComposantePion::CanHaul() const
{
    assert( pType_ );
    return pState_->IsUsable() && CanBeUsed() && pType_->CanHaul();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanHaul
// Created: NLD 2005-03-29
// -----------------------------------------------------------------------------
inline
bool PHY_ComposantePion::CanHaul( const PHY_ComposanteTypePion& composanteType ) const
{
    assert( pType_ );
    return pState_->IsUsable() && CanBeUsed() && pType_->CanHaul( composanteType );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanRepair
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
inline
bool PHY_ComposantePion::CanRepair( const PHY_Breakdown& breakdown ) const
{
    assert( pType_ );
    return pState_->IsUsable() && CanBeUsed() && pType_->CanRepair( breakdown );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanRepair
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
inline
bool PHY_ComposantePion::CanRepair() const
{
    assert( pType_ );
    return pState_->IsUsable() && CanBeUsed() && pType_->CanRepair();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanBeLent
// Created: JVT 2005-01-17
// -----------------------------------------------------------------------------
inline
bool PHY_ComposantePion::CanBeLent() const
{
    return pState_->IsUsable() && CanBeUsed();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanEvacuateCasualties
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
inline
bool PHY_ComposantePion::CanEvacuateCasualties() const
{
    assert( pType_ );
    return pState_->IsUsable() && CanBeUsed() && pType_->CanEvacuateCasualties();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanCollectCasualties
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
inline
bool PHY_ComposantePion::CanCollectCasualties() const
{
    assert( pType_ );
    return pState_->IsUsable() && CanBeUsed() && pType_->CanCollectCasualties();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanHealHumans
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
inline
bool PHY_ComposantePion::CanHealHuman( const PHY_Human& human ) const
{
    assert( pType_ );
    return pState_->IsUsable() && CanBeUsed() && pType_->CanHealHuman( human );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::Heal
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
inline
void PHY_ComposantePion::Heal( PHY_Human& human ) const
{
    assert( pType_ );
    assert( bUsedForLogistic_ );
    
    pType_->Heal( human );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetHealingTime
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
inline
uint PHY_ComposantePion::GetHealingTime( const PHY_Human& human ) const
{
    assert( pType_ );
    assert( bUsedForLogistic_ );
    return pType_->GetHealingTime( human );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanDiagnoseHumans
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
bool PHY_ComposantePion::CanDiagnoseHumans() const
{
    assert( pType_ );
    return pState_->IsUsable() && CanBeUsed() && pType_->CanDiagnoseHumans();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanSortHumans
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
bool PHY_ComposantePion::CanSortHumans() const
{
    assert( pType_ );
    return pState_->IsUsable() && CanBeUsed() && pType_->CanSortHumans();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanConvoyCommand
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
inline
bool PHY_ComposantePion::CanConvoyCommand() const
{
    assert( pType_ );
    return pState_->IsUsable() && CanBeUsed() && pType_->CanConvoyCommand();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanConvoyTransport
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
bool PHY_ComposantePion::CanConvoyTransport() const
{
    assert( pType_ );
    return pState_->IsUsable() && CanBeUsed() && pType_->CanConvoyTransport();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetConvoyTransporterCapacity
// Created: NLD 2005-07-18
// -----------------------------------------------------------------------------
inline
void PHY_ComposantePion::GetConvoyTransporterCapacity( MT_Float& rWeightMax, MT_Float& rVolumeMax ) const
{
    assert( pType_ );
    return pType_->GetConvoyTransporterCapacity( rWeightMax, rVolumeMax );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetConvoyTransporterLoadingTime
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
inline
uint PHY_ComposantePion::GetConvoyTransporterLoadingTime() const
{
    assert( pType_ );
    return pType_->GetConvoyTransporterLoadingTime();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetConvoyTransporterUnloadingTime
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
inline
uint PHY_ComposantePion::GetConvoyTransporterUnloadingTime() const
{
    assert( pType_ );
    return pType_->GetConvoyTransporterUnloadingTime();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::StartUsingForLogistic
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
inline
void PHY_ComposantePion::StartUsingForLogistic()
{
    assert( !bUsedForLogistic_ );
    bUsedForLogistic_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::StopUsingForLogistic
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
inline
void PHY_ComposantePion::StopUsingForLogistic()
{
    assert( bUsedForLogistic_ );
    bUsedForLogistic_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetNeutralizationTime
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
inline
uint PHY_ComposantePion::GetNeutralizationTime() const
{
    assert( pState_ );
 
    if ( !pState_->IsUsable() )
        return 0;
 
    assert( pType_ );
    return pType_->GetProtection().GetNeutralizationTime();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetType
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
inline
const PHY_ComposanteTypePion& PHY_ComposantePion::GetType() const
{
    assert( pType_ );
    return *pType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetState
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
inline
const PHY_ComposanteState& PHY_ComposantePion::GetState() const
{
    assert( pState_ );
    return *pState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyOnSensors
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
template< typename T > inline
void PHY_ComposantePion::ApplyOnSensors( T& func ) const
{
    for( CIT_SensorVector itSensor = sensors_.begin(); itSensor != sensors_.end(); ++itSensor )
        func( **itSensor );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyOnRadars
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
template< typename T > inline
void PHY_ComposantePion::ApplyOnRadars( T& func ) const
{
    GetType().ApplyOnRadars( func );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::IsMajor
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
inline
bool PHY_ComposantePion::IsMajor() const
{
    return bMajor_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetWeight
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposantePion::GetWeight() const
{
    assert( pType_ );
    return pType_->GetWeight();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetMajorScore
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
inline
uint PHY_ComposantePion::GetMajorScore() const
{
    if( !pState_->IsUsable() && CanBeUsed() ) //$$$ C'est de la merde 
        return 0;

    if( IsMajor() )
    {
        if( !pState_->IsDamaged() )
            return 4;
        return 3;
    }
    if( !pState_->IsDamaged() )
        return 2;
    return 1;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanPerceive
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
inline
bool PHY_ComposantePion::CanPerceive() const
{
    assert( pState_ );
    return pState_->IsUsable() && CanBeUsed();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanMove
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
inline
bool PHY_ComposantePion::CanMove() const
{
    assert( pState_ );
    return pState_->IsUsable() && CanBeUsedForMove();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanBeFired
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
inline
bool PHY_ComposantePion::CanBeFired() const
{
    assert( pState_ );
    return pState_->IsUsable() && CanBeUsed();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanTransport
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
inline
bool PHY_ComposantePion::CanTransport() const
{
    assert( pType_ );
    return pState_->CanTransport() && pType_->CanTransport() && CanBeUsed();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanTransportHumans
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
bool PHY_ComposantePion::CanTransportHumans() const
{
    assert( pType_ );
    return !bLoadable_ && pState_->CanTransport() && pType_->CanTransportHumans() && CanBeUsed();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanBeLoaded
// Created: NLD 2004-10-07
// -----------------------------------------------------------------------------
inline
bool PHY_ComposantePion::CanBeLoaded() const
{
    return pState_->IsUsable() && bLoadable_; //$$$$ CanBeUsed() ??
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanBeTransported
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
inline
bool PHY_ComposantePion::CanBeTransported() const
{
    return pState_->IsUsable(); //$$$$ CanBeUsed() ??
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanConstruct
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
inline
bool PHY_ComposantePion::CanConstruct( const MIL_RealObjectType& objectType ) const
{
    assert( pType_ );
    return pType_->CanConstruct( objectType ) && CanBeUsed() && pState_->IsUsable(); 
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanDestroy
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
bool PHY_ComposantePion::CanDestroy( const MIL_RealObjectType& objectType ) const
{
    assert( pType_ );
    return pType_->CanDestroy( objectType ) && CanBeUsed() && pState_->IsUsable(); 
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanMine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
bool PHY_ComposantePion::CanMine( const MIL_RealObjectType& objectType ) const
{
    assert( pType_ );
    return pType_->CanMine( objectType ) && CanBeUsed() && pState_->IsUsable(); 
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanDemine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
bool PHY_ComposantePion::CanDemine( const MIL_RealObjectType& objectType ) const
{
    assert( pType_ );
    return pType_->CanDemine( objectType ) && CanBeUsed() && pState_->IsUsable(); 
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanBypass
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
bool PHY_ComposantePion::CanBypass( const MIL_RealObjectType& objectType, bool bObjectIsMined ) const
{
    assert( pType_ );
    return pType_->CanBypass( objectType, bObjectIsMined ) && CanBeUsed() && pState_->IsUsable(); 
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetConstructionTime
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposantePion::GetConstructionTime( const MIL_RealObjectType& objectType, MT_Float rSizeCoef ) const
{
    if( !( CanBeUsed() && pState_->IsUsable() ) )
        return std::numeric_limits< MT_Float >::max();

    assert( pType_ );
    return pType_->GetConstructionTime( objectType, rSizeCoef );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetDestructionTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposantePion::GetDestructionTime( const MIL_RealObjectType& objectType, MT_Float rSizeCoef ) const
{
    if( !( CanBeUsed() && pState_->IsUsable() ) )
        return std::numeric_limits< MT_Float >::max();

    assert( pType_ );
    return pType_->GetDestructionTime( objectType, rSizeCoef );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetMiningTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposantePion::GetMiningTime( const MIL_RealObjectType& objectType ) const
{
    if( !( CanBeUsed() && pState_->IsUsable() ) )
        return std::numeric_limits< MT_Float >::max();

    assert( pType_ );
    return pType_->GetMiningTime( objectType );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetDeminingTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposantePion::GetDeminingTime( const MIL_RealObjectType& objectType ) const
{
    if( !( CanBeUsed() && pState_->IsUsable() ) )
        return std::numeric_limits< MT_Float >::max();

    assert( pType_ );
    return pType_->GetDeminingTime( objectType );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetBypassTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposantePion::GetBypassTime( const MIL_RealObjectType& objectType, MT_Float rSizeCoef, bool bObjectIsMined ) const
{
    if( !( CanBeUsed() && pState_->IsUsable() ) )
        return std::numeric_limits< MT_Float >::max();

    assert( pType_ );
    return pType_->GetBypassTime( objectType, rSizeCoef, bObjectIsMined );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::HealAllHumans
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
inline
void PHY_ComposantePion::HealAllHumans()
{
    if( *pState_ != PHY_ComposanteState::dead_ ) 
        humans_.HealAllHumans();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::HealHumans
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
inline
uint PHY_ComposantePion::HealHumans( const PHY_HumanRank& rank, uint nNbrToChange )
{
    if( *pState_ != PHY_ComposanteState::dead_ )
        return humans_.HealHumans( rank, nNbrToChange );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::WoundHumans
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
inline
uint PHY_ComposantePion::WoundHumans( const PHY_HumanRank& rank, uint nNbrToChange, const PHY_HumanWound& wound )
{
    if( *pState_ != PHY_ComposanteState::dead_ )
        return humans_.WoundHumans( rank, nNbrToChange, wound );
    return 0;    
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ChangeHumanRank
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
inline
bool PHY_ComposantePion::ChangeHumanRank( const PHY_HumanRank& oldRank, const PHY_HumanRank& newRank, const PHY_HumanWound& wound )
{
    return humans_.ChangeHumanRank( oldRank, newRank, wound );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApproximateTravelTime
// Created: NLD 2004-12-24
// -----------------------------------------------------------------------------
inline
uint PHY_ComposantePion::ApproximateTravelTime( const MT_Vector2D& vSourcePos, const MT_Vector2D& vTargetPos ) const
{
    assert( pType_ );
    assert( pType_->GetMaxSpeed() != 0. );
    return (uint)( 3. * vSourcePos.Distance( vTargetPos ) / pType_->GetMaxSpeed() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetRole
// Created: NLD 2005-03-09
// -----------------------------------------------------------------------------
inline
const PHY_RolePion_Composantes& PHY_ComposantePion::GetRole() const
{
    assert( pRole_ );
    return *pRole_;
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_Dotation.h"
#include "PHY_DotationGroup.h"
#include "PHY_DotationCapacity.h"
#include "PHY_DotationType.h"
#include "PHY_DotationCategory.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationGroupContainer.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "MT_Tools/MT_Logger.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_Dotation )

const double PHY_Dotation::maxCapacity_ = 10000000;

// -----------------------------------------------------------------------------
// Name: PHY_Dotation constructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_Dotation::PHY_Dotation( const PHY_DotationCategory& category, PHY_DotationGroup& group, bool bInfiniteDotations )
    : pCategory_              ( &category )
    , pGroup_                 ( &group )
    , rValue_                 ( 0 )
    , rRequestedValue_        ( 0 )
    , rLastValueSent_         ( 0 )
    , rCapacity_              ( 0 )
    , rConsumptionReservation_( 0 )
    , rFireReservation_       ( 0 )
    , rSupplyThreshold_       ( 0 )
    , bNotified_              ( false )
    , bDotationBlocked_       ( false )
    , bInfiniteDotations_     ( bInfiniteDotations )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
PHY_Dotation::PHY_Dotation()
    : pCategory_              ( 0 )
    , pGroup_                 ( 0 )
    , rValue_                 ( 0 )
    , rRequestedValue_        ( 0 )
    , rLastValueSent_         ( 0 )
    , rCapacity_              ( 0 )
    , rConsumptionReservation_( 0 )
    , rFireReservation_       ( 0 )
    , rSupplyThreshold_       ( 0 )
    , bNotified_              ( false )
    , bDotationBlocked_       ( false )
    , bInfiniteDotations_     ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation destructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_Dotation::~PHY_Dotation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_Dotation::serialize( Archive& ar, const unsigned int )
{
    ar & pCategory_
       & pGroup_
       & rValue_
       & rCapacity_
       & rConsumptionReservation_
       & rFireReservation_
       & rSupplyThreshold_
       & bNotified_
       & bDotationBlocked_
       & bInfiniteDotations_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::ReadValue
// Created: NLD 2005-01-21
// -----------------------------------------------------------------------------
void PHY_Dotation::ReadValue( xml::xistream& xis )
{
    double rValue = xis.attribute< double >( "quantity" );
    if( rValue < 0. )
        throw MASA_EXCEPTION( xis.context() + "rValue is not greater or equal to 0." );
    if( rValue > rCapacity_ )
        rCapacity_ = rValue;
    if( xis.has_attribute( "logistic-threshold" ) )
        rSupplyThreshold_ = std::min( rCapacity_ * xis.attribute< double >( "logistic-threshold" ) / 100.f, rCapacity_ );
    SetValue( rValue );
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::SetValue
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
void PHY_Dotation::SetValue( double rValue )
{
    if( rValue == rValue_ )
        return;
    if( bDotationBlocked_ )
        return;
    if( bInfiniteDotations_ && rValue < rSupplyThreshold_ )
        rValue = rCapacity_;
    rValue = std::min( rValue, maxCapacity_ );
    const bool bSupplyThresholdAlreadyReached = HasReachedSupplyThreshold();
    assert( pCategory_ );
    if( pCategory_->IsSignificantChange( rValue, rLastValueSent_, rCapacity_ ) || ( rValue < rSupplyThreshold_ ) != bSupplyThresholdAlreadyReached )
    {
        assert( pGroup_ );
        pGroup_->NotifyDotationChanged( *this );
        rLastValueSent_ = rValue;
    }
    rValue_ = rValue;
    bNotified_ &= rValue_ <= rSupplyThreshold_; // reset notify flag as soon as value > threshold
    if( HasReachedSupplyThreshold() )
    {
        if( rRequestedValue_ == 0 )
            rRequestedValue_ = rCapacity_ - rValue_;
        NotifySupplyNeeded();
    }
    else
        rRequestedValue_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::NotifySupplyNeeded
// Created: BAX 2014-02-28
// -----------------------------------------------------------------------------
void PHY_Dotation::NotifySupplyNeeded()
{
    const bool first = !bNotified_;
    bNotified_ = true;
    pGroup_->NotifySupplyNeeded( *pCategory_, first );
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::AddCapacity
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
void PHY_Dotation::AddCapacity( const PHY_DotationCapacity& capacity, double quantity )
{
    assert( rValue_ <= rCapacity_ );
    if( bInfiniteDotations_ && MT_IsZero( rCapacity_ ) )
    {
        rCapacity_ = maxCapacity_;
        SetValue( maxCapacity_ );
    }
    else
    {
        rCapacity_ = std::min( rCapacity_ + capacity.GetCapacity(), maxCapacity_ );
        SetValue( std::min( rValue_ + std::min( capacity.GetCapacity(), quantity ), maxCapacity_ ) );
    }
    if( quantity )
        pGroup_->NotifyDotationChanged( *this );
    rSupplyThreshold_ = std::min( rCapacity_, rSupplyThreshold_ + capacity.GetSupplyThreshold() );
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::RemoveCapacity
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
double PHY_Dotation::RemoveCapacity( const PHY_DotationCapacity& capacity )
{
    double ratio = rCapacity_ ? (rValue_ / rCapacity_) : 1.;
    double capacityToRemove = capacity.GetCapacity();
    double quantityToRemove = capacityToRemove * ratio;
    double removed = rCapacity_ > quantityToRemove ? quantityToRemove : rCapacity_;
    assert( rCapacity_ >= capacityToRemove );
    rCapacity_        -= capacityToRemove;
    rSupplyThreshold_ = std::max( std::min( rCapacity_, rSupplyThreshold_ - capacity.GetSupplyThreshold() ), 0. );
    if( rFireReservation_ > rCapacity_ )
    {
        rFireReservation_ = rCapacity_;
        removed = rValue_;
        SetValue( 0. );
    }
    else if( rConsumptionReservation_ + rFireReservation_ > rCapacity_ )
    {
        rFireReservation_ = 0;
        rConsumptionReservation_ = rCapacity_;
        removed = rValue_;
        SetValue( 0. );
    }
    else
    {
        SetValue( rValue_ - quantityToRemove );
        removed = quantityToRemove;
        if( rFireReservation_ + rConsumptionReservation_ + rValue_ > rCapacity_ )
        {
            double newValue = rCapacity_ - rConsumptionReservation_ - rFireReservation_;
            removed += ( rValue_ - newValue );
            SetValue( newValue );
        }
    }
    if( removed )
        pGroup_->NotifyDotationChanged( *this );
    return removed;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::NotifyCaptured
// Created: NLD 2005-03-07
// -----------------------------------------------------------------------------
void PHY_Dotation::NotifyCaptured()
{
    rValue_                  = 0;
    rConsumptionReservation_ = 0;
    rFireReservation_        = 0;
    bDotationBlocked_        = true;
    assert( pGroup_ );
    pGroup_->NotifyDotationChanged( *this );
    rLastValueSent_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::NotifyReleased
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
void PHY_Dotation::NotifyReleased()
{
    bDotationBlocked_ = false;
    Resupply();
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::IsJammed
// Created: MMC 2013-04-24
// -----------------------------------------------------------------------------
bool PHY_Dotation::IsJammed() const
{
    assert( pGroup_ );
    const PHY_DotationGroupContainer* pContainer = pGroup_->GetDotationGroupContainer();
    if( !pContainer )
        return false;
    const dotation::PHY_RoleInterface_Dotations* pRoleDotations = pContainer->GetRoleDotations();
    if( !pRoleDotations )
        return false;
    const MIL_AgentPion* pPion = pRoleDotations->GetPion();
    if( !pPion )
        return false;
    return ( pPion->IsJammed() || pPion->IsLogisticJammed() );
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::AddConsumptionReservation
// Created: NLD 2004-09-30
// -----------------------------------------------------------------------------
double PHY_Dotation::AddConsumptionReservation( double rNbr )
{
    rNbr = Consume( rNbr );
    rConsumptionReservation_ += rNbr;
    return rNbr;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::AddFireReservation
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
double PHY_Dotation::AddFireReservation( double rNbr )
{
    rNbr = Consume( rNbr );
    rFireReservation_ += rNbr;
    return rNbr;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::SetValueUsingTC2
// Created: JSR 2013-10-21
// -----------------------------------------------------------------------------
void PHY_Dotation::SetValueUsingTC2( double newValue, MIL_AutomateLOG& tc2 )
{
    double rValueDiff = newValue - rValue_;
    if( MT_IsZero( rValueDiff ) )
        return;
    assert( pCategory_ );
    if( rValueDiff > 0 )
        rValueDiff = tc2.SupplyGetStock( *pCategory_, rValueDiff ).quantity_;
    else
        tc2.SupplyReturnStock( *pCategory_, -rValueDiff );
    SetValue( rValue_ + rValueDiff );
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::GetCapacity
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
double PHY_Dotation::GetCapacity() const
{
    return rCapacity_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::GetValue
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
double PHY_Dotation::GetValue() const
{
    return rValue_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::Consume
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
double PHY_Dotation::Consume( double rValue )
{
    rValue = std::min( rValue, rValue_ );
    SetValue( rValue_ - rValue );
    return rValue;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::GetCategory
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
const PHY_DotationCategory& PHY_Dotation::GetCategory() const
{
    assert( pCategory_ );
    return *pCategory_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::Resupply
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void PHY_Dotation::Resupply( double rFactor /* = 1. */, bool withLog /* = false */ )
{
    SetValue( withLog? std::max( rCapacity_ * rFactor - rRequestedValue_, rValue_ ) : rCapacity_ * rFactor );
    rConsumptionReservation_ = 0.;
    rFireReservation_        = 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::ChangeDotation
// Created: ABR 2011-08-10
// -----------------------------------------------------------------------------
void PHY_Dotation::ChangeDotation( unsigned int number, float thresholdPercentage )
{
    assert( number <= rCapacity_ );
    SetValue( number );
    rConsumptionReservation_ = 0.;
    rFireReservation_        = 0.;
    rSupplyThreshold_ = std::min( rCapacity_ * thresholdPercentage / 100.f, rCapacity_ );
    assert( pGroup_ );
    pGroup_->NotifyDotationChanged( *this ); // What's the point, c'est déja fait dans SetValue(), bordel
    rLastValueSent_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::CancelConsumptionReservation
// Created: NLD 2004-09-30
// -----------------------------------------------------------------------------
void PHY_Dotation::CancelConsumptionReservation()
{
    SetValue( rValue_ + rConsumptionReservation_ );
    rConsumptionReservation_ = 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::ConsumeConsumptionReservation
// Created: NLD 2004-09-30
// -----------------------------------------------------------------------------
void PHY_Dotation::ConsumeConsumptionReservation()
{
    rConsumptionReservation_ = 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::UpdateSupplyNeeded
// Created: MMC 2013-04-24
// -----------------------------------------------------------------------------
void PHY_Dotation::UpdateSupplyNeeded()
{
    if( HasReachedSupplyThreshold() )
        NotifySupplyNeeded();
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::ConsumeFireReservation
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void PHY_Dotation::ConsumeFireReservation()
{
    rFireReservation_ = 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::NeedSupply
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
bool PHY_Dotation::NeedSupply() const
{
    if( bDotationBlocked_ )
        return false;
    return rCapacity_ > rValue_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::HasReachedSupplyThreshold
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
bool PHY_Dotation::HasReachedSupplyThreshold() const
{
    if( bDotationBlocked_ )
        return false;
    if( bInfiniteDotations_ )
        return false;
    if( IsJammed() )
        return false;
    return rValue_ < rSupplyThreshold_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::Supply
// Created: NLD 2005-01-28
// -----------------------------------------------------------------------------
double PHY_Dotation::Supply( double rSupply )
{
    if( bDotationBlocked_ )
        return 0.;
    const double rNewSupply = std::min( rSupply, rCapacity_ - rConsumptionReservation_ - rFireReservation_ - rValue_ );
    SetValue( rValue_ + rNewSupply );
    return rNewSupply;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::GetSupplyThresholdPercentage
// Created: ABR 2011-07-27
// -----------------------------------------------------------------------------
double PHY_Dotation::GetSupplyThresholdPercentage() const
{
    assert( rSupplyThreshold_ <= rCapacity_ );
    if( rCapacity_ == 0 )
        return 0;
    return rSupplyThreshold_ / rCapacity_ * 100.f;
}

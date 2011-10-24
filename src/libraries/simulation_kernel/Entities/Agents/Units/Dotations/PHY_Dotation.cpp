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
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_Dotation )

const double PHY_Dotation::maxCapacity_ = 10000000;

// -----------------------------------------------------------------------------
// Name: PHY_Dotation constructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_Dotation::PHY_Dotation( const PHY_DotationCategory& category, PHY_DotationGroup& group, bool bInfiniteDotations )
    : pCategory_              ( &category )
    , pGroup_                 ( &group )
    , rValue_                 ( 0. )
    , rCapacity_              ( 0. )
    , rConsumptionReservation_( 0. )
    , rFireReservation_       ( 0. )
    , rSupplyThreshold_       ( 0. )
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
    , rValue_                 ( 0. )
    , rCapacity_              ( 0. )
    , rConsumptionReservation_( 0. )
    , rFireReservation_       ( 0. )
    , rSupplyThreshold_       ( 0. )
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
// Name: PHY_Dotation::load
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
void PHY_Dotation::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    unsigned int nID;
    file >> nID;
    pCategory_ = PHY_DotationType::FindDotationCategory( nID );
    file >> pGroup_
         >> rValue_
         >> rCapacity_
         >> rConsumptionReservation_
         >> rFireReservation_
         >> rSupplyThreshold_
         >> bDotationBlocked_
         >> bInfiniteDotations_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::save
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
void PHY_Dotation::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    unsigned category = pCategory_->GetMosID();
    file << category
         << pGroup_
         << rValue_
         << rCapacity_
         << rConsumptionReservation_
         << rFireReservation_
         << rSupplyThreshold_
         << bDotationBlocked_
         << bInfiniteDotations_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::ReadValue
// Created: NLD 2005-01-21
// -----------------------------------------------------------------------------
void PHY_Dotation::ReadValue( xml::xistream& xis )
{
    double rValue;
    xis >> xml::attribute( "quantity", rValue );
    if( rValue < 0. )
        xis.error( "rValue is not greater or equal to 0." );
    if( rValue > rCapacity_ )
        rCapacity_ = rValue;
    SetValue( rValue );
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::SetValue
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
void PHY_Dotation::SetValue( double rValue )
{
    if( bDotationBlocked_ )
        return;
    if( bInfiniteDotations_ && rValue < rSupplyThreshold_ )
        rValue = rCapacity_;
    rValue = std::min( rValue, maxCapacity_ );
    if( (unsigned int)rValue_ != (unsigned int)rValue )
    {
        assert( pGroup_ );
        pGroup_->NotifyDotationChanged( *this );
    }
    const bool bSupplyThresholdAlreadyReached = HasReachedSupplyThreshold();
    rValue_ = rValue;
    if( HasReachedSupplyThreshold() )
    {
        assert( pGroup_ );
        assert( pCategory_ );
        pGroup_->NotifySupplyNeeded( *pCategory_, !bSupplyThresholdAlreadyReached );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::AddCapacity
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
void PHY_Dotation::AddCapacity( const PHY_DotationCapacity& capacity )
{
    assert( rValue_ <= rCapacity_ );
    if( bInfiniteDotations_ && MT_IsZero( rCapacity_ ) )
    {
        rCapacity_ = maxCapacity_;
        if( !bDotationBlocked_ )
            rValue_ = maxCapacity_;
    }
    else
    {
        rCapacity_ += capacity.GetCapacity();
        if( !bDotationBlocked_ )
            rValue_ += capacity.GetCapacity();
    }
    rCapacity_ = std::min( rCapacity_, maxCapacity_ );
    rValue_ = std::min( rValue_, maxCapacity_ );
    rSupplyThreshold_ = std::min( rCapacity_, rSupplyThreshold_ + capacity.GetSupplyThreshold() );
    assert( pGroup_ );
    pGroup_->NotifyDotationChanged( *this );
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::RemoveCapacity
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
void PHY_Dotation::RemoveCapacity( const PHY_DotationCapacity& capacity )
{
    assert( rCapacity_ >= capacity.GetCapacity() );
    rCapacity_        -= capacity.GetCapacity();
    rSupplyThreshold_ = std::max( std::min( rCapacity_, rSupplyThreshold_ - capacity.GetSupplyThreshold() ), 0. );
    if( rFireReservation_ > rCapacity_ )
    {
        rFireReservation_ = rCapacity_;
        rValue_           = 0;

        assert( pGroup_ );
        pGroup_->NotifyDotationChanged( *this );
    }
    else if( rConsumptionReservation_ + rFireReservation_ > rCapacity_ )
    {
        rConsumptionReservation_ = rCapacity_;
        rValue_                  = 0;

        assert( pGroup_ );
        pGroup_->NotifyDotationChanged( *this );
    }
    else if( rFireReservation_ + rConsumptionReservation_ + rValue_ > rCapacity_ )
    {
        rValue_ = rCapacity_ - rConsumptionReservation_;

        assert( pGroup_ );
        pGroup_->NotifyDotationChanged( *this );
    }
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
// Name: PHY_Dotation::ChangeValueUsingTC2
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
void PHY_Dotation::ChangeValueUsingTC2( double rCapacityFactor, MIL_AutomateLOG& tc2 )
{
    //$$$ checker rFireReservation_ et rConsumptionReservation_
    double rValueDiff = ( rCapacity_ * rCapacityFactor ) - rValue_;
    if( MT_IsZero( rValueDiff ) )
        return;
    assert( pCategory_ );
    if( rValueDiff > 0 )
        rValueDiff = tc2.SupplyGetStock( *pCategory_, rValueDiff );
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
// Name: PHY_Dotation::Reset
// Created: ABR 2011-03-08
// -----------------------------------------------------------------------------
void PHY_Dotation::Reset()
{
    rValue_                  = 0;
    rConsumptionReservation_ = 0;
    rFireReservation_        = 0;
    bDotationBlocked_        = false;
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
void PHY_Dotation::Resupply( double rFactor /* = 1. */ )
{
    SetValue( rCapacity_ * rFactor );
    rConsumptionReservation_ = 0.;
    rFireReservation_        = 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::ChangeDotation
// Created: ABR 2011-08-10
// -----------------------------------------------------------------------------
void PHY_Dotation::ChangeDotation( unsigned int number, float thresholdPercentage )
{
    assert( number < rCapacity_ );
    SetValue( number );
    rConsumptionReservation_ = 0.;
    rFireReservation_        = 0.;
    rSupplyThreshold_ = std::min( rCapacity_ * thresholdPercentage / 100.f, rCapacity_ );
    assert( pGroup_ );
    pGroup_->NotifyDotationChanged( *this );
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
    return rCapacity_ - rValue_ > 0;
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

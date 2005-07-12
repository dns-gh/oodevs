// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Dotations/PHY_Dotation.cpp $
// $Author: Nld $
// $Modtime: 13/05/05 16:08 $
// $Revision: 10 $
// $Workfile: PHY_Dotation.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_Dotation.h"

#include "PHY_DotationGroup.h"
#include "PHY_DotationCapacity.h"
#include "PHY_DotationType.h"
#include "PHY_DotationCategory.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"

// -----------------------------------------------------------------------------
// Name: PHY_Dotation constructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_Dotation::PHY_Dotation( const PHY_DotationCategory& category, PHY_DotationGroup& group )
    : pCategory_              ( &category )
    , pGroup_                 ( &group )
    , rValue_                 ( 0. )
    , rCapacity_              ( 0. )
    , rConsumptionReservation_( 0. )
    , rFireReservation_       ( 0. )
    , rSupplyThreshold_       ( 0. )
    , bDotationBlocked_       ( false )
{
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
{
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation destructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_Dotation::~PHY_Dotation()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_Dotation::load
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
void PHY_Dotation::load( MIL_CheckPointInArchive& file, const uint )
{
    uint nID;
        
    file >> nID;
    pCategory_ = PHY_DotationType::FindDotationCategory( nID );
    
    file >> pGroup_
         >> rValue_
         >> rCapacity_
         >> rConsumptionReservation_
         >> rFireReservation_
         >> rSupplyThreshold_
         >> bDotationBlocked_;    
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::save
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
void PHY_Dotation::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << pCategory_->GetMosID()
         << pGroup_
         << rValue_
         << rCapacity_
         << rConsumptionReservation_
         << rFireReservation_
         << rSupplyThreshold_
         << bDotationBlocked_;
}

// =============================================================================
// OPERATIONS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_Dotation::ReadValue
// Created: NLD 2005-01-21
// -----------------------------------------------------------------------------
void PHY_Dotation::ReadValue( MIL_InputArchive& archive )
{
    MT_Float rValue;
    archive.Read( rValue );

    if( rValue > rCapacity_ )
    {
        rValue = rCapacity_;
        MT_LOG_WARNING_MSG( "PHY_DotationGroup::ReadValues XXX truncation" );
    }
    SetValue( rValue );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::SetValue
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
void PHY_Dotation::SetValue( MT_Float rValue )
{
    if( bDotationBlocked_ )
        return;

    //assert( rValue <= rCapacity_ ); $$$ precision de merde
    if ( (uint)rValue_ != (uint)rValue )
    {
        assert( pGroup_ );
        pGroup_->NotifyDotationChanged( *this );
    }
    
    rValue_ = rValue;

    if ( HasReachedSupplyThreshold() )
    {
        assert( pGroup_ );
        assert( pCategory_ );
        pGroup_->NotifySupplyNeeded( *pCategory_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::AddCapacity
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
void PHY_Dotation::AddCapacity( const PHY_DotationCapacity& capacity )
{
    assert( rValue_ <= rCapacity_ );
    
    rCapacity_        += capacity.GetCapacity();
    rSupplyThreshold_ += capacity.GetSupplyThreshold();
    
    if ( !bDotationBlocked_ )
        rValue_ += capacity.GetCapacity();
        
    assert( pGroup_ );
    pGroup_->NotifyDotationChanged( *this );
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::RemoveCapacity
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
void PHY_Dotation::RemoveCapacity( const PHY_DotationCapacity& capacity )
{
    assert(        rCapacity_        >=        capacity.GetCapacity() );
    //assert( (float)rSupplyThreshold_ >= (float)capacity.GetSupplyThreshold() ); // pb débile de précision
    rCapacity_        -= capacity.GetCapacity();
    rSupplyThreshold_ -= capacity.GetSupplyThreshold();

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
// Name: PHY_Dotation::NotifyTakenPrisoner
// Created: NLD 2005-03-07
// -----------------------------------------------------------------------------
void PHY_Dotation::NotifyTakenPrisoner()
{
    rValue_                  = 0;
    rConsumptionReservation_ = 0;
    rFireReservation_        = 0;
    bDotationBlocked_        = true;
    
    assert( pGroup_ );
    pGroup_->NotifyDotationChanged( *this );
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::AddConsumptionReservation
// Created: NLD 2004-09-30
// -----------------------------------------------------------------------------
MT_Float PHY_Dotation::AddConsumptionReservation( MT_Float rNbr )
{
    rNbr = Consume( rNbr );
    rConsumptionReservation_ += rNbr;    
    return rNbr;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::AddFireReservation
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
MT_Float PHY_Dotation::AddFireReservation( MT_Float rNbr )
{
    rNbr = Consume( rNbr );
    rFireReservation_ += rNbr;    
    return rNbr;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::ChangeValueUsingTC2
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
void PHY_Dotation::ChangeValueUsingTC2( MT_Float rCapacityFactor, MIL_AutomateLOG& tc2 )
{
    //$$$ checker rFireReservation_ et rConsumptionReservation_    
    MT_Float rValueDiff = ( rCapacity_ * rCapacityFactor ) - rValue_;
    if ( rValueDiff == 0. )
        return;

    assert( pCategory_ );
    if ( rValueDiff > 0 )
        rValueDiff = tc2.SupplyGetStock( *pCategory_, rValueDiff );
    else
        tc2.SupplyReturnStock( *pCategory_, -rValueDiff );
    SetValue( rValue_ + rValueDiff );
}

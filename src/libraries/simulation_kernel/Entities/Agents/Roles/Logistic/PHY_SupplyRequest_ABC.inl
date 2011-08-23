// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Roles/Logistic/PHY_SupplyRequest_ABC.inl $
// $Author: Nld $
// $Modtime: 4/02/05 15:34 $
// $Revision: 1 $
// $Workfile: PHY_SupplyRequest_ABC.inl $
//
// *****************************************************************************

#include "Entities/Agents/MIL_AgentPion.h"

// -----------------------------------------------------------------------------
// Name: PHY_SupplyRequest_ABC constructor
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
template< typename T > 
PHY_SupplyRequest_ABC< T >::PHY_SupplyRequest_ABC()
    : requests_            ()
    , rTotalRequestedValue_( 0. )
    , pSupplier_           ( 0 )
    , pStockPion_          ( 0 )
    , rTotalReservedValue_ ( 0. )
    , rTotalConvoyedValue_ ( 0. )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyRequest_ABC destructor
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
template< typename T > 
PHY_SupplyRequest_ABC< T >::~PHY_SupplyRequest_ABC()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_SupplyRequest_ABC< T >::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename T > 
template< typename Archive >
void PHY_SupplyRequest_ABC< T >::sIndividualRequest::serialize( Archive& file, const unsigned int )
{
    file & pRequest_
         & rRequestedValue_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyRequest_ABC< T >::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename T > 
template< typename Archive >
void PHY_SupplyRequest_ABC< T >::serialize( Archive& file, const unsigned int )
{
    file & requests_
         & rTotalRequestedValue_
         & pSupplier_
         & pStockPion_
         & rTotalReservedValue_ 
         & rTotalConvoyedValue_;
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_SupplyRequest_ABC< T >::GetDotationCategory
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
template< typename T > 
const PHY_DotationCategory& PHY_SupplyRequest_ABC< T >::GetDotationCategory() const
{
    assert( !requests_.empty() );
    assert( requests_.front().pRequest_ );
    return requests_.front().pRequest_->GetCategory();
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyRequest_ABC< T >::Supply
// Created: NLD 2005-01-28
// -----------------------------------------------------------------------------
template< typename T > 
double PHY_SupplyRequest_ABC< T >::Supply() const
{
          double rRemainingValue = rTotalConvoyedValue_;
    const double rTmp            = rTotalConvoyedValue_ / rTotalRequestedValue_;
    for( CIT_RequestVector itRequest = requests_.begin(); itRequest != requests_.end(); ++itRequest )
    {
        const sIndividualRequest& request = *itRequest;

        const double rValue = request.rRequestedValue_ * rTmp;
        rRemainingValue -= request.pRequest_->Supply( rValue );
    }
    return rRemainingValue;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyRequest_ABC< T >::Cancel
// Created: NLD 2005-02-11
// -----------------------------------------------------------------------------
template< typename T > 
void PHY_SupplyRequest_ABC< T >::Cancel()
{
    assert( pStockPion_ );
    pStockPion_->GetRole< PHY_RoleInterface_Supply >().RemoveStockReservation( GetDotationCategory(), rTotalConvoyedValue_ );
    rTotalConvoyedValue_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyRequest_ABC< T >::AffectSupplier
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
template< typename T > 
bool PHY_SupplyRequest_ABC< T >::AffectSupplier( MIL_AutomateLOG& supplier )
{
    if( pSupplier_ )
    {
        assert( pStockPion_ );
        return true;
    }

    MIL_AgentPion* pStockPion = supplier.SupplyGetStockPion( GetDotationCategory(), rTotalRequestedValue_ );
    if( !pStockPion )
        return false;

    pSupplier_   = &supplier;
    pStockPion_  = pStockPion;
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyRequest_ABC< T >::HasReachedSupplyThreshold
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
template< typename T > 
bool PHY_SupplyRequest_ABC< T >::HasReachedSupplyThreshold() const
{
    for( CIT_RequestVector itRequest = requests_.begin(); itRequest != requests_.end(); ++itRequest )
    {
        if( itRequest->pRequest_->HasReachedSupplyThreshold() )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyRequest_ABC< T >::RemoveConvoyedMerchandise
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
template< typename T > 
void PHY_SupplyRequest_ABC< T >::RemoveConvoyedMerchandise( double rNbr )
{
    const double rTmp = std::min( rTotalConvoyedValue_, rNbr );
    rTotalConvoyedValue_ -= rTmp;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyRequest_ABC::AddConvoyedMerchandise
// Created: NLD 2005-12-15
// -----------------------------------------------------------------------------
template< typename T > 
void PHY_SupplyRequest_ABC< T >::AddConvoyedMerchandise( double rNbr )
{
    rTotalConvoyedValue_ += rNbr;
    assert( rTotalConvoyedValue_ <= rTotalReservedValue_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyRequest_ABC< T >::ReserveStocks
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
template< typename T > 
void PHY_SupplyRequest_ABC< T >::ReserveStocks()
{
    assert( pSupplier_ );
    assert( pStockPion_ );
    assert( rTotalReservedValue_ == 0. );
    rTotalReservedValue_ = pStockPion_->GetRole< PHY_RoleInterface_Supply >().AddStockReservation( GetDotationCategory(), rTotalRequestedValue_ );
    rTotalConvoyedValue_ = 0.;
    assert( rTotalReservedValue_ > 0. );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyRequest_ABC::CancelMerchandiseOverheadReservation
// Created: NLD 2005-12-15
// -----------------------------------------------------------------------------
template< typename T > 
void PHY_SupplyRequest_ABC< T >::CancelMerchandiseOverheadReservation() 
{
    assert( pStockPion_ );
    double rNbr = rTotalReservedValue_ - rTotalConvoyedValue_;
    assert( rNbr >= 0. );
    if( rNbr > 0. )
        pStockPion_->GetRole< PHY_RoleInterface_Supply >().RemoveStockReservation( GetDotationCategory(), rNbr );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_SupplyRequest_ABC< T >::Serialize
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
template< typename T > 
void PHY_SupplyRequest_ABC< T >::Serialize( sword::DotationQuery& asn ) const
{
    asn.mutable_resource()->set_id( GetDotationCategory().GetMosID() );
    asn.set_requested  ( (unsigned int)rTotalRequestedValue_ );
    asn.set_granted  ( (unsigned int)rTotalReservedValue_ );
    asn.set_convoyed( (unsigned int)rTotalConvoyedValue_ );
}
       
// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_SupplyRequest_ABC< T >::GetSupplier
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
template< typename T > 
MIL_AutomateLOG* PHY_SupplyRequest_ABC< T >::GetSupplier() const
{
    return pSupplier_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyRequest_ABC< T >::GetTotalConvoyedValue
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
template< typename T >
double PHY_SupplyRequest_ABC< T >::GetTotalConvoyedValue() const
{
    return rTotalConvoyedValue_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyRequest_ABC< T >::GetTotalRequestedValue
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
template< typename T > 
double PHY_SupplyRequest_ABC< T >::GetTotalRequestedValue() const
{
    return rTotalRequestedValue_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyRequest_ABC< T >::GetTotalReservedValue
// Created: NLD 2005-02-04
// -----------------------------------------------------------------------------
template< typename T > 
double PHY_SupplyRequest_ABC< T >::GetTotalReservedValue() const
{
    return rTotalReservedValue_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyRequest_ABC< T >::GetStockPion
// Created: MGD 2009-02-11
// -----------------------------------------------------------------------------
template< typename T >
MIL_Automate& PHY_SupplyRequest_ABC< T >::GetStockPion() const
{
  return pStockPion_->GetAutomate();
}

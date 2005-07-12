// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyStockRequest.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 13:44 $
// $Revision: 4 $
// $Workfile: PHY_SupplyStockRequest.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_SupplyStockRequest.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationStock.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_RolePion_Supply.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"

BOOST_CLASS_EXPORT_GUID( PHY_SupplyStockRequest, "PHY_SupplyStockRequest" )

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequest constructor
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
PHY_SupplyStockRequest::PHY_SupplyStockRequest()
    : requests_            ()
    , rTotalRequestedValue_( 0. )
    , pSupplyingAutomate_  ( 0 )
    , pStockPion_          ( 0 )
    , rTotalReservedValue_ ( 0. )
    , rTotalConvoyedValue_ ( 0. )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequest destructor
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
PHY_SupplyStockRequest::~PHY_SupplyStockRequest()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequest::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_SupplyStockRequest::sStockRequest::serialize( Archive& file, const uint )
{
    file & pStock_
         & rRequestedValue_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequest::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_SupplyStockRequest::serialize( Archive& file, const uint )
{
    file & requests_
         & rTotalRequestedValue_
         & pSupplyingAutomate_
         & pStockPion_
         & rTotalReservedValue_ 
         & rTotalConvoyedValue_;
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequest::GetDotationCategory
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
const PHY_DotationCategory& PHY_SupplyStockRequest::GetDotationCategory() const
{
    assert( !requests_.empty() );
    assert( requests_.front().pStock_ );
    return requests_.front().pStock_->GetCategory();
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequest::AddStock
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void PHY_SupplyStockRequest::AddStock( PHY_DotationStock& stock )
{
    sStockRequest request;
    request.pStock_          = &stock;
    request.rRequestedValue_ = stock.GetCapacity() - stock.GetValue();
    if( request.rRequestedValue_ > 0 )
    {
        rTotalRequestedValue_ += request.rRequestedValue_;   
        requests_.push_back( request );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequest::AddStockPushedFlow
// Created: NLD 2005-02-04
// -----------------------------------------------------------------------------
void PHY_SupplyStockRequest::AddStockPushedFlow( PHY_DotationStock& stock )
{
    sStockRequest request;
    request.pStock_          = &stock;
    request.rRequestedValue_ = 0;
    requests_.push_back( request );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequest::SetStockPushedFlowValue
// Created: NLD 2005-02-04
// -----------------------------------------------------------------------------
void PHY_SupplyStockRequest::SetStockPushedFlowValue( MT_Float rPushedFlowValue )
{
    assert( !requests_.empty() );
    rTotalRequestedValue_ = rPushedFlowValue;
    const MT_Float rIndividualValue = rPushedFlowValue / requests_.size();
    for( IT_RequestVector itRequest = requests_.begin(); itRequest != requests_.end(); ++itRequest )
        itRequest->rRequestedValue_ = rIndividualValue;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequest::Supply
// Created: NLD 2005-01-28
// -----------------------------------------------------------------------------
MT_Float PHY_SupplyStockRequest::Supply() const
{
          MT_Float rRemainingValue = rTotalConvoyedValue_;
    const MT_Float rTmp            = rTotalConvoyedValue_ / rTotalRequestedValue_;
    for( CIT_RequestVector itRequest = requests_.begin(); itRequest != requests_.end(); ++itRequest )
    {
        const sStockRequest& request = *itRequest;

        const MT_Float rValue = request.rRequestedValue_ * rTmp;
        rRemainingValue -= request.pStock_->Supply( rValue );
    }        
    return rRemainingValue;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequest::Cancel
// Created: NLD 2005-02-11
// -----------------------------------------------------------------------------
void PHY_SupplyStockRequest::Cancel()
{
    assert( pStockPion_ );
    pStockPion_->RemoveStockReservation( GetDotationCategory(), rTotalConvoyedValue_ );
    rTotalConvoyedValue_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequest::ApplyQuota
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
void PHY_SupplyStockRequest::ApplyQuota( MT_Float rQuota )
{
    const MT_Float rNewTotalRequestedValue_ = std::min( rTotalRequestedValue_, rQuota );
    const MT_Float rTmp                     = rNewTotalRequestedValue_ / rTotalRequestedValue_;

    for( IT_RequestVector itRequest = requests_.begin(); itRequest != requests_.end(); ++itRequest )
    {
        sStockRequest& request = *itRequest;
        request.rRequestedValue_ *= rTmp;
    }        
    rTotalRequestedValue_ = rNewTotalRequestedValue_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequest::AffectAutomate
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
bool PHY_SupplyStockRequest::AffectAutomate( MIL_AutomateLOG& supplyingAutomate )
{
    if( pSupplyingAutomate_ )
    {
        assert( pStockPion_ );
        return true;
    }

    PHY_RolePion_Supply* pStockPion = supplyingAutomate.SupplyGetStockPion( GetDotationCategory(), rTotalRequestedValue_ );
    if( !pStockPion )
        return false;

    pSupplyingAutomate_ = &supplyingAutomate;
    pStockPion_         = pStockPion;
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequest::HasReachedSupplyThreshold
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
bool PHY_SupplyStockRequest::HasReachedSupplyThreshold() const
{
    for( CIT_RequestVector itRequest = requests_.begin(); itRequest != requests_.end(); ++itRequest )
    {
        if( itRequest->pStock_->HasReachedSupplyThreshold() )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequest::ReserveStocks
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
void PHY_SupplyStockRequest::ReserveStocks()
{
    assert( pSupplyingAutomate_ );
    assert( pStockPion_ );
    assert( rTotalReservedValue_ == 0. );
    rTotalReservedValue_ = pStockPion_->AddStockReservation( GetDotationCategory(), rTotalRequestedValue_ );
    rTotalConvoyedValue_ = rTotalReservedValue_;
    assert( rTotalReservedValue_ > 0. );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequest::GetUVolumeRequested
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
MT_Float PHY_SupplyStockRequest::GetUVolumeRequested() const
{
    return GetDotationCategory().ConvertDotationToUVolume( rTotalReservedValue_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequest::RemoveConvoyedStock
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
MT_Float PHY_SupplyStockRequest::RemoveConvoyedStock( MT_Float rUVolumeToRemove )
{
    const MT_Float rTmp = std::min( rTotalConvoyedValue_, GetDotationCategory().ConvertUVolumeToDotation( rUVolumeToRemove ) );
    rTotalConvoyedValue_ -= rTmp;
    return GetDotationCategory().ConvertDotationToUVolume( rTmp );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequest::Serialize
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
void PHY_SupplyStockRequest::Serialize( ASN1T_DemandeDotation& asn ) const
{
    asn.ressource_id        = GetDotationCategory().GetMosID();
    asn.quantite_demandee   = (uint)rTotalRequestedValue_;
    asn.quantite_accordee   = (uint)rTotalReservedValue_;
    asn.quantite_en_transit = (uint)rTotalConvoyedValue_;
}
        
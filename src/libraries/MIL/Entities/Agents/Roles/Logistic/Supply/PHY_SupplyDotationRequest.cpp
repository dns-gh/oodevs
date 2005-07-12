// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyDotationRequest.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 13:39 $
// $Revision: 4 $
// $Workfile: PHY_SupplyDotationRequest.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_SupplyDotationRequest.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_RolePion_Supply.h"
#include "Entities/Agents/Units/Dotations/PHY_Dotation.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"

BOOST_CLASS_EXPORT_GUID( PHY_SupplyDotationRequest, "PHY_SupplyDotationRequest" )

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationRequest constructor
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
PHY_SupplyDotationRequest::PHY_SupplyDotationRequest()
    : requests_            ()
    , rTotalRequestedValue_( 0. )
    , pSupplyingAutomate_  ( 0 )
    , pStockPion_          ( 0 )
    , rTotalReservedValue_ ( 0. )
{

}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationRequest destructor
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
PHY_SupplyDotationRequest::~PHY_SupplyDotationRequest()
{

}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationRequest::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_SupplyDotationRequest::sDotationRequest::serialize( Archive& file, const uint )
{
    file & pDotation_
         & rRequestedValue_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationRequest::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_SupplyDotationRequest::serialize( Archive& file, const uint )
{
    file & requests_
         & rTotalRequestedValue_
         & pSupplyingAutomate_
         & pStockPion_
         & rTotalReservedValue_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationRequest::GetDotationCategory
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
const PHY_DotationCategory& PHY_SupplyDotationRequest::GetDotationCategory() const
{
    assert( !requests_.empty() );
    assert( requests_.front().pDotation_ );
    return requests_.front().pDotation_->GetCategory();
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationRequest::AddDotation
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void PHY_SupplyDotationRequest::AddDotation( PHY_Dotation& dotation )
{
    sDotationRequest request;
    request.pDotation_       = &dotation;
    request.rRequestedValue_ = dotation.GetCapacity() - dotation.GetValue();

    assert( request.rRequestedValue_ > 0. );
    rTotalRequestedValue_ += request.rRequestedValue_;   

    requests_.push_back( request );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationRequest::Supply
// Created: NLD 2005-01-28
// -----------------------------------------------------------------------------
MT_Float PHY_SupplyDotationRequest::Supply() const
{
          MT_Float rRemainingValue = rTotalReservedValue_;
    const MT_Float rTmp            = rTotalReservedValue_ / rTotalRequestedValue_;
    for( CIT_RequestVector itRequest = requests_.begin(); itRequest != requests_.end(); ++itRequest )
    {
        const sDotationRequest& request = *itRequest;

        const MT_Float rValue = request.rRequestedValue_ * rTmp;
        rRemainingValue -= request.pDotation_->Supply( rValue );
    }        
    return rRemainingValue;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationRequest::AffectAutomate
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
bool PHY_SupplyDotationRequest::AffectAutomate( MIL_AutomateLOG& supplyingAutomate )
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
// Name: PHY_SupplyDotationRequest::HasReachedSupplyThreshold
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
bool PHY_SupplyDotationRequest::HasReachedSupplyThreshold() const
{
    for( CIT_RequestVector itRequest = requests_.begin(); itRequest != requests_.end(); ++itRequest )
    {
        if( itRequest->pDotation_->HasReachedSupplyThreshold() )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationRequest::ReserveStocks
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
void PHY_SupplyDotationRequest::ReserveStocks()
{
    assert( pSupplyingAutomate_ );
    assert( pStockPion_ );
    assert( rTotalReservedValue_ == 0. );
    rTotalReservedValue_ = pStockPion_->AddStockReservation( GetDotationCategory(), rTotalRequestedValue_ );
    assert( rTotalReservedValue_ > 0. );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationRequest::GetUVolumeRequested
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
MT_Float PHY_SupplyDotationRequest::GetUVolumeRequested() const
{
    return GetDotationCategory().ConvertDotationToUVolume( rTotalReservedValue_ );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationRequest::Serialize
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
void PHY_SupplyDotationRequest::Serialize( ASN1T_DemandeDotation& asn ) const
{
    asn.ressource_id        = GetDotationCategory().GetMosID();
    asn.quantite_demandee   = (uint)rTotalRequestedValue_;
    asn.quantite_accordee   = (uint)rTotalReservedValue_;
    asn.quantite_en_transit = asn.quantite_accordee;
}
        
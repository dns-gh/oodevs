// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SupplyRequest.h"
#include "SupplyResource_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Specialisations/LOG/LogisticLink_ABC.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "protocol/ClientSenders.h"
#include <boost/foreach.hpp>

using namespace logistic;

// -----------------------------------------------------------------------------
// Name: SupplyRequest constructor
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
SupplyRequest::SupplyRequest( const PHY_DotationCategory& dotationCategory )
    : dotationCategory_   ( dotationCategory )
    , supplier_           ( 0 )
    , requestedQuantity_  ( 0 )
    , grantedQuantity_    ( 0 )
    , convoyedQuantity_   ( 0 )
    , suppliedQuantity_   ( 0 )
    , complementarySupply_( true )
    , supplierQuotas_     ()
{
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest destructorsupplier_
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
SupplyRequest::~SupplyRequest()
{
    //$$$ Return remaining stock automatically - BOF: magic
    if( supplier_ && convoyedQuantity_ > 0 )
        supplier_->SupplyReturnStock( dotationCategory_, convoyedQuantity_ );

    if( supplierQuotas_ && requestedQuantity_ > suppliedQuantity_ )
        supplierQuotas_->ReturnQuota( dotationCategory_, requestedQuantity_ - suppliedQuantity_ );
}

// =============================================================================
// Operations
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyRequest::AddResource
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void SupplyRequest::AddResource( boost::shared_ptr< SupplyResource_ABC > resource, double quantity )
{
    assert( quantity > 0 );
    if( quantity <= 0 )
        return;
    resourceRequests_.push_back( std::make_pair( resource, quantity ) );
    requestedQuantity_ += quantity;
    if( resource->HasReachedSupplyThreshold() )
        complementarySupply_ = false;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::AffectSupplier
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
bool SupplyRequest::AffectSupplier( SupplySupplier_ABC& supplier )
{
    if( supplier_ )
        return true;
    if( supplier.SupplyHasStock( dotationCategory_ ) )
    {
        supplier_ = &supplier;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::AffectSupplier
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
bool SupplyRequest::AffectSupplier( boost::shared_ptr< LogisticLink_ABC > supplier )
{
    if( !AffectSupplier( supplier->GetSuperior() ) )
        return false;

    double tmp = supplier->ConsumeQuota( dotationCategory_, requestedQuantity_ );
    if( tmp <= 0 )
        return false;
    supplierQuotas_ = supplier;
    requestedQuantity_ = tmp;
    return true;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::Supply
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
double SupplyRequest::Supply()
{
          double suppliedQuantity = 0;
    const double tmp              = convoyedQuantity_ / requestedQuantity_;
    BOOST_FOREACH( T_ResourceRequests::value_type& data, resourceRequests_ )
    {
        const double value = data.second * tmp;
        suppliedQuantity += data.first->Supply( value );
    }
    convoyedQuantity_ -= suppliedQuantity;
    suppliedQuantity_ += suppliedQuantity;
    return suppliedQuantity;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::Convoy
// Return the quantity convoyed
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
double SupplyRequest::Convoy( double quantity )
{
    double tmp = std::min( quantity, grantedQuantity_ - convoyedQuantity_ );
    convoyedQuantity_ += tmp;
    return tmp;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::Convoy
// Return the quantity removed
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
double SupplyRequest::RemoveFromConvoy( double quantity )
{
    double tmp = std::min( quantity, convoyedQuantity_ );
    convoyedQuantity_ -= tmp;
    return tmp;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::ReserveStock
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void SupplyRequest::ReserveStock()
{
    if( !supplier_ || grantedQuantity_ > 0 )
        return;

    grantedQuantity_  = supplier_->SupplyGetStock( dotationCategory_, requestedQuantity_ );
    convoyedQuantity_ = 0;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::ReturnStockNotAssignedToConvoy
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void SupplyRequest::ReturnStockNotAssignedToConvoy()
{
    double tmp = grantedQuantity_ - convoyedQuantity_;
    if( tmp > 0 )
        supplier_->SupplyReturnStock( dotationCategory_, tmp );
}

// =============================================================================
// Accessors
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyRequest::GetSupplier
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
SupplySupplier_ABC* SupplyRequest::GetSupplier() const
{
    return supplier_;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::GetDotationCategory
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
const PHY_DotationCategory& SupplyRequest::GetDotationCategory() const
{
    return dotationCategory_;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::GetGrantedValue
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
double SupplyRequest::GetGrantedValue() const
{
    return grantedQuantity_;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::IsComplementary
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
bool SupplyRequest::IsComplementary() const
{
    return complementarySupply_;
}

// =============================================================================
// Network
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyRequest::Serialize
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void SupplyRequest::Serialize( sword::SupplyResourceRequest& msg ) const
{
    msg.mutable_resource()->set_id( dotationCategory_.GetMosID() );
    msg.set_requested( (unsigned)requestedQuantity_ );
    msg.set_granted  ( (unsigned)grantedQuantity_ );
    msg.set_convoyed ( (unsigned)convoyedQuantity_ );
}
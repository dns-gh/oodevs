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
#include "ConsignHelper.h"
#include "SupplyResource_ABC.h"
#include "SupplyRecipient_ABC.h"
#include "Checkpoints/SerializationTools.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Specialisations/LOG/LogisticLink_ABC.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <boost/foreach.hpp>

using namespace logistic;

BOOST_CLASS_EXPORT_IMPLEMENT( logistic::SupplyRequest )

// -----------------------------------------------------------------------------
// Name: SupplyRequest constructor
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
SupplyRequest::SupplyRequest( const PHY_DotationCategory& dotationCategory, unsigned int recipientId )
    : id_                 ( NewConsignId() )
    , recipientId_        ( recipientId )
    , dotationCategory_   ( &dotationCategory )
    , state_              ( sword::LogSupplyRequestUpdate::request_outstanding )
    , supplier_           ( 0 )
    , requestedQuantity_  ( 0 )
    , grantedQuantity_    ( 0 )
    , convoyedQuantity_   ( 0 )
    , suppliedQuantity_   ( 0 )
    , complementarySupply_( true )
    , provider_           ( 0 )
    , needNetworkUpdate_  ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest constructor
// Created: LDC 2013-01-16
// -----------------------------------------------------------------------------
SupplyRequest::SupplyRequest()
    : id_                 ( 0 )
    , recipientId_        ( 0 )
    , dotationCategory_   ( 0 )
    , state_              ( sword::LogSupplyRequestUpdate::request_outstanding )
    , supplier_           ( 0 )
    , requestedQuantity_  ( 0 )
    , grantedQuantity_    ( 0 )
    , convoyedQuantity_   ( 0 )
    , suppliedQuantity_   ( 0 )
    , complementarySupply_( true )
    , provider_           ( 0 )
    , needNetworkUpdate_  ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest destructorsupplier_
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
SupplyRequest::~SupplyRequest()
{
    //$$$ Return remaining stock automatically - BOF: magic
    if( supplier_ && convoyedQuantity_ > 0 )
        supplier_->SupplyReturnStock( *dotationCategory_, convoyedQuantity_ );

    if( supplierQuotas_ && requestedQuantity_ > suppliedQuantity_ )
        supplierQuotas_->ReturnQuota( *dotationCategory_, requestedQuantity_ - suppliedQuantity_ );
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::AddResource
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void SupplyRequest::AddResource( const boost::shared_ptr< SupplyResource_ABC >& resource, const MIL_AgentPion& pion, double quantity )
{
    assert( quantity > 0 );
    if( quantity <= 0 )
        return;
    resources_.push_back( std::make_pair( resource, quantity ) );
    requesters_.push_back( &pion );
    requestedQuantity_ += quantity;
    needNetworkUpdate_ = true;
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
    if( supplier.SupplyHasStock( *dotationCategory_ ) )
    {
        supplier_ = &supplier;
        needNetworkUpdate_ = true;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::AffectSupplier
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
bool SupplyRequest::AffectSupplier( SupplyRecipient_ABC& recipient, const boost::shared_ptr< LogisticLink_ABC >& supplier )
{
    if( supplier_ )
        return true;
    if( !AffectSupplier( supplier->GetSuperior() ) )
    {
        recipient.NotifyStockNotAvailable( *dotationCategory_, requesters_ );
        return false;
    }
    const double authorizedQuantity = supplier->ConsumeQuota( *dotationCategory_, requestedQuantity_, requesters_ );
    if( authorizedQuantity <= 0 )
    {
        supplier_ = 0;
        return false;
    }
    supplierQuotas_ = supplier;
    assert( authorizedQuantity <= requestedQuantity_ );
    UpdateRequestedQuantities( authorizedQuantity );
    return true;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::HasRequester
// Created: JSR 2013-03-01
// -----------------------------------------------------------------------------
bool SupplyRequest::HasRequester( const MIL_AgentPion& pion ) const
{
    for( auto it = requesters_.begin(); it != requesters_.end(); ++it )
        if( *it == &pion )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::HasRequesterDestroyed
// Created: JSR 2013-03-05
// -----------------------------------------------------------------------------
bool SupplyRequest::HasRequesterDestroyed() const
{
    for( auto it = requesters_.begin(); it != requesters_.end(); ++it )
        if( ( *it )->IsMarkedForDestruction() )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::UpdateRequestedQuantities
// Created: MCO 2012-12-11
// -----------------------------------------------------------------------------
void SupplyRequest::UpdateRequestedQuantities( double authorizedQuantity )
{
    const double ratio = authorizedQuantity / requestedQuantity_;
    BOOST_FOREACH( T_Resources::value_type& data, resources_ )
        data.second *= ratio;
    requestedQuantity_ = authorizedQuantity;
    needNetworkUpdate_ = true;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::Supply
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
double SupplyRequest::Supply()
{
          double suppliedQuantity = 0;
    const double tmp              = convoyedQuantity_ / requestedQuantity_;
    BOOST_FOREACH( T_Resources::value_type& data, resources_ )
    {
        const double value = data.second * tmp;
        suppliedQuantity += data.first->Supply( value );
    }
    convoyedQuantity_ -= suppliedQuantity;
    suppliedQuantity_ += suppliedQuantity;
    state_ = sword::LogSupplyRequestUpdate::request_delivered;
    needNetworkUpdate_ = true;
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
    state_ = sword::LogSupplyRequestUpdate::request_conveyed;
    needNetworkUpdate_ = true;
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
    needNetworkUpdate_ = true;
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

    logistic::SupplySupplier_ABC::Stock stock = supplier_->SupplyGetStock( *dotationCategory_, requestedQuantity_ );
    grantedQuantity_ = stock.quantity_;
    if( !provider_ )
        provider_ = stock.provider_;
    convoyedQuantity_ = 0;
    state_ = sword::LogSupplyRequestUpdate::request_granted;
    needNetworkUpdate_ = true;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::ReturnStockNotAssignedToConvoy
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void SupplyRequest::ReturnStockNotAssignedToConvoy()
{
    double tmp = grantedQuantity_ - convoyedQuantity_;
    if( tmp > 0 )
        supplier_->SupplyReturnStock( *dotationCategory_, tmp );
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::NotifySuperiorNotAvailable
// Created: MCO 2012-12-11
// -----------------------------------------------------------------------------
void SupplyRequest::NotifySuperiorNotAvailable( SupplyRecipient_ABC& recipient )
{
    recipient.NotifySuperiorNotAvailable( requesters_ );
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::GetSupplier
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
SupplySupplier_ABC* SupplyRequest::GetSupplier() const
{
    return supplier_;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::GetId
// Created: LGY 2014-04-30
// -----------------------------------------------------------------------------
unsigned int SupplyRequest::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::GetDotationCategory
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
const PHY_DotationCategory& SupplyRequest::GetDotationCategory() const
{
    return *dotationCategory_;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::GetGrantedQuantity
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
double SupplyRequest::GetGrantedQuantity() const
{
    return grantedQuantity_;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::GetRequestedQuantity
// Created: NLD 2012-03-11
// -----------------------------------------------------------------------------
double SupplyRequest::GetRequestedQuantity() const
{
    return requestedQuantity_;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::IsComplementary
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
bool SupplyRequest::IsComplementary() const
{
    return complementarySupply_;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::GetProvider
// Created: LDC 2013-01-08
// -----------------------------------------------------------------------------
const MIL_Agent_ABC* SupplyRequest::GetProvider() const
{
    return provider_;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::Serialize
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void SupplyRequest::Serialize( sword::SupplyResourceRequest& msg ) const
{
    msg.mutable_resource()->set_id( dotationCategory_->GetMosID() );
    msg.set_requested( (unsigned)requestedQuantity_ );
    msg.set_granted  ( (unsigned)grantedQuantity_ );
    msg.set_convoyed ( (unsigned)convoyedQuantity_ );
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::load
// Created: LDC 2013-01-16
// -----------------------------------------------------------------------------
void SupplyRequest::load( MIL_CheckPointInArchive& archive, const unsigned int )
{
    archive >> boost::serialization::base_object< SupplyRequest_ABC >( *this );
    archive >> id_;
    archive >> recipientId_;
    archive >> dotationCategory_;
    archive >> state_;
    archive >> resources_;
    archive >> requesters_;
    archive >> supplier_;
    archive >> requestedQuantity_;
    archive >> grantedQuantity_;
    archive >> convoyedQuantity_;
    archive >> suppliedQuantity_;
    archive >> complementarySupply_;
    archive >> supplierQuotas_;
    archive >> provider_;
    archive >> needNetworkUpdate_;
    RegisterConsignId( id_ );
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::save
// Created: LDC 2013-01-16
// -----------------------------------------------------------------------------
void SupplyRequest::save( MIL_CheckPointOutArchive& archive, const unsigned int ) const
{
    archive << boost::serialization::base_object< SupplyRequest_ABC >( *this );
    archive << id_;
    archive << recipientId_;
    archive << dotationCategory_;
    archive << state_;
    archive << resources_;
    archive << requesters_;
    archive << supplier_;
    archive << requestedQuantity_;
    archive << grantedQuantity_;
    archive << convoyedQuantity_;
    archive << suppliedQuantity_;
    archive << complementarySupply_;
    archive << supplierQuotas_;
    archive << provider_;
    archive << needNetworkUpdate_;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::SendCreation
// Created: LGY 2014-04-30
// -----------------------------------------------------------------------------
void SupplyRequest::SendCreation() const
{
    client::LogSupplyRequestCreation msg;
    msg().mutable_request()->set_id( id_ );
    msg().mutable_requester()->set_id( recipientId_ );
    msg().mutable_resource()->set_id( dotationCategory_->GetMosID() );
    msg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::SendDestruction
// Created: LGY 2014-04-30
// -----------------------------------------------------------------------------
void SupplyRequest::SendDestruction() const
{
    SendChangedState();
    client::LogSupplyRequestDestruction msg;
    msg().mutable_request()->set_id( id_ );
    msg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::SendChangedState
// Created: LGY 2014-04-30
// -----------------------------------------------------------------------------
void SupplyRequest::SendChangedState() const
{
    if( !needNetworkUpdate_ )
        return;
    client::LogSupplyRequestUpdate msg;
    msg().mutable_request()->set_id( id_ );
    msg().set_state( state_ );
    msg().mutable_recipient()->set_id( recipientId_ );
    if( supplier_ )
        msg().mutable_supplier()->set_id( supplier_->GetSupplierId() );
    msg().set_requested( static_cast< unsigned int >( requestedQuantity_ ) );
    msg().set_granted( static_cast< unsigned int >( grantedQuantity_ ) );
    msg().set_convoyed( static_cast< unsigned int >( convoyedQuantity_ ) );
    msg().set_delivered( static_cast< unsigned int >( suppliedQuantity_ ) );
    msg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::SendFullState
// Created: LGY 2014-04-30
// -----------------------------------------------------------------------------
void SupplyRequest::SendFullState() const
{
    SendCreation();
    client::LogSupplyRequestUpdate msg;
    msg().mutable_request()->set_id( id_ );
    msg().set_state( state_ );
    msg().mutable_recipient()->set_id( recipientId_ );
    if( supplier_ )
        msg().mutable_supplier()->set_id( supplier_->GetSupplierId() );
    msg().set_requested( static_cast< unsigned int >( requestedQuantity_ ) );
    msg().set_granted( static_cast< unsigned int >( grantedQuantity_ ) );
    msg().set_convoyed( static_cast< unsigned int >( convoyedQuantity_ ) );
    msg().set_delivered( static_cast< unsigned int >( suppliedQuantity_ ) );
    msg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: SupplyRequest::Clean
// Created: LGY 2014-04-30
// -----------------------------------------------------------------------------
void SupplyRequest::Clean()
{
    needNetworkUpdate_ = false;
}

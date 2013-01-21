//*****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SupplyRequestContainer.h"
#include "SupplyResource_ABC.h"
#include "SupplyRequest.h"
#include "SupplyRequestDispatcher_ABC.h"
#include "SupplyRequestBuilder_ABC.h"
#include "SupplyConsign.h"
#include "SupplyConvoyVirtualFactory.h"
#include <boost/foreach.hpp>

using namespace logistic;

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer constructor
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplyRequestContainer::SupplyRequestContainer( boost::shared_ptr< SupplyRequestBuilder_ABC > builder )
    : builder_             ( builder )
    , transportersProvider_( 0 )
    , convoyFactory_       ( &SupplyConvoyVirtualFactory::Instance() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyStockRequestBuilder destructor
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplyRequestContainer::~SupplyRequestContainer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::AddResource
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyRequestContainer::AddResource( SupplyRecipient_ABC& recipient, const MIL_AgentPion& pion, boost::shared_ptr< SupplyResource_ABC > resource, double quantity )
{
    boost::shared_ptr< SupplyRequest_ABC >& supplyRequest = requests_[ &recipient ][ &resource->GetCategory() ];
    if( !supplyRequest.get() )
        supplyRequest.reset( new SupplyRequest( resource->GetCategory() ) );
    supplyRequest->AddResource( resource, pion, quantity );
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::Prepare
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void SupplyRequestContainer::Prepare()
{
    requests_.clear();
    transporters_.clear();
    recipientPaths_.clear();
    transportersProviderPath_.clear();
    supplierPath_.clear();
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::Execute
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
bool SupplyRequestContainer::Execute( SupplyRequestDispatcher_ABC& dispatcher )
{
    if( !consigns_.empty() )
        return false;

    Prepare();
    builder_->Process( *this );
    BOOST_FOREACH( T_RecipientRequests::value_type& recipient, requests_ )
        BOOST_FOREACH( T_Requests::value_type& request, recipient.second )
            dispatcher.Dispatch( *recipient.first, *request.second );
    if( !dispatcher.AllowSupply() )
        return false;

    bool mandatoryRequestsFullySatisfied = true;
    BOOST_FOREACH( T_RecipientRequests::value_type& recipientRequest, requests_ )
    {
        BOOST_FOREACH( T_Requests::value_type& requestData, recipientRequest.second )
        {
            boost::shared_ptr< SupplyRequest_ABC > request = requestData.second;
            SupplySupplier_ABC* supplier = request->GetSupplier();
            if( supplier )
            {
                boost::shared_ptr< SupplyConsign_ABC >& consign = consigns_[ supplier ];
                if( !consign.get() )
                    consign.reset( new SupplyConsign( *supplier, *this ) );
                consign->AddRequest( *recipientRequest.first, request );
            }
            else if( !request->IsComplementary() )
                mandatoryRequestsFullySatisfied = false;
        }
    }
    BOOST_FOREACH( T_Consigns::value_type& data, consigns_ )
        data.second->Activate();
    return mandatoryRequestsFullySatisfied;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::Update
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
bool SupplyRequestContainer::Update()
{
    for( T_Consigns::iterator it = consigns_.begin(); it != consigns_.end(); )
    {
        if( it->second->IsFinished() )
            it = consigns_.erase( it );
        else
            ++it;
    }
    return consigns_.empty();
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::Clean
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void SupplyRequestContainer::Clean()
{
    BOOST_FOREACH( const T_Consigns::value_type& data, consigns_ )
        data.second->Clean();
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::SetTransportersProvider
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyRequestContainer::SetTransportersProvider( SupplySupplier_ABC* provider )
{
    transportersProvider_ = provider;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::SetTransporters
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplySupplier_ABC* SupplyRequestContainer::GetTransportersProvider() const
{
    return transportersProvider_;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::SetTransporters
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyRequestContainer::SetTransporters( const T_Transporters& transporters )
{
    transporters_ = transporters;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::SetTransporters
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
const SupplyRequestContainer::T_Transporters& SupplyRequestContainer::GetTransporters() const
{
    return transporters_;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::SetPathToRecipient
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyRequestContainer::SetPathToRecipient( SupplyRecipient_ABC& recipient, const T_PointVector& wayPoints )
{
    recipientPaths_[ &recipient ] = wayPoints;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::GetPath
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
const T_PointVector& SupplyRequestContainer::GetPathToRecipient( SupplyRecipient_ABC& recipient ) const
{
    static const T_PointVector default;
    T_RecipientPaths::const_iterator it = recipientPaths_.find( &recipient );
    if( it == recipientPaths_.end() )
        return default;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::SetPathToTransportersProvider
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyRequestContainer::SetPathToTransportersProvider( const T_PointVector& wayPoints )
{
    transportersProviderPath_ = wayPoints;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::GetPathToTransportersProvider
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
const T_PointVector& SupplyRequestContainer::GetPathToTransportersProvider() const
{
    return transportersProviderPath_;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::SetPathToSupplier
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyRequestContainer::SetPathToSupplier( const T_PointVector& wayPoints )
{
    supplierPath_ = wayPoints;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::GetPathToSupplier
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
const T_PointVector& SupplyRequestContainer::GetPathToSupplier() const
{
    return supplierPath_;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::SetConvoyFactory
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyRequestContainer::SetConvoyFactory( const SupplyConvoyFactory_ABC& convoyFactory )
{
    convoyFactory_ = &convoyFactory;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::GetConvoyFactory
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
const SupplyConvoyFactory_ABC& SupplyRequestContainer::GetConvoyFactory() const
{
    assert( convoyFactory_ );
    return *convoyFactory_;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::SendChangedState
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void SupplyRequestContainer::SendChangedState() const
{
    BOOST_FOREACH( const T_Consigns::value_type& data, consigns_ )
        data.second->SendChangedState();
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::SendFullState
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void SupplyRequestContainer::SendFullState() const
{
    BOOST_FOREACH( const T_Consigns::value_type& data, consigns_ )
        data.second->SendFullState();
}

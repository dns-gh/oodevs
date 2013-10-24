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
#include "SupplyConvoyRealFactory.h"
#include "SupplyRecipient_ABC.h"
#include "SupplySupplier_ABC.h"
#include "Checkpoints/MIL_CheckPointInArchive.h"
#include "Checkpoints/MIL_CheckPointOutArchive.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteTypePion.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include <boost/foreach.hpp>

using namespace logistic;

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer constructor
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplyRequestContainer::SupplyRequestContainer( const boost::shared_ptr< SupplyRequestBuilder_ABC >& builder )
    : builder_             ( builder )
    , transportersProvider_( 0 )
    , convoyFactory_       ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer constructor
// Created: LDC 2013-01-16
// -----------------------------------------------------------------------------
SupplyRequestContainer::SupplyRequestContainer()
    : transportersProvider_( 0 )
    , convoyFactory_       ( &SupplyConvoyRealFactory::Instance() ) // $$$$ LDC We are deserializing only real convoys for now
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
// Name: SupplyRequestContainer::ResetConsignsForConvoyPion
// Created: JSR 2013-02-07
// -----------------------------------------------------------------------------
void SupplyRequestContainer::ResetConsignsForConvoyPion( const MIL_AgentPion& pion )
{
    for( auto it = consigns_.begin(); it != consigns_.end(); )
        if( it->second->ResetConsignsForConvoyPion( pion ) )
            it = consigns_.erase( it );
        else
            ++it;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::ResetConsignsForProvider
// Created: JSR 2013-02-14
// -----------------------------------------------------------------------------
void SupplyRequestContainer::ResetConsignsForProvider( const MIL_Agent_ABC& pion )
{
    for( auto it = consigns_.begin(); it != consigns_.end(); )
        if( it->second->ResetConsignsForProvider( pion ) )
            it = consigns_.erase( it );
        else
            ++it;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::ResetConsign
// Created: JSR 2013-02-14
// -----------------------------------------------------------------------------
void SupplyRequestContainer::ResetConsign()
{
    for( auto it = consigns_.begin(); it != consigns_.end(); ++it )
        it->second->ResetConsign();
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::DeleteRequestsForRequester
// Created: JSR 2013-03-01
// -----------------------------------------------------------------------------
void SupplyRequestContainer::DeleteRequestsForRequester( const MIL_AgentPion& pion )
{
    for( auto it = requests_.begin(); it != requests_.end(); ++it )
    {
        auto requests = it->second;
        for( auto it2 = requests.begin(); it2 != requests.end(); )
        {
            if( it2->second->HasRequester( pion ) )
                it2 = requests.erase( it2 );
            else
                ++it2;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::AddResource
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyRequestContainer::AddResource( SupplyRecipient_ABC& recipient, const MIL_AgentPion& pion, const boost::shared_ptr< SupplyResource_ABC >& resource, double quantity )
{
    boost::shared_ptr< SupplyRequest_ABC >& request = requests_[ &recipient ][ &resource->GetCategory() ];
    if( !request )
        request.reset( new SupplyRequest( resource->GetCategory() ) );
    request->AddResource( resource, pion, quantity );
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
    BOOST_FOREACH( const auto& recipient, requests_ )
        BOOST_FOREACH( const auto& request, recipient.second )
            dispatcher.Dispatch( *recipient.first, *request.second );
    if( !dispatcher.AllowSupply() )
        return false;

    bool mandatoryRequestsFullySatisfied = true;
    BOOST_FOREACH( const auto& recipient, requests_ )
        BOOST_FOREACH( const auto& requestData, recipient.second )
        {
            const boost::shared_ptr< SupplyRequest_ABC >& request = requestData.second;
            SupplySupplier_ABC* supplier = request->GetSupplier();
            if( supplier )
            {
                boost::shared_ptr< SupplyConsign_ABC >& consign = consigns_[ supplier ];
                if( !consign )
                    consign.reset( new SupplyConsign( *supplier, *this ) );
                consign->AddRequest( *recipient.first, request );
            }
            else if( !request->IsComplementary() )
                mandatoryRequestsFullySatisfied = false;
        }
    BOOST_FOREACH( const auto& data, consigns_ )
        data.second->Activate();
    std::vector< SupplySupplier_ABC* > consignsToRemove;
    for( auto it = consigns_.begin(); it != consigns_.end(); ++it )
        if( it->second->GrantsNothing() )
            consignsToRemove.push_back( it->first );
    for( auto it = consignsToRemove.begin(); it != consignsToRemove.end(); ++it ) 
        consigns_.erase( *it );
    return mandatoryRequestsFullySatisfied;
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::Update
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
bool SupplyRequestContainer::Update()
{
    for( auto it = consigns_.begin(); it != consigns_.end(); )
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
    BOOST_FOREACH( const auto& data, consigns_ )
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
    auto it = recipientPaths_.find( &recipient );
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
    BOOST_FOREACH( const auto& data, consigns_ )
        data.second->SendChangedState();
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::SendFullState
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void SupplyRequestContainer::SendFullState() const
{
    BOOST_FOREACH( const auto& data, consigns_ )
        data.second->SendFullState();
}

// -----------------------------------------------------------------------------
// Name: void SupplyRequestContainer::serialize
// Created: LDC 2013-01-16
// -----------------------------------------------------------------------------
void SupplyRequestContainer::serialize( MIL_CheckPointInArchive& archive, const unsigned int )
{
    archive >> boost::serialization::base_object< SupplyRequestParameters_ABC >( *this );
    archive >> builder_;
    archive >> dispatcher_;
    size_t requestsSize;
    archive >> requestsSize;
    for( size_t i = 0; i < requestsSize; ++i )
    {
        SupplyRecipient_ABC* recipient; 
        archive >> recipient;
        size_t requestSize;
        archive >> requestSize;
        for( size_t j = 0; j < requestSize; ++j )
        {
            unsigned int dotationCategory;
            archive >> dotationCategory;
            const PHY_DotationCategory* category = PHY_DotationType::FindDotationCategory( dotationCategory );
            boost::shared_ptr< SupplyRequest_ABC > request;
            archive >> request;
            requests_[ recipient ][ category ] = request;
        }
    }
    archive >> consigns_;
    archive >> transportersProvider_;
    archive >> transporters_;
    archive >> recipientPaths_;
    archive >> transportersProviderPath_;
    archive >> supplierPath_;
}

// -----------------------------------------------------------------------------
// Name: void SupplyRequestContainer::serialize
// Created: LDC 2013-01-16
// -----------------------------------------------------------------------------
void SupplyRequestContainer::serialize( MIL_CheckPointOutArchive& archive, const unsigned int )
{
    size_t requestsSize = requests_.size();
    archive << boost::serialization::base_object< SupplyRequestParameters_ABC >( *this );
    archive << builder_;
    archive << dispatcher_;
    archive << requestsSize;
    for( auto it = requests_.begin(); it != requests_.end(); ++it )
    {
        const T_Requests& request = it->second;
        size_t requestSize = request.size();
        archive << it->first;
        archive << requestSize;
        for( auto requestIt = request.begin(); requestIt != request.end(); ++requestIt )
        {
            unsigned int id = requestIt->first->GetMosID();
            archive << id;
            archive << requestIt->second;
        }
    }
    archive << consigns_;
    archive << transportersProvider_;
    archive << transporters_;
    archive << recipientPaths_;
    archive << transportersProviderPath_;
    archive << supplierPath_;
}

BOOST_CLASS_EXPORT_IMPLEMENT( logistic::SupplyRequestContainer )

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
#include "Entities/Orders/MIL_ItineraryParameter.h"
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>

using namespace logistic;

BOOST_CLASS_EXPORT_IMPLEMENT( logistic::SupplyRequestContainer )

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
void SupplyRequestContainer::ResetConsignsForConvoyPion( const MIL_Agent_ABC& pion )
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
        request.reset( new SupplyRequest( resource->GetCategory(), recipient.GetRecipientId() ) );
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
    supplierPath_.reset();
    transportersProviderPath_.reset();
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
    for( auto it = consigns_.begin(); it != consigns_.end(); ++it )
        it->second->Activate();
    for( auto it = consigns_.begin(); it != consigns_.end(); )
        if( it->second->GrantsNothing() )
            consigns_.erase( it++ );
        else
            ++it;
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
// Name: SupplyRequestContainer::GetTransportersProvider
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

void SupplyRequestContainer::SetPathToRecipient( SupplyRecipient_ABC& recipient, const sword::Pathfind& pathfind )
{
    recipientPaths_[ &recipient ] = boost::make_shared< MIL_ItineraryParameter >( pathfind, boost::none );
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::GetPath
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > SupplyRequestContainer::GetPathToRecipient( const MT_Vector2D& start, SupplyRecipient_ABC& recipient ) const
{
    auto it = recipientPaths_.find( &recipient );
    if( it != recipientPaths_.end() )
        return it->second->AddClosestWaypoints( boost::make_shared< MT_Vector2D >( start ), boost::make_shared< MT_Vector2D >( recipient.GetPosition() ) );
    return std::vector< boost::shared_ptr< MT_Vector2D > >();
}

void SupplyRequestContainer::ToRecipientItinerary( SupplyRecipient_ABC& recipient, sword::Pathfind& pathfind ) const
{
    auto it = recipientPaths_.find( &recipient );
    if( it != recipientPaths_.end() )
        it->second->ToItinerary( pathfind );
}

void SupplyRequestContainer::ToTransportersItinerary( sword::Pathfind& pathfind ) const
{
    if( transportersProviderPath_ )
        transportersProviderPath_->ToItinerary( pathfind );
}

void SupplyRequestContainer::ToSupplierItinerary( sword::Pathfind& pathfind ) const
{
    if( supplierPath_ )
        supplierPath_->ToItinerary( pathfind );
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::SetPathToTransportersProvider
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyRequestContainer::SetPathToTransportersProvider( const sword::Pathfind& pathfind )
{
    transportersProviderPath_ = boost::make_shared< MIL_ItineraryParameter >( pathfind, boost::none );
}

namespace
{
    template< typename T >
    std::vector< boost::shared_ptr< MT_Vector2D > > GetClosestWaypoints( const T& itinerary, const MT_Vector2D& start, const MT_Vector2D& end )
    {
        if( itinerary )
            return itinerary->AddClosestWaypoints( boost::make_shared< MT_Vector2D >( start ), boost::make_shared< MT_Vector2D >( end ) );
        return std::vector< boost::shared_ptr< MT_Vector2D > >();
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::GetPathToTransportersProvider
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > SupplyRequestContainer::GetPathToTransportersProvider( const MT_Vector2D& start, const MT_Vector2D& end ) const
{
    return GetClosestWaypoints( transportersProviderPath_, start, end );
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::SetPathToSupplier
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyRequestContainer::SetPathToSupplier( const sword::Pathfind& pathfind )
{
    supplierPath_ = boost::make_shared< MIL_ItineraryParameter >( pathfind, boost::none );
}

// -----------------------------------------------------------------------------
// Name: SupplyRequestContainer::GetPathToSupplier
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > SupplyRequestContainer::GetPathToSupplier( const MT_Vector2D& start, const MT_Vector2D& end ) const
{
    return GetClosestWaypoints( supplierPath_, start, end );
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
    archive >> requests_;
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
    archive << boost::serialization::base_object< SupplyRequestParameters_ABC >( *this );
    archive << builder_;
    archive << dispatcher_;
    archive << requests_;
    archive << consigns_;
    archive << transportersProvider_;
    archive << transporters_;
    archive << recipientPaths_;
    archive << transportersProviderPath_;
    archive << supplierPath_;
}

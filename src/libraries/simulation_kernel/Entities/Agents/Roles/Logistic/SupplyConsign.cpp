// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SupplyConsign.h"
#include "MIL_AgentServer.h"
#include "SupplyRequest_ABC.h"
#include "SupplyConvoy_ABC.h"
#include "SupplyConvoyFactory_ABC.h"
#include "SupplyRecipient_ABC.h"
#include "SupplyRequestParameters_ABC.h"
#include "SupplySupplier_ABC.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <boost/foreach.hpp>
#include <boost/serialization/deque.hpp>
#include <boost/serialization/export.hpp>

using namespace logistic;

MIL_IDManager SupplyConsign::idManager_;

BOOST_CLASS_EXPORT_IMPLEMENT( logistic::SupplyConsign )

// -----------------------------------------------------------------------------
// Name: SupplyConsign constructor
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplyConsign::SupplyConsign( SupplySupplier_ABC& supplier, SupplyRequestParameters_ABC& parameters )
    : id_                       ( idManager_.GetFreeId() )
    , creationTick_             ( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() ) //$$$ Huge shit
    , supplier_                 ( &supplier )
    , provider_                 ( 0 )
    , state_                    ( eConvoyWaitingForTransporters )
    , currentStateEndTimeStep_  ( std::numeric_limits< unsigned >::max() )
    , convoy_                   ( parameters.GetConvoyFactory().Create( *this, supplier, parameters ) )
    , currentRecipient_         ( 0 )
    , needNetworkUpdate_        ( true )
    , requestsNeedNetworkUpdate_( true )
{
    SendMsgCreation();
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign constructor
// Created: JSR 2013-01-15
// -----------------------------------------------------------------------------
SupplyConsign::SupplyConsign()
    : id_                       ( 0 )
    , creationTick_             ( 0 ) 
    , supplier_                 ( 0 )
    , provider_                 ( 0 )
    , state_                    ( eConvoyWaitingForTransporters )
    , currentStateEndTimeStep_  ( std::numeric_limits< unsigned >::max() )
    , currentRecipient_         ( 0 )
    , needNetworkUpdate_        ( true )
    , requestsNeedNetworkUpdate_( true )
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign destructor
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplyConsign::~SupplyConsign()
{
    convoy_->Finish();
    SendMsgDestruction();
    BOOST_FOREACH( const T_RecipientRequests::value_type& data, requestsQueued_ )
        data.first->OnSupplyCanceled( shared_from_this() );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyConsign::AddRequest
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyConsign::AddRequest( SupplyRecipient_ABC& recipient, boost::shared_ptr< SupplyRequest_ABC > request )
{
    T_Requests* requests = 0;
    BOOST_FOREACH( T_RecipientRequests::value_type& data, requestsQueued_ )
    {
        if( data.first == &recipient )
            requests = &data.second;
    }
    if( !requests )
    {
        requestsQueued_.push_back( std::make_pair( &recipient, T_Requests() ) );
        requests = &requestsQueued_.back().second;
    }
    (*requests)[ &request->GetDotationCategory() ] = request;
    requestsNeedNetworkUpdate_ = true;
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::Activate
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyConsign::Activate()
{
    resources_.clear();
    BOOST_FOREACH( T_RecipientRequests::value_type& recipientRequest, requestsQueued_ )
        BOOST_FOREACH( T_Requests::value_type& requestData, recipientRequest.second )
        {
            boost::shared_ptr< SupplyRequest_ABC > request = requestData.second;
            request->ReserveStock();
            resources_[ &request->GetDotationCategory() ] += request->GetGrantedQuantity();
            if( !provider_ )
                provider_ = const_cast< MIL_Agent_ABC* >( request->GetProvider() );
        }
    BOOST_FOREACH( const T_RecipientRequests::value_type& data, requestsQueued_ )
        data.first->OnSupplyScheduled( shared_from_this() );
    supplier_->SupplyHandleRequest( shared_from_this() );
    requestsNeedNetworkUpdate_ = true;
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyConsign::UpdateTimer
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyConsign::UpdateTimer( unsigned timeRemaining )
{
    unsigned tmp = std::numeric_limits< unsigned >::max();
    if( timeRemaining != std::numeric_limits< unsigned >::max() )
        tmp = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() + timeRemaining;

    if( tmp != currentStateEndTimeStep_ )
    {
        currentStateEndTimeStep_ = tmp;
        needNetworkUpdate_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::GetCurrentSupplyRecipient
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplyRecipient_ABC* SupplyConsign::GetCurrentSupplyRecipient() const
{
    return requestsQueued_.empty() ? 0 : requestsQueued_.front().first;
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::SupplyCurrentRecipient
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyConsign::SupplyCurrentRecipient()
{
    assert( !requestsQueued_.empty() );
    assert( currentRecipient_ == requestsQueued_.front().first );

    currentRecipient_->OnSupplyDone( shared_from_this() ); //$$$ CRADE MUST BE CALL BEFORE convoy_->Supply() to allow recipient to reschedule supply immediately
    BOOST_FOREACH( T_Requests::value_type& data, requestsQueued_.front().second )
    {
        boost::shared_ptr< SupplyRequest_ABC > request = data.second;
        double quantitySupplied = request->Supply();
        convoy_->Supply( *currentRecipient_, request->GetDotationCategory(), quantitySupplied );
    }
    requestsQueued_.pop_front();
    requestsNeedNetworkUpdate_ = true;
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::IsSupplying
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
bool SupplyConsign::IsSupplying( const PHY_DotationCategory& dotationCategory, const SupplyRecipient_ABC& recipient ) const
{
    BOOST_FOREACH( const T_RecipientRequests::value_type& data, requestsQueued_ )
    {
        if( data.first == &recipient )
        {
            const T_Requests& requests = data.second;
            return requests.find( &dotationCategory ) != requests.end();
        }
        data.first->OnSupplyScheduled( shared_from_this() );
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::IsFinished
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
bool SupplyConsign::IsFinished() const
{
    return state_ == eFinished;
}

// -----------------------------------------------------------------------------
// Name: (Supply)Consign::SetState
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyConsign::SetState( E_State newState )
{
    if( newState != state_ )
    {
        state_ = newState;
        UpdateTimer( std::numeric_limits< unsigned >::max() );
        needNetworkUpdate_ = true;
    }
}

// =============================================================================
// FSM
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyConsign::IsActionDone
// Created: NLD 2005-12-14
// -----------------------------------------------------------------------------
bool SupplyConsign::IsActionDone( unsigned timeRemaining )
{
    UpdateTimer( timeRemaining );
    return currentStateEndTimeStep_ != std::numeric_limits< unsigned >::max() && MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() >= currentStateEndTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::SupplyAndProceedWithNextRecipient
// Created: NLD 2005-12-14
// -----------------------------------------------------------------------------
void SupplyConsign::SupplyAndProceedWithNextRecipient()
{
    if( currentRecipient_ )
        SupplyCurrentRecipient();

    currentRecipient_ = GetCurrentSupplyRecipient();
    convoy_->SetCurrentSupplyRecipient( currentRecipient_ );
    if( !currentRecipient_ )
        SetState( eConvoyGoingBackToFormingPoint );
    else
        SetState( eConvoyGoingToUnloadingPoint );
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::DoConvoyReserveTransporters
// Created: NLD 2005-12-14
// -----------------------------------------------------------------------------
void SupplyConsign::DoConvoyReserveTransporters()
{
    if( IsActionDone( convoy_->ReserveTransporters( resources_ ) ) )
        SetState( eConvoySetup );
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::DoConvoySetup
// Created: NLD 2005-12-14
// -----------------------------------------------------------------------------
void SupplyConsign::DoConvoySetup()
{
    if( IsActionDone( convoy_->Setup() ) )
    {
        SetState( eConvoyGoingToLoadingPoint );
        convoy_->SetProvider( provider_ );
        convoy_->GetTransportersProvider().OnSupplyConvoyLeaving( shared_from_this() );
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::DoConvoyMoveToSupplier
// Created: NLD 2005-12-14
// -----------------------------------------------------------------------------
void SupplyConsign::DoConvoyMoveToSupplier()
{
    if( IsActionDone( convoy_->MoveToSupplier() ) )
    {
        SetState( eConvoyLoading );
        supplier_->OnSupplyConvoyArriving( shared_from_this() );
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::DoConvoyLoad
// Created: NLD 2005-12-14
// -----------------------------------------------------------------------------
void SupplyConsign::DoConvoyLoad()
{
    if( IsActionDone( convoy_->Load() ) )
    {
        supplier_->OnSupplyConvoyLeaving( shared_from_this() );
        SupplyAndProceedWithNextRecipient();
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::DoConvoyMoveToSupplyRecipient
// Created: NLD 2005-12-14
// -----------------------------------------------------------------------------
void SupplyConsign::DoConvoyMoveToSupplyRecipient()
{
    assert( currentRecipient_ );
    if( IsActionDone( convoy_->MoveToSupplyRecipient( ) ) )
    {
        currentRecipient_->OnSupplyConvoyArriving( shared_from_this() );
        SetState( eConvoyUnloading );
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::DoConvoyUnload
// Created: NLD 2005-12-14
// -----------------------------------------------------------------------------
void SupplyConsign::DoConvoyUnload()
{
    assert( currentRecipient_ );
    if( IsActionDone( convoy_->Unload() ) )
    {
        currentRecipient_->OnSupplyConvoyLeaving( shared_from_this() );
        SupplyAndProceedWithNextRecipient();
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::DoConvoyMoveToTransportersProvider
// Created: NLD 2005-12-14
// -----------------------------------------------------------------------------
void SupplyConsign::DoConvoyMoveToTransportersProvider()
{
    if( IsActionDone( convoy_->MoveToTransportersProvider() ) )
    {
        SetState( eFinished ); 
        convoy_->GetTransportersProvider().OnSupplyConvoyArriving( shared_from_this() );
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::Update
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool SupplyConsign::Update()
{
    switch( state_ )
    {
        case eConvoyWaitingForTransporters  : DoConvoyReserveTransporters(); break;
        case eConvoySetup                   : DoConvoySetup(); break;
        case eConvoyGoingToLoadingPoint     : DoConvoyMoveToSupplier(); break;
        case eConvoyLoading                 : DoConvoyLoad(); break; 
        case eConvoyGoingToUnloadingPoint   : DoConvoyMoveToSupplyRecipient(); break;
        case eConvoyUnloading               : DoConvoyUnload(); break;
        case eConvoyGoingBackToFormingPoint : DoConvoyMoveToTransportersProvider(); break;
        case eFinished                      : convoy_->Finish(); break;
        default:
            assert( false );
    }
    return state_ == eFinished;
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::Clean
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void SupplyConsign::Clean()
{
    needNetworkUpdate_ = false;
    requestsNeedNetworkUpdate_ = false;
}

// =============================================================================
// SupplyConvoyEventsObserver_ABC
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyConsign::OnAllResourcesAssignedToConvoy
// Created: NLD 2008-08-01
// -----------------------------------------------------------------------------
void SupplyConsign::OnAllResourcesAssignedToConvoy()
{
    BOOST_FOREACH( T_RecipientRequests::value_type& it1, requestsQueued_ )
        BOOST_FOREACH( T_Requests::value_type& it2, it1.second )
            it2.second->ReturnStockNotAssignedToConvoy();
    requestsNeedNetworkUpdate_ = true;
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::OnResourceAssignedToConvoy
// Created: NLD 2008-08-01
// -----------------------------------------------------------------------------
void SupplyConsign::OnResourceAssignedToConvoy( const PHY_DotationCategory& dotationCategory, double quantity )
{
    for( T_RecipientRequests::const_iterator data = requestsQueued_.begin(); data != requestsQueued_.end() && quantity > 0; ++data )
    {
        const T_Requests& requests = data->second;
        T_Requests::const_iterator it = requests.find( &dotationCategory );
        if( it != requests.end() )
            quantity -= it->second->Convoy( quantity );
    }
    assert( quantity == 0 );
    requestsNeedNetworkUpdate_ = true;
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::OnResourceAssignedToConvoy
// Created: NLD 2008-08-01
// -----------------------------------------------------------------------------
void SupplyConsign::OnResourceRemovedFromConvoy( const PHY_DotationCategory& dotationCategory, double quantity )
{
    for( T_RecipientRequests::const_iterator data = requestsQueued_.begin(); data != requestsQueued_.end() && quantity > 0; ++data )
    {
        const T_Requests& requests = data->second;
        T_Requests::const_iterator it = requests.find( &dotationCategory );
        if( it != requests.end() )
            quantity -= it->second->RemoveFromConvoy( quantity );
    }
    assert( quantity == 0 );
    requestsNeedNetworkUpdate_ = true;
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::OnConvoyEndMission
// Created: NLD 2008-08-01
// -----------------------------------------------------------------------------
void SupplyConsign::OnConvoyEndMission()
{
    SetState( eFinished );
}

// =============================================================================
// Network
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyConsign::SendMsgCreation
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void SupplyConsign::SendMsgCreation() const
{
    client::LogSupplyHandlingCreation msg;
    msg().mutable_request()->set_id( id_ );
    msg().set_tick( creationTick_ );
    supplier_->Serialize( *msg().mutable_supplier() );
    convoy_->GetTransportersProvider().Serialize( *msg().mutable_transporters_provider() );
    msg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::SendFullState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void SupplyConsign::SendFullState() const
{
    SendMsgCreation();

    client::LogSupplyHandlingUpdate msg;
    msg().mutable_request()->set_id( id_ );
    convoy_->Serialize( *msg().mutable_convoyer() );
    msg().set_state( (sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus)state_ );
    msg().set_current_state_end_tick( currentStateEndTimeStep_ );
    BOOST_FOREACH( const T_RecipientRequests::value_type& recipientRequest, requestsQueued_ )
    {
        sword::SupplyRecipientResourcesRequest* req = msg().mutable_requests()->add_requests();
        recipientRequest.first->Serialize( *req->mutable_recipient() );
        BOOST_FOREACH( const T_Requests::value_type& requestData, recipientRequest.second )
            requestData.second->Serialize( *req->add_resources() );
    }
    msg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::SendChangedState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void SupplyConsign::SendChangedState() const
{
    if( !needNetworkUpdate_ && !requestsNeedNetworkUpdate_ )
        return;

    client::LogSupplyHandlingUpdate msg;
    msg().mutable_request()->set_id( id_ );
    convoy_->Serialize( *msg().mutable_convoyer() );
    msg().set_state( (sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus)state_ );
    msg().set_current_state_end_tick( currentStateEndTimeStep_ );
    if( requestsNeedNetworkUpdate_ )
    {
        msg().mutable_requests();
        BOOST_FOREACH( const T_RecipientRequests::value_type& recipientRequest, requestsQueued_ )
        {
            sword::SupplyRecipientResourcesRequest* req = msg().mutable_requests()->add_requests();
            recipientRequest.first->Serialize( *req->mutable_recipient() );
            BOOST_FOREACH( const T_Requests::value_type& requestData, recipientRequest.second )
                requestData.second->Serialize( *req->add_resources() );
        }
    }
    msg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::SendMsgDestruction
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void SupplyConsign::SendMsgDestruction() const
{
    client::LogSupplyHandlingDestruction msg;
    msg().mutable_request()->set_id( id_ );
    msg.Send( NET_Publisher_ABC::Publisher() );
}


// =============================================================================
// TEST
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyConsign::WillGoTo
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
bool SupplyConsign::WillGoTo( const MIL_AutomateLOG& destination ) const
{
    enum 
    {
        eCheckSupplier = 0x01,
        eCheckRecipients = 0x02,
        eCheckTransportersProvider = 0x04
    };
    unsigned check = 0;
    switch( state_ )
    {
        case eConvoyWaitingForTransporters  : check = eCheckSupplier | eCheckRecipients | eCheckTransportersProvider; break;
        case eConvoySetup                   : check = eCheckSupplier | eCheckRecipients | eCheckTransportersProvider; break;
        case eConvoyGoingToLoadingPoint     : check = eCheckSupplier | eCheckRecipients | eCheckTransportersProvider; break;
        case eConvoyLoading                 : check = eCheckRecipients | eCheckTransportersProvider; break;
        case eConvoyGoingToUnloadingPoint   : check = eCheckRecipients | eCheckTransportersProvider; break;
        case eConvoyUnloading               : check = eCheckRecipients | eCheckTransportersProvider; break;
        case eConvoyGoingBackToFormingPoint : check = eCheckTransportersProvider; break;
        case eFinished                      : break;
    }

    if( (check & eCheckSupplier) && supplier_->BelongsToLogisticBase( destination ) )
        return true;

    if( (check & eCheckTransportersProvider) && convoy_->GetTransportersProvider().BelongsToLogisticBase( destination ) )
        return true;

    if( check & eCheckRecipients )
    {
        BOOST_FOREACH( const T_RecipientRequests::value_type& data, requestsQueued_ )
        {
            if( data.first->BelongsToLogisticBase( destination ) )
                return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::IsAt
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
bool SupplyConsign::IsAt( const MIL_AutomateLOG& destination ) const
{
    if( state_ == eConvoyLoading )
        return supplier_->BelongsToLogisticBase( destination );
    else if( state_ == eConvoyUnloading )
    {
        assert( currentRecipient_ );
        return currentRecipient_->BelongsToLogisticBase( destination );
    }
    else if( state_ == eFinished )
        return convoy_->GetTransportersProvider().BelongsToLogisticBase( destination );
    return false;
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::GetConvoy
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
boost::shared_ptr< SupplyConvoy_ABC > SupplyConsign::GetConvoy() const
{
    return convoy_;
}

// -----------------------------------------------------------------------------
// Name: template< class Archive > void SupplyConsign::load
// Created: LDC 2013-01-15
// -----------------------------------------------------------------------------
template< class Archive > void SupplyConsign::load( Archive& archive, const unsigned int )
{
    archive >> boost::serialization::base_object< SupplyConvoyEventsObserver_ABC >( *this );
    archive >> boost::serialization::base_object< SupplyConsign_ABC >( *this );
    archive >> id_;
    archive >> creationTick_;
    archive >> supplier_;
    archive >> provider_;
    archive >> state_;
    archive >> currentStateEndTimeStep_;
    archive >> convoy_;
    size_t resourcesSize;
    archive >> resourcesSize;
    for( size_t i = 0; i < resourcesSize; ++i )
    {
        int dotationId;
        archive >> dotationId;
        double quantity;
        archive >> quantity;
        resources_[ PHY_DotationType::FindDotationCategory( dotationId ) ] = quantity;
    }
    archive >> resourcesSize;
    for( size_t i = 0; i < resourcesSize; ++i ) //std::deque< std::pair< SupplyRecipient_ABC*, T_Requests > > 
    {
        SupplyRecipient_ABC* recipient;
        archive >> recipient;
        size_t requestsSize;
        archive >> requestsSize;
        T_Requests requestMap;
        for( size_t j = 0; j < requestsSize; ++j ) //std::map< const PHY_DotationCategory*, boost::shared_ptr< SupplyRequest_ABC > >
        { 
            int dotationId;
            archive >> dotationId;
            boost::shared_ptr< SupplyRequest_ABC > request;
            archive >> request;
            requestMap[ PHY_DotationType::FindDotationCategory( dotationId ) ] = request;
        }
        requestsQueued_.push_back( std::pair< SupplyRecipient_ABC*, T_Requests >( recipient, requestMap ) );
    }
    archive >> currentRecipient_;
    archive >> needNetworkUpdate_;
    archive >> requestsNeedNetworkUpdate_;
}

// -----------------------------------------------------------------------------
// Name: template< class Archive > void SupplyConsign::save
// Created: LDC 2013-01-15
// -----------------------------------------------------------------------------
template< class Archive > void SupplyConsign::save( Archive& archive, const unsigned int ) const
{
    archive << boost::serialization::base_object< SupplyConvoyEventsObserver_ABC >( *this );
    archive << boost::serialization::base_object< SupplyConsign_ABC >( *this );
    archive << id_;
    archive << creationTick_;
    archive << supplier_;
    archive << provider_;
    archive << state_;
    archive << currentStateEndTimeStep_;
    archive << convoy_;
    size_t size = resources_.size();
    archive << size;
    for( auto it = resources_.begin(); it != resources_.end(); ++it )
    {
        int dotation = it->first->GetMosID();
        archive << dotation;
        archive << it->second;
    }
    size = requestsQueued_.size();
    archive << size;
    for( auto it = requestsQueued_.begin(); it != requestsQueued_.end(); ++it )
    {
        archive << it->first;
        size_t requestsSize = it->second.size();
        archive << requestsSize;
        for( auto requestsIt = it->second.begin(); requestsIt != it->second.end(); ++requestsIt )
        {
            int dotation = requestsIt->first->GetMosID();
            archive << dotation;
            archive << requestsIt->second;
        }
    }
    archive << currentRecipient_;
    archive << needNetworkUpdate_;
    archive << requestsNeedNetworkUpdate_;
}

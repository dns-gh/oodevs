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
#include "SupplyRequest_ABC.h"
#include "SupplyConvoy_ABC.h"
#include "SupplyConvoyFactory_ABC.h"
#include "SupplyRecipient_ABC.h"
#include "SupplyRequestParameters_ABC.h"
#include "SupplySupplier_ABC.h"
#include "Checkpoints/MIL_CheckPointInArchive.h"
#include "Checkpoints/MIL_CheckPointOutArchive.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "MIL_Time_ABC.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "Tools/MIL_IDManager.h"
#include <boost/foreach.hpp>
#include <boost/serialization/deque.hpp>
#include <boost/serialization/export.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( logistic::SupplyConsign )

using namespace logistic;

namespace
{
    MIL_IDManager idManager;
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign constructor
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplyConsign::SupplyConsign( SupplySupplier_ABC& supplier, SupplyRequestParameters_ABC& parameters )
    : id_                       ( idManager.GetId() )
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
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::AddRequest
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyConsign::AddRequest( SupplyRecipient_ABC& recipient, boost::shared_ptr< SupplyRequest_ABC > request )
{
    T_Requests* requests = 0;
    BOOST_FOREACH( auto& data, requestsQueued_ )
        if( data.first == &recipient )
            requests = &data.second;
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
    BOOST_FOREACH( const auto& recipient, requestsQueued_ )
        BOOST_FOREACH( const auto& data, recipient.second )
        {
            const boost::shared_ptr< SupplyRequest_ABC >& request = data.second;
            request->ReserveStock();
            resources_[ &request->GetDotationCategory() ] += request->GetGrantedQuantity();
            if( !provider_ )
                provider_ = const_cast< MIL_Agent_ABC* >( request->GetProvider() );
        }
    BOOST_FOREACH( const auto& data, requestsQueued_ )
        data.first->OnSupplyScheduled( shared_from_this() );
    supplier_->SupplyHandleRequest( shared_from_this() );
    requestsNeedNetworkUpdate_ = true;
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::UpdateTimer
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyConsign::UpdateTimer( unsigned timeRemaining )
{
    unsigned tmp = std::numeric_limits< unsigned >::max();
    if( timeRemaining != std::numeric_limits< unsigned >::max() )
        tmp = MIL_Time_ABC::GetTime().GetCurrentTimeStep() + timeRemaining;
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
    BOOST_FOREACH( const auto& data, requestsQueued_.front().second )
    {
        boost::shared_ptr< SupplyRequest_ABC > request = data.second;
        double quantitySupplied = request->Supply();
        convoy_->Supply( *currentRecipient_, request->GetDotationCategory(), quantitySupplied );
    }
    requestsQueued_.pop_front();
    requestsNeedNetworkUpdate_ = true;
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::UpdateRequestsIfUnitDestroyed
// Created: JSR 2013-03-05
// -----------------------------------------------------------------------------
void SupplyConsign::UpdateRequestsIfUnitDestroyed()
{
    bool resetConsign = false;
    for( auto it = requestsQueued_.begin(); it != requestsQueued_.end(); )
    {
        bool hasRequesterDestroyed = false;
        for( auto request = it->second.begin(); request != it->second.end(); )
        {
            if( request->second->HasRequesterDestroyed() )
            {
                request = it->second.erase( request );
                currentRecipient_ = GetCurrentSupplyRecipient();
                hasRequesterDestroyed = true;
                requestsNeedNetworkUpdate_ = true;
            }
            else
                ++request;
        }
        if( hasRequesterDestroyed && it->second.empty() )
        {
            resetConsign = true;
            it->first->OnSupplyCanceled( shared_from_this() );
            it = requestsQueued_.erase( it );
        }
        else
            ++it;
    }
    if( resetConsign && requestsQueued_.empty() )
    {
        if( convoy_ )
        {
            convoy_->ResetConveyors( *this );
            convoy_->Finish();
        }
        SetState( eFinished );
        SendChangedState();
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::IsSupplying
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
bool SupplyConsign::IsSupplying( const PHY_DotationCategory& dotationCategory, const SupplyRecipient_ABC& recipient )
{
    BOOST_FOREACH( const auto& data, requestsQueued_ )
    {
        if( data.first == &recipient )
            return data.second.find( &dotationCategory ) != data.second.end();
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
// Name: SupplyConsign::ResetConsignsForConvoyPion
// Created: JSR 2013-02-06
// -----------------------------------------------------------------------------
bool SupplyConsign::ResetConsignsForConvoyPion( const MIL_AgentPion& pion )
{
    if( convoy_ && convoy_->HasConvoy( pion ) )
    {
        convoy_->ResetConveyors( *this );
        convoy_->Finish( convoy_->IsFinished() );
        SetState( state_ == eConvoyGoingBackToFormingPoint ? eFinished : eConvoyWaitingForTransporters );
        SendChangedState();
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::ResetConsignsForProvider
// Created: JSR 2013-02-11
// -----------------------------------------------------------------------------
bool SupplyConsign::ResetConsignsForProvider( const MIL_Agent_ABC& pion )
{
    if( provider_ == &pion )
    {
        ResetConsign();
        return true;
    }
    return false;
}


// -----------------------------------------------------------------------------
// Name: SupplyConsign::ResetConsign
// Created: JSR 2013-02-14
// -----------------------------------------------------------------------------
void SupplyConsign::ResetConsign()
{
    if( convoy_ )
    {
        convoy_->ResetConveyors( *this );
        convoy_->Finish();
    }
    BOOST_FOREACH( const auto& data, requestsQueued_ )
        data.first->OnSupplyCanceled( shared_from_this() );
    requestsQueued_.clear();
    SetState( eFinished );
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::GrantsNothing
// Created: LDC 2013-02-28
// -----------------------------------------------------------------------------
bool SupplyConsign::GrantsNothing() const
{
    for( auto it = resources_.begin(); it != resources_.end(); ++it )
        if( it->second != 0. )
            return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::SetState
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

// -----------------------------------------------------------------------------
// Name: SupplyConsign::IsActionDone
// Created: NLD 2005-12-14
// -----------------------------------------------------------------------------
bool SupplyConsign::IsActionDone( unsigned timeRemaining )
{
    UpdateTimer( timeRemaining );
    return currentStateEndTimeStep_ != std::numeric_limits< unsigned >::max() && MIL_Time_ABC::GetTime().GetCurrentTimeStep() >= currentStateEndTimeStep_;
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
    if( convoy_->IsImpossible() )
        ResetConsign();
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
    UpdateRequestsIfUnitDestroyed();
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

// -----------------------------------------------------------------------------
// Name: SupplyConsign::OnAllResourcesAssignedToConvoy
// Created: NLD 2008-08-01
// -----------------------------------------------------------------------------
void SupplyConsign::OnAllResourcesAssignedToConvoy()
{
    BOOST_FOREACH( const auto& it1, requestsQueued_ )
        BOOST_FOREACH( const auto& it2, it1.second )
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
    requestsNeedNetworkUpdate_ = true;
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::OnConvoyEndMission
// Created: NLD 2008-08-01
// -----------------------------------------------------------------------------
void SupplyConsign::OnConvoyEndMission()
{
    ResetConsign();
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::SendMsgCreation
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void SupplyConsign::SendMsgCreation() const
{
    client::LogSupplyHandlingCreation msg;
    msg().mutable_request()->set_id( id_ );
    msg().set_tick( MIL_Time_ABC::GetTime().GetCurrentTimeStep() ); //$$$ Huge shit
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
    BOOST_FOREACH( const auto& recipient, requestsQueued_ )
    {
        sword::SupplyRecipientResourcesRequest* req = msg().mutable_requests()->add_requests();
        recipient.first->Serialize( *req->mutable_recipient() );
        BOOST_FOREACH( const auto& data, recipient.second )
            data.second->Serialize( *req->add_resources() );
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
        BOOST_FOREACH( const auto& recipient, requestsQueued_ )
        {
            sword::SupplyRecipientResourcesRequest* req = msg().mutable_requests()->add_requests();
            recipient.first->Serialize( *req->mutable_recipient() );
            BOOST_FOREACH( const auto& data, recipient.second )
                data.second->Serialize( *req->add_resources() );
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
        BOOST_FOREACH( const auto& data, requestsQueued_ )
            if( data.first->BelongsToLogisticBase( destination ) )
                return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::WillGoTo
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
bool SupplyConsign::IsAt( const MIL_AutomateLOG& destination ) const
{
    if( state_ == eConvoyLoading )
        return supplier_->BelongsToLogisticBase( destination );
    if( state_ == eConvoyUnloading )
    {
        assert( currentRecipient_ );
        return currentRecipient_->BelongsToLogisticBase( destination );
    }
    if( state_ == eFinished )
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
// Name: SupplyConsign::load
// Created: LDC 2013-01-15
// -----------------------------------------------------------------------------
void SupplyConsign::load( MIL_CheckPointInArchive& archive, const unsigned int )
{
    size_t resourcesSize;
    archive >> boost::serialization::base_object< SupplyConvoyEventsObserver_ABC >( *this );
    archive >> boost::serialization::base_object< SupplyConsign_ABC >( *this );
    archive >> id_;
    archive >> supplier_;
    archive >> provider_;
    archive >> state_;
    archive >> currentStateEndTimeStep_;
    archive >> convoy_;
    archive >> resourcesSize;
    for( size_t i = 0; i < resourcesSize; ++i )
    {
        int dotationId;
        double quantity;
        archive >> dotationId;
        archive >> quantity;
        resources_[ PHY_DotationType::FindDotationCategory( dotationId ) ] = quantity;
    }
    archive >> resourcesSize;
    for( size_t i = 0; i < resourcesSize; ++i )
    {
        SupplyRecipient_ABC* recipient;
        size_t requestsSize;
        T_Requests requestMap;
        archive >> recipient;
        archive >> requestsSize;
        for( size_t j = 0; j < requestsSize; ++j )
        {
            int dotationId;
            boost::shared_ptr< SupplyRequest_ABC > request;
            archive >> dotationId;
            archive >> request;
            requestMap[ PHY_DotationType::FindDotationCategory( dotationId ) ] = request;
        }
        requestsQueued_.push_back( std::make_pair( recipient, requestMap ) );
    }
    archive >> currentRecipient_;
    archive >> needNetworkUpdate_;
    archive >> requestsNeedNetworkUpdate_;
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::save
// Created: LDC 2013-01-15
// -----------------------------------------------------------------------------
void SupplyConsign::save( MIL_CheckPointOutArchive& archive, const unsigned int ) const
{
    archive << boost::serialization::base_object< SupplyConvoyEventsObserver_ABC >( *this );
    archive << boost::serialization::base_object< SupplyConsign_ABC >( *this );
    archive << id_;
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
        const size_t requestsSize = it->second.size();
        archive << it->first;
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

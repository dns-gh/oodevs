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
#include "MIL_AgentServer.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <boost/foreach.hpp>

using namespace logistic;

MIL_IDManager SupplyConsign::idManager_;

// -----------------------------------------------------------------------------
// Name: SupplyConsign constructor
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplyConsign::SupplyConsign( SupplySupplier_ABC& supplier, SupplyRequestParameters_ABC& parameters )
    : id_                       ( idManager_.GetFreeId() )
    , creationTick_             ( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() ) //$$$ Huge shit
    , supplier_                 ( supplier )
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
// Name: SupplyConsign destructor
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplyConsign::~SupplyConsign()
{
    convoy_->Finish();
    SendMsgDestruction();
    BOOST_FOREACH( const T_RecipientRequests::value_type& data, requestsQueued_ )
        data.first->OnSupplyCanceled();
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
            resources_[ &request->GetDotationCategory() ] += request->GetGrantedValue();
        }
    supplier_.SupplyHandleRequest( shared_from_this() );
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
// Name: SupplyConsign::GetCurrentSupplyRecipient
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyConsign::SupplyCurrentRecipient()
{
    assert( !requestsQueued_.empty() );
    assert( currentRecipient_ == requestsQueued_.front().first );

    BOOST_FOREACH( T_Requests::value_type& data, requestsQueued_.front().second )
    {
        boost::shared_ptr< SupplyRequest_ABC > request = data.second;
        double quantitySupplied = request->Supply();
        convoy_->Supply( *currentRecipient_, request->GetDotationCategory(), quantitySupplied );
    }
    currentRecipient_->OnSupplyDone();
    requestsQueued_.pop_front();
    requestsNeedNetworkUpdate_ = true;
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::GetCurrentSupplyRecipient
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
bool SupplyConsign::IsSupplying( const PHY_DotationCategory& dotationCategory ) const
{
    return resources_.find( &dotationCategory ) != resources_.end();
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
// Name: SupplyConsign::GetCurrentSupplyRecipient
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
// Name: SupplyConsign::EnterStateConvoyForming
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
// Name: SupplyConsign::Update
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool SupplyConsign::Update()
{
    switch( state_ )
    {
        case eConvoyWaitingForTransporters  : if( IsActionDone( convoy_->ReserveTransporters( resources_ ) ) )  SetState( eConvoySetup );               break;
        case eConvoySetup                   : if( IsActionDone( convoy_->Setup() ) )                            SetState( eConvoyGoingToLoadingPoint ); break;
        case eConvoyGoingToLoadingPoint     : if( IsActionDone( convoy_->MoveToSupplier() ) )                   SetState( eConvoyLoading );             break;
        case eConvoyLoading                 : if( IsActionDone( convoy_->Load() ) )                             SupplyAndProceedWithNextRecipient();    break;
        case eConvoyGoingToUnloadingPoint   : if( IsActionDone( convoy_->MoveToSupplyRecipient( ) ) )           SetState( eConvoyUnloading );           break;
        case eConvoyUnloading               : if( IsActionDone( convoy_->Unload() ) )                           SupplyAndProceedWithNextRecipient();    break;
        case eConvoyGoingBackToFormingPoint : if( IsActionDone( convoy_->MoveToTransportersProvider() ) )       SetState( eFinished );                  break;
        case eFinished                      : convoy_->Finish();                                                                                        break;
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
    supplier_.Serialize( *msg().mutable_supplier() );
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


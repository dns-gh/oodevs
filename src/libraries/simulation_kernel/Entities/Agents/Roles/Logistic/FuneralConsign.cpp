// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "FuneralConsign.h"
#include "FuneralRequest_ABC.h"
#include "FuneralPackagingResource.h"
#include "SupplyConsign_ABC.h"
#include "SupplyConvoy_ABC.h"
#include "FuneralConfig.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "MIL_AgentServer.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <tools/iterator.h>

using namespace logistic;

//$$$$ TMP

LogisticVirtualAction::LogisticVirtualAction()
    : currentActionId_( std::numeric_limits< unsigned >::max() )
    , timeRemainingForCurrentAction_( std::numeric_limits< unsigned >::max() )
{
}

LogisticVirtualAction::~LogisticVirtualAction()
{
}


unsigned LogisticVirtualAction::GetTimeRemaining( unsigned actionId, unsigned duration )
{
    if( currentActionId_ == actionId )
    {
        if( timeRemainingForCurrentAction_ > 0 ) 
            --timeRemainingForCurrentAction_;
        else
            currentActionId_ = std::numeric_limits< unsigned >::max();
    }
    else
    {
        currentActionId_ = actionId;
        timeRemainingForCurrentAction_ = duration;//(unsigned int)( timeComputer( static_cast< double >( conveyors_.size() ) ) );
    }
    return timeRemainingForCurrentAction_;
}
//$$$$ TMP

MIL_IDManager FuneralConsign::idManager_;

// -----------------------------------------------------------------------------
// Name: FuneralConsign constructor
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
FuneralConsign::FuneralConsign( boost::shared_ptr< FuneralRequest_ABC > request )
    : id_                     ( idManager_.GetFreeId() )
    , creationTick_           ( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() ) //$$$ Huge shit
    , request_                ( request )
    , handler_                ( 0 )
    , position_               ( request->GetPosition() )
    , packaging_              ( 0 )
    , state_                  ( eWaitingForHandling )
    , currentStateEndTimeStep_( std::numeric_limits< unsigned >::max() )
    , needNetworkUpdate_      ( true )
{
    request_->OnHandledByFuneral();
    SendMsgCreation();
}

// -----------------------------------------------------------------------------
// Name: FuneralConsign destructor
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
FuneralConsign::~FuneralConsign()
{
    request_->OnBackFromFuneral();
    SendMsgDestruction();
}

// =============================================================================
// Tools
// =============================================================================

// -----------------------------------------------------------------------------
// Name: FuneralConsign::UpdateTimer
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
void FuneralConsign::UpdateTimer( unsigned timeRemaining )
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
// Name: FuneralConsign::SetState
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void FuneralConsign::SetState( E_State newState )
{
    if( newState != state_ )
    {
        state_ = newState;
        UpdateTimer( std::numeric_limits< unsigned >::max() );
        needNetworkUpdate_ = true;
    }
}

// =============================================================================
// Operations
// =============================================================================

// =============================================================================
// FSM
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyConsign::IsActionDone
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
bool FuneralConsign::IsActionDone( unsigned timeRemaining )
{
    UpdateTimer( timeRemaining );
    return currentStateEndTimeStep_ != std::numeric_limits< unsigned >::max() && MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() >= currentStateEndTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: FuneralConsign::MoveTo
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
unsigned FuneralConsign::MoveTo( const MT_Vector2D& position )
{
    const unsigned travelTime = (unsigned)( 1.439 * position_.Distance( position ) / FuneralConfig::GetFakeTransporterSpeed() ); //$$$ Stupid formula
    if( travelTime <= 0 )
        position_ = position;
    else
    {
        MT_Vector2D tmp( position - position_ );
        position_ += tmp.Normalized() * tmp.Magnitude() / travelTime;
    }
    return travelTime;
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::DoWaitForHandling
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
void FuneralConsign::DoWaitForHandling()
{
    tools::Iterator< MIL_AutomateLOG& > it = request_->GetLogisticHierarchy().CreateSuperiorsIterator();
    while( it.HasMoreElements() )
    {
        FuneralHandler_ABC& handler = it.NextElement();
        if( handler.FuneralHandleConsign( shared_from_this() ) )
        {
            handler_ = &handler;
            SetState( eTransportingUnpackaged );
            return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::DoTransportUnpackaged
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
void FuneralConsign::DoTransportUnpackaged()
{
    assert( handler_ );
    if( IsActionDone( MoveTo( handler_->GetPosition() ) ) )
        SetState( eWaitingForPackaging );
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::DoWaitForPackaging
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
void FuneralConsign::DoWaitForPackaging()
{
    assert( handler_ );
    const FuneralPackagingResource* newPacking = handler_->FuneralGetNextPackagingResource( packaging_ );
    if( newPacking )
    {
        needNetworkUpdate_ = true;
        packaging_ = newPacking;
        SetState( ePackaging );
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::DoPackage
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
void FuneralConsign::DoPackage()
{
    assert( handler_ );
    assert( packaging_ );
    if( !IsActionDone( currentAction_.GetTimeRemaining( ePackaging, packaging_->GetProcessDuration() ) ) )
        return;

    DoTransitionAfterPackaging();
}

// -----------------------------------------------------------------------------
// Name: SupplyConsign::DoPackage
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
void FuneralConsign::DoTransitionAfterPackaging()
{
    assert( handler_ );
    if( handler_->GetLogisticHierarchy().HasSuperior() )
    {
        SetState( eWaitingForTransporter );
        handler_->AddSupplyConvoysObserver( *this );
    }
    else
        SetState( eFinished );
}

// -----------------------------------------------------------------------------
// Name: FuneralConsign::Update
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
bool FuneralConsign::Update()
{
    switch( state_ )
    {
        case eWaitingForHandling    : DoWaitForHandling(); break;
        case eTransportingUnpackaged: DoTransportUnpackaged(); break;
        case eWaitingForPackaging   : DoWaitForPackaging(); break;
        case ePackaging             : DoPackage(); break;
        case eWaitingForTransporter : break; // Managed by the OnConvoyLeaving / Arriving events
        case eTransportingPackaged  : break; // Managed by the OnConvoyLeaving / Arriving events
        case eFinished              : break;
        default:
            assert( false );
    }
    return state_ == eFinished;
}

// -----------------------------------------------------------------------------
// Name: FuneralConsign::Update
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void FuneralConsign::Cancel()
{
    SetState( eFinished );
    convoy_.reset();
    handler_ = 0;
}

// =============================================================================
// Events
// =============================================================================

// -----------------------------------------------------------------------------
// Name: FuneralConsign::OnSupplyConvoyLeaving
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void FuneralConsign::OnSupplyConvoyLeaving( boost::shared_ptr< const SupplyConsign_ABC > supplyConsign )
{
    if( state_ != eWaitingForTransporter )
        return;
    
    assert( !convoy_ );
    assert( handler_ );

    tools::Iterator< MIL_AutomateLOG& > it = handler_->GetLogisticHierarchy().CreateSuperiorsIterator();
    while( it.HasMoreElements() )
    {
        MIL_AutomateLOG& logisticBase = it.NextElement();
        if( supplyConsign->WillGoTo( logisticBase ) )
        {
            //$$$ contraintes sur poids, nature & co
//            consign.AddPayload( *this );
            handler_->RemoveSupplyConvoysObserver( *this );
            handler_ = &logisticBase;
            handler_->AddSupplyConvoysObserver( *this );
            convoy_ = supplyConsign->GetConvoy();
            needNetworkUpdate_ = true;
            SetState( eTransportingPackaged );
            return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: FuneralConsign::OnSupplyConvoyArriving
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void FuneralConsign::OnSupplyConvoyArriving( boost::shared_ptr< const SupplyConsign_ABC > supplyConsign )
{
    if( state_ != eTransportingPackaged || supplyConsign->GetConvoy() != convoy_ )
        return;

    handler_->RemoveSupplyConvoysObserver( *this );
    convoy_.reset();
    if( !packaging_->IsTerminal() )
        SetState( eWaitingForPackaging );
    else
        DoTransitionAfterPackaging();
}

// =============================================================================
// Network
// =============================================================================

// -----------------------------------------------------------------------------
// Name: FuneralConsign::SendMsgCreation
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void FuneralConsign::SendMsgCreation() const
{
    client::LogFuneralHandlingCreation msg;

    msg().mutable_request()->set_id( id_ );
    msg().set_tick( creationTick_ );
    msg().set_rank( request_->GetRank() );
    request_->Serialize( *msg().mutable_unit() );
    msg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: FuneralConsign::SendFullState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void FuneralConsign::SendFullState( unsigned int context ) const
{
    SendMsgCreation();

    client::LogFuneralHandlingUpdate msg;
    msg().mutable_request()->set_id( id_ );
    msg().set_state( (sword::LogFuneralHandlingUpdate::EnumLogFuneralHandlingStatus)state_ );
    msg().set_current_state_end_tick( currentStateEndTimeStep_ );
    if( handler_ )
        handler_->Serialize( *msg().mutable_handling_unit() );
    if( convoy_ )
        convoy_->Serialize( *msg().mutable_convoying_unit() );
    if( packaging_ )
        msg().mutable_packaging_resource()->set_id( packaging_->GetDotationCategory().GetMosID() );
    msg.Send( NET_Publisher_ABC::Publisher(), context );
}

// -----------------------------------------------------------------------------
// Name: FuneralConsign::SendChangedState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void FuneralConsign::SendChangedState() const
{
    if( !needNetworkUpdate_ )
        return;

    client::LogFuneralHandlingUpdate msg;
    msg().mutable_request()->set_id( id_ );
    msg().set_state( (sword::LogFuneralHandlingUpdate::EnumLogFuneralHandlingStatus)state_ );
    msg().set_current_state_end_tick( currentStateEndTimeStep_ );
    handler_ ? handler_->Serialize( *msg().mutable_handling_unit() ) : msg().mutable_handling_unit()->mutable_automat()->set_id( 0 );
    convoy_ ? convoy_->Serialize( *msg().mutable_convoying_unit() ) : msg().mutable_convoying_unit()->set_id( 0 );
    if( packaging_ )
        msg().mutable_packaging_resource()->set_id( packaging_->GetDotationCategory().GetMosID() );
    msg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: FuneralConsign::SendMsgDestruction
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void FuneralConsign::SendMsgDestruction() const
{
    client::LogFuneralHandlingDestruction msg;
    msg().mutable_request()->set_id( id_ );
    msg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: FuneralConsign::Clean
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void FuneralConsign::Clean()
{
    needNetworkUpdate_ = false;
}

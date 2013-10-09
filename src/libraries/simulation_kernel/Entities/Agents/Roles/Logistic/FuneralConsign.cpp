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
#include "FuneralPackagingResource.h"
#include "FuneralConfig.h"
#include "MIL_Time_ABC.h"
#include "SupplyConsign_ABC.h"
#include "SupplyConvoy_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Humans/Human_ABC.h"
#include "Entities/Agents/Units/Humans/PHY_HumanRank.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Orders/MIL_DecisionalReport.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "Tools/MIL_IDManager.h"
#include <tools/iterator.h>

using namespace logistic;

LogisticVirtualAction::LogisticVirtualAction()
    : currentActionId_( std::numeric_limits< unsigned >::max() )
    , timeRemainingForCurrentAction_( std::numeric_limits< unsigned >::max() )
{
    // NOTHING
}

unsigned LogisticVirtualAction::GetTimeRemaining( unsigned actionId, unsigned duration )
{
    if( currentActionId_ == actionId )
    {
        if( timeRemainingForCurrentAction_ > 0 ) 
            --timeRemainingForCurrentAction_;
        if( timeRemainingForCurrentAction_ == 0 ) 
            currentActionId_ = std::numeric_limits< unsigned >::max();
    }
    else
    {
        currentActionId_ = actionId;
        timeRemainingForCurrentAction_ = duration;
    }
    return timeRemainingForCurrentAction_;
}

namespace
{
    MIL_IDManager idManager;
}

// -----------------------------------------------------------------------------
// Name: FuneralConsign constructor
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
FuneralConsign::FuneralConsign( Human_ABC& human )
    : id_                     ( idManager.GetId() )
    , human_                  ( human )
    , handler_                ( 0 )
    , position_               ( human.GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition() )
    , packaging_              ( 0 )
    , state_                  ( eWaitingForHandling )
    , currentStateEndTimeStep_( std::numeric_limits< unsigned >::max() )
    , needNetworkUpdate_      ( true )
{
    human_.NotifyHandledByFuneral();
    SendMsgCreation();
}

// -----------------------------------------------------------------------------
// Name: FuneralConsign destructor
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
FuneralConsign::~FuneralConsign()
{
    human_.NotifyBackFromFuneral();
    if( handler_ )
        handler_->RemoveSupplyConvoysObserver( *this );
    SendMsgDestruction();
}

// -----------------------------------------------------------------------------
// Name: FuneralConsign::UpdateTimer
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
void FuneralConsign::UpdateTimer( unsigned timeRemaining )
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

// -----------------------------------------------------------------------------
// Name: FuneralConsign::IsActionDone
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
bool FuneralConsign::IsActionDone( unsigned timeRemaining )
{
    UpdateTimer( timeRemaining );
    return currentStateEndTimeStep_ != std::numeric_limits< unsigned >::max() && MIL_Time_ABC::GetTime().GetCurrentTimeStep() >= currentStateEndTimeStep_;
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
// Name: FuneralConsign::DoWaitForHandling
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
void FuneralConsign::DoWaitForHandling()
{
    tools::Iterator< MIL_AutomateLOG& > it = human_.GetPion().GetLogisticHierarchy().CreateSuperiorsIterator();
    if( it.HasMoreElements() )
    {
        FuneralHandler_ABC& handler = it.NextElement();
        if( handler_ )
            handler_->RemoveSupplyConvoysObserver( *this );
        handler_ = &handler;
        SetState( eTransportingUnpackaged );
    }
}

// -----------------------------------------------------------------------------
// Name: FuneralConsign::DoTransportUnpackaged
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
void FuneralConsign::DoTransportUnpackaged()
{
    assert( handler_ );
    if( IsActionDone( MoveTo( handler_->GetPosition() ) ) )
        SetState( eWaitingForPackaging );
}

// -----------------------------------------------------------------------------
// Name: FuneralConsign::DoWaitForPackaging
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
// Name: FuneralConsign::DoPackage
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
// Name: FuneralConsign::DoPackage
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
// Name: FuneralConsign::IsFinished
// Created: JSR 2013-02-19
// -----------------------------------------------------------------------------
bool FuneralConsign::IsFinished() const
{
    return convoy_ && ( convoy_->IsConvoyDestroyed() || convoy_->IsFinished() );
}

// -----------------------------------------------------------------------------
// Name: FuneralConsign::OnSupplyConvoyLeaving
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void FuneralConsign::OnSupplyConvoyLeaving( const boost::shared_ptr< const SupplyConsign_ABC >& consign )
{
    if( state_ != eWaitingForTransporter )
        return;
    assert( !convoy_ );
    assert( handler_ );

    tools::Iterator< MIL_AutomateLOG& > it = handler_->GetLogisticHierarchy().CreateSuperiorsIterator();
    while( it.HasMoreElements() )
    {
        MIL_AutomateLOG& logisticBase = it.NextElement();
        if( consign->WillGoTo( logisticBase ) && ( !packaging_ || consign->GetConvoy()->CanTransport( packaging_->GetDotationCategory() ) ) )
        {
            handler_->RemoveSupplyConvoysObserver( *this );
            handler_ = &logisticBase;
            handler_->AddSupplyConvoysObserver( *this );
            convoy_ = consign->GetConvoy();
            needNetworkUpdate_ = true;
            const MIL_Agent_ABC* reporter = convoy_ ? convoy_->GetReporter() : 0;
            if( reporter )
                MIL_Report::PostEvent< MIL_Agent_ABC >( *reporter, report::eRC_CorpseTransported, packaging_->GetDotationCategory() );
            SetState( eTransportingPackaged );
            return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: FuneralConsign::OnSupplyConvoyArriving
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void FuneralConsign::OnSupplyConvoyArriving( const boost::shared_ptr< const SupplyConsign_ABC >& consign )
{
    if( state_ != eTransportingPackaged || consign->GetConvoy() != convoy_ )
        return;
    handler_->RemoveSupplyConvoysObserver( *this );
    convoy_.reset();
    if( !packaging_->IsTerminal() )
        SetState( eWaitingForPackaging );
    else
        DoTransitionAfterPackaging();
}

// -----------------------------------------------------------------------------
// Name: FuneralConsign::SendMsgCreation
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void FuneralConsign::SendMsgCreation() const
{
    client::LogFuneralHandlingCreation msg;
    msg().mutable_request()->set_id( id_ );
    msg().set_tick( MIL_Time_ABC::GetTime().GetCurrentTimeStep() ); //$$$ Huge shit
    msg().set_rank( human_.GetRank().GetAsnID() );
    msg().mutable_unit()->set_id( human_.GetPion().GetID() );
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

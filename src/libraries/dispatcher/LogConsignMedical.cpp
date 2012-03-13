// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "LogConsignMedical.h"
#include "Model.h"
#include "Agent.h"
#include "protocol/ClientPublisher_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogConsignMedical constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
LogConsignMedical::LogConsignMedical( const Model& model, const sword::LogMedicalHandlingCreation& msg )
    : SimpleEntity< >     ( msg.request().id() )
    , model_              ( model )
    , agent_              ( model.Agents().Get( msg.unit().id() ) )
    , nTickCreation_      ( msg.tick() )
    , pTreatingAgent_     ( 0 )
    , nRank_              ( msg.rank() )
    , nWound_             ( msg.wound() )
    , bMentalDiseased_    ( msg.mental_wound() != 0 )
    , bContaminated_      ( msg.nbc_contaminated() != 0 )
    , nState_             ( sword::LogMedicalHandlingUpdate::collection_ambulance_unloading )
    , currentStateEndTick_( std::numeric_limits< unsigned long >::max() )
    , bDiagnosed_         ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogConsignMedical destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
LogConsignMedical::~LogConsignMedical()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogConsignMedical::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void LogConsignMedical::Update( const sword::LogMedicalHandlingUpdate& msg )
{
    if( msg.has_provider() )
        pTreatingAgent_ = ( msg.provider().id() == 0 ) ? 0 : &model_.Agents().Get( msg.provider().id() );
    if( msg.has_wound() )
        nWound_ = msg.wound();
    if( msg.has_mental_wound() )
        bMentalDiseased_ = msg.mental_wound() != 0;
    if( msg.has_nbc_contaminated() )
        bContaminated_ = msg.nbc_contaminated() != 0;
    if( msg.has_state() )
        nState_ = msg.state();
    if( msg.has_current_state_end_tick() )
        currentStateEndTick_ = msg.current_state_end_tick();
    if( msg.has_diagnosed() )
        bDiagnosed_ = msg.diagnosed() != 0;
}

// -----------------------------------------------------------------------------
// Name: LogConsignMedical::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void LogConsignMedical::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::LogMedicalHandlingCreation asn;

    asn().mutable_request()->set_id( GetId() );
    asn().mutable_unit()->set_id( agent_.GetId() );
    asn().set_tick( nTickCreation_ );
    asn().set_rank( nRank_ );
    asn().set_wound( nWound_ );
    asn().set_mental_wound( bMentalDiseased_ );
    asn().set_nbc_contaminated( bContaminated_ );

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: LogConsignMedical::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void LogConsignMedical::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::LogMedicalHandlingUpdate asn;
    asn().mutable_request()->set_id( GetId() );
    asn().mutable_unit()->set_id( agent_.GetId() );
    asn().mutable_provider()->set_id( pTreatingAgent_ ? pTreatingAgent_->GetId() : 0);
    asn().set_mental_wound( bMentalDiseased_);
    asn().set_wound( nWound_);
    asn().set_nbc_contaminated( bContaminated_);
    asn().set_diagnosed( bDiagnosed_);
    asn().set_state( nState_);
    asn().set_current_state_end_tick( currentStateEndTick_ );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: LogConsignMedical::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void LogConsignMedical::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::LogMedicalHandlingDestruction asn;
    asn().mutable_request()->set_id( GetId());
    asn().mutable_unit()->set_id( agent_.GetId());
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: LogConsignMedical::Accept
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void LogConsignMedical::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

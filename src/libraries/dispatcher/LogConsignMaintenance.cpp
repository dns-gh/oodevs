// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "LogConsignMaintenance.h"
#include "Model.h"
#include "protocol/ClientPublisher_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "Agent.h"
#include "protocol/ClientSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogConsignMaintenance constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
LogConsignMaintenance::LogConsignMaintenance( const Model& model, const MsgsSimToClient::MsgLogMaintenanceHandlingCreation& msg )
    : SimpleEntity< >   ( msg.id().id() )
    , model_            ( model )
    , agent_            ( model.Agents().Get( msg.unit().id() ) )
    , nTickCreation_    ( msg.tick_creation() )
    , nEquipmentType_   ( msg.equipement().id() )
    , nBreakdownType_   ( msg.breakdown().id() )
    , pTreatingAgent_   ( 0 )
    , nState_           ( Common::attente_disponibilite_pieces )
    , bDiagnosed_       ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogConsignMaintenance destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
LogConsignMaintenance::~LogConsignMaintenance()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogConsignMaintenance::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void LogConsignMaintenance::Update( const MsgsSimToClient::MsgLogMaintenanceHandlingUpdate& msg )
{
    if( msg.has_diagnostique_effectue() )
        bDiagnosed_ = msg.diagnostique_effectue() != 0;
    if( msg.has_etat() )
        nState_ = msg.etat();
    pTreatingAgent_ = ( msg.provider().id() == 0 )? 0 : &model_.Agents().Get( msg.provider().id() );
}

// -----------------------------------------------------------------------------
// Name: LogConsignMaintenance::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void LogConsignMaintenance::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::LogMaintenanceHandlingCreation message;

    message().mutable_id()->set_id( GetId() );
    message().mutable_unit()->set_id( agent_.GetId() );
    message().set_tick_creation ( nTickCreation_ );

    message().mutable_equipement()->set_id( nEquipmentType_ );
    message().mutable_breakdown()->set_id( nBreakdownType_ );

    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: LogConsignMaintenance::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void LogConsignMaintenance::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::LogMaintenanceHandlingUpdate message;

    message().mutable_id()->set_id( GetId() );
    message().mutable_unit()->set_id( agent_.GetId() );
    message().mutable_provider()->set_id( pTreatingAgent_ ? pTreatingAgent_->GetId() : 0 );
    message().set_etat( nState_ );
    message().set_diagnostique_effectue( bDiagnosed_ );

    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: LogConsignMaintenance::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void LogConsignMaintenance::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::LogMaintenanceHandlingDestruction message;
    message().mutable_id()->set_id( GetId() );
    message().mutable_unit()->set_id( agent_.GetId() );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: LogConsignMaintenance::Accept
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void LogConsignMaintenance::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

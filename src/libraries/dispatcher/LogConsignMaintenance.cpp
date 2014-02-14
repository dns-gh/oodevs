// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "LogConsignMaintenance.h"
#include "Model.h"
#include "Agent.h"
#include "protocol/ClientPublisher_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogConsignMaintenance constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
LogConsignMaintenance::LogConsignMaintenance( const Model& model, const sword::LogMaintenanceHandlingCreation& msg )
    : SimpleEntity<>      ( msg.request().id() )
    , model_              ( model )
    , agent_              ( model.Agents().Get( msg.unit().id() ) )
    , nTickCreation_      ( msg.tick() )
    , nEquipmentType_     ( msg.equipement().id() )
    , nBreakdownType_     ( msg.breakdown().id() )
    , providerId_         ( 0 )
    , nState_             ( sword::LogMaintenanceHandlingUpdate::waiting_for_parts )
    , currentStateEndTick_( std::numeric_limits< unsigned long >::max() )
    , bDiagnosed_         ( false )
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
void LogConsignMaintenance::Update( const sword::LogMaintenanceHandlingUpdate& msg )
{
    if( msg.has_diagnosed() )
        bDiagnosed_ = msg.diagnosed() != 0;
    if( msg.has_state() )
        nState_ = msg.state();
    if( msg.has_current_state_end_tick() )
        currentStateEndTick_ = msg.current_state_end_tick();
    else
        currentStateEndTick_ = std::numeric_limits< unsigned long >::max();
    providerId_ = msg.provider().id();
}

// -----------------------------------------------------------------------------
// Name: LogConsignMaintenance::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void LogConsignMaintenance::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::LogMaintenanceHandlingCreation message;
    message().mutable_request()->set_id( GetId() );
    message().mutable_unit()->set_id( agent_.GetId() );
    message().set_tick ( nTickCreation_ );
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
    message().mutable_request()->set_id( GetId() );
    message().mutable_unit()->set_id( agent_.GetId() );
    message().mutable_provider()->set_id( providerId_ );
    message().set_state( nState_ );
    message().set_diagnosed( bDiagnosed_ );
    if( currentStateEndTick_ != std::numeric_limits< unsigned long >::max() )
        message().set_current_state_end_tick( currentStateEndTick_ );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: LogConsignMaintenance::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void LogConsignMaintenance::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::LogMaintenanceHandlingDestruction message;
    message().mutable_request()->set_id( GetId() );
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

// -----------------------------------------------------------------------------
// Name: LogConsignMaintenance::IsObsoleteForUnit
// Created: JSR 2013-03-14
// -----------------------------------------------------------------------------
bool LogConsignMaintenance::IsObsoleteForUnit( unsigned int id ) const
{
    return agent_.GetId() == id || ( providerId_ == id );
}

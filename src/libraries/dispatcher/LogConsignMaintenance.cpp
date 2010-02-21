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
#include "ClientPublisher_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "Agent.h"
#include "protocol/clientsenders.h"

////using namespace Common;
//using namespace MsgsSimToClient;

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogConsignMaintenance constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
LogConsignMaintenance::LogConsignMaintenance( const Model& model, const MsgsSimToClient::MsgLogMaintenanceHandlingCreation& msg )
    : SimpleEntity< >   ( msg.oid_consigne() )
    , model_            ( model )
    , agent_            ( model.agents_.Get( msg.oid_pion() ) )
    , nTickCreation_    ( msg.tick_creation() )
    , nEquipmentType_   ( msg.type_equipement() )
    , nBreakdownType_   ( msg.type_panne() )
    , pTreatingAgent_   ( 0 )
    , nState_           ( Common::EnumLogMaintenanceHandlingStatus::attente_disponibilite_pieces )
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
    pTreatingAgent_ = ( msg.oid_pion_log_traitant() == 0 ) ? 0 : &model_.agents_.Get( msg.oid_pion_log_traitant() );
}

// -----------------------------------------------------------------------------
// Name: LogConsignMaintenance::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void LogConsignMaintenance::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::LogMaintenanceHandlingCreation asn;
    
    asn().set_oid_consigne  ( GetId() );
    asn().set_oid_pion      ( agent_.GetId() );
    asn().set_tick_creation ( nTickCreation_ );

    asn().set_type_equipement ( nEquipmentType_ );
    asn().set_type_panne      ( nBreakdownType_ );

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: LogConsignMaintenance::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void LogConsignMaintenance::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::LogMaintenanceHandlingUpdate asn;

    asn().set_oid_consigne( GetId() );
    asn().set_oid_pion( agent_.GetId() );

//    asn.set_diagnostique_effectuePresent( 1 );
//    asn.set_etatPresent( 1 );

    asn().set_oid_pion_log_traitant( pTreatingAgent_ ? pTreatingAgent_->GetId() : 0 );
    asn().set_etat( nState_ );
    asn().set_diagnostique_effectue( bDiagnosed_ );

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: LogConsignMaintenance::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void LogConsignMaintenance::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::LogMaintenanceHandlingDestruction asn;
    asn().set_oid_consigne( GetId() );
    asn().set_oid_pion( agent_.GetId() );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: LogConsignMaintenance::Accept
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void LogConsignMaintenance::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

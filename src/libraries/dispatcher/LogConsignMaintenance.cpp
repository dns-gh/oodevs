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
#include "Network_Def.h"
#include "ModelVisitor_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogConsignMaintenance constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
LogConsignMaintenance::LogConsignMaintenance( Model& model, const ASN1T_MsgLogMaintenanceTraitementEquipementCreation& msg )
    : model_            ( model )
    , nID_              ( msg.oid_consigne )
    , agent_            ( model.GetAgents().Get( msg.oid_pion ) )
    , nTickCreation_    ( msg.tick_creation )
    , nEquipmentType_   ( msg.type_equipement )
    , nBreakdownType_   ( msg.type_panne )
    , pTreatingAgent_   ( 0 )
    , nState_           ( EnumLogMaintenanceTraitementEtat::attente_disponibilite_pieces )
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

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: LogConsignMaintenance::Update
// Created: AGE 2007-04-16
// -----------------------------------------------------------------------------
void LogConsignMaintenance::Update( const ASN1T_MsgLogMaintenanceTraitementEquipementCreation& )
{
    FlagUpdate();
}

// -----------------------------------------------------------------------------
// Name: LogConsignMaintenance::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void LogConsignMaintenance::Update( const ASN1T_MsgLogMaintenanceTraitementEquipementUpdate& msg )
{
    if( msg.m.diagnostique_effectuePresent )
        bDiagnosed_ = msg.diagnostique_effectue;

    if( msg.m.etatPresent )
        nState_ = msg.etat;

    pTreatingAgent_ = ( msg.oid_pion_log_traitant == 0 ) ? 0 : &model_.GetAgents().Get( msg.oid_pion_log_traitant );
}

// -----------------------------------------------------------------------------
// Name: LogConsignMaintenance::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void LogConsignMaintenance::SendCreation( Publisher_ABC& publisher ) const
{
    AsnMsgInClientLogMaintenanceTraitementEquipementCreation asn;
    
    asn().oid_consigne  = nID_;
    asn().oid_pion      = agent_.GetID();
    asn().tick_creation = nTickCreation_;

    asn().type_equipement = nEquipmentType_;
    asn().type_panne      = nBreakdownType_;

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: LogConsignMaintenance::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void LogConsignMaintenance::SendFullUpdate( Publisher_ABC& publisher ) const
{
    AsnMsgInClientLogMaintenanceTraitementEquipementUpdate asn;

    asn().oid_consigne = nID_;
    asn().oid_pion     = agent_.GetID();

    asn().m.diagnostique_effectuePresent = 1;
    asn().m.etatPresent                  = 1;

    asn().oid_pion_log_traitant = pTreatingAgent_ ? pTreatingAgent_->GetID() : 0;
    asn().etat                  = nState_;
    asn().diagnostique_effectue = bDiagnosed_;

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: LogConsignMaintenance::CommitDestruction
// Created: AGE 2007-04-16
// -----------------------------------------------------------------------------
void LogConsignMaintenance::CommitDestruction()
{
    AsnMsgInClientLogMaintenanceTraitementEquipementDestruction asn;
    asn().oid_consigne = nID_;
    asn().oid_pion     = agent_.GetID();
    Send( asn );
}

// -----------------------------------------------------------------------------
// Name: LogConsignMaintenance::Accept
// Created: AGE 2007-04-16
// -----------------------------------------------------------------------------
void LogConsignMaintenance::Accept( ModelVisitor_ABC& visitor )
{
    visitor.Visit( *this );
}

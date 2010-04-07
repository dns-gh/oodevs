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
#include "protocol/ClientPublisher_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "Agent.h"
#include "protocol/clientsenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogConsignMedical constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
LogConsignMedical::LogConsignMedical( const Model& model, const MsgsSimToClient::MsgLogMedicalHandlingCreation& msg )
    : SimpleEntity< >   ( msg.oid_consigne() )
    , model_            ( model )
    , agent_            ( model.agents_.Get( msg.oid_pion() ) )
    , nTickCreation_    ( msg.tick_creation() )
    , pTreatingAgent_   ( 0 )
    , nRank_            ( msg.rang() )
    , nWound_           ( msg.blessure() )
    , bMentalDiseased_  ( msg.blesse_mental() != 0 )
    , bContaminated_    ( msg.contamine_nbc() != 0 )
    , nState_           ( Common::ambulance_ramassage_dechargement )
    , bDiagnosed_       ( false )
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
void LogConsignMedical::Update( const MsgsSimToClient::MsgLogMedicalHandlingUpdate& msg )
{
    if( msg.has_oid_pion_log_traitant() )
        pTreatingAgent_ = ( msg.oid_pion_log_traitant() == 0 ) ? 0 : &model_.agents_.Get( msg.oid_pion_log_traitant() );

    if( msg.has_blessure() )
        nWound_ = msg.blessure();

    if( msg.has_blesse_mental() )
        bMentalDiseased_ = msg.blesse_mental() != 0;

    if( msg.has_contamine_nbc() )
        bContaminated_ = msg.contamine_nbc() != 0;

    if( msg.has_etat() )
        nState_ = msg.etat();

    if( msg.has_diagnostique_effectue() )
        bDiagnosed_ = msg.diagnostique_effectue() != 0;
}

// -----------------------------------------------------------------------------
// Name: LogConsignMedical::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void LogConsignMedical::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::LogMedicalHandlingCreation asn;

    asn().set_oid_consigne( GetId() );
    asn().set_oid_pion( agent_.GetId() );
    asn().set_tick_creation( nTickCreation_ );
    asn().set_rang( nRank_ );
    asn().set_blessure( nWound_ );
    asn().set_blesse_mental( bMentalDiseased_ );
    asn().set_contamine_nbc( bContaminated_ );

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: LogConsignMedical::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void LogConsignMedical::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::LogMedicalHandlingUpdate asn;

    asn().set_oid_consigne ( GetId() );
    asn().set_oid_pion     ( agent_.GetId() );

//    asn.set_oid_pion_log_traitantPresent( 1 );
//    asn.set_blesse_mentalPresent( 1 );
//    asn.set_blessurePresent( 1 );
//    asn.set_contamine_nbcPresent( 1 );
//    asn.set_diagnostique_effectuePresent( 1 );
//    asn.set_etatPresent( 1 );

    asn().set_oid_pion_log_traitant ( pTreatingAgent_ ? pTreatingAgent_->GetId() : 0);
    asn().set_blesse_mental         ( bMentalDiseased_);
    asn().set_blessure              ( nWound_);
    asn().set_contamine_nbc         ( bContaminated_);
    asn().set_diagnostique_effectue ( bDiagnosed_);
    asn().set_etat                  ( nState_);
    
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: LogConsignMedical::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void LogConsignMedical::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::LogMedicalHandlingDestruction asn;
    asn().set_oid_consigne ( GetId());
    asn().set_oid_pion     ( agent_.GetId());
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

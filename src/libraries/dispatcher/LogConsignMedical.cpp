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
#include "ClientPublisher_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "Agent.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogConsignMedical constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
LogConsignMedical::LogConsignMedical( const Model& model, const ASN1T_MsgLogMedicalHandlingCreation& msg )
    : SimpleEntity< >   ( msg.oid_consigne )
    , model_            ( model )
    , agent_            ( model.agents_.Get( msg.oid_pion ) )
    , nTickCreation_    ( msg.tick_creation )
    , pTreatingAgent_   ( 0 )
    , nRank_            ( msg.rang )
    , nWound_           ( msg.blessure )
    , bMentalDiseased_  ( msg.blesse_mental != 0 )
    , bContaminated_    ( msg.contamine_nbc != 0 )
    , nState_           ( EnumLogMedicalHandlingStatus::ambulance_ramassage_dechargement )
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
void LogConsignMedical::Update( const ASN1T_MsgLogMedicalHandlingUpdate& msg )
{
    if( msg.m.oid_pion_log_traitantPresent )
        pTreatingAgent_ = ( msg.oid_pion_log_traitant == 0 ) ? 0 : &model_.agents_.Get( msg.oid_pion_log_traitant );

    if( msg.m.blessurePresent )
        nWound_ = msg.blessure;

    if( msg.m.blesse_mentalPresent )
        bMentalDiseased_ = msg.blesse_mental != 0;

    if( msg.m.contamine_nbcPresent )
        bContaminated_ = msg.contamine_nbc != 0;

    if( msg.m.etatPresent )
        nState_ = msg.etat;

    if( msg.m.diagnostique_effectuePresent )
        bDiagnosed_ = msg.diagnostique_effectue != 0;
}

// -----------------------------------------------------------------------------
// Name: LogConsignMedical::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void LogConsignMedical::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::LogMedicalHandlingCreation asn;

    asn().oid_consigne  = GetId();
    asn().oid_pion      = agent_.GetId();
    asn().tick_creation = nTickCreation_;
    asn().rang          = nRank_;
    asn().blessure      = nWound_;
    asn().blesse_mental = bMentalDiseased_;
    asn().contamine_nbc = bContaminated_;

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: LogConsignMedical::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void LogConsignMedical::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::LogMedicalHandlingUpdate asn;

    asn().oid_consigne = GetId();
    asn().oid_pion     = agent_.GetId();

    asn().m.oid_pion_log_traitantPresent = 1;
    asn().m.blesse_mentalPresent         = 1;
    asn().m.blessurePresent              = 1;
    asn().m.contamine_nbcPresent         = 1;
    asn().m.diagnostique_effectuePresent = 1;
    asn().m.etatPresent                  = 1;

    asn().oid_pion_log_traitant = pTreatingAgent_ ? pTreatingAgent_->GetId() : 0;
    asn().blesse_mental         = bMentalDiseased_;
    asn().blessure              = nWound_;
    asn().contamine_nbc         = bContaminated_;
    asn().diagnostique_effectue = bDiagnosed_;
    asn().etat                  = nState_;
    
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: LogConsignMedical::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void LogConsignMedical::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::LogMedicalHandlingDestruction asn;
    asn().oid_consigne = GetId();
    asn().oid_pion     = agent_.GetId();
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

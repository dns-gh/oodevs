// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "Automat.h"

#include "Side.h"
#include "Formation.h"
#include "Model.h"
#include "KnowledgeGroup.h"
#include "Network_Def.h"
#include "Agent.h"
#include "DotationQuota.h"
#include "ModelVisitor_ABC.h"
#include "AutomatOrder.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Automat::Automat( Model& model, const ASN1T_MsgAutomatCreation& msg )
    : model_            ( model )
    , nID_              ( msg.oid )
    , nType_            ( msg.type_automate )
    , strName_          ( msg.nom )
    , side_             ( model.GetSides     ().Get( msg.oid_camp ) )
    , formation_        ( model.GetFormations().Get( msg.oid_formation ) )
    , pKnowledgeGroup_  ( &model.GetKnowledgeGroups().Get( msg.oid_groupe_connaissance ) )
    , agents_           ()
    , quotas_           ()
    , pTC2_             ( 0 )
    , pLogMaintenance_  ( 0 )
    , pLogMedical_      ( 0 )
    , pLogSupply_       ( 0 )
    , nAutomatState_    ( EnumAutomatMode::debraye )
    , nForceRatioState_ ( EnumForceRatioStatus::neutre )
    , nCloseCombatState_( EnumMeetingEngagementStatus::etat_fixe )
    , nOperationalState_( EnumOperationalStatus::detruit_totalement )
    , nRoe_             ( EnumRoe::tir_interdit )
    , pOrder_           ( 0 )
{
    pKnowledgeGroup_->GetAutomats().Register( *this );
    formation_.GetAutomats().Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Automat destructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Automat::~Automat()
{
    formation_.GetAutomats().Unregister( *this );
    pKnowledgeGroup_->GetAutomats().Unregister( *this );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Automat::Update
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Automat::Update( const ASN1T_MsgAutomatCreation& msg )
{
    const bool kgChanged = pKnowledgeGroup_->GetID() != msg.oid_groupe_connaissance;
    FlagUpdate( kgChanged );
    if( kgChanged )
    {
        pKnowledgeGroup_->GetAutomats().Unregister( *this );
        pKnowledgeGroup_ = &model_.GetKnowledgeGroups().Get( msg.oid_groupe_connaissance );
        pKnowledgeGroup_->GetAutomats().Register( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: Automat::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Automat::Update( const ASN1T_MsgAutomatChangeLogisticLinksAck& msg )
{
    if( msg.m.oid_tc2Present )
        pTC2_ = msg.oid_tc2 == 0 ? 0 : &model_.GetAutomats().Get( msg.oid_tc2 );
    if( msg.m.oid_maintenancePresent )
        pLogMaintenance_ = msg.oid_maintenance == 0 ? 0 : &model_.GetAutomats().Get( msg.oid_maintenance );
    if( msg.m.oid_santePresent )
        pLogMedical_ = msg.oid_sante == 0 ? 0 : &model_.GetAutomats().Get( msg.oid_sante );
    if( msg.m.oid_ravitaillementPresent )
        pLogSupply_ = msg.oid_ravitaillement == 0 ? 0 : &model_.GetAutomats().Get( msg.oid_ravitaillement );
}

// -----------------------------------------------------------------------------
// Name: Automat::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Automat::Update( const ASN1T_MsgAutomatChangeLogisticLinks& msg )
{
    if( msg.m.oid_tc2Present )
        pTC2_ = msg.oid_tc2 == 0 ? 0 : &model_.GetAutomats().Get( msg.oid_tc2 );
    if( msg.m.oid_maintenancePresent )
        pLogMaintenance_ = msg.oid_maintenance == 0 ? 0 : &model_.GetAutomats().Get( msg.oid_maintenance );
    if( msg.m.oid_santePresent )
        pLogMedical_ = msg.oid_sante == 0 ? 0 : &model_.GetAutomats().Get( msg.oid_sante );
    if( msg.m.oid_ravitaillementPresent )
        pLogSupply_ = msg.oid_ravitaillement == 0 ? 0 : &model_.GetAutomats().Get( msg.oid_ravitaillement );
}

// -----------------------------------------------------------------------------
// Name: Automat::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Automat::Update( const ASN1T_MsgAutomatChangeKnowledgeGroupAck& msg )
{
    pKnowledgeGroup_->GetAutomats().Unregister( *this );
    pKnowledgeGroup_ = &model_.GetKnowledgeGroups().Get( msg.oid_groupe_connaissance );
    pKnowledgeGroup_->GetAutomats().Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Automat::Update
// Created: NLD 2006-10-05
// -----------------------------------------------------------------------------
void Automat::Update( const ASN1T_MsgAutomatAttributes& msg )
{
    if( msg.m.etat_automatePresent )
        nAutomatState_ = msg.etat_automate;
    if( msg.m.rapport_de_forcePresent )
        nForceRatioState_ = msg.rapport_de_force;
    if( msg.m.combat_de_rencontrePresent )
        nCloseCombatState_ = msg.combat_de_rencontre;
    if( msg.m.etat_operationnelPresent )
        nOperationalState_ = msg.etat_operationnel;
    if( msg.m.roePresent )
        nRoe_ = msg.roe;
}

// -----------------------------------------------------------------------------
// Name: Automat::Update
// Created: ZEBRE 2007-06-21
// -----------------------------------------------------------------------------
void Automat::Update( const ASN1T_MsgDecisionalState& asnMsg )
{
    decisionalInfos_.Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Automat::Update
// Created: NLD 2007-03-29
// -----------------------------------------------------------------------------
void Automat::Update( const ASN1T_MsgLogSupplyQuotas& msg )
{
    quotas_.Clear();
    for( unsigned i = 0; i < msg.quotas.n; ++i )
        quotas_.Create( model_, msg.quotas.elem[ i ].ressource_id, msg.quotas.elem[ i ] );
}

// -----------------------------------------------------------------------------
// Name: Automat::Update
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void Automat::Update( const ASN1T_MsgAutomatOrder& asnMsg )
{
    delete pOrder_;
    pOrder_ = 0;
    if( asnMsg.mission != 0 )
        pOrder_ = new AutomatOrder( model_, *this, asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Automat::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Automat::SendCreation( Publisher_ABC& publisher ) const
{
    AsnMsgSimToClientAutomatCreation asn;
    asn().oid                     = nID_;
    asn().type_automate           = nType_;
    asn().nom                     = strName_.c_str(); // !! pointeur sur const char*
    asn().oid_camp                = side_.GetID();
    asn().oid_groupe_connaissance = pKnowledgeGroup_->GetID();
    asn().oid_formation           = formation_.GetID();
    asn.Send( publisher );

    AsnMsgSimToClientLogSupplyQuotas asnQuotas;
    asnQuotas().oid_automate = nID_;
    quotas_.Send< ASN1T__SeqOfDotationQuota, ASN1T_DotationQuota >( asnQuotas().quotas );
    asnQuotas.Send( publisher );
    if( asnQuotas().quotas.n > 0 )
        delete [] asnQuotas().quotas.elem;
}

// -----------------------------------------------------------------------------
// Name: Automat::SendFullUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Automat::SendFullUpdate( Publisher_ABC& publisher ) const
{
    {
        AsnMsgSimToClientAutomatAttributes asn;
        asn().m.etat_automatePresent = 1;
        asn().m.rapport_de_forcePresent = 1;
        asn().m.combat_de_rencontrePresent = 1;
        asn().m.etat_operationnelPresent = 1;
        asn().m.roePresent = 1;
        asn().oid = nID_;
        asn().etat_automate = nAutomatState_;
        asn().rapport_de_force = nForceRatioState_;
        asn().combat_de_rencontre = nCloseCombatState_;
        asn().etat_operationnel = nOperationalState_;
        asn().roe = nRoe_;
        asn.Send( publisher );
    }

    {
        AsnMsgSimToClientAutomatChangeLogisticLinks asn;
        asn().oid  = nID_;
        if( pTC2_ )
        {
            asn().m.oid_tc2Present = 1;
            asn().oid_tc2 = pTC2_->GetID();
        }
        if( pLogMaintenance_ )
        {
            asn().m.oid_maintenancePresent = 1;
            asn().oid_maintenance = pLogMaintenance_->GetID();
        }
        if( pLogMedical_ )
        {
            asn().m.oid_santePresent = 1;
            asn().oid_sante = pLogMedical_->GetID();
        }
        if( pLogSupply_ )
        {
            asn().m.oid_ravitaillementPresent = 1;
            asn().oid_ravitaillement = pLogSupply_->GetID();
        }
        asn.Send( publisher );
    }
    if( pOrder_ )
        pOrder_->Send( publisher );
    else
        AutomatOrder::SendNoMission( *this, publisher );

    decisionalInfos_.Send( nID_, publisher );
}

// -----------------------------------------------------------------------------
// Name: Automat::Accept
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Automat::Accept( ModelVisitor_ABC& visitor )
{
    visitor.Visit( *this );
    agents_.Apply( std::mem_fun_ref( &Agent::Accept ), visitor );
}

// -----------------------------------------------------------------------------
// Name: Automat::SendSpecialUpdate
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void Automat::SendSpecialUpdate( Publisher_ABC& publisher ) const
{
    AsnMsgSimToClientAutomatChangeKnowledgeGroupAck ack;
    ack().error_code = EnumChangeHierarchyErrorCode::no_error;
    ack().oid = nID_;
    ack().oid_camp = side_.GetID();
    ack().oid_groupe_connaissance = pKnowledgeGroup_->GetID();
    ack.Send( publisher );
}

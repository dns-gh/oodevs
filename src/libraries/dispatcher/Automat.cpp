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

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Automat::Automat( Model& model, const ASN1T_MsgAutomateCreation& msg )
    : model_            ( model )
    , nID_              ( msg.oid_automate )
    , nType_            ( msg.type_automate )
    , strName_          ( msg.nom )
    , side_             ( model.GetSides     ().Get( msg.oid_camp ) )
    , formation_        ( model.GetFormations().Get( msg.oid_formation ) )
    , pKnowledgeGroup_  ( &model.GetKnowledgeGroups().Get( msg.oid_groupe_connaissance ) )
    , agents_           ()
    , pTC2_             ( 0 )
    , pLogMaintenance_  ( 0 )
    , pLogMedical_      ( 0 )
    , pLogSupply_       ( 0 )
    , nAutomatState_    ( EnumAutomateState::debraye )
    , nForceRatioState_ ( EnumEtatRapFor::neutre )
    , nCloseCombatState_( EnumEtatCombatRencontre::etat_fixe )
    , nOperationalState_( EnumEtatOperationnel::detruit_totalement )
    , nRoe_             ( EnumRoe::tir_interdit )
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
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Automat::Update( const ASN1T_MsgChangeLiensLogistiquesAck& msg )
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
void Automat::Update( const ASN1T_MsgChangeLiensLogistiques& msg )
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
void Automat::Update( const ASN1T_MsgChangeGroupeConnaissanceAck& msg )
{
    pKnowledgeGroup_->GetAutomats().Unregister( *this );
    pKnowledgeGroup_ = &model_.GetKnowledgeGroups().Get( msg.oid_groupe_connaissance );
    pKnowledgeGroup_->GetAutomats().Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Automat::Update
// Created: NLD 2006-10-05
// -----------------------------------------------------------------------------
void Automat::Update( const ASN1T_MsgAutomateAttributes& msg )
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
// Name: Automat::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Automat::SendCreation( Publisher_ABC& publisher ) const
{
    AsnMsgInClientAutomateCreation asn;
    asn().oid_automate            = nID_;
    asn().type_automate           = nType_;
    asn().nom                     = strName_.c_str(); // !! pointeur sur const char*
    asn().oid_camp                = side_.GetID();
    asn().oid_groupe_connaissance = pKnowledgeGroup_->GetID();
    asn().oid_formation           = formation_.GetID();
    asn.Send( publisher );

	agents_.Apply( std::mem_fun_ref( &Agent::SendCreation ), publisher );
}

// -----------------------------------------------------------------------------
// Name: Automat::SendFullUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Automat::SendFullUpdate( Publisher_ABC& publisher ) const
{
    {
        AsnMsgInClientAutomateAttributes asn;
        asn().m.etat_automatePresent = 1;
        asn().m.rapport_de_forcePresent = 1;
        asn().m.combat_de_rencontrePresent = 1;
        asn().m.etat_operationnelPresent = 1;
        asn().m.roePresent = 1;
        asn().oid_automate = nID_;
        asn().etat_automate = nAutomatState_;
        asn().rapport_de_force = nForceRatioState_;
        asn().combat_de_rencontre = nCloseCombatState_;
        asn().etat_operationnel = nOperationalState_;
        asn().roe = nRoe_;
        asn.Send( publisher );
    }

    {
        AsnMsgInClientChangeLiensLogistiques asn;
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

	agents_.Apply( std::mem_fun_ref( &Agent::SendFullUpdate ), publisher );
}


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
#include "Model.h"
#include "KnowledgeGroup.h"
#include "Network_Def.h"

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
    , side_             ( model.GetSides          ().Get( msg.oid_camp ) )
    , pKnowledgeGroup_  ( &model.GetKnowledgeGroups().Get( msg.oid_groupe_connaissance ) )
    , agents_           ()
    , nTC2_             ( msg.m.oid_tc2Present ? msg.oid_tc2 : 0 )
    , nLogMaintenance_  ( msg.m.oid_maintenancePresent ? msg.oid_maintenance : 0 )
    , nLogMedical_      ( msg.m.oid_santePresent ? msg.oid_sante : 0 )
    , nLogSupply_       ( msg.m.oid_ravitaillementPresent ? msg.oid_ravitaillement : 0 )
    , nAutomatState_    ( EnumAutomateState::debraye )
    , nForceRatioState_ ( EnumEtatRapFor::neutre )
    , nCloseCombatState_( EnumEtatCombatRencontre::etat_fixe )
    , nOperationalState_( EnumEtatOperationnel::detruit_totalement )
    , nRoe_             ( EnumRoe::tir_interdit )
{
    pKnowledgeGroup_->GetAutomats().Register( *this );
    side_            .GetAutomats().Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Automat destructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Automat::~Automat()
{
    pKnowledgeGroup_->GetAutomats().Unregister( *this );
    side_            .GetAutomats().Unregister( *this );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Automat::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Automat::Update( const ASN1T_MsgChangeLiensLogistiquesAck& asnMsg )
{
    if( asnMsg.m.oid_tc2Present )
        nTC2_ = asnMsg.oid_tc2;
    if( asnMsg.m.oid_maintenancePresent )
        nLogMaintenance_ = asnMsg.oid_maintenance;
    if( asnMsg.m.oid_santePresent )
        nLogMedical_ = asnMsg.oid_sante;
    if( asnMsg.m.oid_ravitaillementPresent )
        nLogSupply_ = asnMsg.oid_ravitaillement;
}

// -----------------------------------------------------------------------------
// Name: Automat::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Automat::Update( const ASN1T_MsgChangeGroupeConnaissanceAck& asnMsg )
{
    pKnowledgeGroup_->GetAutomats().Unregister( *this );
    pKnowledgeGroup_ = &model_.GetKnowledgeGroups().Get( asnMsg.oid_groupe_connaissance );
    pKnowledgeGroup_->GetAutomats().Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Automat::Update
// Created: NLD 2006-10-05
// -----------------------------------------------------------------------------
void Automat::Update( const ASN1T_MsgAutomateAttributes& asnMsg )
{
    if( asnMsg.m.etat_automatePresent )
        nAutomatState_ = asnMsg.etat_automate;
    if( asnMsg.m.rapport_de_forcePresent )
        nForceRatioState_ = asnMsg.rapport_de_force;
    if( asnMsg.m.combat_de_rencontrePresent )
        nCloseCombatState_ = asnMsg.combat_de_rencontre;
    if( asnMsg.m.etat_operationnelPresent )
        nOperationalState_ = asnMsg.etat_operationnel;
    if( asnMsg.m.roePresent )
        nRoe_ = asnMsg.roe;
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

    if( nTC2_ != 0 )
    {
        asn().m.oid_tc2Present = 1;
        asn().oid_tc2 = nTC2_;
    }
    if( nLogMaintenance_ != 0 )
    {
        asn().m.oid_maintenancePresent = 1;
        asn().oid_maintenance = nLogMaintenance_;
    }
    if( nLogMedical_ != 0 )
    {
        asn().m.oid_santePresent = 1;
        asn().oid_sante = nLogMedical_;
    }
    if( nLogSupply_ != 0 )
    {
        asn().m.oid_ravitaillementPresent = 1;
        asn().oid_ravitaillement = nLogSupply_;
    }

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Automat::SendFullUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Automat::SendFullUpdate( Publisher_ABC& publisher ) const
{
    AsnMsgInClientAutomateAttributes asn;

    asn().m.etat_automatePresent = 1;
    asn().m.rapport_de_forcePresent = 1;
    asn().m.combat_de_rencontrePresent = 1;
    asn().m.etat_operationnelPresent = 1;
    asn().m.roePresent = 1;

    asn().etat_automate = nAutomatState_;
    asn().rapport_de_force = nForceRatioState_;
    asn().combat_de_rencontre = nCloseCombatState_;
    asn().etat_operationnel = nOperationalState_;
    asn().roe = nRoe_;

    asn.Send( publisher );
}


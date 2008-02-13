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
#include "AgentTypes.h"
#include "Report.h"
#include "tools/App6Symbol.h"

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
    , pParentFormation_ ( msg.oid_parent.t == T_MsgAutomatCreation_oid_parent_formation ? &model.GetFormations().Get( msg.oid_parent.u.formation ) : 0 )
    , pParentAutomat_   ( msg.oid_parent.t == T_MsgAutomatCreation_oid_parent_automate  ? &model.GetAutomats  ().Get( msg.oid_parent.u.automate  ) : 0 )
    , pKnowledgeGroup_  ( &model.GetKnowledgeGroups().Get( msg.oid_groupe_connaissance ) )
    , agents_           ()
    , automats_         ()
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
    assert( pParentFormation_ || pParentAutomat_ );

    pKnowledgeGroup_->GetAutomats().Register( *this );
    if( pParentFormation_ )
        pParentFormation_->GetAutomats().Register( *this );
    else if( pParentAutomat_ )
        pParentAutomat_->GetAutomats().Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Automat destructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Automat::~Automat()
{
    if( pParentFormation_ )
        pParentFormation_->GetAutomats().Unregister( *this );
    else if( pParentAutomat_ )
        pParentAutomat_->GetAutomats().Unregister( *this );

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
    FlagUpdate();
    if( pKnowledgeGroup_->GetID() != msg.oid_groupe_connaissance )
    {
        pKnowledgeGroup_->GetAutomats().Unregister( *this );
        pKnowledgeGroup_ = &model_.GetKnowledgeGroups().Get( msg.oid_groupe_connaissance );
        pKnowledgeGroup_->GetAutomats().Register( *this );
    }
    if(   pParentFormation_ && ( msg.oid_parent.t == T_MsgAutomatCreation_oid_parent_automate  || ( msg.oid_parent.t == T_MsgAutomatCreation_oid_parent_formation && msg.oid_parent.u.formation != pParentFormation_->GetID() ) )
       || pParentAutomat_   && ( msg.oid_parent.t == T_MsgAutomatCreation_oid_parent_formation || ( msg.oid_parent.t == T_MsgAutomatCreation_oid_parent_automate  && msg.oid_parent.u.automate  != pParentAutomat_  ->GetID() ) )
       )
    {
        if( pParentFormation_ )
            pParentFormation_->GetAutomats().Unregister( *this );
        if( pParentAutomat_ )
            pParentAutomat_->GetAutomats().Unregister( *this );
        pParentFormation_ = 0;
        pParentAutomat_   = 0;
        if( msg.oid_parent.t == T_MsgAutomatCreation_oid_parent_formation )
        {
            pParentFormation_ = &model_.GetFormations().Get( msg.oid_parent.u.formation );
            pParentFormation_->GetAutomats().Register( *this );
        }
        else if( msg.oid_parent.t == T_MsgAutomatCreation_oid_parent_automate )
        {
            pParentAutomat_ = &model_.GetAutomats().Get( msg.oid_parent.u.automate );
            pParentAutomat_->GetAutomats().Register( *this );
        }        
        
    }
    decisionalInfos_.Clear();
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
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
void Automat::Update( const ASN1T_MsgAutomatChangeSuperior& msg )
{
    if( pParentFormation_ )
        pParentFormation_->GetAutomats().Unregister( *this );
    if( pParentAutomat_ )
        pParentAutomat_->GetAutomats().Unregister( *this );
    pParentFormation_ = 0;
    pParentAutomat_   = 0;
    if( msg.oid_superior.t == T_MsgAutomatChangeSuperior_oid_superior_formation )
    {
        pParentFormation_ = &model_.GetFormations().Get( msg.oid_superior.u.formation );
        pParentFormation_->GetAutomats().Register( *this );
    }
    else if( msg.oid_superior.t == T_MsgAutomatChangeSuperior_oid_superior_automate )
    {
        pParentAutomat_ = &model_.GetAutomats().Get( msg.oid_superior.u.automate );
        pParentAutomat_->GetAutomats().Register( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: Automat::Update
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
void Automat::Update( const ASN1T_MsgAutomatChangeKnowledgeGroup& msg )
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
void Automat::SendCreation( ClientPublisher_ABC& publisher ) const
{
    AsnMsgSimToClientAutomatCreation asn;
    asn().oid                     = nID_;
    asn().type_automate           = nType_;
    asn().nom                     = strName_.c_str(); // !! pointeur sur const char*
    asn().oid_camp                = side_.GetID();
    asn().oid_groupe_connaissance = pKnowledgeGroup_->GetID();

    if( pParentFormation_ )
    {
        asn().oid_parent.t           = T_MsgAutomatCreation_oid_parent_formation;
        asn().oid_parent.u.formation = pParentFormation_->GetID();
    }
    else if( pParentAutomat_ )
    {
        asn().oid_parent.t          = T_MsgAutomatCreation_oid_parent_automate;
        asn().oid_parent.u.automate = pParentAutomat_->GetID();
    }
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
void Automat::SendFullUpdate( ClientPublisher_ABC& publisher ) const
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
        AsnMsgSimToClientAutomatChangeKnowledgeGroup asn;
        asn().oid = nID_;
        asn().oid_camp = side_.GetID();
        asn().oid_groupe_connaissance = pKnowledgeGroup_->GetID();
        asn.Send( publisher );

    }
    {
        AsnMsgSimToClientAutomatChangeSuperior asn;
        asn().oid = nID_;
        if( pParentFormation_ )
        {
            asn().oid_superior.t           = T_MsgAutomatCreation_oid_parent_formation;
            asn().oid_superior.u.formation = pParentFormation_->GetID();
        }
        else if( pParentAutomat_ )
        {
            asn().oid_superior.t          = T_MsgAutomatCreation_oid_parent_automate;
            asn().oid_superior.u.automate = pParentAutomat_->GetID();
        }
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
    automats_.Apply( std::mem_fun_ref( &Automat::Accept ), visitor );
    agents_  .Apply( std::mem_fun_ref( &Agent  ::Accept ), visitor );
}

namespace
{
    struct SymbolAggregator
    {
        SymbolAggregator() : level_( 0 ) {}
        void operator()( Agent& agent )
        {
            const std::string agentSymbol = agent.BuildSymbol( false );
            level_ = std::max( level_, tools::app6::GetLevel( agentSymbol ) );
            symbol_ = tools::app6::FilterSymbol( agentSymbol, symbol_ );
        }
        std::string symbol_;
        unsigned int level_;
    };
}

// -----------------------------------------------------------------------------
// Name: Automat::BuildSymbol
// Created: SBO 2007-08-22
// -----------------------------------------------------------------------------
std::string Automat::BuildSymbol( bool up /*= true*/ ) const
{
    SymbolAggregator aggregator;
    agents_.Apply< SymbolAggregator& >( aggregator );
    if( up )
        aggregator.symbol_ = tools::app6::MergeSymbol( pParentFormation_ ? pParentFormation_->BuildSymbol() : pParentAutomat_->BuildSymbol(), aggregator.symbol_ );
    tools::app6::SetLevel( aggregator.symbol_, aggregator.level_ + 1 );
    return aggregator.symbol_;
}
